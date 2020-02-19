#pragma once
#include "stdafx.h"


class PropComboBox : public CComboBox
{
   // Construction
public:
   PropComboBox();

   // Attributes
public:

   // Operations
public:
   void ResetContent();
   int AddStringVector(std::vector<std::pair<size_t, std::string>> &&data);

   
public:
   virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
public:
   virtual ~PropComboBox();

   BOOL m_bAutoComplete;

   // Generated message map functions
protected:
   afx_msg void OnEditUpdate();

   void prepareComboBox(const char* filter);

   DECLARE_MESSAGE_MAP()
private:
   std::vector<std::pair<size_t, std::string>> m_data;
};