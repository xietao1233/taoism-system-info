
// TaoismSystemInfoView.cpp : implementation of the CTaoismSystemInfoView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TaoismSystemInfo.h"
#endif
#include "SystemView.h"
#include "TaoismSystemInfoDoc.h"
#include "MainFrm.h"
#include "TaoismSystemInfoView.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


TABCTRLINFO CTaoismSystemInfoView::stTabInfo[] = {
	{ TAB_INDEX_PROCESS, L"          进程信息          ", 100 },
	{ TAB_INDEX_MODUOLE, L"          模块信息          ", 100 },
	{ TAB_INDEX_THREAD, L"          线程信息          ", 100 },
	{ TAB_INDEX_WINDOW, L"          窗口信息          ", 100 }
};

// CTaoismSystemInfoView

IMPLEMENT_DYNCREATE(CTaoismSystemInfoView, CTabView)

BEGIN_MESSAGE_MAP(CTaoismSystemInfoView, CTabView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CTabView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CTabView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTabView::OnFilePrintPreview)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CTaoismSystemInfoView construction/destruction

CTaoismSystemInfoView::CTaoismSystemInfoView()
{
	// TODO: add construction code here

}

CTaoismSystemInfoView::~CTaoismSystemInfoView()
{
}

BOOL CTaoismSystemInfoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CTabView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}


// CTaoismSystemInfoView printing

BOOL CTaoismSystemInfoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CTabView preparation
	return CTabView::OnPreparePrinting(pInfo);
}

void CTaoismSystemInfoView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CTabView begin printing
	CTabView::OnBeginPrinting(pDC, pInfo);
}

void CTaoismSystemInfoView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CTabView end printing
	CTabView::OnEndPrinting(pDC, pInfo);
}


// CTaoismSystemInfoView diagnostics

#ifdef _DEBUG
void CTaoismSystemInfoView::AssertValid() const
{
	CTabView::AssertValid();
}

void CTaoismSystemInfoView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}

CTaoismSystemInfoDoc* CTaoismSystemInfoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTaoismSystemInfoDoc)));
	return (CTaoismSystemInfoDoc*)m_pDocument;
}
#endif //_DEBUG


// CTaoismSystemInfoView message handlers


void CTaoismSystemInfoView::OnInitialUpdate()
{
	CTabView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	AddView(RUNTIME_CLASS(CSystemView), stTabInfo[TAB_INDEX_PROCESS].LabelText, TAB_INDEX_PROCESS);
	AddView(RUNTIME_CLASS(CSystemView), stTabInfo[TAB_INDEX_MODUOLE].LabelText, TAB_INDEX_MODUOLE);
	AddView(RUNTIME_CLASS(CSystemView), stTabInfo[TAB_INDEX_THREAD].LabelText, TAB_INDEX_THREAD);
	AddView(RUNTIME_CLASS(CSystemView), stTabInfo[TAB_INDEX_WINDOW].LabelText, TAB_INDEX_WINDOW);

	this->GetTabControl().ModifyTabStyle(CMFCTabCtrl::STYLE_3D_VS2005);    //风格
	this->GetTabControl().EnableAutoColor(TRUE);  //自动着色
	this->GetTabControl().SetTabBorderSize(5); //边框大小
	//this->GetTabControl().HideSingleTab(TRUE);   //单个Tab时候不显示Tab标签
	this->GetTabControl().EnableTabSwap(FALSE);    //禁止拖动 
	this->GetTabControl().SetFlatFrame(TRUE);
	this->GetTabControl().SetLocation(CMFCTabCtrl::LOCATION_TOP);	//标签出现在顶部
}


BOOL CTaoismSystemInfoView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CTabView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CTaoismSystemInfoView::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CTaoismSystemInfoDoc* pDoc = reinterpret_cast<CTaoismSystemInfoDoc*>(GetDocument());
	ASSERT(pDoc && pDoc->IsKindOf(RUNTIME_CLASS(CTaoismSystemInfoDoc)));
	CSystemView* pSystemView = NULL;
	POSITION   pos = pDoc->GetFirstViewPosition(); //GetNextView(pos);
	while (pos)
	{
		pSystemView = reinterpret_cast<CSystemView*>(pDoc->GetNextView(pos));
		ASSERT(pSystemView);
		if (pSystemView->GetParent()->GetSafeHwnd() != GetTabControl().GetSafeHwnd())
			continue;
		ASSERT(pSystemView->IsKindOf(RUNTIME_CLASS(CSystemView)));
		pSystemView->SendMessage(WM_CLOSE, NULL, NULL);
	}

	CTabView::OnClose();
}
