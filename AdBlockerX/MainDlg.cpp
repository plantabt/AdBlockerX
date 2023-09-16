// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "CPopSysMenu.h"
#include "Config.h"
#include "AboutDlg.h"
#include "ComboLanguage.h"
#include <winuser.h>
#include <iostream>
#include <vector>
#include <fstream>



#define COMMONSET "COMMON"
#define ZH_CN "ZH_CN"
#define EN_US "EN_US"
#define CONFIG_FILE "config.ini"
#define CURRENT_LANG "CURR_LANGUAGE"
#define STR_CAPTURE_FINFO "STR_CAPTURE_FINFO"
#define STR_SET_CAPTURE "STR_SET_CAPTURE"
#define STR_SET_POPMENU "STR_SET_POPMENU"
#define STR_SETTING_BTN "STR_SETTING_BTN"
#define CAPTURE_HOTKEY "CAPTURE_HOTKEY"
#define POPMENU_HOTKEY "POPMENU_HOTKEY"
#define STR_BTN_HIDE "STR_BTN_HIDE"
#define MWND_HOTKEY "MWND_HOTKEY"
#define STR_SET_MWND "STR_SET_MWND"
#define KEYS_POPMENU "KEYS_POPMENU"
#define KEYS_CAPTURE "KEYS_CAPTURE"
#define KEYS_MWND "KEYS_MWND"
#define KEYS_ESC "KEYS_ESC"
#define STR_ADD_STARTUP "STR_ADD_STARTUP"
#define TXT_OK "OK"
#define STARTUP_NAME "STARTUP_ADBLOCKER"
#define CHECKED_STARTUP "CHECKED_STARTUP"

#define APP_NAME "ADBLOCKX_PLANTLABS"
#define UPGRADE_INFO "Upgrade to v%s"
#define SCAN_TIMER 100

#define PLTLABS_WEBSITE _T("https://www.plt-labs.com")
//检查更新
#define CHECK_UPDATE_URL _T("https://www.plt-labs.com/chkblkx")

/*
#if _DEBUG
#define CHECK_UPDATE_URL _T("http://127.0.0.1/chkblkx")
#else

#endif
*/
#if _DEBUG
#define LOGINFO(Info,br)  APIS::Files.LogInfo("c:\\adblx.log",Info,br)
#else
#define LOGINFO(Info,br) 
#endif
string CMainDlg::m_curr_lang = "";
TCHAR lpSelExePath[MAX_PATH] = { NULL };
HWND g_catched_window = NULL;
HWND g_mainWindow = NULL;
BOOL g_showTip = FALSE;
HHOOK  gMouseHook=NULL;
HWND g_tipDlg=NULL;
INT_PTR TipDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam);
void TipWindow();
LRESULT WINAPI ThreadUpdateProc(DWORD arg);
CComboBox m_comboLan;
CMainDlg *gMainDlg = NULL;
string g_tip_fmt = "";

static DWORD heartbeat = 1, oldheartbeat = 0;
bool IsMsg = false;


LRESULT CMainDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRet = CAxDialogImpl<CMainDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	APIS::Sys.PROCESS.EnableDebugPrivilege();

	gMainDlg = this;

	// center the dialog on the screen
	CenterWindow();
	
	g_mainWindow = m_hWnd;
	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);
	g_tipDlg = ::CreateDialog(NULL, MAKEINTRESOURCE(IDD_TIP_WINDOW), 0, TipDlgProc);

	::SetWindowLongPtr(g_tipDlg, GWL_EXSTYLE, ::GetWindowLongPtr(g_tipDlg, GWL_EXSTYLE) | WS_EX_TOOLWINDOW );//524288
#ifndef _DEBUG
	gMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, GetModuleHandle(0), 0);
#else
	//gMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, GetModuleHandle(0), 0);
