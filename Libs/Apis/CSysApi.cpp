#include "CSysApi.h"

void CSysApi::tagWINDOW::CenterWindow(HWND hWnd)
{
	// Get the client rectangle of the window on which we want to center
	// Make sure the pointer is not NULL first

	if (hWnd == NULL)
		return;

	RECT rectToCenterOn;
	GetWindowRect(GetDesktopWindow(), &rectToCenterOn);

	RECT rectSubDialog;
	GetWindowRect(hWnd, &rectSubDialog);


	int xLeft = (rectToCenterOn.left + rectToCenterOn.right) / 2 - rectSubDialog.right / 2;
	int yTop = (rectToCenterOn.top + rectToCenterOn.bottom) / 2 - rectSubDialog.bottom / 2;

	// Move the window to the correct coordinates with SetWindowPos()
	SetWindowPos(hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

}
void CSysApi::tagWINDOW::ActivateWindow(const string& clasName,const string& title)
{

	// ������ǰ������ʵ���������ھ��
	HWND hWnd = FindWindow(clasName.c_str(), title.c_str());

	if (hWnd)
	{
		// ����ҵ���ǰ������ʵ���Ĵ��ڣ�����ڲ��������ǰ̨
		// ����ҵ���ǰ������ʵ���Ĵ��ڣ���ʾ���ڲ��������ǰ̨
		//ShowWindow(hWnd, SW_MINIMIZE);
		/*
		ShowWindow(hWnd, SW_SHOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetForegroundWindow(hWnd);
		SetActiveWindow(hWnd);
		*/
		ShowWindow(hWnd, SW_SHOW);

		DWORD foregroundThreadID = GetWindowThreadProcessId(hWnd, nullptr);
		DWORD currentThreadID = GetCurrentThreadId();

		// ����ǰ�̸߳��ӵ������̣߳�ȷ�����������ڴ����߳���ִ��
		AttachThreadInput(currentThreadID, foregroundThreadID, TRUE);

		// ���ô���Ϊ����
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		// ����ڲ����������õ�����
		SetForegroundWindow(hWnd);
		SetFocus(hWnd);
		SetActiveWindow(hWnd);
		// ����̸߳���
		AttachThreadInput(currentThreadID, foregroundThreadID, FALSE);
	}
}



HWND CSysApi::tagWINDOW::GetTopmostWindow()
{
	HWND hWndTopmost = nullptr;
	DWORD dwTopProcess = 0;

	HWND hWnd = GetTopWindow(nullptr);
	while (hWnd != nullptr)
	{
		DWORD dwProcess = 0;
		GetWindowThreadProcessId(hWnd, &dwProcess);

		if (dwProcess != dwTopProcess && IsWindowVisible(hWnd) && (hWnd == GetForegroundWindow()))
		{
			dwTopProcess = dwProcess;
			hWndTopmost = hWnd;
		}

		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	}

	return hWndTopmost;
}
HWND CSysApi::tagWINDOW::GetRootWindow(HWND hWnd) {
	return GetAncestor(hWnd, GA_ROOT);
}
void CSysApi::tagWINDOW::HideWindow(HWND hWnd) {
	ShowWindow(hWnd, SW_HIDE);
	//SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
}
/*
#include <windows.h>

void BringWindowToTop(HWND hWnd)
{
DWORD foregroundThreadID = GetWindowThreadProcessId(GetForegroundWindow(), nullptr);
DWORD currentThreadID = GetCurrentThreadId();

// ����ǰ�̸߳��ӵ������̣߳�ȷ�����������ڴ����߳���ִ��
AttachThreadInput(currentThreadID, foregroundThreadID, TRUE);

// ���ô���Ϊ����
SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

// ����ڲ����������õ�����
SetForegroundWindow(hWnd);
SetFocus(hWnd);

// ����̸߳���
AttachThreadInput(currentThreadID, foregroundThreadID, FALSE);
}

*/
void CSysApi::tagWINDOW::ActivateWindow(HWND hWnd)
{

	if (hWnd)
	{
		/*
		// ����ҵ���ǰ������ʵ���Ĵ��ڣ�����ڲ��������ǰ̨
		// ����ҵ���ǰ������ʵ���Ĵ��ڣ���ʾ���ڲ��������ǰ̨
		//ShowWindow(hWnd, SW_MINIMIZE);
		ShowWindow(hWnd, SW_SHOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetForegroundWindow(hWnd);
		SetActiveWindow(hWnd);
		*/
		ShowWindow(hWnd, SW_SHOW);
		
		
		
		DWORD foregroundThreadID = GetWindowThreadProcessId(hWnd, nullptr);
		DWORD currentThreadID = GetCurrentThreadId();

		// ����ǰ�̸߳��ӵ������̣߳�ȷ�����������ڴ����߳���ִ��
		AttachThreadInput(currentThreadID, foregroundThreadID, TRUE);

		// ���ô���Ϊ����
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		// ����ڲ����������õ�����
		SetForegroundWindow(hWnd);
		SetFocus(hWnd);
		SetActiveWindow(hWnd);
		// ����̸߳���
		AttachThreadInput(currentThreadID, foregroundThreadID, FALSE);
	}
}
DWORD CSysApi::tagPROCESS::GetProcessIdByName(const char* processName)
{
	DWORD processId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 processEntry = { 0 };
		processEntry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &processEntry))
		{
			do
			{
				if (::_stricmp(processEntry.szExeFile, processName) == 0)
				{
					processId = processEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &processEntry));
		}
		CloseHandle(hSnapshot);
	}
	return processId;
}
BOOL CSysApi::tagPROCESS::KillProcess(DWORD pid) {

	::TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, false, pid), 0);

	TCHAR command[MAX_PATH];
	wsprintf(command, _T("/F /PID %lu"), pid);

	ShellExecute(NULL, "open", _T("taskkill.exe"), command, NULL, SW_HIDE);
	return TRUE;
}
BOOL CSysApi::tagPROCESS::InjectDLL(const string& exePath, const string& workDir,const string& injectDll)
{
	// Ŀ���������·��
	const char* targetProcess = exePath.c_str();

	// ע��� DLL ·��
	const char* dllPath = injectDll.c_str();

	// ����������Ϣ�ṹ��
	STARTUPINFOA startupInfo = { sizeof(STARTUPINFOA) };
	PROCESS_INFORMATION processInfo;

	// ����Ŀ�����
	if (!CreateProcessA(NULL, (LPSTR)targetProcess, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, workDir.c_str(), &startupInfo, &processInfo))
	{
		cout << "Failed to create the target process." << endl;
		return 1;
	}

	// ��ȡĿ����̾��
	HANDLE hProcess = processInfo.hProcess;

	// ��Ŀ������з����ڴ�
	LPVOID pRemoteMem = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (pRemoteMem == NULL)
	{
		cout << "Failed to allocate memory in the target process." << endl;
		CloseHandle(hProcess);
		return 1;
	}

	// �� DLL ·��д��Ŀ������ڴ�
	if (!WriteProcessMemory(hProcess, pRemoteMem, dllPath, strlen(dllPath) + 1, NULL))
	{
		cout << "Failed to write to the target process memory." << endl;
		VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	// ��ȡ kernel32.dll ģ��ľ��
	HMODULE hKernel32 = GetModuleHandle("kernel32.dll");
	if (hKernel32 == NULL)
	{
		cout << "Failed to get the handle of kernel32.dll." << endl;
		VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	// ��ȡ LoadLibrary �����ĵ�ַ
	FARPROC fnLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");
	if (fnLoadLibrary == NULL)
	{
		cout << "Failed to get the address of LoadLibraryA function." << endl;
		VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	// ��Ŀ������д���Զ���̣߳�����ע��� DLL
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)fnLoadLibrary, pRemoteMem, 0, NULL);
	if (hThread == NULL)
	{
		cout << "Failed to create remote thread in the target process." << endl;
		VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	// �ȴ�Զ���߳�ִ�����
	if (WaitForSingleObject(hThread, INFINITE) != WAIT_OBJECT_0)
	{
		cout << "Failed to wait for the remote thread to finish." << endl;
	}
	// �ָ�Ŀ����̵�ִ��
	ResumeThread(processInfo.hThread);

	// ������Դ
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
	CloseHandle(hProcess);

	cout << "Remote injection completed." << endl;

	return 0;
}
BOOL CSysApi::tagPROCESS::EnableDebugPrivilege()
{
	HANDLE token;
	//����Ȩ��
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token))
	{
		::MessageBox(NULL, "�򿪽�������ʧ��...", "����", MB_ICONSTOP);
		return FALSE;
	}
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(token, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		::MessageBox(NULL, "��������Ȩ��ʧ��...", "����", MB_ICONSTOP);
		return FALSE;
	}
	CloseHandle(token);
	return TRUE;
}
void CSysApi::tagDC::GetCtrlTextSize(HWND parent, DWORD ctrlId, SIZE* outSize) {
	TCHAR lpOutBuff[1024] = { NULL };
	HFONT hDefaultSystemFont = GetDefaultFont();
	
	GetDlgItemText(parent, ctrlId, lpOutBuff, sizeof(lpOutBuff));
	
	//��������ı�
	REGEX_RESULTS resultReg = CStringApi::regex_match_all(lpOutBuff, "([\\w\\W]*?)(?:[\\n\\r\\t\\v]{1})");

	HWND hCtrl = GetDlgItem(parent, ctrlId);
	HDC hdc = GetWindowDC(hCtrl);
	HFONT old = (HFONT)SelectObject(hdc, hDefaultSystemFont);

	int max = 0;
	string maxPtr = "";
	int lens = (int)resultReg.size();
	for (auto istr : resultReg) {
		if (max < istr.length()) {
			max = (int)istr.length();
			maxPtr = istr.c_str();
		}
	}

	::GetTextExtentPoint32(hdc, maxPtr.c_str(), (int)maxPtr.length() + 2, outSize);
	outSize->cy = outSize->cy * lens;
	// int nDPIx = outSize->cx * 72 / GetDeviceCaps(hdc, LOGPIXELSX);
	 //int nDPIy = outSize->cy * 72 / GetDeviceCaps(hdc, LOGPIXELSY);
	SelectObject(hdc, old);
	ReleaseDC(hCtrl, hdc);
	DeleteDC(hdc);
	DeleteObject(hDefaultSystemFont);

}
DWORD CSysApi::tagPROCESS::GetProcessId(const string& filePath)
{
	// �������̿���
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		cout << "Failed to create snapshot. Error code: " << GetLastError() << endl;
		return 0;
	}

	// �������̿���
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hSnapshot, &pe32))
	{
		do
		{
			// �жϽ���λ��
			
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe32.th32ProcessID);
			if (hProcess != NULL)
			{
				//if (IsWow64Process(hProcess, &is32Bit) && is32Bit)
				{
					TCHAR lpExePath[MAX_PATH] = { NULL };
					// DWORD exePathlen;
					//QueryFullProcessImageName(hProcess, 0, lpExePath, &exePathlen);
					// ƥ�������
					if (string(pe32.szExeFile) == filePath)
					{
						CloseHandle(hSnapshot);
						CloseHandle(hProcess);
						return pe32.th32ProcessID;
					}
				}

				CloseHandle(hProcess);
			}
		} while (Process32Next(hSnapshot, &pe32));
	}

	CloseHandle(hSnapshot);

	return 0;
}
string CSysApi::tagPROCESS::GetCurrentAppFullPath() {

	TCHAR moduleName[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, moduleName, sizeof(moduleName));
	return moduleName;
}
DWORD CSysApi::tagPROCESS::GetProcessIdFromWindow(HWND hWNd) {
	DWORD pid=0;
	::GetWindowThreadProcessId(CSysApi::WINDOW.GetRootWindow(hWNd), &pid);
	return pid;
}
DWORD CSysApi::tagPROCESS::GetProcessFilePath(DWORD pid, TCHAR* lpExePath, DWORD exePathlen, BOOL isfullPath) {

	HANDLE hProcess = NULL;


	hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if (hProcess) {
		QueryFullProcessImageName(hProcess, 0, lpExePath, &exePathlen);
		CloseHandle(hProcess);
	}
	return exePathlen;
}
HFONT CSysApi::tagDC::GetDefaultFont()
{

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
	return CreateFontIndirect(&ncm.lfMessageFont);

}



