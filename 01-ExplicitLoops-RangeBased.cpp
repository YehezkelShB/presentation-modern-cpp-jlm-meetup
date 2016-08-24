#include <iostream>
#include <map>
#include <vector>
#include <string>

std::string toWords(int)
{
    return "test"; // Just for demonstration
}

int main()
{
    std::srand(4);

    const int count = 10;
    std::vector<int> v(count);
    for (auto &i : v)
    {
        i = std::rand() % 1000;
    }
    
    int countOfHundreds = 0;
    for (auto i : v)
    {
        if (i >= 100)
        {
            ++countOfHundreds;
        }
    }
    
    if (countOfHundreds < 5)
    {
        return 0;
    }
    
    std::map<int, std::string> m;
    for (auto i : v)
    {
        m[i] = toWords(i);
    }
    
    for (const auto& val : m)
    {
        std::cout << val.first << " - " << val.second << '\n';
    }
}
