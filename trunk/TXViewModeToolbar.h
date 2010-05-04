#if !defined(AFX_TXVIEWMODETOOLBAR_H__D13BAB9C_FF98_4840_8456_13D3919F1407__INCLUDED_)
#define AFX_TXVIEWMODETOOLBAR_H__D13BAB9C_FF98_4840_8456_13D3919F1407__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TXViewModeToolbar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTXViewModeToolbar form view

#include "resource.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTXViewModeToolbar : public CFormView
{
protected:
	CTXViewModeToolbar();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTXViewModeToolbar)

// Form Data
public:
	//{{AFX_DATA(CTXViewModeToolbar)
	enum { IDD = IDD_TXVIEWMODE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void ShowTextures();
	void ShowBlockList();
	void ShowLogFile();
	int GetCurrent();
	void Init();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTXViewModeToolbar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTXViewModeToolbar();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTXViewModeToolbar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TXVIEWMODETOOLBAR_H__D13BAB9C_FF98_4840_8456_13D3919F1407__INCLUDED_)
