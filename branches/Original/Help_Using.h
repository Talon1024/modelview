#if !defined(AFX_HELP_USING_H__B34675C3_AAFF_49A7_8E65_AADD402DFEF3__INCLUDED_)
#define AFX_HELP_USING_H__B34675C3_AAFF_49A7_8E65_AADD402DFEF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Help_Using.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHelp_Using dialog

class CHelp_Using : public CDialog
{
// Construction
public:
	CHelp_Using(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHelp_Using)
	enum { IDD = IDD_HELP_USING };
	CStatic	m_HelpUsing_Keyboard;
	CStatic	m_HelpUsing_Mouse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelp_Using)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHelp_Using)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELP_USING_H__B34675C3_AAFF_49A7_8E65_AADD402DFEF3__INCLUDED_)
