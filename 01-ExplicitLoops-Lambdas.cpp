#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

std::string toWords(int)
{
    return "test"; // Just for demonstration
}

int main()
{
    std::srand(4);

    const int count = 10;
    std::vector<int> v(count);
    std::generate_n(v.begin(), v.size(), [] { return std::rand() % 1000; });
    
    if (5 > std::count_if(v.cbegin(), v.cend(), [](int i) { return i >= 100; }))
    {
        return 0;
    }

    std::map<int, std::string> m;
    std::transform(v.cbegin(), v.cend(), std::inserter(m, m.begin()),
                   [](int i) { return std::make_pair(i, toWords(i)); });

    std::for_each(m.cbegin(), m.cend(), [](const auto& p) {
        std::cout << p.first << " - " << p.second << '\n';
    });
}
