#if !defined(AFX_EDITORFS_TGUN_H__9AF76CE7_6369_4677_B568_A3778B32EE8B__INCLUDED_)
#define AFX_EDITORFS_TGUN_H__9AF76CE7_6369_4677_B568_A3778B32EE8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorFS_TGUN.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_TGUN form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "StaticWithColor.h"

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CEditorFS_TGUN : public CFormView
{
protected:
	CToolBar m_TBElements,m_TBApplyer,m_TBFiringPoints;
	CEditorFS_TGUN();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditorFS_TGUN)
	void AdaptEditorBox(int cx,CWnd *ed);
	void AdaptVectorBox(int cx,CEdit *ed1,CEdit *ed2,CEdit *ed3);

	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();

	// Form Data
public:
	//{{AFX_DATA(CEditorFS_TGUN)
	enum { IDD = IDD_EDITORFS_TGUN };
	CButton	m_Convert;
	CComboBox	m_SObj_Parent;
	CComboBox	m_SObj_Parent_Phys;
	CEdit	m_Normal_Z;
	CEdit	m_Normal_Y;
	CEdit	m_Normal_X;
	CEdit	m_FiringPoint_Y;
	CEdit	m_FiringPoint_Z;
	CEdit	m_FiringPoint_X;
	CListCtrl	m_FiringPoints_List;
	CTabCtrl	m_GunMode;
	CListBox	m_List;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	BOOL FinishPendingEditing();
	void DoApply();
	void DoDiscard();
	void UpdateFiringPointsList();
	void DoCopyFiringPoint();
	void DoDeleteFiringPoint();
	void DoNewFiringPoint();
	void DoCopyItem();
	void DoDeleteItem();
	void DoNewItem();
	int GetCurrentFiringPointSelection();
	afx_msg void OnApplyerBarClick(UINT nID);
	afx_msg void OnEditorBarClick(UINT nID);
	afx_msg void OnEditor2BarClick(UINT nID);
	void UpdateEditorDisplay(BOOL editing);
	void OnSelchangeTgunFiringPointsList();
	BOOL m_DataLocked;
	BOOL m_DataChanged;
	BOOL m_IsNewItem;
	void InitGunMode();
	void Init();
	BOOL UnInit();
	BOOL m_AmEditing;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorFS_TGUN)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEditorFS_TGUN();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEditorFS_TGUN)
	afx_msg void OnSelchangeTgunmode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeEditorfsTgunList();
	afx_msg void OnItemchangedEditorfsTgunFiringPointsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeData();
	afx_msg void OnConvert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORFS_TGUN_H__9AF76CE7_6369_4677_B568_A3778B32EE8B__INCLUDED_)
