#if !defined(AFX_DETAILS_H__E551B76A_4D06_422E_A783_4BA86845C1F0__INCLUDED_)
#define AFX_DETAILS_H__E551B76A_4D06_422E_A783_4BA86845C1F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Details.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetails dialog

#include "TextureView.h"

class CDetails : public CDialog
{
// Construction
public:
	void DrawBitmap(CDC *pDC, CBitmap &bitmap, CPalette *pPal);
	unsigned char *m_TextureData;
	int m_TextureResX;
	int m_TextureResY;
	int m_TextureResL;
	unsigned char m_TexturePale[768];
	void GetTexture();
	void ShowTextureView(BOOL show);
	void FillBlockOrder();
	void DeleteAllColumns();
	void FillTextureInformation();
	CDetails();   // standard constructor
	~CDetails();
	CTextureView m_TexView;
	BOOL texviewvisible;

// Dialog Data
	//{{AFX_DATA(CDetails)
	enum { IDD = IDD_DETAILS };
	CStatic	m_TextureView;
	CButton	m_TexViewState;
	CTabCtrl	m_Tabs;
	CListCtrl	m_ListView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetails)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetails)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTextureview();
	afx_msg void OnPaint();
	afx_msg void OnClickListview(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETAILS_H__E551B76A_4D06_422E_A783_4BA86845C1F0__INCLUDED_)
