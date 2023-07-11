// AdBlockerX.cpp : main source file for AdBlockerX.exe
//

#include "stdafx.h"

#include "resource.h"

#include "MainDlg.h"
#include <string>
#include <vector>
using namespace std;
CAppModule _Module;


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	AtlAxWinInit();
	// ½« lpCmdLine ×ª»»Îª¿í×Ö·û×Ö·û´®£¨UNICODE£©
	
	int nRet = 0;
	// BLOCK: Run application
	{
		CMainDlg *dlgMain = new CMainDlg();
		dlgMain->Create(nullptr);
		
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
