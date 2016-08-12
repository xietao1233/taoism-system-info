// UserTimerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TaoismSystemInfo.h"
#include "DetailDlg.h"
#include "DisplayInfoDlg.h"
#include "afxdialogex.h"


// CUserTimerDlg 对话框

IMPLEMENT_DYNAMIC(CDisplayInfoDlg, CDialogEx)


CDisplayInfoDlg::CDisplayInfoDlg(UINT uDlgType, CWnd* pParent /*=NULL*/)
	: CDialogEx(CDisplayInfoDlg::IDD, pParent)
	, m_uDlgType(0)
{
	m_uDlgType = uDlgType;
}

// CDisplayInfoDlg::CDisplayInfoDlg(CWnd* pParent /*= NULL*/)
// 	: CDialogEx(CDisplayInfoDlg::IDD, pParent)
// {
// 
// }

CDisplayInfoDlg::~CDisplayInfoDlg()
{
}

void CDisplayInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USERTIMER, m_listMainInfo);
}


BEGIN_MESSAGE_MAP(CDisplayInfoDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CUserTimerDlg 消息处理程序


void CDisplayInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	UpdateDisplayDlgLocation();
}


CDetailDlg* CDisplayInfoDlg::GetDetailDlgPtr()
{
	CTabCtrl* pTab = reinterpret_cast<CTabCtrl*>(GetParent());
	ASSERT(pTab && pTab->IsKindOf(RUNTIME_CLASS(CTabCtrl)));

	return reinterpret_cast<CDetailDlg*>(pTab->GetParent());
}


VOID CDisplayInfoDlg::UpdateDisplayDlgLocation()
{
	CTabCtrl* pTab = reinterpret_cast<CTabCtrl*>(GetParent());
	ASSERT(pTab && pTab->IsKindOf(RUNTIME_CLASS(CTabCtrl)));
	CDetailDlg* pDetailDlg = GetDetailDlgPtr();
	ASSERT(pDetailDlg && pDetailDlg->IsKindOf(RUNTIME_CLASS(CDetailDlg)));
	CRect rcMargin;
	RECT rcTab = { 0 };

	pTab->GetClientRect(&rcTab);
	m_rcDisplayDlg = pDetailDlg->GetDisplayDlgMargin();
	rcMargin.left = m_rcDisplayDlg.left;
	rcMargin.top = m_rcDisplayDlg.top;
	rcMargin.right = rcTab.right - 3;
	rcMargin.bottom = rcTab.bottom - 2;
	MoveWindow(&rcMargin);

	UpdateControlLocation();
}


BOOL CDisplayInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetControlLocation();
	InitDisplayList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


VOID CDisplayInfoDlg::SetControlLocation()
{
	RECT rcDlg = { 0 }, rcList = { 0 };

	//保存list控件边距信息
	GetClientRect(&rcDlg);
	m_listMainInfo.GetWindowRect(&rcList);
	ScreenToClient(&rcList);
	m_rcMainList.left = rcList.left - rcDlg.left;
	m_rcMainList.top = rcList.top - rcDlg.top;
	m_rcMainList.right = rcDlg.right - m_rcMainList.left;
	m_rcMainList.bottom = rcDlg.bottom - m_rcMainList.top;
	m_listMainInfo.MoveWindow(&m_rcMainList);
}


VOID CDisplayInfoDlg::UpdateControlLocation()
{
	if (m_listMainInfo)
		SetControlLocation();
}


VOID CDisplayInfoDlg::InitDisplayList()
{
	LONG_PTR uWndStyle = 0;
	UINT uIndex = 0;
	LISTCOLUMNINFO* pListColumnInfo = CDetailDlg::stProcesDetailDlg[m_uDlgType];

	while (wcslen(pListColumnInfo->ColumnText))
	{
		m_listMainInfo.InsertColumn(pListColumnInfo->Index, pListColumnInfo->ColumnText, pListColumnInfo->Align, pListColumnInfo->Width);
		pListColumnInfo++;
	}

	uWndStyle = ::GetWindowLongPtrW(m_listMainInfo.GetSafeHwnd(), GWL_STYLE);
	uWndStyle |= (WS_HSCROLL | WS_VSCROLL);
	::SetWindowLongPtrW(m_listMainInfo.GetSafeHwnd(), GWL_STYLE, uWndStyle);
}