#endif
	
	
	string exePath = APIS::Sys.PROCESS.GetCurrentAppFullPath();
	string caption = APIS::Sys.RES.GetFileVersionItem(exePath, APIS::Sys.RES.VER_INTERNAL_NAME)+" " + APIS::Sys.RES.GetApplicationVersion(); //APIS::Sys.RES.LoadStringFromResource(ID_VERSION);
	if (APIS::Sys.SYSTEM.IsAnotherInstanceRunning(APP_NAME)) {
		TCHAR classn[MAX_PATH] = { NULL };
		GetClassName(m_hWnd, classn, sizeof(classn));
		APIS::Sys.WINDOW.ActivateWindow(classn, caption);
		TerminateProcess(GetCurrentProcess(), 0);
	}
	SetWindowText(caption.c_str());
	// 初始化自定义组合框
	m_comboLan = GetDlgItem(IDC_COMBO_LANGUAGE);
	m_label_capture = GetDlgItem(IDC_CATCH_HOTKEY);
	m_label_popmenu = GetDlgItem(IDC_CATCH_HOTKEY2);
	m_label_capture_key = GetDlgItem(IDC_LAB_CATCH_KEY);
	m_label_popmenu_key = GetDlgItem(IDC_LAB_POPMENU_KEY);
	m_btn_popmenu_set = GetDlgItem(IDC_BTN_SET_POPMENU_KEY);
	m_btn_capture_set = GetDlgItem(IDC_BTN_SET_CAPTURE_KEY);
	m_btn_hide = GetDlgItem(ID_BTN_HIDE);
	m_label_mwnd_key = GetDlgItem(IDC_LAB_MWND_KEY);
	m_label_mwnd = GetDlgItem(IDC_LAB_MWND);
	m_btn_mwnd_set = GetDlgItem(IDC_BTN_SET_MWND_KEY);
	m_chk_add_startup = GetDlgItem(IDC_CHK_ADD_STARTUP);

	m_label_upgrade = GetDlgItem(IDC_HREF_NEW_VER);
	m_label_upgrade.SetLinkUri(PLTLABS_WEBSITE);


	string	cfgPath = APIS::Sys.SYSTEM.GetCurrentDir() ;

	cfgPath = APIS::Strings.Format("%s%s", cfgPath.c_str(), CONFIG_FILE);
	LOGINFO("cfgPath:" + cfgPath, 1);

	APIS::Config.Init(cfgPath);


	ReadConfig();
	if (m_chk_add_startup.GetCheck()) {
		
		//APIS::Sys.WINDOW.HideWindow(m_hWnd);
		ShowWindow(SW_HIDE);
	}
	else {
		ShowWindow(SW_SHOW);
	}
	SetTimer(SCAN_TIMER, 5000, NULL);

	DragAcceptFiles(TRUE);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadUpdateProc, 0, 0, 0);
	return lRet;
}
CMainDlg::CMainDlg(){
	

	g_tipDlg = NULL;

}
CMainDlg::~CMainDlg() {
	//UnregisterHotKey(NULL, 1);
	//UnhookWindowsHookEx(hkbdHook);
	UnhookWindowsHookEx(gMouseHook);
}

