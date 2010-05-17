// OpenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MODVIEW32.h"
#include "MODVIEW32Doc.h"
#include "OpenDlg.h"
#include <afxadv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenDlg dialog


COpenDlg::COpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	pImageList=new CImageList();
	num_files=0;
	for(int i=0;i<256;i++)
		gameroot[i]=NULL;
}

COpenDlg::~COpenDlg()
{
	delete(pImageList);
}



void COpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenDlg)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_FileName);
	DDX_Control(pDX, IDC_FILETREE, m_FileTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenDlg, CDialog)
	//{{AFX_MSG_MAP(COpenDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_FILETREE, OnSelchangedFiletree)
	ON_EN_CHANGE(IDC_EDIT_FILENAME, OnChangeEditFilename)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_NOTIFY(NM_DBLCLK, IDC_FILETREE, OnDblclkFiletree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenDlg message handlers

#define GAME_FS1 GAME_FS
#define GAME_FS2 GAME_FS+1
#define GAME_RECENT	10

#define ICON_RECENT	4

BOOL COpenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	//gameroot=m_FileTree.InsertItem("Conflict, Descent: FreeSpace");
	//m_FileTree.EnsureVisible(m_FileTree.InsertItem("FREESPACE.VP",gameroot));

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
	bitmap.LoadBitmap(IDB_ICON_RECENT);
	pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();
	
	m_FileTree.SetImageList(pImageList,TVSIL_NORMAL);

	//Add standard files
	InsertItem(GAME_FS2,"FreeSpace 2 Main VP archive","D:\\Games\\FreeSpace 2 USA\\","SPARKY_FS2.VP",3);

	//Add recent file list files
	CRecentFileList mru(0,"Recent File List","File%d",10,100);
	mru.ReadList();
	for(int i=0;i<mru.GetSize();i++)
		InsertItem(GAME_RECENT,mru[i],mru[i],ICON_RECENT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COpenDlg::InsertItem(int game,CString desc, CString filename, int icon)
{
	if (game >= 256)
		return FALSE;
	if(num_files>=MAX_FILES)
		return FALSE; //Out of memory
	
	if(!FileExists(filename))
		return FALSE; //File does not exist

	for(int i=0;i<num_files;i++)
	{
		CString x1=filename;
		CString x2=file_filename[i];
		x1.MakeLower();
		x2.MakeLower();
		if(x1==x2)
			return FALSE; //Already in list
	}

	//Game name
	CString gamename;
	switch(game)
	{
	case GAME_FS1: gamename="Conflict, Descent: FreeSpace"; break;
	case GAME_FS2: gamename="FreeSpace 2"; break;
	case GAME_RECENT: gamename="Recent opened files"; break;
	default: ASSERT(FALSE);
	}

	//Add root item
	if(gameroot[game]==NULL)
		gameroot[game]=m_FileTree.InsertItem(gamename,icon,icon,TVI_ROOT);

	file_treeitem[num_files]=m_FileTree.InsertItem(desc,icon,icon,gameroot[game]);
	file_filename[num_files]=filename;
	m_FileTree.EnsureVisible(file_treeitem[num_files]);
	num_files++;

	return TRUE;
}

BOOL COpenDlg::InsertItem(int game,CString desc, CString dir, CString filename, int icon)
{
	if(dir=="")
		return FALSE;
	if(dir.Right(1)!="\\")
		dir+="\\";
	return InsertItem(game,desc,dir+filename,icon);
}

void COpenDlg::OnSelchangedFiletree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM sel=m_FileTree.GetSelectedItem();
	CString filename="";

	for(int i=0;i<num_files;i++)
	{
		if(file_treeitem[i]==sel)
			filename=file_filename[i];
	}
	
	m_FileName.SetWindowText(filename);

	*pResult = 0;
}

void COpenDlg::OnChangeEditFilename() 
{
	CString x;
	m_FileName.GetWindowText(x);
	m_OK.EnableWindow(FileExists(x));
}

BOOL COpenDlg::FileExists(CString filename)
{
	CFile f;
	if(!f.Open(filename,CFile::modeRead))
		return FALSE;
	f.Close();
	return TRUE;
}

void COpenDlg::OnBrowse() 
{
	CFileDialog*	pFileDialog;
	CMODVIEW32App*	pApp = (CMODVIEW32App*)AfxGetApp();

	pFileDialog = new CFileDialog(TRUE,"",NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		"All supported model and archive files|*.oof;*.hog;*.pof;*.vp;*.ham;*.hxm|Descent 2 model file (*.pol)|*.pol|Descent 2 archive (*.ham, *.hxm)|*.ham;*.hxm|Descent 3 model files (*.oof)|*.oof|Descent 3 archive (*.hog)|*.hog|FreeSpace 1/2 model files (*.pof)|*.pof|FreeSpace 1/2 archive (*.vp)|*.vp|All Files (*.*)|*.*||",(CWnd*)this);	
	//pFileDialog->m_ofn.lpstrInitialDir = pApp->m_sD3Path;

	if(pFileDialog->DoModal() == IDOK)
	{
		//m_FileTree.Select(NULL,TVGN_CARET);

		CString filename=pFileDialog->GetPathName();
		m_FileName.SetWindowText(filename);

		//Select file if it is already in the list
		CString x1=filename;
		x1.MakeLower();
		for(int i=0;i<num_files;i++)
		{
			CString x2=file_filename[i];
			x2.MakeLower();
			if(x1==x2)
			{
				m_FileTree.Select(file_treeitem[i],TVGN_CARET);
				m_FileTree.EnsureVisible(file_treeitem[i]);
			}
		}
	}
		//pApp->OpenDocumentFile(pFileDialog->GetPathName());
		
	delete pFileDialog;

	return;	
}

void COpenDlg::OnOK() 
{
	m_FileName.GetWindowText(m_OpenFileName);
	
	CDialog::OnOK();
}

void COpenDlg::OnDblclkFiletree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_OK.IsWindowEnabled())
		OnOK();

	*pResult = 0;
}
