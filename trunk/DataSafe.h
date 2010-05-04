#if !defined(AFX_DATASAFE_H__8CFA7CC1_B0E2_11D3_9455_00105A9D6FFE__INCLUDED_)
#define AFX_DATASAFE_H__8CFA7CC1_B0E2_11D3_9455_00105A9D6FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSafe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataSafe window

class CDataSafe : public CWnd
{
// Construction
public:
	CDataSafe();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSafe)
	//}}AFX_VIRTUAL

// Implementation
public:
	static int m_LastHeight;
	static int m_LastWidth;
	virtual ~CDataSafe();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDataSafe)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASAFE_H__8CFA7CC1_B0E2_11D3_9455_00105A9D6FFE__INCLUDED_)
