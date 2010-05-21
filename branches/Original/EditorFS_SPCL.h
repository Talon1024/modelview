#if !defined(AFX_EDITORFS_SPCL_H__7BD1C03C_CA3E_4390_84F7_0A54BAA0B1B8__INCLUDED_)
#define AFX_EDITORFS_SPCL_H__7BD1C03C_CA3E_4390_84F7_0A54BAA0B1B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorFS_SPCL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SPCL form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "StaticWithColor.h"

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CEditorFS_SPCL : public CFormView
{
protected:
	CToolBar m_TBElements,m_TBApplyer;
	CEditorFS_SPCL();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditorFS_SPCL)
	void AdaptEditorBox(int cx,CWnd *ed);
	void AdaptVectorBox(int cx,CEdit *ed1,CEdit *ed2,CEdit *ed3);

	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();

//Editor data
	BOOL m_DataChanged;
	BOOL m_DataLocked;

// Form Data
public:
	//{{AFX_DATA(CEditorFS_SPCL)
	enum { IDD = IDD_FREESPACE_EDITOR_SUBSYSTEMS };
	CListBox	m_List;
	CComboBox	m_Properties;
	CStatic	m_Header;
	CEdit	m_CenterZ;
	CEdit	m_CenterY;
	CEdit	m_CenterX;
	CEdit	m_Radius;
	CEdit	m_Name;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	BOOL FinishPendingEditing();
	void DoCopyItem();
	void DoNewItem();
	BOOL m_IsNewItem;
	void DoDeleteItem();
	void UpdateEditorDisplay(BOOL editing);
	void DoDiscard();
	BOOL DoApply();
	void Init();
	BOOL UnInit();
	BOOL m_AmEditing;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorFS_SPCL)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEditorFS_SPCL();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEditorFS_SPCL)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeEditorfsSubsystemsList();
	afx_msg void OnChangeData();
	afx_msg void OnApplyerBarClick(UINT nID);
	afx_msg void OnEditorBarClick(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORFS_SPCL_H__7BD1C03C_CA3E_4390_84F7_0A54BAA0B1B8__INCLUDED_)
