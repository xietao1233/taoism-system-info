#include "stdafx.h"
#include "Export.h"
#include "ThreadInfo.h"


//////////////////////////////////////////////////////////////////////////
//		���ݴ�����߳�id����̵߳���ʼ��ַ
//
FARPROC GetThreadStartAddress(UINT_PTR uThreadID)
{
	FARPROC pfnResult = 0, uValue = 0;
	HANDLE hThread = NULL;
	ZwQueryInformationThread pfnZwQueryInformationThread = NULL;

	__try
	{
		if (!uThreadID)		__leave;
		hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, (DWORD)uThreadID);
		if (!hThread)			__leave;
		pfnZwQueryInformationThread = (ZwQueryInformationThread)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwQueryInformationThread");
		if (!pfnZwQueryInformationThread)			__leave;
		if (pfnZwQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &uValue, sizeof(UINT_PTR), NULL) != STATUS_SUCCESS)		__leave;
		pfnResult = (FARPROC)uValue;
	}
	__finally
	{
		CloseHandle(hThread);
	}

	return pfnResult;
}

//////////////////////////////////////////////////////////////////////////
//		���ָ���̵߳�TEB
//
UINT_PTR GetThreadTEB(UINT_PTR uThreadID)
{
	UINT_PTR uResult = 0;
	HANDLE hThread = 0;
	ZwQueryInformationThread pfnZwQueryInformationThread = NULL;
	THREAD_BASIC_INFORMATION* pTBI = NULL;

	__try
	{
		if (!uThreadID)		__leave;
		hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, (DWORD)uThreadID);
		if (!hThread)			__leave;
		pfnZwQueryInformationThread = (ZwQueryInformationThread)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwQueryInformationThread");
		if (!pfnZwQueryInformationThread)			__leave;
		pTBI = new THREAD_BASIC_INFORMATION;
		if (pfnZwQueryInformationThread(hThread, ThreadBasicInformation, pTBI, sizeof(THREAD_BASIC_INFORMATION), NULL) != STATUS_SUCCESS)			__leave;
		uResult = (UINT_PTR)pTBI->TebBaseAddress;
	}
	__finally
	{
		delete pTBI;
		CloseHandle(hThread);
	}

	return uResult;
}
