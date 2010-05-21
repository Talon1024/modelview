#if !defined(AFX_TXVIEW_H__CD94F934_331D_46FA_9BA5_8C430F86CFAC__INCLUDED_)
#define AFX_TXVIEW_H__CD94F934_331D_46FA_9BA5_8C430F86CFAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TXView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTXView view

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CTXView : public CScrollView
{
protected:
	CTXView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTXView)

// Attributes
public:

// Operations
public:
	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTXView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTXView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTXView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TXVIEW_H__CD94F934_331D_46FA_9BA5_8C430F86CFAC__INCLUDED_)
