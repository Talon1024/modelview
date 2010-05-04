#if !defined(AFX_EDITORFS_FUEL_H__1BF2B714_02F9_49D2_BDE4_68950FCAC045__INCLUDED_)
#define AFX_EDITORFS_FUEL_H__1BF2B714_02F9_49D2_BDE4_68950FCAC045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorFS_FUEL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_FUEL form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CEditorFS_FUEL : public CFormView
{
protected:
	CToolBar m_TBElements,m_TBApplyer,m_TBGlowPoints;
	CEditorFS_FUEL();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditorFS_FUEL)
	void AdaptEditorBox(int cx,CWnd *ed);
	void AdaptVectorBox(int cx,CEdit *ed1,CEdit *ed2,CEdit *ed3);

	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();

// Form Data
public:
	//{{AFX_DATA(CEditorFS_FUEL)
	enum { IDD = IDD_EDITORFS_FUEL };
	CListBox	m_List;
	CEdit	m_PositionZ;
	CEdit	m_PositionY;
	CEdit	m_PositionX;
	CEdit	m_NormalZ;
	CEdit	m_NormalY;
	CEdit	m_NormalX;
	CEdit	m_Radius;
	CEdit	m_Properties;
	CListCtrl	m_GlowList;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void DoDiscard();
	BOOL DoApply();
	void UpdateEditorDisplay(BOOL editing);
	void UpdateGlowPointsList();
	int GetCurrentGlowListSelection();
	void OnSelchangeEditorfsGlowList();
	BOOL m_DataLocked;
	BOOL m_DataChanged;
	BOOL m_IsNewItem;
	void Init();
	BOOL UnInit();
	BOOL FinishPendingEditing();
	BOOL m_AmEditing;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorFS_FUEL)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEditorFS_FUEL();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEditorFS_FUEL)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeEditorfsFuelList();
	afx_msg void OnItemchangedEditorfsFuelGlowlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeData();
	afx_msg void OnApplyerBarClick(UINT nID);
	afx_msg void OnEditorBarClick(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORFS_FUEL_H__1BF2B714_02F9_49D2_BDE4_68950FCAC045__INCLUDED_)
