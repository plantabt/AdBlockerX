#pragma once
#include "atlwin.h"
#include "resource.h"
#include "CStaticHref.h"

class CAboutDlg :	public CAxDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTBOX};

	BEGIN_MSG_MAP(CMainDlg)
	
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
	END_MSG_MAP()
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	CAboutDlg();
	~CAboutDlg();
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	CStatic m_label_aboutinfo;
	CStaticHref m_home_page;
};

