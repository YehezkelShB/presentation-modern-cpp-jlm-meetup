/*
 * Code snippets that use ScopeGuard for ad-hoc RAII
 */

{
    // ...
    // Getting a message from WMI to handle
    // ...

    _variant_t vtProp;
    auto hres = message.Get(L"Message", 0, &vtProp, 0, 0);
    if (FAILED(hres))
    {
         // ... Handling errors
    }

    // We know there is an array there. It's easier to access it with CComSafeArray.
    // We can do it, but then we have to detach it or we'll get a double free (because
    // _variant_t owns it).
    CComSafeArray<uint8_t> arr;
    arr.Attach(vtProp.parray);
    const auto& detach = Utils::makeScopeGuard([&arr] { arr.Detach(); });

    // Continue working; no fear from throwing exceptions
    // ...
}

{
    // ...
    // Preparations to FW update
    // ...
    
    m_inFWUpdate          = true;
    const auto& flagGuard = Utils::makeScopeGuard([this] { m_inFWUpdate = false; });

    // Continue working; status will auto reset itself
    // ...
}
