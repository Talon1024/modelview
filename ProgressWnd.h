#if !defined(AFX_PROGRESSWND_H__018FAC39_2505_48B1_954E_7ECFDA804744__INCLUDED_)
#define AFX_PROGRESSWND_H__018FAC39_2505_48B1_954E_7ECFDA804744__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressWnd dialog

class CProgressWnd : public CDialog
{
// Construction
public:
	Create(CWnd* pParent);
	CProgressWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressWnd)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_Progress1;
	CStatic	m_Frame;
	CStatic	m_Action;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressWnd)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSWND_H__018FAC39_2505_48B1_954E_7ECFDA804744__INCLUDED_)
