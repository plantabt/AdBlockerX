
#include "CKeybd.h"
CKeybd::HOT_KEY_MAP CKeybd::g_currKeys;
CKeybd::HOT_KEY_MAP CKeybd::m_keyMap;
map<string, pair<WORD, string>> CKeybd::m_hotKeys;
HWND CKeybd::hMainWindow=NULL;
HHOOK CKeybd::hkbd=NULL;
static DWORD heartbeat = 1, oldheartbeat = 0;
string CKeybd::GetKeyName(BYTE vkey) {
	if (m_keyMap.size() == 0) {
		Init();
	}
	auto it = m_keyMap.find(vkey);
	if (it == m_keyMap.end())return "";

	return m_keyMap[vkey].second;
}
BYTE CKeybd::GetKeyCode(const string &keyname) {
	if (m_keyMap.size() == 0) {
		Init();
	}
	for (auto ik : m_keyMap) {
		if (ik.second.second == keyname) {
			return ik.first;
		}
	}
	return -1;
}

void CKeybd::HotKeyDown(BYTE key, const string &name)
{
	g_currKeys[key] = make_pair(key, name);
}
void CKeybd::HotKeyUp(BYTE key)
{
	g_currKeys.erase(key);
}


void CKeybd::WeakupScan(){
	if (oldheartbeat == heartbeat) {
		if (hkbd != NULL){
			UnhookWindowsHookEx(hkbd);
		}
		hkbd = SetWindowsHookEx(WH_KEYBOARD_LL, KeybdHookll, NULL, NULL);
	}
	oldheartbeat = heartbeat;
}
void CKeybd::SetHotKeys(HWND hMainWind,const string &name,WORD ctrlId, const  string &keyNames) {
	if (hkbd == NULL) {
		hMainWindow = hMainWind;
		hkbd = SetWindowsHookEx(WH_KEYBOARD_LL, KeybdHookll, NULL, NULL);
	}
	m_hotKeys[name] = make_pair(ctrlId,keyNames);
}

