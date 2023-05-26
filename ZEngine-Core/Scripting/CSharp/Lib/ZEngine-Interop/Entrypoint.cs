using System;
using System.Runtime.InteropServices;

namespace ZEngine.Core.Interop
{
    public class Entrypoint
    {
        /**
         * This is the entrypoint for the assembly. It is required to be present in order to load the assembly.
         */
        [UnmanagedCallersOnly]
        public static void Initialize()
        {
            // Empty stub, required to load the assembly.
        }
    }
}
