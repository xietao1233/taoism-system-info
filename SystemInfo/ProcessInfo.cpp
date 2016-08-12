#include "stdafx.h"
#include "Export.h"
#include "ProcessInfo.h"
#include <Psapi.h>
#include <Shellapi.h>

//综述： GetProcessImageFileName 函数 封装了 ZwQueryInformationProcess 函数的ProcessImageFileName参数


struct LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
} *lpTranslate;

//////////////////////////////////////////////////////////////////////////
//	由ZwQuerySystemInformation函数获得系统中所有的进程信息
//
BOOL GetAllProcessByQuerySystemInformation(__out LPBYTE pOutBuff, __inout SIZE_T &uBuffSize)
{
	BOOL bResult = FALSE;
	ZwQuerySystemInformation pfnZwQuerySystemInformation = NULL;
	PSYSTEM_PROCESS_INFORMATION pSRI = NULL;
	LPBYTE pBuff = NULL;
	ULONG uResult = 0;
	SIZE_T uSize = 0;

	__try
	{
		uSize = uBuffSize;
		uBuffSize = 0;
		pfnZwQuerySystemInformation = (ZwQuerySystemInformation)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwQuerySystemInformation");
		if (!pfnZwQuerySystemInformation)		__leave;

		//1. 获得请求内存的大小
		pfnZwQuerySystemInformation(SystemProcessInformation, NULL, NULL, &uResult);
		if (!uResult)			__leave;
		if (uSize < uResult || !pOutBuff)
		{
			uBuffSize = uResult;		//传入的大小不足或第一个参数为NULL，则返回所需的大小
			__leave;
		}
		pBuff = new BYTE[uResult];
		RtlZeroMemory(pBuff, uResult);
		pSRI = (PSYSTEM_PROCESS_INFORMATION)pBuff;

		//2. 获得所有的进程信息
		if (pfnZwQuerySystemInformation(SystemProcessInformation, pSRI, uResult, &uResult) != STATUS_SUCCESS)			__leave;

		//3. 循环遍历所有进程
		do 
		{
			//MessageBoxW(NULL, pSRI->ImageName.Buffer, L"Title", MB_OK);
			if (!pSRI->NextEntryOffset)
				break;
			pSRI = (PSYSTEM_PROCESS_INFORMATION)((LPBYTE)pSRI + pSRI->NextEntryOffset);
		} while (TRUE);

		bResult = TRUE;
	}
	__finally
	{
		delete[] pBuff;
	}

	return bResult;
}


