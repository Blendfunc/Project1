// QQLLKAssist.cpp: 定义控制台应用程序的入口点。
//
#include"stdafx.h"
#include"stdio.h"
#include"windows.h"

int main()
{
	DWORD LLKID = 14860;
	HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, LLKID);
	LPVOID lpAddress = (LPVOID)1679208; // 0x00199F68
	SIZE_T dwSize = 19 * 11;
	DWORD flOldProtect = 0;
	PDWORD lpflOldProtect = &flOldProtect;
	BOOL bv = VirtualProtectEx(h, lpAddress, dwSize, PAGE_EXECUTE_READWRITE, lpflOldProtect);
	void * p = malloc(19 * 10);
	memset(p, 0, 19 * 11);
	BOOL b = WriteProcessMemory(h, lpAddress, (LPCVOID)p, 19 * 10, NULL);
	VirtualProtectEx(h, lpAddress, dwSize, flOldProtect, lpflOldProtect);
	CloseHandle(h);
	free(p);
}

