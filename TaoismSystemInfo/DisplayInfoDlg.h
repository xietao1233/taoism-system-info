#pragma once
#include "afxcmn.h"


// CUserTimerDlg �Ի���

class CDisplayInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplayInfoDlg)

public:
	//CDisplayInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CDisplayInfoDlg(UINT uDlgType, CWnd* pParent =NULL);
	virtual ~CDisplayInfoDlg();

// �Ի�������
	enum { IDD = IDD_DLG_DISPLAYINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	UINT m_uDlgType;
	RECT m_rcDisplayDlg;
	RECT m_rcMainList;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CDetailDlg* GetDetailDlgPtr();
	VOID UpdateDisplayDlgLocation();
	virtual BOOL OnInitDialog();
	VOID SetControlLocation();
	CListCtrl m_listMainInfo;
	VOID UpdateControlLocation();
	VOID InitDisplayList();
};
