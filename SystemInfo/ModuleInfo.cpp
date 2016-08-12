#include "stdafx.h"
#include "Export.h"
#include "ModuleInfo.h"
#include <Psapi.h>


//////////////////////////////////////////////////////////////////////////
//	由 EnumProcessModules 函数获得 指定进程的模块信息
//
BOOL GetProcessPathNameByEnumProcessModules(UINT_PTR uProcessID, __out LPWSTR pOutBuff, SIZE_T uBuffSize)
{
	BOOL bResult = FALSE;
	HANDLE hProcess = NULL;
	DWORD dwResult = 0, dwSize = 0;
	UINT_PTR u = 0;
	HMODULE* pModuleData = NULL;
	MODULEINFO stModuleInfo = { 0 };

	__try
	{
		if (!uProcessID || !pOutBuff || !uBuffSize)		__leave;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)uProcessID);
		if (!hProcess)		__leave;
		if (!dwSize)
			dwSize = 1;
		
		do 
		{
			pModuleData = new HMODULE[dwSize];
			RtlZeroMemory(pModuleData, sizeof(HMODULE)*dwSize);
			EnumProcessModules(hProcess, (HMODULE*)pModuleData, dwSize*sizeof(HMODULE), &dwResult);
			if (dwSize < (dwResult / sizeof(HMODULE)))		//大小不足，重新申请内存
			{
				delete[] pModuleData;
				dwSize = dwResult / sizeof(HMODULE);
			}
			else
				break;
		} while (TRUE);

		for (u = 0; u < dwSize; u++);
		{
			//循环遍历所有模块句柄
			//获取模块名与路径
			GetModuleBaseName(hProcess, pModuleData[u], pOutBuff, (DWORD)uBuffSize);			//仅获得模块名
			GetModuleFileNameEx(hProcess, pModuleData[u], pOutBuff, (DWORD)uBuffSize);		//获得模块全路径
			//获取模块基址、大小与入口点
			if (GetModuleInformation(hProcess, pModuleData[u], &stModuleInfo, sizeof(stModuleInfo)))
				MessageBoxW(NULL, (LPCWSTR)stModuleInfo.lpBaseOfDll, L"Title", MB_OK);
		}		

		bResult = TRUE;
	}
	__finally
	{
		delete[] pModuleData;
		CloseHandle(hProcess);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//	由 ZwQuerySystemInformation 函数获取系统驱动模块信息
//
BOOL GetAllDriverNameByQuerySystemInformation(__out LPWSTR pOutBuff, SIZE_T uBuffSize)
{
	BOOL bResult = FALSE;
	ULONG uResult = 0;
	ZwQuerySystemInformation pfnZwQuerySystemInformation = NULL;
	LPBYTE pModuleData = NULL;
	PSYSTEM_MODULE_INFORMATION pSMI = NULL;

	__try
	{
		pfnZwQuerySystemInformation = (ZwQuerySystemInformation)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwQuerySystemInformation");
		if (!pfnZwQuerySystemInformation)		__leave;
		pfnZwQuerySystemInformation(SystemModuleInformation, NULL, NULL, &uResult);
		if (!uResult)			__leave;
		pModuleData = new BYTE[uResult];
		RtlZeroMemory(pModuleData, uResult);
		if (pfnZwQuerySystemInformation(SystemModuleInformation, pModuleData, uResult, &uResult) != STATUS_SUCCESS)			__leave;
		pSMI = (PSYSTEM_MODULE_INFORMATION)pModuleData;

// 		lv.mask = LVIF_TEXT;
		for (UINT_PTR u = 0; u < pSMI->Count; u++)
		{
			//遍历所有驱动模块 如：\SystemRoot\system32\ntoskrnl.exe
// 			lv.iItem = ListView_GetItemCount(hList);
// 			lv.iSubItem = 0;
// 			lv.pszText = pSMI->Module[u].ImageName;
// 			SendMessageA(hList, LVM_INSERTITEMA, NULL, (LPARAM)&lv);
		}

		bResult = TRUE;
	}
	__finally
	{
		delete[] pModuleData;
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//	从文件头中获得本进程模块大小(仅支持32位)
//
BOOL GetImageSizeByHeader(HMODULE hMod, __out SIZE_T &uModSize)
{
	BOOL bResult = FALSE;
	IMAGE_DOS_HEADER* pIDH = 0;
	IMAGE_OPTIONAL_HEADER32* pIOH = NULL;

	__try
	{
		if (!hMod)				__leave;
		uModSize = 0;

		pIDH = (IMAGE_DOS_HEADER*)hMod;
		pIOH = (IMAGE_OPTIONAL_HEADER32*)((UINT_PTR)pIDH->e_lfanew + (UINT_PTR)hMod + 4 + sizeof(IMAGE_FILE_HEADER));
		if (!pIOH->SizeOfImage)
			__leave;
		uModSize = pIOH->SizeOfImage;

		bResult = TRUE;
	}
	__finally
	{

	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//	获得LDR指针(支持64位与32位)
//
PLDR_MODULE GetLdrModulePtr()
{
	PLDR_MODULE pResult = NULL;
	UINT_PTR* pPtr;

	__try
	{
		pPtr = (UINT_PTR*)NtCurrentTeb();
		pPtr += 0xc;		//TEB地12个成员即为PEB指针
		pPtr = (UINT_PTR*)*pPtr;
		pPtr += 0x3;
		pPtr = (UINT_PTR*)*pPtr;
		pResult = (PLDR_MODULE)(pPtr + 0x3);
	}
	__finally
	{

	}

	return pResult;
}

//////////////////////////////////////////////////////////////////////////
//	从LDR中获得本进程模块大小(支持64位与32位)
//
BOOL GetImageSizeByLDR(HMODULE hMod, __out SIZE_T &uModSize)
{
	BOOL bResult = FALSE;
	PLDR_MODULE pNext = NULL, pLast = NULL;
	PLDR_MODULE pCurrent = NULL, pFirst = NULL;

	__try
	{
		if (!hMod)				__leave;
		uModSize = 0;

		pFirst = GetLdrModulePtr();
		if (!pFirst)		__leave;
		pCurrent = (PLDR_MODULE)*(UINT_PTR*)pFirst;
		
		do
		{
			pNext = (PLDR_MODULE)pCurrent->InLoadOrderModuleList.Flink;
			pLast = (PLDR_MODULE)pCurrent->InLoadOrderModuleList.Blink;
			if (pCurrent->BaseAddress == hMod)
			{
				if (pCurrent->SizeOfImage)
					uModSize = pCurrent->SizeOfImage;
				break;
			}
			pCurrent = pNext;
		} while (pCurrent != pFirst);

		bResult = TRUE;
	}
	__finally
	{

	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//		根据传入的地址和进程ID判断在该进程中属于哪个模块，并输出该模块的名字(32位系统使用)
//
BOOL GetModuleNameByAddress32(UINT uProcessID, UINT_PTR uDestAddress, __out LPWSTR pModuleName, SIZE_T uNameSize)
{
	BOOL bResult = FALSE;
	HANDLE hProcess = NULL;
	DWORD dwResult = 0, dwSize = 0;
	UINT_PTR uIndex = 0;
	HMODULE* pModuleData = NULL;
	MODULEINFO stModuleInfo = { 0 };

	__try
	{
		if (!uProcessID || !pModuleName || !uNameSize || !uDestAddress)		__leave;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, uProcessID);
		if (!hProcess)		__leave;
		if (!dwSize)
			dwSize = 1;
		RtlZeroMemory(pModuleName, uNameSize);

		do
		{
			pModuleData = new HMODULE[dwSize];
			RtlZeroMemory(pModuleData, sizeof(HMODULE)*dwSize);
			EnumProcessModules(hProcess, (HMODULE*)pModuleData, dwSize*sizeof(HMODULE), &dwResult);
			if (dwSize < (dwResult / sizeof(HMODULE)))		//大小不足，重新申请内存
			{
				delete[] pModuleData;
				dwSize = dwResult / sizeof(HMODULE);
			}
			else
				break;
		} while (TRUE);

		for (uIndex = 0; uIndex < dwSize; uIndex++)
		{
			//循环遍历所有模块句柄
			//获取模块基址、大小与入口点
			if (GetModuleInformation(hProcess, pModuleData[uIndex], &stModuleInfo, sizeof(stModuleInfo)))
			{
				if (uDestAddress>(UINT_PTR)stModuleInfo.lpBaseOfDll && uDestAddress < ((UINT_PTR)stModuleInfo.lpBaseOfDll + stModuleInfo.SizeOfImage))
				{
					GetModuleBaseName(hProcess, pModuleData[uIndex], pModuleName, (DWORD)uNameSize);
					break;
				}
			}
		}

		bResult = TRUE;
	}
	__finally
	{
		delete[] pModuleData;
		CloseHandle(hProcess);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//		根据传入的地址和进程ID判断在该进程中属于哪个模块，并输出该模块的名字(64位系统使用)
//
BOOL GetModuleNameByAddress64(UINT uProcessID, UINT_PTR uDestAddress, __out LPWSTR pModuleName, SIZE_T uNameSize)
{
	BOOL bResult = FALSE;
	HANDLE hProcess = NULL;
	DWORD dwResult = 0, dwSize = 0;
	UINT_PTR uIndex = 0;
	HMODULE* pModuleData = NULL;
	MODULEINFO stModuleInfo = { 0 };

	__try
	{
		if (!uProcessID || !pModuleName || !uNameSize || !uDestAddress)		__leave;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, uProcessID);
		if (!hProcess)		__leave;
		if (!dwSize)
			dwSize = 1;
		RtlZeroMemory(pModuleName, uNameSize);

		do
		{
			pModuleData = new HMODULE[dwSize];
			RtlZeroMemory(pModuleData, sizeof(HMODULE)*dwSize);
			EnumProcessModulesEx(hProcess, (HMODULE*)pModuleData, dwSize*sizeof(HMODULE), &dwResult, LIST_MODULES_ALL);
			if (dwSize < (dwResult / sizeof(HMODULE)))		//大小不足，重新申请内存
			{
				delete[] pModuleData;
				dwSize = dwResult / sizeof(HMODULE);
			}
			else
				break;
		} while (TRUE);

		for (uIndex = 0; uIndex < dwSize; uIndex++)
		{
			//循环遍历所有模块句柄
			//获取模块基址、大小与入口点
			if (GetModuleInformation(hProcess, pModuleData[uIndex], &stModuleInfo, sizeof(stModuleInfo)))
			{
				if (uDestAddress>(UINT_PTR)stModuleInfo.lpBaseOfDll && uDestAddress < ((UINT_PTR)stModuleInfo.lpBaseOfDll + stModuleInfo.SizeOfImage))
				{
					GetModuleBaseName(hProcess, pModuleData[uIndex], pModuleName, (DWORD)uNameSize);
					break;
				}
			}
		}

		bResult = TRUE;
	}
	__finally
	{
		delete[] pModuleData;
		CloseHandle(hProcess);
	}

	return bResult;
}
