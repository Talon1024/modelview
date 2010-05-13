// EditorFS_SHLD.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "EditorFS_SHLD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SHLD

IMPLEMENT_DYNCREATE(CEditorFS_SHLD, CFormView)

CEditorFS_SHLD::CEditorFS_SHLD()
	: CFormView(CEditorFS_SHLD::IDD)
{
	//{{AFX_DATA_INIT(CEditorFS_SHLD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_DataLocked=FALSE;
}

CEditorFS_SHLD::~CEditorFS_SHLD()
{
}

void CEditorFS_SHLD::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorFS_SHLD)
	DDX_Control(pDX, IDC_EDITORFS_SHLD_MODIFY_MULTZ, m_Modify_MultZ);
	DDX_Control(pDX, IDC_EDITORFS_SHLD_MODIFY_MULTY, m_Modify_MultY);
	DDX_Control(pDX, IDC_EDITORFS_SHLD_MODIFY_MULTX, m_Modify_MultX);
	DDX_Control(pDX, ID_EDITORFS_SHLD_CURPOLY, m_Current_Polygons);
	DDX_Control(pDX, ID_EDITORFS_SHLD_CURVERT, m_Current_Vertices);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorFS_SHLD, CFormView)
	//{{AFX_MSG_MAP(CEditorFS_SHLD)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EDITORFS_SHLD_RESTOREDATA, OnEditorfsShldRestoredata)
	ON_BN_CLICKED(IDC_EDITORFS_SHLD_REMOVEDATA, OnEditorfsShldRemovedata)
	ON_BN_CLICKED(IDC_EDITORFS_SHLD_IMPORT, OnEditorfsShldImport)
	ON_EN_CHANGE(IDC_EDITORFS_SHLD_MODIFY_MULTX, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_SHLD_MODIFY_MULTY, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_SHLD_MODIFY_MULTZ, OnChangeData)
	ON_COMMAND_RANGE(ID_ELEMENTAPPLYER_APPLY, ID_ELEMENTAPPLYER_DISCARD, OnApplyerBarClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SHLD diagnostics

#ifdef _DEBUG
void CEditorFS_SHLD::AssertValid() const
{
	CFormView::AssertValid();
}

void CEditorFS_SHLD::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CMainFrame * CEditorFS_SHLD::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CEditorFS_SHLD::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CEditorFS_SHLD::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SHLD message handlers

void CEditorFS_SHLD::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CEditorFS_SHLD::Init()
{
	m_AmEditing=FALSE;
	m_TBApplyer.ShowWindow(FALSE);
	m_OwnShieldsBackup=GetDocument()->m_FS_Model.shields;
	UpdateEditorDisplay();
}

void CEditorFS_SHLD::UpdateEditorDisplay()
{
	CString txt;
	txt.Format("%i vertices",GetDocument()->m_FS_Model.shields.Vcount);
	m_Current_Vertices.SetWindowText(txt);
	txt.Format("%i faces",GetDocument()->m_FS_Model.shields.Fcount);
	m_Current_Polygons.SetWindowText(txt);
}

BOOL CEditorFS_SHLD::UnInit()
{
	return FinishPendingEditing();
}

BOOL CEditorFS_SHLD::FinishPendingEditing()
{
	if(m_AmEditing)
	{
		int a=MessageBox("Do you want to apply changes first?\n\nPress Yes to Apply, No to Discard, Cancel to do nothing.","Apply changes",MB_YESNOCANCEL);
		switch(a)
		{
		case IDYES:	DoApply(); break;
		case IDNO: DoDiscard(); break;
		case IDCANCEL: return FALSE;
		default: ASSERT(FALSE);
		}
	}
	return TRUE;
}

void CEditorFS_SHLD::OnEditorfsShldModifyApply() 
{
	CString t;
	m_Modify_MultX.GetWindowText(t);
	float mx=(float)atof(t);
	m_Modify_MultY.GetWindowText(t);
	float my=(float)atof(t);
	m_Modify_MultZ.GetWindowText(t);
	float mz=(float)atof(t);

	if(mx==0 || my==0 || mz==0)
	{
		MessageBox("None of the multipliers may be 0.","Error");
		return;
	}

	for(unsigned int i=0;i<GetDocument()->m_FS_Model.shields.Vcount;i++)
	{
		GetDocument()->m_FS_Model.shields.Vpoint[i][0]*=mx;
		GetDocument()->m_FS_Model.shields.Vpoint[i][1]*=my;
		GetDocument()->m_FS_Model.shields.Vpoint[i][2]*=mz;
	}
	m_AmEditing=TRUE;
	m_TBApplyer.ShowWindow(TRUE);
	GetMainView()->InvalidateRect(NULL);
}

void CEditorFS_SHLD::OnEditorfsShldRestoredata() 
{
	if(MessageBox("This will revert all your changes to the state since you last saved the model. Are you sure you want to do this?","Confirmation",MB_YESNO)==IDYES)
	{
		m_AmEditing=TRUE;
		m_TBApplyer.ShowWindow(TRUE);
		GetDocument()->m_FS_Model.shields=GetDocument()->m_EditorFS_ShieldsBackup;
		UpdateEditorDisplay();
		GetMainView()->InvalidateRect(NULL);
	}
}

void CEditorFS_SHLD::OnEditorfsShldRemovedata() 
{
	if(MessageBox("This will delete shield data. Are you sure you want to do this?","Confirmation",MB_YESNO)==IDYES)
	{
		m_AmEditing=TRUE;
		m_TBApplyer.ShowWindow(TRUE);
		GetDocument()->m_FS_Model.shields.Vcount=0;
		GetDocument()->m_FS_Model.shields.Fcount=0;
		UpdateEditorDisplay();
		GetMainView()->InvalidateRect(NULL);
	}
}

void CEditorFS_SHLD::OnEditorfsShldImport() 
{
	//Prompt for filename
	CFileDialog*	pFileDialog;
	pFileDialog = new CFileDialog(TRUE,"",NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		"FreeSpace 1/2 POF file|*.pof||",(CWnd*)this);
	//pFileDialog->Title="Select POF file to import shield data from";
	int u=pFileDialog->DoModal();
	CString filename=pFileDialog->GetPathName();
	delete pFileDialog;

	//If user pressed cancel, jump out
	if(u!=IDOK)
		return;

	//Import shields
	FS_SHIELDS ImportShields;
	switch(ReadShieldData(filename,&ImportShields))
	{
	case -2:
		MessageBox("Shield block in this file contains too many vertices or faces (>1600).","Cannot import");
		break;

	case -1:
		MessageBox("Error while reading file. Either the file is not accessable or it is not a valid or supported POF file.","Cannot import");
		break;
	
	case 0:
		MessageBox("This POF file does not contain any shield data.","Cannot import");
		break;

	case 1:
		m_AmEditing=TRUE;
		m_TBApplyer.ShowWindow(TRUE);
		GetDocument()->m_FS_Model.shields=ImportShields;
		UpdateEditorDisplay();
		GetMainView()->InvalidateRect(NULL);
		break;

	default:
		MessageBox("An unknown error occured.","Cannot import");
		ASSERT(FALSE);
	}
}

//0 = no shield data, 1=shield data, -1=file error
int CEditorFS_SHLD::ReadShieldData(CString fn, FS_SHIELDS *sh)
{
	sh->Fcount=0;
	sh->Vcount=0;

	CFile f;
	if(!f.Open(fn,CFile::modeRead))
		return -1;

	f.Seek(0L, SEEK_END);
	ULONGLONG filesize=f.GetPosition();
	f.Seek(0L, SEEK_SET);

	int ri;
	f.Read(&ri,4);
	if(ri!=0x4f505350)
		return -1;
	f.Read(&ri,4);

	int chunktype;
	int chunksize;
	ULONGLONG chunkposi;
	while(f.GetPosition()<filesize)
	{
		f.Read(&chunktype,4);
		f.Read(&chunksize,4);
		chunkposi=f.GetPosition();

		if(chunktype==ID_SHLD)
		{
			f.Read(&sh->Vcount,4);
			if(sh->Vcount>MAX_FS_SHIELDVERTICES)
				return -2;
			for( unsigned int i=0;i<sh->Vcount;i++)
			{
				f.Read(&sh->Vpoint[i][0],4);
				f.Read(&sh->Vpoint[i][1],4);
				f.Read(&sh->Vpoint[i][2],4);
			}

			f.Read(&sh->Fcount,4);
			if(sh->Fcount>MAX_FS_SHIELDFACES)
				return -2;
			for(unsigned int i=0; i<sh->Fcount; i++)
			{
				f.Read(&sh->Face[i].Normal.x,4);
				f.Read(&sh->Face[i].Normal.y,4);
				f.Read(&sh->Face[i].Normal.z,4);
				f.Read(&sh->Face[i].Vface[0],4);
				f.Read(&sh->Face[i].Vface[1],4);
				f.Read(&sh->Face[i].Vface[2],4);
				f.Read(&sh->Face[i].Nface[0],4);
				f.Read(&sh->Face[i].Nface[1],4);
				f.Read(&sh->Face[i].Nface[2],4);
			}
			ASSERT(f.GetPosition()==chunkposi+chunksize);
#ifdef _DEBUG
{
	CString x;
	x.Format("Fcount=%i,Vcount=%i\n",sh->Fcount,sh->Vcount);
	OutputDebugString(x);
}
#endif
			return 1;
		}


		f.Seek(chunkposi+chunksize,SEEK_SET);
	}
	return 0; //no SHLD chunk
}


void CEditorFS_SHLD::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if(!::IsWindow(m_TBApplyer))
	{
		WINDOWPLACEMENT wndPlmnt;
		CStatic *placeHld;

		m_TBApplyer.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_AUTOSIZE, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_TBApplyer.LoadToolBar(IDR_ELEMENTAPPLYER);
		m_TBApplyer.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_TBApplyer.SetButtonText(0,"Apply");
		m_TBApplyer.SetButtonText(1,"Discard");
		GetMainFrame()->ResizeToolbar(&m_TBApplyer,TRUE);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_SHLD_TOOLBARAPPLYER);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBApplyer.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);	
		m_TBApplyer.ShowWindow(FALSE);
		m_AmEditing=FALSE;

	}
	m_DataLocked=TRUE;
	m_Modify_MultX.SetWindowText("1.0");
	m_Modify_MultY.SetWindowText("1.0");
	m_Modify_MultZ.SetWindowText("1.0");
	m_DataLocked=FALSE;
}


