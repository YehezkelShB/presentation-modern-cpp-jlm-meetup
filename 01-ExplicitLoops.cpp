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
    for (int i = 0; i < count; ++i)
    {
        v[i] = std::rand() % 1000;
    }
    
    int countOfHundreds = 0;
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (v[i] >= 100)
        {
            ++countOfHundreds;
        }
    }
    
    if (countOfHundreds < 5)
    {
        return 0;
    }
    
    std::map<int, std::string> m;
    for (size_t i = 0; i < v.size(); ++i)
    {
        m[v[i]] = toWords(v[i]);
    }
    
    for (auto i = m.cbegin(); i != m.cend(); ++i)
    {
        std::cout << i->first << " - " << i->second << '\n';
    }
}
