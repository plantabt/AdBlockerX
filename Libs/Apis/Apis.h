#pragma once

#include "common.h"
#include "CFileApi.h"
#include "CSysApi.h"
#include "CStringApi.h"
#include "CKeybd.h"
#include "CNet.h"
#include "CPEEngine.h"
#include "Config.h"
using namespace std;
/*
BOOL EnableDebugPrivilege();
void CenterWindow(HWND hWnd);
DWORD GetProcessName(DWORD pid, TCHAR* lpExePath, DWORD strLen, BOOL isfullPath);
HFONT GetDefaultFont();

BYTE* ReadFileToBuff(TCHAR* pfilepath, DWORD* rdSize);
BOOL SaveBuffToFile(TCHAR* pfilepath, BYTE* in, DWORD inSize);
*/




class APIS {
public:
	static CConfig Config;
	static CStringApi Strings;
	static CFileApi Files;
	static CSysApi Sys;
	static CKeybd Keybd;
	static CNet Net;
	static CPEEngine PeEngine;
	
};