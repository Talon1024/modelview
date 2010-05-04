// EditorFS_SPCL.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "EditorFS_SPCL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SPCL

IMPLEMENT_DYNCREATE(CEditorFS_SPCL, CFormView)

CEditorFS_SPCL::CEditorFS_SPCL()
	: CFormView(CEditorFS_SPCL::IDD)
{
	//{{AFX_DATA_INIT(CEditorFS_SPCL)
	//}}AFX_DATA_INIT
	m_DataLocked=FALSE;
}

CEditorFS_SPCL::~CEditorFS_SPCL()
{
}

void CEditorFS_SPCL::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorFS_SPCL)
	DDX_Control(pDX, IDC_EDITORFS_SUBSYSTEMS_LIST, m_List);
	DDX_Control(pDX, IDC_EDITORFS_SPCL_PROPERTIES, m_Properties);
	DDX_Control(pDX, IDC_EDITORFS_SPCL_CENTER_Z, m_CenterZ);
	DDX_Control(pDX, IDC_EDITORFS_SPCL_CENTER_Y, m_CenterY);
	DDX_Control(pDX, IDC_EDITORFS_SPCL_CENTER_X, m_CenterX);
	DDX_Control(pDX, IDC_EDITORFS_SPCL_RADIUS, m_Radius);
	DDX_Control(pDX, IDC_EDITORFS_SPCL_NAME, m_Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorFS_SPCL, CFormView)
	//{{AFX_MSG_MAP(CEditorFS_SPCL)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_EDITORFS_SUBSYSTEMS_LIST, OnSelchangeEditorfsSubsystemsList)
	ON_EN_CHANGE(IDC_EDITORFS_SPCL_CENTER_X, OnChangeData)
	ON_COMMAND_RANGE(ID_ELEMENTAPPLYER_APPLY, ID_ELEMENTAPPLYER_DISCARD, OnApplyerBarClick)
	ON_COMMAND_RANGE(ID_ELEMENTEDITOR_CREATE,ID_ELEMENTEDITOR_PASTE, OnEditorBarClick)
	ON_EN_CHANGE(IDC_EDITORFS_SPCL_CENTER_Y, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_SPCL_CENTER_Z, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_SPCL_NAME, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_SPCL_RADIUS, OnChangeData)
	ON_CBN_SELCHANGE(IDC_EDITORFS_SPCL_PROPERTIES, OnChangeData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SPCL diagnostics

#ifdef _DEBUG
void CEditorFS_SPCL::AssertValid() const
{
	CFormView::AssertValid();
}

void CEditorFS_SPCL::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CMainFrame * CEditorFS_SPCL::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CEditorFS_SPCL::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CEditorFS_SPCL::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}


/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SPCL message handlers

void CEditorFS_SPCL::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if(!::IsWindow(m_TBElements))
	{
		WINDOWPLACEMENT wndPlmnt;
		CStatic *placeHld;

		m_TBElements.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBElements.LoadToolBar(IDR_ELEMENTEDITOR);
		m_TBElements.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_SPCL_TOOLBARELEMENTS);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBElements.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		m_TBApplyer.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_AUTOSIZE, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_TBApplyer.LoadToolBar(IDR_ELEMENTAPPLYER);
		m_TBApplyer.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_TBApplyer.SetButtonText(0,"Apply");
		m_TBApplyer.SetButtonText(1,"Discard");
		GetMainFrame()->ResizeToolbar(&m_TBApplyer,TRUE);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_SPCL_TOOLBARAPPLYER);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBApplyer.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

		m_TBApplyer.ShowWindow(FALSE);
	}
}

