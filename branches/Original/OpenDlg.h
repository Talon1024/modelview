#if !defined(AFX_OPENDLG_H__4A33F9F5_ABDF_11D3_BA1C_0080ADC9BC05__INCLUDED_)
#define AFX_OPENDLG_H__4A33F9F5_ABDF_11D3_BA1C_0080ADC9BC05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenDlg dialog

#define MAX_FILES	8192

class COpenDlg : public CDialog
{
// Construction
public:
	CString m_OpenFileName;
	BOOL FileExists(CString filename);
	BOOL InsertItem(int game,CString desc, CString dir, CString filename, int icon);
	BOOL InsertItem(int game,CString desc,CString filename,int icon);
	COpenDlg(CWnd* pParent = NULL);   // standard constructor
	~COpenDlg();
	HTREEITEM gameroot[256];
	CImageList *pImageList;

	int num_files;
	HTREEITEM file_treeitem[MAX_FILES];
	CString file_filename[MAX_FILES];

// Dialog Data
	//{{AFX_DATA(COpenDlg)
	enum { IDD = IDD_OPEN };
	CButton	m_OK;
	CEdit	m_FileName;
	CTreeCtrl	m_FileTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedFiletree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditFilename();
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnDblclkFiletree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENDLG_H__4A33F9F5_ABDF_11D3_BA1C_0080ADC9BC05__INCLUDED_)
