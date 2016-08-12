#include "stdafx.h"
#include "Process.h"
#include "ProcessInfo.h"


CProcess::CProcess(UINT uProcessId)
{
	InitProcessInfo();
}

CProcess::CProcess(LPWSTR szProcessName)
{
	SetProcessName(szProcessName);
}

CProcess::~CProcess()
{
	ClearProcessName();
}

VOID CProcess::InitProcessInfo()
{
	m_ProcessName = NULL;
}

BOOL CProcess::SetProcessName(LPWSTR szProcessName)
{
	BOOL bResult = FALSE;
	ULONG uNameLen = MAX_PATH;
	
	__try
	{
		m_ProcessName = new WCHAR[uNameLen];
		assert(m_ProcessName);
		RtlZeroMemory(m_ProcessName, uNameLen * 2);
		if (wcscpy_s(m_ProcessName, uNameLen, szProcessName))			__leave;
		bResult = TRUE;
	}
	__finally
	{

	}

	return bResult;
}

VOID CProcess::ClearProcessName()
{
	if (m_ProcessName)
	{
		delete[] m_ProcessName;
		m_ProcessName = NULL;
	}
}
