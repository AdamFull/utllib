#pragma once

#include <cstdint>
#include <functional>
#include <tuple>
#include <variant>
#include <optional>
#include <string>
#include <string_view>
#include <chrono>
#include <type_traits>
#include <list>
#include <queue>
#include <unordered_set>
#include <memory>

#include "json.hpp"

#if (201711 <= __cpp_impl_three_way_comparison) && __has_include(<compare> )
#define HAS_SPACESHIP_OPERATOR
#endif
#if defined(HAS_SPACESHIP_OPERATOR)
#include <compare>
#endif

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

namespace utl
{
	template<class _Ty, class _KTy>
	inline constexpr _Ty cast(_KTy object)
	{
		return static_cast<_Ty>(object);
	}

    template<class _Ty, class _ReturnType = typename std::underlying_type<_Ty>::type>
    inline constexpr _ReturnType enum_cast(_Ty enum_value)
    {
        return cast<_ReturnType>(enum_value);
    }

    constexpr inline const u32 packUint16x2(u16 lhs, u16 rhs)
    {
        return (rhs << 16) | (lhs & 0xFFFF);
    }

    inline const void unpackUint16x2(u32 packed, u16& lhs, u16& rhs)
    {
        rhs = cast<u16>(packed >> 16);
        lhs = cast<u16>(packed & 0xFFFF);
    }

    // ----------------------BEGIN UFUNCTION----------------------
    /**
     * @brief Implementation of a simple delegate based on the std::function functionality from
     * the c++ standard library. Allows you to implement a functional object of a class
     * method or a static function for further invocation.
     *
     * @tparam _signature signature of the delegate function
     */
    template <class _signature>
    class function
    {
        using base_t = std::function<_signature>;

    public:
        function() = default;

        /**
         * @brief Construct a new delegate object with lambda function or static function.
         *
         * @tparam _LabbdaFunction
         * @param lfunc
         */
        template <class _LabbdaFunction>
        function(_LabbdaFunction&& lfunc)
        {
            attach(std::forward<_LabbdaFunction>(lfunc));
        }

        /**
         * @brief Construct a new delegate object for class or const class method. Receiving class pointer and class method reference.
         *
         * @param c Class pointer
         * @param m Method reference
         *
         */
        template <class _Class, class _ReturnType, class... _Types>
        function(_Class* c, _ReturnType(_Class::* m)(_Types...)) noexcept
        {
            attach(c, m);
        }

        /**
         * @brief Default copy constructor
         *
         * @param rDelegate
         */
        function(const function<_signature>& rDelegate) = default;

        /**
         * @brief Default copy assignment operator
         *
         * @param lDelegate
         * @return function<_signature>&
         */
        function<_signature>& operator=(const function<_signature>& lDelegate) = default;

        /**
         * @brief Default move constructor
         *
         * @param rDelegate
         */
        function(function<_signature>&& rDelegate) = default;

        /**
         * @brief Default move assignment constructor
         *
         * @param rDelegate
         * @return function<_signature>&
         */
        function<_signature>& operator=(function<_signature>&& rDelegate) = default;

        /**
         * @brief The method is intended for binding a lambda function or function to a delegate.
         *
         * @tparam LabbdaFunction lambda function type transited with template parameter.
         * @param lfunc
         */
        template <class _LabbdaFunction>
        inline void attach(_LabbdaFunction&& lfunc) noexcept
        {
            m_pFunction = std::forward<_LabbdaFunction>(lfunc);
        }

        /**
         * @brief For class or const class method. Receiving class pointer and class method reference.
         *
         * @param c Class pointer
         * @param m Method reference
         *
         */
        template <class _Class, class _ReturnType, class... _Types>
        inline void attach(_Class* c, _ReturnType(_Class::* m)(_Types...)) noexcept
        {
            m_pFunction = std::move(make_delegate(c, m));
        }

        operator bool() const
        {
            return m_pFunction != nullptr;
        }

        /**
         * @brief Detaching function delegate
         *
         */
        inline void detach() noexcept
        {
            m_pFunction = nullptr;
        }

        /**
         * @brief Redefining the parenthesis operator for convenient delegate invocation
         *
         * @tparam _Types Templated std::tuple arguments
         * @param args Delegate arguments
         * @return auto
         */
        template <class... _Types>
        inline auto operator()(_Types &&...args)
        {
            return m_pFunction(std::forward<_Types>(args)...);
        }

