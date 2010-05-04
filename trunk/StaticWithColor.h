#if !defined(AFX_STATICWITHCOLOR_H__4CD57B36_452B_4B15_8BF1_6632758B51F7__INCLUDED_)
#define AFX_STATICWITHCOLOR_H__4CD57B36_452B_4B15_8BF1_6632758B51F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticWithColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticWithColor window

class CStaticWithColor : public CStatic
{
// Construction
public:
	CStaticWithColor();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticWithColor)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_HeaderMode;
	void SetHeaderMode();
	BOOL m_Centered;
	CFont * m_Font;
	virtual ~CStaticWithColor();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticWithColor)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICWITHCOLOR_H__4CD57B36_452B_4B15_8BF1_6632758B51F7__INCLUDED_)
