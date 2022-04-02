#include <utl.hpp>
#include <iostream>

class Zalupa{};
int main()
{
    auto var = 10;
    auto converted = utl::string_util::conv(var);
    auto casted = utl::string_util::cast<int>(converted);
    std::cout << var << " " << utl::string_util::name_of(var) << converted << " " 
    << utl::string_util::name_of(converted) << casted << " " 
    << utl::string_util::name_of(casted) << " " 
    << utl::string_util::name_of<Zalupa>() << std::endl;

    utl::rint32_t penis;
    penis.bind([&](const int32_t& a, const int32_t& b){
        std::cout << a << b << std::endl;
    });
    penis = 1488;

    utl::rstring rstr;
    rstr.bind([&](const std::string& a, const std::string& b){
        std::cout << a << b << std::endl;
    });
    rstr = std::string("hello world");

    auto pointer = utl::make_unique<utl::vector<utl::string>>();

    return 0;
}