LRESULT WINAPI ThreadUpdateProc(DWORD arg) {
	string version = APIS::Sys.RES.GetApplicationVersion();
	string exePath = APIS::Sys.PROCESS.GetCurrentAppFullPath();

	string internalname = APIS::Sys.RES.GetFileVersionItem(exePath,APIS::Sys.RES.VER_INTERNAL_NAME);// GetInternalName();
	string agent = internalname +" "+ version;
	string pageInfo = APIS::Net.GetWebPage(CHECK_UPDATE_URL, agent);
	if (pageInfo == "")return 0;
	vector<string> pageItem = APIS::Strings.Split(pageInfo, ";");
	string new_ver = pageItem[0];
	if (version != new_ver) {
		new_ver = APIS::Strings.Format(UPGRADE_INFO, new_ver.c_str());
		gMainDlg->m_label_upgrade.SetWindowText(new_ver.c_str());
		gMainDlg->m_label_upgrade.ShowWindow(SW_SHOW);
		return 0;
	}
	gMainDlg->m_label_upgrade.ShowWindow(SW_HIDE);
	return 0;
}
void CMainDlg::ReadConfig() {
	// 添加选项
	

	LOGINFO("-------------------SetHotKeys Begin!-------------------", 1);
	string curLang = APIS::Config.GetValue(COMMONSET, CURRENT_LANG);
	LOGINFO("CURRENT_LANG:" + curLang, 1);
	for (int cnt = 0; cnt < 10; cnt++) {
		string langi = APIS::Strings.Format("LNG%d", cnt);
		string ln = APIS::Config.GetValue(COMMONSET, langi);
		if (ln == "")break;
		m_comboLan.DeleteString(cnt);
		m_comboLan.InsertString(cnt, ln.c_str());
	}
	int curli = APIS::Strings.Str2Long(curLang, 10);

	CMainDlg::m_curr_lang = APIS::Strings.Format("LNG_SEC%d", curli);
	m_curr_lang = APIS::Config.GetValue(COMMONSET, m_curr_lang);
	m_comboLan.SetCurSel(curli);
	
	string str = APIS::Config.GetValue(m_curr_lang, STR_SET_CAPTURE);
	m_label_capture.SetWindowText(str.c_str());

	str = APIS::Config.GetValue(m_curr_lang, STR_ADD_STARTUP);
	m_chk_add_startup.SetWindowText(str.c_str());

	str = APIS::Config.GetValue(COMMONSET, CHECKED_STARTUP);
	m_chk_add_startup.SetCheck(APIS::Strings.Str2Long(str, 10));

	str = APIS::Config.GetValue(m_curr_lang, STR_SET_POPMENU);
	LOGINFO("STR_SET_POPMENU:" + str, 1);
	m_label_popmenu.SetWindowText(str.c_str());
	str = APIS::Config.GetValue(m_curr_lang, STR_SETTING_BTN);

	m_btn_popmenu_set.SetWindowText(str.c_str());
	m_btn_capture_set.SetWindowText(str.c_str());

	str = APIS::Config.GetValue(m_curr_lang, STR_BTN_HIDE);
	m_btn_hide.SetWindowText(str.c_str());

	str = APIS::Config.GetValue(COMMONSET, CAPTURE_HOTKEY);
	LOGINFO("CAPTURE_HOTKEY:" + str, 1);
	if (str == "") {
		str = "LALT+W";
	}
	m_label_capture_key.SetWindowText(str.c_str());
	APIS::Keybd.SetHotKeys(m_hWnd, KEYS_CAPTURE, CTRL_HOT_KEY_CAPTURE, str);



	str = APIS::Config.GetValue(COMMONSET, POPMENU_HOTKEY);
	LOGINFO("POPMENU_HOTKEY:" + str, 1);
	if (str == "") {
		str = "LALT+X";
	}
	m_label_popmenu_key.SetWindowText(str.c_str());
	APIS::Keybd.SetHotKeys(m_hWnd, KEYS_POPMENU, CTRL_HOT_KEY_POPMENU, str);

	str = APIS::Config.GetValue(COMMONSET, MWND_HOTKEY);
	LOGINFO("MWND_HOTKEY:" + str, 1);
	if (str == "") {
		str = "LALT+M";
	}
	APIS::Keybd.SetHotKeys(m_hWnd, KEYS_MWND, CTRL_HOT_KEY_MWND, str);
	m_label_mwnd_key.SetWindowText(str.c_str());

	str = APIS::Config.GetValue(m_curr_lang, STR_SET_MWND);
	m_label_mwnd.SetWindowText(str.c_str());
	str = APIS::Config.GetValue(m_curr_lang, STR_SETTING_BTN);
	m_btn_mwnd_set.SetWindowText(str.c_str());




	APIS::Keybd.SetHotKeys(m_hWnd, KEYS_ESC, CTRL_HOT_KEY_ESC, "ESC");
	APIS::Keybd.SetHotKeys(m_hWnd, "KEYS_K", CTRL_HOT_KEY_K, "K");
	APIS::Keybd.SetHotKeys(m_hWnd, "KEYS_H", CTRL_HOT_KEY_H, "H");
	APIS::Keybd.SetHotKeys(m_hWnd, "KEYS_D", CTRL_HOT_KEY_D, "D");
	APIS::Keybd.SetHotKeys(m_hWnd, "KEYS_E", CTRL_HOT_KEY_E, "E");
	APIS::Keybd.SetHotKeys(m_hWnd, "KEYS_L", CTRL_HOT_KEY_L, "L");
	APIS::Keybd.SetHotKeys(m_hWnd, "KEYS_B", CTRL_HOT_KEY_B, "B");
	
	g_tip_fmt = APIS::Config.GetValue(CMainDlg::m_curr_lang, STR_CAPTURE_FINFO);
	LOGINFO("STR_CAPTURE_FINFO:" + g_tip_fmt, 1);
	CPopSysMenu::getInstance()->Init(CMainDlg::m_curr_lang);
	LOGINFO("-------------------SetHotKeys Finish!-------------------", 1);
	//APIS::Keybd.SetHotKeys(m_hWnd, "KEYS_6", CTRL_HOT_KEY_6, "6");
}


// 监听鼠标移动消息的回调函数
LRESULT CALLBACK MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	heartbeat++;
	if (nCode >= 0 && wParam == WM_MOUSEMOVE)
	{
		
		TipWindow();

	}else if (CPopSysMenu::getInstance()->IsShow()) {

		CPopSysMenu::getInstance()->Show(!CPopSysMenu::getInstance()->IsShow());
		return 1;
	}

	// 调用下一个钩子
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}


