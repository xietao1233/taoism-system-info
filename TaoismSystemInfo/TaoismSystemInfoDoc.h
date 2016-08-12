
// TaoismSystemInfoDoc.h : interface of the CTaoismSystemInfoDoc class
//


#pragma once

#include "SystemView.h"


class CTaoismSystemInfoDoc : public CDocument
{
protected: // create from serialization only
	CTaoismSystemInfoDoc();
	DECLARE_DYNCREATE(CTaoismSystemInfoDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CTaoismSystemInfoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

#ifdef SHARED_HANDLERS
private:
	CString m_strSearchContent;
	CString m_strThumbnailContent;
#endif // SHARED_HANDLERS
public:
	LPNMITEMACTIVATE m_pCurrentItemActive;
	BOOL SetCurSelProcessItem(LPNMITEMACTIVATE pProcessItem);
	BOOL UpdateActiveViewInfo(INT iCurSelIndex);
	BOOL UpdateProcessInfo(CSystemView* pProcessView);
	BOOL UpdateModuleInfo(CSystemView* pModuleView);
	BOOL UpdateThreadInfo(CSystemView* pThreadView);
	BOOL UpdateWindowInfo(CSystemView* pWindowView);
	static INT_PTR UpdateProcessContent(CSystemView* pProcessView);
	static INT_PTR UpdateModuleContent(CSystemView* pModuleView);
	static INT_PTR UpdateThreadContent(CSystemView* pThreadView);

	UINT GetCurSelProcessID();
};
