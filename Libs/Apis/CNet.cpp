#include "CNet.h"


CNet::CNet()
{
}


CNet::~CNet()
{
}


string CNet::GetWebPage(const string& url,const string& agent)
{
	string content="";

	HINTERNET hInternet = InternetOpen(agent.c_str(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL)
	{
		return content;
	}

	HINTERNET hConnect = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return content;
	}

	TCHAR buffer[4096] = { NULL };
	DWORD bytesRead = 0;
	while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0)
	{
		buffer[bytesRead] = '\0';
		content += buffer;
	}

	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	return content;
}