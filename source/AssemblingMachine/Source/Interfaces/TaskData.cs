using System;
using System.Runtime.Serialization;

namespace TaskExecutor
{
   [DataContract(Namespace = "http://TaskExecutor.TaskExecutor")]
   public class TaskData
   {
      [DataMember(Name = "guid")]
      public Guid? Guid;

      [DataMember(Name = "name")]
      public string Name;

      [DataMember(Name = "tag")]
      public string Tag;

      [DataMember(Name = "exec")]
      public string Exec;

      [DataMember(Name = "args")]
      public string[] Args;

      [DataMember(Name = "status")]
      public string Status;

      [DataMember(Name = "time_added")]
      public string AddTimeUtc;

      [DataMember(Name = "time_started")]
      public string StartTimeUtc;

      [DataMember(Name = "duration")]
      public int? Duration;

      [DataMember(Name = "estimation")]
      public int? Estimation;

      [DataMember(Name = "progress_total")]
      public int? ProgressTotal;

      [DataMember(Name = "progress_current")]
      public int? ProgressCurrent;
   }
}
