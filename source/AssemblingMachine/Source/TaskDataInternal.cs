using System;
using System.IO;
using System.Text;
using System.Runtime.Serialization;

namespace TaskExecutor
{
   public enum TaskStatus
   {
      None,
      Pending,
      Running,
      Done,
      Canceled,
      Failed
   }

   [DataContract]
   public class TaskDataInternal
   {
      private TaskStatus _status;
      private string _statusStr;

      public TaskDataInternal(Guid guid, TaskData data)
      {
         Guid = guid;
         Name = data.Name;
         Tag  = data.Tag;
         Exec = data.Exec;
         Args = data.Args;

         _status = TaskStatus.Pending;
         _statusStr = StatusString();

         TaskDir = string.IsNullOrEmpty(Tag)
            ? Path.Combine(SettingsAccessor.TasksDir, "data", $"{Guid}")
            : Path.Combine(SettingsAccessor.TasksDir, "data", Tag, $"{Guid}");
      }

      [DataMember(Name = "guid")]
      public Guid Guid;
      [DataMember(Name = "name")]
      public string Name;
      [DataMember(Name = "tag")]
      public string Tag;

      public string   Exec;
      public string[] Args;

      public TaskStatus Status
      {
         get => _status;
         set
         {
            _status = value;
            _statusStr = StatusString();
         }
      }

      [DataMember(Name = "status")]
      public string StatusStr
      {
         get => _statusStr;
         set
         {
            _statusStr = value;
            _status = FromStatusString();
         }
      }

      [DataMember(Name = "time_added")]
      public DateTime? AddTimeUtc;

      [DataMember(Name = "time_started")]
      public DateTime? StartTimeUtc;

      [DataMember(Name = "duration")]
      public int? Duration;

      public int? Estimation;

      public int? ProgressTotal;
      public int? ProgressCurrent;

      [DataMember(Name = "dir")]
      public string TaskDir;

      public string DataPath   => Path.Combine(TaskDir, "data.json");
      public string OutputPath => Path.Combine(TaskDir, "output.txt");

      public override string ToString()
      {
         var builder = new StringBuilder();
         builder.Append($"task '{Name}'");
         builder.Append(string.IsNullOrEmpty(Tag) ? $" ({Guid})" : $" ({Guid} {Tag})");
         return builder.ToString();
      }

      public void FromTaskData(TaskData data)
      {
         if (data == null)
            return;
         Exec = data.Exec;
         Args = data.Args;
      }

      public TaskData ToTaskData() => new TaskData
      {
         Guid = Guid, Name = Name, Tag = Tag,
         Exec = Exec, Args = Args,
         Status = StatusString(),
         AddTimeUtc = AddTimeUtc?.ToString("o"), StartTimeUtc = StartTimeUtc?.ToString("o"),
         Duration = Duration, Estimation = Estimation, ProgressCurrent = ProgressCurrent, ProgressTotal = ProgressTotal
      };

      public string CommandLineString() => $"{Exec} {string.Join(" ", Args)}";

      public string StatusString()
      {
         switch (Status)
         {
            case TaskStatus.Pending:  return "pending";
            case TaskStatus.Running:  return "running";
            case TaskStatus.Done:     return "done";
            case TaskStatus.Canceled: return "canceled";
            case TaskStatus.Failed:   return "failed";
         }
         return string.Empty;
      }

      public TaskStatus FromStatusString()
      {
         switch (StatusStr)
         {
            case "pending":  return TaskStatus.Pending;
            case "running":  return TaskStatus.Running;
            case "done":     return TaskStatus.Done;
            case "canceled": return TaskStatus.Canceled;
            case "failed":   return TaskStatus.Failed;
         }
         return TaskStatus.None;
      }
   }
}
