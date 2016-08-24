#pragma once

/**
 * \brief Smart pointer (std::unique_ptr) that handles COM objects correctly
 *
 * These set of definitions give us a convenient way to create std::unique_ptr
 * pointer that holds pointer to a COM object and uses Release() for deleting
 * it.
 *
 * Actually, by-design, there is no such things as "exclusive ownership" on
 * COM objects, as they use reference counting internally. Still, we usually
 * want to use them like unique_ptr both for efficiency and preventing an
 * accidental copy.
 */

namespace WindowsUtils
{
    
template <typename T>
struct Releaser
{
    void operator()(T* p)
    {
        if (p) // to make it suitable for shared_ptr, too
        {
            p->Release();
        }
    }
};

/**
 * If you have to declare the pointer type explicitly instead of using "auto"
 * (e.g. when having it as class data member), use COMStyleUniquePtr_t<T>,
 * when T is the COM type you want to point to.
 * For example: COMStyleUniquePtr_t<IUnknown>
 */
template <typename T>
using COMStyleUniquePtr_t = std::unique_ptr<T, Releaser<T>>;

// For backward compatibility with code that was written against the VS2010
// version of this type
template <typename T>
struct COMStyleUniquePtr
{
    typedef COMStyleUniquePtr_t<T> type;

    // To make sure no one mistakenly create an instance of this struct
    ~COMStyleUniquePtr() = delete;
};

/**
 * \brief Convert raw pointer to our smart pointer; convenient function
 */
template <typename T>
auto toCOMStyleUniquePtr(T* p)
{
    return COMStyleUniquePtr_t<T>(p);
}

/**
 * \brief Equivalent to std::make_unique
 */
template <typename T, typename... Args>
auto makeCOMStyleUnique(Args&&... args)
{
    return COMStyleUniquePtr_t<T>(std::forward<Args>(args)...);
}

} // namespace WindowsUtils
