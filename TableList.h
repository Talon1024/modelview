#if !defined(AFX_TABLELIST_H__F9D3001F_FC58_4ED7_93E1_7BD454E45171__INCLUDED_)
#define AFX_TABLELIST_H__F9D3001F_FC58_4ED7_93E1_7BD454E45171__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TableList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTableList dialog

class CTableList : public CDialog
{
// Construction
public:
	int CALLBACK MySort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	typedef struct modelentrytag
	{
		int filesize;
		int filevers;
		int vertices;
		int polygons;
		int textures;
		int submodels;
		int guns;
		int positions;
		CString name;
		CString desc;
	} modelentry;

	CListCtrl * GetListControl();
	CTableList();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTableList)
	enum { IDD = IDD_TABLE };
	CListCtrl	m_ListView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTableList)
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLELIST_H__F9D3001F_FC58_4ED7_93E1_7BD454E45171__INCLUDED_)
