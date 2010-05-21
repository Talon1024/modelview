#if !defined(AFX_WELCOME_H__86C294D0_A3DF_467E_91FF_1050247CBCE2__INCLUDED_)
#define AFX_WELCOME_H__86C294D0_A3DF_467E_91FF_1050247CBCE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Welcome.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWelcome dialog

class CWelcome : public CDialog
{
// Construction
public:
	int m_SelectedGame;
	CImageList *pImageList;
	CWelcome(CWnd* pParent = NULL);   // standard constructor
	~CWelcome();

// Dialog Data
	//{{AFX_DATA(CWelcome)
	enum { IDD = IDD_WELCOME };
	CListCtrl	m_Games;
	CCheckListBox	m_FileTypes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWelcome)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWelcome)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WELCOME_H__86C294D0_A3DF_467E_91FF_1050247CBCE2__INCLUDED_)