void CEditorFS_SPCL::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (::IsWindow(m_hWnd))
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
		
	/*CStatic *head=(CStatic *)GetDlgItem(IDC_EDITORFS_SPCL_HEADER);
	if(head!=NULL)
	{
		if(::IsWindow(head->m_hWnd))
			head->SetWindowPos(NULL,1,2,cx-69,22,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}*/

	if(::IsWindow(m_TBElements.m_hWnd))
		m_TBElements.SetWindowPos(NULL,cx-69,0,0,0,SWP_NOSIZE | SWP_NOREDRAW | SWP_NOZORDER);
	//if(::IsWindow(m_TBApplyer.m_hWnd))
	//	m_TBApplyer.SetWindowPos(NULL,cx-132,200,0,0,SWP_NOSIZE | SWP_NOREDRAW | SWP_NOZORDER);

	CListBox *lbox=(CListBox *)GetDlgItem(IDC_EDITORFS_SUBSYSTEMS_LIST);
	if(lbox!=NULL)
	{
		if(::IsWindow(lbox->m_hWnd))
			lbox->SetWindowPos(NULL,1,2,cx-2,160/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	
	AdaptEditorBox(cx,&m_Name);
	AdaptEditorBox(cx,&m_Properties);
	AdaptEditorBox(cx,&m_Radius);
	AdaptVectorBox(cx,&m_CenterX,&m_CenterY,&m_CenterZ);
	InvalidateRect(NULL);
}





void CEditorFS_SPCL::AdaptEditorBox(int cx,CWnd *ed)
{
	if(ed==NULL)
		return;
	if(!::IsWindow(ed->m_hWnd))
		return;

	CRect rect1,rect2;
	GetWindowRect(rect1);
	ed->GetWindowRect(rect2);
	rect2.right-=rect2.left;
	rect2.bottom-=rect2.top;
	rect2.left-=rect1.left;
	rect2.top-=rect1.top;

	ed->SetWindowPos(NULL,rect2.left,rect2.top,cx-rect2.left-BOXES_DISTANCE,rect2.bottom,SWP_NOZORDER);
}

void CEditorFS_SPCL::AdaptVectorBox(int cx,CEdit *ed1,CEdit *ed2,CEdit *ed3)
{
	if(ed1==NULL || ed2==NULL || ed3==NULL)
		return;
	if(!::IsWindow(ed1->m_hWnd) || !::IsWindow(ed2->m_hWnd) || !::IsWindow(ed3->m_hWnd))
		return;

	CRect rect1,rect2;
	GetWindowRect(rect1);
	ed1->GetWindowRect(rect2);
	rect2.right-=rect2.left;
	rect2.bottom-=rect2.top;
	rect2.left-=rect1.left;
	rect2.top-=rect1.top;

	int width=cx-rect2.left-BOXES_DISTANCE;
	width-=BOXES_DISTANCE*2;
	width/=3;

	/*CString x;
	x.Format("%i %i %i %i\n",rect2.left,rect2.top,rect2.right,rect2.bottom);
	OutputDebugString(x);*/

	ed1->SetWindowPos(NULL,rect2.left,rect2.top,width,rect2.bottom,SWP_NOZORDER);
	ed2->SetWindowPos(NULL,rect2.left+width+BOXES_DISTANCE,rect2.top,width,rect2.bottom,SWP_NOZORDER);
	ed3->SetWindowPos(NULL,rect2.left+width*2+BOXES_DISTANCE*2,rect2.top,width,rect2.bottom,SWP_NOZORDER);
}

void CEditorFS_SPCL::Init()
{
	CListBox *lbox=(CListBox *)GetDlgItem(IDC_EDITORFS_SUBSYSTEMS_LIST);
	CMODVIEW32Doc *pDoc=GetDocument();
		
	lbox->ResetContent();
	for(int i=0;i<pDoc->m_FS_NumSubsystems;i++)
		lbox->AddString(pDoc->m_FS_Subsystems[i].Desc);
	lbox->SetCurSel(0);
	m_AmEditing=FALSE;
	OnSelchangeEditorfsSubsystemsList();
	UpdateEditorDisplay(FALSE);
}

BOOL CEditorFS_SPCL::UnInit()
{
	if(!FinishPendingEditing())
		return FALSE;
	GetMainView()->m_FS_DisplaySubsystem=-1;
	return TRUE;
}

BOOL CEditorFS_SPCL::FinishPendingEditing()
{
	if(m_AmEditing)
	{
		int a=MessageBox("Do you want to apply changes first?\n\nPress Yes to Apply, No to Discard, Cancel to do nothing.","Apply changes",MB_YESNOCANCEL);
		switch(a)
		{
		case IDYES:	return DoApply(); break;
		case IDNO: DoDiscard(); break;
		case IDCANCEL: return FALSE;
		default: ASSERT(FALSE);
		}
	}
	return TRUE;
}


void CEditorFS_SPCL::OnSelchangeEditorfsSubsystemsList() 
{
	UnInit();
	if(m_List.GetCount()==0)
		return;

	//Fill editor boxes
	m_DataLocked=TRUE;
	m_IsNewItem=FALSE;
	int m_CurrentItem=m_List.GetCurSel();
	CString fl;
	fl.Format("%.4f",GetDocument()->m_FS_Subsystems[m_CurrentItem].Center.x);
	m_CenterX.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_FS_Subsystems[m_CurrentItem].Center.y);
	m_CenterY.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_FS_Subsystems[m_CurrentItem].Center.z);
	m_CenterZ.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_FS_Subsystems[m_CurrentItem].Radius);
	m_Radius.SetWindowText(fl);
	m_Name.SetWindowText(GetDocument()->m_FS_Subsystems[m_CurrentItem].Name);
	if(GetDocument()->m_FS_Subsystems[m_CurrentItem].Properties.Find("subsystem")==-1)
		m_Properties.SetCurSel(1);
	else
		m_Properties.SetCurSel(0);
	m_DataLocked=FALSE;

	//Update display
	GetDocument()->m_EditorFS_Subsystem=GetDocument()->m_FS_Subsystems[m_CurrentItem];
	GetMainView()->m_FS_DisplaySubsystem=-2;
	GetMainView()->InvalidateRect(NULL);
	//GetMainFrame()->m_Editor_DocChanged
}

