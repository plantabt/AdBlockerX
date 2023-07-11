#include "stdafx.h"
#include "AboutDlg.h"

#include "Apis.h"

#define HOME_PAGE "https://www.plt-labs.com"
CAboutDlg::CAboutDlg()
{
}


CAboutDlg::~CAboutDlg()
{
}

LRESULT CAboutDlg::OnClose(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	EndDialog(0);
	return 0;
}

LRESULT CAboutDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(0);
	return 0;
}

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_home_page = GetDlgItem(IDC_LAB_HOME_PAGE);
	m_home_page.SetWindowText(HOME_PAGE);
	m_home_page.SetLinkUri(HOME_PAGE);
	m_label_aboutinfo = GetDlgItem(IDC_LAB_ABOUT);
	string exePath = APIS::Sys.PROCESS.GetCurrentAppFullPath();
	string caption = APIS::Sys.RES.GetFileVersionItem(exePath, APIS::Sys.RES.VER_INTERNAL_NAME) + " " + APIS::Sys.RES.GetApplicationVersion();
	caption += "\nDeveloped by Plan T Labs \n(c) Copyright 2023";
	m_label_aboutinfo.SetWindowText(caption.c_str());
	
	return 0;
}
