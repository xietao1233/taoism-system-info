// SystemView.cpp : implementation file
//

#include "stdafx.h"
#include "TaoismSystemInfo.h"
#include "MainFrm.h"
#include "TaoismSystemInfoView.h"
#include "SystemView.h"
#include "DetailDlg.h"
#include "Common.h"


// CSystemView
LISTCOLUMNINFO stProcessListColumnInfo[] = {
	{ LIST_PROCESS_PID, L"����ID", LVCFMT_LEFT, 90 },
	{ LIST_PROCESS_NAME, L"������", LVCFMT_LEFT, 140 },
	{ LIST_PROCESS_PARENTID, L"������ID", LVCFMT_LEFT, 90 },
	{ LIST_PROCESS_PATH, L"����·��", LVCFMT_LEFT, 550 },
	{ LIST_PROCESS_USERNAME, L"�û���", LVCFMT_LEFT, 150 },
	{ LIST_PROCESS_STATE, L"����״̬", LVCFMT_LEFT, 120 },
	{ LIST_PROCESS_CO, L"������˾", LVCFMT_LEFT, 250 },
	{ 0, 0, 0, 0 }
};

LISTCOLUMNINFO stModuleListColumnInfo[] = {
	{ LIST_MODULE_NAME, L"ģ����", LVCFMT_LEFT, 150 },
	{ LIST_MODULE_PATH, L"ģ��·��", LVCFMT_LEFT, 550 },
	{ LIST_MODULE_BASEADDR, L"ģ���ַ", LVCFMT_CENTER, 220 },
	{ LIST_MODULE_SIZE, L"ģ���С", LVCFMT_CENTER, 220 },
	{ LIST_MODULE_OEP, L"��ڵ�ַ", LVCFMT_CENTER, 220 },
	{ LIST_MODULE_CO, L"������˾", LVCFMT_LEFT, 180 },
	{ 0, 0, 0, 0 }
};

LISTCOLUMNINFO stThreadListColumnInfo[] = {
	{ LIST_THREAD_ID, L"�߳�ID",LVCFMT_LEFT,100 },
	{ LIST_THREAD_PID, L"��������ID", LVCFMT_LEFT, 110 },
	{ LIST_THREAD_PRIORITY, L"���ȼ�", LVCFMT_LEFT, 80 },
	{ LIST_THREAD_TEB, L"TEB", LVCFMT_LEFT, 220 },
	{ LIST_THREAD_ADDRESS, L"��ڵ�ַ", LVCFMT_LEFT, 220 },
	{ LIST_THREAD_PARAM, L"�̲߳���", LVCFMT_LEFT, 120 },
	{ LIST_THREAD_MODULE, L"����ģ��", LVCFMT_LEFT, 140 },
	{ LIST_THREAD_SWITCH, L"�л�����", LVCFMT_LEFT, 90 },
	{ LIST_THREAD_STATE, L"�߳�״̬", LVCFMT_LEFT, 90 },
	{ 0, 0, 0, 0 }
};

LISTCOLUMNINFO stWindowListColmunInfo[] = {
	{ LIST_WINDOW_HANDLE, L"���ھ��",LVCFMT_LEFT,100 },
	{ LIST_WINDOW_TITLE, L"���ڱ���", LVCFMT_LEFT, 180 },
	{ LIST_WINDOW_CLASSNAME, L"��������", LVCFMT_LEFT, 180 },
	{ LIST_WINDOW_SHOW, L"���ڿɼ���", LVCFMT_LEFT, 120 },
	{ LIST_WINDOW_PID, L"��������ID", LVCFMT_LEFT, 120 },
	{ LIST_WINDOW_TID, L"�����߳�ID", LVCFMT_LEFT, 120 },
	{ LIST_WINDOW_CALLBACKPROC, L"�ص�������ַ",LVCFMT_LEFT,150 },
	{ 0, 0, 0, 0 }
};

LPLISTCOLUMNINFO CSystemView::m_pListHeadInfo[] = {
	(LPLISTCOLUMNINFO)&stProcessListColumnInfo,
	(LPLISTCOLUMNINFO)&stModuleListColumnInfo,
	(LPLISTCOLUMNINFO)&stThreadListColumnInfo,
	(LPLISTCOLUMNINFO)&stWindowListColmunInfo,
	NULL
};

IMPLEMENT_DYNCREATE(CSystemView, CListView)

