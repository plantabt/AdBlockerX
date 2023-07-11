#include "CStringApi.h"


CHAR* CStringApi::pNextPointer = 0;
BYTE* CStringApi::pDataBlock = 0;
TCHAR* CStringApi::StrToLow(TCHAR* in)
{
	return CharLower(in);
}
TCHAR* CStringApi::StrToUp(TCHAR* in)
{
	return CharUpper(in);
}


wstring CStringApi::Format(const wchar_t* pszFmt, ...)
{
	wstring str;
	va_list args;
	va_start(args, pszFmt);

	int nLength = _vscwprintf(pszFmt, args);
	nLength += 1;  //上面返回的长度是包含\0，这里加上
	vector<wchar_t> chars(nLength);

	_vsnwprintf_s(chars.data(), nLength, nLength, pszFmt, args);

	str.assign(chars.data());

	va_end(args);
	return str;
}
string CStringApi::Format(const char* pszFmt, ...)
{
	string str;
	va_list args;
	va_start(args, pszFmt);

	int nLength = _vscprintf(pszFmt, args);
	nLength += 1;  //上面返回的长度是包含\0，这里加上
	vector<char> chars(nLength);

	_vsnprintf_s(chars.data(), nLength, nLength, pszFmt, args);

	str.assign(chars.data());

	va_end(args);
	return str;
}
#define MD5_DIGEST_LENGTH 16

string CStringApi::GetStringMD5(const string& strtomd5) {
	
	HCRYPTPROV hProv = 0;
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {

		throw runtime_error("Failed to acquire cryptographic context");
	}

	HCRYPTHASH hHash = 0;
	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
		CryptReleaseContext(hProv, 0);

		throw runtime_error("Failed to create hash");
	}


	BYTE* buffer = (BYTE*)strtomd5.c_str();
	DWORD bytesRead = strtomd5.length();

	
		if (!CryptHashData(hHash, buffer, bytesRead, 0)) {
			CryptDestroyHash(hHash);
			CryptReleaseContext(hProv, 0);
			throw runtime_error("Failed to hash data");
		}


	


	BYTE hashValue[MD5_DIGEST_LENGTH];
	DWORD hashLength = MD5_DIGEST_LENGTH;
	if (!CryptGetHashParam(hHash, HP_HASHVAL, hashValue, &hashLength, 0)) {
		CryptDestroyHash(hHash);
		throw runtime_error("Failed to get hash value");
	}

	CryptReleaseContext(hProv, 0);
	CryptDestroyHash(hHash);

	ostringstream md5Stream;
	md5Stream << hex << setfill('0');
	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		md5Stream << setw(2) << static_cast<int>(hashValue[i]);
	}

	return md5Stream.str();
}


BOOL CStringApi:: IsWideChar(const string& str) {
	locale loc("");
	wstring_convert<codecvt_utf8<wchar_t>> converter;

	try {
		converter.from_bytes(str);
	}
	catch (const range_error&) {
		return FALSE; // 字符串是窄字符
	}

	return TRUE; // 字符串是宽字符
}
BOOL CStringApi::IsWideChar(BYTE* pBuf) {
	BOOL bTrue=TRUE;
	if(pBuf[0]>0 && pBuf[1]>0){
		bTrue = FALSE; // 字符串是窄字符
	}
	else if (pBuf[0] > 0 && *(WORD*)(pBuf + 1) == 0) //判断是否为单字符
	{
		bTrue = FALSE;// 字符串是窄字符
	}else {
		bTrue = TRUE;// 字符串是宽字符
	}
	return bTrue; 
}
void CStringApi::StringReplace(string& strBase, string strSrc, string strDes)
{
	string::size_type pos = 0;
	string::size_type srcLen = strSrc.size();
	string::size_type desLen = strDes.size();
	pos = strBase.find(strSrc, pos);
	while ((pos != string::npos))
	{
		strBase.replace(pos, srcLen, strDes);
		pos = strBase.find(strSrc, (pos + desLen));
	}
}
bool CStringApi::IsStrCmp(TCHAR* _srcstr, TCHAR* _cmpstr)
{
	return lstrcmpi(_srcstr, _cmpstr) == 0 ? true : false;
}
CHAR* CStringApi::StringToIntString(CHAR* in, DWORD lenstr, CHAR* split)
{
	BYTE vInt = 0;
	CHAR* beginPtr = in;
	if (!beginPtr)return 0;
	int encLen = 0;
	if (!lenstr)
		encLen = lstrlenA(in);
	else
		encLen = lenstr;
	CHAR* sIntString = (CHAR*)GlobalAlloc(GPTR, (encLen + 1) * 4);
	//static char sIntString[MAX_PATH];
	//ZeroMemory(sIntString,(encLen+1)*4);
	while ((in - beginPtr) != encLen)
	{
		vInt = (BYTE) * (in);
		wsprintfA((char*)(sIntString + lstrlenA(sIntString)), "%u%s", vInt, split);
		in++;
	}
	*(CHAR*)(sIntString + lstrlenA(sIntString) - 1) = 0;
	//free(sIntString);
	return sIntString;
}
vector<string> CStringApi::Split(const string& str, const string& delimiter)
{
	vector<string> result;
	size_t start = 0;
	size_t end = str.find(delimiter);

	while (end != string::npos)
	{
		string token = str.substr(start, end - start);
		result.push_back(token);
		start = end + delimiter.length();
		end = str.find(delimiter, start);
	}

	string lastToken = str.substr(start);
	result.push_back(lastToken);

	return result;
}

