#pragma once

#include "stdafx.h"
#include "Common.h"
#include "ProcessInfo.h"

typedef struct _LDR_MODULE
{
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID BaseAddress;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	LIST_ENTRY HashTableEntry;
	ULONG TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE, *PLML;

typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY
{
	ULONG Unknow1;
	ULONG Unknow2;
#ifdef _WIN64
	ULONG Unknow3;
	ULONG Unknow4;
#endif
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT Index;
	USHORT NameLength;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	CHAR ImageName[256];
}SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG Count;//内核中以加载的模块的个数
	SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
}SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

