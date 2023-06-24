using ABI.Windows.Foundation;
using ABI.WinRT.Interop;
using AppExtensionCSharp;
using Windows.ApplicationModel.Core;
using Windows.Foundation;
using Windows.Graphics.Capture;
using WinRT.Interop;

internal static class Program
{
    public static void Main()
    {
        CoreApplication.RunWithActivationFactories(new ActivationFactoryGetter());
    }
}

internal class ActivationFactoryGetter : IGetActivationFactory
{
    public object GetActivationFactory(string activatableClassId)
    {
        return WinRT.Module.GetActivationFactory(activatableClassId);
    }
}
