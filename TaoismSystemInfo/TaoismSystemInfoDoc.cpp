
// TaoismSystemInfoDoc.cpp : implementation of the CTaoismSystemInfoDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TaoismSystemInfo.h"
#endif
#include "..\\SystemInfo\\Export.h"
#include "MainFrm.h"
#include "SystemView.h"
#include "TaoismSystemInfoDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTaoismSystemInfoDoc

typedef BOOL (WINAPI *ENUMPROCESSMODULEEX)(
	_In_  HANDLE  hProcess,
	_Out_ HMODULE *lphModule,
	_In_  DWORD   cb,
	_Out_ LPDWORD lpcbNeeded,
	_In_  DWORD   dwFilterFlag
	);



IMPLEMENT_DYNCREATE(CTaoismSystemInfoDoc, CDocument)

BEGIN_MESSAGE_MAP(CTaoismSystemInfoDoc, CDocument)
END_MESSAGE_MAP()


// CTaoismSystemInfoDoc construction/destruction

CTaoismSystemInfoDoc::CTaoismSystemInfoDoc()
{
	// TODO: add one-time construction code here
	m_pCurrentItemActive = new NMITEMACTIVATE;
	RtlZeroMemory(m_pCurrentItemActive, sizeof(NMITEMACTIVATE));
}

CTaoismSystemInfoDoc::~CTaoismSystemInfoDoc()
{
	delete m_pCurrentItemActive;
}

BOOL CTaoismSystemInfoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if (!m_viewList.IsEmpty())
	{
		reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(NULL);
	}

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTaoismSystemInfoDoc serialization

void CTaoismSystemInfoDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	if (!m_viewList.IsEmpty())
	{
		reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
	}
#ifdef SHARED_HANDLERS

	if (m_viewList.IsEmpty() && ar.IsLoading())
	{
		CFile* pFile = ar.GetFile();
		pFile->Seek(0, FILE_BEGIN);
		ULONGLONG nFileSizeBytes = pFile->GetLength();
		ULONGLONG nFileSizeChars = nFileSizeBytes/sizeof(TCHAR);
		LPTSTR lpszText = (LPTSTR)malloc(((size_t)nFileSizeChars + 1) * sizeof(TCHAR));
		if (lpszText != NULL)
		{
			ar.Read(lpszText, (UINT)nFileSizeBytes);
			lpszText[nFileSizeChars] = '\0';
			m_strThumbnailContent = lpszText;
			m_strSearchContent = lpszText;
		}
	}
#endif
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CTaoismSystemInfoDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(m_strThumbnailContent, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CTaoismSystemInfoDoc::InitializeSearchContent()
{
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// Use the entire text file content as the search content.
	SetSearchContent(m_strSearchContent);
}

void CTaoismSystemInfoDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CTaoismSystemInfoDoc diagnostics

#ifdef _DEBUG
void CTaoismSystemInfoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTaoismSystemInfoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTaoismSystemInfoDoc commands

BOOL CTaoismSystemInfoDoc::UpdateActiveViewInfo(INT iCurSelIndex)
{
	BOOL bResult = FALSE;
	CMainFrame* pMain = reinterpret_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd);
	ASSERT(pMain->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CSystemView* pView = reinterpret_cast<CSystemView*>(pMain->GetActiveView());
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CSystemView)));

	switch (iCurSelIndex)
	{
	case TAB_INDEX_PROCESS:
		bResult = UpdateProcessInfo(pView);
		break;
	case TAB_INDEX_MODUOLE:
		bResult = UpdateModuleInfo(pView);
		break;
	case TAB_INDEX_THREAD:
		bResult = UpdateThreadInfo(pView);
		break;
	case TAB_INDEX_WINDOW:
		bResult = UpdateWindowInfo(pView);
		break;
	}
	return bResult;
}


BOOL CTaoismSystemInfoDoc::UpdateProcessInfo(CSystemView* pProcessView)
{
	BOOL bResult = FALSE;
	CWinThread* pThread = NULL;

	pThread = AfxBeginThread((AFX_THREADPROC)CTaoismSystemInfoDoc::UpdateProcessContent, pProcessView);
	if (pThread->m_hThread)
	{
		bResult = TRUE;
	}

	return bResult;
}


