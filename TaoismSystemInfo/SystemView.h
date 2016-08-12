#pragma once
#include "Common.h"
#include "..\\SystemInfo\\ProcessInfo.h"

// CSystemView view

class CSystemView : public CListView
{
	DECLARE_DYNCREATE(CSystemView)

protected:
	CSystemView();           // protected constructor used by dynamic creation
	virtual ~CSystemView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	static LPLISTCOLUMNINFO m_pListHeadInfo[];
	CRITICAL_SECTION m_csUpdateInfo;
	UINT m_uSortSubIndex;
	INT m_iSortSubOrder;
	ZwQuerySystemInformation m_pfnZwQuerySystemInformation;
	static INT CALLBACK SystemListCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

private:
	INT GetCurSelTabIndex();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	INT SortItemByNum(LPARAM lParam1, LPARAM lParam2, UINT_PTR uCloumnIndex);
	INT SortItemByString(LPARAM lParam1, LPARAM lParam2, UINT_PTR uCloumnIndex);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};


