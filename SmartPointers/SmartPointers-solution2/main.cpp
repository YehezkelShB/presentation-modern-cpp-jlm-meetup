#include <iostream>
#include <memory>

class Base
{
public:
    virtual ~Base() = default;
    virtual void doSomething() = 0;
};

class Derived1 : public Base
{
public:
    void doSomething() override
    {
        std::cout << "Derived1::doSomething()\n";
    }
};

class Derived2 : public Base
{
public:
    void doSomething() override
    {
        std::cout << "Derived2::doSomething()\n";
    }
};

std::unique_ptr<Base> factory(char num)
{
    switch (num)
    {
    case '1':
        return std::unique_ptr<Base>(new Derived1);
    case '2':
        return std::make_unique<Derived2>();
    }
    return nullptr; // or throw an exception
}

int main()
{
    char c = 0;
    std::cout << "Choose which class to instantiate (1-2): ";
    std::cin >> c;

    auto p = factory(c);

    if (!p)
    {
        std::cout << "Invalid choice!" << std::endl;
        return EXIT_FAILURE;
    }

    // Now a lot of code that uses p, for example:
    p->doSomething();
}
