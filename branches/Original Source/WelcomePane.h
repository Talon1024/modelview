#if !defined(AFX_WELCOMEPANE_H__46DE7E27_AAAF_4B87_82E8_DA62FEFE0B73__INCLUDED_)
#define AFX_WELCOMEPANE_H__46DE7E27_AAAF_4B87_82E8_DA62FEFE0B73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WelcomePane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWelcomePane form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "staticwithcolor.h"

class CWelcomePane : public CFormView
{
protected:
	CWelcomePane();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWelcomePane)

// Form Data
public:
	//{{AFX_DATA(CWelcomePane)
	enum { IDD = IDD_WELCOMEPANE };
	CStaticWithColor	m_WelcomeText;
	CStaticWithColor	m_NextTipButton;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void ShowNextTip();

private:
	int _OnSize_cy;
	int _OnSize_cx;
	UINT _OnSize_nType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWelcomePane)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWelcomePane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWelcomePane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnNexttip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WELCOMEPANE_H__46DE7E27_AAAF_4B87_82E8_DA62FEFE0B73__INCLUDED_)
