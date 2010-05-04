// EditorFSDlg_MomentOfInertia.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "EditorFSDlg_MomentOfInertia.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorFSDlg_MomentOfInertia dialog


CEditorFSDlg_MomentOfInertia::CEditorFSDlg_MomentOfInertia(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorFSDlg_MomentOfInertia::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditorFSDlg_MomentOfInertia)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEditorFSDlg_MomentOfInertia::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorFSDlg_MomentOfInertia)
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MOMOFINERTIA_22, m_MomInertia_22);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MOMOFINERTIA_21, m_MomInertia_21);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MOMOFINERTIA_20, m_MomInertia_20);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MOMOFINERTIA_12, m_MomInertia_12);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MOMOFINERTIA_11, m_MomInertia_11);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MOMOFINERTIA_10, m_MomInertia_10);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MOMOFINERTIA_02, m_MomInertia_02);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MOMOFINERTIA_01, m_MomInertia_01);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MOMOFINERTIA_00, m_MomInertia_00);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorFSDlg_MomentOfInertia, CDialog)
	//{{AFX_MSG_MAP(CEditorFSDlg_MomentOfInertia)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorFSDlg_MomentOfInertia message handlers

BOOL CEditorFSDlg_MomentOfInertia::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString t;
	t.Format("%.20f",moment_inertia[0][0]);
	m_MomInertia_00.SetWindowText(t);
	t.Format("%.20f",moment_inertia[0][1]);
	m_MomInertia_01.SetWindowText(t);
	t.Format("%.20f",moment_inertia[0][2]);
	m_MomInertia_02.SetWindowText(t);
	t.Format("%.20f",moment_inertia[1][0]);
	m_MomInertia_10.SetWindowText(t);
	t.Format("%.20f",moment_inertia[1][1]);
	m_MomInertia_11.SetWindowText(t);
	t.Format("%.20f",moment_inertia[1][2]);
	m_MomInertia_12.SetWindowText(t);
	t.Format("%.20f",moment_inertia[2][0]);
	m_MomInertia_20.SetWindowText(t);
	t.Format("%.20f",moment_inertia[2][1]);
	m_MomInertia_21.SetWindowText(t);
	t.Format("%.20f",moment_inertia[2][2]);
	m_MomInertia_22.SetWindowText(t);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditorFSDlg_MomentOfInertia::OnOK() 
{
	CString t;
	m_MomInertia_00.GetWindowText(t);
	moment_inertia[0][0]=(float)atof(t);
	m_MomInertia_01.GetWindowText(t);
	moment_inertia[0][1]=(float)atof(t);
	m_MomInertia_02.GetWindowText(t);
	moment_inertia[0][2]=(float)atof(t);
	m_MomInertia_10.GetWindowText(t);
	moment_inertia[1][0]=(float)atof(t);
	m_MomInertia_11.GetWindowText(t);
	moment_inertia[1][1]=(float)atof(t);
	m_MomInertia_12.GetWindowText(t);
	moment_inertia[1][2]=(float)atof(t);
	m_MomInertia_20.GetWindowText(t);
	moment_inertia[2][0]=(float)atof(t);
	m_MomInertia_21.GetWindowText(t);
	moment_inertia[2][1]=(float)atof(t);
	m_MomInertia_22.GetWindowText(t);
	moment_inertia[2][2]=(float)atof(t);
	
	CDialog::OnOK();
}
