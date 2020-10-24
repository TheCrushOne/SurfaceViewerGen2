#pragma once

#include "colreg/Communicator.h"
#include <unordered_map>

namespace SV
{
   const static std::unordered_map<ICommunicator::MessageType, const char*> typeMap =
   {
      { ICommunicator::MessageType::MT_INFO, "INFO: " },
      { ICommunicator::MessageType::MT_ERROR, "ERROR: " },
      { ICommunicator::MessageType::MT_WARNING, "WARNING: " },
      { ICommunicator::MessageType::MT_DEBUG, "DEBUG: " },
      { ICommunicator::MessageType::MT_OUTPUT, "OUTPUT: " },
      { ICommunicator::MessageType::MT_PERFORMANCE, "PERFORMANCE: " },

      { ICommunicator::MessageType::MT_EndProgress, "END_PROGRESS: "},
      { ICommunicator::MessageType::MT_Progress, "PROGRESS: " },
      { ICommunicator::MessageType::MT_InProgress, "IN_PROGRESS: " },
      { ICommunicator::MessageType::MT_AbortSupport, "ABORT_SUPPORT: " },
   };

   class ConsoleCommunicator : public ICommunicator
   {
   public:
      bool stdCallConv Message(ICommunicator::MessageType t, const char* msg, ...) override final;
      void stdCallConv SetProgress(unsigned int progress) override final;
      bool stdCallConv GetTerminateFlag() override final { return true; }
      bool stdCallConv UpdateUI() override final { return true; }
      bool stdCallConv RaiseError() override final { return true; }
      bool stdCallConv OnPathFound() override final { return true; }
   };
}