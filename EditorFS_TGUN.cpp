// EditorFS_TGUN.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "EditorFS_TGUN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_TGUN

IMPLEMENT_DYNCREATE(CEditorFS_TGUN, CFormView)

CEditorFS_TGUN::CEditorFS_TGUN()
	: CFormView(CEditorFS_TGUN::IDD)
{
	//{{AFX_DATA_INIT(CEditorFS_TGUN)
	//}}AFX_DATA_INIT
	m_DataLocked=FALSE;
}

CEditorFS_TGUN::~CEditorFS_TGUN()
{
}

void CEditorFS_TGUN::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorFS_TGUN)
	DDX_Control(pDX, IDC_EDITORFS_TGUN_CONVERT, m_Convert);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_SOBJ_PARENT, m_SObj_Parent);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_SOBJ_PAR_PHYS, m_SObj_Parent_Phys);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_NORMAL_Z, m_Normal_Z);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_NORMAL_Y, m_Normal_Y);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_NORMAL_X, m_Normal_X);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_FIRINGPOINT_Y, m_FiringPoint_Y);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_FIRINGPOINT_Z, m_FiringPoint_Z);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_FIRINGPOINT_X, m_FiringPoint_X);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_FIRING_POINTS_LIST, m_FiringPoints_List);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_GUNMODE, m_GunMode);
	DDX_Control(pDX, IDC_EDITORFS_TGUN_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorFS_TGUN, CFormView)
	//{{AFX_MSG_MAP(CEditorFS_TGUN)
	ON_NOTIFY(TCN_SELCHANGE, IDC_EDITORFS_TGUN_GUNMODE, OnSelchangeTgunmode)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_EDITORFS_TGUN_LIST, OnSelchangeEditorfsTgunList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EDITORFS_TGUN_FIRING_POINTS_LIST, OnItemchangedEditorfsTgunFiringPointsList)
	ON_CBN_SELCHANGE(IDC_EDITORFS_TGUN_SOBJ_PARENT, OnChangeData)
	ON_CBN_SELCHANGE(IDC_EDITORFS_TGUN_SOBJ_PAR_PHYS, OnChangeData)
	ON_COMMAND_RANGE(ID_ELEMENTAPPLYER_APPLY, ID_ELEMENTAPPLYER_DISCARD, OnApplyerBarClick)
	ON_COMMAND_RANGE(ID_ELEMENTEDITOR_CREATE,ID_ELEMENTEDITOR_DELETE, OnEditorBarClick)
	ON_COMMAND_RANGE(ID_ELEMENTEDITOR2_CREATE,ID_ELEMENTEDITOR2_DELETE, OnEditor2BarClick)
	ON_EN_CHANGE(IDC_EDITORFS_TGUN_NORMAL_X, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_TGUN_NORMAL_Y, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_TGUN_NORMAL_Z, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_TGUN_FIRINGPOINT_X, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_TGUN_FIRINGPOINT_Y, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_TGUN_FIRINGPOINT_Z, OnChangeData)
	ON_BN_CLICKED(IDC_EDITORFS_TGUN_CONVERT, OnConvert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_TGUN diagnostics

#ifdef _DEBUG
void CEditorFS_TGUN::AssertValid() const
{
	CFormView::AssertValid();
}

void CEditorFS_TGUN::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CMainFrame * CEditorFS_TGUN::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CEditorFS_TGUN::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CEditorFS_TGUN::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_TGUN message handlers

void CEditorFS_TGUN::AdaptEditorBox(int cx,CWnd *ed)
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

void CEditorFS_TGUN::AdaptVectorBox(int cx,CEdit *ed1,CEdit *ed2,CEdit *ed3)
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

void CEditorFS_TGUN::Init()
{
	//Fill submodels
	m_SObj_Parent.ResetContent();
	m_SObj_Parent_Phys.ResetContent();
	for(int i=0;i<GetDocument()->m_FS_NumSOBJ;i++)
	{
		m_SObj_Parent.AddString(GetDocument()->m_FS_SOBJ[i].submodel_name);
		m_SObj_Parent_Phys.AddString(GetDocument()->m_FS_SOBJ[i].submodel_name);
	}

	InitGunMode();
}

BOOL CEditorFS_TGUN::UnInit()
{
	if(!FinishPendingEditing())
		return FALSE;
	GetMainView()->m_EditorFS_HighLight_Segment_1=-1;
	GetMainView()->m_EditorFS_HighLight_Segment_2=-1;
	GetMainView()->m_DisplayGun=-1;
	return TRUE;
}

BOOL CEditorFS_TGUN::FinishPendingEditing()
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

void CEditorFS_TGUN::OnSelchangeTgunmode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	InitGunMode();	
	*pResult = 0;
}