CSystemView::CSystemView()
: m_uSortSubIndex(0)
, m_iSortSubOrder(0)
{
	::InitializeCriticalSection(&m_csUpdateInfo);
	m_pfnZwQuerySystemInformation = (ZwQuerySystemInformation)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwQuerySystemInformation");
}

CSystemView::~CSystemView()
{
	::DeleteCriticalSection(&m_csUpdateInfo);
}

BEGIN_MESSAGE_MAP(CSystemView, CListView)
	ON_NOTIFY_REFLECT(NM_CLICK, &CSystemView::OnNMClick)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CSystemView::OnLvnColumnclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CSystemView::OnNMDblclk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSystemView diagnostics

#ifdef _DEBUG
void CSystemView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CSystemView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSystemView message handlers

INT CSystemView::GetCurSelTabIndex()
{
	CMFCTabCtrl* tab = reinterpret_cast<CMFCTabCtrl*>(GetParent());
	ASSERT(tab->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)));
	CTaoismSystemInfoView* pView = reinterpret_cast<CTaoismSystemInfoView*>(tab->GetParent());
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CTaoismSystemInfoView)));

	return INT(pView->FindTab(GetSafeHwnd()));
}


void CSystemView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CMFCTabCtrl* tab = reinterpret_cast<CMFCTabCtrl*>(GetParent());
	ASSERT(tab->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)));
	CString strTitle;
	LISTCOLUMNINFO* pListHeader = NULL;
	LONG_PTR lListStyle = 0;

	INT i = GetCurSelTabIndex();
	switch (i)
	{
	case TAB_INDEX_PROCESS:
		pListHeader = m_pListHeadInfo[TAB_INDEX_PROCESS];
		tab->GetTabLabel(tab->GetActiveTab(), strTitle);
		strTitle.Trim();
		GetDocument()->SetTitle(strTitle);
		break;
	case TAB_INDEX_MODUOLE:
		pListHeader = m_pListHeadInfo[TAB_INDEX_MODUOLE];
		break;
	case TAB_INDEX_THREAD:
		pListHeader = m_pListHeadInfo[TAB_INDEX_THREAD];
		break;
	case TAB_INDEX_WINDOW:
		pListHeader = m_pListHeadInfo[TAB_INDEX_WINDOW];
		break;
	}

	while (wcslen(pListHeader->ColumnText))
	{
		GetListCtrl().InsertColumn(pListHeader->Index, pListHeader->ColumnText, pListHeader->Align, pListHeader->Width);
		pListHeader = reinterpret_cast<LPLISTCOLUMNINFO>((LPBYTE)pListHeader + sizeof(LISTCOLUMNINFO));
	}
	//list��ʽ����
	lListStyle = ::GetWindowLongPtrW(GetListCtrl().GetSafeHwnd(), GWL_STYLE);
	lListStyle &= ~LVS_TYPEMASK;
	lListStyle |= (LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	::SetWindowLongPtrW(GetListCtrl().GetSafeHwnd(), GWL_STYLE, lListStyle);
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SINGLEROW | LVS_EX_GRIDLINES);
}


void CSystemView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Add your specialized code here and/or call the base class
	if (bActivate && pActivateView != pDeactiveView)
	{
		CTaoismSystemInfoDoc* pDoc = reinterpret_cast<CTaoismSystemInfoDoc*>(GetDocument());
		ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CTaoismSystemInfoDoc)));
		pDoc->UpdateActiveViewInfo(GetCurSelTabIndex());
	}

	CListView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CSystemView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CMFCTabCtrl* tab = reinterpret_cast<CMFCTabCtrl*>(GetParent());
	ASSERT(tab->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)));
	CTaoismSystemInfoDoc* pDoc = reinterpret_cast<CTaoismSystemInfoDoc*>(GetDocument());
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CTaoismSystemInfoDoc)));
	INT iCur = GetCurSelTabIndex();
	CString strTitle, str;

	if (iCur == 0)
	{
		pDoc->SetCurSelProcessItem(pNMItemActivate);
		tab->GetTabLabel(iCur, strTitle);
		strTitle.Trim();
		str = GetListCtrl().GetItemText(pNMItemActivate->iItem, LIST_PROCESS_PATH);
		strTitle += L" - ";
		strTitle += str;
		pDoc->SetTitle(strTitle);
	}

	*pResult = 0;
}


