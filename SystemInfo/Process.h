#pragma once

#include "System.h"

class CProcess
	: public CSystem
{
public:
	CProcess(UINT uProcessId);
	CProcess(LPWSTR szProcessName);
	~CProcess();

private:
	UINT m_ProcessId;
	LPWSTR m_ProcessName;

	VOID InitProcessInfo();
	BOOL SetProcessName(LPWSTR szProcessName);
	VOID ClearProcessName();
};

