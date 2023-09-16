#pragma once
#include "common.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include "CStringApi.h"
#include "CDynamicBuffer.h"
#include "CSysApi.h"
#include "CFileApi.h"
#define WM_HOTKEY_LL WM_USER*2
struct CompareGreater
{
	bool operator()(const BYTE& lhs, const BYTE& rhs) const
	{
		return lhs > rhs;
	}
};

class CKeybd
{
public:
	typedef map<BYTE, pair<BYTE, string>, CompareGreater> HOT_KEY_MAP;
	//typedef map<BYTE, pair<BYTE, string>, CompareGreater> HOT_KEY_MAP_DESC;
	static string GetKeyName(BYTE vkey);
	static BYTE GetKeyCode(const string &keyname);
	static void SetHotKeys(HWND hMainWind, const string &name, WORD ctrlId, const string &keyNames);
	static HOT_KEY_MAP GetCurrentKeys();
	static void WeakupScan();
private:
	
	static LRESULT CALLBACK KeybdHookll(int nCode, WPARAM wParam, LPARAM lParam);
	static void Init();
	static map<string, pair<WORD, string>> m_hotKeys;
	static HOT_KEY_MAP m_keyMap;

	static HOT_KEY_MAP g_currKeys;
	static HHOOK hkbd;
	static HWND hMainWindow;
	static void HotKeyDown(BYTE key, const string &name);
	static void ResetKeyStatus();
	static void HotKeyUp(BYTE key);
	
};

