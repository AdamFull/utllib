#pragma once

#include <standart_library.h>
#include <nlohmann/json.hpp>

namespace utl
{
	namespace parsing
	{
        template<class _Ty>
        inline void parse_to(const stl::string& name, const nlohmann::json& json, _Ty& type)
        {
            if (auto obj = json.find(name); obj != json.end())
                type = obj->get<_Ty>();
        }

        template<class _Ty>
        inline void serialize_from(const stl::string& name, nlohmann::json& json, const _Ty& type, bool condition)
        {
            if (condition)
                json[name] = type;
        }
	}
}

namespace nlohmann
{
    template <typename _Ty>
    struct adl_serializer<stl::optional<_Ty>>
    {
        static void to_json(json& j, const stl::optional<_Ty>& opt)
        {
            if (!opt.has_value())
                j = nullptr;
            else
                j = opt.value();
        }

        static void from_json(const json& j, stl::optional<_Ty>& opt)
        {
            if (j.is_null())
                opt = stl::nullopt;
            else
                opt = j.get<_Ty>();
        }
    };
}