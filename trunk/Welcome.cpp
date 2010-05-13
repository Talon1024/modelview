// Welcome.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "modview32doc.h"
#include "Welcome.h"
#include "optionsdlg.h"
#include "folderdlg.h"
#include "DM_Reg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWelcome dialog


CWelcome::CWelcome(CWnd* pParent /*=NULL*/)
	: CDialog(CWelcome::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWelcome)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pImageList=new CImageList();
}

CWelcome::~CWelcome()
{
	delete(pImageList);
}

void CWelcome::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcome)
	DDX_Control(pDX, IDC_GAMES, m_Games);
	DDX_Control(pDX, IDC_FILETYPES, m_FileTypes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWelcome, CDialog)
	//{{AFX_MSG_MAP(CWelcome)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcome message handlers

BOOL CWelcome::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Image list
	CBitmap bitmap;
	pImageList->Create(16,16,ILC_MASK,4,0);
	bitmap.LoadBitmap(IDB_ICON_DESCENT2);
	pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_ICON_DESCENT3);
	pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_ICON_FREESPACE1);
	pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_ICON_FREESPACE2);
	pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	
	//Games
	m_Games.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_Games.SetImageList(pImageList,LVSIL_SMALL);
	m_Games.InsertColumn(0,"Game",LVCFMT_LEFT,90);
	m_Games.InsertColumn(1,"Directory",LVCFMT_LEFT,230);
	m_Games.InsertItem(0,"Descent 2",0);
	m_Games.InsertItem(1,"Descent 3",1);
	m_Games.InsertItem(2,"FreeSpace 1",2);
	m_Games.InsertItem(3,"FreeSpace 2",3);
	m_Games.SetItemState(0,LVIS_SELECTED | LVIS_FOCUSED,LVIS_SELECTED | LVIS_FOCUSED);

	//Read from INI file
	COptionsDlg dlg;
	m_Games.SetItemText(0,1,dlg.GetD2Path());
	m_Games.SetItemText(1,1,dlg.GetD3Path());
	m_Games.SetItemText(2,1,dlg.GetF1Path());
	m_Games.SetItemText(3,1,dlg.GetF2Path());

	//If something is not already configured in the INI file, try to use DVM and the registry
	
	//Associations
	m_FileTypes.AddString("*.HAM files - Descent 2 model archives");
	m_FileTypes.AddString("*.HXM files - Descent 2 add-on model archives");
	m_FileTypes.AddString("*.POL files - Descent 2 models");
	m_FileTypes.AddString("*.OOF files - Descent 3 models");
	m_FileTypes.AddString("*.POF files - FreeSpace 1/2 models");
	for(int i=0;i<5;i++)
		m_FileTypes.SetCheck(i,TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWelcome::OnOK() 
{
	CString exename,ininame;
	
	//Get name of .exe and .ininame
	exename=AfxGetApp()->m_pszHelpFilePath;
	exename.MakeUpper();
	exename.TrimRight(".HLP");
	ininame=exename;
	exename=exename+".EXE";
	ininame=ininame+".INI";
	while(exename.Replace("\\","@")!=0)
	{
	}
	while(exename.Replace("@","\\\\")!=0)
	{
	}

	//Build .REG filename in [StIf]/Shell directory
	CString temp=DMReg_GetTEMPDir()+"MODVIW32.REG";
	
	//Write .REG file
	CStdioFile ft;
	ft.Open(temp,CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	ft.WriteString("REGEDIT4\n\n");
	
	if(m_Games.GetCheck(0))
	{
		ft.WriteString("[HKEY_CLASSES_ROOT\\.HAM]\n");
		ft.WriteString("@=\"Descent_Manager.MODELVIEW32\"\n\n");
	}
	if(m_Games.GetCheck(1))
	{
		ft.WriteString("[HKEY_CLASSES_ROOT\\.HXM]\n");
		ft.WriteString("@=\"Descent_Manager.MODELVIEW32\"\n\n");
	}
	if(m_Games.GetCheck(2))
	{
		ft.WriteString("[HKEY_CLASSES_ROOT\\.POL]\n");
		ft.WriteString("@=\"Descent_Manager.MODELVIEW32\"\n\n");
	}
	if(m_Games.GetCheck(3))
	{
		ft.WriteString("[HKEY_CLASSES_ROOT\\.OOF]\n");
		ft.WriteString("@=\"Descent_Manager.MODELVIEW32\"\n\n");
	}
	if(m_Games.GetCheck(4))
	{
		ft.WriteString("[HKEY_CLASSES_ROOT\\.POF]\n");
		ft.WriteString("@=\"Descent_Manager.MODELVIEW32\"\n\n");
	}

	ft.WriteString("[HKEY_CLASSES_ROOT\\Descent_Manager.MODELVIEW32]\n");
	ft.WriteString("@=\"Descent/FreeSpace model file\"\n\n");
	ft.WriteString("[HKEY_CLASSES_ROOT\\Descent_Manager.MODELVIEW32\\shell]\n\n");
	ft.WriteString("[HKEY_CLASSES_ROOT\\Descent_Manager.MODELVIEW32\\shell\\open]\n");
	ft.WriteString("@=\"\"\n\n");
	ft.WriteString("[HKEY_CLASSES_ROOT\\Descent_Manager.MODELVIEW32\\shell\\open\\command]\n");
	ft.WriteString("@=\""+exename+" \\\"%1\\\"\"\n");
	ft.Close();

	//Call .REG file
	CString temp2="/s "; // "/s" runs REGEDIT in quiet mode
	temp2=temp2+temp;
	ShellExecute(NULL,"open","regedit",temp2,NULL,SW_SHOWNORMAL);
	//ShellExecute(NULL,"open","notepad",temp,NULL,SW_SHOWNORMAL);
	
	CDialog::OnOK();
}

void CWelcome::OnEdit() 
{
	COptionsDlg Odlg;

	CString game_dir;
	CString game_title;
	CString game_diniheader;
	CString game_dinikey;
	
	int nItem=-1;
	POSITION pos=m_Games.GetFirstSelectedItemPosition();
	while((pos!=NULL) && (nItem==-1))
	{
		nItem=m_Games.GetNextSelectedItem(pos);
		if(m_Games.GetItemState(nItem,LVIS_FOCUSED)!=LVIS_FOCUSED)
			nItem=-1;
	}
	
	switch(nItem)
	{
	case 0:
		game_title="Descent 2";
		game_diniheader="Descent";
		game_dinikey="D2Full";
		break;

	case 1:
		game_title="Descent 3";
		game_diniheader="Descent";
		game_dinikey="D3Full";
		break;

	case 2:
		game_title="FreeSpace 1";
		game_diniheader="FreeSpace";
		game_dinikey="F1Full";
		break;

	case 3:
		game_title="FreeSpace 2";
		game_diniheader="FreeSpace";
		game_dinikey="F2Full";
		break;
	
	default:
		ASSERT(FALSE);
	}

	game_dir=m_Games.GetItemText(nItem,1);
	CString title="Select directory containing " + game_title + ":";

	CFolderDialog dlg(title,game_dir,0,this);
	if(dlg.DoModal()==IDOK)
	{
		char *temp=new char[MAX_FILENAMELEN];
		WritePrivateProfileString(game_diniheader,game_dinikey,dlg.GetPathName(),Odlg.GetDescentINI(temp));
		m_Games.SetItemText(nItem,1,dlg.GetPathName());
		delete[] temp;
	}
}


void CWelcome::OnDelete() 
{
	COptionsDlg Odlg;

	//CString game_dir;
	CString game_title;
	CString game_diniheader;
	CString game_dinikey;
	
	int nItem=-1;
	POSITION pos=m_Games.GetFirstSelectedItemPosition();
	while((pos!=NULL) && (nItem==-1))
	{
		nItem=m_Games.GetNextSelectedItem(pos);
		if(m_Games.GetItemState(nItem,LVIS_FOCUSED)!=LVIS_FOCUSED)
			nItem=-1;
	}
	
	switch(nItem)
	{
	case 0:
		game_title="Descent 2";
		game_diniheader="Descent";
		game_dinikey="D2Full";
		break;

	case 1:
		game_title="Descent 3";
		game_diniheader="Descent";
		game_dinikey="D3Full";
		break;

	case 2:
		game_title="FreeSpace 1";
		game_diniheader="FreeSpace";
		game_dinikey="F1Full";
		break;

	case 3:
		game_title="FreeSpace 2";
		game_diniheader="FreeSpace";
		game_dinikey="F2Full";
		break;
	
	default:
		ASSERT(FALSE);
	}


	if(MessageBox("Are you sure you want to remove the game directory information (note: this will NOT delete the game itself)?","Game Directory",MB_YESNO)==IDYES)
	{
		char *temp=new char[MAX_FILENAMELEN];
		WritePrivateProfileString(game_diniheader,game_dinikey,"",Odlg.GetDescentINI(temp));
		m_Games.SetItemText(nItem,1,"");
		delete[] temp;
	}
}