CHAR* CStringApi::MakeRetValue(CHAR* psrc, DWORD lenstr)
{
	CHAR* retStr = 0;
	//CHAR *P;
	int encLen = 0;
	if (!lenstr)
		encLen = lstrlenA(psrc);
	else
		encLen = lenstr;
	//	P = StringToIntString(psrc,lenstr,split);
	retStr = (CHAR*)GlobalAlloc(GPTR, (encLen + 1) + MAX_PATH);
	lstrcpyA(retStr, "<string>");
	lstrcatA(retStr, psrc);
	lstrcatA(retStr, "</string>");
	//wsprintfA(retStr,"<string>%s</string>",psrc);

	//FreeString((BYTE*)P);
	return retStr;
}

CHAR* CStringApi::GetNextString(CHAR* endFlag)
{
	CHAR* pbegin = pNextPointer;
	pNextPointer = StrStrIA(pNextPointer, endFlag);
	if (!pNextPointer)return 0;
	*pNextPointer = 0;
	pNextPointer++;
	if (!pbegin)return 0;
	return pbegin;
}

DWORD CStringApi::GetNextInt(CHAR* endFlag)
{
	CHAR* ptr = GetNextString(endFlag);
	if (!ptr)return -1;
	return StrToIntA(ptr);
}

CHAR* CStringApi::GetFirstString(CHAR* srcXMLString, CHAR* endFlag)
{
	//CHAR *beginFlag = "<string>",
	CHAR* pbegin = srcXMLString;
	//pbegin=(CHAR*)(StrStrIA(srcXMLString,beginFlag)+lstrlenA(beginFlag));
	if (!pbegin)return 0;
	pNextPointer = StrStrIA(pbegin, endFlag);
	*pNextPointer = 0;
	pNextPointer++;
	return pbegin;
}
DWORD CStringApi::GetFirstInt(CHAR* srcXMLString, CHAR* endFlag)
{
	return StrToIntA(GetFirstString(srcXMLString, endFlag));
}
string CStringApi::ReplaceUnEchoStrToC(TCHAR* strBuf, DWORD strLen) {
	string strOut = "";
	int cnt = 0;
	for (; cnt < strLen; cnt++) {
		BYTE ec = (BYTE)strBuf[cnt];
		if (!isgraph(ec)) {
			TCHAR fmt[10] = { NULL };
			if (ec == 0x20 || ec == 0x0d || ec == 0x0a) {
				strOut += ec;
			}
			else {
				wsprintf(fmt, "0x%02X", ec);
				strOut += fmt;
			}
		}
		else {
			strOut += ec;
		}

	}
	return strOut;
}

