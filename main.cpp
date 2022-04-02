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

    auto pool = utl::threadpool();
    utl::vector<std::future<uint32_t>> results{};
    for(uint32_t t = 0; t < 1000; t++) results.emplace_back(pool.submit([=](const uint32_t a, const uint32_t b, const uint32_t c) -> uint32_t
    { 
        return (a*1000/10)*(120 + b*t+c)/(a+b*c)+1; 
    }, static_cast<uint32_t>(t*t, t/(t+1)), static_cast<uint32_t>((t*t)*t/t+1), static_cast<uint32_t>(((t-t)+(t/t+1)))));
    pool.wait();

    uint32_t sum{0};
    for(auto& res : results)
    {
        sum += res.get();
    }
    std::cout << "Future result: " << sum << std::endl;
    return 0;
}