    private:
        std::function<_signature> m_pFunction{ nullptr };

        /**
         * @brief
         *
         * Implementation of creating a delegate for a static method.
         *
         * @tparam ReturnType (set automatically in c++17) class type
         * @tparam _Types (set automatically in c++17) another arguments
         * @param m reference to function
         * @return std::function<ReturnType(_Types...)>
         */
        template <class _ReturnType, class... _Types>
        inline std::function<_ReturnType(_Types...)> make_delegate(_ReturnType(*m)(_Types...)) noexcept
        {
            return [=](_Types &&...args)
                { return (*m)(std::forward<_Types>(args)...); };
        }

        /**
         * @brief
         *
         * Implementation of creating a delegate for a class.
         *
         * @tparam Class (set automatically in c++17) class type
         * @tparam ReturnType (set automatically in c++17)
         * @tparam _Types (set automatically in c++17)
         * @param c pointer to class
         * @param m reference to class method
         * @return std::function<ReturnType(_Types...)>
         */
        template <class _Class, class _ReturnType, class... _Types>
        inline std::function<_ReturnType(_Types...)> make_delegate(_Class* c, _ReturnType(_Class::* m)(_Types...)) noexcept
        {
            return [=](_Types &&...args)
                { return (c->*m)(std::forward<_Types>(args)...); };
        }

        /**
         * @brief
         *
         * Implementation of creating a delegate for a constant class.
         *
         * @tparam Class (set automatically in c++17)
         * @tparam ReturnType (set automatically in c++17)
         * @tparam _Types (set automatically in c++17)
         * @param c const pointer to class
         * @param m reference to class method
         * @return std::function<ReturnType(_Types...)>
         */
        template <class _Class, class _ReturnType, class... _Types>
        inline std::function<_ReturnType(_Types...)> make_delegate(const _Class* c, _ReturnType(_Class::* m)(_Types...) const) noexcept
        {
            return [=](_Types &&...args)
                { return (c->*m)(std::forward<_Types>(args)...); };
        }
    };
    // ----------------------END UFUNCTION----------------------

    // ----------------------BEGIN REACTIVE VARIABLE----------------------
    using avaliable_types_t = std::tuple<i8, u8, i16, u16, i32, u32, i64, u64, bool, f32, f64, std::string>;

    template <class T>
    struct variable
    {
        static_assert(!std::is_same<T, avaliable_types_t>::value,
            "Placed type is not avaliable. variable<T> support only types: int8_t, uint8_t, int16_t, uint16_t, i32, u32, int64_t, uint64_t, bool, f32, f64.");

    public:
        variable() = default;

        variable(const T& val) { value = val; }

        template <class... _Types>
        variable(_Types... args) { bind(std::forward<_Types>(args)...); }

        ~variable() { notify = nullptr; }

        inline const std::optional<T>& get() const { return value; }

        template <class... _Types>
        void bind(_Types... args)
        {
            notify = std::move(function<void(const T&, const T&)>(std::forward<_Types>(args)...));
        }

        inline T& operator=(const T& val) noexcept
        {
            if (notify && !ncompare(last_changed, val))
            {
                notify(last_changed, val);
                last_changed = value.value();
                value = val;
            }
            return last_changed;
        }

        inline T& operator+=(const T& val)
        {
            return operator=(val + value.value());
        }

    private:
        template <typename U = T>
        typename std::enable_if<std::is_floating_point<U>::value, bool>::type
            ncompare(const T& first, const T& second)
        {
            return std::fabs(first - second) <= std::numeric_limits<T>::epsilon();
        }

        template <typename U = T>
        typename std::enable_if<!std::is_floating_point<U>::value, bool>::type
            ncompare(const T& first, const T& second)
        {
            return first == second;
        }

        T last_changed{ NULL };
        std::optional<T> value{ T() };
        function<void(const T&, const T&)> notify{ nullptr };
    };

    using react_i8 = variable<i8>;
    using react_u8 = variable<u8>;
    using react_i16 = variable<i16>;
    using react_u16 = variable<u16>;
    using react_i32 = variable<i32>;
    using react_u32 = variable<u32>;
    using react_i64 = variable<i64>;
    using react_u64 = variable<u64>;

    using react_bool = variable<bool>;

    using react_f32 = variable<f32>;
    using react_f64 = variable<f64>;

    using react_string = variable<std::string>;

