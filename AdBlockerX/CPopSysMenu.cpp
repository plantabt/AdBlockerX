
#include "resource.h"
#include "CPopSysMenu.h"
#define STR_POPMENU "STR_POPMENU"
CPopSysMenu* CPopSysMenu::mSelf = NULL;

CPopSysMenu::CPopSysMenu() {
    m_isShow = false;
    m_hWnd = NULL;
    //初始化PopSysMenu的功能调用
    m_PopSysMenuCallTable.insert(CALL_TABLE::value_type((BYTE)('1'), DoCmdCloseWindow));
    m_PopSysMenuCallTable.insert(CALL_TABLE::value_type((BYTE)('2'), DoCmdHideWindow));
    m_PopSysMenuCallTable.insert(CALL_TABLE::value_type((BYTE)('3'), DoCmdDisable));
    m_PopSysMenuCallTable.insert(CALL_TABLE::value_type((BYTE)('4'), DoCmdEnable));
}
QWORD CPopSysMenu::DoCmd(CMD_ARGS args) {

    int exec = 0;

    CALL_METHOD method = m_PopSysMenuCallTable[args.cmd];
    if (method) {
        method(args);
        exec = 1;
    }

    return exec;//1 执行命令，0未执行
}
QWORD CPopSysMenu::DoCmdCloseWindow(CMD_ARGS args) {
    PostMessage((HWND)(args.wparam), WM_CLOSE, 0, 0);
    return 0;
}
QWORD CPopSysMenu::DoCmdHideWindow(CMD_ARGS args) {
    //PostMessage((HWND)(args.wparam), WM_SHOWWINDOW, FALSE, 0);
    ShowWindow((HWND)(args.wparam), SW_HIDE);
    return 0;
}
QWORD CPopSysMenu::DoCmdDisable(CMD_ARGS args) {
    EnableWindow((HWND)(args.wparam), FALSE);
    //PostMessage((HWND)(args.wparam), WM_ENABLE, 0, 0);
    return 0;
}
QWORD CPopSysMenu::DoCmdEnable(CMD_ARGS args) {
    EnableWindow((HWND)(args.wparam), 1);
    //PostMessage((HWND)(args.wparam), WM_ENABLE, 1, 0);
    return 0;
}
void CPopSysMenu::Init(const string& curlang) {
	m_curlang = curlang;
}
BOOL CPopSysMenu::IsShow() {

    return m_isShow;

}

bool AdjustWindowPosition(HWND hWnd) {
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
	if (point.x + windowWidth + 12 >= screenRect.right)
	{
		x = screenRect.right - windowWidth;
		y = point.y +  4;

		//MoveWindow(hWnd, x, y, width, height, TRUE);
		isAdjust = true;
	}
	if (point.y + windowHeight >= screenRect.bottom) {
		x = x == 0 ? point.x + 14 : x;
		y = screenRect.bottom - windowHeight;
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
		//MoveWindow(hWnd, x, y, windowWidth, windowHeight, TRUE);
		SetWindowPos(hWnd, HWND_TOPMOST, x, y, 0, 0, SWP_SHOWWINDOW || SWP_NOSIZE);
	}
	return isAdjust;
}

void CPopSysMenu::Show(BOOL bShow) {
    if (m_hWnd == NULL) {
        m_hWnd = ::CreateDialog(NULL, MAKEINTRESOURCE(IDD_SYS_MENU), 0, PopSysMenuProc);

		::SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, ::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
    }
	string str = CConfig::GetValue(m_curlang, STR_POPMENU);
	SetDlgItemText(m_hWnd, IDC_LAB_MENU_LIST, str.c_str());
    m_isShow = bShow;
    if(bShow){
        POINT point;
        RECT rect;
        GetCursorPos(&point);
        GetWindowRect(m_hWnd,&rect);
		if (!AdjustWindowPosition(m_hWnd)) {
			SetWindowPos(m_hWnd, HWND_TOPMOST, point.x + 12, point.y + 4, 0, 0, SWP_SHOWWINDOW || SWP_NOSIZE);
		}
		/*
		if (!::IsWindowVisible(m_hWnd)) {
			SetWindowPos(m_hWnd, HWND_TOPMOST, point.x + 12, point.y + 4, 0, 0, SWP_SHOWWINDOW || SWP_NOSIZE);
		}else{
			MoveWindow(m_hWnd,  point.x + 12, point.y + 4, 0, 0,TRUE);
		}
		*/
		ShowWindow(m_hWnd, SW_SHOW);
    }else{
        ShowWindow(m_hWnd, SW_HIDE);
    }
}

INT_PTR CPopSysMenu::PopSysMenuProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
   // RECT rect;
    switch (msg) {
    case WM_INITDIALOG:

        SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 180, LWA_ALPHA);

        break;
    case WM_TIMER:
        //switch (wParam) {
        //case ID_TIP_TIMER:
        //    break;
        //}
        break;
    case WM_SIZE:
        // GetWindowRect(hWnd, &rect);
        // SetWindowPos(GetDlgItem(hWnd, IDC_TIP_TEXT), NULL, 8, 8, rect.right-rect.left, rect.bottom- rect.top, SWP_SHOWWINDOW);
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
//CALL_METHOD iter = g_PopSysMenuCallTable[(BYTE)'X'];
//iter(1);