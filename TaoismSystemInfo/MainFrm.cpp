
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TaoismSystemInfo.h"
#include "TaoismSystemInfoView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_QUIT, &CMainFrame::OnFileQuit)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers



BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
{
	// TODO:  在此添加专用代码和/或调用基类

	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, MAKEINTRESOURCEW(IDR_MENU_MAIN), dwExStyle, pContext);
}


void CMainFrame::OnFileQuit()
{
	// TODO:  在此添加命令处理程序代码
	SendMessageW(WM_CLOSE, NULL, NULL);
}


BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO:  在此添加专用代码和/或调用基类
	CTaoismSystemInfoView* pView = NULL;

	switch (message)
	{
	case  WM_CLOSE:
		pView = GetTaoismViewPtr();
		ASSERT(pView && pView->IsKindOf(RUNTIME_CLASS(CTaoismSystemInfoView)));
		pView->SendMessage(WM_CLOSE, wParam, lParam);
		break;
	}

	return CFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
}

CTaoismSystemInfoView* CMainFrame::GetTaoismViewPtr()
{
	CTaoismSystemInfoView* pResult = 0;
	CSystemView* pSystemView = GetActiveSystemViewPtr();
	ASSERT(pSystemView && pSystemView->IsKindOf(RUNTIME_CLASS(CSystemView)));
	CMFCTabCtrl* pTab = reinterpret_cast<CMFCTabCtrl*>(pSystemView->GetParent());
	ASSERT(pTab && pTab->IsKindOf(RUNTIME_CLASS(CMFCTabCtrl)));
	
	return reinterpret_cast<CTaoismSystemInfoView*>(pTab->GetParent());
}

CSystemView* CMainFrame::GetActiveSystemViewPtr()
{
	return reinterpret_cast<CSystemView*>(GetActiveView());
}