BOOL CSysApi::tagMEM::EnumVirtualMemoryEx(HANDLE hp, QWORD beginaddr, MEMORY_BASIC_INFORMATION* t, SEARCH_PAGE_TYPE stype)
{
	VOID* pAddress = (VOID*)beginaddr;

	QWORD dwResult = 0;
	do
	{
		dwResult = VirtualQueryEx(hp, pAddress, t, sizeof(MEMORY_BASIC_INFORMATION));
		if ((stype == SEARCH_PAGE_RW) && ((t->Protect == PAGE_WRITECOMBINE) || (t->Protect == PAGE_WRITECOPY) || (t->Protect == PAGE_READONLY) || (t->Protect == PAGE_READWRITE)))
		{
			return TRUE;
		}
		if ((stype == SEARCH_PAGE_EXEC) && ((t->Protect == PAGE_EXECUTE) || (t->Protect == PAGE_EXECUTE_READ) || (t->Protect == PAGE_EXECUTE_READWRITE) || (t->Protect == PAGE_EXECUTE_WRITECOPY)))
		{
			return TRUE;
		}
		pAddress = (PBYTE)pAddress + t->RegionSize;

	} while (dwResult);
	return FALSE;
}

QWORD CSysApi::tagMEM::SearchKeyStr(HANDLE hp, BYTE* pbegin, CHAR* fcode, QWORD lenCode, QWORD searchLen, SEARCH_PAGE_TYPE stype)
{
	QWORD	cntSearch = 0, cntCmp = 0;
	CHAR* ptr;
	BYTE* pCode;
	BYTE* pRemoteMemroy = 0;
	BOOL	isgo = FALSE;
	QWORD	rlen = lenCode / 2;
	MEMORY_BASIC_INFORMATION t = { 0 };
	ptr = fcode;
	pCode = (BYTE*)pbegin;
	//if(!pCode)return 0;
	//ת��16����
	BYTE* hexcode = (BYTE*)malloc(lenCode >> 1);
	for (int cnt = 0; cnt < lenCode; cnt += 2)
	{
		if (*(WORD*)((BYTE*)(ptr)+cnt) == ANY_CODE)
		{
			hexcode[cnt >> 1] = 0;
			continue;
		}
		hexcode[cnt >> 1] = (BYTE)CStringApi::ConvertHexToDec((CHAR*)(ptr + cnt), 1);
	}
	///ö�����������ڴ�ҳ
#ifndef LOLINTERFACETESTDLL_EXPORTS	//���Թ��̱�� 
	try
	{
#endif
		do
		{
			isgo = EnumVirtualMemoryEx(hp, (QWORD)pCode, &t, stype);
			if (!isgo)
			{
				free(hexcode);
				return 0;
			}

			pCode = (BYTE*)t.BaseAddress;
			searchLen = t.RegionSize;
			if (pRemoteMemroy)free(pRemoteMemroy);
			pRemoteMemroy = (BYTE*)malloc(searchLen);
			ReadProcessMemory(hp, (LPCVOID)pCode, pRemoteMemroy, searchLen, 0);
			for (cntSearch = 0; cntSearch < searchLen; cntSearch++)
			{
				for (cntCmp = 0; cntCmp < lenCode; cntCmp += 2)
				{

					if (*(WORD*)((BYTE*)(ptr)+cntCmp) == ANY_CODE)
					{
						continue;
					}
					if (0 == pCode)
					{
						//__asm int 3
						cntSearch = searchLen;
						break;
					}
					if ((searchLen - cntSearch) < (lenCode >> 1))
					{
						cntSearch = searchLen;
						break;
					}
					if (pRemoteMemroy[cntSearch + (cntCmp >> 1)] == hexcode[cntCmp >> 1])
					{
						if ((cntCmp + 2 == lenCode) && (hexcode != (BYTE*)(pRemoteMemroy + cntSearch)))
						{
							free(hexcode);
							free(pRemoteMemroy);
							return (QWORD)(BYTE*)(pCode + cntSearch);
						}
					}
					else
					{
						break;
					}

				}
			}
			pCode = (BYTE*)((QWORD)(t.BaseAddress) + t.RegionSize);
		} while (isgo);

	}
	catch (...)
	{
		//e->Delete();
	}

	free(pRemoteMemroy);
	free(hexcode);
	return 0;
}
QWORD CSysApi::tagMEM::SearchKeyCode(QWORD Pid, QWORD searchFrom, char* KeyCode, QWORD searchSize)
{

	QWORD ret = 0;
	char* cmpcode = 0;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)Pid);
	if (!searchSize)searchSize = 0x0080000;



	if (Pid == GetCurrentProcessId())
	{
		cmpcode = (char*)searchFrom;
		ret = SearchKeyStr(GetCurrentProcess(), (BYTE*)cmpcode, KeyCode, (QWORD)lstrlenA(KeyCode), (QWORD)searchSize - 4, SEARCH_PAGE_EXEC);
	}
	else
	{
		cmpcode = (char*)malloc(searchSize);
		ZeroMemory(cmpcode, searchSize);
		ReadProcessMemory(hProcess, (LPCVOID)searchFrom, cmpcode, searchSize, 0);
		ret = SearchKeyStr(hProcess, (BYTE*)cmpcode, KeyCode, (QWORD)lstrlenA(KeyCode), (QWORD)searchSize - 4, SEARCH_PAGE_EXEC);
		free(cmpcode);
	}

	CloseHandle(hProcess);
	return ret;
}


