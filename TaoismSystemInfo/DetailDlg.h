#pragma once
#include "afxcmn.h"
#include "Common.h"

class CDisplayInfoDlg;
// CDetailDlg �Ի���

class CDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDetailDlg)

public:
	CDetailDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDetailDlg();
	static TABCTRLINFO stTabInfo[];
	static LISTCOLUMNINFO stLCListSecurityInfo[];
	static LISTCOLUMNINFO stLCListModuleInfo[];
	static LISTCOLUMNINFO stLCThreadInfo[];
	static LISTCOLUMNINFO stLCWindowInfo[];
	static LISTCOLUMNINFO stLCUserTimerInfo[];

	static LPLISTCOLUMNINFO stProcesDetailDlg[];

	enum enumDlgType
	{
		ModuleInfo,
		ThreadInfo,
		SeccrityInfo,
		WindowInfo,
		UserTimer,
	};

// �Ի�������
	enum { IDD = IDD_DLG_DETAILINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	UINT m_uProcessId;
	RECT m_rcMarginTab;
	RECT m_rcMarginDisplayDlg;
	HICON m_hIcon;

	VOID ShowActiveTabItem();					//��ʾ������, ���طǼ�����

public:
	VOID SetProcessId(UINT uPid)			{ m_uProcessId = uPid; }
	CTabCtrl m_tabMainInfo;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	VOID ShowIndexDialog(UINT_PTR uIndexDlg);
	VOID ChangeTitle();
	VOID StringTrim(CString& str);
	CDisplayInfoDlg* GetDisplayInfoDlgPtr(UINT uIndex);
	VOID InitDlgSize();
	VOID ClearAllDialogInfo();
	afx_msg void OnClose();
	CRect GetDisplayDlgMargin()		{ return CRect(m_rcMarginDisplayDlg); }
	afx_msg void OnSize(UINT nType, int cx, int cy);
	VOID ResetControlLocation(UINT nType, int cx, int cy);
	VOID SetTabLocation();
	VOID NotifyChildDlg(WPARAM wParam, LPARAM lParam);
	VOID SetIconHwnd(HICON hIcon)				{ m_hIcon = hIcon; }
};
