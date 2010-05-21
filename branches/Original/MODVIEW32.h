// MODVIEW32.h : main header file for the MODVIEW32 application
//

#if !defined(AFX_MODVIEW32_H__1ECD6139_A743_11D3_9455_00105A9D6FFE__INCLUDED_)
#define AFX_MODVIEW32_H__1ECD6139_A743_11D3_9455_00105A9D6FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "progresswnd.h"

/////////////////////////////////////////////////////////////////////////////
// CMODVIEW32App:
// See MODVIEW32.cpp for the implementation of this class
//

class CMODVIEW32App : public CWinApp
{
public:
	CButton *m_GameButton;
	RECT m_GameButtonRect;

	CString m_LogFile;
	void ReloadFile(CString fname);
	CMODVIEW32App();
	BOOL showsplashlonger;
	BOOL splashisdisplayed;
	void RedrawProgressWnd();
	void StatusProgressWnd(char *x);
	BOOL Check1stStartup();
	void CloseProgressWnd();
	void UpdateProgressWnd(int,int);
	void DisplayProgressWnd(char *);
	CProgressWnd progWnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMODVIEW32App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMODVIEW32App)
	afx_msg void OnAppAbout();
	afx_msg void OnUpdateAppAbout(CCmdUI* pCmdUI);
	afx_msg void OnHelpCheckfornewversion();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODVIEW32_H__1ECD6139_A743_11D3_9455_00105A9D6FFE__INCLUDED_)
