#pragma once

#include "function.h"

namespace utl
{
	namespace reactive
	{
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
	}
}