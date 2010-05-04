#if !defined(AFX_EDITORFSDLG_TEXTURES_H__1BA12443_D964_4749_954F_5C54B19253DB__INCLUDED_)
#define AFX_EDITORFSDLG_TEXTURES_H__1BA12443_D964_4749_954F_5C54B19253DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorFSDlg_Textures.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorFSDlg_Textures dialog

class CEditorFSDlg_Textures : public CDialog
{
// Construction
public:
	CMODVIEW32View * GetMainView();
	CMODVIEW32Doc * GetDocument();
	CEditorFSDlg_Textures(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditorFSDlg_Textures)
	enum { IDD = IDD_EDITORFSDLG_TEXTURENAMES };
	CListCtrl m_TextureNames_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorFSDlg_Textures)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditorFSDlg_Textures)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITORFSDLG_TEXTURES_H__1BA12443_D964_4749_954F_5C54B19253DB__INCLUDED_)
