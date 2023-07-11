#pragma once
#include "Apis.h"
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlbase.h>
#include <atlwin.h>
#include <atlctrls.h>
class CPopSysMenu
{
public:
	void Show(BOOL bShow);
	static CPopSysMenu* getInstance() {
		if (mSelf == NULL) {
			mSelf = new CPopSysMenu();
		}
		return mSelf;
	}
	QWORD DoCmd(CMD_ARGS args);
	static QWORD DoCmdCloseWindow(CMD_ARGS args);
	static QWORD DoCmdHideWindow(CMD_ARGS args);
	static QWORD DoCmdDisable(CMD_ARGS args);
	static QWORD DoCmdEnable(CMD_ARGS args);

	static INT_PTR PopSysMenuProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL m_isShow;
	BOOL IsShow();
	HWND m_hWnd;
	void Init(const string& strList);
private:
	string m_curlang;
	string m_menu_list;
	CALL_TABLE m_PopSysMenuCallTable;
	static CPopSysMenu *mSelf;
	CPopSysMenu();

};

