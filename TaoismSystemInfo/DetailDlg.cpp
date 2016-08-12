// DetailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaoismSystemInfo.h"
#include "DetailDlg.h"
#include "afxdialogex.h"
#include "DisplayInfoDlg.h"
#include "Common.h"


TABCTRLINFO CDetailDlg::stTabInfo[] = {
	{ CDetailDlg::ModuleInfo, L"         ģ����Ϣ         ", 100 },
	{ CDetailDlg::ThreadInfo, L"         �߳���Ϣ         ", 100 },
	{ CDetailDlg::SeccrityInfo, L"         ��ȫ��Ϣ         ", 100 },
	{ CDetailDlg::WindowInfo, L"         ������Ϣ         ", 100 },
	{ CDetailDlg::UserTimer, L"         ��ʱ��         ", 100 },
};

LISTCOLUMNINFO CDetailDlg::stLCListModuleInfo[] = {
	{ LIST_MODULE_NAME, L"ģ����", LVCFMT_LEFT, 150 },
	{ LIST_MODULE_PATH, L"ģ��·��", LVCFMT_LEFT, 350 },
	{ LIST_MODULE_BASEADDR, L"ģ���ַ", LVCFMT_CENTER, 220 },
	{ LIST_MODULE_SIZE, L"ģ���С", LVCFMT_CENTER, 220 },
	{ LIST_MODULE_OEP, L"��ڵ�ַ", LVCFMT_CENTER, 220 },
	{ LIST_MODULE_CO, L"������˾", LVCFMT_LEFT, 180 },
	{ 0, 0, 0, 0 }
};

LISTCOLUMNINFO CDetailDlg::stLCThreadInfo[] = {
	{ LIST_THREAD_ID, L"�߳�ID", LVCFMT_LEFT, 100 },
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

LISTCOLUMNINFO CDetailDlg::stLCListSecurityInfo[] = {
	{ LIST_PROCESS_DETAIL_TITLE, L"��Ŀ", LVCFMT_LEFT, 100 },
	{ LIST_PROCESS_DETAIL_CONTENT, L"����", LVCFMT_LEFT, 500 },
	{ 0, 0, 0, 0 }
};

LISTCOLUMNINFO CDetailDlg::stLCWindowInfo[] = {
	{ LIST_WINDOW_HANDLE, L"���ھ��", LVCFMT_LEFT, 100 },
	{ LIST_WINDOW_TITLE, L"���ڱ���", LVCFMT_LEFT, 180 },
	{ LIST_WINDOW_CLASSNAME, L"��������", LVCFMT_LEFT, 180 },
	{ LIST_WINDOW_SHOW, L"���ڿɼ���", LVCFMT_LEFT, 120 },
	{ LIST_WINDOW_PID, L"��������ID", LVCFMT_LEFT, 120 },
	{ LIST_WINDOW_TID, L"�����߳�ID", LVCFMT_LEFT, 120 },
	{ LIST_WINDOW_CALLBACKPROC, L"�ص�������ַ", LVCFMT_LEFT, 150 },
	{ 0, 0, 0, 0 }
};

LISTCOLUMNINFO CDetailDlg::stLCUserTimerInfo[] = {
	{ LIST_USERTIMER_CALLBACKFUNC, L"�ص�������ַ", LVCFMT_LEFT, 100 },
	{ LIST_USERTIMER_MODULEINFO, L"����ģ��", LVCFMT_LEFT, 100 },
	{ LIST_USERTIMER_TID, L"�߳�ID", LVCFMT_LEFT, 100 },
	{ LIST_USERTIMER_PID, L"����ID", LVCFMT_LEFT, 100 }, 
	{ 0, 0, 0, 0 }
};

LPLISTCOLUMNINFO CDetailDlg::stProcesDetailDlg[] = {
	(LPLISTCOLUMNINFO)&CDetailDlg::stLCListSecurityInfo,
	(LPLISTCOLUMNINFO)&CDetailDlg::stLCListModuleInfo,
	(LPLISTCOLUMNINFO)&CDetailDlg::stLCThreadInfo,
	(LPLISTCOLUMNINFO)&CDetailDlg::stLCWindowInfo,
	(LPLISTCOLUMNINFO)&CDetailDlg::stLCUserTimerInfo,
};

// CDetailDlg �Ի���

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


// CDetailDlg ��Ϣ�������


void CDetailDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UINT uItem = m_tabMainInfo.GetCurSel();
	ShowIndexDialog(uItem);	
	ShowActiveTabItem();

	*pResult = 0;
}


BOOL CDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitDlgSize();
	
	for (UINT u = 0; u < _countof(stTabInfo); u++)
	{
		m_tabMainInfo.InsertItem(TCIF_PARAM | TCIF_TEXT, u, stTabInfo[u].LabelText, NULL, NULL);
	}

	if (m_hIcon)
		SetIcon(m_hIcon, FALSE);

	ShowIndexDialog(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
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

	//ȥ��β���ո�
	for (INT u = iLen - 1; u >= 0; u--)
	{
		if (*(pBuff + u) == ' ')
			*(pBuff + u) = 0;
		else
			break;
	}

	//ȥ��ͷ���ո�
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

	//����ԭʼֵ
	GetClientRect(&rcDlg);
	m_tabMainInfo.GetWindowRect(&rcTab);
	ScreenToClient(&rcTab);
	m_rcMarginTab.left = rcTab.left - rcDlg.left;			//������߾�
	m_rcMarginTab.top = rcTab.top - rcDlg.top;
	m_rcMarginTab.right = rcDlg.right - rcTab.right;
	m_rcMarginTab.bottom = rcDlg.bottom - rcTab.bottom;

	//���ĶԻ����С
	GetWindowRect(&rcDlg);
	rcDlg.right += 0x90;
	rcDlg.bottom += 0x90;
	SetWindowPos(NULL, rcDlg.left, rcDlg.top, rcDlg.right - rcDlg.left, rcDlg.bottom - rcDlg.top, NULL);

	//����Tab�ؼ�λ��
	SetTabLocation();

	//��ʼ��DispalyDlg�߾�
	m_tabMainInfo.GetClientRect(&rcTab);
	m_tabMainInfo.GetItemRect(0, &rcItem);
	m_rcMarginDisplayDlg.left = rcTab.left + 1;				//��¼��߾�
	m_rcMarginDisplayDlg.top = rcItem.bottom - rcItem.top + 4;			//��¼�����߾�
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ClearAllDialogInfo();
	::DestroyIcon(m_hIcon);
	
	CDialogEx::OnClose();
}


void CDetailDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	ResetControlLocation(nType, cx, cy);
}


VOID CDetailDlg::ResetControlLocation(UINT nType, int cx, int cy)
{
	if (m_tabMainInfo)
	{
		SetTabLocation();			//����tab�ؼ�λ��
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