HWND GetCursorPosWindow() {
	HWND targetWindow = nullptr;
	HWND currentWindow = NULL;
	POINT cursorPos = { NULL };
	GetCursorPos(&cursorPos);


	currentWindow = WindowFromPoint(cursorPos);
	RECT rect = { 0 };
	POINT point = { 0 };

	//先检查子窗口
	HWND childWindow = currentWindow;

	do
	{
		childWindow = GetWindow(childWindow, GW_CHILD);

		GetWindowRect(childWindow, &rect);
		if (PtInRect(&rect, cursorPos)) {
			return childWindow;
		}
		// 获取下一个子窗口
				
	} while(childWindow != NULL);
	return currentWindow;
	/*
	do
	{
		targetWindow = FindWindowEx(currentWindow, nullptr, nullptr, nullptr);
		if (targetWindow != nullptr)
		{
			// 检查鼠标指针是否在窗口的客户区域内
			RECT windowRect;
			GetWindowRect(targetWindow, &windowRect);
			MapWindowPoints(targetWindow, nullptr, reinterpret_cast<POINT*>(&windowRect), 2);
			GetCursorPos(&cursorPos);
			if (PtInRect(&windowRect, cursorPos))
			{
				// 找到指向的窗口句柄
				return targetWindow;
			}
		}
		currentWindow = targetWindow;
	} while (targetWindow != nullptr);

	return NULL;
	*/
}
bool AdjustWindowPosition(HWND hWnd,int width,int height) {
	// 获取屏幕信息
	bool isAdjust = false;
	RECT screenRect;
	POINT point = { 0 };
	SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
	GetCursorPos(&point);
	// 获取窗口信息
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);
	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	// 窗口右侧超出屏幕右侧，则调整窗口位置
	int x = 0, y = 0;
	if (point.x+ width +12 >= screenRect.right)
	{
		x = screenRect.right - width;
		y = point.y - windowHeight - 4;

		//MoveWindow(hWnd, x, y, width, height, TRUE);
		isAdjust = true;
	}
	if (point.y - height <= 0) {
		x = x == 0 ? point.x + 14 : x;
		y = point.y + 14;
		isAdjust = true;
	}
	// 窗口顶部超出屏幕顶部，则调整窗口位置
	/*
	if (point.y- height <= 0)
	{
		//int newTop = screenRect.top - height;
		MoveWindow(hWnd, x == 0 ? point.x + 12: x, point.y + 12, width, height, TRUE);
		isAdjust = true;
	}
	*/
	if (isAdjust) {
		MoveWindow(hWnd, x, y, width, height, TRUE);
	}
	return isAdjust;
}

