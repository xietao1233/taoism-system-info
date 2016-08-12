#include "stdafx.h"
#include "System.h"
#include "ProcessInfo.h"


CSystem::CSystem()
{
}

CSystem::~CSystem()
{
}

UINT CSystem::GetProcessId(LPWSTR pProcessName)
{
	UINT uResult = 0;
	ZwQuerySystemInformation pfnZwQuerySystemInformation = NULL;
	PSYSTEM_PROCESS_INFORMATION pSRI = NULL;
	LPBYTE pBuff = NULL;
	LPWSTR pName = NULL;
	SIZE_T uSize = 0, uNameLen = MAX_PATH;

	__try
	{
		pfnZwQuerySystemInformation = (ZwQuerySystemInformation)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwQuerySystemInformation");
		if (!pfnZwQuerySystemInformation)		__leave;

		//1. 获得请求内存的大小
		pfnZwQuerySystemInformation(SystemProcessInformation, NULL, NULL, (PULONG)&uSize);
		if (!uSize)			__leave;

		pBuff = new BYTE[uSize];
		RtlZeroMemory(pBuff, uSize);
		pSRI = (PSYSTEM_PROCESS_INFORMATION)pBuff;

		//2. 获得所有的进程信息
		if (pfnZwQuerySystemInformation(SystemProcessInformation, pSRI, uResult, (PULONG)&uSize) != STATUS_SUCCESS)			__leave;

		//3. 循环遍历所有进程
		pName = new WCHAR[uNameLen];
		do
		{
			if (pSRI->ImageName.Length > uNameLen)
			{
				delete[] pName;
				uNameLen = pSRI->ImageName.MaximumLength;
				pName = new WCHAR[uNameLen];
			}
			RtlZeroMemory(pName, uNameLen * 2);
			wcscpy_s(pName, uNameLen, pSRI->ImageName.Buffer);
			if (!_wcsicmp(pName, pProcessName))
			{
				uResult = (UINT)pSRI->ProcessId;
				break;
			}
			if (!pSRI->NextEntryOffset)
				break;
			pSRI = (PSYSTEM_PROCESS_INFORMATION)((LPBYTE)pSRI + pSRI->NextEntryOffset);
		} while (TRUE);
	}
	__finally
	{
		delete[] pName;
		delete[] pBuff;
	}

	return uResult;
}
