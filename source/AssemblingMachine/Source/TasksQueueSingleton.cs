using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Collections.Generic;
using System.Runtime.Serialization.Json;

namespace TaskExecutor
{
   using TasksList = List<TaskDataInternal>;

   internal class TasksQueueSingleton
   {
      private static readonly TasksQueueSingleton _inst = new TasksQueueSingleton();

      private readonly object _lock = new object();

      private readonly string _tasksPath = Path.Combine(SettingsAccessor.TasksDir, "tasks.json");
      private readonly TasksList _tasks;

      private TasksQueueSingleton()
      {
         Logger.Log.Debug("Tasks queue: creation");
         _tasks = loadJson<TasksList>(_tasksPath) ?? new TasksList();
         // Восстанавливаем исполняемый файл и аргументы
         _tasks.ForEach( t => t.FromTaskData(loadTaskData(t.DataPath)) );
         // Считаем ожидаемое время выполнения для задач с одинковым именем
         calcTasksEstimationTime();
      }

      public static TasksQueueSingleton Inst() => _inst;

      public Guid? Add(TaskDataInternal task)
      {
         Logger.Log.Debug("Tasks queue: add task");
         lock (_lock)
         {
            task.AddTimeUtc = DateTime.UtcNow;
            if ( !dumpTaskData(task) )
               return null;

            _tasks.Add(task);
            dumpJson(_tasks, _tasksPath);
            return task.Guid;
         }
      }

      public TaskDataInternal Get(Guid? guid)
      {
         lock (_lock)
            return getTask(guid);
      }

      public bool GetNext(out TaskDataInternal task)
      {
         lock (_lock)
         {

            task = _tasks.FirstOrDefault(t => t.Status == TaskStatus.Pending);
            return task != null;
         }
      }

      public bool UpdateStatus(Guid? guid, TaskStatus stat)
      {
         lock (_lock)
         {
            Logger.Log.Debug("Tasks queue: update task status");
            TaskDataInternal task = getTask(guid);

            switch (stat)
            {
               case TaskStatus.Running:
                  task.StartTimeUtc = DateTime.UtcNow;
                  break;
               case TaskStatus.Done:
                  Debug.Assert(task.StartTimeUtc.HasValue);
                  task.Duration = (int?)DateTime.UtcNow.Subtract(task.StartTimeUtc ?? DateTime.UtcNow).TotalSeconds;
                  break;
            }
            calcTasksEstimationTime();

            task.Status = stat;
            dumpTaskData(task);
            dumpJson(_tasks, _tasksPath);

            Logger.Log.Debug($"Tasks queue: {task} status updated");
            return true;
         }
      }

      public TaskDataInternal[] GetTasks(string tag = null)
      {
         Logger.Log.Debug("Tasks queue: getting tasks");
         lock (_lock)
            return string.IsNullOrEmpty(tag) ? _tasks.ToArray() : _tasks.Where(t => t.Tag == tag).ToArray();
      }

      private TaskDataInternal getTask(Guid? guid)
      {
         Logger.Log.Debug($"Tasks queue: get task with guid '{guid}'");
         if ( !guid.HasValue )
         {
            Logger.Log.Error("Tasks queue: trying to get task with null guid");
            return null;
         }

         TaskDataInternal task = _tasks?.FirstOrDefault(t => t.Guid == guid);
         if (task == null)
            Logger.Log.Error($"Tasks queue: task with guid '{guid}' not found");
         return task;
      }

      private static TaskData loadTaskData(string taskDataPath)
      {
         try
         {
            Logger.Log.Debug($"Tasks queue: task data loading from '{taskDataPath}'");
            if ( File.Exists(taskDataPath) )
               return loadJson<TaskData>(taskDataPath);
            Logger.Log.Error($"Tasks queue: task data doesn't exist '{taskDataPath}'");
            return null;
         }
         catch (Exception e)
         {
            Logger.Log.Error($"Tasks queue: task data loading failed! Exception: {e}");
            return null;
         }
      }

      private void calcTasksEstimationTime()
      {
         var groupedByNames = from task in _tasks
                              group task by task.Name into newGroup
                              orderby newGroup.Key
                              select newGroup;

         foreach (var group in groupedByNames)
         {
            int? durationSum = group.Where(t => t.Duration.HasValue).Sum(t => t.Duration);
            if ( !durationSum.HasValue || durationSum == 0 )
               continue;

            foreach (TaskDataInternal task in group)
               task.Estimation = durationSum / group.Count();
         }
      }

      private static bool dumpTaskData(TaskDataInternal task)
      {
         try
         {
            Logger.Log.Debug($"Tasks queue: {task} data prepare");
            
            if ( !Directory.Exists(task.TaskDir) )
               Directory.CreateDirectory(task.TaskDir);

            return dumpJson(task.ToTaskData(), task.DataPath);
         }
         catch (Exception e)
         {
            Logger.Log.Error($"Tasks queue: {task} data preparation failed! Exception: {e}");
            return false;
         }
      }

      private static T loadJson<T>(string path)
      {
         Logger.Log.Debug($"Tasks queue: loading from '{path}'");

         if ( !File.Exists(path) )
         {
            Logger.Log.Debug($"Tasks queue: '{path}' doesn't exist!");
            return default(T);
         }

         T data;
         try
         {
            using (Stream stream = File.OpenRead(path))
            {
               if (stream.Length == 0)
               {
                  Logger.Log.Debug($"Tasks queue: '{path}' is empty!");
                  return default(T);
               }

               var serializer = new DataContractJsonSerializer(typeof(T));
               data = (T)serializer.ReadObject(stream);
            }
         }
         catch (Exception e)
         {
            Logger.Log.Error($"Tasks queue: loading failed! Exception: {e}");
            return default(T);
         }

         Logger.Log.Debug("Tasks queue: loading done");
         return data;
      }

      private static bool dumpJson<T>(T data, string path)
      {
         Logger.Log.Debug($"Tasks queue: dumping to '{path}'");
         try
         {
            using (Stream stream = File.Open(path, FileMode.Create))
            {
               using (var writer = JsonReaderWriterFactory.CreateJsonWriter(stream, Encoding.UTF8, true, true, "  "))
               {
                  var serializer = new DataContractJsonSerializer(typeof(T));
                  serializer.WriteObject(writer, data);
                  writer.Flush();
               }
            }
         }
         catch (Exception e)
         {
            Logger.Log.Error($"Tasks queue: dumping failed! Exception: {e}");
            return false;
         }
         Logger.Log.Debug("Tasks queue: dumping done");
         return true;
      }
   }
}
