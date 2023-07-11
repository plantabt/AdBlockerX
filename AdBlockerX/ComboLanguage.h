#pragma once

#include <atlctrls.h>

class CComboLanguage : public CComboBox
{
public:
	//DECLARE_WND_SUPERCLASS(_T("CComboLanguage"), CComboBox::GetWndClassName())
	
	BEGIN_MSG_MAP(CComboLanguage)
		
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
	END_MSG_MAP()

	~CComboLanguage();
	CComboLanguage();
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};