#pragma once
#include <functional>

namespace utl
{
    namespace delegate
    {
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
            function(_LabbdaFunction &&lfunc)
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
            template <class _Class, class _ReturnType, class... Args>
            function(_Class *c, _ReturnType (_Class::*m)(Args...)) noexcept
            {
                attach(c, m);
            }

            /**
             * @brief Default copy constructor
             *
             * @param rDelegate
             */
            function(const function<_signature> &rDelegate) = default;

            /**
             * @brief Default copy assignment operator
             *
             * @param lDelegate
             * @return function<_signature>&
             */
            function<_signature> &operator=(const function<_signature> &lDelegate) = default;

            /**
             * @brief Default move constructor
             *
             * @param rDelegate
             */
            function(function<_signature> &&rDelegate) = default;

            /**
             * @brief Default move assignment constructor
             *
             * @param rDelegate
             * @return function<_signature>&
             */
            function<_signature> &operator=(function<_signature> &&rDelegate) = default;

            /**
             * @brief The method is intended for binding a lambda function or function to a delegate.
             *
             * @tparam LabbdaFunction lambda function type transited with template parameter.
             * @param lfunc
             */
            template <class _LabbdaFunction>
            inline void attach(_LabbdaFunction &&lfunc) noexcept
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
            template <class _Class, class _ReturnType, class... Args>
            inline void attach(_Class *c, _ReturnType (_Class::*m)(Args...)) noexcept
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
             * @tparam Args Templated std::tuple arguments
             * @param args Delegate arguments
             * @return auto
             */
            template <class... Args>
            inline auto operator()(Args &&...args)
            {
                return m_pFunction(std::forward<Args>(args)...);
            }

        private:
            std::function<_signature> m_pFunction{nullptr};

            /**
             * @brief
             *
             * Implementation of creating a delegate for a static method.
             *
             * @tparam ReturnType (set automatically in c++17) class type
             * @tparam Args (set automatically in c++17) another arguments
             * @param m reference to function
             * @return std::function<ReturnType(Args...)>
             */
            template <class _ReturnType, class... Args>
            inline std::function<_ReturnType(Args...)> make_delegate(_ReturnType (*m)(Args...)) noexcept
            {
                return [=](Args &&...args)
                { return (*m)(std::forward<Args>(args)...); };
            }

            /**
             * @brief
             *
             * Implementation of creating a delegate for a class.
             *
             * @tparam Class (set automatically in c++17) class type
             * @tparam ReturnType (set automatically in c++17)
             * @tparam Args (set automatically in c++17)
             * @param c pointer to class
             * @param m reference to class method
             * @return std::function<ReturnType(Args...)>
             */
            template <class _Class, class _ReturnType, class... Args>
            inline std::function<_ReturnType(Args...)> make_delegate(_Class *c, _ReturnType (_Class::*m)(Args...)) noexcept
            {
                return [=](Args &&...args)
                { return (c->*m)(std::forward<Args>(args)...); };
            }

            /**
             * @brief
             *
             * Implementation of creating a delegate for a constant class.
             *
             * @tparam Class (set automatically in c++17)
             * @tparam ReturnType (set automatically in c++17)
             * @tparam Args (set automatically in c++17)
             * @param c const pointer to class
             * @param m reference to class method
             * @return std::function<ReturnType(Args...)>
             */
            template <class _Class, class _ReturnType, class... Args>
            inline std::function<_ReturnType(Args...)> make_delegate(const _Class *c, _ReturnType (_Class::*m)(Args...) const) noexcept
            {
                return [=](Args &&...args)
                { return (c->*m)(std::forward<Args>(args)...); };
            }
        };

        template <class _signature>
        using TDelegate = function<_signature>;
    }
}