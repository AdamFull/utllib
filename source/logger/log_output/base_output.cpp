#include "base_output.h"

#include <logger/formatter.h>

using namespace utl::logger;

void COutputBase::init(const std::string& app_name, const std::string& app_version)
{
    write("#Software: " + app_name);
    write("#Version: " + app_version);
    write(std::format("#Date: {}[{}]", formatter::get_formatted_datetime(), formatter::get_formatted_timezone()));
}