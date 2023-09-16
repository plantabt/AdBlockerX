#include "CFileApi.h"
#include <ShellAPI.h>

HTTP_REQUEST_RECORD_MAP CFileApi::tagHTTP_LOG::m_RequestRecordMap;
HTTP_CONNECT_RECORD_MAP CFileApi::tagHTTP_LOG::m_ConnectRecordMap;
string CFileApi::tagHTTP_LOG::m_logPath;
VOID CFileApi::CloseHandleExt(BYTE* pHandle)
{
	GlobalFree(pHandle);
}
string CFileApi::GetPathFromFullPath(const string& fullPath) {
	size_t found = fullPath.rfind("/");
	if(found==string::npos)
		found = fullPath.rfind("\\");
	if (found != string::npos) {
		return fullPath.substr(0,found+1);
	}
	return "";
}



LPHANDLE_MAPPING CFileApi::OpenFileToMapping(const string& filepath) {
	
	LPHANDLE_MAPPING hMapping = (LPHANDLE_MAPPING)malloc(sizeof(HANDLE_MAPPING));
	HANDLE hFile = CreateFile(filepath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)return NULL;
	hMapping->fileSize = GetFileSize(hFile, 0);
	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	BYTE* lpImageBase = (BYTE*)MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
	hMapping->lpImageBase = (BYTE*)malloc(hMapping->fileSize);

	CopyMemory(hMapping->lpImageBase, lpImageBase, hMapping->fileSize);

	/*
	IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)lpImageBase;
	IMAGE_NT_HEADERS64* ntHeader = (IMAGE_NT_HEADERS64*)(dosHeader->e_lfanew + lpImageBase);
	BYTE* EntryPoint = (BYTE*)(ntHeader->OptionalHeader.AddressOfEntryPoint + (QWORD)(lpImageBase));

	IMAGE_DOS_HEADER* dosHeader1 = (IMAGE_DOS_HEADER*)outlpImageBase;
	IMAGE_NT_HEADERS64* ntHeader1 = (IMAGE_NT_HEADERS64*)(dosHeader1->e_lfanew + outlpImageBase);
	BYTE* EntryPoint1 = (BYTE*)(ntHeader1->OptionalHeader.AddressOfEntryPoint + (QWORD)(outlpImageBase));
	*/
	UnmapViewOfFile(lpImageBase);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);
	return hMapping;
}

