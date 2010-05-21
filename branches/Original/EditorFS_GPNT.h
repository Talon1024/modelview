#if !defined(AFX_EDITORFS_GPNT_H__A1749411_57C0_4226_A837_3B99E6C5031A__INCLUDED_)
#define AFX_EDITORFS_GPNT_H__A1749411_57C0_4226_A837_3B99E6C5031A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorFS_GPNT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_GPNT form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CEditorFS_GPNT : public CFormView
{
protected:
	CToolBar m_TBElements,m_TBApplyer,m_TBFiringPoints;
	CEditorFS_GPNT();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditorFS_GPNT)
	void AdaptEditorBox(int cx,CWnd *ed);
	void AdaptVectorBox(int cx,CEdit *ed1,CEdit *ed2,CEdit *ed3);

	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();

	// Form Data
public:
	//{{AFX_DATA(CEditorFS_GPNT)
	enum { IDD = IDD_EDITORFS_GPNT };
	CTabCtrl	m_GunMode;
	CListBox	m_List;
	CListCtrl	m_FiringPoints_List;
	CEdit		m_FiringPoint_PosX;
	CEdit		m_FiringPoint_PosY;
	CEdit		m_FiringPoint_PosZ;
	CEdit		m_FiringPoint_NormX;
	CEdit		m_FiringPoint_NormY;
	CEdit		m_FiringPoint_NormZ;
	CStatic		m_Convert;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	BOOL FinishPendingEditing();
	BOOL m_AmEditing;
	void DoApply();
	void DoDiscard();
	void DoCopyFiringPoint();
	void DoDeleteFiringPoint();
	void DoNewFiringPoint();
	void DoCopyItem();
	void DoDeleteItem();
	void DoNewItem();
	void UpdateFiringPointsList();
	void InitGunMode();
	void Init();
	BOOL UnInit();
	BOOL m_DataLocked;
	BOOL m_DataChanged;
	BOOL m_IsNewItem;
	int GetCurrentFiringPointSelection();
	void OnSelchangeGpntFiringPointsList();
	void UpdateEditorDisplay(BOOL editing);
	afx_msg void OnApplyerBarClick(UINT nID);
	afx_msg void OnEditorBarClick(UINT nID);
	afx_msg void OnEditor2BarClick(UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorFS_GPNT)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEditorFS_GPNT();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEditorFS_GPNT)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeGpntmode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeEditorfsGpntList();
	afx_msg void OnItemchangedEditorfsGpntFiringPointsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeData();
	afx_msg void OnConvert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORFS_GPNT_H__A1749411_57C0_4226_A837_3B99E6C5031A__INCLUDED_)