int CStringApi::IndexOf(const string &src, const string& search) {
	return src.find(search);
}
string CStringApi::StringToLower(string strin) {
	
	transform(strin.begin(), strin.end(), strin.begin(), [](unsigned char c) {return tolower(c); });
	return strin;
}
BYTE* CStringApi::IntStringToBuffer(CHAR* in, CHAR* split)
{

	BYTE vInt = 0, * siPtr = 0, * outBuff = 0;;
	//CHAR *beginPtr = in;
	DWORD encLen = 0;
	encLen = lstrlenA(in);

	siPtr = outBuff = (BYTE*)GlobalAlloc(GPTR, (encLen + 1));
	vInt = (BYTE)GetFirstInt(in, split);
	do
	{
		*(BYTE*)(siPtr) = vInt;
		vInt = (BYTE)GetNextInt(split);
		siPtr++;
	} while ((DWORD)(pNextPointer - in) < encLen);
	//*(CHAR*)(sIntString+lstrlenA(sIntString)-1)=0;
	//free(sIntString);
	return outBuff;
}
CHAR* CStringApi::IntStringToString(CHAR* in, DWORD lenstr, CHAR* split)
{

	BYTE vInt = 0;
	//CHAR *beginPtr = in;
	int encLen = 0;
	if (!lenstr)
		encLen = lstrlenA(in);
	else
		encLen = lenstr;
	CHAR* siPtr = 0, * sIntString = 0;
	siPtr = sIntString = (CHAR*)GlobalAlloc(GPTR, (encLen + 1));
	//static char sIntString[MAX_PATH];
	//ZeroMemory(sIntString,(encLen+1)*2);
	vInt = (CHAR)GetFirstInt(in, split);
	while (vInt != 0xFF)
	{
		*(BYTE*)(siPtr) = vInt;
		vInt = (BYTE)GetNextInt(split);
		siPtr++;
	}
	//*(CHAR*)(sIntString+lstrlenA(sIntString)-1)=0;
	//free(sIntString);
	return sIntString;
}

VOID CStringApi::FreeString(TCHAR* pStr)
{

	GlobalFree(pStr);
}
wstring CStringApi::PCharToWideChar(CHAR* in_text)
{

	if (!in_text)return 0;
	if (!*in_text)return 0;
	WCHAR* pret;
	DWORD dwNum;
	dwNum = MultiByteToWideChar(CP_ACP, NULL, in_text, -1, NULL, 0);
	pret = (WCHAR*)GlobalAlloc(GPTR, dwNum * 2 + 2);
	MultiByteToWideChar(CP_ACP, NULL, in_text, -1, pret, dwNum);
	wstring wstr = pret;
	GlobalFree(pret);
	return wstr;
}

BOOL CStringApi::HexToDec(WCHAR* shex, int& idec)
{
	string outc = WideCharToPChar(shex);
	CHAR* ptr = (CHAR*)outc.c_str();
	BOOL ret = HexToDec(ptr, idec);
	FreeString(ptr);
	return ret;
}
BOOL CStringApi::HexToDec(CHAR* shex, int& idec)
{
	int i, mid;

	int len = lstrlenA(shex);
	if (len % 3 == 1) return FALSE;
	CHAR* pCpyStr = (CHAR*)malloc(len * 2);
	CHAR* pTmpStr = pCpyStr;
	ZeroMemory(pTmpStr, len * 2);
	CopyMemory(pTmpStr, shex, len);
	mid = 0; idec = 0;
	for (i = 0; i < len; i++)
	{

		*pTmpStr |= 0x20;//全转小写
		if (*pTmpStr >= '0' && *pTmpStr <= '9')
		{
			mid = *pTmpStr - '0';
		}
		else if (*pTmpStr >= 'a' && *pTmpStr <= 'f')
		{
			mid = *pTmpStr - 'a' + 10;
		}
		else
		{
			mid = 0;
		}
		mid <<= ((len - i - 1) << 2);
		idec |= mid;
		pTmpStr++;
	}
	free(pCpyStr);
	return TRUE;
}
#ifdef UNICODE
BOOL CStringApi::Hex2Str(BYTE* inHex, WCHAR* outStr, int size)
{
	WCHAR* ptr = outStr;
	for (int cnt = 0; cnt < size; cnt++)
	{
		wsprintfW(ptr, _T("%0.2X "), inHex[cnt]);
		ptr += 3;
	}

	return TRUE;
}
#else
BOOL CStringApi::Hex2Str(BYTE* inHex, CHAR* outStr, int size)
{
	CHAR* ptr = outStr;
	for (int cnt = 0; cnt < size; cnt++)
	{
		wsprintfA(ptr, "%0.2X ", inHex[cnt]);
		ptr += 3;
	}

	return TRUE;
}
#endif
long CStringApi::Str2Long(string &str, int radix)
{
	return strtol(str.c_str(), NULL, radix);
}
long CStringApi::Str2Long(WCHAR* str, int radix)
{
	return wcstol(str, NULL, radix);
}
DWORD CStringApi::Str2DWORD(CHAR* str, int radix)
{
	return strtoul(str, NULL, radix);
}
DWORD CStringApi::Str2DWORD(WCHAR* str, int radix)
{
	return wcstoul(str, NULL, radix);
}
double CStringApi::Str2Double(CHAR* str)
{
	return strtod(str, NULL);
}
double CStringApi::Str2Double(WCHAR* str)
{
	return wcstod(str, NULL);
}
/*
long Str2Long(TCHAR *str, int radix)
{
#ifdef _UNICODE
		return wcstol(str, NULL,radix);
#else
		return strtol(str, NULL,radix);
#endif
	}
	DWORD Str2DWORD(TCHAR* str, int radix)
	{
#ifdef _UNICODE
		return wcstoul(str, NULL, radix);
#else
		return strtoul(str, NULL, radix);
#endif
	}
	double Str2Double(TCHAR* str)
	{
#ifdef _UNICODE
		return wcstod(str, NULL);
#else
		return strtod(str, NULL);
#endif
	}*/
