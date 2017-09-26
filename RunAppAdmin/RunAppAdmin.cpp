#include "stdafx.h"
#include <windows.h>
#include <userenv.h>
#include <iostream>
//#include <string>

void PrintHelp();

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD     dwSize;
	HANDLE    hToken;
	LPVOID    lpvEnv = NULL;
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO         si = { 0 };
	WCHAR               szUserProfile[256] = L"";
	DWORD Error = 0;
	si.cb = sizeof(STARTUPINFO);

	TCHAR *name = NULL;// = L"Andrey";
	TCHAR *password = NULL;// = L"123456";
	TCHAR *cmd = NULL;// = L"C:\\Program Files (x86)\\Far Manager\\Far.exe";

	if (argc!=4)
	{
		PrintHelp();
		exit(0);
	}

	name = &*argv[1];
	password = &*argv[2];
	cmd = &*argv[3];

	
	if (argc != 4)
	{
		wprintf(L"Usage: %s [username] [password] [cmd]", argv[0]);
		wprintf(L"\n\n");
		return 0;
	}

	if (!LogonUser(name, NULL, password, LOGON32_LOGON_INTERACTIVE,	LOGON32_PROVIDER_DEFAULT, &hToken))
	{
		exit(GetLastError());
	}

	CreateEnvironmentBlock(&lpvEnv, hToken, FALSE);
	
	dwSize = sizeof(szUserProfile) / sizeof(WCHAR);

	if (!GetUserProfileDirectory(hToken, szUserProfile, &dwSize))
	{
		exit(GetLastError());
	}
	
	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	si.cb = sizeof(si);

	if (!CreateProcessWithLogonW(name, L"NT AUTHORITY", password,
		LOGON_WITH_PROFILE, NULL, cmd,
		CREATE_UNICODE_ENVIRONMENT, lpvEnv, szUserProfile,
		&si, &pi))
		exit(GetLastError());
	
	if (!DestroyEnvironmentBlock(lpvEnv))
		exit(GetLastError());

	CloseHandle(hToken);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
	return 0;
}

void PrintHelp()
{
	std::cout << std::endl;
	std::cout << "RunAppAdmin - application for run programs from admin account" << std::endl;
	std::cout << "Usage: RunAppAdmin.exe UserName UserPassword PathToExecutableModule" << std::endl;
	std::cout << "Example: RunAppAdmin.exe Administrator p@ssw0rd \"C:\\Windows\\notepad.exe D:\\Work\\1.txt\"" << std::endl;
	std::cout << "===================================================" << std::endl;
	std::cout << "andy.sinenko@gmail.com" << std::endl;
}