    using variants = std::variant<react_i8, react_u8, react_i16, react_u16, react_i32, react_u32, react_i64, react_u64, react_bool, react_f32, react_f64, react_string>;

    template <class T, class _variant>
    struct is_variant_type;

    template <class T, class... _Types>
    struct is_variant_type<T, std::variant<_Types...>>
        : public std::disjunction<std::is_same<T, _Types>...>
    {
    };

    template <class T>
    using is_react_value = is_variant_type<T, variants>;
    // ----------------------END REACTIVE VARIABLE----------------------

    class stopwatch
    {
        using timepoint_t = std::chrono::steady_clock::time_point;
    public:
        stopwatch() noexcept
        {
            sp = std::chrono::high_resolution_clock::now();
        }

        template<class _Ty>
        _Ty stop()
        {
            auto now = std::chrono::high_resolution_clock::now();
            auto secs = std::chrono::duration<_Ty>(now - sp).count();
            sp = now;
            return secs;
        }
    private:
        timepoint_t sp;
    };

    template<class _Ty>
    class unique_queue
    {
    public:
        void push(const _Ty& value)
        {
            if (_unique_values.insert(value).second)
                _q.push(value);
        }

        void pop() {
            if (!_q.empty())
            {
                _unique_values.erase(_q.front());
                _q.pop();
            }
        }

        const _Ty& front() const
        {
            return _q.front();
        }

        const _Ty& back() const
        {
            return _q.back();
        }

        bool empty() const
        {
            return _q.empty();
        }

        u64 size() const
        {
            return _q.size();
        }
    private:
        std::queue<_Ty> _q;
        std::unordered_set<_Ty> _unique_values;
    };

    // ----------------------BEGIN REFLECTION/HASH----------------------
    template <typename T> constexpr std::string_view type_name();

    template <>
    constexpr std::string_view type_name<void>() { return "void"; }

    namespace detail
    {
        using type_name_prober = void;

        template <typename T>
        constexpr inline std::string_view wrapped_type_name() noexcept
        {
#ifdef __clang__
            return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
            return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
            return __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
        }

        constexpr inline u64 wrapped_type_name_prefix_length() noexcept {
            return wrapped_type_name<type_name_prober>().find(type_name<type_name_prober>());
        }

        constexpr inline u64 wrapped_type_name_suffix_length() noexcept {
            return wrapped_type_name<type_name_prober>().length()
                - wrapped_type_name_prefix_length()
                - type_name<type_name_prober>().length();
        }

    } // namespace detail

    template <class _Ty>
    constexpr inline std::string_view type_name()
    {
        constexpr auto wrapped_name = detail::wrapped_type_name<_Ty>();
        constexpr auto prefix_length = detail::wrapped_type_name_prefix_length();
        constexpr auto suffix_length = detail::wrapped_type_name_suffix_length();
        constexpr auto type_name_length = wrapped_name.length() - prefix_length - suffix_length;
        return wrapped_name.substr(prefix_length, type_name_length);
    }

    constexpr uint64_t fnv_prime = 1099511628211ull;
    constexpr uint64_t fnv_offset_basis = 14695981039346656037ull;

    constexpr inline uint64_t fnv1a_64_hash(const char* str, uint64_t hash = fnv_offset_basis) noexcept
    {
        while (*str != '\0')
        {
            hash = (hash ^ cast<uint64_t>(*str)) * fnv_prime;
            str++;
        }
        return hash;
    }

    constexpr inline uint32_t const_hash(char const* input) noexcept
    {
        return *input ? cast<uint32_t>(*input) + 33 * const_hash(input + 1) : 5381u;
    }

