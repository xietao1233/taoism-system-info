// SystemInfo.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Export.h"


//////////////////////////////////////////////////////////////////////////
//	判断当前是否为64位的系统
//
BOOL IsWIndows64OS()
{
	BOOL bResult = FALSE;
	SYSTEM_INFO stSI = { 0 };

	__try
	{
		GetNativeSystemInfo(&stSI);
		if (stSI.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || stSI.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		{
			bResult = TRUE;
			OutputDebugStringW(L"当前是64位系统\n");

		}
		else
		{
			OutputDebugStringW(L"当前是32位系统\n");
		}
	}
	__finally
	{

	}

	return bResult;
}