void SetTipWindowText() {
	DWORD pid;
	TCHAR lpOutBuff[1024] = { NULL };
	TCHAR lpTitle[MAX_PATH * 2] = { NULL };
	TCHAR lpClassname[MAX_PATH * 2] = { NULL };
	GetWindowText(g_catched_window, lpTitle, sizeof(lpTitle));
	GetClassName(g_catched_window, lpClassname, sizeof(lpClassname));
	GetWindowThreadProcessId(g_catched_window, &pid);
	APIS::Sys.PROCESS.GetProcessFilePath(pid, lpSelExePath, sizeof(lpSelExePath), false);

	wsprintf(lpOutBuff, g_tip_fmt.c_str(), (DWORD)g_catched_window, (DWORD)g_catched_window, pid, pid, lpTitle, lpClassname, lpSelExePath);
	SetDlgItemText(g_tipDlg, IDC_TIP_TEXT, lpOutBuff);
}
void TipWindow() {
	static bool isTopmost = false;

	static HWND oldHwnd = NULL;
	
	POINT point;
	
	SIZE txSize = { NULL };
	
	if (g_showTip) {
		GetCursorPos(&point);
		g_catched_window = GetCursorPosWindow();
		HWND rootWnd = APIS::Sys.WINDOW.GetRootWindow(g_catched_window);
		if ((rootWnd == g_mainWindow) || (rootWnd == CPopSysMenu::getInstance()->m_hWnd) || (rootWnd == g_tipDlg) || GetCurrentProcessId() == APIS::Sys.PROCESS.GetProcessIdFromWindow(g_catched_window)) {
			g_catched_window = oldHwnd;
		}
		else {
			oldHwnd = g_catched_window;
		}
		APIS::Sys.DC.GetCtrlTextSize(g_tipDlg, IDC_TIP_TEXT, &txSize);
		
		if ((APIS::Sys.WINDOW.GetRootWindow(APIS::Sys.WINDOW.GetTopmostWindow())!=g_mainWindow) && (APIS::Sys.WINDOW.GetTopmostWindow() != g_tipDlg)) {
			if (!AdjustWindowPosition(g_tipDlg, txSize.cx, txSize.cy)) {
				SetWindowPos(g_tipDlg, HWND_TOPMOST, point.x + 14, point.y - txSize.cy, txSize.cx, txSize.cy - 4, SWP_SHOWWINDOW);
			}
		}
		if (!::IsWindowVisible(g_tipDlg)) {
			SetWindowPos(g_tipDlg, HWND_TOPMOST, point.x + 14, point.y - txSize.cy, txSize.cx, txSize.cy - 4, SWP_SHOWWINDOW);
		}
		/*

		if ((rootWnd == g_tipDlg) || (rootWnd == CPopSysMenu::getInstance()->m_hWnd)) {
			
			if(!AdjustWindowPosition(g_tipDlg, txSize.cx, txSize.cy)){
				::MoveWindow(g_tipDlg, point.x + 22, point.y - txSize.cy, txSize.cx, txSize.cy - 4, TRUE);
			}
			
			return;
		}
		*/
		//DeleteDC(hdc);
		SetTipWindowText();

		
		if (!AdjustWindowPosition(g_tipDlg,txSize.cx, txSize.cy)) {
			::MoveWindow(g_tipDlg, point.x + 14, point.y - txSize.cy, txSize.cx, txSize.cy - 4, TRUE);
		}
		
	}
	else {
		ShowWindow(g_tipDlg, SW_HIDE);
		//SetWindowPos(g_tipDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_HIDEWINDOW);
	}
}



LRESULT CMainDlg::OnClose(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	bool oldgtp = g_showTip;
	g_showTip = false;
	if(MessageBox("Are u sure?", "Exit?", MB_OKCANCEL | MB_ICONQUESTION)==1){
		PostQuitMessage(0);
	}
	
	g_showTip = oldgtp;
	
	return 0;
}
bool IsWindowFullyVisible(HWND hWnd)
{
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWnd, &wp);

	BOOL isVisible = IsWindowVisible(hWnd);
	bool isFullyVisible = (wp.showCmd == SW_SHOW && isVisible);

	return isVisible;
}
void WeakupScan() {
	if (oldheartbeat == heartbeat) {
		if (gMouseHook != NULL) {
			UnhookWindowsHookEx(gMouseHook);
		}
		gMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, GetModuleHandle(0), 0);
	}
	oldheartbeat = heartbeat;
}
LRESULT CMainDlg::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	TCHAR dropPath[MAX_PATH] = { 0 };
	DragQueryFile((HDROP)wParam, 0, dropPath, sizeof(dropPath));
	//解锁oep
	if (APIS::PeEngine.LockOEP(dropPath, FALSE)) {
		MessageBox("Restore success!!!", "恢复成功!", MB_OK | MB_ICONINFORMATION);
	}
	DragFinish((HDROP)wParam);
	
	return true;
}
LRESULT CMainDlg::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	
	if(wParam== SCAN_TIMER){
		LOGINFO("-------------------SCAN_TIMER Begin-------------------", 1);
		APIS::Keybd.WeakupScan();
		WeakupScan();
		LOGINFO("-------------------SCAN_TIMER End!-------------------", 1);
	}
	return true;
}
LRESULT CMainDlg::OnComboSelChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if (wID == IDC_COMBO_LANGUAGE) {
		//TCHAR seltxt[100] = { NULL };
		//m_comboLan.GetLBText(m_comboLan.GetCurSel(), seltxt);

		APIS::Config.SetValue(COMMONSET, CURRENT_LANG, to_string(m_comboLan.GetCurSel()));
		ReadConfig();
		return true;
	}
	return true;
}
LRESULT CMainDlg::OnHotKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if (wParam == 1){
		g_showTip = !g_showTip;
		TipWindow();
	}
	if (wParam == 2 && g_showTip) {
		//static BOOL showMenu = false;
		//showMenu = !showMenu;
		CPopSysMenu::getInstance()->Show(!CPopSysMenu::getInstance()->IsShow());
	}
	return true;
}
INT_PTR TipDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	RECT rect;
	switch (msg) {
	case WM_INITDIALOG:

		SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 220, LWA_ALPHA);

		break;
	case WM_TIMER:

		break;
	case WM_SIZE:
		GetWindowRect(hWnd, &rect);
		SetWindowPos(GetDlgItem(hWnd, IDC_TIP_TEXT), NULL, 4, 8, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
		break;
	case WM_SYSCOMMAND:
		break;
	case WM_COMMAND:

		break;
	case WM_CLOSE:

		break;
	}

	return 0;
}

