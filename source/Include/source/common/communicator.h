#pragma once

#include <stdio.h>
#include <wchar.h>
#include <cstdlib>
#include <cstdarg>

#ifdef _WIN32
#pragma warning (disable: 4996)
#define stdCallConv __stdcall
#endif
#if defined(__clang__) || defined(__GNUC__)
#define stdCallConv
#endif


struct ICommunicator
{
   enum MessageType
   {
      MS_Info = 0,
      MS_Error = 1,
      MS_Warning = 2,
      MS_Debug = 3,
      // MS_Progress и MS_InProgress могут комбинироваться с MS_AbortSupport
      MS_EndProgress = 4,
      MS_Progress = 8,
      MS_InProgress = 16,
      MS_AbortSupport = 32
   };
   virtual bool stdCallConv Message(MessageType t, const char* msg) = 0;
   virtual void stdCallConv SetProgress(unsigned int progress) = 0;  // 0..100
   virtual bool stdCallConv GetTerminateFlag() = 0;
   virtual bool stdCallConv UpdateUI() = 0;
};

#ifdef _WIN32
MIDL_INTERFACE("1C2DA8BB-EDA2-42cc-954A-643F47AC9832")
ICommunicable : IUnknown
{
   virtual bool stdCallConv SetCommunicator(ICommunicator * pCommunicator) = 0;
};


_COM_SMARTPTR_TYPEDEF(ICommunicable, __uuidof(ICommunicable));

namespace Communicator
{
   inline bool SetCommunicatorTo(IUnknown* pTo, ICommunicator* pCommunicator)
   {
      ICommunicablePtr pCommunication(pTo);

      if (pCommunication)
         return pCommunication->SetCommunicator(pCommunicator);
      return false;
   }
}

#endif

inline bool CommunicatorMessage(ICommunicator* pCommunicator, ICommunicator::MessageType t, const char* msg, va_list ap)
{
   if (pCommunicator == nullptr)
      return false;

   int const bufSize = 4096;

   static char buf[bufSize];
   int const res = vsnprintf(buf, bufSize, msg, ap);

   if (res == -1)
      buf[bufSize - 1] = '\0';

   return pCommunicator->Message(t, buf);
}

inline bool CommunicatorMessage(ICommunicator* pCommunicator, ICommunicator::MessageType t, const wchar_t* msg, va_list ap)
{
   if (pCommunicator == nullptr)
      return false;

   int const bufSize = 4096;

   wchar_t buf[bufSize];
   int const res = vswprintf(buf, bufSize, msg, ap);

   if (res == -1)
      buf[bufSize - 1] = '\0';

   char mbsBuf[bufSize];
   //Possible lose of part of the string in case of non ANSI symbols
   wcstombs(mbsBuf, buf, bufSize);

   return pCommunicator->Message(t, mbsBuf);
}

inline bool CommunicatorGeometryMessage(ICommunicator* pCommunicator, ICommunicator::MessageType t, double x, double y, const char* msg, va_list ap)
{
   if (pCommunicator == nullptr)
      return false;

   char msgBuf[1024];
   sprintf(msgBuf, "<GeometryLink x=%f y=%f />%s", x, y, msg);

   return CommunicatorMessage(pCommunicator, t, msgBuf, ap);
}

#pragma warning( push )
#pragma warning (disable : 4793 ) // function compiled as native

template <class TString>
bool CommunicatorMessageString(ICommunicator* pCommunicator, ICommunicator::MessageType t, TString msg, ...)
{
   va_list ap;
   va_start(ap, msg);

   bool result = CommunicatorMessage(pCommunicator, t, msg, ap);

   va_end(ap);

   return result;
}

inline bool CommunicatorGeometryMessageString(ICommunicator* pCommunicator, ICommunicator::MessageType t, double x, double y, const char* msg, ...)
{
   if (pCommunicator == nullptr)
      return false;

   va_list ap;
   va_start(ap, msg);

   bool result = CommunicatorGeometryMessage(pCommunicator, t, x, y, msg, ap);

   va_end(ap);

   return result;
}
#pragma warning( pop )


// Wrapper class for ICommunicator
class CCommunicable
{
protected:
   // This class is designed only for inheritance ( not for creating objects )
   CCommunicable(ICommunicator* comm = nullptr) : _comm(comm) {}

   bool stdCallConv GeometryMessageStringInternal(ICommunicator::MessageType t, double x, double y, const char* msg, va_list ap) const
   {
      return CommunicatorGeometryMessage(_comm, t, x, y, msg, ap);
   }

#pragma warning( push )
#pragma warning (disable : 4793 ) // function compiled as native
   template<class TString>
   bool stdCallConv MessageString(ICommunicator::MessageType t, TString msg, ...) const
   {
      va_list ap;
      va_start(ap, msg);

      bool result = CommunicatorMessage(_comm, t, msg, ap);

      va_end(ap);

      return result;
   }

#define INT_DECLARE_ALIAS_TO(alias, messageType) \
   template<class TString> \
   bool stdCallConv alias(TString msg, ...) const \
   { \
      va_list ap; \
      va_start(ap, msg); \
      bool result = CommunicatorMessage(_comm, messageType, msg, ap); \
      va_end(ap); \
      return result; \
   }

   INT_DECLARE_ALIAS_TO(MsgDbg, ICommunicator::MS_Debug)
      INT_DECLARE_ALIAS_TO(MsgInf, ICommunicator::MS_Info)
      INT_DECLARE_ALIAS_TO(MsgWrn, ICommunicator::MS_Warning)
      INT_DECLARE_ALIAS_TO(MsgErr, ICommunicator::MS_Error)

