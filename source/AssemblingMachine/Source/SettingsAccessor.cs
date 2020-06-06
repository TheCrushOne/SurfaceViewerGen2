using System;
using System.Configuration;

namespace TaskExecutor
{
   internal class SettingsAccessor
   {
      public const string TasksDirTag = "TasksDir";

      public static string TasksDir
      {
         get
         {
            try
            {
               return ConfigurationManager.AppSettings[TasksDirTag];
            }
            catch (Exception e)
            {
               Logger.Log.Error($"Settings accessor: failed to get output file tasks output directory! Exception: {e}");
            }
            return string.Empty;
         }
      }
   }
}