#define CANCEL_SET 0xFFFF1111

LRESULT CMainDlg::OnBnClickedBtnSetCapturehKey(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	TCHAR str[MAX_PATH] = { NULL };
	m_btn_capture_set.GetWindowText(str,sizeof(str));
	string is = str;
	if (is == TXT_OK) {

		string val = APIS::Config.GetValue(m_curr_lang, STR_SETTING_BTN);
		//SetDlgItemText(IDC_BTN_SET_CAPTURE_KEY, val.c_str());
		m_btn_capture_set.SetWindowText(val.c_str());
		m_btn_popmenu_set.EnableWindow(TRUE);
		m_btn_mwnd_set.EnableWindow(TRUE);
		m_btn_hide.EnableWindow(TRUE);
		if ((DWORD)hWndCtl == CANCEL_SET) {
			val = APIS::Config.GetValue(COMMONSET, CAPTURE_HOTKEY);
			m_label_capture_key.SetWindowText(val.c_str());
		}
		else {
			m_label_capture_key.GetWindowText(str, sizeof(str));
			APIS::Keybd.SetHotKeys(m_hWnd, KEYS_CAPTURE, CTRL_HOT_KEY_CAPTURE, str);
			APIS::Config.SetValue(COMMONSET, CAPTURE_HOTKEY, str);
		}


	}
	else {

		m_btn_capture_set.SetWindowText(TXT_OK);
		m_btn_popmenu_set.EnableWindow(FALSE);
		m_btn_mwnd_set.EnableWindow(FALSE);
		m_btn_hide.EnableWindow(FALSE);
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnSetPopmenuKey(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
	TCHAR str[MAX_PATH] = { NULL };
	m_btn_popmenu_set.GetWindowText(str, sizeof(str));
	string is = str;
	if (is == TXT_OK) {
		string val = APIS::Config.GetValue(m_curr_lang, STR_SETTING_BTN);
		m_btn_popmenu_set.SetWindowText(val.c_str());
		m_btn_capture_set.EnableWindow(TRUE);
		m_btn_mwnd_set.EnableWindow(TRUE);
		m_btn_hide.EnableWindow(TRUE);
		if ((DWORD)hWndCtl == CANCEL_SET) {
			val = APIS::Config.GetValue(COMMONSET, POPMENU_HOTKEY);
			m_label_popmenu_key.SetWindowText(val.c_str());
		}
		else {
			m_label_popmenu_key.GetWindowText(str, sizeof(str));
			APIS::Keybd.SetHotKeys(m_hWnd, KEYS_POPMENU, CTRL_HOT_KEY_POPMENU, str);
			APIS::Config.SetValue(COMMONSET, POPMENU_HOTKEY, str);
		}
		
	}
	else {

		m_btn_popmenu_set.SetWindowText(TXT_OK);
		m_btn_capture_set.EnableWindow(FALSE);
		m_btn_mwnd_set.EnableWindow(FALSE);
		m_btn_hide.EnableWindow(FALSE);
	}
	return 0;
}
LRESULT CMainDlg::OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	/*
	POINT pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);

	RECT rc;
	
	m_label_upgrade.GetWindowRect(&rc);
	ScreenToClient(&rc);

	if (PtInRect(&rc,pt))
	{
		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		return TRUE;
	}
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	*/
	return FALSE;
}
LRESULT CMainDlg::OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	/*
	if ((HWND)lParam == GetDlgItem(IDC_HREF_NEW_VER))
	{
		// 设置文本颜色为蓝色
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(0, 0, 255));

		// 设置背景透明
		SetBkMode(hdcStatic, TRANSPARENT);

		// 返回背景刷子
		return (LRESULT)GetStockObject(NULL_BRUSH);
	}

	bHandled = FALSE;
	*/
	return 0;
}