BOOL CStringApi::Hex2HexView(BYTE* inHex, TCHAR* outStr, int size, int line_width)
{
	int lines = 0;
	int cnt = 0;
	int remain = 0;
	//TCHAR *outStr = new TCHAR[size * 4];
	TCHAR* hexStr = new TCHAR[size * 4];
	TCHAR lineStr[10];
	ZeroMemory(hexStr, sizeof(TCHAR) * size * 4);

	//格式化成编辑器显示格式
	if (line_width == 0)
	{
		CStringApi::Hex2Str(inHex, outStr, size);

	}
	else
	{
		CStringApi::Hex2Str(inHex, hexStr, size);
		if (size > line_width)
		{
			for (lines = 0; lines < size / line_width; lines++)
			{
				wsprintf(lineStr, _T("%0.4X "), lines);
				lstrcpy(&outStr[lines * (6 + line_width * 3)], lineStr);
				for (cnt = 0; cnt < line_width; cnt++)
				{
					CopyMemory(&outStr[lines * (6 + line_width * 3) + cnt * 3 + 5], &hexStr[lines * 3 * line_width + cnt * 3], sizeof(TCHAR) * 3);
				}
				lstrcat(outStr, _T("\n"));
			}
		}
		wsprintf(lineStr, _T("%0.4X "), lines);
		lstrcpy(&outStr[lines * (6 + line_width * 3)], lineStr);
		for (remain = 0; remain < size - (size / line_width) * line_width; remain++)
		{

			CopyMemory(&outStr[lines * (6 + line_width * 3) + (remain) * 3 + 5], &hexStr[lines * 3 * line_width + (remain) * 3], sizeof(TCHAR) * 3);

		}
	}

	delete hexStr;
	return TRUE;
}
BOOL CStringApi::Hex2IsPrint(BYTE* inHex, TCHAR* outStr, int size, int line_width)
{
	int lines = 0;
	int cnt = 0;
	int remain = 0;
	if (size > line_width)
	{
		for (lines = 0; lines < size / line_width; lines++)
		{

			for (cnt = 0; cnt < line_width; cnt++)
			{
				if (iswprint(inHex[lines * line_width + cnt]))
				{
					wsprintf(&outStr[lines * (line_width + 1) + cnt], _T("%c"), inHex[lines * line_width + cnt]);//一行17个字符
				}
				else
				{
					lstrcat(outStr, _T("."));
				}
			}
			lstrcat(outStr, _T("\n"));
		}
	}
	for (remain = 0; remain < size - (size / line_width) * line_width; remain++)
	{
		if (iswprint(inHex[lines * line_width + remain]))
		{
			wsprintf(&outStr[lines * (line_width + 1) + remain], _T("%c"), inHex[lines * line_width + remain]);//一行17个字符
		}
		else
		{
			lstrcat(outStr, _T("."));
		}
	}
	return TRUE;
}
string CStringApi::WideCharToPChar(WCHAR* in_text)
{

	if (!in_text)return 0;
	if (!*in_text)return 0;
	CHAR* pret;
	DWORD dwNum;
	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, in_text, -1, NULL, 0, NULL, FALSE);
	pret = (CHAR*)GlobalAlloc(GPTR, dwNum + 1);
	WideCharToMultiByte(CP_OEMCP, NULL, in_text, -1, pret, dwNum, NULL, FALSE);
	string retstr = pret;
	GlobalFree(pret);
	return retstr;
}
CHAR* CStringApi::WideCharToUtf8(WCHAR* in_text)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, in_text, -1, NULL, 0, NULL, NULL);
	CHAR* szUtf8 = (CHAR*)GlobalAlloc(GPTR, len + 1);
	ZeroMemory(szUtf8, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, in_text, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}
