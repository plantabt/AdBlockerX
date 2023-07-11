#pragma once
#include "CStringApi.h"
#include <string>
using namespace std;
class CConfig
{
public:
	//static CConfig* GetInstance();

	static void Init(const string& filePath);

	static string GetValue(const string& section, const string& key, const string& defaultValue = "");
	static bool SetValue(const string& section, const string& key, const string& value);

private:
	//static CConfig *instance;
	CConfig();
	~CConfig();


	static string m_filePath;
};