void CEditorFS_TGUN::InitGunMode()
{
	int gunmode=m_GunMode.GetCurSel();
	ASSERT(gunmode==0 || gunmode==1);

	m_List.ResetContent();
	for(int i=0;i<GetDocument()->m_FS_Turrets[gunmode].Num;i++)
	{
		CString x;
		x.Format("%ix in submodel %s",GetDocument()->m_FS_Turrets[gunmode].Bank[i].num_firing_points,GetDocument()->m_FS_SOBJ[GetDocument()->m_FS_Turrets[gunmode].Bank[i].sobj_parent].submodel_name);
		m_List.AddString(x);
	}
	m_List.SetCurSel(0);
	OnSelchangeEditorfsTgunList();

	if(gunmode==0)
		m_Convert.SetWindowText("Convert this turret to Missile");
	else
		m_Convert.SetWindowText("Convert this turret to Gun");
	UpdateEditorDisplay(FALSE);
}

void CEditorFS_TGUN::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if(!::IsWindow(m_TBElements))
	{
		WINDOWPLACEMENT wndPlmnt;
		CStatic *placeHld;

		m_TBElements.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBElements.LoadToolBar(IDR_ELEMENTEDITOR);
		m_TBElements.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_TGUN_TOOLBARELEMENTS);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBElements.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		m_TBApplyer.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_AUTOSIZE, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_TBApplyer.LoadToolBar(IDR_ELEMENTAPPLYER);
		m_TBApplyer.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_TBApplyer.SetButtonText(0,"Apply");
		m_TBApplyer.SetButtonText(1,"Discard");
		GetMainFrame()->ResizeToolbar(&m_TBApplyer,TRUE);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_TGUN_TOOLBARAPPLYER);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBApplyer.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		m_TBFiringPoints.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBFiringPoints.LoadToolBar(IDR_ELEMENTEDITOR2);
		m_TBFiringPoints.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_TGUN_TOOLBARFIRINGPOINTS);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBFiringPoints.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

		m_TBApplyer.ShowWindow(FALSE);
	}	

	m_GunMode.DeleteAllItems();
	m_GunMode.InsertItem(0,"Gun Turrets");
	m_GunMode.InsertItem(1,"Missile Turrets");

	m_FiringPoints_List.DeleteAllItems();
	for(int i=0;i<20;i++)
		m_FiringPoints_List.DeleteColumn(0);
	m_FiringPoints_List.InsertColumn(0,"#",LVCFMT_LEFT,15,0);
	m_FiringPoints_List.InsertColumn(1,"X",LVCFMT_RIGHT,60,1);
	m_FiringPoints_List.InsertColumn(2,"Y",LVCFMT_RIGHT,60,2);
	m_FiringPoints_List.InsertColumn(3,"Z",LVCFMT_RIGHT,60,3);
	m_FiringPoints_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FLATSB);
}

