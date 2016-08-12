#pragma once

#include "stdafx.h"

//��ȡ������Ϣ
extern BOOL GetAllProcessByQuerySystemInformation(__out LPBYTE pOutBuff, __inout SIZE_T &uBuffSize);		//���ϵͳ���ȫ��������Ϣ

//��ȡ����·��
extern BOOL GetProcessPathNameByQueryInformationProcess(UINT_PTR uProcessID, __out LPWSTR pOutBuff, SIZE_T uBuffSize);		//���ָ������ID������·��
extern BOOL GetProcessPathByGetModuleFileNameEx(UINT_PTR uProcessID, __out LPWSTR pOutBuff, SIZE_T uBuffSize);		//���ָ������ID������·��
extern BOOL GetProcessPathNameByGetProcessImageFileName(UINT_PTR uProcessID, __out LPWSTR pOutBuff, SIZE_T uBuffSize);
extern BOOL IsWindows64Process(UINT_PTR uProcessID, BOOL& bProcess64);
extern BOOL GetFileCompanyName(LPWSTR pFileName, __out LPWSTR pCompanyName, SIZE_T uNameSize);
extern BOOL GetProcessUsername(UINT uProcessId, __out LPWSTR pUserName, SIZE_T uNameLen);			//��ý����û���

//ģ����Ϣ
BOOL GetImageSizeByHeader(HMODULE hMod, __out SIZE_T &uModSize);	//��֧��32λ
BOOL GetImageSizeByLDR(HMODULE hMod, __out SIZE_T &uModSize);		//֧��64λ��32λ
BOOL GetModuleNameByAddress32(UINT uProcessID, UINT_PTR uDestAddress, __out LPWSTR pModuleName, SIZE_T uNameSize);		//���ݴ���ĵ�ַ�ͽ���ID�ж��ڸý����������ĸ�ģ�飬�������ģ�������
BOOL GetModuleNameByAddress64(UINT uProcessID, UINT_PTR uDestAddress, __out LPWSTR pModuleName, SIZE_T uNameSize);		//���ݴ���ĵ�ַ�ͽ���ID�ж��ڸý����������ĸ�ģ�飬�������ģ�������

#ifdef _W64
#define GetModuleNameByAddress GetModuleNameByAddress64
#else 
#define GetModuleNameByAddress GetModuleNameByAddress32
#endif // _W64

//ϵͳ��Ϣ
extern BOOL IsWIndows64OS();				//�жϵ�ǰϵͳ��64λ����32λ

//�߳���Ϣ
extern FARPROC GetThreadStartAddress(UINT_PTR uThreadID);
extern UINT_PTR GetThreadTEB(UINT_PTR uThreadID);		//���ָ���̵߳�TEB��ַ
extern HICON GetProcessIconSmall(UINT uProcessId);					//	��ý���(С)ͼ��