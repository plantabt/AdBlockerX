#pragma once
#include "common.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <Windows.h>
class CStringApi
{

	public:
		static string StringToLower(string strin);
		static int IndexOf(const string& src, const string& search);
		static CHAR* pNextPointer;
		static BYTE* pDataBlock;
		static ULONG ConvertHexToDec(CHAR* szHex, BYTE nHexLen);
		
		static REGEX_RESULTS regex_match_all(const string& src, const string& pattern);
		static string ReplaceUnEchoStrToC(TCHAR* strBuf, DWORD strLen);
		static DWORD GetFirstInt(CHAR* srcXMLString, CHAR* endFlag);
		static CHAR* GetFirstString(CHAR* srcXMLString, CHAR* endFlag);
		static DWORD GetNextInt(CHAR* endFlag);
		static CHAR* GetNextString(CHAR* endFlag);

		static string Format(const char* pszFmt, ...);
		static wstring Format(const wchar_t* pszFmt, ...);
		static TCHAR* StrToLow(TCHAR* in);
		static TCHAR* StrToUp(TCHAR* in);
		static VOID FreeString(TCHAR* pStr);
		static string WideCharToPChar(WCHAR* in_text);
		static CHAR* IntStringToString(CHAR* in, DWORD lenstr, CHAR* split);
		static CHAR* StringToIntString(CHAR* in, DWORD lenstr, CHAR* split);
		static bool IsStrCmp(TCHAR* _srcstr, TCHAR* _cmpstr);
		static CHAR* MakeRetValue(CHAR* psrc, DWORD lenstr);
		static vector<string> Split(const string& str, const string& delimiter);
		static BYTE* IntStringToBuffer(CHAR* in, CHAR* split);
		static wstring PCharToWideChar(CHAR* in_text);
		static CHAR* WideCharToUtf8(WCHAR* in_text);
		static CHAR* PCharToUtf8(CHAR* in);
		static WCHAR* Utf8ToWideChar(CHAR* in);
		static BOOL HexStr2Hex(CHAR* InStr, BYTE* OutStr);
		static BOOL HexStr2Hex(WCHAR* InStr, BYTE* OutStr);
		static BOOL Hex2Str(BYTE* inHex, CHAR* outStr, int size);
		static BOOL Hex2Str(BYTE* inHex, WCHAR* outStr, int size);
		static BOOL HexToDec(CHAR* shex, int& idec);
		static BOOL HexToDec(WCHAR* shex, int& idec);
		static BOOL Hex2HexView(BYTE* inHex, TCHAR* outStr, int size, int line_width);
		static BOOL Hex2IsPrint(BYTE* inHex, TCHAR* outStr, int size, int line_width);
		static CHAR* PUTF8ToChar(CHAR* szInfo);
		static long Str2Long(string &str, int radix);
		static long Str2Long(WCHAR* str, int radix);
		static DWORD Str2DWORD(CHAR* str, int radix);
		static DWORD Str2DWORD(WCHAR* str, int radix);
		static double Str2Double(CHAR* str);
		static double Str2Double(WCHAR* str);
		static void StringReplace(string& strBase, string strSrc, string strDes);
		static string GetStringMD5(const string& strtomd5);
		static BOOL IsWideChar(const string& str);
		static BOOL IsWideChar(BYTE* pBuf);
};