void CEditorFS_SPCL::OnChangeData() 
{
	if(!m_DataLocked)
	{
		OutputDebugString("OnChangeData\n");
		UpdateEditorDisplay(TRUE);
		
		//Read data
		CString t;
		m_Name.GetWindowText(t);
		GetDocument()->m_EditorFS_Subsystem.Name=t;
		m_CenterX.GetWindowText(t);
		GetDocument()->m_EditorFS_Subsystem.Center.x=(float)atof(t);
		m_CenterY.GetWindowText(t);
		GetDocument()->m_EditorFS_Subsystem.Center.y=(float)atof(t);
		m_CenterZ.GetWindowText(t);
		GetDocument()->m_EditorFS_Subsystem.Center.z=(float)atof(t);
		m_Radius.GetWindowText(t);
		GetDocument()->m_EditorFS_Subsystem.Radius=(float)atof(t);
		if(m_Properties.GetCurSel()==1)
			GetDocument()->m_EditorFS_Subsystem.Properties="";
		else
			GetDocument()->m_EditorFS_Subsystem.Properties="$special=subsystem";
		
		GetMainView()->InvalidateRect(NULL);
	}
}

BOOL CEditorFS_SPCL::DoApply()
{
	int m_CurrentItem=m_List.GetCurSel();
	CString x1=GetDocument()->m_EditorFS_Subsystem.Name;
	x1.MakeLower();
	if(x1.Left(1)!="$")
		x1="$"+x1;
	GetDocument()->m_EditorFS_Subsystem.Name=x1;

	//Check whether the name is valid
	if(GetDocument()->m_EditorFS_Subsystem.Name.GetLength()<2)
	{
		MessageBox("The name must not be empty. Please specify a name!","Error applying item");
		return FALSE;
	}

	//Check whether the name is really unique
	for(int i=0;i<m_List.GetCount();i++)
	{
		CString x2=GetDocument()->m_FS_Subsystems[i].Name;
		x2.MakeLower();
		if((x1==x2) && (i!=m_CurrentItem))
		{
			MessageBox("There is already an item with this name. Please specify a different name!","Error applying item");
			return FALSE;
		}
	}

	//Replace old item
	GetDocument()->m_EditorFS_Subsystem.Desc=GetDocument()->FS_MakeSubsystemDesc(x1,GetDocument()->m_EditorFS_Subsystem.Properties);
	GetDocument()->m_FS_Subsystems[m_CurrentItem]=GetDocument()->m_EditorFS_Subsystem;
	m_List.InsertString(m_CurrentItem,GetDocument()->m_EditorFS_Subsystem.Desc);
	m_List.DeleteString(m_CurrentItem+1);
	m_List.SetCurSel(m_CurrentItem);

	//Handle new item
	if(m_IsNewItem)
		GetDocument()->m_FS_NumSubsystems++;

	m_IsNewItem=FALSE;
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	DoDiscard();
	return TRUE;
}

void CEditorFS_SPCL::DoDiscard()
{
	if(m_IsNewItem)
	{
		m_List.DeleteString(m_List.GetCount()-1);
		if(m_List.GetCount()!=0)
			m_List.SetCurSel(0);
	}
	UpdateEditorDisplay(FALSE);
	OnSelchangeEditorfsSubsystemsList();
}

void CEditorFS_SPCL::OnApplyerBarClick(UINT nID)
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

