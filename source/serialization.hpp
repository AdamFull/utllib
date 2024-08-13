#pragma once

#include <nlohmann/json.hpp>
#include "string.hpp"
#include <charconv>

namespace utl
{
    template<typename _Ty, typename _HashType>
    void to_json(nlohmann::json& j, const id_string<_Ty, _HashType>& idStr)
    {
        if (idStr.valid())
            j = std::format("{}#{}", idStr.str(), idStr.hash());
        else
            j = ""; // or handle empty string case
    }

    // Function to convert JSON to id_string
    template<typename _Ty, typename _HashType>
    void from_json(const nlohmann::json& j, id_string<_Ty, _HashType>& idStr) 
    {
        using id_string_t = id_string<_Ty, _HashType>;
        using string_t = id_string_t::value_type;

        string_t::value_type serialized = j.get<string_t>();
        auto pos = serialized.find('#');
        if (pos != string_t::npos) 
        {
            string_t name = serialized.substr(0, pos);
            _HashType hash;
            auto [ptr, ec] = std::from_chars(serialized.data() + pos + 1, serialized.data() + serialized.size(), hash);
            if (ec == std::errc())
                idStr = id_string_t(name, hash);
        }
    }
}