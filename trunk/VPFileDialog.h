#if !defined(AFX_VPFILEDIALOG_H__DBF76955_79E8_4A9B_A487_39DBAE5514E2__INCLUDED_)
#define AFX_VPFILEDIALOG_H__DBF76955_79E8_4A9B_A487_39DBAE5514E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VPFileDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVPFileDialog dialog

#include "VPFileDialogSubWnd.h"

class CVPFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CVPFileDialog)

public:
	CPoint GetPositionForMenu();
	void SetPath(CString path);
	int m_nSubType;
	//CStatic m_Static;
	//CComboBox m_Combo;
	CButton m_GameButton;
	CVPFileDialogSubWnd m_DlgWnd;

	CVPFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	//{{AFX_MSG(CVPFileDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnVpfiledialogJumptoredfaction1fullversiondirectory();
	virtual void OnFolderChange( );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VPFILEDIALOG_H__DBF76955_79E8_4A9B_A487_39DBAE5514E2__INCLUDED_)
