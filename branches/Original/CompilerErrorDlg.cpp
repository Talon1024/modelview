// CompilerErrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "CompilerErrorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompilerErrorDlg dialog


CCompilerErrorDlg::CCompilerErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompilerErrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompilerErrorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCompilerErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompilerErrorDlg)
	DDX_Control(pDX, IDC_ERRORLIST, m_ErrorList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCompilerErrorDlg, CDialog)
	//{{AFX_MSG_MAP(CCompilerErrorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompilerErrorDlg message handlers

BOOL CCompilerErrorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ErrorList.SetWindowText(errstring);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
