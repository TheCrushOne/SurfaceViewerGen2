using System;
using System.ServiceModel;
using System.ServiceModel.Web;

namespace TaskExecutor
{
   [ServiceContract(Namespace = "http://TaskExecutor.TaskExecutor")]
   public interface ITaskExecutor
   {
      [WebInvoke(Method = "POST", UriTemplate = "tasks",
                 RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json,
                 BodyStyle = WebMessageBodyStyle.Bare)]
      [OperationContract]
      Guid? AddTask(TaskData task);

      [WebGet(UriTemplate = "tasks?tag={tag}",
              ResponseFormat = WebMessageFormat.Json)]
      [OperationContract]
      TaskData[] GetTasks(string tag);

      [WebGet(UriTemplate = "tasks/{guid}",
              ResponseFormat = WebMessageFormat.Json)]
      [OperationContract]
      TaskData GetTask(string guid);

      [WebGet(UriTemplate = "tasks/{guid}/output",
              ResponseFormat = WebMessageFormat.Json)]
      [OperationContract]
      string GetTaskOutput(string guid);

      [WebGet(UriTemplate = "tasks/running",
              ResponseFormat = WebMessageFormat.Json)]
      [OperationContract]
      TaskData GetRunningTask();

      [WebInvoke(Method = "POST", UriTemplate = "tasks/cancel",
                 RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json,
                 BodyStyle = WebMessageBodyStyle.WrappedRequest)]
      [OperationContract]
      bool Cancel(string tag, string guid);
   }
}
