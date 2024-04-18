#include <iostream>
#include <standart_library.h>
#include <container/hash_map.h>

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

int main()
{
    utl::hash_map<std::string, int> my_map;
    my_map.emplace("One", 1);
    my_map.emplace("Two", 2);
    my_map.emplace("Three", 3);
    my_map.emplace("Four", 4);
    my_map.emplace("Five", 5);
    my_map.emplace("Siz", 6);
    my_map.emplace("Seven", 7);
    my_map.emplace("Eight", 8);
    my_map.emplace("Nine", 9);
    my_map.emplace("Ten", 10);
    my_map.emplace("Eleven", 11);
    my_map.emplace("Twelve", 12);
    my_map.emplace("Thirty", 13);
    my_map.emplace("Fourty", 14);
    my_map.emplace("Fifty", 15);
    my_map.emplace("Sixty", 16);

    utl::hash_map<std::string, int> my_another_map;
    my_another_map = my_map;

    for (auto it = my_map.begin(); it != my_map.end(); ) 
    {
        if (it->second % 2 == 0) 
        {
            std::cout << "Erasing element: " << it->first << std::endl;
            it = my_map.erase(it);

            auto other = my_map.find("Five");
            other->first;
        }
        else
            ++it;
    }

    std::cout << "Remaining elements:" << std::endl;
    for (const auto& [name, value] : my_map)
    {
        std::cout << name << ": " << value << std::endl;
    }

    utl::hash_map<u64, std::unique_ptr<u32>> unique_map{};
    unique_map.emplace(0, std::make_unique<u32>(0));
    unique_map.emplace(1, std::make_unique<u32>(1));
    unique_map.emplace(2, std::make_unique<u32>(2));
    unique_map.emplace(3, std::make_unique<u32>(3));
    unique_map.emplace(4, std::make_unique<u32>(4));
    unique_map.emplace(5, std::make_unique<u32>(5));
    unique_map.emplace(6, std::make_unique<u32>(6));
    unique_map.emplace(7, std::make_unique<u32>(7));
    unique_map.emplace(8, std::make_unique<u32>(8));
    unique_map.emplace(9, std::make_unique<u32>(9));

    utl::hash_map<u64, std::unique_ptr<CMyClass>> object_map;
    object_map.emplace(0, std::make_unique<CMyClass>());
    object_map.emplace(1, std::make_unique<CMyClass>());
    object_map.emplace(2, std::make_unique<CMyClass>());
    object_map.emplace(3, std::make_unique<CMyClass>());
    object_map.emplace(4, std::make_unique<CMyClass>());
    object_map.emplace(5, std::make_unique<CMyClass>());
    object_map.emplace(6, std::make_unique<CMyClass>());

    object_map.clear();

    return 0;
}