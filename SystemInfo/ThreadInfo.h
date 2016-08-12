#pragma once

#include "stdafx.h"
#include "Common.h"

typedef   enum   _THREADINFOCLASS   {
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair_Reusable,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadIsIoPending,
	ThreadHideFromDebugger,
	ThreadBreakOnTermination,
	MaxThreadInfoClass
}   THREADINFOCLASS;


typedef   struct   _THREAD_BASIC_INFORMATION   {
	LONG        ExitStatus;
	PVOID       TebBaseAddress;
	CLIENT_ID   ClientId;
	LONG        AffinityMask;
	LONG        Priority;
	LONG        BasePriority;
}   THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

typedef NTSTATUS(WINAPI *ZwQueryInformationThread)   (
	IN   HANDLE   ThreadHandle,
	IN   THREADINFOCLASS   ThreadInformationClass,
	OUT   PVOID   ThreadInformation,
	IN   ULONG   ThreadInformationLength,
	OUT   PULONG   ReturnLength   OPTIONAL
	);
