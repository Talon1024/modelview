#if !defined(AFX_VPFILEDIALOGSUBWND_H__AA59215E_ACEF_42CF_8BFA_E61EB65D4A6C__INCLUDED_)
#define AFX_VPFILEDIALOGSUBWND_H__AA59215E_ACEF_42CF_8BFA_E61EB65D4A6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VPFileDialogSubWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVPFileDialogSubWnd window

#include "BCMenu.h"

class CVPFileDialogSubWnd : public CWnd
{
// Construction
public:
	CVPFileDialogSubWnd();

// Attributes
public:

// Operations
public:

protected:
	BCMenu m_PopupMenu;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVPFileDialogSubWnd)
	virtual void OnGameButton();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVPFileDialogSubWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVPFileDialogSubWnd)
	afx_msg void OnVpfiledialogJumptoredfaction1fullversiondirectory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VPFILEDIALOGSUBWND_H__AA59215E_ACEF_42CF_8BFA_E61EB65D4A6C__INCLUDED_)