void CEditorFS_SHLD::DoApply()
{
	m_OwnShieldsBackup=GetDocument()->m_FS_Model.shields;
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	DoDiscard();
}

void CEditorFS_SHLD::DoDiscard()
{
	m_DataLocked=TRUE;
	GetDocument()->m_FS_Model.shields=m_OwnShieldsBackup;
	m_AmEditing=FALSE;
	m_Modify_MultX.SetWindowText("1.0");
	m_Modify_MultY.SetWindowText("1.0");
	m_Modify_MultZ.SetWindowText("1.0");
	m_DataLocked=FALSE;
	GetMainView()->InvalidateRect(NULL);
	m_TBApplyer.ShowWindow(FALSE);
}

void CEditorFS_SHLD::OnApplyerBarClick(UINT nID)
{
	switch(nID)
	{
	case ID_ELEMENTAPPLYER_APPLY:
		DoApply();
		break;

	case ID_ELEMENTAPPLYER_DISCARD:
		DoDiscard();
		break;
	}
}

void CEditorFS_SHLD::OnChangeData()
{
	if(!m_DataLocked)
	{
		CString t;
		m_Modify_MultX.GetWindowText(t);
		float mx=(float)atof(t);
		m_Modify_MultY.GetWindowText(t);
		float my=(float)atof(t);
		m_Modify_MultZ.GetWindowText(t);
		float mz=(float)atof(t);

		if(mx==0) mx=1;
		if(my==0) my=1;
		if(mz==0) mz=1;

		for(unsigned int i=0;i<GetDocument()->m_FS_Model.shields.Vcount;i++)
		{
			GetDocument()->m_FS_Model.shields.Vpoint[i][0]=m_OwnShieldsBackup.Vpoint[i][0]*mx;
			GetDocument()->m_FS_Model.shields.Vpoint[i][1]=m_OwnShieldsBackup.Vpoint[i][1]*my;
			GetDocument()->m_FS_Model.shields.Vpoint[i][2]=m_OwnShieldsBackup.Vpoint[i][2]*mz;
		}
		m_AmEditing=TRUE;
		m_TBApplyer.ShowWindow(TRUE);
		GetMainView()->InvalidateRect(NULL);
	}
}