void CEditorFS_SPCL::OnEditorBarClick(UINT nID)
{
	switch(nID)
	{
	case ID_ELEMENTEDITOR_CREATE:
		DoNewItem();
		break;

	case ID_ELEMENTEDITOR_DELETE:
		DoDeleteItem();
		break;

	case ID_ELEMENTEDITOR_COPY:
		DoCopyItem();
		break;
	
	default:
		ASSERT(FALSE);
		AfxAbort(); //"OnEditorBarClick with unknown nID");
	}
}


void CEditorFS_SPCL::UpdateEditorDisplay(BOOL editing)
{
	//If there is at least one item
	m_AmEditing=editing;
	BOOL ic=(m_List.GetCount()>0);
	if(ic)
	{
		m_DataChanged=editing;
		m_List.EnableWindow(!editing);
		m_TBElements.ShowWindow(!editing);
		m_TBApplyer.ShowWindow(editing);
	}
	else
	{
		m_TBElements.ShowWindow(TRUE);
		m_TBApplyer.ShowWindow(FALSE);
	}

	m_CenterX.EnableWindow(ic);
	m_CenterY.EnableWindow(ic);
	m_CenterZ.EnableWindow(ic);
	m_Radius.EnableWindow(ic);
	m_Name.EnableWindow(ic);
	m_Properties.EnableWindow(ic);
}


void CEditorFS_SPCL::DoDeleteItem()
{
	if(m_List.GetCurSel()<0 || m_List.GetCount()<=0)
		return;

	if(MessageBox("Are you sure you want to delete this item?","Confirmation",MB_YESNO)!=IDYES)
		return;

	//Delete subsystem
	int m_CurrentItem=m_List.GetCurSel();
	for(int i=m_CurrentItem;i<m_List.GetCount()-1;i++)
		GetDocument()->m_FS_Subsystems[i]=GetDocument()->m_FS_Subsystems[i+1];
	GetDocument()->m_FS_NumSubsystems--;

	//Remove from list
	m_List.DeleteString(m_CurrentItem);
	
	//Update display
	if(GetDocument()->m_FS_NumSubsystems==0)
	{
		m_CenterX.SetWindowText("");
		m_CenterY.SetWindowText("");
		m_CenterZ.SetWindowText("");
		m_Radius.SetWindowText("");
		m_Name.SetWindowText("");
	}
	else if(GetDocument()->m_FS_NumSubsystems<=m_CurrentItem)
		m_List.SetCurSel(m_CurrentItem-1);
	else
		m_List.SetCurSel(m_CurrentItem);

	UpdateEditorDisplay(FALSE);
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	OnSelchangeEditorfsSubsystemsList();
}

void CEditorFS_SPCL::DoNewItem()
{
	OutputDebugString("DoNewItem\n");

	//Prepare display
	m_List.AddString("<New item>");
	m_List.SetCurSel(m_List.GetCount()-1);
	UpdateEditorDisplay(TRUE);

	//Prepare Editor
	m_DataLocked=TRUE;
	CString t;
	m_Name.SetWindowText("");
	GetDocument()->m_EditorFS_Subsystem.Name="";
	m_CenterX.SetWindowText("0.0000");
	GetDocument()->m_EditorFS_Subsystem.Center.x=0.0;
	m_CenterY.SetWindowText("0.0000");
	GetDocument()->m_EditorFS_Subsystem.Center.y=0.0;
	m_CenterZ.SetWindowText("0.0000");
	GetDocument()->m_EditorFS_Subsystem.Center.z=0.0;
	m_Radius.SetWindowText("0.0000");
	GetDocument()->m_EditorFS_Subsystem.Radius=0.0;
	m_Properties.SetCurSel(0);
	m_DataLocked=FALSE;
	
	GetMainView()->m_FS_DisplaySubsystem=-2;
	GetMainView()->InvalidateRect(NULL);
	m_IsNewItem=TRUE;
}

void CEditorFS_SPCL::DoCopyItem()
{
	if(m_List.GetCount()==0)
	{
		//Create new item instead
		DoNewItem();
		return;
	}

	//Add item to listbox
	m_List.AddString("<Copied item>");
	m_List.SetCurSel(m_List.GetCount()-1);

	//Prepare editor
	CString t;
	m_Name.GetWindowText(t);
	m_Name.SetWindowText(t+"_copy");
	UpdateEditorDisplay(TRUE);
	GetMainView()->InvalidateRect(NULL);
	m_IsNewItem=TRUE;
}

