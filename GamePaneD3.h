#if !defined(AFX_GAMEPANED3_H__E2001EB4_6F38_499E_B022_C9058F96F037__INCLUDED_)
#define AFX_GAMEPANED3_H__E2001EB4_6F38_499E_B022_C9058F96F037__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GamePaneD3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGamePaneD3 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

#define MAX_N_TEXS	256

class CGamePaneD3 : public CFormView
{
protected:
	CGamePaneD3();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGamePaneD3)

// Form Data
public:
	int m_sel2id[MAX_N_TEXS];
	//{{AFX_DATA(CGamePaneD3)
	enum { IDD = IDD_GAMEPANE_D3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void OnPositionChange();
	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();
	void FillTab();
	void Init();

private:
	int _OnSize_cy;
	int _OnSize_cx;
	UINT _OnSize_nType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGamePaneD3)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGamePaneD3();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGamePaneD3)
	afx_msg void OnSelchangeD3Display(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeD3Positions();
	afx_msg void OnSelchangeD3Texturelist();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEPANED3_H__E2001EB4_6F38_499E_B022_C9058F96F037__INCLUDED_)