LRESULT CALLBACK BlockThreadProc(DWORD arg) {
	IsMsg = true;
	bool oldgtp = g_showTip;
	g_showTip = false;
	string info = "确定禁用程序吗?\nAre u sure?\n";
	info += lpSelExePath;
	if (MessageBox(0,info.c_str(), "禁用程序 BlockApp?", MB_OKCANCEL | MB_ICONWARNING) == 1) {

		DWORD pid = APIS::Sys.PROCESS.GetProcessIdFromWindow(g_catched_window);
		APIS::Sys.PROCESS.KillProcess(pid);
		if (APIS::PeEngine.LockOEP(lpSelExePath, TRUE)) {
			MessageBox(0, "Success!!!", "禁用成功!", MB_OK | MB_ICONINFORMATION);
		}
		
	}
	g_showTip = oldgtp;
	IsMsg = false;
	return 0;
}
LRESULT CMainDlg::OnHotkeyLL(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	LRESULT ret = 0;
	if(wParam==-1){
		string keys = "";
		string title_capture = "", title_popmenu = "", title_mwnd="";
		TCHAR tmp[MAX_PATH] = { NULL };
		m_btn_capture_set.GetWindowText(tmp, sizeof(tmp));
		title_capture = tmp;

		m_btn_popmenu_set.GetWindowText(tmp, sizeof(tmp));
		title_popmenu = tmp;


		m_btn_mwnd_set.GetWindowText(tmp, sizeof(tmp));
		title_mwnd = tmp;

		CKeybd::HOT_KEY_MAP keyMap = APIS::Keybd.GetCurrentKeys();

		for (const auto& entry : keyMap)
		{
			keys == "" ? keys = entry.second.second : keys += "+" + entry.second.second;
		}


		if (title_capture == TXT_OK) {
			ret = 1;
			m_label_capture_key.SetWindowText(keys.c_str());
			
			return 1;
		}

		if (title_popmenu == TXT_OK) {
			ret = 1;
			m_label_popmenu_key.SetWindowText(keys.c_str());
			return 1;
		}

		if (title_mwnd == TXT_OK) {
			ret = 1;
			m_label_mwnd_key.SetWindowText(keys.c_str());
			return 1;
		}
	}else if (wParam == CTRL_HOT_KEY_CAPTURE) {
		LOGINFO("CTRL_HOT_KEY_CAPTURE", 1);
		ret = 1;
		g_showTip = !g_showTip;
		g_catched_window = GetCursorPosWindow();
		SetTipWindowText();
		TipWindow();
		
	}else if (wParam == CTRL_HOT_KEY_POPMENU) {
		LOGINFO("CTRL_HOT_KEY_POPMENU", 1);
		if(g_showTip){
			ret = 1;
			CPopSysMenu::getInstance()->Show(!CPopSysMenu::getInstance()->IsShow());

		}
	}
	else if (wParam == CTRL_HOT_KEY_MWND) {
		LOGINFO("CTRL_HOT_KEY_MWND", 1);
		ret = 1;
		IsWindowVisible() ? ShowWindow(SW_HIDE) : APIS::Sys.WINDOW.ActivateWindow(m_hWnd);
		
	}
	else if (wParam == CTRL_HOT_KEY_ESC) {
		CPopSysMenu::getInstance()->Show(false);

		//取消热键输入

		TCHAR str[MAX_PATH] = { NULL };
		CMainDlg::m_btn_capture_set.GetWindowText(str, sizeof(str));
		string is = str;
		if (is == TXT_OK) {
			ret = 1;
			SendMessage(m_hWnd, WM_COMMAND, IDC_BTN_SET_CAPTURE_KEY, CANCEL_SET);
		}

		CMainDlg::m_btn_popmenu_set.GetWindowText(str, sizeof(str));
		is = str;
		if (is == TXT_OK) {
			ret = 1;
			SendMessage(m_hWnd, WM_COMMAND, IDC_BTN_SET_POPMENU_KEY, CANCEL_SET);
		}

		CMainDlg::m_btn_mwnd_set.GetWindowText(str, sizeof(str));
		is = str;
		if (is == TXT_OK) {
			ret = 1;
			SendMessage(m_hWnd, WM_COMMAND, IDC_BTN_SET_MWND_KEY, CANCEL_SET);
		}

	}else if (wParam == CTRL_HOT_KEY_K) {
		if (CPopSysMenu::getInstance()->IsShow()) {
			ret = 1;
			if (APIS::Sys.PROCESS.GetProcessIdFromWindow(g_catched_window) == GetCurrentProcessId() || (APIS::Sys.WINDOW.GetRootWindow(g_catched_window) == g_tipDlg)) {
				g_catched_window = NULL;
				MessageBeep(0);
				
			}
			else {
				DWORD pid = APIS::Sys.PROCESS.GetProcessIdFromWindow(g_catched_window);
				APIS::Sys.PROCESS.KillProcess(pid);
				CPopSysMenu::getInstance()->Show(false);
			}

		}
	}else if (wParam == CTRL_HOT_KEY_H) {
		if (CPopSysMenu::getInstance()->IsShow()) {
			ret = 1;
			::ShowWindow(g_catched_window, SW_HIDE);
			CPopSysMenu::getInstance()->Show(false);
		}
	}else if (wParam == CTRL_HOT_KEY_D) {
		if (CPopSysMenu::getInstance()->IsShow()) {
			ret = 1;
			::EnableWindow(g_catched_window,FALSE);
			CPopSysMenu::getInstance()->Show(false);
		}
	}else if (wParam == CTRL_HOT_KEY_E) {
		if (CPopSysMenu::getInstance()->IsShow()) {
			ret = 1;
			::EnableWindow(g_catched_window, TRUE);
			CPopSysMenu::getInstance()->Show(false);
		}
	}else if (wParam == CTRL_HOT_KEY_L) {
		if (CPopSysMenu::getInstance()->IsShow()) {
			ret = 1;
			CPopSysMenu::getInstance()->Show(false);
			APIS::Files.OpenFolderAndSelectFile(lpSelExePath);
		}
	}else if (wParam == CTRL_HOT_KEY_B) {
		if (CPopSysMenu::getInstance()->IsShow()) {
			ret = 1;
			CPopSysMenu::getInstance()->Show(false);
			if(!IsMsg){
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)BlockThreadProc, 0, 0, 0);
			}
		}
	}

	return ret;
}



