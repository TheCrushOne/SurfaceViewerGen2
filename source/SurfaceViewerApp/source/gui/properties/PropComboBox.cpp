#include "stdafx.h"
#include "PropComboBox.h"


PropComboBox::PropComboBox()
{
   m_bAutoComplete = TRUE;
}

PropComboBox::~PropComboBox()
{
}


BEGIN_MESSAGE_MAP(PropComboBox, CComboBox)
   ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
END_MESSAGE_MAP()


BOOL PropComboBox::PreTranslateMessage(MSG* pMsg)
{
   if (pMsg->message == WM_KEYDOWN)
   {
      m_bAutoComplete = TRUE;

      int nVirtKey = (int)pMsg->wParam;
      if (nVirtKey == VK_DELETE || nVirtKey == VK_BACK)
         m_bAutoComplete = FALSE;
   }

   return CComboBox::PreTranslateMessage(pMsg);
}

void PropComboBox::OnEditUpdate()
{
   CString str;
   GetWindowText(str);
   int nLength = str.GetLength();

   DWORD dwCurSel = GetEditSel();
   WORD dStart = LOWORD(dwCurSel);
   WORD dEnd = HIWORD(dwCurSel);

   std::string s_str(str);
   prepareComboBox(s_str.size() ? s_str.c_str() : nullptr);

   //if (!m_bAutoComplete)
      //return;

   if (SelectString(-1, str) == CB_ERR)
   {
      SetWindowText(str);
      if (dwCurSel != CB_ERR)
         SetEditSel(dStart, dEnd);
   }

   if (dEnd < nLength && dwCurSel != CB_ERR)
      SetEditSel(dStart, dEnd);
   else
      SetEditSel(nLength, -1);
}

void PropComboBox::ResetContent()
{
   CComboBox::ResetContent();
}


int PropComboBox::AddStringVector(std::vector<std::pair<size_t, std::string>> &&data)
{
   m_data.swap(data);
   prepareComboBox(nullptr);
   return 0;
}


void PropComboBox::prepareComboBox(const char* filter)
{
   ResetContent();
   LockWindowUpdate();
   for (const auto& elem : m_data)
   {
      std::string title = "ID: " + std::to_string(elem.first) + " " + elem.second;
      if ((!filter) || (title.find(filter) != std::string::npos))
      {
         CComboBox::AddString(title.c_str());
         CComboBox::SetItemData(CComboBox::GetCount() - 1, elem.first);
      }
   }
   UnlockWindowUpdate();
}