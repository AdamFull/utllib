#pragma once

#include <standart_library.h>

// Got it from here https://gist.github.com/AlexBAV/b58e92d7632bae5d6f5947be455f796f
namespace utl
{
    struct uuid_v4
    {
        static std::string gen();
    };
}