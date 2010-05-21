#if !defined(AFX_EDITORFS_SHLD_H__9E7C2ED7_A335_4DF8_91C6_E48628F9705A__INCLUDED_)
#define AFX_EDITORFS_SHLD_H__9E7C2ED7_A335_4DF8_91C6_E48628F9705A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorFS_SHLD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SHLD form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CEditorFS_SHLD : public CFormView
{
protected:
	CEditorFS_SHLD();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditorFS_SHLD)

	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();

// Form Data
public:
	//{{AFX_DATA(CEditorFS_SHLD)
	enum { IDD = IDD_EDITORFS_SHLD };
	CEdit	m_Modify_MultZ;
	CEdit	m_Modify_MultY;
	CEdit	m_Modify_MultX;
	CStatic	m_Current_Polygons;
	CStatic	m_Current_Vertices;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void OnApplyerBarClick(UINT nID);
	void DoDiscard();
	void DoApply();
	FS_SHIELDS m_OwnShieldsBackup;
	void UpdateEditorDisplay();
	BOOL FinishPendingEditing();
	BOOL ReadShieldData(CString fn,FS_SHIELDS *sh);
	BOOL UnInit();
	void Init();
	BOOL m_AmEditing;
	BOOL m_DataLocked;
	CToolBar m_TBApplyer;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorFS_SHLD)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEditorFS_SHLD();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEditorFS_SHLD)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditorfsShldModifyApply();
	afx_msg void OnEditorfsShldRestoredata();
	afx_msg void OnEditorfsShldRemovedata();
	afx_msg void OnEditorfsShldImport();
	afx_msg void OnChangeData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORFS_SHLD_H__9E7C2ED7_A335_4DF8_91C6_E48628F9705A__INCLUDED_)
