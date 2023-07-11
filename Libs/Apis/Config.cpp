#pragma once
#include "Config.h"
#include <iostream>
#include <Windows.h>
#include <vector>
#pragma comment(lib, "Version.lib")
using namespace std;
string CConfig::m_filePath = "";
CConfig::CConfig()
{
}

CConfig::~CConfig()
{
}
/*
CConfig* CConfig::GetInstance()
{
	if (instance == NULL) {
		instance = new CConfig();
	}
	return instance;
}
*/
void CConfig::Init(const string& filePath)
{
	m_filePath = filePath;

}



string CConfig::GetValue(const string& section, const string& key, const string& defaultValue)
{
	char buffer[1024];
	GetPrivateProfileString(section.c_str(), key.c_str(), defaultValue.c_str(), buffer, sizeof(buffer), m_filePath.c_str());
	string retstr = buffer;
	CStringApi::StringReplace(retstr, "\\n","\x0a");
	CStringApi::StringReplace(retstr, "\\r", "\x0d");
	return retstr;
}

bool CConfig::SetValue(const string& section, const string& key, const string& value)
{
	return WritePrivateProfileString(section.c_str(), key.c_str(), value.c_str(), m_filePath.c_str()) != 0;
}

