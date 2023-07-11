#pragma once
#include <windows.h>
#include <string>
#include <tchar.h>
#include <Wininet.h>
#pragma comment(lib, "Wininet.lib")
using namespace std;
class CNet
{
public:
	CNet();
	~CNet();
	static string GetWebPage(const string& url, const string& agent);
};

