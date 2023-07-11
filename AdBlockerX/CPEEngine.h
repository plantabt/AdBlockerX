#pragma once
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include ""
using namespace std;
class CPEEngine
{
public:
	CPEEngine();
	~CPEEngine();
	static void BandNopOEP(const string& filepath);
private:
	static string m_filePath;
};

