#include "pch.h"
#include <wil/resource.h>
#include <iostream>
#include <string>
#include <type_traits>
#include <winrt/Windows.ApplicationModel.AppExtensions.h>
#include <winrt/SDK.h>

namespace wf = winrt::Windows::Foundation;
namespace wfc = winrt::Windows::Foundation::Collections;
namespace wamae = winrt::Windows::ApplicationModel::AppExtensions;

constexpr auto extensionName = L"com.packagedOopWinRtServer.host";

template<typename T>
auto TryGet(wfc::IPropertySet properties, std::wstring_view key)
{
    auto value{ properties.TryLookup(key) };
    if (value == nullptr)
    {
        if constexpr (std::is_same_v<T, winrt::hstring>)
        {
            return std::optional<T>(std::nullopt);
        }
        else
        {
            return T(nullptr);
        }
    }
    return value.try_as<T>();
}

wf::IAsyncOperation<int> MainAsync()
{
    auto catalog{ wamae::AppExtensionCatalog::Open(extensionName) };
    if (catalog == nullptr)
    {
        std::wcerr << L"Failed to get extension catalog" << std::endl;
        co_return 1;
    }
    auto extensions{ co_await catalog.FindAllAsync() };
    for (auto&& extension : extensions)
    {
        auto package{ extension.Package() };
        auto id{ package.Id() };
        auto pfn{ id.FamilyName() };
        auto winrtVersion{ package.Id().Version() };
        PACKAGE_VERSION version{};
        version.Major = winrtVersion.Major;
        version.Minor = winrtVersion.Minor;
        version.Build = winrtVersion.Build;
        version.Revision = winrtVersion.Revision;
        wchar_t* packageDependencyId{};
        if (FAILED(TryCreatePackageDependency(
            nullptr,
            pfn.c_str(),
            version,
            PackageDependencyProcessorArchitectures_X64, PackageDependencyLifetimeKind_Process,
            nullptr,
            CreatePackageDependencyOptions_None,
            &packageDependencyId)))
        {
            std::cerr << L"Failed to create package dependency for " << pfn.c_str() << std::endl;
            co_return 1;
        }
        auto idCleanup{ wil::scope_exit([&]()
            {
                if (packageDependencyId != nullptr)
                {
                    if (!HeapFree(GetProcessHeap(), 0, packageDependencyId))
                    {
                        std::cerr << L"Failed to free dependency id. GetLastError=" << GetLastError() << std::endl;
                    }
                }
            }) };
        PACKAGEDEPENDENCY_CONTEXT packageDependencyContext{};
        if (FAILED(AddPackageDependency(
            packageDependencyId,
            1,
            AddPackageDependencyOptions_None,
            &packageDependencyContext,
            nullptr)))
        {
            std::cerr << L"Failed to add dependency for " << pfn.c_str() << L" with id " << packageDependencyId << std::endl;
            co_return 1;
        }
        auto properties{ co_await extension.GetExtensionPropertiesAsync() };
        auto provider{ TryGet<wfc::IPropertySet>(properties, L"WidgetProvider")};
        if (provider == nullptr)
        {
            std::wcout << L"Could not find widget provider" << std::endl;
            continue;
        }
        auto activation{ TryGet<wfc::IPropertySet>(provider, L"Activation") };
        if (activation == nullptr)
        {
            std::wcout << L"Could not find activation" << std::endl;
            continue;
        }
        auto activatableClass{ TryGet<wfc::IPropertySet>(activation, L"ActivatableClass") };
        if (activatableClass == nullptr)
        {
            std::wcout << L"Could not find activatable class" << std::endl;
            continue;
        }
        auto classIdOpt{ TryGet<winrt::hstring>(activatableClass, L"@ActivatableClassId") };
        if (!classIdOpt)
        {
            std::wcout << L"Could not find class Id" << std::endl;
            continue;
        }
        auto classId{ *classIdOpt };
        auto factory{ winrt::get_activation_factory(classId)};
        auto widgetProvider{ factory.ActivateInstance<winrt::SDK::IWidgetProvider>() };
        if (widgetProvider == nullptr)
        {
            std::wcout << L"Extension's activatable class isn't an IWidgetProvider" << std::endl;
            continue;
        }
        auto hello{ widgetProvider.SayHello() };
        std::wcout << L"widget from " << pfn.c_str() << L" saying hello: " << hello.c_str() << std::endl;
    }
    co_return 0;
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/, int /*nCmdShow*/)
{
    winrt::init_apartment();
    try
    {
        return MainAsync().get();
    }
    catch (...)
    {
        return 1;
    }
}
