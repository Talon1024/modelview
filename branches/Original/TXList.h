#if !defined(AFX_TXLIST_H__1EA0DA95_2E1A_4387_BD97_A81E7CC3A711__INCLUDED_)
#define AFX_TXLIST_H__1EA0DA95_2E1A_4387_BD97_A81E7CC3A711__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TXList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTXList view

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CTXList : public CListView
{
protected:
	CTXList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTXList)

// Attributes
public:

// Operations
public:
	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();
	void FillTextureInfo(int mode);
	void DeleteAllColumns();

private:
	int current_mode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTXList)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTXList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTXList)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TXLIST_H__1EA0DA95_2E1A_4387_BD97_A81E7CC3A711__INCLUDED_)
