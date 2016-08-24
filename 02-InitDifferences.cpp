struct S
{
    int i;
    double d;
};

class C
{
public:
    C();

private:
    int m_i;
    S m_s;
    static const int arrSize = 10;
    long m_numbers[arrSize];
    std::map<std::string, int> m_map;
};

C::C() : m_i(42)
{
    m_s.i = 7;
    m_s.d = 3.14;
    // Or: S s = {7, 3.14}; m_s = s;
    
    for (long i = 0; i < arrSize; ++i)
    {
        m_numbers[i] = i;
    }
    // vs. non-member array init:
    // long numbers[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    m_map.insert(std::make_pair("a", 1));
    m_map.insert(std::make_pair("b", 2));
    m_map.insert(std::make_pair("c", 3));
    m_map.insert(std::make_pair("d", 4));
    m_map.insert(std::make_pair("e", 5));
}

// The modern implementation with brace initialization
C::C(): m_i(42),
        m_s{7, 3.14},
        m_numbers{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        m_map{
            {"a", 1},
            {"b", 2},
            {"c", 3},
            {"d", 4},
            {"e", 5},    
        }
{
}

// Even better implementation with in-class init
class C
{
private:
    int m_i = 42;
    S m_s{7, 3.14};
    static const int arrSize = 10;
    long m_numbers[arrSize] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::map<std::string, int> m_map{
            {"a", 1},
            {"b", 2},
            {"c", 3},
            {"d", 4},
            {"e", 5},    
        };
};
