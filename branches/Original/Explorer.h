#if !defined(AFX_EXPLORER_H__E18AE9B5_A993_11D3_BA1B_0080ADC9BC05__INCLUDED_)
#define AFX_EXPLORER_H__E18AE9B5_A993_11D3_BA1B_0080ADC9BC05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Explorer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExplorer view


#define MAX_TYPEROOTS	256

class CExplorer : public CTreeView
{
protected:
	CExplorer();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CExplorer)

// Attributes
public:
	HTREEITEM typeroot[MAX_TYPEROOTS];
	int typeroot_num[MAX_TYPEROOTS];
	CString typeroot_str[MAX_TYPEROOTS];

// Operations
public:
	CString m_RootName;
	int m_NumModelsListed;
	HTREEITEM GetNextItem(HTREEITEM hItem);
	BOOL DoEventhandling;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExplorer)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CExplorer();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CExplorer)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLORER_H__E18AE9B5_A993_11D3_BA1B_0080ADC9BC05__INCLUDED_)
