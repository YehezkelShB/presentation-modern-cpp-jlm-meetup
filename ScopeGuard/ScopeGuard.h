#pragma once

namespace Utils
{

/**
 * \brief Ad-hoc RAII utility class
 *
 * This class can be used for having an ad-hoc RAII behavior without the need
 * to write a dedicated wrapper class for each and every case we need RAII.
 *
 * This class is Movable and NonCopyable.
 *
 * Usually you want to create it using makeScopeGuard() helper function.
 *
 * disable() function is const (and thus m_enable member must be mutable) because
 * ScopeGuard is usually held as const.
 *
 * If you need to explicitly declare the variable (probably because you can't
 * initializing it yet), you probably want to use std::function as template
 * argument.
 *
 * It's possible to improve it to have versions that run the function only on
 * return or only on exception (using std::uncaught_exceptions()).
 *
 * There are probably better implementations (e.g. look in Microsoft GSL), these
 * is just an example. 
 */
template <typename Callable>
class ScopeGuard
{
    template <typename Callable2>
    friend class ScopeGuard;

public:
    ScopeGuard();
    explicit ScopeGuard(Callable func);

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    ScopeGuard(ScopeGuard&& other);
    ScopeGuard& operator=(ScopeGuard&& other);

    template <typename Callable2>
    ScopeGuard(ScopeGuard<Callable2>&& other);
    template <typename Callable2>
    ScopeGuard& operator=(ScopeGuard<Callable2>&& other);

    ~ScopeGuard();

    void disable() const { m_enabled = false; }
    bool isEnabled() const { return m_enabled; }

private:
    template <typename Callable2>
    ScopeGuard& doMove(ScopeGuard<Callable2>&& other);
    void cleanup();
    Callable m_func;
    mutable bool m_enabled = false;
};

template <typename Callable>
ScopeGuard<Callable>::ScopeGuard() = default;

template <typename Callable>
ScopeGuard<Callable>::ScopeGuard(Callable func) : m_func(func), m_enabled(true)
{
}

template <typename Callable>
ScopeGuard<Callable>::ScopeGuard(ScopeGuard&& other) : m_func(std::move(other.m_func)), m_enabled(other.m_enabled)
{
    other.disable();
}

template <typename Callable>
ScopeGuard<Callable>& ScopeGuard<Callable>::operator=(ScopeGuard&& other)
{
    return doMove(std::move(other));
}

template <typename Callable>
template <typename Callable2>
ScopeGuard<Callable>::ScopeGuard(ScopeGuard<Callable2>&& other)
    : m_func(std::move(other.m_func)), m_enabled(other.m_enabled)
{
    other.disable();
}

template <typename Callable>
template <typename Callable2>
ScopeGuard<Callable>& ScopeGuard<Callable>::operator=(ScopeGuard<Callable2>&& other)
{
    return doMove(std::move(other));
}

template <typename Callable>
ScopeGuard<Callable>::~ScopeGuard()
{
    cleanup();
}

template <typename Callable>
template <typename Callable2>
ScopeGuard<Callable>& ScopeGuard<Callable>::doMove(ScopeGuard<Callable2>&& other)
{
    cleanup();
    m_func    = std::move(other.m_func);
    m_enabled = other.m_enabled;
    other.disable();
    return *this;
}

template <typename Callable>
void ScopeGuard<Callable>::cleanup()
{
    if (isEnabled())
    {
        m_func();
    }
}

template <typename Callable>
ScopeGuard<Callable> makeScopeGuard(Callable func)
{
    return ScopeGuard<Callable>(func);
}

} // namespace Utils
