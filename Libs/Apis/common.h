#pragma once
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <sstream>
#include <string>
#include <regex>
#include <iostream>
#include <list>
#include <stdio.h>
#include <ctype.h>
#include <unordered_map>
#include <map>
#include <wininet.h>
#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Wininet.lib")
using namespace std;

#define QWORD unsigned long long

typedef struct tagCMD_ARGS {
	BYTE cmd;
	QWORD wparam;
	QWORD lparam;
}CMD_ARGS,*LPCMD_ARGS;

typedef QWORD(*CALL_METHOD)(CMD_ARGS);

typedef unordered_map<BYTE, CALL_METHOD> CALL_TABLE;


enum SEARCH_PAGE_TYPE
{
	SEARCH_PAGE_RW,
	SEARCH_PAGE_EXEC

};
typedef list<string> REGEX_RESULTS;
#define ANY_CODE 0x2A2A	//两个星号 作为一个字节的通配符号
