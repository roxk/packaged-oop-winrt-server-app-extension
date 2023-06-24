#include "pch.h"
#include "WidgetProvider.h"
#if __has_include("WidgetProvider.g.cpp")
#include "WidgetProvider.g.cpp"
#endif

namespace winrt::PackagedOOPWinRTServerAppExtension::implementation
{
    WidgetProvider::WidgetProvider()
    {
        auto provider = get_strong().try_as<winrt::SDK::IWidgetProvider>();
        if (provider == nullptr)
        {

        }
    }
    winrt::hstring WidgetProvider::SayHello()
    {
        return L"Greeting from C++ extension!";
    }
}
