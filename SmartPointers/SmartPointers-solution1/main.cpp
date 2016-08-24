#include <iostream>

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

// Very bad; what about copy operations? Never mind, it's just for describing
// the idea behind smart pointers
class BasePointer
{
public:
    BasePointer(Base* p = nullptr) : m_p(p) {}
    ~BasePointer() { delete m_p; }

    Base& operator*() const { return *m_p; }
    Base* operator->() const { return m_p; }
    Base* get() const { return m_p; }

private:
    Base* m_p = nullptr;
};

BasePointer factory(char num)
{
    switch (num)
    {
    case '1':
        return new Derived1;
    case '2':
        return new Derived2;
    }
    return nullptr; // or throw an exception
}

int main()
{
    char c = 0;
    std::cout << "Choose which class to instantiate (1-2): ";
    std::cin >> c;

    auto p = factory(c);

    if (!p.get()) // BasePointer can be improved to support if(!p) syntax
    {
        std::cout << "Invalid choice!" << std::endl;
        return EXIT_FAILURE;
    }

    // Now a lot of code that uses p, for example:
    p->doSomething();
}
