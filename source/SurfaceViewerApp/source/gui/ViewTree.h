
#pragma once

/////////////////////////////////////////////////////////////////////////////
// Окно CViewTree
namespace SV
{
   class CViewTree : public CTreeCtrl
   {
      // Создание
   public:
      CViewTree() noexcept;

      // Переопределение
   protected:
      virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

      // Реализация
   public:
      virtual ~CViewTree();

   protected:
      DECLARE_MESSAGE_MAP()
   };
}