// From MSDN - User-Defined Literals (C++)
// https://msdn.microsoft.com/en-us/library/dn919277.aspx

#include <iostream>

struct Distance
{
private:
    explicit Distance(long double val) : kilometers(val) {}

    friend Distance operator"" _km(long double val);
    friend Distance operator"" _mi(long double val);

    long double kilometers{0};

public:
    long double get_kilometers() { return kilometers; }

    Distance operator+(Distance& other) { return Distance(get_kilometers() + other.get_kilometers()); }
};

Distance operator"" _km(long double val)
{
    return Distance(val);
}

Distance operator"" _mi(long double val)
{
    return Distance(val * 1.6);
}

int main(int argc, char* argv[])
{
    // Must have a decimal point to bind to the operator we defined!
    Distance d{402.0_km};                                           // construct using kilometers
    std::cout << "Kilometers in d: " << d.get_kilometers() << '\n'; // 402

    Distance d2{402.0_mi}; // construct using miles
    std::cout << "Kilometers in d2: " << d2.get_kilometers() << '\n'; // 643.2

    // add distances constructed with different units
    Distance d3 = 36.0_mi + 42.0_km;
    std::cout << "d3 value = " << d3.get_kilometers() << '\n'; // 99.6

    // Distance d4(90.0); // error: constructor not accessible
}