CHAR* CStringApi::PCharToUtf8(CHAR* in)
{
	CHAR* szUtf8;
	WCHAR* wszUtf8;
	int len;
	len = MultiByteToWideChar(CP_ACP, 0, in, -1, NULL, 0);
	wszUtf8 = new WCHAR[len + 1];
	ZeroMemory(wszUtf8, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, in, -1, wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
	szUtf8 = (CHAR*)GlobalAlloc(GPTR, len + 1);
	ZeroMemory(szUtf8, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL, NULL);
	delete[] wszUtf8;
	return szUtf8;
}
WCHAR* CStringApi::Utf8ToWideChar(CHAR* in)
{
	WCHAR* wszUtf8 = NULL;
	int len = 0;
	len = MultiByteToWideChar(CP_UTF8, 0, in, -1, NULL, 0);
	wszUtf8 = new WCHAR[len + 1];
	ZeroMemory(wszUtf8, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, in, -1, wszUtf8, len);
	return wszUtf8;
}

CHAR* CStringApi::PUTF8ToChar(CHAR* szInfo)
{

	int nCount = MultiByteToWideChar(CP_UTF8, 0, szInfo, -1, NULL, 0);
	WCHAR* pWideChar = new WCHAR[nCount + 1];
	ZeroMemory(pWideChar, (nCount + 1) * sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8, 0, szInfo, -1, pWideChar, nCount);

	//return szwchar;


	CHAR* pret;
	DWORD dwNum;
	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, pWideChar, -1, NULL, 0, NULL, FALSE);
	pret = (CHAR*)GlobalAlloc(GPTR, dwNum + 1);
	WideCharToMultiByte(CP_OEMCP, NULL, pWideChar, -1, pret, dwNum, NULL, FALSE);
	delete[]pWideChar;

	return pret;

}

BOOL CStringApi::HexStr2Hex(WCHAR* InStr, BYTE* OutStr)
{
	if (!OutStr)return FALSE;
	string ouc = WideCharToPChar(InStr);
	CHAR* p = (CHAR*)ouc.c_str();
	HexStr2Hex(p, OutStr);
	FreeString(p);
	return TRUE;
}


BOOL CStringApi::HexStr2Hex(CHAR* InStr, BYTE* OutStr)
{
	if (!OutStr)return FALSE;
	DWORD i = 0, j = 0;
	DWORD len = 0;
	BYTE* Str = OutStr;

	len = lstrlenA(InStr) - 2;//-0x
	Str = (BYTE*)GlobalAlloc(GPTR, len + 2);
	if ((len % 2) != 0)
	{
		GlobalFree(Str);

		return FALSE;
	}

	ZeroMemory(OutStr, len / 2);

	lstrcpyA((char*)(Str), InStr);


	for (i = 0; i < len; i++)
	{
		if ((Str[i] > 0x2f) && (Str[i] < 0x3a))
		{
			Str[i] -= 0x30;
		}
		else if ((Str[i] > 64) && (Str[i] < 0x47))
		{
			Str[i] -= 55;
		}
		else if ((Str[i] > 0x60) && (Str[i] < 0x67))
		{
			Str[i] -= 0x57;
		}
		else
		{
			GlobalFree(Str);
			return FALSE;
		}
	}

	i = 0;
	while (i < len)
	{ // 转换 
		OutStr[j] = Str[i] * 16 + Str[i + 1];
		i += 2;
		j++;
	}

	GlobalFree(Str);
	return TRUE;
}

REGEX_RESULTS CStringApi::regex_match_all(const string& src, const string& pattern) {
	REGEX_RESULTS matchs;
	smatch match;
	regex pat(pattern);   //regex 表示一个正则表达式类
	auto citer = src.cbegin();
	while (regex_search(citer, src.cend(), match, pat))//循环匹配
	{
		citer = match[0].second;

		for (size_t i = 1; i < match.size(); ++i)
		{
			matchs.push_back(match[i]);
		}
	}
	matchs.push_back(citer._Ptr);
	return matchs;
}



ULONG CStringApi::ConvertHexToDec(CHAR* szHex, BYTE nHexLen)
{
	CHAR c = 0;
	int nShift = 0;
	ULONG nDec = 0;
	nHexLen <<= 1;
	if (szHex[nHexLen - 1] == '*')return 0;
	while (nHexLen > 0)
	{

		c = szHex[nHexLen - 1];
		if (c >= '0' && c <= '9')
		{
			nDec += ((ULONG)(c - '0')) << nShift;
		}
		else if (c >= 'A' && c <= 'F')
		{
			nDec += ((ULONG)(10 + c - 'A')) << nShift;
		}
		else if (c >= 'a' && c <= 'f')
		{
			nDec += ((ULONG)(10 + c - 'a')) << nShift;
		}
		--nHexLen;
		nShift += 4;
	}
	return nDec;
}
