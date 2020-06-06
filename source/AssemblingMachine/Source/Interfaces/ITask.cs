namespace TaskExecutor
{
   internal interface ITask
   {
      TaskStatus Run();
      void Cancel();

      TaskDataInternal Data { get; }

      bool IsRunning { get; }
   }
}
