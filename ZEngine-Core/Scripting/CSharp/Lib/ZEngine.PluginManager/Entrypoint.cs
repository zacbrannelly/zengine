using System;
using System.Runtime.InteropServices;

namespace ZEngine.Core.PluginManager
{
    public class Entrypoint
    {
        /**
         * This is the entrypoint for the assembly. It is required to be present in order to load the assembly.
         * WARNING: Do not remove this method or assembly load will fail.
         */
        [UnmanagedCallersOnly]
        public static void Initialize()
        {
            System.Runtime.GCSettings.LatencyMode = System.Runtime.GCLatencyMode.SustainedLowLatency;
        }
    }
}