void CEditorFS_TGUN::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (::IsWindow(m_hWnd))
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
		
	/*CStatic *head=(CStatic *)GetDlgItem(IDC_EDITORFS_TGUN_HEADER);
	if(head!=NULL)
	{
		if(::IsWindow(head->m_hWnd))
			head->SetWindowPos(NULL,1,2,cx-69,22,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}*/

	if(::IsWindow(m_TBElements.m_hWnd))
		m_TBElements.SetWindowPos(NULL,cx-69,0,0,0,SWP_NOSIZE | SWP_NOREDRAW | SWP_NOZORDER);
	if(::IsWindow(m_TBFiringPoints.m_hWnd))
		m_TBFiringPoints.SetWindowPos(NULL,cx-69,326,0,0,SWP_NOSIZE | SWP_NOREDRAW | SWP_NOZORDER);

	CTabCtrl *tc=(CTabCtrl *)GetDlgItem(IDC_EDITORFS_TGUN_GUNMODE);
	if(tc!=NULL)
	{
		if(::IsWindow(tc->m_hWnd))
			tc->SetWindowPos(NULL,1,2,cx-5,180/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	CListBox *lbox=(CListBox *)GetDlgItem(IDC_EDITORFS_TGUN_LIST);
	if(lbox!=NULL)
	{
		if(::IsWindow(lbox->m_hWnd))
			lbox->SetWindowPos(NULL,1,2,cx-16,150/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	CListCtrl *lviw=(CListCtrl *)GetDlgItem(IDC_EDITORFS_TGUN_FIRING_POINTS_LIST);
	if(lviw!=NULL)
	{
		if(::IsWindow(lviw->m_hWnd))
			lviw->SetWindowPos(NULL,1,2,cx-5,78/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	CStatic *cbtn=(CStatic *)GetDlgItem(IDC_EDITORFS_TGUN_CONVERT);
	if(cbtn!=NULL)
	{
		if(::IsWindow(cbtn->m_hWnd))
			cbtn->SetWindowPos(NULL,1,2,cx-5,24/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}

	AdaptEditorBox(cx,&m_SObj_Parent);
	AdaptEditorBox(cx,&m_SObj_Parent_Phys);
	//AdaptEditorBox(cx,&m_Radius);
	AdaptVectorBox(cx,&m_Normal_X,&m_Normal_Y,&m_Normal_Z);
	AdaptVectorBox(cx,&m_FiringPoint_X,&m_FiringPoint_Y,&m_FiringPoint_Z);
	InvalidateRect(NULL);
}

void CEditorFS_TGUN::OnSelchangeEditorfsTgunList() 
{
	if(m_List.GetCount()==0)
	{
		GetMainView()->m_EditorFS_HighLight_Segment_1=-1;
		GetMainView()->m_EditorFS_HighLight_Segment_2=-1;
		GetMainView()->m_DisplayGun=-1;
		GetMainView()->InvalidateRect(NULL);
		return;
	}

	//Fill editor boxes
	m_DataLocked=TRUE;
	m_IsNewItem=FALSE;
	int m_CurrentItem=m_List.GetCurSel();
	CString fl;
	fl.Format("%.4f",GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[m_CurrentItem].turret_normal.x);
	m_Normal_X.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[m_CurrentItem].turret_normal.y);
	m_Normal_Y.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[m_CurrentItem].turret_normal.z);
	m_Normal_Z.SetWindowText(fl);
	m_SObj_Parent.SetCurSel(GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[m_CurrentItem].sobj_parent);
	m_SObj_Parent_Phys.SetCurSel(GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[m_CurrentItem].sobj_par_phys);
	m_DataLocked=FALSE;

	//Update display
	GetMainView()->m_EditorFS_HighLight_Segment_1=GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[m_CurrentItem].sobj_parent;
	GetMainView()->m_EditorFS_HighLight_Segment_2=GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[m_CurrentItem].sobj_par_phys;
	GetDocument()->m_EditorFS_Turret=GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[m_CurrentItem];
	GetMainView()->m_DisplayGun=-3;
	UpdateFiringPointsList();
	GetMainView()->InvalidateRect(NULL);

	if(GetDocument()->m_EditorFS_Turret.num_firing_points>0)
		m_FiringPoints_List.SetItemState(0,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);	
	OnSelchangeTgunFiringPointsList();
}

void CEditorFS_TGUN::OnItemchangedEditorfsTgunFiringPointsList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	OnSelchangeTgunFiringPointsList();
	*pResult = 0;
}

int CEditorFS_TGUN::GetCurrentFiringPointSelection()
{
	POSITION pos=m_FiringPoints_List.GetFirstSelectedItemPosition();
	int m_CurrentItem=m_List.GetCurSel();
	int i=m_FiringPoints_List.GetNextSelectedItem(pos);
	if(i==-1) //if nothing is selected we default to item #1
		i=0;
	if(i>MAX_FS_FIRING_POINTS)
	{
		i=MAX_FS_FIRING_POINTS-1;
		ASSERT(FALSE);
	}
	return i;
}

void CEditorFS_TGUN::OnSelchangeTgunFiringPointsList()
{
	m_DataLocked=TRUE;
	int m_CurrentItem=m_List.GetCurSel();
	int i=GetCurrentFiringPointSelection();
	GetMainView()->m_EditorFS_TGUN_FiringPoint=i;

	CString fl;
	fl.Format("%.4f",GetDocument()->m_EditorFS_Turret.firing_point[i].x);
	m_FiringPoint_X.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_Turret.firing_point[i].y);
	m_FiringPoint_Y.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_Turret.firing_point[i].z);
	m_FiringPoint_Z.SetWindowText(fl);

	GetMainView()->InvalidateRect(NULL);
	m_DataLocked=FALSE;
}

void CEditorFS_TGUN::UpdateEditorDisplay(BOOL editing)
{
	//If there is at least one item
	m_GunMode.EnableWindow(!editing);
	m_AmEditing=editing;
	BOOL ic=(m_List.GetCount()>0);
	if(ic)
	{
		m_DataChanged=editing;
		m_List.EnableWindow(!editing);
		m_TBElements.ShowWindow(!editing);
		m_TBApplyer.ShowWindow(editing);
		m_Convert.EnableWindow(!editing);
	}
	else
	{
		m_TBElements.ShowWindow(TRUE);
		m_TBApplyer.ShowWindow(FALSE);
		m_Convert.EnableWindow(FALSE);
	}

	m_Normal_X.EnableWindow(ic);
	m_Normal_Y.EnableWindow(ic);
	m_Normal_Z.EnableWindow(ic);
	m_FiringPoint_X.EnableWindow(ic);
	m_FiringPoint_Y.EnableWindow(ic);
	m_FiringPoint_Z.EnableWindow(ic);
	m_FiringPoints_List.EnableWindow(ic);
	m_SObj_Parent.EnableWindow(ic);
	m_SObj_Parent_Phys.EnableWindow(ic);
}

void CEditorFS_TGUN::OnChangeData() 
{
	if(!m_DataLocked)
	{
		UpdateEditorDisplay(TRUE);
		
		//Read data
		GetDocument()->m_EditorFS_Turret.sobj_parent=m_SObj_Parent.GetCurSel();
		GetDocument()->m_EditorFS_Turret.sobj_par_phys=m_SObj_Parent_Phys.GetCurSel();
		GetMainView()->m_EditorFS_HighLight_Segment_1=m_SObj_Parent.GetCurSel();
		GetMainView()->m_EditorFS_HighLight_Segment_2=m_SObj_Parent_Phys.GetCurSel();
		CString t;
		m_Normal_X.GetWindowText(t);
		GetDocument()->m_EditorFS_Turret.turret_normal.x=(float)atof(t);
		m_Normal_Y.GetWindowText(t);
		GetDocument()->m_EditorFS_Turret.turret_normal.y=(float)atof(t);
		m_Normal_Z.GetWindowText(t);
		GetDocument()->m_EditorFS_Turret.turret_normal.z=(float)atof(t);

		//Read firing point data
		if(GetDocument()->m_EditorFS_Turret.num_firing_points>0)
		{
			int f=GetCurrentFiringPointSelection();
			m_FiringPoint_X.GetWindowText(t);
			GetDocument()->m_EditorFS_Turret.firing_point[f].x=(float)atof(t);
			m_FiringPoint_Y.GetWindowText(t);
			GetDocument()->m_EditorFS_Turret.firing_point[f].y=(float)atof(t);
			m_FiringPoint_Z.GetWindowText(t);
			GetDocument()->m_EditorFS_Turret.firing_point[f].z=(float)atof(t);
			//Update listview right away
			t.Format("%.4f",GetDocument()->m_EditorFS_Turret.firing_point[f].x);
			m_FiringPoints_List.SetItemText(f,1,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_Turret.firing_point[f].y);
			m_FiringPoints_List.SetItemText(f,2,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_Turret.firing_point[f].z);
			m_FiringPoints_List.SetItemText(f,3,t);
		}
		
		GetMainView()->InvalidateRect(NULL);
	}
}

void CEditorFS_TGUN::OnApplyerBarClick(UINT nID)
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

void CEditorFS_TGUN::DoApply()
{
	int m_CurrentItem=m_List.GetCurSel();

	//Replace old item
	GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[m_CurrentItem]=GetDocument()->m_EditorFS_Turret;
	CString x;
	x.Format("%ix in submodel %s",GetDocument()->m_EditorFS_Turret.num_firing_points,GetDocument()->m_FS_SOBJ[GetDocument()->m_EditorFS_Turret.sobj_parent].submodel_name);

	m_List.InsertString(m_CurrentItem,x);
	m_List.DeleteString(m_CurrentItem+1);
	m_List.SetCurSel(m_CurrentItem);

	//Handle new item
	if(m_IsNewItem)
		GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Num++;

	m_IsNewItem=FALSE;
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	DoDiscard();
}

void CEditorFS_TGUN::DoDiscard()
{
	if(m_IsNewItem)
	{
		m_List.DeleteString(m_List.GetCount()-1);
		if(m_List.GetCount()!=0)
			m_List.SetCurSel(0);
	}
	UpdateEditorDisplay(FALSE);
	OnSelchangeEditorfsTgunList();
}

void CEditorFS_TGUN::OnEditorBarClick(UINT nID)
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

void CEditorFS_TGUN::OnEditor2BarClick(UINT nID)
{
	if(m_List.GetCurSel()<0 || m_List.GetCount()<=0)
		return;

	switch(nID)
	{
	case ID_ELEMENTEDITOR2_CREATE:
		DoNewFiringPoint();
		break;

	case ID_ELEMENTEDITOR2_DELETE:
		DoDeleteFiringPoint();
		break;

	case ID_ELEMENTEDITOR2_COPY:
		DoCopyFiringPoint();
		break;
	
	default:
		ASSERT(FALSE);
		AfxAbort(); //"OnEditor2BarClick with unknown nID");
	}
}


void CEditorFS_TGUN::DoNewItem()
{
	//Prepare display
	m_List.AddString("<New item>");
	m_List.SetCurSel(m_List.GetCount()-1);
	UpdateEditorDisplay(TRUE);

	//Prepare Editor
	m_DataLocked=TRUE;
	m_SObj_Parent.SetCurSel(0);
	GetDocument()->m_EditorFS_Turret.sobj_parent=0;
	m_SObj_Parent_Phys.SetCurSel(0);
	GetDocument()->m_EditorFS_Turret.sobj_par_phys=0;
	m_Normal_X.SetWindowText("0.0");
	GetDocument()->m_EditorFS_Turret.turret_normal.x=0;
	m_Normal_Y.SetWindowText("0.0");
	GetDocument()->m_EditorFS_Turret.turret_normal.y=0;
	m_Normal_Z.SetWindowText("0.0");
	GetDocument()->m_EditorFS_Turret.turret_normal.z=0;

	GetDocument()->m_EditorFS_Turret.num_firing_points=1;
	m_FiringPoint_X.SetWindowText("0.0");
	GetDocument()->m_EditorFS_Turret.firing_point[0].x=0;
	m_FiringPoint_Y.SetWindowText("0.0");
	GetDocument()->m_EditorFS_Turret.firing_point[0].y=0;
	m_FiringPoint_Z.SetWindowText("0.0");
	GetDocument()->m_EditorFS_Turret.firing_point[0].z=0;
	UpdateFiringPointsList();
	m_FiringPoints_List.SetItemState(0,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
	m_DataLocked=FALSE;
	GetMainView()->m_DisplayGun=-3;
	GetMainView()->InvalidateRect(NULL);
	m_IsNewItem=TRUE;
}

void CEditorFS_TGUN::DoDeleteItem()
{
	if(m_List.GetCurSel()<0 || m_List.GetCount()<=0)
		return;

	if(MessageBox("Are you sure you want to delete this item?","Confirmation",MB_YESNO)!=IDYES)
		return;

	int m_CurrentItem = m_List.GetCurSel();
	for(int i = m_CurrentItem + 1; i < GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Num; i++)
		GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[i-1] = GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Bank[i];
	GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Num--;

	//Remove from list
	m_List.DeleteString(m_CurrentItem);
	
	//Update display
	if(GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Num==0)
	{
		m_Normal_X.SetWindowText("");
		m_Normal_Y.SetWindowText("");
		m_Normal_Z.SetWindowText("");
		m_FiringPoints_List.DeleteAllItems();
		m_FiringPoint_X.SetWindowText("");
		m_FiringPoint_Y.SetWindowText("");
		m_FiringPoint_Z.SetWindowText("");
	}
	else if(GetDocument()->m_FS_Turrets[m_GunMode.GetCurSel()].Num<=m_CurrentItem)
		m_List.SetCurSel(m_CurrentItem-1);
	else
		m_List.SetCurSel(m_CurrentItem);

	UpdateEditorDisplay(FALSE);
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	OnSelchangeEditorfsTgunList();
}

void CEditorFS_TGUN::DoCopyItem()
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
	UpdateEditorDisplay(TRUE);
	GetMainView()->InvalidateRect(NULL);
	m_IsNewItem=TRUE;
}

void CEditorFS_TGUN::OnConvert()
{
	if(m_List.GetCount()==0)
		return;

	int cgm=m_GunMode.GetCurSel();
	int agm=1-cgm;
	int ntm=GetDocument()->m_FS_Turrets[agm].Num;

	//Create new item
	GetDocument()->m_FS_Turrets[agm].Num++;
	GetDocument()->m_FS_Turrets[agm].Bank[ntm]=GetDocument()->m_EditorFS_Turret;

	//Delete old item
	int m_CurrentItem=m_List.GetCurSel();
	for(int i=m_CurrentItem+1;i<GetDocument()->m_FS_Turrets[cgm].Num;i++)
		GetDocument()->m_FS_Turrets[cgm].Bank[i-1]=GetDocument()->m_FS_Turrets[cgm].Bank[i];
	GetDocument()->m_FS_Turrets[cgm].Num--;

	//Update display
	m_GunMode.SetCurSel(agm);
	InitGunMode();
	m_List.SetCurSel(ntm);
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	OnSelchangeEditorfsTgunList();
}

void CEditorFS_TGUN::DoNewFiringPoint()
{
	if(GetDocument()->m_EditorFS_Turret.num_firing_points>=MAX_FS_FIRING_POINTS)
	{
		MessageBox("Already too many firing points.","Error");
		return;
	}
	m_DataLocked=TRUE;
	int i=GetDocument()->m_EditorFS_Turret.num_firing_points;
	GetDocument()->m_EditorFS_Turret.num_firing_points++;
	m_FiringPoint_X.SetWindowText("0.0");
	GetDocument()->m_EditorFS_Turret.firing_point[i].x=0;
	m_FiringPoint_Y.SetWindowText("0.0");
	GetDocument()->m_EditorFS_Turret.firing_point[i].y=0;
	m_FiringPoint_Z.SetWindowText("0.0");
	GetDocument()->m_EditorFS_Turret.firing_point[i].z=0;
	m_DataLocked=FALSE;
	UpdateEditorDisplay(TRUE);
	UpdateFiringPointsList();
	m_FiringPoints_List.SetItemState(i,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
}

void CEditorFS_TGUN::DoDeleteFiringPoint()
{
	if(GetDocument()->m_EditorFS_Turret.num_firing_points<2)
	{
		if(MessageBox("Cannot delete last firing point. Do you want to delete the whole turret instead?","Confirmation",MB_YESNO | MB_DEFBUTTON2)==IDYES)
			DoDeleteItem();
		return;
	}

	if(MessageBox("Are you sure you want to delete this firing point?","Confirmation",MB_YESNO)==IDNO)
		return;

	int i2=GetCurrentFiringPointSelection();
	for(int i=i2+1;i<GetDocument()->m_EditorFS_Turret.num_firing_points;i++)
		GetDocument()->m_EditorFS_Turret.firing_point[i-1]=GetDocument()->m_EditorFS_Turret.firing_point[i];
	GetDocument()->m_EditorFS_Turret.num_firing_points--;
	UpdateEditorDisplay(TRUE);
	UpdateFiringPointsList();
	int sel=i2;
	if(GetDocument()->m_EditorFS_Turret.num_firing_points<=sel)
		sel=GetDocument()->m_EditorFS_Turret.num_firing_points-1;
	if(sel>=0)
		m_FiringPoints_List.SetItemState(sel,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
}

void CEditorFS_TGUN::DoCopyFiringPoint()
{
	if(GetDocument()->m_EditorFS_Turret.num_firing_points>=MAX_FS_FIRING_POINTS)
	{
		MessageBox("Already too many firing points.","Error");
		return;
	}
	int i=GetDocument()->m_EditorFS_Turret.num_firing_points;
	int i2=GetCurrentFiringPointSelection();
	GetDocument()->m_EditorFS_Turret.num_firing_points++;
	GetDocument()->m_EditorFS_Turret.firing_point[i].x=GetDocument()->m_EditorFS_Turret.firing_point[i2].x;
	GetDocument()->m_EditorFS_Turret.firing_point[i].y=GetDocument()->m_EditorFS_Turret.firing_point[i2].y;
	GetDocument()->m_EditorFS_Turret.firing_point[i].z=GetDocument()->m_EditorFS_Turret.firing_point[i2].z;
	UpdateEditorDisplay(TRUE);
	UpdateFiringPointsList();
	m_FiringPoints_List.SetItemState(i,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
}

void CEditorFS_TGUN::UpdateFiringPointsList()
{
	CString fl;
	m_FiringPoints_List.DeleteAllItems();
	for(int i=0;i<GetDocument()->m_EditorFS_Turret.num_firing_points;i++)
	{
		char t2[256];
		_itoa(i,t2,10);
		VERIFY(m_FiringPoints_List.InsertItem(i,t2)==i);
		fl.Format("%.4f",GetDocument()->m_EditorFS_Turret.firing_point[i].x);
		m_FiringPoints_List.SetItemText(i,1,fl);
		fl.Format("%.4f",GetDocument()->m_EditorFS_Turret.firing_point[i].y);
		m_FiringPoints_List.SetItemText(i,2,fl);
		fl.Format("%.4f",GetDocument()->m_EditorFS_Turret.firing_point[i].z);
		m_FiringPoints_List.SetItemText(i,3,fl);
	}
}



