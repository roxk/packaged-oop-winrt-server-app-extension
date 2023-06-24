#include "pch.h"
#include "WidgetProvider.h"
#if __has_include("WidgetProvider.g.cpp")
#include "WidgetProvider.g.cpp"
#endif

namespace winrt::PackagedOOPWinRTServerAppExtension::implementation
{
    WidgetProvider::WidgetProvider()
    {
    }
    winrt::hstring WidgetProvider::SayHello()
    {
        return L"Greeting from C++ extension!";
    }
}
