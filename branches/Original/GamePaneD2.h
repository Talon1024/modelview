#if !defined(AFX_GAMEPANED2_H__6D218452_8723_4129_B46A_5241DA2C26DB__INCLUDED_)
#define AFX_GAMEPANED2_H__6D218452_8723_4129_B46A_5241DA2C26DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GamePaneD2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGamePaneD2 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CGamePaneD2 : public CFormView
{
protected:
	CGamePaneD2();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGamePaneD2)

// Form Data
public:
	//{{AFX_DATA(CGamePaneD2)
	enum { IDD = IDD_GAMEPANE_D2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
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
	//{{AFX_VIRTUAL(CGamePaneD2)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGamePaneD2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGamePaneD2)
	afx_msg void OnSelchangeD2Display(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeD2Positions();
	afx_msg void OnSelchangeD2Texturelist();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEPANED2_H__6D218452_8723_4129_B46A_5241DA2C26DB__INCLUDED_)
