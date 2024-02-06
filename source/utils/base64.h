#pragma once

#include <standart_library.h>

namespace utl
{
    namespace utils
    {
        struct base64
        {
            static stl::string encode(uint8_t const* buf, unsigned int bufLen);
            static stl::vector<uint8_t> decode(stl::string const&);

            static inline bool is_base64(uint8_t c) {
                return (isalnum(c) || (c == '+') || (c == '/'));
            }
        };
    }
}
