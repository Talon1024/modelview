#if !defined(AFX_EDITORFSDLG_TEXTURENAMESEDIT_H__6087DBEB_9CCD_41BE_84CB_4C6824461DA8__INCLUDED_)
#define AFX_EDITORFSDLG_TEXTURENAMESEDIT_H__6087DBEB_9CCD_41BE_84CB_4C6824461DA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorFSDlg_TextureNamesEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorFSDlg_TextureNamesEdit dialog

class CEditorFSDlg_TextureNamesEdit : public CDialog
{
// Construction
public:
	CString texname;
	CEditorFSDlg_TextureNamesEdit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditorFSDlg_TextureNamesEdit)
	enum { IDD = IDD_EDITORFSDLG_TEXTURENAMES_EDIT };
	CEdit	m_TextureName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorFSDlg_TextureNamesEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditorFSDlg_TextureNamesEdit)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORFSDLG_TEXTURENAMESEDIT_H__6087DBEB_9CCD_41BE_84CB_4C6824461DA8__INCLUDED_)
