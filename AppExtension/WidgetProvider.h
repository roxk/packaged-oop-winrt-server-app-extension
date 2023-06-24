#pragma once

#include <winrt/SDK.h>
#include "WidgetProvider.g.h"

namespace winrt::PackagedOOPWinRTServerAppExtension::implementation
{
    struct WidgetProvider : WidgetProviderT<WidgetProvider>
    {
        WidgetProvider();
        winrt::hstring SayHello();
    };
}

namespace winrt::PackagedOOPWinRTServerAppExtension::factory_implementation
{
    struct WidgetProvider : WidgetProviderT<WidgetProvider, implementation::WidgetProvider>
    {
    };
}