LRESULT CALLBACK CKeybd::KeybdHookll(int nCode, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;
	if (nCode == HC_ACTION)
	{
		heartbeat++;
		KBDLLHOOKSTRUCT* pKeyboardStruct = (KBDLLHOOKSTRUCT*)(lParam);
		BYTE key = pKeyboardStruct->vkCode;
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			HotKeyDown(key, GetKeyName(key));
			//ESC不能设置为热键
			if(key!=GetKeyCode("ESC")){
				ret = SendMessage(hMainWindow, WM_HOTKEY_LL, -1, key);
				if (ret != 0)return ret;
			}
			for (auto n : m_hotKeys) {
				string keystr = n.second.second;
				vector<string> keys = CStringApi::Split(keystr, "+");
				if(keys.size() == g_currKeys.size()){
					bool is_allhit = true;
					for (auto ik : keys) {
						BYTE ikc =GetKeyCode(ik);
						if (g_currKeys.find(ikc) == g_currKeys.end()) {
							is_allhit = false;
							break;
						}
					}
					if (is_allhit) {
						//触发热键
						is_allhit = false;
						ret = SendMessage(hMainWindow, WM_HOTKEY_LL, n.second.first, key);
						if (ret != 0)return ret;
					}
				}
				
			}
		}
		else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
		{
			HotKeyUp(key);
		}
	}
	return CallNextHookEx(hkbd, nCode, wParam, lParam);
}
CKeybd::HOT_KEY_MAP CKeybd::GetCurrentKeys() {
	return (CKeybd::HOT_KEY_MAP)CKeybd::g_currKeys;
}
void CKeybd::Init()
{
	m_keyMap[0x01] = make_pair(0x01, "LBUTTON");
	m_keyMap[0x02] = make_pair(0x02, "RBUTTON");
	m_keyMap[0x03] = make_pair(0x03, "CANCEL");
	m_keyMap[0x04] = make_pair(0x04, "MBUTTON");    /* NOT contiguous with L & RBUTTON */


	m_keyMap[0x05] = make_pair(0x05, "XBUTTON1");    /* NOT contiguous with L & RBUTTON */
	m_keyMap[0x06] = make_pair(0x06, "XBUTTON2");    /* NOT contiguous with L & RBUTTON */

	m_keyMap[0x08] = make_pair(0x08, "BACK");
	m_keyMap[0x09] = make_pair(0x09, "TAB");


	m_keyMap[0x0C] = make_pair(0x0C, "CLEAR");
	m_keyMap[0x0D] = make_pair(0x0D, "RETURN");

	m_keyMap[0x10] = make_pair(0x10, "SHIFT");
	m_keyMap[0x11] = make_pair(0x11, "CONTROL");
	m_keyMap[0x12] = make_pair(0x12, "MENU");
	m_keyMap[0x13] = make_pair(0x13, "PAUSE");
	m_keyMap[0x14] = make_pair(0x14, "CAP");

	m_keyMap[0x15] = make_pair(0x15, "KANA");
	m_keyMap[0x15] = make_pair(0x15, "HANGEUL");
	m_keyMap[0x15] = make_pair(0x15, "HANGUL");
	m_keyMap[0x17] = make_pair(0x17, "JUNJA");
	m_keyMap[0x18] = make_pair(0x18, "FINAL");
	m_keyMap[0x19] = make_pair(0x19, "HANJA");
	m_keyMap[0x19] = make_pair(0x19, "KANJI");

	m_keyMap[0x1B] = make_pair(0x1B, "ESC");

	m_keyMap[0x1C] = make_pair(0x1C, "CONVERT");
	m_keyMap[0x1D] = make_pair(0x1D, "NONCONVERT");
	m_keyMap[0x1E] = make_pair(0x1E, "ACCEPT");
	m_keyMap[0x1F] = make_pair(0x1F, "MODECHANGE");

	m_keyMap[0x20] = make_pair(0x20, "SPACE");
	m_keyMap[0x21] = make_pair(0x21, "PUP");
	m_keyMap[0x22] = make_pair(0x22, "PDWN");
	m_keyMap[0x23] = make_pair(0x23, "END");
	m_keyMap[0x24] = make_pair(0x24, "HOME");
	m_keyMap[0x25] = make_pair(0x25, "LEFT");
	m_keyMap[0x26] = make_pair(0x26, "UP");
	m_keyMap[0x27] = make_pair(0x27, "RIGHT");
	m_keyMap[0x28] = make_pair(0x28, "DOWN");
	m_keyMap[0x29] = make_pair(0x29, "SELECT");
	m_keyMap[0x2A] = make_pair(0x2A, "PRINT");
	m_keyMap[0x2B] = make_pair(0x2B, "EXECUTE");
	m_keyMap[0x2C] = make_pair(0x2C, "SNAPSHOT");
	m_keyMap[0x2D] = make_pair(0x2D, "INSERT");
	m_keyMap[0x2E] = make_pair(0x2E, "DELETE");
	m_keyMap[0x2F] = make_pair(0x2F, "HELP");

	m_keyMap[0x5B] = make_pair(0x5B, "LWIN");
	m_keyMap[0x5C] = make_pair(0x5C, "RWIN");
	m_keyMap[0x5D] = make_pair(0x5D, "APPS");

	m_keyMap[0x5F] = make_pair(0x5F, "SLEEP");

	m_keyMap[0x60] = make_pair(0x60, "NUMPAD0");
	m_keyMap[0x61] = make_pair(0x61, "NUMPAD1");
	m_keyMap[0x62] = make_pair(0x62, "NUMPAD2");
	m_keyMap[0x63] = make_pair(0x63, "NUMPAD3");
	m_keyMap[0x64] = make_pair(0x64, "NUMPAD4");
	m_keyMap[0x65] = make_pair(0x65, "NUMPAD5");
	m_keyMap[0x66] = make_pair(0x66, "NUMPAD6");
	m_keyMap[0x67] = make_pair(0x67, "NUMPAD7");
	m_keyMap[0x68] = make_pair(0x68, "NUMPAD8");
	m_keyMap[0x69] = make_pair(0x69, "NUMPAD9");
	m_keyMap[0x6A] = make_pair(0x6A, "MULTIPLY");
	m_keyMap[0x6B] = make_pair(0x6B, "ADD");
	m_keyMap[0x6C] = make_pair(0x6C, "SEPARATOR");
	m_keyMap[0x6D] = make_pair(0x6D, "SUBTRACT");
	m_keyMap[0x6E] = make_pair(0x6E, "DECIMAL");
	m_keyMap[0x6F] = make_pair(0x6F, "DIVIDE");
	m_keyMap[0x70] = make_pair(0x70, "F1");
	m_keyMap[0x71] = make_pair(0x71, "F2");
	m_keyMap[0x72] = make_pair(0x72, "F3");
	m_keyMap[0x73] = make_pair(0x73, "F4");
	m_keyMap[0x74] = make_pair(0x74, "F5");
	m_keyMap[0x75] = make_pair(0x75, "F6");
	m_keyMap[0x76] = make_pair(0x76, "F7");
	m_keyMap[0x77] = make_pair(0x77, "F8");
	m_keyMap[0x78] = make_pair(0x78, "F9");
	m_keyMap[0x79] = make_pair(0x79, "F10");
	m_keyMap[0x7A] = make_pair(0x7A, "F11");
	m_keyMap[0x7B] = make_pair(0x7B, "F12");
	m_keyMap[0x7C] = make_pair(0x7C, "F13");
	m_keyMap[0x7D] = make_pair(0x7D, "F14");
	m_keyMap[0x7E] = make_pair(0x7E, "F15");
	m_keyMap[0x7F] = make_pair(0x7F, "F16");
	m_keyMap[0x80] = make_pair(0x80, "F17");
	m_keyMap[0x81] = make_pair(0x81, "F18");
	m_keyMap[0x82] = make_pair(0x82, "F19");
	m_keyMap[0x83] = make_pair(0x83, "F20");
	m_keyMap[0x84] = make_pair(0x84, "F21");
	m_keyMap[0x85] = make_pair(0x85, "F22");
	m_keyMap[0x86] = make_pair(0x86, "F23");
	m_keyMap[0x87] = make_pair(0x87, "F24");

	m_keyMap[0x90] = make_pair(0x90, "NLOCK");
	m_keyMap[0x91] = make_pair(0x91, "SCROLL");


	m_keyMap[0x92] = make_pair(0x92, "OEM_NEC_EQUAL");   // '=' key on numpad

	m_keyMap[0x92] = make_pair(0x92, "OEM_FJ_JISHO");   // 'Dictionary' key
	m_keyMap[0x93] = make_pair(0x93, "OEM_FJ_MASSHOU");   // 'Unregister word' key
	m_keyMap[0x94] = make_pair(0x94, "OEM_FJ_TOUROKU");   // 'Register word' key
	m_keyMap[0x95] = make_pair(0x95, "OEM_FJ_LOYA");   // 'Left OYAYUBI' key
	m_keyMap[0x96] = make_pair(0x96, "OEM_FJ_ROYA");   // 'Right OYAYUBI' key

	m_keyMap[0xA0] = make_pair(0xA0, "LSHIFT");
	m_keyMap[0xA1] = make_pair(0xA1, "RSHIFT");
	m_keyMap[0xA2] = make_pair(0xA2, "LCTR");
	m_keyMap[0xA3] = make_pair(0xA3, "RCTR");
	m_keyMap[0xA4] = make_pair(0xA4, "LALT");
	m_keyMap[0xA5] = make_pair(0xA5, "RALT");

	m_keyMap[0xA6] = make_pair(0xA6, "BROWSER_BACK");
	m_keyMap[0xA7] = make_pair(0xA7, "BROWSER_FORWARD");
	m_keyMap[0xA8] = make_pair(0xA8, "BROWSER_REFRESH");
	m_keyMap[0xA9] = make_pair(0xA9, "BROWSER_STOP");
	m_keyMap[0xAA] = make_pair(0xAA, "BROWSER_SEARCH");
	m_keyMap[0xAB] = make_pair(0xAB, "BROWSER_FAVORITES");
	m_keyMap[0xAC] = make_pair(0xAC, "BROWSER_HOME");

	m_keyMap[0xAD] = make_pair(0xAD, "VOLUME_MUTE");
	m_keyMap[0xAE] = make_pair(0xAE, "VOLUME_DOWN");
	m_keyMap[0xAF] = make_pair(0xAF, "VOLUME_UP");
	m_keyMap[0xB0] = make_pair(0xB0, "MEDIA_NEXT_TRACK");
	m_keyMap[0xB1] = make_pair(0xB1, "MEDIA_PREV_TRACK");
	m_keyMap[0xB2] = make_pair(0xB2, "MEDIA_STOP");
	m_keyMap[0xB3] = make_pair(0xB3, "MEDIA_PLAY_PAUSE");
	m_keyMap[0xB4] = make_pair(0xB4, "LAUNCH_MAIL");
	m_keyMap[0xB5] = make_pair(0xB5, "LAUNCH_MEDIA_SELECT");
	m_keyMap[0xB6] = make_pair(0xB6, "LAUNCH_APP1");
	m_keyMap[0xB7] = make_pair(0xB7, "LAUNCH_APP2");


	m_keyMap[0xBA] = make_pair(0xBA, ";");   // ';:' for US
	m_keyMap[0xBB] = make_pair(0xBB, "=");   // '+' any country
	m_keyMap[0xBC] = make_pair(0xBC, ",");   // ',' any country
	m_keyMap[0xBD] = make_pair(0xBD, "-");   // '-' any country
	m_keyMap[0xBE] = make_pair(0xBE, ".");   // '.' any country
	m_keyMap[0xBF] = make_pair(0xBF, "/");   // '/?' for US
	m_keyMap[0xC0] = make_pair(0xC0, "`");   // '`~' for US


	m_keyMap[0xDB] = make_pair(0xDB, "[");  //  '[{' for US
	m_keyMap[0xDC] = make_pair(0xDC, "|");  //  '\|' for US
	m_keyMap[0xDD] = make_pair(0xDD, "]");  //  ']}' for US
	m_keyMap[0xDE] = make_pair(0xDE, "'");  //  ''"' for US
	m_keyMap[0xDF] = make_pair(0xDF, "OEM_8");

	m_keyMap[0xE1] = make_pair(0xE1, "OEM_AX");  //  'AX' key on Japanese AX kbd
	m_keyMap[0xE2] = make_pair(0xE2, "OEM_102");  //  "<>" or "\|" on RT 102-key kbd.
	m_keyMap[0xE3] = make_pair(0xE3, "ICO_HELP");  //  Help key on ICO
	m_keyMap[0xE4] = make_pair(0xE4, "ICO_00");  //  00 key on ICO


	m_keyMap[0xE5] = make_pair(0xE5, "PROCESSKEY");


	m_keyMap[0xE6] = make_pair(0xE6, "ICO_CLEAR");

	m_keyMap[0xE7] = make_pair(0xE7, "PACKET");

	m_keyMap[0xE9] = make_pair(0xE9, "OEM_RESET");
	m_keyMap[0xEA] = make_pair(0xEA, "OEM_JUMP");
	m_keyMap[0xEB] = make_pair(0xEB, "OEM_PA1");
	m_keyMap[0xEC] = make_pair(0xEC, "OEM_PA2");
	m_keyMap[0xED] = make_pair(0xED, "OEM_PA3");
	m_keyMap[0xEE] = make_pair(0xEE, "OEM_WSCTRL");
	m_keyMap[0xEF] = make_pair(0xEF, "OEM_CUSEL");
	m_keyMap[0xF0] = make_pair(0xF0, "OEM_ATTN");
	m_keyMap[0xF1] = make_pair(0xF1, "OEM_FINISH");
	m_keyMap[0xF2] = make_pair(0xF2, "OEM_COPY");
	m_keyMap[0xF3] = make_pair(0xF3, "OEM_AUTO");
	m_keyMap[0xF4] = make_pair(0xF4, "OEM_ENLW");
	m_keyMap[0xF5] = make_pair(0xF5, "OEM_BACKTAB");

	m_keyMap[0xF6] = make_pair(0xF6, "ATTN");
	m_keyMap[0xF7] = make_pair(0xF7, "CRSEL");
	m_keyMap[0xF8] = make_pair(0xF8, "EXSEL");
	m_keyMap[0xF9] = make_pair(0xF9, "EREOF");
	m_keyMap[0xFA] = make_pair(0xFA, "PLAY");
	m_keyMap[0xFB] = make_pair(0xFB, "ZOOM");
	m_keyMap[0xFC] = make_pair(0xFC, "NONAME");
	m_keyMap[0xFD] = make_pair(0xFD, "PA1");
	m_keyMap[0xFE] = make_pair(0xFE, "OEM_CLEAR");


	m_keyMap[0x30] = make_pair(0x30, "0");
	m_keyMap[0x31] = make_pair(0x31, "1");
	m_keyMap[0x32] = make_pair(0x32, "2");
	m_keyMap[0x33] = make_pair(0x33, "3");
	m_keyMap[0x34] = make_pair(0x34, "4");
	m_keyMap[0x35] = make_pair(0x35, "5");
	m_keyMap[0x36] = make_pair(0x36, "6");
	m_keyMap[0x37] = make_pair(0x37, "7");
	m_keyMap[0x38] = make_pair(0x38, "8");
	m_keyMap[0x39] = make_pair(0x39, "9");

	for (BYTE cnt = 0x41; cnt < 0x5B; cnt++) {
		/*0x41 - 0x5A*/
		string ch =	CStringApi::Format("%c", cnt);
		m_keyMap[cnt] = make_pair(cnt, ch);
	}
	
}
