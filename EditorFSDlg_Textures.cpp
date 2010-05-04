// EditorFSDlg_Textures.cpp : implementation file
//

#include "stdafx.h"
#include "MODVIEW32.h"
#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "MainFrm.h"
#include "EditorFSDlg_Textures.h"
#include "EditorFSDlg_TextureNamesEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorFSDlg_Textures dialog

CEditorFSDlg_Textures::CEditorFSDlg_Textures(CWnd* pParent /*=NULL*/)
	: CDialog(CEditorFSDlg_Textures::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditorFSDlg_Textures)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEditorFSDlg_Textures::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorFSDlg_Textures)
	DDX_Control(pDX, IDC_TEXTURENAMES, m_TextureNames_List);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorFSDlg_Textures, CDialog)
	//{{AFX_MSG_MAP(CEditorFSDlg_Textures)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorFSDlg_Textures message handlers

BOOL CEditorFSDlg_Textures::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_TextureNames_List.SetExtendedStyle(LVS_EX_FULLROWSELECT /*| LVS_EX_GRIDLINES*/);
	m_TextureNames_List.InsertColumn(0,"Texture name",LVCFMT_LEFT,360);

/*		for(i=m_FS_PofDataL[ActivePM];i<(m_FS_PofDataH[ActivePM]+1);i++)
		{
			FileType=NEW_FILE;
			m_FS_BitmapData.pic[i].valid=1;

			m_TexturesNum++;
			m_Textures[m_TexturesNum].Filename=m_FS_BitmapData.pic[i].name;*/


	for(int i=0; i<GetDocument()->m_TexturesNum+1; i++)
		VERIFY(m_TextureNames_List.InsertItem(i,GetDocument()->m_FS_BitmapData.pic[GetDocument()->m_FS_PofDataL[GetMainView()->m_Detaillevel]+i].name)==i);
		//VERIFY(m_TextureNames_List.InsertItem(i,GetDocument()->m_Textures[i].Filename)==i);
	//
	m_TextureNames_List.SetItemState(0,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);

	return TRUE;
}

void CEditorFSDlg_Textures::OnOK()
{
	CString x;
	POSITION pos=m_TextureNames_List.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{
		MessageBox("Nothing selected.","Error");
		return;
	}
	int nItem=m_TextureNames_List.GetNextSelectedItem(pos);

	CEditorFSDlg_TextureNamesEdit dlg;
	dlg.texname=m_TextureNames_List.GetItemText(nItem,0);
	if(dlg.DoModal()==IDOK)
	{
		strcpy_s(GetDocument()->m_FS_BitmapData.pic[GetDocument()->m_FS_PofDataL[GetMainView()->m_Detaillevel]+nItem].name,dlg.texname);
		m_TextureNames_List.SetItemText(nItem,0,dlg.texname);
		GetDocument()->SetModifiedFlag(TRUE);
	}
}


CMODVIEW32Doc * CEditorFSDlg_Textures::GetDocument()
{
	CMODVIEW32App *m_App=(CMODVIEW32App *)AfxGetApp();
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_App->m_pMainWnd);
	return (CMODVIEW32Doc *)viewFrame->GetDocument();
}

CMODVIEW32View * CEditorFSDlg_Textures::GetMainView()
{
	CMODVIEW32App *m_App=(CMODVIEW32App *)AfxGetApp();
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_App->m_pMainWnd);
	return (CMODVIEW32View *)viewFrame->GetCurrentView();
}