string CSysApi::tagNET::AppendRandcodeToRequest(const string& url) {
	string modifiedURL = url;
	// �������е� ?= ����
	size_t pos = modifiedURL.rfind("?");
	if (pos != string::npos) {
		// �滻���е� ?= ����Ϊ�µ������
		string otherArgs = modifiedURL.substr(pos+1);

		string randomNumber = to_string(time(nullptr));
		modifiedURL = modifiedURL.substr(0, pos) + "?=" + randomNumber ;
		modifiedURL += otherArgs.length() > 0 ? "&" + otherArgs : "";
	}
	else {
		// �� URL �� .js ����׷�� ?= �����
		//size_t pos = url.rfind('.');
		//string extName = url.substr(pos);
		//string newUrl = url.substr(0, pos-1);
		string randomNumber = to_string(time(nullptr));
		modifiedURL = url + "?=" + randomNumber;
	}

	return modifiedURL;
}
BOOL CSysApi::tagNET::CheckHttpReqeustInListFile(const string& url) {

	string FILE_LIST[] = { ".png",".jpg",".css",".js",".html" };
	size_t arraySize = sizeof(FILE_LIST) / sizeof(FILE_LIST[0]);
	for (int cnt = 0; cnt < arraySize; cnt++) {
		string nurl = CStringApi::StringToLower(url);
		if (CStringApi::IndexOf(nurl, FILE_LIST[cnt]) > -1) {
			return TRUE;
		}
	}
	return FALSE;
}

