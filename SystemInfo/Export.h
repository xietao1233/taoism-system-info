#pragma once

#include "stdafx.h"

//获取进程信息
extern BOOL GetAllProcessByQuerySystemInformation(__out LPBYTE pOutBuff, __inout SIZE_T &uBuffSize);		//获得系统里的全部进程信息

//获取进程路径
extern BOOL GetProcessPathNameByQueryInformationProcess(UINT_PTR uProcessID, __out LPWSTR pOutBuff, SIZE_T uBuffSize);		//获得指定进程ID的完整路径
extern BOOL GetProcessPathByGetModuleFileNameEx(UINT_PTR uProcessID, __out LPWSTR pOutBuff, SIZE_T uBuffSize);		//获得指定进程ID的完整路径
extern BOOL GetProcessPathNameByGetProcessImageFileName(UINT_PTR uProcessID, __out LPWSTR pOutBuff, SIZE_T uBuffSize);
extern BOOL IsWindows64Process(UINT_PTR uProcessID, BOOL& bProcess64);
extern BOOL GetFileCompanyName(LPWSTR pFileName, __out LPWSTR pCompanyName, SIZE_T uNameSize);
extern BOOL GetProcessUsername(UINT uProcessId, __out LPWSTR pUserName, SIZE_T uNameLen);			//获得进程用户名

//模块信息
BOOL GetImageSizeByHeader(HMODULE hMod, __out SIZE_T &uModSize);	//仅支持32位
BOOL GetImageSizeByLDR(HMODULE hMod, __out SIZE_T &uModSize);		//支持64位与32位
BOOL GetModuleNameByAddress32(UINT uProcessID, UINT_PTR uDestAddress, __out LPWSTR pModuleName, SIZE_T uNameSize);		//根据传入的地址和进程ID判断在该进程中属于哪个模块，并输出该模块的名字
BOOL GetModuleNameByAddress64(UINT uProcessID, UINT_PTR uDestAddress, __out LPWSTR pModuleName, SIZE_T uNameSize);		//根据传入的地址和进程ID判断在该进程中属于哪个模块，并输出该模块的名字

#ifdef _W64
#define GetModuleNameByAddress GetModuleNameByAddress64
#else 
#define GetModuleNameByAddress GetModuleNameByAddress32
#endif // _W64

//系统信息
extern BOOL IsWIndows64OS();				//判断当前系统是64位或是32位

//线程信息
extern FARPROC GetThreadStartAddress(UINT_PTR uThreadID);
extern UINT_PTR GetThreadTEB(UINT_PTR uThreadID);		//获得指定线程的TEB基址
extern HICON GetProcessIconSmall(UINT uProcessId);					//	获得进程(小)图标