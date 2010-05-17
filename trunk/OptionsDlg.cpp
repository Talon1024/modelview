// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MODVIEW32.h"
#include "MODVIEW32Doc.h"
#include "OptionsDlg.h"
#include "FolderDlg.h"
#include "MainFrm.h"
#include "DM_Reg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	//}}AFX_DATA_INIT
	pImageList=new CImageList();
	m_GameConfigChanged=FALSE;
}

COptionsDlg::~COptionsDlg()
{
	delete(pImageList);
}



void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Control(pDX, IDC_MENU_STYLE, c_MenuStyle);
	DDX_Control(pDX, IDC_QUICKRENDERING, m_QuickRendering);
	DDX_Control(pDX, IDC_D2_SMOOTHPOSITIONCHANGE, m_D2_SmoothPositionChange);
	DDX_Control(pDX, IDC_EXPLORERINDEX, m_ExplorerIndex);
	DDX_Control(pDX, IDC_SWAPMOUSEBUTTONS, m_SwapMouseButtons);
	DDX_Control(pDX, IDC_TOOLBAR_TEXT, m_ToolBar_Text);
	DDX_Control(pDX, IDC_TOOLBAR_SIZE, m_ToolBar_Size);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_BN_CLICKED(IDC_CONFIGURE, OnConfigure)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MENU_STYLE, &COptionsDlg::OnBnClickedMenuStyle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ToolBar_Text.AddString("No button text");
	m_ToolBar_Text.AddString("Selective text on right");
	m_ToolBar_Text.AddString("Text below button");

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
	
	//Read toolbar settings
	char *options=new char[256];
	LoadOptions(options);
	m_ToolBar_Text.SetCurSel(*(options+0));
	if(*(options+1)==TOOLBAR_SIZE_SMALL)
		m_ToolBar_Size.SetCheck(1);
	if(*(options+2)==1)
		m_SwapMouseButtons.SetCheck(1);
	if(*(options+3)==1)
		m_ExplorerIndex.SetCheck(1);
	if(*(options+4)==1)
		m_D2_SmoothPositionChange.SetCheck(1);
	if(*(options+5)==1)
		m_QuickRendering.SetCheck(1);

	if(DMReg_ReadHKCUint("DisableXPMenuStyle",0)==0)
		c_MenuStyle.SetCheck(TRUE);

	delete[] options;
	
	return TRUE;	// return TRUE unless you set the focus to a control
					// EXCEPTION: OCX Property Pages should return FALSE
}


CString COptionsDlg::GetF1Path()
{
	return DMReg_GetGameDir(DMREG_GAMETYPE_FREESPACE1,DMREG_DISTRIBUTIONTYPE_FULL);
}

CString COptionsDlg::GetF2Path()
{
	return DMReg_GetGameDir(DMREG_GAMETYPE_FREESPACE2,DMREG_DISTRIBUTIONTYPE_FULL);
}

void COptionsDlg::OnOK() 
{
	CString ininame=GetINIfile();

	DMReg_WriteHKCU("SwapMouseButtons",m_SwapMouseButtons.GetCheck());
	DMReg_WriteHKCU("ExplorerGroupModels",m_ExplorerIndex.GetCheck());
	DMReg_WriteHKCU("QuickRendering",m_QuickRendering.GetCheck());
	DMReg_WriteHKCU("ToolbarButtonText",m_ToolBar_Text.GetCurSel());
	DMReg_WriteHKCU("ToolbarButtonSize",m_ToolBar_Size.GetCheck()); //Important: Invert!!
	DMReg_WriteHKCU("DisableXPMenuStyle",1-c_MenuStyle.GetCheck()); //Important: Invert!!

	//TODO: Now reload options into View!!

	if(m_GameConfigChanged)
		MessageBox("Note: The tool will be restarted now.","Descent Manager MODELVIEW32");

	CDialog::OnOK();
}

char * COptionsDlg::GetMainPath(char *path)
{
	strcpy(path,AfxGetApp()->m_pszHelpFilePath);
	RemoveFilename(path);
	return path;
}

//Removes a filename from a full path statement,
//so that only the directory stays.
//RETURNS: 1 if a "\" was found
//         0 if no "\" was found (path is not modified then)
int COptionsDlg::RemoveFilename(char *path)
{
	char *p=path;
	char *d=path;
	int len=strlen(path);
	for(int i=0;i<len;i++)
	{
		if(*p=='\\')
			d=p;
		p++;
	}

	if(d!=path)
	{
		d++;
		*d='\0';
		return 1;
	}
	else
		return 0;
}

void COptionsDlg::LoadOptions(char *options)
{
	//Convert from older MODELVIEW versions (only once)
	if(DMReg_ReadHKLMint("Configured",0)!=2)
	{
		DMReg_WriteHKLM("Configured",2);
		CString ininame=GetINIfile();
		DMReg_WriteHKCU("ToolbarButtonText",GetPrivateProfileInt("Toolbar","ButtonText",TOOLBAR_TEXT_RIGHT,ininame));
		DMReg_WriteHKCU("ToolbarButtonSize",1-GetPrivateProfileInt("Toolbar","ButtonSize",1,ininame));
		DMReg_WriteHKCU("SwapMouseButtons",GetPrivateProfileInt("Controls","SwapMouseButtons",0,ininame));
		DMReg_WriteHKCU("ExplorerGroupModels",GetPrivateProfileInt("Display","ExplorerIndex",1,ininame));
		DMReg_WriteHKLM("OpenGL_WarningMessage",GetPrivateProfileInt("General","OpenGL_WarningMessage",0,ininame));
		DMReg_WriteHKCU("TipOfTheDay",GetPrivateProfileInt("General","TipOfTheDay",0,ininame));
		TRY
		{
			CFile::Remove(ininame); //Delete old INI file -> we do not need it anymore
		}
		CATCH(CFileException,e)
		{
		}
		END_CATCH
	}

	//Load options
	*(options+0)=DMReg_ReadHKCUint("ToolbarButtonText",TOOLBAR_TEXT_RIGHT);
	*(options+1)=DMReg_ReadHKCUint("ToolbarButtonSize",TOOLBAR_SIZE_LARGE);
	*(options+2)=DMReg_ReadHKCUint("SwapMouseButtons",0);
	*(options+3)=DMReg_ReadHKCUint("ExplorerGroupModels",1);
	*(options+4)=DMReg_ReadHKCUint("QuickRendering",1);
}

CString COptionsDlg::GetINIfile()
{
	CString ininame=AfxGetApp()->m_pszHelpFilePath;
	ininame.TrimRight(".HLP");
	ininame.TrimRight(".hlp");
	ininame=ininame+".ini";
	return ininame;
}

void COptionsDlg::OnConfigure() 
{
	//DoGamesConfig();
	m_GameConfigChanged=TRUE;
}


void COptionsDlg::OnBnClickedMenuStyle()
{
	// TODO: Add your control notification handler code here
}
