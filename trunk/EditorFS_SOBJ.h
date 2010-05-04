#if !defined(AFX_EDITORFS_SOBJ_H__B3F5443B_40E0_4178_BAE2_D1D84B2D1AB7__INCLUDED_)
#define AFX_EDITORFS_SOBJ_H__B3F5443B_40E0_4178_BAE2_D1D84B2D1AB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorFS_SOBJ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SOBJ form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CEditorFS_SOBJ : public CFormView
{
protected:
	CToolBar m_TBApplyer;
	CEditorFS_SOBJ();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditorFS_SOBJ)

	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();

// Form Data
public:
	//{{AFX_DATA(CEditorFS_SOBJ)
	enum { IDD = IDD_EDITORFS_SOBJ };
	CEdit	m_Info;
	CEdit	m_Name;
	CEdit	m_Properties;
	CComboBox	m_RotateAxis;
	CButton	m_RotateMode;
	CListBox	m_ListSOBJ;
	CTabCtrl	m_TabSOBJ;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	BOOL FinishPendingEditing();
	void DoDiscard();
	void DoApply();
	BOOL m_DataLocked;
	void InitSOBJMode();
	BOOL UnInit();
	void Init();
	BOOL m_AmEditing;
	afx_msg void OnApplyerBarClick(UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorFS_SOBJ)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEditorFS_SOBJ();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEditorFS_SOBJ)
	afx_msg void OnSelchangeEditorfsSobjDetaillevel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeEditorfsSobjSubmodelslist();
	afx_msg void OnEditorfsSobjRotatemode();
	afx_msg void OnChangeData();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORFS_SOBJ_H__B3F5443B_40E0_4178_BAE2_D1D84B2D1AB7__INCLUDED_)
