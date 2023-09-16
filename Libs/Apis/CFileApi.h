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

typedef struct tagHTTP_REQUEST_RECORD {
	QWORD hRequest;
	HINTERNET hConnect;
	BOOL bFirstLog;
	string   lpszURI;
	string	 lpszURL;
	string	 lpMethod;
	string   lpResType;
}HTTP_REQUEST_RECORD, * LPHTTP_REQUEST_RECORD;
typedef struct tagHTTP_CONNECT_RECORD {
	QWORD hConnect;
	string   lpszServerName;

}HTTP_CONNECT_RECORD, * LPHTTP_CONNECT_RECORD;
typedef unordered_map<QWORD, HTTP_REQUEST_RECORD> HTTP_REQUEST_RECORD_MAP;
typedef unordered_map<QWORD, HTTP_CONNECT_RECORD> HTTP_CONNECT_RECORD_MAP;

typedef struct tagHTTP_SEND_INFO {
	HINTERNET hHttp;
	LPVOID   lpszHeaders;
	DWORD     dwHeadersLength;
	LPVOID    lpOptional;
	DWORD     dwOptionalLength;
}HTTP_SEND_INFO, * LPHTTP_SEND_INFO;
typedef struct tagHTTP_RECV_INFO {
	HINTERNET hHttp;
	LPVOID lpBuffer;
	LPINTERNET_BUFFERSW lpBufferOutW;
	DWORD readSize;
}HTTP_RECV_INFO, * LPHTTP_RECV_INFO;

typedef struct tagHANDLE_MAPPING{
	BYTE* lpImageBase;
	DWORD fileSize;
}HANDLE_MAPPING, *LPHANDLE_MAPPING;
class CFileApi
{
public:
	class tagHTTP_LOG {
	private:
		static string m_logPath;
		static HTTP_REQUEST_RECORD_MAP m_RequestRecordMap;
		static HTTP_CONNECT_RECORD_MAP m_ConnectRecordMap;
	public:
		static void SetLogPath(const string& logPath);
		static string GetRequestedFile(string& url);
		static void SetConnectRecord(LPHTTP_CONNECT_RECORD lpCr);
		static void UnsetConnectRecord(HINTERNET hConnect);
		static void SetRequestRecord(LPHTTP_REQUEST_RECORD lpQr);
		static void UnsetRequestRecord(HINTERNET hRequest);
		static BOOL SaveHttpRecv(LPHTTP_RECV_INFO lphttpRecvInfo);
		static BOOL SaveHttpSend(LPHTTP_SEND_INFO lphttpSendInfo);
	};
	tagHTTP_LOG HTTPLOG;

	
	static string GetFileMD5(const string& filePath);
	static string GetPathFromFullPath(const string &fullPath);
	static DWORD LogInfo(const string& pfilepath, const string& in,  BOOL br);
	static DWORD LogInfo(const string& pfilepath, TCHAR* in,DWORD inSize, BOOL br);
	static VOID CloseHandleExt(BYTE* pHandle);
	static BYTE* ReadFileToBuff(TCHAR* pfilepath, DWORD* rdSize);
	static DWORD GetFileSizeExt(TCHAR* pfilepath);
	static BOOL SaveBuffToFile(const string& pfilepath, BYTE* in, DWORD inSize,  BOOL bCreateNew=TRUE,  BOOL bAppend = FALSE);
	static TCHAR* GetFileNameFromePath(TCHAR* filePath, bool hasExt);
	static string GetFilePathFromFullpath(const string filePath);
	static BOOL IsFileExist(const string& filePath);
	static LPHANDLE_MAPPING OpenFileToMapping(const string& filePath);
	static void CloseFileMapping(LPHANDLE_MAPPING hMpping);
	static void OpenFolderAndSelectFile(const string& filePath);
};

