#pragma once

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////
//	预定义
//

//TAB CONTRL
#define TAB_INDEX_PROCESS 0
#define TAB_INDEX_MODUOLE 1
#define TAB_INDEX_THREAD 2
#define TAB_INDEX_WINDOW 3

//Process Column Header
#define LIST_PROCESS_NAME 0
#define LIST_PROCESS_PID 1
#define LIST_PROCESS_PARENTID 2
#define LIST_PROCESS_PATH 3
#define LIST_PROCESS_USERNAME 4
#define LIST_PROCESS_STATE 5
#define LIST_PROCESS_CO 6
#define LIST_PROCESS_DETAIL_TITLE 7
#define LIST_PROCESS_DETAIL_CONTENT 8

//Module Column Header
#define LIST_MODULE_NAME 0
#define LIST_MODULE_PATH 1
#define LIST_MODULE_BASEADDR 2
#define LIST_MODULE_SIZE 3
#define LIST_MODULE_OEP 4
#define LIST_MODULE_CO 5

//Thread Column Header
#define LIST_THREAD_ID 0
#define LIST_THREAD_PID 1
#define LIST_THREAD_PRIORITY 2
#define LIST_THREAD_TEB 3
#define LIST_THREAD_ADDRESS 4
#define LIST_THREAD_PARAM 5
#define LIST_THREAD_MODULE 6
#define LIST_THREAD_SWITCH 7
#define LIST_THREAD_STATE 8

//Window Column Header
#define LIST_WINDOW_HANDLE 0
#define LIST_WINDOW_TITLE 1
#define LIST_WINDOW_CLASSNAME 2
#define LIST_WINDOW_SHOW 3
#define LIST_WINDOW_PID 4
#define LIST_WINDOW_TID 5
#define LIST_WINDOW_CALLBACKPROC 6

//Timer Column Header
#define LIST_USERTIMER_CALLBACKFUNC 0
#define LIST_USERTIMER_MODULEINFO 1
#define LIST_USERTIMER_TID 2
#define LIST_USERTIMER_PID 3


typedef struct tagTabCtrlInfo
{
	UINT_PTR Index;
	WCHAR LabelText[0x20];
	INT_PTR Width;
}TABCTRLINFO, *LPTABCTRLINFO;

typedef struct tagListColumnInfo
{
	INT Index;
	WCHAR ColumnText[0x1c];
	INT Align;
	INT Width;
}LISTCOLUMNINFO, *LPLISTCOLUMNINFO;



