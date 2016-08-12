// DetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TaoismSystemInfo.h"
#include "DetailDlg.h"
#include "afxdialogex.h"
#include "DisplayInfoDlg.h"
#include "Common.h"


TABCTRLINFO CDetailDlg::stTabInfo[] = {
	{ CDetailDlg::ModuleInfo, L"         模块信息         ", 100 },
	{ CDetailDlg::ThreadInfo, L"         线程信息         ", 100 },
	{ CDetailDlg::SeccrityInfo, L"         安全信息         ", 100 },
	{ CDetailDlg::WindowInfo, L"         窗口信息         ", 100 },
	{ CDetailDlg::UserTimer, L"         定时器         ", 100 },
};

LISTCOLUMNINFO CDetailDlg::stLCListModuleInfo[] = {
	{ LIST_MODULE_NAME, L"模块名", LVCFMT_LEFT, 150 },
	{ LIST_MODULE_PATH, L"模块路径", LVCFMT_LEFT, 350 },
	{ LIST_MODULE_BASEADDR, L"模块基址", LVCFMT_CENTER, 220 },
	{ LIST_MODULE_SIZE, L"模块大小", LVCFMT_CENTER, 220 },
	{ LIST_MODULE_OEP, L"入口地址", LVCFMT_CENTER, 220 },
	{ LIST_MODULE_CO, L"开发公司", LVCFMT_LEFT, 180 },
	{ 0, 0, 0, 0 }
};

LISTCOLUMNINFO CDetailDlg::stLCThreadInfo[] = {
	{ LIST_THREAD_ID, L"线程ID", LVCFMT_LEFT, 100 },
	{ LIST_THREAD_PID, L"所属进程ID", LVCFMT_LEFT, 110 },
	{ LIST_THREAD_PRIORITY, L"优先级", LVCFMT_LEFT, 80 },
	{ LIST_THREAD_TEB, L"TEB", LVCFMT_LEFT, 220 },
	{ LIST_THREAD_ADDRESS, L"入口地址", LVCFMT_LEFT, 220 },
	{ LIST_THREAD_PARAM, L"线程参数", LVCFMT_LEFT, 120 },
	{ LIST_THREAD_MODULE, L"所属模块", LVCFMT_LEFT, 140 },
	{ LIST_THREAD_SWITCH, L"切换次数", LVCFMT_LEFT, 90 },
	{ LIST_THREAD_STATE, L"线程状态", LVCFMT_LEFT, 90 },
	{ 0, 0, 0, 0 }
};

LISTCOLUMNINFO CDetailDlg::stLCListSecurityInfo[] = {
	{ LIST_PROCESS_DETAIL_TITLE, L"项目", LVCFMT_LEFT, 100 },
	{ LIST_PROCESS_DETAIL_CONTENT, L"内容", LVCFMT_LEFT, 500 },
	{ 0, 0, 0, 0 }
};

LISTCOLUMNINFO CDetailDlg::stLCWindowInfo[] = {
	{ LIST_WINDOW_HANDLE, L"窗口句柄", LVCFMT_LEFT, 100 },
	{ LIST_WINDOW_TITLE, L"窗口标题", LVCFMT_LEFT, 180 },
	{ LIST_WINDOW_CLASSNAME, L"窗口类名", LVCFMT_LEFT, 180 },
	{ LIST_WINDOW_SHOW, L"窗口可见性", LVCFMT_LEFT, 120 },
	{ LIST_WINDOW_PID, L"所属进程ID", LVCFMT_LEFT, 120 },
	{ LIST_WINDOW_TID, L"所属线程ID", LVCFMT_LEFT, 120 },
	{ LIST_WINDOW_CALLBACKPROC, L"回调函数地址", LVCFMT_LEFT, 150 },
	{ 0, 0, 0, 0 }
};

LISTCOLUMNINFO CDetailDlg::stLCUserTimerInfo[] = {
	{ LIST_USERTIMER_CALLBACKFUNC, L"回调函数地址", LVCFMT_LEFT, 100 },
	{ LIST_USERTIMER_MODULEINFO, L"所属模块", LVCFMT_LEFT, 100 },
	{ LIST_USERTIMER_TID, L"线程ID", LVCFMT_LEFT, 100 },
	{ LIST_USERTIMER_PID, L"进程ID", LVCFMT_LEFT, 100 }, 
	{ 0, 0, 0, 0 }
};

