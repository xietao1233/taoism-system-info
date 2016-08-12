
// TaoismSystemInfoView.h : interface of the CTaoismSystemInfoView class
//

#pragma once
#include "Common.h"
#include "TaoismSystemInfoDoc.h"

class CTaoismSystemInfoView : public CTabView
{
protected: // create from serialization only
	CTaoismSystemInfoView();
	DECLARE_DYNCREATE(CTaoismSystemInfoView)

// Attributes
public:
	CTaoismSystemInfoDoc* GetDocument() const;
	static TABCTRLINFO stTabInfo[];

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTaoismSystemInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnClose();
};

#ifndef _DEBUG  // debug version in TaoismSystemInfoView.cpp
inline CTaoismSystemInfoDoc* CTaoismSystemInfoView::GetDocument() const
   { return reinterpret_cast<CTaoismSystemInfoDoc*>(m_pDocument); }
#endif

