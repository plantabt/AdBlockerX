#pragma once
#include "stdafx.h"

#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atlbase.h>
#include <atlwin.h>
#include <atlctrls.h>

class CStaticHref : public CWindowImpl<CStaticHref, CStatic>
{
	
public:
	//DECLARE_WND_CLASS_EX(NULL, CS_HREDRAW | CS_VREDRAW, COLOR_WINDOW)
	DECLARE_WND_SUPERCLASS(NULL, CStatic::GetWndClassName())
	CStaticHref(HWND hWnd = NULL) :bMouseIn(false), m_link_url("")
	{

	}

	CStaticHref& operator =(CWindow cwnd)
	{
		//this->m_hWnd = hWnd;
		SubclassWindow(cwnd.m_hWnd);
		ModifyStyle(0, SS_NOTIFY);
		
		// 设置静态文本控件的样式为 SS_NOTIFY

		return *this;
	}
	
	
	BEGIN_MSG_MAP(CStaticHref)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnMouseDown)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)

		//MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)

	
		DEFAULT_REFLECTION_HANDLER()
		
	END_MSG_MAP()
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnMouseDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	

	void SetLinkUri(const string& uri);
	
private:
	string m_link_url;
	bool bMouseIn;
};
