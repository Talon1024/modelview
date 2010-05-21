#if !defined(AFX_EDITORFSDLG_MOMENTOFINERTIA_H__3FA84C08_D977_4BBF_8753_4FA5B53B21E6__INCLUDED_)
#define AFX_EDITORFSDLG_MOMENTOFINERTIA_H__3FA84C08_D977_4BBF_8753_4FA5B53B21E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorFSDlg_MomentOfInertia.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorFSDlg_MomentOfInertia dialog

class CEditorFSDlg_MomentOfInertia : public CDialog
{
// Construction
public:
	CEditorFSDlg_MomentOfInertia(CWnd* pParent = NULL);   // standard constructor
	float moment_inertia[3][3];

// Dialog Data
	//{{AFX_DATA(CEditorFSDlg_MomentOfInertia)
	enum { IDD = IDD_EDITORFSDLG_MOMENTOFINERTIA };
	CEdit	m_MomInertia_22;
	CEdit	m_MomInertia_21;
	CEdit	m_MomInertia_20;
	CEdit	m_MomInertia_12;
	CEdit	m_MomInertia_11;
	CEdit	m_MomInertia_10;
	CEdit	m_MomInertia_02;
	CEdit	m_MomInertia_01;
	CEdit	m_MomInertia_00;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorFSDlg_MomentOfInertia)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditorFSDlg_MomentOfInertia)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORFSDLG_MOMENTOFINERTIA_H__3FA84C08_D977_4BBF_8753_4FA5B53B21E6__INCLUDED_)
