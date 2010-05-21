// EditorFSDlg_TextureNamesEdit.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "EditorFSDlg_TextureNamesEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorFSDlg_TextureNamesEdit dialog


CEditorFSDlg_TextureNamesEdit::CEditorFSDlg_TextureNamesEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorFSDlg_TextureNamesEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditorFSDlg_TextureNamesEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEditorFSDlg_TextureNamesEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorFSDlg_TextureNamesEdit)
	DDX_Control(pDX, IDC_TEXTURENAME, m_TextureName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorFSDlg_TextureNamesEdit, CDialog)
	//{{AFX_MSG_MAP(CEditorFSDlg_TextureNamesEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorFSDlg_TextureNamesEdit message handlers


BOOL CEditorFSDlg_TextureNamesEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_TextureName.SetWindowText(texname);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditorFSDlg_TextureNamesEdit::OnOK() 
{
	m_TextureName.GetWindowText(texname);
	if(texname.GetLength()<1)
	{
		MessageBox("Texture name may be empty.","Error");
		return;
	}
	if(texname.GetLength()>29)
	{
		MessageBox("Texture name may not exceed 29 characters.","Error");
		return;
	}
	
	CDialog::OnOK();
}

