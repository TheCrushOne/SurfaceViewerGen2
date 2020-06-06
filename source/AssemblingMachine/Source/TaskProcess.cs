using System;
using System.IO;
using System.Threading;
using System.Diagnostics;
using System.Linq;

namespace TaskExecutor
{
   internal class TaskProcess : ITask
   {
      private readonly Process _process = new Process
      {
         StartInfo =
         {
            UseShellExecute = false,
            CreateNoWindow = true,
            RedirectStandardOutput = true,
            RedirectStandardError = true
   }
      };

      private readonly CancellationTokenSource _cancelTokenSrc = new CancellationTokenSource();

      private volatile bool _isRunning;

      public TaskProcess(TaskDataInternal data)
      {
         Logger.Log.Debug($"Task process: create new process for {data}");

         Data = data;

         _process.StartInfo.EnvironmentVariables["PYTHONUNBUFFERED"] = "1"; // if not set, output will be flushed after exit

         _process.StartInfo.FileName = Data.Exec;
         // wrap quotes around arguments with spaces
         // if that argument was not in quotes already
         _process.StartInfo.Arguments = string.Join(" ", Data.Args.Select(arg => !arg.StartsWith("\"") && arg.Contains(" ") ? $"\"{arg}\"" : arg));

         _cancelTokenSrc.Token.Register(() => _process.Kill());
      }

      public TaskDataInternal Data { get; }

      public bool IsRunning => _isRunning;

      public TaskStatus Run()
      {
         try
         {
            void OutHandler(object s, DataReceivedEventArgs e)
            {
               using (StreamWriter outputStream = getOutputStream())
               {
                  if (string.IsNullOrEmpty(e.Data))
                     return;
                  if (e.Data.Contains("$progress$"))
                  {
                     string[] strs = e.Data.Substring(e.Data.IndexOf("$progress$")).Split(',');
                     Data.ProgressCurrent = int.Parse(strs[1]);
                     Data.ProgressTotal = int.Parse(strs[2]);
                  }  
                  else
                     outputStream?.WriteLine(e.Data);
               }
            }
            _process.OutputDataReceived += OutHandler;
            _process.ErrorDataReceived += OutHandler;

            _isRunning = true;
            _process.Start();
            Logger.Log.Debug($"Task process: start {Data} process PID {_process.Id}");

            _process.BeginOutputReadLine();
            _process.BeginErrorReadLine();
            _process.WaitForExit();

            if ( !_isRunning ) // canceled
               return TaskStatus.Canceled;

            _isRunning = false;

            Logger.Log.Debug($"Task process: {Data} process exit code {_process.ExitCode}");
            return _process.ExitCode == 0 ? TaskStatus.Done : TaskStatus.Failed;
         }
         catch (Exception e)
         {
            Logger.Log.Error($"Task process: {Data} failed on start! Exception: {e}");
            return TaskStatus.Failed;
         }
      }

      public void Cancel()
      {
         _isRunning = false;
         _cancelTokenSrc.Cancel();
      }

      private StreamWriter getOutputStream()
      {
         StreamWriter stream = StreamWriter.Null;
         try
         {
            stream = File.AppendText(Data.OutputPath);
            stream.AutoFlush = true;
         }
         catch (Exception e)
         {
            Logger.Log.Error($"Tasks output: failed to get output file stream '{Data.OutputPath}'! Exception: {e}");
         }
         return stream;
      }
   }
}
