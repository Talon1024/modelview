#if !defined(AFX_EDITORFS_MODEL_H__D1D98711_30DD_4EDF_B681_093800E6B460__INCLUDED_)
#define AFX_EDITORFS_MODEL_H__D1D98711_30DD_4EDF_B681_093800E6B460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorFS_MODEL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_MODEL form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CEditorFS_MODEL : public CFormView
{
protected:
	CEditorFS_MODEL();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditorFS_MODEL)

	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();
	
	CToolBar m_TBCrossSections,m_TBLights,m_TBApplyer;

// Form Data
public:
	//{{AFX_DATA(CEditorFS_MODEL)
	enum { IDD = IDD_EDITORFS_MODEL };
	CEdit	m_Lights_Type;
	CEdit	m_Lights_Position_Z;
	CEdit	m_Lights_Position_Y;
	CEdit	m_Lights_Position_X;
	CListCtrl	m_List_Lights;
	CEdit	m_MassCenter_Z;
	CEdit	m_MassCenter_Y;
	CEdit	m_MassCenter_X;
	CEdit	m_Mass;
	CEdit	m_ACenPoint_Z;
	CEdit	m_ACenPoint_Y;
	CEdit	m_ACenPoint_X;
	CEdit	m_Eye_Position_Z;
	CEdit	m_Eye_Position_Y;
	CEdit	m_Eye_Position_X;
	CEdit	m_Eye_Normal_Z;
	CEdit	m_Eye_Normal_Y;
	CEdit	m_Eye_Normal_X;
	CComboBox	m_Eye_Submodel;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void ChangeFSMode();
	BOOL FinishPendingEditing();
	void DoDiscard();
	void DoApply();
	void DoCopyLight();
	void DoDeleteLight();
	void DoNewLight();
	void UpdateEditorDisplay();
	void FillInfo();
	afx_msg void OnApplyerBarClick(UINT nID);
	afx_msg void OnEditor2BarClick(UINT nID);
	afx_msg void OnEditorBarClick(UINT nID);
	int GetCurrentLightSelection();
	void OnSelchangeLights();
	BOOL UnInit();
	void Init();
	BOOL m_DataLocked;
	BOOL m_AmEditing;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorFS_MODEL)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEditorFS_MODEL();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEditorFS_MODEL)
	afx_msg void OnSelchangeEditorfsModelEyepointSubmodel();
	afx_msg void OnChangeData();
	afx_msg void OnItemchangedEditorfsModelLights(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonMomentOfInertia();
	afx_msg void OnButtonTextureNames();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORFS_MODEL_H__D1D98711_30DD_4EDF_B681_093800E6B460__INCLUDED_)


/*
	CListCtrl	m_List_CrossSections;
	CEdit	m_CrossSections_Depth;
	CEdit	m_CrossSections_Radius;
*/