string CSysApi::tagRES::GetFileVersionItem(const string& strFilePath, const string& infoItem)
{
	DWORD dwSize;
	DWORD dwRtn;
	string szVersion;
	//��ȡ�汾��Ϣ��С
	dwSize = GetFileVersionInfoSize(strFilePath.c_str(), NULL);
	if (dwSize == 0)
	{
		return "";
	}
	char *pBuf;
	pBuf = new char[dwSize + 1];
	if (pBuf == NULL)
		return "";
	memset(pBuf, 0, dwSize + 1);
	//��ȡ�汾��Ϣ
	dwRtn = GetFileVersionInfo(strFilePath.c_str(), NULL, dwSize, pBuf);
	if (dwRtn == 0)
	{
		return "";
	}
	LPVOID lpBuffer = NULL;
	UINT uLen = 0;
	//�汾��Դ�л�ȡ��Ϣ
	string resPath = "\\StringFileInfo\\000404b0\\" + infoItem;
	//0804����
	//04b0��1252,ANSI
	//���Դ�ResourceView�е�Version��BlockHeader�п���
	//���Բ��Ե�����
	/*
	CompanyName
	FileDescription
	FileVersion
	InternalName
	LegalCopyright
	OriginalFilename
	ProductName
	ProductVersion
	Comments
	LegalTrademarks
	PrivateBuild
	SpecialBuild
	*/
	dwRtn = VerQueryValue(pBuf, resPath.c_str(),
		&lpBuffer,
		&uLen);
	if (dwRtn == 0)
	{
		return "";
	}
	szVersion = (char*)lpBuffer;
	delete pBuf;
	return szVersion;
}
string CSysApi::tagRES::GetApplicationVersion()
{
	TCHAR szFullPath[MAX_PATH] = { 0 };
	DWORD dwVerInfoSize = 0;
	DWORD dwVerHnd;
	VS_FIXEDFILEINFO * pFileInfo;

	GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
	if (dwVerInfoSize)
	{
		// If we were able to get the information, process it:
		HANDLE  hMem;
		LPVOID  lpvMem;
		unsigned int uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);
		::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);

		//int ret = GetLastError();
		WORD m_nProdVersion[4];

		// Product version from the FILEVERSION of the version info resource 
		m_nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
		m_nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS);

		string strVersion;
		strVersion = CStringApi::Format(_T("%d.%d.%d.%d"), m_nProdVersion[0], m_nProdVersion[1], m_nProdVersion[2], m_nProdVersion[3]);

		GlobalUnlock(hMem);
		GlobalFree(hMem);

		return strVersion;
	}
	return "";
}
string CSysApi::tagRES::VER_COMPANY_NAME = "CompanyName";
string CSysApi::tagRES::VER_FILE_DESCRIPTION = "FileDescription";
string CSysApi::tagRES::VER_FILE_VERSION = "FileVersion";
string CSysApi::tagRES::VER_INTERNAL_NAME = "InternalName";
string CSysApi::tagRES::VER_LEGAL_COPYRIGHT = "LegalCopyright";
string CSysApi::tagRES::VER_ORIGINAL_FILENAME = "OriginalFilename";
string CSysApi::tagRES::VER_PRODUCT_NAME = "ProductName";
string CSysApi::tagRES::VER_PRODUCT_VERSION = "ProductVersion";
string CSysApi::tagRES::VER_COMMENTS = "Comments";
string CSysApi::tagRES::VER_LEGAL_TRADEMARKS = "LegalTrademarks";
string CSysApi::tagRES::VER_PRIVATE_BUILD = "PrivateBuild";
string CSysApi::tagRES::VER_SPACIAL_BUILD = "SpecialBuild";
string CSysApi::tagRES::LoadStringFromResource(int resourceId)
{
	HMODULE hModule = GetModuleHandle(nullptr);
	if (hModule == nullptr)
	{
		// ��ȡģ����ʧ��
		return "";
	}

	TCHAR buffer[1024];
	int length = LoadString(hModule, resourceId, buffer, sizeof(buffer) / sizeof(TCHAR));
	if (length == 0)
	{
		// ����Դ�����ַ���ʧ��
		return "";
	}

	return string(buffer, length);
}
bool CSysApi::tagSYSTEM::AddToStartup(const string &startname,const string& exePath)
{
	// ���ݲ���ϵͳ�汾ѡ��ע���·��
	const char* regPath = nullptr;

	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osvi))
	{
		if (osvi.dwMajorVersion >= 6)
		{
			if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
				regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; // Windows 7
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
				regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; // Windows 8
			else if (osvi.dwMajorVersion >= 10)
				regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; // Windows 10 and Windows 11
		}
	}

	if (regPath != nullptr)
	{
		// �򿪵�ǰ�û���ע����
		HKEY hKey;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
		{
			// ����ִ���ļ���·����ӵ�ע�����
			if (RegSetValueEx(hKey, startname.c_str(), 0, REG_SZ, (BYTE*)exePath.c_str(), exePath.length() + 1) == ERROR_SUCCESS)
			{
				cout << "�����ѳɹ���ӵ����������" << endl;
				RegCloseKey(hKey);
				return true;
			}
			else
			{
				cout << "�޷���ӳ��򵽿��������" << endl;
			}

			// �ر�ע����
			RegCloseKey(hKey);
		}
		else
		{
			cout << "�޷���ע������" << endl;
		}
	}
	else
	{
		cout << "��֧�ֵĲ���ϵͳ�汾��" << endl;
	}

	return false;
}
string CSysApi::tagSYSTEM::GetAppFullPath() {
	TCHAR path[MAX_PATH] = { NULL };
	GetModuleFileName(GetModuleHandle(0), path, sizeof(path));
	return path;
}
string CSysApi::tagSYSTEM::GetCurrentDir() {

	string fullpath = GetAppFullPath();
	fullpath = CFileApi::GetFilePathFromFullpath(fullpath);
	// ::GetCurrentDirectory(sizeof(path), path);
	return fullpath;
}
bool CSysApi::tagSYSTEM::IsAnotherInstanceRunning(const string& appName)
{

	// ���Դ���������
	HANDLE hMutex = CreateMutex(NULL, TRUE, appName.c_str());

	// ��黥�����Ƿ��Ѿ�����
	if (hMutex && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// �رջ��������������true����ʾ��һ��ʵ����������
		CloseHandle(hMutex);
		return true;
	}

	// ����false����ʾ��ǰ�ǵ�һ��ʵ��
	return false;
}
bool CSysApi::tagSYSTEM::RemoveFromStartup(const string &startname)
{
	// ���ݲ���ϵͳ�汾ѡ��ע���·��
	const char* regPath = nullptr;

	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osvi))
	{
		if (osvi.dwMajorVersion >= 6)
		{
			if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
				regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; // Windows 7
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
				regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; // Windows 8
			else if (osvi.dwMajorVersion >= 10)
				regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; // Windows 10 and Windows 11
		}
	}

	if (regPath != nullptr)
	{
		// �򿪵�ǰ�û���ע����
		HKEY hKey;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
		{
			// ��ע������Ƴ�ָ���ļ�
			if (RegDeleteValue(hKey, startname.c_str()) == ERROR_SUCCESS)
			{
				cout << "�����ѳɹ��ӿ������������Ƴ���" << endl;
				RegCloseKey(hKey);
				return true;
			}
			else
			{
				cout << "�޷��ӿ������������Ƴ�����" << endl;
			}

			// �ر�ע����
			RegCloseKey(hKey);
		}
		else
		{
			cout << "�޷���ע������" << endl;
		}
	}
	else
	{
		cout << "��֧�ֵĲ���ϵͳ�汾��" << endl;
	}

	return false;
}
string CSysApi::tagSYSTEM::GetCurrentTimestamp() {
	// ��ȡ��ǰʱ���

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	// ��ȡ������
	SYSTEMTIME sysTimeWithMilliseconds;
	GetSystemTime(&sysTimeWithMilliseconds);
	int milliseconds = sysTimeWithMilliseconds.wMilliseconds;

	// ��������ʱ���ַ���
	TCHAR forTime[60]{ 0 };
	/*
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(4) << sysTime.wYear << "/";
	oss << std::setfill('0') << std::setw(2) << sysTime.wMonth << "/";
	oss << std::setfill('0') << std::setw(2) << sysTime.wDay << " ";
	oss << std::setfill('0') << std::setw(2) << sysTime.wHour << ":";
	oss << std::setfill('0') << std::setw(2) << sysTime.wMinute << ":";
	oss << std::setfill('0') << std::setw(2) << sysTime.wSecond << ".";
	oss << std::setfill('0') << std::setw(3) << milliseconds;
	*/
	wsprintf(forTime, "%d/%d/%d %d:%d:%d.%d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, milliseconds);

	return forTime;
}
vector<string> CSysApi::tagSYSTEM::GetCommandLineArguments()
{
	vector<string> arguments;

	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	for (int i = 0; i < argc; ++i)
	{
		int length = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, nullptr, 0, nullptr, nullptr);
		if (length > 0)
		{
			string argument(length, '\0');
			WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, &argument[0], length, nullptr, nullptr);
			arguments.push_back(argument);
		}
	}

	LocalFree(argv);

	return arguments;
}

string CSysApi::tagSYSTEM::GetCommandLineArgumentByIndex(int index)
{
	vector<string> arguments = GetCommandLineArguments();
	if (index >= 0 && index < static_cast<int>(arguments.size()))
	{
		return arguments[index];
	}
	else
	{
		return string();
	}
}