LRESULT CMainDlg::OnBnClickedBtnHide(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_HIDE);
	return 0;
}


LRESULT CMainDlg::OnBnClickedBtnSetMwndKey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	TCHAR str[MAX_PATH] = { NULL };
	m_btn_mwnd_set.GetWindowText(str, sizeof(str));
	string is = str;
	if (is == TXT_OK) {
		string val = APIS::Config.GetValue(m_curr_lang, STR_SETTING_BTN);
		m_btn_mwnd_set.SetWindowText(val.c_str());
		m_btn_popmenu_set.EnableWindow(TRUE);
		m_btn_capture_set.EnableWindow(TRUE);
		m_btn_hide.EnableWindow(TRUE);
		if ((DWORD)hWndCtl == CANCEL_SET) {
			val = APIS::Config.GetValue(COMMONSET, MWND_HOTKEY);
			m_label_mwnd_key.SetWindowText(val.c_str());
		}
		else {
			m_label_mwnd_key.GetWindowText(str, sizeof(str));
			APIS::Keybd.SetHotKeys(m_hWnd, KEYS_MWND, CTRL_HOT_KEY_MWND, str);
			APIS::Config.SetValue(COMMONSET, MWND_HOTKEY, str);
		}

	}
	else {

		m_btn_mwnd_set.SetWindowText(TXT_OK);
		m_btn_popmenu_set.EnableWindow(FALSE);
		m_btn_capture_set.EnableWindow(FALSE);
		m_btn_hide.EnableWindow(FALSE);
	}
	return 0;
}


LRESULT CMainDlg::OnBnClickedChkAddStartup(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	
	string spath = APIS::Sys.SYSTEM.GetAppFullPath() + " \"" + APIS::Sys.SYSTEM.GetCurrentDir() + "\"";

	// TODO: 在此添加控件通知处理程序代码
	if (m_chk_add_startup.GetCheck()) {

		APIS::Sys.SYSTEM.AddToStartup(STARTUP_NAME, spath);

		APIS::Config.SetValue(COMMONSET, CHECKED_STARTUP,"1");

	}
	else {
		APIS::Sys.SYSTEM.RemoveFromStartup(STARTUP_NAME);
		APIS::Config.SetValue(COMMONSET, CHECKED_STARTUP, "0");
	}
	return 0;
}


LRESULT CMainDlg::OnBnClickedAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	bool oldgtp = g_showTip;
	g_showTip = false;
	CAboutDlg dlg;
	dlg.DoModal();
	g_showTip = oldgtp;
	return 0;
}


LRESULT CMainDlg::OnStnClickedHrefNewVer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	return 0;
}