void CFileApi::CloseFileMapping(LPHANDLE_MAPPING hMpping) {
	free(hMpping->lpImageBase);
	free(hMpping);
}
TCHAR* CFileApi::GetFileNameFromePath(TCHAR* filePath, bool hasExt)
{
	TCHAR fielName[MAX_PATH] = { NULL };
	TCHAR* p = 0;
	lstrcpy(fielName, filePath);

	if (!hasExt)
	{
		*(::strrchr(fielName, '.')) = 0;

		p = ::strrchr(fielName, '\\');
		*p = 0;
		p++;
		return p;
	}

	p = StrStr(fielName, "res\\");
	p += 3;
	*p = 0;
	p++;

	return p;
}
string CFileApi::GetFilePathFromFullpath(const string filePath)
{
	TCHAR fileName[MAX_PATH] = { NULL };
	TCHAR* p = 0;
	p = StrRChr(filePath.c_str(), 0, '\\');
	CopyMemory(fileName, filePath.c_str(), (p - filePath.c_str())+1);
	return fileName;
}
BYTE* CFileApi::ReadFileToBuff(TCHAR* pfilepath, DWORD* rdSize)
{
	BYTE* pBuff = 0;
	HANDLE hFile = 0;
	DWORD fileSize = 0;
	hFile = CreateFile(pfilepath, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 	return 0;
	fileSize = GetFileSize(hFile, NULL) + 1;
	pBuff = (BYTE*)GlobalAlloc(GPTR, fileSize);
	if (!rdSize)
		ReadFile(hFile, pBuff, fileSize - 1, (DWORD*)&rdSize, 0);
	else
		ReadFile(hFile, pBuff, fileSize - 1, rdSize, 0);
	CloseHandle(hFile);
	return pBuff;

}




BOOL CFileApi::SaveBuffToFile(const string& pfilepath, BYTE* in, DWORD inSize,BOOL bCreateNew,BOOL bAppend)
{
	DWORD rd = 0;
	//DWORD fileSize=0;
	HANDLE hFile = 0;
	DWORD CF = OPEN_ALWAYS;
	if (bCreateNew) {
		CF = CREATE_ALWAYS;
	}
	hFile = CreateFile(pfilepath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CF, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 	return FALSE;
	if (bAppend) {
		SetFilePointer(hFile, 0, NULL, FILE_END);
	}
	if (!WriteFile(hFile, in, inSize, &rd, 0))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	CloseHandle(hFile);

	return TRUE;

}

DWORD CFileApi::LogInfo(const string& pfilepath, TCHAR* in,DWORD inSize, BOOL br)
{
	DWORD rd = 0;
	//DWORD fileSize=0;
	HANDLE hFile = 0;

	hFile = CreateFile(pfilepath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 	return FALSE;
	SetFilePointer(hFile, 0, 0, FILE_END);
	if (!WriteFile(hFile, in, inSize, &rd, 0))
	{
		GetLastError();
		CloseHandle(hFile);
		return FALSE;
	}
	if (br)WriteFile(hFile, "\r\n", 2, &rd, 0);
	CloseHandle(hFile);

	return TRUE;

}
DWORD CFileApi::LogInfo(const string& pfilepath, const string& in, BOOL br)
{
	DWORD rd = 0;
	//DWORD fileSize=0;
	HANDLE hFile = 0;
	string fullInfo = CSysApi::SYSTEM.GetCurrentTimestamp() + " " + in ;
	hFile = CreateFile(pfilepath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 	return FALSE;
	SetFilePointer(hFile, 0, 0, FILE_END);
	if (!WriteFile(hFile, fullInfo.c_str(), fullInfo.size(), &rd, 0))
	{
		GetLastError();
		CloseHandle(hFile);
		return FALSE;
	}
	if (br)WriteFile(hFile, "\r\n", 2, &rd, 0);
	CloseHandle(hFile);

	return TRUE;

}
DWORD CFileApi::GetFileSizeExt(TCHAR* pfilepath)
{
	HANDLE hFile; // the file handle
	DWORD dwFileSize; // temporary storage for file sizes

	hFile = CreateFile(pfilepath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) 	return 0;
	dwFileSize = GetFileSize(hFile, NULL);
	CloseHandle(hFile);

	return dwFileSize;

}


#define MD5_DIGEST_LENGTH 16

string CFileApi::GetFileMD5(const string& filePath) {
	HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		//throw runtime_error("Failed to open file");
		return "";
	}

	HCRYPTPROV hProv = 0;
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		CloseHandle(hFile);
		throw runtime_error("Failed to acquire cryptographic context");
		return "";
	}

	HCRYPTHASH hHash = 0;
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		CryptReleaseContext(hProv, 0);
		CloseHandle(hFile);
		throw runtime_error("Failed to create hash");
	}

	const DWORD bufferSize = 4096;
	BYTE buffer[bufferSize];
	DWORD bytesRead;

	while (ReadFile(hFile, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
		if (!CryptHashData(hHash, buffer, bytesRead, 0)) {
			CryptDestroyHash(hHash);
			CryptReleaseContext(hProv, 0);
			CloseHandle(hFile);
			throw runtime_error("Failed to hash data");
		}
	}

	
	CloseHandle(hFile);

	DWORD hashValueSize = MD5_DIGEST_LENGTH;
	BYTE hashValue[MD5_DIGEST_LENGTH];
	if (!CryptGetHashParam(hHash, HP_HASHVAL, hashValue, &hashValueSize, 0)) {
		CryptDestroyHash(hHash);
		throw std::runtime_error("Failed to get hash value");
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	std::ostringstream md5Stream;
	md5Stream << std::hex << std::setfill('0');
	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		md5Stream << std::setw(2) << static_cast<int>(hashValue[i]);
	}

	return md5Stream.str();
}


BOOL CFileApi::IsFileExist(const string& filePath)
{
	return PathFileExists(filePath.c_str());
}


/*****************************************/
/////////////HTTP LOG/////////////////////
/****************************************/
BOOL CFileApi::tagHTTP_LOG::SaveHttpSend(LPHTTP_SEND_INFO lphttpSendInfo) {
	BYTE* pBt = (BYTE*)lphttpSendInfo->lpszHeaders;
	string cStr = "";
	
	if(pBt!=NULL){
		if (pBt[0] > 0 && pBt[1] == 0 && pBt[2] > 0 && pBt[3] == 0){
			cStr = CStringApi::WideCharToPChar((WCHAR*)lphttpSendInfo->lpszHeaders);
		}else {
			cStr = (TCHAR*)lphttpSendInfo->lpszHeaders;
		}
	}
	else {
		cStr = "";
	}

	string header = CStringApi::ReplaceUnEchoStrToC((TCHAR*)cStr.c_str(), cStr.length());
	string data = CStringApi::ReplaceUnEchoStrToC((TCHAR*)lphttpSendInfo->lpOptional, lphttpSendInfo->dwOptionalLength);
	DWORD outSize = header.length() + data.length() + sizeof(QWORD) * 2 + 100;
	TCHAR* outBuf = (TCHAR*)malloc(outSize);
	ZeroMemory(outBuf, outSize);


	//if(IsFileExist(filePath)){}
	auto rc = m_RequestRecordMap.find((QWORD)lphttpSendInfo->hHttp);

	wsprintf((TCHAR*)outBuf, _T("HTTP_SEND (%X):"),lphttpSendInfo->hHttp);
	string httpSend = outBuf;
	httpSend += rc->second.lpszURL;


	LogInfo(m_logPath, (TCHAR*)httpSend.c_str(), httpSend.length(), TRUE);
	wsprintf((TCHAR*)outBuf, _T("Len:%d\r\n"), lphttpSendInfo->dwOptionalLength);
	LogInfo(m_logPath, outBuf, lstrlen((TCHAR*)outBuf), TRUE);
	string tagHeader = "Header:\r\n";
	tagHeader += header;
	//wsprintf((TCHAR*)outBuf, _T("Header:%s\r\n"),  header.c_str());

	LogInfo(m_logPath, (TCHAR*)tagHeader.c_str(), tagHeader.length(), TRUE);
	
	string tagData = "Data:\r\n";
	tagData += data;

	LogInfo(m_logPath,(TCHAR*)(tagData.c_str()), tagData.length(), TRUE);
	free(outBuf);
	return TRUE;
}




string CFileApi::tagHTTP_LOG::GetRequestedFile(string& url) {
	// 查找第一个斜杠后的部分作为文件路径
	size_t startPos = url.rfind('/');
	if (startPos != string::npos) {
		// 从斜杠位置开始提取子字符串作为文件路径
		string fileName = url.substr(startPos+1);
		size_t qPos = fileName.rfind('?');
		if (qPos != -1) {
			fileName = fileName.substr(0, qPos);
		}
		return fileName;
	}

	// 如果没有斜杠，则返回空字符串
	return "";
}
BOOL CFileApi::tagHTTP_LOG::SaveHttpRecv(LPHTTP_RECV_INFO lphttpRecvInfo) {

	auto rc = m_RequestRecordMap.find((QWORD)lphttpRecvInfo->hHttp);
	BOOL inList = CSysApi::NET.CheckHttpReqeustInListFile(rc->second.lpszURI);

	string saveFile = GetPathFromFullPath(m_logPath) + GetRequestedFile(rc->second.lpszURL);
	if (!inList) {
		saveFile = GetPathFromFullPath(m_logPath) + CStringApi::Format("%X", (QWORD)rc->second.hRequest);
	}
	if (lphttpRecvInfo->lpBuffer != 0) {
		
		string data = CStringApi::ReplaceUnEchoStrToC((TCHAR*)lphttpRecvInfo->lpBuffer, lphttpRecvInfo->readSize);
		DWORD outSize = data.length() + sizeof(QWORD) * 2 + 100;
		TCHAR* outBuf = (TCHAR*)malloc(outSize);
		ZeroMemory(outBuf, outSize);
		
		if (rc->second.bFirstLog) {
			rc->second.bFirstLog = FALSE;

			wsprintf((TCHAR*)outBuf, _T("HTTP_RECV (%X):"), lphttpRecvInfo->hHttp);
			string httpRecv = outBuf;
			httpRecv += rc->second.lpszURL;
			LogInfo(m_logPath, (TCHAR*)httpRecv.c_str(), httpRecv.length(), TRUE);


			//wsprintf((TCHAR*)outBuf, _T("HTTP_RECV (%X):\r\n"), lphttpRecvInfo->hHttp);
			//LogInfo(m_logPath, outBuf, lstrlen((TCHAR*)outBuf), TRUE);
			wsprintf((TCHAR*)outBuf, _T("Len:%d\r\n"), lstrlen(outBuf));
			LogInfo(m_logPath, outBuf, lstrlen((TCHAR*)outBuf), TRUE);

			string tagData = "Data:\r\n";
			tagData += data;


			SaveBuffToFile(saveFile, (BYTE*)lphttpRecvInfo->lpBuffer, lphttpRecvInfo->readSize,TRUE, FALSE);

			LogInfo(m_logPath, (TCHAR*)tagData.c_str(), tagData.length(), TRUE);
		}
		else {

			SaveBuffToFile(saveFile, (BYTE*)lphttpRecvInfo->lpBuffer, lphttpRecvInfo->readSize, FALSE, TRUE);
			LogInfo(m_logPath, (TCHAR*)data.c_str(), data.length(), TRUE);
		}

	
		free(outBuf);
	}
	else if (lphttpRecvInfo->lpBufferOutW != 0) {

		string data = "";
		string header = "";
		int dataSize = 0;
		CDynamicBuffer dyBuffer;
		INTERNET_BUFFERSW* pNext = lphttpRecvInfo->lpBufferOutW;
		while (pNext) {
			dataSize += pNext->dwBufferLength;
			data += CStringApi::ReplaceUnEchoStrToC((TCHAR*)pNext->lpvBuffer, pNext->dwBufferLength);
			dyBuffer.Append(pNext->lpvBuffer, pNext->dwBufferLength);

			pNext = pNext->Next;
		}
		pNext = lphttpRecvInfo->lpBufferOutW;
		while (pNext) {
			header += CStringApi::ReplaceUnEchoStrToC((TCHAR*)pNext->lpcszHeader, pNext->dwHeadersLength);
			pNext = pNext->Next;
		}
		DWORD outSize = data.length() + header.length() + sizeof(QWORD) * 2 + 100;
		TCHAR* outBuf = (TCHAR*)malloc(outSize);
		ZeroMemory(outBuf, outSize);
		if (rc->second.bFirstLog) {
			rc->second.bFirstLog = FALSE;
			
			wsprintf((TCHAR*)outBuf, _T("HTTP_RECV (%X):"), lphttpRecvInfo->hHttp);
			string httpRecv = outBuf;
			httpRecv += rc->second.lpszURI;
			LogInfo(m_logPath, (TCHAR*)httpRecv.c_str(), httpRecv.length(), TRUE);

			wsprintf((TCHAR*)outBuf, _T("Len: %d\r\n"), lphttpRecvInfo->lpBufferOutW->dwBufferTotal);
			LogInfo(m_logPath, outBuf, lstrlen((TCHAR*)outBuf), TRUE);
			string tagHeader = "Header:\r\n";
			tagHeader += header;
			LogInfo(m_logPath, (TCHAR*)tagHeader.c_str(), tagHeader.length(), TRUE);

			string tagData = "Data:\r\n";
			tagData += data;
			if (inList) {
				SaveBuffToFile(saveFile, (BYTE*)dyBuffer.Buffer(), dyBuffer.Size() ,TRUE, FALSE);
			}
			LogInfo(m_logPath, (TCHAR*)tagData.c_str(), tagData.length(), TRUE);
		}
		else {
			if (inList) {
				SaveBuffToFile(saveFile, (BYTE*)dyBuffer.Buffer(), dyBuffer.Size(), FALSE,TRUE);
			}
			LogInfo(m_logPath, (TCHAR*)data.c_str(), data.length(), TRUE);
		}

		free(outBuf);
	}


	return TRUE;
}

void CFileApi::tagHTTP_LOG::SetLogPath(const string& logPath) {
	m_logPath = logPath;
}
void CFileApi::tagHTTP_LOG::SetConnectRecord(LPHTTP_CONNECT_RECORD lpCr) {
	HTTP_CONNECT_RECORD record = {};
	auto iter = m_ConnectRecordMap.find(lpCr->hConnect);
	if (iter == m_ConnectRecordMap.end()) {
		m_ConnectRecordMap.emplace(lpCr->hConnect, record);
		iter = m_ConnectRecordMap.find(lpCr->hConnect);
	}
	iter->second.hConnect = lpCr->hConnect;
	iter->second.lpszServerName = lpCr->lpszServerName.length() > 0 ? lpCr->lpszServerName : iter->second.lpszServerName;


	TCHAR outBuf[1024] = { NULL };
	//string header = CStringApi::ReplaceUnEchoStrToC((TCHAR*)lphttpSendInfo->lpszHeaders, lphttpSendInfo->dwHeadersLength);
	
	wsprintf((TCHAR*)outBuf, _T("HTTP_HOST (%X): %s\r\n\r\n"),lpCr->hConnect, lpCr->lpszServerName.c_str());
	LogInfo(m_logPath, outBuf, lstrlen((TCHAR*)outBuf), TRUE);
}
void CFileApi::tagHTTP_LOG::UnsetConnectRecord(HINTERNET hConnect) {
	auto iter = m_ConnectRecordMap.find((QWORD)hConnect);
	if (iter != m_ConnectRecordMap.end()) {
		m_ConnectRecordMap.erase((QWORD)hConnect);
	}
}
void CFileApi::tagHTTP_LOG::SetRequestRecord(LPHTTP_REQUEST_RECORD lpQr) {
	HTTP_REQUEST_RECORD record = {};
	auto iter = m_RequestRecordMap.find(lpQr->hRequest);
	if (iter == m_RequestRecordMap.end()) {
		m_RequestRecordMap.emplace(lpQr->hRequest, record);
		iter = m_RequestRecordMap.find(lpQr->hRequest);
		iter->second.bFirstLog = TRUE;

	}
	iter->second.hRequest = lpQr->hRequest;
	iter->second.hConnect = lpQr->hConnect;
	iter->second.lpMethod = lpQr->lpMethod.length() > 0 ? lpQr->lpMethod : iter->second.lpMethod;
	iter->second.lpResType = lpQr->lpResType.length() > 0 ? lpQr->lpResType : iter->second.lpResType;
	iter->second.lpszURI = lpQr->lpszURI.length() > 0 ? lpQr->lpszURI : iter->second.lpszURI;
	
	TCHAR outBuf[1024] = { NULL };
	auto con = m_ConnectRecordMap.find((QWORD)lpQr->hConnect);
	string url = con->second.lpszServerName + lpQr->lpszURI;
	iter->second.lpszURL = url;

	wsprintf((TCHAR*)outBuf, _T("HTTP_REQUEST (%X)(%X): \r\nMethod:%s\r\nResType:%s\r\nURL:%s\r\n\r\n"), lpQr->hConnect,lpQr->hRequest, lpQr->lpMethod.c_str(), lpQr->lpResType.c_str(), url.c_str());
	LogInfo(m_logPath, outBuf, lstrlen((TCHAR*)outBuf), TRUE);

}
void CFileApi::tagHTTP_LOG::UnsetRequestRecord(HINTERNET hRequest) {
	auto iter = m_RequestRecordMap.find((QWORD)hRequest);
	if (iter != m_RequestRecordMap.end()) {
		m_RequestRecordMap.erase((QWORD)hRequest);
	}
}



void CFileApi::OpenFolderAndSelectFile( const string& filePath)
{
	string folderPath = GetPathFromFullPath(filePath);
	

	// 打开目录并选中文件
	//ShellExecute(NULL, "open", folderPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	ShellExecute(NULL, "open", "explorer.exe", ("/select,\"" + filePath + "\"").c_str(), NULL, SW_SHOWNORMAL);
}