      bool stdCallConv GeometryMessageString(ICommunicator::MessageType t, unsigned int id, float x, float y, const char* msg, ...) const
   {
      if (_comm == nullptr)
         return false;

      char msgBuf[1024];
      sprintf(msgBuf, "<GeometryLink id=%u x=%f y=%f />%s", id, x, y, msg);
      va_list ap;
      va_start(ap, msg);

      bool result = CommunicatorMessage(_comm, t, msgBuf, ap);

      va_end(ap);

      return result;
   }

   bool stdCallConv GeometryMessageString(ICommunicator::MessageType t, float x, float y, const char* msg, ...) const
   {
      if (_comm == nullptr)
         return false;

      va_list ap;
      va_start(ap, msg);

      bool result = GeometryMessageStringInternal(t, x, y, msg, ap);

      va_end(ap);

      return result;
   }

   bool stdCallConv GeometryMessageString(ICommunicator::MessageType t, unsigned int id, const char* msg, ...) const
   {
      if (_comm == nullptr)
         return false;

      char msgBuf[1024];
      sprintf(msgBuf, "<GeometryLink id=%u />%s", id, msg);

      va_list ap;
      va_start(ap, msg);

      bool result = CommunicatorMessage(_comm, t, msgBuf, ap);

      va_end(ap);

      return result;
   }

   bool UpdateUI() const
   {
      return _comm == nullptr ? false : _comm->UpdateUI();
   }

   bool GetTerminateFlag() const
   {
      return _comm == nullptr ? false : _comm->GetTerminateFlag();
   }

   void Progress(unsigned int progress) const
   {
      if (_comm == nullptr)
         return;

      _comm->SetProgress(progress);
   }

   void SetCommunicator(ICommunicator* pCommunicator)
   {
      _comm = pCommunicator;
   }

   ICommunicator* GetCommunicator() const
   {
      return _comm;
   }

#ifdef _WIN32
   void PassCommunicator(IUnknown* pTo) const
   {
      Communicator::SetCommunicatorTo(pTo, GetCommunicator());
   }


   bool ErrorString(const _com_error& e, LPCSTR msg) const
   {
      return MessageString(ICommunicator::MS_Error, "Error : %s (%s : 0x%08x)", msg, e.ErrorMessage(), e.Error());
   }
   bool ErrorString(HRESULT hr, LPCSTR msg, ...) const
   {
      va_list ap;
      va_start(ap, msg);
      int const bufSize = 1024;
      char buf[bufSize];
      int const res = _vsnprintf_s(buf, bufSize, msg, ap);
      if (res == -1)
         buf[bufSize - 1] = '\0';
      va_end(ap);

      _com_error e(hr);
      return MessageString(ICommunicator::MS_Error, "Error : %s (%s : 0x%08x)", res, e.ErrorMessage(), e.Error());
   }

   bool VerifyMsg(HRESULT hr, LPCSTR msg, ...) const
   {
      if (SUCCEEDED(hr))
         return true;

      va_list ap;
      va_start(ap, msg);
      int const bufSize = 1024;
      char buf[bufSize];
      int const res = _vsnprintf_s(buf, bufSize, msg, ap);
      if (res == -1)
         buf[bufSize - 1] = '\0';
      va_end(ap);

      _com_error e(hr);
      MessageString(ICommunicator::MS_Error, "Error : %s (%s : 0x%08x)", buf, e.ErrorMessage(), e.Error());
      return false;
   }
   bool VerifyMsg(bool ok, LPCSTR msg, ...) const
   {
      if (ok)
         return true;

      va_list ap;
      va_start(ap, msg);
      int const bufSize = 1024;
      char buf[bufSize];
      int const result = _vsnprintf_s(buf, bufSize, msg, ap);
      if (result == -1)
         buf[bufSize - 1] = '\0';
      va_end(ap);

      MessageString(ICommunicator::MS_Error, buf);
      return false;
   }
#pragma warning( pop )

#endif

private:
   ICommunicator* _comm;
};

// класс с возможностью внешней установки коммуникатора
// используется только в некомовских объектах
class CCommunicableSimple
   : public CCommunicable
{
public:
   void stdCallConv SetCommunicator(ICommunicator* pCommunicator)
   {
      CCommunicable::SetCommunicator(pCommunicator);
   }
};

#ifdef _WIN32
// используется в COM объектах
class CCommunicableImpl
   : public CCommunicable
   , public ICommunicable
{
   // ICommunicable
private:
   bool stdCallConv SetCommunicator(ICommunicator* pCommunicator)
   {
      CCommunicable::SetCommunicator(pCommunicator);
      return true;
   }
};

// класс используется когда сам класс является CCommunicable, а также нужно устанавливать коммуникатор во внутренние классы
// для внутренних классов нужно вызвать SetCommunicator(this)
template <typename CCommunicableType>
class CCommunicatorDelegate
   : public CCommunicableType
   , public ICommunicator
{
protected:
   bool stdCallConv Message(MessageType t, const char* msg)
   {
      if (GetCommunicator() != nullptr)
         return GetCommunicator()->Message(t, msg);
      return false;
   }
   void stdCallConv SetProgress(unsigned int progress)
   {
      if (GetCommunicator() != nullptr)
         GetCommunicator()->SetProgress(progress);
   }
   bool stdCallConv GetTerminateFlag()
   {
      if (GetCommunicator() != nullptr)
         return GetCommunicator()->GetTerminateFlag();
      return false;
   }
   bool stdCallConv UpdateUI()
   {
      if (GetCommunicator() != nullptr)
         return GetCommunicator()->UpdateUI();
      return false;
   }
};

typedef CCommunicatorDelegate<CCommunicableSimple>    CCommunicableSimpleExt;
typedef CCommunicatorDelegate<CCommunicableImpl>      CCommunicableImplExt;

#endif
