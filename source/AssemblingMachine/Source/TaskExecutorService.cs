using System;
using System.Linq;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace TaskExecutor
{
   public class TaskExecutorService : ITaskExecutor
   {
      public Guid? AddTask(TaskData data) =>
         TasksQueueSingleton.Inst().Add(new TaskDataInternal(Guid.NewGuid(), data));

      public TaskData[] GetTasks(string tag)
      {
         var tasks = string.IsNullOrEmpty(tag) ? TasksQueueSingleton.Inst().GetTasks() : selectByTag(tag);
         return tasks.Select(t => t.ToTaskData()).ToArray();
      }

      public TaskData GetTask(string guid) => getTask(guid)?.ToTaskData();

      public string GetTaskOutput(string guid)
      {
         TaskDataInternal task = getTask(guid);
         if (task == null || !File.Exists(task.OutputPath))
            return null;

         string output = string.Empty;
         try
         {
            output = File.ReadAllText(task.OutputPath, Encoding.UTF8);
         }
         catch (Exception e)
         {
            Logger.Log.Error($"Failed to get task output! Exception: {e}");
            return string.Empty;
         }
         return output;
      }

      public TaskData GetRunningTask() =>
         TasksQueueSingleton.Inst().Get(TasksRunnerSingleton.Inst().RunningTask?.Guid)?.ToTaskData();

      public bool Cancel(string tag, string guid)
      {
         // Должен быть либо tag либо guid
         if (string.IsNullOrEmpty(tag) && string.IsNullOrEmpty(guid))
            return false;
         if (!string.IsNullOrEmpty(tag) && !string.IsNullOrEmpty(guid))
            return false;

         if (string.IsNullOrEmpty(tag))
         {
            if ( !Guid.TryParse(guid, out Guid g) )
               return false;

            if (TasksRunnerSingleton.Inst().RunningTask?.Guid == g)
               TasksRunnerSingleton.Inst().CancelRunningTask();
            else
               setCancelStatus(TasksQueueSingleton.Inst().Get(g));
            return true;
         }

         List<TaskDataInternal> tasks = selectByTag(tag).ToList();
         tasks.ForEach(setCancelStatus);
         return TasksRunnerSingleton.Inst().RunningTask?.Tag != tag || TasksRunnerSingleton.Inst().CancelRunningTask();
      }

      private static IEnumerable<TaskDataInternal> selectByTag(string tag) =>
         from t in TasksQueueSingleton.Inst().GetTasks()
         where t.Tag == tag
         select t;

      private static TaskDataInternal getTask(string guid) =>
         Guid.TryParse(guid, out Guid g) ? TasksQueueSingleton.Inst().Get(g) : null;

      private static void setCancelStatus(TaskDataInternal task)
      {
         // Меняем только Pending, Running отменится на CancelRunningTask
         if (task != null && task.Status == TaskStatus.Pending)
            TasksQueueSingleton.Inst().UpdateStatus(task.Guid, TaskStatus.Canceled);
      }
   }
}
