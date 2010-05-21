#if !defined(AFX_COMPILERERRORDLG_H__A7D62BCE_1CF6_4BF4_A635_64BAE121F22F__INCLUDED_)
#define AFX_COMPILERERRORDLG_H__A7D62BCE_1CF6_4BF4_A635_64BAE121F22F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CompilerErrorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCompilerErrorDlg dialog

class CCompilerErrorDlg : public CDialog
{
// Construction
public:
	CString errstring;
	CCompilerErrorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCompilerErrorDlg)
	enum { IDD = IDD_COMPILERERRORDLG };
	CEdit	m_ErrorList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompilerErrorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCompilerErrorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPILERERRORDLG_H__A7D62BCE_1CF6_4BF4_A635_64BAE121F22F__INCLUDED_)
