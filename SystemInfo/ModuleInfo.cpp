#include "stdafx.h"
#include "Export.h"
#include "ModuleInfo.h"
#include <Psapi.h>


//////////////////////////////////////////////////////////////////////////
//	�� EnumProcessModules ������� ָ�����̵�ģ����Ϣ
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
			if (dwSize < (dwResult / sizeof(HMODULE)))		//��С���㣬���������ڴ�
			{
				delete[] pModuleData;
				dwSize = dwResult / sizeof(HMODULE);
			}
			else
				break;
		} while (TRUE);

		for (u = 0; u < dwSize; u++);
		{
			//ѭ����������ģ����
			//��ȡģ������·��
			GetModuleBaseName(hProcess, pModuleData[u], pOutBuff, (DWORD)uBuffSize);			//�����ģ����
			GetModuleFileNameEx(hProcess, pModuleData[u], pOutBuff, (DWORD)uBuffSize);		//���ģ��ȫ·��
			//��ȡģ���ַ����С����ڵ�
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
//	�� ZwQuerySystemInformation ������ȡϵͳ����ģ����Ϣ
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
			//������������ģ�� �磺\SystemRoot\system32\ntoskrnl.exe
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
//	���ļ�ͷ�л�ñ�����ģ���С(��֧��32λ)
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
//	���LDRָ��(֧��64λ��32λ)
//
PLDR_MODULE GetLdrModulePtr()
{
	PLDR_MODULE pResult = NULL;
	UINT_PTR* pPtr;

	__try
	{
		pPtr = (UINT_PTR*)NtCurrentTeb();
		pPtr += 0xc;		//TEB��12����Ա��ΪPEBָ��
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
//	��LDR�л�ñ�����ģ���С(֧��64λ��32λ)
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
//		���ݴ���ĵ�ַ�ͽ���ID�ж��ڸý����������ĸ�ģ�飬�������ģ�������(32λϵͳʹ��)
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
			if (dwSize < (dwResult / sizeof(HMODULE)))		//��С���㣬���������ڴ�
			{
				delete[] pModuleData;
				dwSize = dwResult / sizeof(HMODULE);
			}
			else
				break;
		} while (TRUE);

		for (uIndex = 0; uIndex < dwSize; uIndex++)
		{
			//ѭ����������ģ����
			//��ȡģ���ַ����С����ڵ�
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
//		���ݴ���ĵ�ַ�ͽ���ID�ж��ڸý����������ĸ�ģ�飬�������ģ�������(64λϵͳʹ��)
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
			if (dwSize < (dwResult / sizeof(HMODULE)))		//��С���㣬���������ڴ�
			{
				delete[] pModuleData;
				dwSize = dwResult / sizeof(HMODULE);
			}
			else
				break;
		} while (TRUE);

		for (uIndex = 0; uIndex < dwSize; uIndex++)
		{
			//ѭ����������ģ����
			//��ȡģ���ַ����С����ڵ�
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