BOOL CTaoismSystemInfoDoc::UpdateModuleInfo(CSystemView* pModuleView)
{
	BOOL bResult = FALSE;
	CWinThread* pThread = NULL;

	pThread = AfxBeginThread((AFX_THREADPROC)CTaoismSystemInfoDoc::UpdateModuleContent, pModuleView);
	if (pThread->m_hThread)
	{
		bResult = TRUE;
	}

	return bResult;
}


BOOL CTaoismSystemInfoDoc::UpdateThreadInfo(CSystemView* pThreadView)
{
	BOOL bResult = FALSE;
	CWinThread* pThread = NULL;

	pThread = AfxBeginThread((AFX_THREADPROC)CTaoismSystemInfoDoc::UpdateThreadContent, pThreadView);
	if (pThread->m_hThread)
	{
		bResult = TRUE;
	}

	return bResult;
}


BOOL CTaoismSystemInfoDoc::UpdateWindowInfo(CSystemView* pWindowView)
{
	BOOL bResult = FALSE;
	CListCtrl& list = pWindowView->GetListCtrl();
	ASSERT_VALID(&list);

	list.DeleteAllItems();

	return bResult;
}


INT_PTR CTaoismSystemInfoDoc::UpdateProcessContent(CSystemView* pProcessView)
{
	CListCtrl& listProcess = pProcessView->GetListCtrl();
	PSYSTEM_PROCESS_INFORMATION pSRI = NULL;
	LPBYTE pBuff = NULL;
	BOOL bProcess64 = FALSE;
	ULONG uResult = 0;
	LVITEMW stItem = { 0 };
	CString str, strPath;
	CImageList* pImageList = listProcess.GetImageList(LVSIL_SMALL);
	HICON hSmall = NULL;
	INT iImageIndex = 0;

	if (!listProcess || !::IsWindow(listProcess.GetSafeHwnd()))
		return 0;

	listProcess.DeleteAllItems();
	if (pImageList)
	{
		for (INT u = pImageList->GetImageCount() - 1; u >= 0; u--)
			pImageList->Remove(u);
	}
	else
	{
		pImageList = new CImageList;
		pImageList->Create(16, 16, ILC_COLOR32, 100, 10);
		listProcess.SetImageList(pImageList, LVSIL_SMALL);
	}

	stItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;

	::EnterCriticalSection(&pProcessView->m_csUpdateInfo);

	pProcessView->m_pfnZwQuerySystemInformation(SystemProcessInformation, NULL, NULL, &uResult);
	if (uResult)
	{
		pBuff = new BYTE[uResult];
		pSRI = (PSYSTEM_PROCESS_INFORMATION)pBuff;
		RtlZeroMemory(pSRI, uResult);
		//GetAllProcessByQuerySystemInformation(pBuff,(SIZE_T&)uResult);		//���Ժ���

		if (pProcessView->m_pfnZwQuerySystemInformation(SystemProcessInformation, pSRI, uResult, &uResult) == STATUS_SUCCESS)
		{
			do
			{
				if (pSRI->ProcessId)
				{
					//��ȡ�ý��̵�ICONͼ��
					hSmall = GetProcessIconSmall((UINT)pSRI->ProcessId);
					if (!hSmall)
						hSmall = LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDI_ICON_DEF));
					iImageIndex = pImageList->Add(hSmall);

					//������
					stItem.iItem = listProcess.GetItemCount();
					stItem.iSubItem = LIST_PROCESS_NAME;
					stItem.lParam = (LPARAM)pSRI->ProcessId;
					stItem.iImage = iImageIndex;
					stItem.pszText = pSRI->ImageName.Buffer;
					listProcess.InsertItem(&stItem);

					//����ID
					stItem.iSubItem = LIST_PROCESS_PID;
					str.Format(L"%X", pSRI->ProcessId);
					listProcess.SetItemText(stItem.iItem, LIST_PROCESS_PID, str.GetBuffer());
					str.ReleaseBuffer();

					//������id
					str.Format(L"%X", pSRI->InheritedFromProcessId);
					listProcess.SetItemText(stItem.iItem, LIST_PROCESS_PARENTID, str.GetBuffer());
					str.ReleaseBuffer();

					//����·��
					GetProcessPathByGetModuleFileNameEx(pSRI->ProcessId, str.GetBuffer(MAX_PATH), MAX_PATH);
					str.ReleaseBuffer();
					listProcess.SetItemText(stItem.iItem, LIST_PROCESS_PATH, str.GetBuffer());
					str.ReleaseBuffer();
					strPath = str;

					//��ý����������û���
					str.Empty();
					GetProcessUsername((INT)pSRI->ProcessId, str.GetBuffer(0x200), 0x200);
					listProcess.SetItemText(stItem.iItem, LIST_PROCESS_USERNAME, str.GetBuffer());
					str.ReleaseBuffer();

					//״̬
					if (strPath.IsEmpty())
						listProcess.SetItemText(stItem.iItem, LIST_PROCESS_STATE, L"�޷�����");
					else if (IsWindows64Process(pSRI->ProcessId, bProcess64))
					{
						if (bProcess64)
							listProcess.SetItemText(stItem.iItem, LIST_PROCESS_STATE, L"����");
						else
							listProcess.SetItemText(stItem.iItem, LIST_PROCESS_STATE, L"32λ");
					}

					//��˾��Ϣ
					if (!strPath.IsEmpty())
					{
						GetFileCompanyName(strPath.GetBuffer(), str.GetBuffer(MAX_PATH), MAX_PATH);
						strPath.ReleaseBuffer();
						listProcess.SetItemText(stItem.iItem, LIST_PROCESS_CO, str.GetBuffer());
						str.ReleaseBuffer();
					}
				}

				if (!pSRI->NextEntryOffset)
					break;
				pSRI = (PSYSTEM_PROCESS_INFORMATION)((LPBYTE)pSRI + pSRI->NextEntryOffset);
			} while (TRUE);
		}
		delete pBuff;
	}

	::LeaveCriticalSection(&pProcessView->m_csUpdateInfo);

	return 1;
}


