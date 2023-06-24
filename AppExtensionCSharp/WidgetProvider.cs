using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AppExtensionCSharp
{
    public sealed class WidgetProvider : SDK.IWidgetProvider
    {
        public WidgetProvider() { }

        public string SayHello()
        {
            return "Greeting from C# Extension!";
        }
    }
}
