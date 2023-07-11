#include "CStaticHref.h"

LRESULT CStaticHref::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = m_hWnd;
	TrackMouseEvent(&tme);
	bMouseIn = true;
	Invalidate();

	return 0;
}

LRESULT CStaticHref::OnMouseDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	//ShellExecute(NULL, "open", "explorer.exe", ("/select,\"" + filePath + "\"").c_str(), NULL, SW_SHOWNORMAL);
	ShellExecute(NULL, "open", m_link_url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	return 0;
}
LRESULT CStaticHref::OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	bMouseIn = false;
	Invalidate();
	return 0;
}
LRESULT CStaticHref::OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// 阻止系统设置默认的鼠标样式

	::SetCursor(::LoadCursor(NULL, IDC_HAND));
	return TRUE;
}


LRESULT CStaticHref::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// 返回非零值，表示已擦除背景




	CDC dc;
	dc.Attach(reinterpret_cast<HDC>(wParam));

	// 填充整个客户区域
	RECT rect;
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, GetSysColor(COLOR_WINDOW));
	
	return 1;
}
LRESULT CStaticHref::OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(&ps);

	// 创建 CDC 对象
	CDC dc;
	dc.Attach(hdc);

	// 设置绘制的文本颜色为蓝色
	//dc.SetTextColor(RGB(0, 0, 255));

	// 绘制背景
	RECT rect;
	GetClientRect(&rect);
	
	//dc.FillSolidRect(&rect, GetBkColor(dc));
	dc.FillSolidRect(&rect, GetSysColor(COLOR_MENU));
	//dc.InvertRect(&rect);



	HPEN b = CreatePen(PS_SOLID, 1,RGB(0, 0, 255));
	dc.SelectPen(b);

	// 选择字体到设备上下文
	HFONT hOldFont = dc.SelectFont(GetFont());

	if (bMouseIn) {
		dc.SetTextColor(RGB(0, 0, 255));
		dc.MoveTo(rect.left, rect.bottom-2);
		dc.LineTo(rect.right, rect.bottom-2);
	}
	else{
		dc.SetTextColor(RGB(0, 0, 255));
	}

	// 获取控件文本
	TCHAR strText[200];
	GetWindowText(strText, sizeof(strText));
	
	dc.SetBkMode(TRANSPARENT);
	
	dc.DrawText(strText, -1, &rect, DT_SINGLELINE | DT_CENTER);

	dc.SelectFont(hOldFont);
	// 释放 CDC 对象
	dc.Detach();

	// 结束绘制
	EndPaint(&ps);
	DeleteObject(b);
	// 绘制文本
	//DrawText(dc, strStr.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	return 0;
}
void CStaticHref::SetLinkUri(const string& uri) {
	m_link_url = uri;
}