LPLISTCOLUMNINFO CDetailDlg::stProcesDetailDlg[] = {
	(LPLISTCOLUMNINFO)&CDetailDlg::stLCListSecurityInfo,
	(LPLISTCOLUMNINFO)&CDetailDlg::stLCListModuleInfo,
	(LPLISTCOLUMNINFO)&CDetailDlg::stLCThreadInfo,
	(LPLISTCOLUMNINFO)&CDetailDlg::stLCWindowInfo,
	(LPLISTCOLUMNINFO)&CDetailDlg::stLCUserTimerInfo,
};

// CDetailDlg 对话框

IMPLEMENT_DYNAMIC(CDetailDlg, CDialogEx)

CDetailDlg::CDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDetailDlg::IDD, pParent)
	, m_uProcessId(0)
	, m_rcMarginTab({0})
	, m_rcMarginDisplayDlg({0})
{

}

CDetailDlg::~CDetailDlg()
{

}

void CDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabMainInfo);
}


BEGIN_MESSAGE_MAP(CDetailDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CDetailDlg::OnTcnSelchangeTabMain)
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDetailDlg 消息处理程序


void CDetailDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	UINT uItem = m_tabMainInfo.GetCurSel();
	ShowIndexDialog(uItem);	
	ShowActiveTabItem();

	*pResult = 0;
}


BOOL CDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitDlgSize();
	
	for (UINT u = 0; u < _countof(stTabInfo); u++)
	{
		m_tabMainInfo.InsertItem(TCIF_PARAM | TCIF_TEXT, u, stTabInfo[u].LabelText, NULL, NULL);
	}

	if (m_hIcon)
		SetIcon(m_hIcon, FALSE);

	ShowIndexDialog(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


VOID CDetailDlg::ShowIndexDialog(UINT_PTR uIndexDlg)
{
	RECT rc = { 0 }, rcItem = { 0 };
	CDisplayInfoDlg* pDlg = GetDisplayInfoDlgPtr(m_tabMainInfo.GetCurSel());

	ASSERT(pDlg);
	pDlg->ShowWindow(SW_SHOW);

	ChangeTitle();
}


VOID CDetailDlg::ChangeTitle()
{
	UINT uItem = m_tabMainInfo.GetCurSel();
	CString strTitle;
	TCITEMW stTC = { 0 };

	stTC.mask = TCIF_TEXT;
	stTC.pszText = strTitle.GetBuffer(0x200);
	strTitle.ReleaseBuffer();
	stTC.cchTextMax = 0x200;
	m_tabMainInfo.GetItem(uItem, &stTC);
	StringTrim(strTitle);
	SetWindowTextW(strTitle);
}


VOID CDetailDlg::StringTrim(CString& str)
{
	CString strResult = L"";
	LPWSTR pBuff = str.GetBuffer();
	INT iLen = (INT)wcslen(pBuff);

	//去除尾部空格
	for (INT u = iLen - 1; u >= 0; u--)
	{
		if (*(pBuff + u) == ' ')
			*(pBuff + u) = 0;
		else
			break;
	}

	//去除头部空格
	for (INT u = 0; u < iLen; u++)
	{
		if (*pBuff == ' ')
			pBuff++;
		else
			break;
	}

	strResult = pBuff;
	str.ReleaseBuffer();
	str = strResult;
}


CDisplayInfoDlg* CDetailDlg::GetDisplayInfoDlgPtr(UINT uIndex)
{
	CDisplayInfoDlg* pResult = 0;
	TCITEMW stTC = { 0 };

	stTC.mask = TCIF_PARAM;
	m_tabMainInfo.GetItem(uIndex, &stTC);
	pResult = reinterpret_cast<CDisplayInfoDlg*>(stTC.lParam);
	if (!pResult)
	{
		pResult = new CDisplayInfoDlg(uIndex);
		stTC.lParam = (LPARAM)pResult;
		m_tabMainInfo.SetItem(uIndex, &stTC);
		pResult->Create(IDD_DLG_DISPLAYINFO, (CWnd*)&m_tabMainInfo);
	}

	return pResult;
}


VOID CDetailDlg::InitDlgSize()
{
	RECT rcDlg = { 0 }, rcTab = { 0 }, rcItem = { 0 };

	//保存原始值
	GetClientRect(&rcDlg);
	m_tabMainInfo.GetWindowRect(&rcTab);
	ScreenToClient(&rcTab);
	m_rcMarginTab.left = rcTab.left - rcDlg.left;			//保存左边距
	m_rcMarginTab.top = rcTab.top - rcDlg.top;
	m_rcMarginTab.right = rcDlg.right - rcTab.right;
	m_rcMarginTab.bottom = rcDlg.bottom - rcTab.bottom;

	//更改对话框大小
	GetWindowRect(&rcDlg);
	rcDlg.right += 0x90;
	rcDlg.bottom += 0x90;
	SetWindowPos(NULL, rcDlg.left, rcDlg.top, rcDlg.right - rcDlg.left, rcDlg.bottom - rcDlg.top, NULL);

	//设置Tab控件位置
	SetTabLocation();

	//初始化DispalyDlg边距
	m_tabMainInfo.GetClientRect(&rcTab);
	m_tabMainInfo.GetItemRect(0, &rcItem);
	m_rcMarginDisplayDlg.left = rcTab.left + 1;				//记录左边距
	m_rcMarginDisplayDlg.top = rcItem.bottom - rcItem.top + 4;			//记录顶部边距
// 	m_rcMarginDisplayDlg.right = rcTab.right - 3;
// 	m_rcMarginDisplayDlg.bottom = rcTab.bottom - 2;
}


VOID CDetailDlg::ClearAllDialogInfo()
{
	CDisplayInfoDlg* pDisplayDlg = NULL;
	TCITEMW stTC = { 0 };

	stTC.mask = TCIF_PARAM;
	for (INT u = 0; u < m_tabMainInfo.GetItemCount(); u++)
	{
		m_tabMainInfo.GetItem(u, &stTC);
		pDisplayDlg = reinterpret_cast<CDisplayInfoDlg*>(stTC.lParam);
		if (pDisplayDlg)
		{
			ASSERT(pDisplayDlg && pDisplayDlg->IsKindOf(RUNTIME_CLASS(CDisplayInfoDlg)));
			delete pDisplayDlg;
			pDisplayDlg = NULL;
			stTC.lParam = NULL;
			m_tabMainInfo.SetItem(u, &stTC);
		}
	}
}


void CDetailDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	ClearAllDialogInfo();
	::DestroyIcon(m_hIcon);
	
	CDialogEx::OnClose();
}


void CDetailDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	ResetControlLocation(nType, cx, cy);
}


VOID CDetailDlg::ResetControlLocation(UINT nType, int cx, int cy)
{
	if (m_tabMainInfo)
	{
		SetTabLocation();			//更新tab控件位置
		NotifyChildDlg(nType, MAKELPARAM(cx, cy));
	}
}


VOID CDetailDlg::SetTabLocation()
{
	RECT rcDlg = { 0 }, rcTab = { 0 };

	GetClientRect(&rcDlg);
	rcTab.left = rcDlg.left + m_rcMarginTab.left;
	rcTab.top = rcDlg.top + m_rcMarginTab.top;
	rcTab.right = rcDlg.right - m_rcMarginTab.right;
	rcTab.bottom = rcDlg.bottom - m_rcMarginTab.bottom;
	m_tabMainInfo.MoveWindow(&rcTab);
}


VOID CDetailDlg::NotifyChildDlg(WPARAM wParam, LPARAM lParam)
{
	CDisplayInfoDlg* pDisplayDlg = NULL;
	TCITEMW stTC = { 0 };

	stTC.mask = TCIF_PARAM;
	for (INT i = 0; i < m_tabMainInfo.GetItemCount(); i++)
	{
		m_tabMainInfo.GetItem(i, &stTC);
		pDisplayDlg = reinterpret_cast<CDisplayInfoDlg*>(stTC.lParam);
		if (pDisplayDlg)
			pDisplayDlg->SendMessageW(WM_SIZE, wParam, lParam);
	}
}


VOID CDetailDlg::ShowActiveTabItem()
{
	CDisplayInfoDlg* pDlg = NULL;
	INT iActive = m_tabMainInfo.GetCurSel();
	INT iCount = m_tabMainInfo.GetItemCount();
	TCITEMW stTC = { 0 };

	stTC.mask = TCIF_PARAM;
	for (INT i = 0; i < iCount; i++)
	{
		m_tabMainInfo.GetItem(i, &stTC);
		pDlg = reinterpret_cast<CDisplayInfoDlg*>(stTC.lParam);
		if (pDlg)
		{
			ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CDisplayInfoDlg)));
			if (i == iActive)
				pDlg->ShowWindow(SW_SHOW);
			else
				pDlg->ShowWindow(SW_HIDE);
		}
	}
}