INT_PTR CTaoismSystemInfoDoc::UpdateModuleContent(CSystemView* pModuleView)
{
	CListCtrl& listModule = pModuleView->GetListCtrl();
	CTaoismSystemInfoDoc* pDoc = reinterpret_cast<CTaoismSystemInfoDoc*>(pModuleView->GetDocument());
	ASSERT(pDoc);
	CString str, strPath;
	HANDLE hProcess = NULL;
	DWORD dwResult = 0, dwSize = 0;
	UINT_PTR uIndex = 0;
	HMODULE* pModuleData = NULL;
	ENUMPROCESSMODULEEX myEnumProcessModules = NULL;
	MODULEINFO stModuleInfo = { 0 };
	LVITEMW lv = { 0 };
	UINT_PTR uProcessID = pDoc->GetCurSelProcessID();
	
	if (!uProcessID)
		return FALSE;

	::EnterCriticalSection(&pModuleView->m_csUpdateInfo);
	listModule.DeleteAllItems();
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)uProcessID);
	if (hProcess)
	{
		if (!dwSize)
			dwSize = 1;
		do
		{
			pModuleData = new HMODULE[dwSize];
			RtlZeroMemory(pModuleData, sizeof(HMODULE)*dwSize);
			if (IsWIndows64OS())
			{
				//64λϵͳ ��̬��ȡ EnumProcessModulesEx ������ַ, ����32λϵͳxpû�д˺���
				myEnumProcessModules = (ENUMPROCESSMODULEEX)GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "EnumProcessModulesEx");
				if (!myEnumProcessModules)
					myEnumProcessModules = (ENUMPROCESSMODULEEX)GetProcAddress(GetModuleHandleW(L"Psapi.dll"), "EnumProcessModulesEx");
				if (myEnumProcessModules)
					myEnumProcessModules(hProcess, (HMODULE*)pModuleData, dwSize*sizeof(HMODULE), &dwResult, LIST_MODULES_ALL);
				else
				{
					dwSize = 0;
					break;
				}
			}
			else
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
			lv.mask = LVIF_TEXT | LVIF_PARAM;

			//1. ���ģ����
			GetModuleBaseName(hProcess, pModuleData[uIndex], str.GetBuffer(MAX_PATH), MAX_PATH);
			str.ReleaseBuffer();
			GetModuleInformation(hProcess, pModuleData[uIndex], &stModuleInfo, sizeof(stModuleInfo));
			lv.iItem = (INT)uIndex;
			lv.iSubItem = LIST_MODULE_NAME;
			lv.lParam = (LPARAM)stModuleInfo.lpBaseOfDll;
			lv.pszText = str.GetBuffer();
			str.ReleaseBuffer();
			listModule.InsertItem(&lv);

			lv.mask = LVIF_TEXT;
			//2.���ģ��·��
			GetModuleFileNameEx(hProcess, pModuleData[uIndex], str.GetBuffer(MAX_PATH), MAX_PATH);
			str.ReleaseBuffer();
			strPath = str;
			lv.iSubItem = LIST_MODULE_PATH;
			lv.pszText = str.GetBuffer();
			str.ReleaseBuffer();
			listModule.SetItem(&lv);

			//3. ģ���ַ
			str.Format(L"0x%p", stModuleInfo.lpBaseOfDll);
			lv.iSubItem = LIST_MODULE_BASEADDR;
			lv.pszText = str.GetBuffer();
			str.ReleaseBuffer();
			listModule.SetItem(&lv);

			//4. ģ���С
			str.Format(L"0x%p", stModuleInfo.SizeOfImage);
			lv.iSubItem = LIST_MODULE_SIZE;
			lv.pszText = str.GetBuffer();
			str.ReleaseBuffer();
			listModule.SetItem(&lv);

			//5. ģ����ڵ�ַ
			str.Format(L"0x%p", stModuleInfo.EntryPoint);
			lv.iSubItem = LIST_MODULE_OEP;
			lv.pszText = str.GetBuffer();
			str.ReleaseBuffer();
			listModule.SetItem(&lv);

			//6. ������˾
			if (GetFileCompanyName(strPath.GetBuffer(), str.GetBuffer(MAX_PATH), MAX_PATH))
			{
				strPath.ReleaseBuffer();
				lv.iSubItem = LIST_MODULE_CO;
				lv.pszText = str.GetBuffer();
				listModule.SetItem(&lv);
				str.ReleaseBuffer();
			}
		}

		delete[] pModuleData;
		CloseHandle(hProcess);
	}
	::LeaveCriticalSection(&pModuleView->m_csUpdateInfo);

	return INT_PTR(TRUE);
}


