#include <iostream>
#include <standart_library.h>

#include <benchmark/benchmark.h>

static int counter = 0;

class CMyClass
{
public:
    CMyClass()
    {
        std::cout << " Constructed " << number << std::endl;
    }

    ~CMyClass()
    {
        std::cout << " Destructed " << number << std::endl;
    }

private:
    int number{ counter++ };
};

std::string gen_random(const int len) 
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

    return tmp_s;
}

static void BM_unordered_map_simple(benchmark::State& state)
{
    std::unordered_map<uint64_t, uint64_t> mmap;
    for (auto _ : state)
    {
        for (size_t idx = 0ull; idx < state.range(); ++idx)
            mmap.emplace(idx, idx);

        benchmark::DoNotOptimize(mmap);
        benchmark::ClobberMemory();
    }
}

static void BM_unordered_map_big(benchmark::State& state)
{
    std::unordered_map<std::string, std::string> mmap;
    for (auto _ : state)
    {
        for (size_t idx = 0ull; idx < state.range(); ++idx)
            mmap.emplace(gen_random(64), gen_random(64));

        benchmark::DoNotOptimize(mmap);
        benchmark::ClobberMemory();
    }
}

static void BM_hash_map_simple(benchmark::State& state)
{
    std::unordered_map<uint64_t, uint64_t> mmap;
    for (auto _ : state)
    {
        for (size_t idx = 0ull; idx < state.range(); ++idx)
            mmap.emplace(idx, idx);

        benchmark::DoNotOptimize(mmap);
        benchmark::ClobberMemory();
    }
}

static void BM_hash_map_big(benchmark::State& state)
{
    std::unordered_map<std::string, std::string> mmap;
    for (auto _ : state)
    {
        for (size_t idx = 0ull; idx < state.range(); ++idx)
            mmap.emplace(gen_random(64), gen_random(64));

        benchmark::DoNotOptimize(mmap);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_unordered_map_simple)->Range(1024, 8 * 1024)->RangeMultiplier(1024)->Iterations(100);
BENCHMARK(BM_unordered_map_big)->Range(1024, 8 * 1024)->RangeMultiplier(1024)->Iterations(100);
BENCHMARK(BM_hash_map_simple)->Range(1024, 8 * 1024)->RangeMultiplier(1024)->Iterations(100);
BENCHMARK(BM_hash_map_big)->Range(1024, 8 * 1024)->RangeMultiplier(1024)->Iterations(100);

BENCHMARK_MAIN();

//int main()
//{
//    std::unordered_map<std::string, int> my_map;
//    my_map.emplace("One", 1);
//    my_map.emplace("Two", 2);
//    my_map.emplace("Three", 3);
//    my_map.emplace("Four", 4);
//    my_map.emplace("Five", 5);
//    my_map.emplace("Siz", 6);
//    my_map.emplace("Seven", 7);
//    my_map.emplace("Eight", 8);
//    my_map.emplace("Nine", 9);
//    my_map.emplace("Ten", 10);
//    my_map.emplace("Eleven", 11);
//    my_map.emplace("Twelve", 12);
//    my_map.emplace("Thirty", 13);
//    my_map.emplace("Fourty", 14);
//    my_map.emplace("Fifty", 15);
//    my_map.emplace("Sixty", 16);
//    
//    std::unordered_map<std::string, int> my_another_map;
//    my_another_map = my_map;
//    
//    for (auto it = my_map.begin(); it != my_map.end(); ) 
//    {
//        if (it->second % 2 == 0) 
//        {
//            std::cout << "Erasing element: " << it->first << std::endl;
//            it = my_map.erase(it);
//    
//            auto other = my_map.find("Five");
//            other->first;
//        }
//        else
//            ++it;
//    }
//    
//    std::cout << "Remaining elements:" << std::endl;
//    for (const auto& [name, value] : my_map)
//    {
//        std::cout << name << ": " << value << std::endl;
//    }
//    
//    std::unordered_map<uint64_t, std::unique_ptr<uint32_t>> unique_map{};
//    unique_map.emplace(0, std::make_unique<uint32_t>(0));
//    unique_map.emplace(1, std::make_unique<uint32_t>(1));
//    unique_map.emplace(2, std::make_unique<uint32_t>(2));
//    unique_map.emplace(3, std::make_unique<uint32_t>(3));
//    unique_map.emplace(4, std::make_unique<uint32_t>(4));
//    unique_map.emplace(5, std::make_unique<uint32_t>(5));
//    unique_map.emplace(6, std::make_unique<uint32_t>(6));
//    unique_map.emplace(7, std::make_unique<uint32_t>(7));
//    unique_map.emplace(8, std::make_unique<uint32_t>(8));
//    unique_map.emplace(9, std::make_unique<uint32_t>(9));
//    
//    std::unordered_map<uint64_t, std::unique_ptr<CMyClass>> object_map;
//    object_map.emplace(0, std::make_unique<CMyClass>());
//    object_map.emplace(1, std::make_unique<CMyClass>());
//    object_map.emplace(2, std::make_unique<CMyClass>());
//    object_map.emplace(3, std::make_unique<CMyClass>());
//    object_map.emplace(4, std::make_unique<CMyClass>());
//    object_map.emplace(5, std::make_unique<CMyClass>());
//    object_map.emplace(6, std::make_unique<CMyClass>());
//    
//    object_map.clear();
//
//    //std::unordered_map<uint64_t, uint64_t> object_map;
//    //for (uint64_t idx = 0ull; idx < 1000000000ull; ++idx)
//    //    object_map.emplace(idx, idx);
//    //
//    //return 0;
//}