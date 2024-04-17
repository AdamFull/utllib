#include <iostream>
#include <standart_library.h>
#include <container/hash_map.h>


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

    return 0;
}