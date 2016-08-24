#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

std::string toWords(int)
{
    return "test"; // Just for demonstration
}

int randomNum()
{
    return std::rand() % 1000;
}

bool hundredOrMore(int i)
{
    return i >= 100;
}

std::pair<int, std::string> transformer(int i)
{
    return std::make_pair(i, toWords(i));
}

void printer(const std::pair<int, std::string>& p)
{
    std::cout << p.first << " - " << p.second << '\n';
}

int main()
{
    std::srand(4);

    const int count = 10;
    std::vector<int> v(count);
    std::generate_n(v.begin(), v.size(), randomNum);
    
    if (std::count_if(v.cbegin(), v.cend(), hundredOrMore) < 5)
    {
        return 0;
    }

    std::map<int, std::string> m;
    std::transform(v.cbegin(), v.cend(), std::inserter(m, m.begin()), transformer);

    std::for_each(m.cbegin(), m.cend(), printer);
}
