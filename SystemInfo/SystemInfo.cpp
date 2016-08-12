// SystemInfo.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Export.h"


//////////////////////////////////////////////////////////////////////////
//	�жϵ�ǰ�Ƿ�Ϊ64λ��ϵͳ
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
			OutputDebugStringW(L"��ǰ��64λϵͳ\n");

		}
		else
		{
			OutputDebugStringW(L"��ǰ��32λϵͳ\n");
		}
	}
	__finally
	{

	}

	return bResult;
}
