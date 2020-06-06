using System;
using System.Linq;
using System.Reflection;
using System.ServiceModel;
using System.ServiceProcess;
using System.ComponentModel;
using System.Configuration.Install;

namespace TaskExecutor
{
   public class TaskExecutorWinService : ServiceBase
   {
      private ServiceHost _serviceHost;

      public TaskExecutorWinService()
      {
         ServiceName = "Task Executor";
      }

      public static int Main(params string[] args)
      {
         try
         {
            if (args.Any())
            {
               if (args.Contains("-debug"))
                  runConsole();
               else if (args.Contains("-u"))
                  ManagedInstallerClass.InstallHelper(new[] { "/u", Assembly.GetExecutingAssembly().Location });
               else if (args.Contains("-i"))
                  ManagedInstallerClass.InstallHelper(new[] { Assembly.GetExecutingAssembly().Location });
            }
            else
               runService();
         }
         catch (Exception)
         {
            return 1;
         }
         return 0;
      }

      protected override void OnStart(string[] args)
      {
         _serviceHost?.Close();
         _serviceHost = new ServiceHost(typeof(TaskExecutorService));
         _serviceHost.Open();

         TasksRunnerSingleton.Inst().Start();
      }

      protected override void OnStop()
      {
         if (_serviceHost == null)
            return;
         _serviceHost.Close();
         _serviceHost = null;

         TasksRunnerSingleton.Inst().Stop();
      }

      private static void runService() => Run(new TaskExecutorWinService());

      private static void runConsole()
      {
         using (var serviceHost = new ServiceHost(typeof(TaskExecutorService)))
         {
            try
            {
               serviceHost.Open();
               TasksRunnerSingleton.Inst().Start();

               Console.WriteLine("The service is ready.");
               Console.WriteLine("Press <ENTER> to terminate service.");
               Console.ReadLine();

               serviceHost.Close();
            }
            catch (System.TimeoutException timeProblem)
            {
               Console.WriteLine(timeProblem.Message);
               Console.ReadLine();
            }
            catch (CommunicationException commProblem)
            {
               Console.WriteLine(commProblem.Message);
               Console.ReadLine();
            }
         }
      }
   }

   [RunInstaller(true)]
   public class ProjectInstaller : Installer
   {
      private readonly ServiceProcessInstaller _process;
      private readonly ServiceInstaller _service;

      public ProjectInstaller()
      {
         _process = new ServiceProcessInstaller { Account = ServiceAccount.LocalSystem };
         _service = new ServiceInstaller { ServiceName = "Task Executor" };
         Installers.Add(_process);
         Installers.Add(_service);
      }
   }
}
