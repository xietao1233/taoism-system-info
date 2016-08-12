#pragma once

#include "stdafx.h"

#define STATUS_SUCCESS 0

typedef LONG NTSTATUS;
typedef LONG KPRIORITY;

typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;
