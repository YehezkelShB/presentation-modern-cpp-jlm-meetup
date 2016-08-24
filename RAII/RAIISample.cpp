/**
 * Based on sample code from MSDN - Example: Getting WMI Data from the Local Computer
 * https://msdn.microsoft.com/en-us/library/aa390423(v=vs.85).aspx
 * The code was changed and adapted to use RAII consistently.
 * There are many more improvements that can be done in the code (the most obvious
 * one is replacing NULL with nullptr, but there are more subtle ones, like handling
 * the various cases of CoInitializeEx() return values), but the target here was to
 * demonstrate how to apply RAII idiom to an existing code.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <comdef.h>
#include <Wbemidl.h>
#include <atlcomcli.h>

#pragma comment(lib, "wbemuuid.lib")

#include "COMStyleUniquePtr.h"

std::string toHex(int value)
{
    std::ostringstream oss;
    oss << std::showbase << std::hex << value;
    return oss.str();
}

struct CoInitHandler
{
    CoInitHandler(COINIT mode)
    {
        auto hres = CoInitializeEx(0, mode);
        if (FAILED(hres))
        {
            throw std::runtime_error("Failed to initialize COM library. Error code = " + toHex(hres));
        }
    }

    ~CoInitHandler()
    {
        CoUninitialize();
    }
};

auto createLocator()
{
    IWbemLocator *pLoc = NULL;

    auto hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres))
    {
        throw std::runtime_error("Failed to create IWbemLocator object. Err code = " + toHex(hres));
    }

    return WindowsUtils::toCOMStyleUniquePtr(pLoc);
}

auto connectServer(IWbemLocator& loc)
{
    IWbemServices *pSvc = NULL;

    // Connect to the root\cimv2 namespace with
    // the current user and obtain pointer pSvc
    // to make IWbemServices calls.
    auto hres = loc.ConnectServer(_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
                                  NULL,                    // User name. NULL = current user
                                  NULL,                    // User password. NULL = current
                                  0,                       // Locale. NULL indicates current
                                  NULL,                    // Security flags.
                                  0,                       // Authority (for example, Kerberos)
                                  0,                       // Context object
                                  &pSvc);                  // pointer to IWbemServices proxy

    if (FAILED(hres))
    {
        throw std::runtime_error("Could not connect. Error code = " + toHex(hres));
    }
    return WindowsUtils::toCOMStyleUniquePtr(pSvc);
}

auto execQuery(IWbemServices& svc)
{
    IEnumWbemClassObject* pEnumerator = NULL;
    auto hres = svc.ExecQuery(bstr_t("WQL"),
                              bstr_t("SELECT * FROM Win32_OperatingSystem"),
                              WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                              NULL,
                              &pEnumerator);

    if (FAILED(hres))
    {
        throw std::runtime_error("Query for operating system name failed. Error code = " + toHex(hres));
    }

    return WindowsUtils::toCOMStyleUniquePtr(pEnumerator);
}

auto getNext(IEnumWbemClassObject& enumerator)
{
    IWbemClassObject *pclsObj = nullptr;
    ULONG uReturn = 0;
    HRESULT hr = enumerator.Next(WBEM_INFINITE,
                                 1,
                                 &pclsObj,
                                 &uReturn);

    return WindowsUtils::toCOMStyleUniquePtr(pclsObj);
}

int main() try
{
    // Step 1: --------------------------------------------------
    // Initialize COM. ------------------------------------------

    CoInitHandler coInit(COINIT::COINIT_MULTITHREADED);

    // Step 2: --------------------------------------------------
    // Set general COM security levels --------------------------

    auto hres = CoInitializeSecurity(NULL,
                                     -1,                          // COM authentication
                                     NULL,                        // Authentication services
                                     NULL,                        // Reserved
                                     RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
                                     RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
                                     NULL,                        // Authentication info
                                     EOAC_NONE,                   // Additional capabilities
                                     NULL                         // Reserved
                                     );

    if (FAILED(hres))
    {
        throw std::runtime_error("Failed to initialize security. Error code = " + toHex(hres));
    }

    // Step 3: ---------------------------------------------------
    // Obtain the initial locator to WMI -------------------------

    auto pLoc = createLocator();

    // Step 4: -----------------------------------------------------
    // Connect to WMI through the IWbemLocator::ConnectServer method

    auto pSvc = connectServer(*pLoc);
    std::cout << "Connected to ROOT\\CIMV2 WMI namespace\n";

    // Step 5: --------------------------------------------------
    // Set security levels on the proxy -------------------------

    hres = CoSetProxyBlanket(pSvc.get(),                  // Indicates the proxy to set
                             RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                             RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                             NULL,                        // Server principal name 
                             RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
                             RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                             NULL,                        // client identity
                             EOAC_NONE);                  // proxy capabilities 

    if (FAILED(hres))
    {
        throw std::runtime_error("Could not set proxy blanket. Error code = " + toHex(hres));
    }

    // Step 6: --------------------------------------------------
    // Use the IWbemServices pointer to make requests of WMI ----

    // For example, get the name of the operating system
    auto pEnumerator = execQuery(*pSvc);

    // Step 7: -------------------------------------------------
    // Get the data from the query in step 6 -------------------

    while (pEnumerator)
    {
        auto pclsObj = getNext(*pEnumerator);

        if (!pclsObj)
        {
            break;
        }

        CComVariant vtProp;

        // Get the value of the Name property
        hres = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        std::wcout << " OS Name: " << vtProp.bstrVal << L'\n';
    }
}
catch(std::exception& e)
{
    std::cerr << "Exception: " << e.what() << '\n';
    return EXIT_FAILURE;
}
catch(...)
{
    std::cerr << "Unknown exception\n";
    return EXIT_FAILURE;
}
