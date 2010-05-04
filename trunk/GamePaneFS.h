#if !defined(AFX_GAMEPANEFS_H__113FD49D_F9D9_48F4_9DC0_421581D36D9E__INCLUDED_)
#define AFX_GAMEPANEFS_H__113FD49D_F9D9_48F4_9DC0_421581D36D9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GamePaneFS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGamePaneFS form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "mainfrm.h"

class CGamePaneFS : public CFormView
{
protected:
	CGamePaneFS();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGamePaneFS)

// Form Data
public:
	//{{AFX_DATA(CGamePaneFS)
	enum { IDD = IDD_GAMEPANE_FS };
	CListCtrl	m_MyDetLvls;
	CListBox	m_MyListBox;
	CTabCtrl	m_MyTab;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void UpdateIt(BOOL ShowShield, BOOL ShowThruster, BOOL UpdateDisplayList=FALSE);
	void GetPolyInfo(int detaillevel, int *vert, int *poly, int *txtr, int *sobj);
	CMODVIEW32View * GetMainView();
	CMainFrame * GetMainFrame();
	CMODVIEW32Doc * GetDocument();
	void FillTab();
	void Init();

private:
	int _OnSize_cy;
	int _OnSize_cx;
	UINT _OnSize_nType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGamePaneFS)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGamePaneFS();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGamePaneFS)
	afx_msg void OnClickDetaillevel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowshield();
	afx_msg void OnSelchangeFsTexturelist();
	afx_msg void OnSelchangeFsDisplay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEPANEFS_H__113FD49D_F9D9_48F4_9DC0_421581D36D9E__INCLUDED_)
