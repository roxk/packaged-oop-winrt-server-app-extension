#include "pch.h"
#include <winrt/Windows.ApplicationModel.Core.h>

namespace wf = winrt::Windows::Foundation;
namespace wamc = winrt::Windows::ApplicationModel::Core;

void* __stdcall winrt_get_activation_factory([[maybe_unused]] std::wstring_view const& name);

struct ActivationFactoryGetter : winrt::implements<ActivationFactoryGetter, wf::IGetActivationFactory>
{
    wf::IActivationFactory GetActivationFactory(winrt::hstring const& classId) noexcept
    {
        return wf::IActivationFactory{winrt_get_activation_factory(classId), winrt::take_ownership_from_abi};
    }
};

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*pCmdLine*/, int /*nCmdShow*/)
{
    winrt::init_apartment();
    wamc::CoreApplication::RunWithActivationFactories(winrt::make<ActivationFactoryGetter>());
}
