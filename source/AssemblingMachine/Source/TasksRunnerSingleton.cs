using System.IO;
using System.Threading;
using System.Threading.Tasks;

namespace TaskExecutor
{
   internal class TasksRunnerSingleton
   {
      private static readonly TasksRunnerSingleton _inst = new TasksRunnerSingleton();

      private readonly CancellationTokenSource _runnerCancelTokenSrc = new CancellationTokenSource();
      private readonly Task _tasksRunner;

      private ITask _taskProc;

      private TasksRunnerSingleton()
      {
         Logger.Log.Debug("Tasks runner: creation");
         _tasksRunner =  new Task(tasksRunner, _runnerCancelTokenSrc.Token);
      }

      public static TasksRunnerSingleton Inst() => _inst;

      public void Start()
      {
         Logger.Log.Debug("Tasks runner: start runner");
         _tasksRunner.Start();
      }

      public void Stop()
      {
         Logger.Log.Debug("Tasks runner: stop runner");
         cancelTask();
         _runnerCancelTokenSrc.Cancel();
      }

      public TaskDataInternal RunningTask => isRunningTask ? _taskProc.Data : null;

      public bool CancelRunningTask() => cancelTask();

      private void tasksRunner()
      {
         Logger.Log.Debug("Tasks runner: start");
         while (true)
         {
            Thread.Sleep(100);
            if ( !TasksQueueSingleton.Inst().GetNext(out TaskDataInternal task) )
               continue;

            if (needCancelTask(task.Tag))
            {
               TasksQueueSingleton.Inst().UpdateStatus(task.Guid, TaskStatus.Canceled);
               continue;
            }

            TasksQueueSingleton.Inst().UpdateStatus(task.Guid, TaskStatus.Running);

            string ext = Path.GetExtension(task.Exec)?.ToLower() ?? string.Empty;
            switch (ext)
            {
               case "":
               case ".exe":
                  _taskProc = new TaskProcess(task);
                  TasksQueueSingleton.Inst().UpdateStatus(task.Guid, _taskProc.Run());
                  Logger.Log.Debug($"Tasks runner: {task} - {task.StatusString()}");
                  break;
               default:
                  TasksQueueSingleton.Inst().UpdateStatus(task.Guid, TaskStatus.Failed);
                  Logger.Log.Error($"Tasks runner: {task} unsupported command:\n{task.CommandLineString()}");
                  break;
            }
         }
      }

      private bool cancelTask()
      {
         Logger.Log.Debug("Tasks runner: cancel current task");
         if (!isRunningTask)
            return true;

         _taskProc.Cancel();
         return TasksQueueSingleton.Inst().UpdateStatus(_taskProc.Data.Guid, TaskStatus.Canceled);
      }

      private bool needCancelTask(string tag)
      {
         if (_taskProc == null)
            return false;
         if (_taskProc.Data.Tag != tag)
            return false;
         return _taskProc.Data.Status == TaskStatus.Failed || _taskProc.Data.Status == TaskStatus.Canceled;
      }

      private bool isRunningTask => _taskProc?.IsRunning ?? false;
   }
}
