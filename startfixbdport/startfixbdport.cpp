// startfixbdport.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "startfixbdport.h"
#include <direct.h>
#include <stdio.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	char buf1[256];
	_getcwd(buf1, sizeof(buf1));
	TCHAR szDll[256] = {0};
	sprintf(szDll, "%s\\fixbdeport.dll", buf1);
	
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	TCHAR szCommandLine[MAX_PATH] = {0};
	sprintf(szCommandLine, "%s\\_st.exe", buf1);

	CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	LPVOID Param = VirtualAllocEx(pi.hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(pi.hProcess, Param, (LPVOID)szDll, _tcslen(szDll)+ sizeof(TCHAR), NULL);

	HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, Param, CREATE_SUSPENDED, NULL);
	ResumeThread(pi.hThread);

	if (hThread)
	{
		ResumeThread(hThread);
		WaitForSingleObject(hThread, INFINITE);
	}
    return 0;
}


