// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Apis.h"
#include "CStaticHref.h"
#define  CTRL_HOT_KEY_CAPTURE WM_USER+100
#define  CTRL_HOT_KEY_POPMENU WM_USER+101
#define  CTRL_HOT_KEY_MWND WM_USER+102
#define  CTRL_HOT_KEY_ESC WM_USER+200
#define  CTRL_HOT_KEY_ESC WM_USER+200
#define  CTRL_HOT_KEY_K WM_USER+201
#define  CTRL_HOT_KEY_H WM_USER+202
#define  CTRL_HOT_KEY_D WM_USER+203
#define  CTRL_HOT_KEY_E WM_USER+204
#define  CTRL_HOT_KEY_L WM_USER+205
#define  CTRL_HOT_KEY_B WM_USER+206
class CMainDlg : public CAxDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_HOTKEY,OnHotKey)
		//MESSAGE_HANDLER(WM_SHOWWINDOW, OnShow)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_HOTKEY_LL, OnHotkeyLL)
		//MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		//MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)

		COMMAND_HANDLER(IDC_COMBO_LANGUAGE, CBN_SELCHANGE, OnComboSelChange)
		COMMAND_HANDLER(IDC_BTN_SET_CAPTURE_KEY, BN_CLICKED, OnBnClickedBtnSetCapturehKey)
		COMMAND_HANDLER(IDC_BTN_SET_POPMENU_KEY, BN_CLICKED, OnBnClickedBtnSetPopmenuKey)

		COMMAND_HANDLER(ID_BTN_HIDE, BN_CLICKED, OnBnClickedBtnHide)
		COMMAND_HANDLER(IDC_BTN_SET_MWND_KEY, BN_CLICKED, OnBnClickedBtnSetMwndKey)
		COMMAND_HANDLER(IDC_CHK_ADD_STARTUP, BN_CLICKED, OnBnClickedChkAddStartup)
		COMMAND_HANDLER(ID_APP_ABOUT, BN_CLICKED, OnBnClickedAppAbout)
		COMMAND_HANDLER(IDC_HREF_NEW_VER, STN_CLICKED, OnStnClickedHrefNewVer)

	END_MSG_MAP()
	~CMainDlg();
	CMainDlg();
// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	LRESULT OnComboSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHotkeyLL(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT OnShow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHotKey(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	CComboBox m_combo;
	CStatic m_label_capture_key;
	CStatic m_label_popmenu_key;
	CStatic m_label_capture;
	CStatic m_label_popmenu;
	CButton m_btn_capture_set;
	CButton m_btn_popmenu_set;
	CButton m_btn_hide;
	CButton m_chk_add_startup;
	CStaticHref m_label_upgrade;
	CStatic m_label_mwnd_key;
	CStatic m_label_mwnd;
	CButton m_btn_mwnd_set;
	
	LRESULT OnBnClickedBtnSetCapturehKey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnBnClickedBtnSetPopmenuKey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	static string m_curr_lang;
	LRESULT OnBnClickedBtnHide(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBtnSetMwndKey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedChkAddStartup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void ReadConfig();
	LRESULT OnBnClickedAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnStnClickedHrefNewVer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
