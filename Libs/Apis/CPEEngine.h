#pragma once
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include "CFileApi.h"
using namespace std;
class CPEEngine
{
public:
	CPEEngine();
	~CPEEngine();
	static void LockOEP(const string& filepath);
private:
	static string m_filePath;
};

