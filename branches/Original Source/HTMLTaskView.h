#if !defined(AFX_HTMLTASKVIEW_H__C3134331_8C5E_446F_9EAD_719D096168FD__INCLUDED_)
#define AFX_HTMLTASKVIEW_H__C3134331_8C5E_446F_9EAD_719D096168FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HTMLTaskView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHTMLTaskView html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CHTMLTaskView : public CHtmlView
{
protected:
	CHTMLTaskView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHTMLTaskView)

// html Data
public:
	//{{AFX_DATA(CHTMLTaskView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

private:
	CString m_HTMLFile_Open;
	CString m_HTMLFile_Archive;
	CString m_HTMLFile_Error;
	CString m_HTMLFile_NoTextures;

// Operations
public:
	void ShowHTML(int type);
	void GenerateHTMLHeader(CStdioFile *ft);
	void GenerateHTMLFooter(CStdioFile *ft);
	void GenerateHTML();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTMLTaskView)
	public:
	virtual void OnInitialUpdate();
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHTMLTaskView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHTMLTaskView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLTASKVIEW_H__C3134331_8C5E_446F_9EAD_719D096168FD__INCLUDED_)
