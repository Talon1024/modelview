// ImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "ImportDlg.h"
#include "ImportCOB.h"
#include "OptionsDlg.h"
#include "DM_Reg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportDlg dialog


CImportDlg::CImportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportDlg)
	DDX_Control(pDX, IDOK, c_OK);
	DDX_Control(pDX, IDC_SOURCEFILE, m_SourceFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportDlg, CDialog)
	//{{AFX_MSG_MAP(CImportDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnChange)
	ON_BN_CLICKED(IDC_RADIO2, OnChange)
	ON_EN_CHANGE(IDC_SOURCEFILE, OnChange)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportDlg message handlers

BOOL CImportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Load options
	switch(DMReg_ReadHKCUint("Import_LastGame",0))
	{
	case 1:
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
		break;
	case 2:
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
		break;
	}

	//Display options
	m_SourceFile.SetWindowText(DMReg_ReadHKCU("Import_LastSource",""));
	OnChange();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportDlg::OnOK() 
{
	CString cobname;
	m_SourceFile.GetWindowText(cobname);
	BOOL gameisfs1=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
	BOOL gameisfs2=((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck();
	ASSERT(gameisfs1!=gameisfs2);
	int m_Game=1;
	if(gameisfs2==1)
		m_Game=2;

	//Save options
	DMReg_WriteHKCU("Import_LastSource",cobname);
	DMReg_WriteHKCU("Import_LastGame",m_Game);

	//Do the import
	CImportCOB *impcob;
	impcob=new CImportCOB();
	impcob->m_Game=m_Game;

	CFile f;
	f.Open(cobname,CFile::modeRead);
	errnum=impcob->ReadCOB(&f);
	if(errnum==ERROR_GEN_NOERROR)
		/*errnum=*/impcob->ConvertCob2Model();
	if(errnum==ERROR_GEN_NOERROR)
		/*errnum=*/impcob->MakePof(/*"C:\\test.pof"*/);
	errstring=impcob->errstring;
	delete(impcob);
		
	CDialog::OnOK();
}

void CImportDlg::OnChange() 
{
	BOOL enableit=TRUE;
	BOOL gameisfs1=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
	BOOL gameisfs2=((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck();
	if(gameisfs1==gameisfs2)
		enableit=FALSE;
	CString x;
	m_SourceFile.GetWindowText(x);
	if(x.IsEmpty())
		enableit=FALSE;
	if(!_FileExists(x))
		enableit=FALSE;
	c_OK.EnableWindow(enableit);
}

void CImportDlg::OnBrowse() 
{
	CFileDialog*	pFileDialog;

	pFileDialog = new CFileDialog(TRUE,"",NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		"TrueSpace COB file|*.cob|All Files (*.*)|*.*||",(CWnd*)this);	
	//pFileDialog->m_ofn.lpstrInitialDir = pApp->m_sD3Path;

	if(pFileDialog->DoModal() == IDOK)
		m_SourceFile.SetWindowText(pFileDialog->GetPathName());
		
	delete pFileDialog;
}
