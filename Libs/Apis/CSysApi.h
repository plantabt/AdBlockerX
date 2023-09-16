#pragma once
#include "common.h"
#include "CStringApi.h"
#include "CFileApi.h"
#include <tlhelp32.h>
class CSysApi
{
public:
	class tagNET {
		public:
			static string AppendRandcodeToRequest(const string& url);
			static BOOL CheckHttpReqeustInListFile(const string &url);
	};
	static tagNET NET;


	struct tagWINDOW {
		static void CenterWindow(HWND hWnd);
		static void ActivateWindow(const string& clasName, const string& title);
		static void ActivateWindow(HWND hWnd);
		static void HideWindow(HWND hWnd);
		static HWND GetRootWindow(HWND hWnd);
		static HWND GetTopmostWindow();
	
	};
	static tagWINDOW WINDOW;


	struct tagMEM{
		static QWORD SearchKeyCode(QWORD Pid, QWORD searchFrom, char* KeyCode, QWORD searchSize);
		static QWORD SearchKeyStr(HANDLE hp, BYTE* pbegin, CHAR* fcode, QWORD lenCode, QWORD searchLen, SEARCH_PAGE_TYPE stype);
		static BOOL EnumVirtualMemoryEx(HANDLE hp, QWORD beginaddr, MEMORY_BASIC_INFORMATION* t, SEARCH_PAGE_TYPE stype);
	};
	static tagMEM MEM;


	struct tagDC {
		static HFONT GetDefaultFont();
		static void GetCtrlTextSize(HWND parent, DWORD ctrlId, SIZE* outSize);
	};
	static tagDC DC;


	struct tagPROCESS {
		static DWORD GetProcessIdFromWindow(HWND hWNd);
		static DWORD GetProcessFilePath(DWORD pid, TCHAR* lpExePath, DWORD exePathlen, BOOL isfullPath);
		static BOOL EnableDebugPrivilege();
		static DWORD GetProcessIdByName(const char* processName);
		static BOOL InjectDLL(const string& exePath, const string& workDir, const string& injectDll);
		static BOOL KillProcess(DWORD PID);
		static string GetCurrentAppFullPath();
		static DWORD GetProcessId(const string& filePath);
	};
	static tagPROCESS PROCESS;
	
	struct tagRES {
		static string LoadStringFromResource(int resourceId);
		static string GetApplicationVersion();
		static string GetFileVersionItem(const string& strFilePath, const string& infoItem);

		static string VER_COMPANY_NAME;
		static string VER_FILE_DESCRIPTION;
		static string VER_FILE_VERSION;
		static string VER_INTERNAL_NAME;
		static string VER_LEGAL_COPYRIGHT;
		static string VER_ORIGINAL_FILENAME;
		static string VER_PRODUCT_NAME;
		static string VER_PRODUCT_VERSION;
		static string VER_COMMENTS;
		static string VER_LEGAL_TRADEMARKS;
		static string VER_PRIVATE_BUILD;
		static string VER_SPACIAL_BUILD;
		
	};
	static tagRES RES;

	struct tagSYSTEM {
		static bool AddToStartup(const string &startname, const std::string& exePath);
		static bool RemoveFromStartup(const string &startname);
		static bool IsAnotherInstanceRunning(const string& appName);
		static string GetCurrentDir();
		static string GetAppFullPath();
		static string GetCommandLineArgumentByIndex(int index);
		static vector<string> GetCommandLineArguments();
		static string GetCurrentTimestamp();
	};
	static tagSYSTEM SYSTEM;

};

