using log4net;
using log4net.Config;

namespace TaskExecutor
{
   internal static class Logger
   {
      public static ILog Log { get; } = LogManager.GetLogger("LOGGER");
   }
}