void CSystemView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_uSortSubIndex = pNMLV->iSubItem;
	m_iSortSubOrder = ~m_iSortSubOrder;
	GetListCtrl().SortItemsEx(CSystemView::SystemListCompareFunc, (UINT_PTR)this);

	*pResult = 0;
}

INT CALLBACK CSystemView::SystemListCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	INT iResult = 0;
	CSystemView* pSystemView = reinterpret_cast<CSystemView*>(lParamSort);
	INT iIndex = pSystemView->GetCurSelTabIndex();				//��õ�ǰѡ�е�ѡ�����

	if (iIndex==0)		//����ѡ�
	{
		switch (pSystemView->m_uSortSubIndex)
		{
		case LIST_PROCESS_PID:
		case LIST_PROCESS_PARENTID:
			iResult = pSystemView->SortItemByNum(lParam1, lParam2, pSystemView->m_uSortSubIndex);
			break;
		default:
			iResult = pSystemView->SortItemByString(lParam1, lParam2, pSystemView->m_uSortSubIndex);
			break;
		}
	}
	else if (iIndex == 1)			//ģ��ѡ�
	{
		iResult = pSystemView->SortItemByString(lParam1, lParam2, pSystemView->m_uSortSubIndex);
	}
	else if (iIndex == 2)			//�߳�ѡ�
	{
		switch (pSystemView->m_uSortSubIndex)
		{
		case LIST_THREAD_ID:
		case LIST_THREAD_PID:
		case LIST_THREAD_SWITCH:
		case LIST_THREAD_PRIORITY:
			iResult = pSystemView->SortItemByNum(lParam1, lParam2, pSystemView->m_uSortSubIndex);
			break;
		default:
			iResult = pSystemView->SortItemByString(lParam1, lParam2, pSystemView->m_uSortSubIndex);
			break;
		}
	}

	return iResult;
}

//////////////////////////////////////////////////////////////////////////
//	�������ִ�С����
//
INT CSystemView::SortItemByNum(LPARAM lParam1, LPARAM lParam2, UINT_PTR uCloumnIndex)
{
	INT iResult = 0;
	CString strText1, strText2;
	UINT_PTR uValue1 = 0, uValue2 = 0;

	strText1 = GetListCtrl().GetItemText((INT)lParam1, (INT)uCloumnIndex);
	strText2 = GetListCtrl().GetItemText((INT)lParam2, (INT)uCloumnIndex);
	uValue1 = wcstoul(strText1.GetBuffer(), NULL, 16);
	uValue2 = wcstoul(strText2.GetBuffer(), NULL, 16);
	strText1.ReleaseBuffer();
	strText2.ReleaseBuffer();
	if (uValue1 != uValue2)
		if (m_iSortSubOrder)
			iResult = (uValue1 < uValue2) ? -1 : 1;
		else
			iResult = (uValue1 < uValue2) ? 1 : -1;

	return iResult;
}

//////////////////////////////////////////////////////////////////////////
//	�����ַ���ascii������
//
INT CSystemView::SortItemByString(LPARAM lParam1, LPARAM lParam2, UINT_PTR uCloumnIndex)
{
	INT iResult = 0;
	CString strText1, strText2;

	strText1 = GetListCtrl().GetItemText((INT)lParam1, (INT)uCloumnIndex);
	strText2 = GetListCtrl().GetItemText((INT)lParam2, (INT)uCloumnIndex);
	iResult = wcscmp(strText1.GetBuffer(), strText2.GetBuffer());
	strText1.ReleaseBuffer();
	strText2.ReleaseBuffer();
	if (!m_iSortSubOrder)
		if (iResult>0)
			iResult = -1;
		else
			iResult = 1;


	return iResult;
}


void CSystemView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDetailDlg dlg;
	CImageList* pImageList = GetListCtrl().GetImageList(LVSIL_SMALL);
	HICON hIcon = 0;

	dlg.SetProcessId((UINT)GetListCtrl().GetItemData(pNMItemActivate->iItem));
	if (pImageList)
	{
		hIcon = ImageList_GetIcon(pImageList->m_hImageList, pNMItemActivate->iItem, ILD_IMAGE);
		if (hIcon)
			dlg.SetIconHwnd(hIcon);
	}
	dlg.DoModal();

	*pResult = 0;
}


void CSystemView::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CImageList* pImageList = GetListCtrl().GetImageList(LVSIL_SMALL);

	if (pImageList)
	{
		pImageList->DeleteImageList();
	}

	CListView::OnClose();
}
