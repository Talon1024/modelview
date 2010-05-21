#if !defined(AFX_SPLASHNEW_H__DACE8E97_8E46_4176_B45C_FC9A1D889649__INCLUDED_)
#define AFX_SPLASHNEW_H__DACE8E97_8E46_4176_B45C_FC9A1D889649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashNew.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplashNew dialog

class CSplashNew : public CDialog
{
// Construction
public:
	BOOL Create(CWnd *pParent);
	CSplashNew(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSplashNew)
	enum { IDD = IDD_SPLASH_NEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSplashNew)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHNEW_H__DACE8E97_8E46_4176_B45C_FC9A1D889649__INCLUDED_)
