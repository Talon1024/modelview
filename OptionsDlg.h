#if !defined(AFX_OPTIONSDLG_H__D2B57517_A9EF_11D3_BA1B_0080ADC9BC05__INCLUDED_)
#define AFX_OPTIONSDLG_H__D2B57517_A9EF_11D3_BA1B_0080ADC9BC05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
// Construction
public:
	BOOL m_GameConfigChanged;
	CString GetINIfile();
	void LoadOptions(char *options);
	CImageList *pImageList;
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor
	~COptionsDlg();
	int RemoveFilename(char *path);
	char * GetMainPath(char *path);
	CString GetF2Path();
	CString GetF1Path();

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONS };
	CButton	c_MenuStyle;
	CButton	m_QuickRendering;
	//CButton	m_D2_SmoothPositionChange;
	CButton	m_ExplorerIndex;
	CButton	m_SwapMouseButtons;
	CComboBox	m_ToolBar_Text;
	CButton	m_ToolBar_Size;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnConfigure();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMenuStyle();
//	afx_msg void OnBnClickedQuickrendering();
	afx_msg void OnBnClickedQuickrendering();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__D2B57517_A9EF_11D3_BA1B_0080ADC9BC05__INCLUDED_)