    constexpr inline uint32_t const_hash(char const* s, uint64_t count) noexcept
    {
        return ((count ? const_hash(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
    }

    constexpr inline uint32_t operator "" _utl_hash(char const* s, uint64_t count) noexcept
    {
        return const_hash(s, count);
    }

    template<class _Ty>
    constexpr inline uint32_t type_hash() noexcept
    {
        constexpr auto typestr = type_name<_Ty>();
        return const_hash(typestr.data());
    }
    // ----------------------END REFLECTION/HASH----------------------

    // ----------------------BEGIN ENUM FLAGS----------------------
    template <typename FlagBitsType>
    struct FlagTraits
    {
    };

    template <typename BitType>
    class Flags
    {
    public:
        using MaskType = typename std::underlying_type<BitType>::type;

        // constructors
        constexpr Flags() noexcept : m_mask(0) {}

        constexpr Flags(BitType bit) noexcept : m_mask(cast<MaskType>(bit)) {}

        constexpr Flags(Flags<BitType> const& rhs) noexcept = default;

        constexpr Flags(MaskType Flags) noexcept : m_mask(Flags) {}

        // relational operators
#if defined(HAS_SPACESHIP_OPERATOR)
        auto operator<=>(Flags<BitType> const&) const = default;
#else
        constexpr bool operator<(Flags<BitType> const& rhs) const noexcept
        {
            return m_mask < rhs.m_mask;
        }

        constexpr bool operator<=(Flags<BitType> const& rhs) const noexcept
        {
            return m_mask <= rhs.m_mask;
        }

        constexpr bool operator>(Flags<BitType> const& rhs) const noexcept
        {
            return m_mask > rhs.m_mask;
        }

        constexpr bool operator>=(Flags<BitType> const& rhs) const noexcept
        {
            return m_mask >= rhs.m_mask;
        }

        constexpr bool operator==(Flags<BitType> const& rhs) const noexcept
        {
            return m_mask == rhs.m_mask;
        }

        constexpr bool operator!=(Flags<BitType> const& rhs) const noexcept
        {
            return m_mask != rhs.m_mask;
        }
#endif

        // logical operator
        constexpr bool operator!() const noexcept
        {
            return !m_mask;
        }

        // bitwise operators
        constexpr Flags<BitType> operator&(Flags<BitType> const& rhs) const noexcept
        {
            return Flags<BitType>(m_mask & rhs.m_mask);
        }

        constexpr Flags<BitType> operator|(Flags<BitType> const& rhs) const noexcept
        {
            return Flags<BitType>(m_mask | rhs.m_mask);
        }

        constexpr Flags<BitType> operator^(Flags<BitType> const& rhs) const noexcept
        {
            return Flags<BitType>(m_mask ^ rhs.m_mask);
        }

        constexpr Flags<BitType> operator~() const noexcept
        {
            return Flags<BitType>(m_mask ^ FlagTraits<BitType>::allflags);
        }

        // assignment operators
        constexpr Flags<BitType>& operator=(Flags<BitType> const& rhs) noexcept = default;

        constexpr Flags<BitType>& operator|=(Flags<BitType> const& rhs) noexcept
        {
            m_mask |= rhs.m_mask;
            return *this;
        }

        constexpr Flags<BitType>& operator&=(Flags<BitType> const& rhs) noexcept
        {
            m_mask &= rhs.m_mask;
            return *this;
        }

        constexpr Flags<BitType>& operator^=(Flags<BitType> const& rhs) noexcept
        {
            m_mask ^= rhs.m_mask;
            return *this;
        }

        // cast operators
        explicit constexpr operator bool() const noexcept
        {
            return !!m_mask;
        }

        explicit constexpr operator MaskType() const noexcept
        {
            return m_mask;
        }

    private:
        MaskType m_mask;
    };
    // ----------------------END ENUM FLAGS----------------------

    // ----------------------BEGIN PATTERN----------------------
    class non_copyable
    {
    protected:
        non_copyable() = default;
        virtual ~non_copyable() = default;

    public:
        non_copyable(const non_copyable&) = delete;
        non_copyable& operator=(const non_copyable&) = delete;
    };

    class non_movable
    {
    protected:
        non_movable() = default;
        virtual ~non_movable() = default;

    public:
        non_movable(non_movable&&) noexcept = delete;
        non_movable& operator=(non_movable&&) noexcept = delete;
    };

    class non_copy_movable : public non_copyable, public non_movable
    {
    protected:
        non_copy_movable() = default;
        virtual ~non_copy_movable() = default;
    };

    template <class _Ty>
    class singleton : public non_copy_movable
    {
    public:
        static inline const std::unique_ptr<_Ty>& getInstance()
        {
            static std::unique_ptr<_Ty> _instance;
            if (!_instance)
                _instance = std::make_unique<_Ty>();
            return _instance;
        }
    };

    template<class _Ty>
    class base_polymorphus
    {
    public:
        base_polymorphus() = default;
        virtual ~base_polymorphus() = default;

        template<class _Kty>
        constexpr _Kty* as() noexcept
        {
            if (is_same<_Kty>())
                return cast<_Kty*>(this);
            return nullptr;
        }

        template<class _Kty>
        constexpr bool is_same() noexcept
        {
            return delivered == _Kty::self_hash;
        }

    protected:
        u64 delivered{ 0 };
    };

    template<class _Ty, class _Kty>
    class derived_polymorphus : public _Ty
    {
    public:
        constexpr derived_polymorphus() noexcept { _Ty::delivered = self_hash; }
        virtual ~derived_polymorphus() = default;
        static constexpr const u64 self_hash{ type_hash<_Kty>() };
    };

    template<class _Ty>
    class observer
    {
    public:
        virtual ~observer() = default;
        virtual void notification(const _Ty&) = 0;
    };

    template<class _Ty, class _Kty>
    class subject
    {
    public:
        virtual ~subject() = default;

        virtual void attach(_Ty* _ptr) { vObservers.emplace_back(_ptr); }

        virtual void detach(_Ty* _ptr) { vObservers.remove(_ptr); }

        virtual void notify()
        {
            for (auto& obs : vObservers)
                obs->notification(notificationData);
        }

    protected:
        std::list<_Ty*> vObservers;
        _Kty notificationData;
    };
    // ----------------------END PATTERN----------------------

    template<class _Ty>
    inline void parse_to(const std::string& name, const nlohmann::json& json, _Ty& type)
    {
        if (auto obj = json.find(name); obj != json.end())
            type = obj->get<_Ty>();
    }

    template<class _Ty>
    inline void serialize_from(const std::string& name, nlohmann::json& json, const _Ty& type, bool condition)
    {
        if (condition)
            json[name] = type;
    }
}

namespace nlohmann
{
    template <typename _Ty>
    struct adl_serializer<std::optional<_Ty>>
    {
        static void to_json(json& j, const std::optional<_Ty>& opt)
        {
            if (!opt.has_value())
                j = nullptr;
            else
                j = opt.value();
        }

        static void from_json(const json& j, std::optional<_Ty>& opt)
        {
            if (j.is_null())
                opt = std::nullopt;
            else
                opt = j.get<_Ty>();
        }
    };
}

#if !defined(HAS_SPACESHIP_OPERATOR)
// relational operators only needed for pre C++20
template <typename BitType>
constexpr bool operator<(BitType bit, utl::Flags<BitType> const& Flags) noexcept
{
    return Flags.operator>(bit);
}

template <typename BitType>
constexpr bool operator<=(BitType bit, utl::Flags<BitType> const& Flags) noexcept
{
    return Flags.operator>=(bit);
}

template <typename BitType>
constexpr bool operator>(BitType bit, utl::Flags<BitType> const& Flags) noexcept
{
    return Flags.operator<(bit);
}

template <typename BitType>
constexpr bool operator>=(BitType bit, utl::Flags<BitType> const& Flags) noexcept
{
    return Flags.operator<=(bit);
}

template <typename BitType>
constexpr bool operator==(BitType bit, utl::Flags<BitType> const& Flags) noexcept
{
    return Flags.operator==(bit);
}

template <typename BitType>
constexpr bool operator!=(BitType bit, utl::Flags<BitType> const& Flags) noexcept
{
    return Flags.operator!=(bit);
}
#endif

// bitwise operators
template <typename BitType>
constexpr utl::Flags<BitType> operator&(BitType bit, utl::Flags<BitType> const& Flags) noexcept
{
    return Flags.operator&(bit);
}

template <typename BitType>
constexpr utl::Flags<BitType> operator|(BitType bit, utl::Flags<BitType> const& Flags) noexcept
{
    return Flags.operator|(bit);
}

template <typename BitType>
constexpr utl::Flags<BitType> operator^(BitType bit, utl::Flags<BitType> const& Flags) noexcept
{
    return Flags.operator^(bit);
}

#define UTL_REGISTER_ENUM_CLASS_AS_FLAG_TYPE(FlagsName, EnumType) \
using FlagsName = utl::Flags<EnumType>; \
inline FlagsName operator|(EnumType a, EnumType b) { return FlagsName(utl::cast<FlagsName::MaskType>(a) | utl::cast<FlagsName::MaskType>(b)); } \
inline FlagsName operator&(EnumType a, EnumType b) { return FlagsName(utl::cast<FlagsName::MaskType>(a) & utl::cast<FlagsName::MaskType>(b)); } \
inline FlagsName operator^(EnumType a, EnumType b) { return FlagsName(utl::cast<FlagsName::MaskType>(a) ^ utl::cast<FlagsName::MaskType>(b)); }