#pragma once

#include <standart_library.h>

namespace utl
{
	namespace pattern
	{
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
            static inline const std::unique_ptr<_Ty>& GetInstance()
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
            constexpr _Kty* As() noexcept
            {
                if (is_same<_Kty>())
                    return cast<_Kty*>(this);
                return nullptr;
            }

            template<class _Kty>
            constexpr bool IsSame() noexcept
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
            virtual void Notification(const _Ty&) = 0;
        };

        template<class _Ty, class _Kty>
        class subject
        {
        public:
            virtual ~subject() = default;

            virtual void Attach(_Ty* _ptr) { vObservers.emplace_back(_ptr); }

            virtual void Detach(_Ty* _ptr) { vObservers.remove(_ptr); }

            virtual void Notify()
            {
                for (auto& obs : vObservers)
                    obs->Notification(notificationData);
            }

        protected:
            std::list<_Ty*> vObservers;
            _Kty notificationData;
        };
	}
}