//////////////////////////////////////////////////////////////////////////
//	通过GetModuleFileNameEx 函数获取进程路径
//
BOOL GetProcessPathByGetModuleFileNameEx(UINT_PTR uProcessID, __out LPWSTR pOutBuff, SIZE_T uBuffSize)
{
	BOOL bResult = FALSE;
	HANDLE hProcess = NULL;

	__try
	{
		if (!uProcessID || !pOutBuff || uBuffSize<MAX_PATH)			__leave;

		*pOutBuff = 0;
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)uProcessID);
		if (!hProcess)			__leave;
		if (!GetModuleFileNameExW(hProcess, NULL, pOutBuff, (DWORD)uBuffSize))			__leave;

		bResult = TRUE;
	}
	__finally
	{
		CloseHandle(hProcess);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//	通过ZwQueryInformationProcess 函数获取进程路径
//
BOOL GetProcessPathNameByQueryInformationProcess(UINT_PTR uProcessID, __out LPWSTR pOutBuff, SIZE_T uBuffSize)
{
	BOOL bResult = FALSE;
	HANDLE hProcess = NULL;
	UNICODE_STRING* usName = NULL;
	ULONG uResult = 0;
	ZwQueryInformationProcess pfnZwQueryInformationProcess = NULL;

	__try
	{
		if (!pOutBuff || !uProcessID)			__leave;
		*pOutBuff = 0;
		pfnZwQueryInformationProcess = (ZwQueryInformationProcess)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwQueryInformationProcess");
		if (!pfnZwQueryInformationProcess)			__leave;
		hProcess = OpenProcess(READ_CONTROL | PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, (DWORD)uProcessID);
		if (!hProcess)		__leave;
		pfnZwQueryInformationProcess(hProcess, ProcessImageFileName, NULL, 0, &uResult);			//获得所需的大小
		if (!uResult)			__leave;
		usName = (UNICODE_STRING*)new BYTE[uResult];
		if (pfnZwQueryInformationProcess(hProcess, ProcessImageFileName, usName, uResult, NULL) != STATUS_SUCCESS)			__leave;
		if (uBuffSize < usName->Length)		__leave;
		wcscpy_s(pOutBuff, usName->Length, usName->Buffer);

		bResult = TRUE;
	}
	__finally
	{
		delete[] (LPBYTE)usName;
		CloseHandle(hProcess);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//	由 GetProcessImageFileName 获得进程路径
//
BOOL GetProcessPathNameByGetProcessImageFileName(UINT_PTR uProcessID, __out LPWSTR pOutBuff, SIZE_T uBuffSize)
{
	BOOL bResult = FALSE;
	HANDLE hProcess = NULL;

	__try
	{
		if (!pOutBuff || !uProcessID || uBuffSize<MAX_PATH)			__leave;
		*pOutBuff = 0;

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)uProcessID);
		if (!hProcess)			__leave;
		if (!GetProcessImageFileNameW(hProcess, pOutBuff, (DWORD)uBuffSize))			__leave;

		bResult = TRUE;
	}
	__finally
	{
		CloseHandle(hProcess);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//		判断是否是一个运行在64位系统上的64位程序
//
BOOL IsWindows64Process(UINT_PTR uProcessID, BOOL& bProcess64)
{
	BOOL bResult = FALSE;
	HANDLE hProcess = NULL;
	BOOL bWow64 = FALSE;
	LPWSTR pDllPath = NULL;

	__try
	{
		bProcess64 = FALSE;
		if (!uProcessID)		__leave;
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, (DWORD)uProcessID);
		if (!hProcess)		__leave;
		pDllPath = new WCHAR[MAX_PATH];
		if (!IsWow64Process(hProcess, &bWow64))			__leave;
		if (!bWow64 && GetSystemWow64Directory(pDllPath, MAX_PATH))				//该程序没有运行在Wow64子系统上并且系统存在Wow64目录，表示该程序为运行在64系统上的64位程序
			bProcess64 = TRUE;
		else
			bProcess64 = FALSE;
		bResult = TRUE;
	}
	__finally
	{
		delete[] pDllPath;
		CloseHandle(hProcess);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//	获得指定文件的公司名称
//
BOOL GetFileCompanyName(LPWSTR pFileName, __out LPWSTR pCompanyName, SIZE_T uNameSize)
{
	BOOL bResult = FALSE;
	LPWSTR pResult = NULL;
	LPBYTE pFileData = NULL;
	UINT uSize = NULL;
	WCHAR szTemp[MAX_PATH] = { 0 };

	__try
	{
		if (!pFileName || !pCompanyName || !uNameSize)			__leave;
		*pCompanyName = 0;
		uSize = GetFileVersionInfoSizeW(pFileName, NULL);			//请求获得数据的大小
		if (!uSize)			__leave;
		pFileData = new BYTE[uSize];
		RtlZeroMemory(pFileData, uSize);
		if (!GetFileVersionInfoW(pFileName, NULL, uSize, pFileData))			__leave;		//申请内存，并获得数据
		if (!VerQueryValueW(pFileData, L"\\VarFileInfo\\Translation", (LPVOID*)&lpTranslate, &uSize))			__leave;		//请求语言代码页
		wsprintfW(szTemp, L"\\StringFileInfo\\%04x%04x\\CompanyName", lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);		//构造请求字符串
		if (!VerQueryValueW(pFileData, szTemp, (LPVOID*)&pResult, (PUINT)&uSize))			__leave;			//获得公司信息
		if (uNameSize < wcslen(pResult))			__leave;
		wcscpy_s(pCompanyName, uNameSize, pResult);
				
		bResult = TRUE;
	}
	__finally
	{
		delete[] pFileData;
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//	获得进程(小)图标
//
HICON GetProcessIconSmall(UINT uProcessId)
{
	HICON hResult = NULL;
	LPWSTR pBuff = NULL;
	UINT uNum = 0;
	//ICONINFO stII = { 0 };

	__try
	{
		pBuff = new WCHAR[MAX_PATH];
		RtlZeroMemory(pBuff, MAX_PATH * sizeof(WCHAR));
		GetProcessPathByGetModuleFileNameEx(uProcessId, pBuff, MAX_PATH);
		hResult = ExtractIconW(GetModuleHandleW(NULL), pBuff, 0);
		//GetIconInfo(hResult, &stII);
	}
	__finally
	{
		delete[] pBuff;
	}

	return hResult;
}

//////////////////////////////////////////////////////////////////////////
//	根据传入的进程ID, 获得该进程的用户名
//
BOOL GetProcessUsername(UINT uProcessId, __out LPWSTR pUserName, SIZE_T uNameLen)
{
	BOOL bResult = FALSE;
	HANDLE hProcess = 0, hToken = 0;
	PTOKEN_USER pTokenUser = NULL;
	DWORD dwSize = 0;
	LPWSTR pDomainName = NULL;
	DWORD dwDomainNameLen = 0x200;
	UINT_PTR SNU = 0;

	__try
	{
		if (!pUserName || !uNameLen)			__leave;
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, uProcessId);
		if (!hProcess)					__leave;
		if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))				__leave;
		GetTokenInformation(hToken, TokenUser, pTokenUser, NULL, &dwSize);
		if (!dwSize)						__leave;
		pTokenUser = (PTOKEN_USER)new BYTE[dwSize];
		if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize))			__leave;
		dwSize = (UINT)uNameLen;
		pDomainName = new WCHAR[dwDomainNameLen];
		if (!LookupAccountSid(NULL, pTokenUser->User.Sid, pUserName, &dwSize, pDomainName, &dwDomainNameLen, (PSID_NAME_USE)&SNU))			__leave;

		bResult = TRUE;
	}
	__finally
	{
		delete[] pDomainName;
		delete[] (LPBYTE)pTokenUser;
		CloseHandle(hToken);
		CloseHandle(hProcess);
	}

	return bResult;
}