INT_PTR CTaoismSystemInfoDoc::UpdateThreadContent(CSystemView* pThreadView)
{
	CListCtrl& listThread = pThreadView->GetListCtrl();
	CTaoismSystemInfoDoc* pDoc = reinterpret_cast<CTaoismSystemInfoDoc*>(pThreadView->GetDocument());
	ASSERT(pDoc);
	ULONG uResult = 0;
	FARPROC pfnFuncAddr = NULL;
	PSYSTEM_PROCESS_INFORMATION pSRI = NULL;
	LPBYTE pBuff = NULL;
	CString str;
	LVITEMW lv = { 0 };
	UINT uProcessID = pDoc->GetCurSelProcessID();

	if (!uProcessID)
		return FALSE;

	::EnterCriticalSection(&pThreadView->m_csUpdateInfo);
	listThread.DeleteAllItems();
	pThreadView->m_pfnZwQuerySystemInformation(SystemProcessInformation, NULL, NULL, &uResult);
	if (uResult)
	{
		pBuff = new BYTE[uResult];
		RtlZeroMemory(pBuff, uResult);
		pSRI = (PSYSTEM_PROCESS_INFORMATION)pBuff;
		if (pThreadView->m_pfnZwQuerySystemInformation(SystemProcessInformation, pSRI, uResult, &uResult) == STATUS_SUCCESS)
		{
			//1. �ҵ�Ŀ�����ID
			while (TRUE)
			{
				if (pSRI->ProcessId == uProcessID)
				{
					for (UINT_PTR u = 0; u < pSRI->NumberOfThreads; u++)
					{
						lv.mask = LVIF_TEXT | LVIF_PARAM;

						//1. �߳�ID
						str.Format(L"%X", pSRI->Threads[u].ClientId.UniqueThread);
						lv.iItem = (INT)u;
						lv.iSubItem = LIST_THREAD_ID;
						lv.lParam = (LPARAM)pSRI->Threads[u].ClientId.UniqueThread;
						lv.pszText = str.GetBuffer();
						str.ReleaseBuffer();
						listThread.InsertItem(&lv);

						lv.mask = LVIF_TEXT;
						//2. ��������ID
						str.Format(L"%X", uProcessID);
						lv.iSubItem = LIST_THREAD_PID;
						lv.pszText = str.GetBuffer();
						str.ReleaseBuffer();
						listThread.SetItem(&lv);

						//3. �߳����ȼ�
						str.Format(L"%d", pSRI->Threads[u].Priority);
						lv.iSubItem = LIST_THREAD_PRIORITY;
						lv.pszText = str.GetBuffer();
						str.ReleaseBuffer();
						listThread.SetItem(&lv);

						//5. ��ڵ�ַ
						pfnFuncAddr = (FARPROC)GetThreadStartAddress((UINT_PTR)pSRI->Threads[u].ClientId.UniqueThread);
						if (pfnFuncAddr)
						{
							str.Format(L"0x%p", pfnFuncAddr);
							lv.iSubItem = LIST_THREAD_ADDRESS;
							lv.pszText = str.GetBuffer();
							str.ReleaseBuffer();
							listThread.SetItem(&lv);

							//6. ����ģ��
							if (GetModuleNameByAddress(uProcessID, (UINT_PTR)pfnFuncAddr, str.GetBuffer(MAX_PATH), MAX_PATH))
							{
								lv.iSubItem = LIST_THREAD_MODULE;
								lv.pszText = str.GetBuffer();
								listThread.SetItem(&lv);
							}
							str.ReleaseBuffer();
						}

						//7. ���TEB��ַ
						pfnFuncAddr = (FARPROC)GetThreadTEB((UINT_PTR)pSRI->Threads[u].ClientId.UniqueThread);
						if (pfnFuncAddr)
						{
							str.Format(L"0x%p", pfnFuncAddr);
							lv.iSubItem = LIST_THREAD_TEB;
							lv.pszText = str.GetBuffer();
							str.ReleaseBuffer();
							listThread.SetItem(&lv);
						}

						//7. �л�����
						str.Format(L"%d", pSRI->Threads[u].ContextSwitchCount);
						lv.iSubItem = LIST_THREAD_SWITCH;
						lv.pszText = str.GetBuffer();
						str.ReleaseBuffer();
						listThread.SetItem(&lv);

						//8. �߳�״̬
						lv.iSubItem = LIST_THREAD_STATE;
						switch (pSRI->Threads[u].ThreadState)
						{
						case StateInitialized:
							lv.pszText = L"��ʼ��";
							break;
						case StateReady:
							lv.pszText = L"�ȴ�����";
							break;
						case StateRunning:
							lv.pszText = L"��������";
							break;
						case StateStandby:
							lv.pszText = L"׼��";
							break;
						case StateTerminated:
							lv.pszText = L"����ֹ";
							break;
						case StateWait:
							lv.pszText = L"�ȴ��л�";
							break;
						case StateTransition:
							lv.pszText = L"�л�";
							break;
						default:
							lv.pszText = L"δ֪";
							break;
						}
						listThread.SetItem(&lv);
					}
					break;
				}
				else
				{
					if (!pSRI->NextEntryOffset)
						break;
					pSRI = (PSYSTEM_PROCESS_INFORMATION)((LPBYTE)pSRI + pSRI->NextEntryOffset);
				}
			}
		}
		delete[] pBuff;
	}

	::LeaveCriticalSection(&pThreadView->m_csUpdateInfo);

	return INT_PTR(TRUE);
}


BOOL CTaoismSystemInfoDoc::SetCurSelProcessItem(LPNMITEMACTIVATE pProcessItem)
{
	ASSERT(m_pCurrentItemActive);

	RtlCopyMemory(m_pCurrentItemActive, pProcessItem, sizeof(NMITEMACTIVATE));

	return TRUE;
}


UINT CTaoismSystemInfoDoc::GetCurSelProcessID()
{
	UINT uResult = 0;
	LVITEMW lv = { 0 };

	if (!m_pCurrentItemActive || m_pCurrentItemActive->iItem != -1)
	{
		lv.mask = LVIF_PARAM;
		lv.iItem = m_pCurrentItemActive->iItem;
		lv.iSubItem = LIST_PROCESS_PID;
		if (ListView_GetItem(m_pCurrentItemActive->hdr.hwndFrom, &lv))
			uResult = (UINT)lv.lParam;
	}

	return UINT(uResult);
}
