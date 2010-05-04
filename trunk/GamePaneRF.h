#if !defined(AFX_GAMEPANERF_H__F6E49BB4_7D81_4D15_9168_7D474768D86C__INCLUDED_)
#define AFX_GAMEPANERF_H__F6E49BB4_7D81_4D15_9168_7D474768D86C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GamePaneRF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGamePaneRF form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CGamePaneRF : public CFormView
{
protected:
	CGamePaneRF();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGamePaneRF)

// Form Data
public:
	//{{AFX_DATA(CGamePaneRF)
	enum { IDD = IDD_REDFACTION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();
	void Init();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGamePaneRF)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGamePaneRF();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGamePaneRF)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEPANERF_H__F6E49BB4_7D81_4D15_9168_7D474768D86C__INCLUDED_)
