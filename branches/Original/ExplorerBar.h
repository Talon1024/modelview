#if !defined(AFX_EXPLORERBAR_H__DE9B765C_742C_4FAE_9FBD_A6C7B0DBC5FD__INCLUDED_)
#define AFX_EXPLORERBAR_H__DE9B765C_742C_4FAE_9FBD_A6C7B0DBC5FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExplorerBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExplorerBar form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CExplorerBar : public CFormView
{
protected:
	CExplorerBar();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CExplorerBar)

// Form Data
public:
	//{{AFX_DATA(CExplorerBar)
	enum { IDD = IDD_EXPLORER_BAR };
	CStatic	m_ToolBarPlaceHolder;
	//}}AFX_DATA

// Attributes
public:
	CToolBar m_ToolBar;
	CToolTipCtrl m_ToolTip;
	BOOL alreadyinited;

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExplorerBar)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CExplorerBar();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CExplorerBar)
	afx_msg void OnExplorerBarClick(UINT nID);
	afx_msg void OnUpdateExplorerBarTreeView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExplorerBarFilter(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExplorerviewComparisontable();
	afx_msg void OnUpdateExplorerviewComparisontable(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLORERBAR_H__DE9B765C_742C_4FAE_9FBD_A6C7B0DBC5FD__INCLUDED_)
