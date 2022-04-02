#pragma once

namespace utl
{
    template <class T>
    class observable_data
    {
    };

    template <class T>
    class observer_interface
    {
    public:
        virtual ~observer_interface(){};
        virtual void update(const observable_data<T> &) = 0;
    };

    template <class T>
    class observable_subject
    {
    public:
        virtual ~observable_subject(){};
        virtual void attach(observer_interface<T> *observer) = 0;
        virtual void detach(observer_interface<T> *observer) = 0;
        virtual void notify() = 0;
    };
}