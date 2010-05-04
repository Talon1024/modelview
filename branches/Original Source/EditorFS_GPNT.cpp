// EditorFS_GPNT.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "EditorFS_GPNT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_GPNT

IMPLEMENT_DYNCREATE(CEditorFS_GPNT, CFormView)

CEditorFS_GPNT::CEditorFS_GPNT()
	: CFormView(CEditorFS_GPNT::IDD)
{
	//{{AFX_DATA_INIT(CEditorFS_GPNT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_DataLocked=FALSE;
}

CEditorFS_GPNT::~CEditorFS_GPNT()
{
}

void CEditorFS_GPNT::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorFS_GPNT)
	DDX_Control(pDX, IDC_EDITORFS_GPNT_GUNMODE, m_GunMode);
	DDX_Control(pDX, IDC_EDITORFS_GPNT_LIST, m_List);
	DDX_Control(pDX, IDC_EDITORFS_GPNT_FIRINGPOINT_POSX, m_FiringPoint_PosX);
	DDX_Control(pDX, IDC_EDITORFS_GPNT_FIRINGPOINT_POSY, m_FiringPoint_PosY);
	DDX_Control(pDX, IDC_EDITORFS_GPNT_FIRINGPOINT_POSZ, m_FiringPoint_PosZ);
	DDX_Control(pDX, IDC_EDITORFS_GPNT_FIRINGPOINT_NORMX, m_FiringPoint_NormX);
	DDX_Control(pDX, IDC_EDITORFS_GPNT_FIRINGPOINT_NORMY, m_FiringPoint_NormY);
	DDX_Control(pDX, IDC_EDITORFS_GPNT_FIRINGPOINT_NORMZ, m_FiringPoint_NormZ);
	DDX_Control(pDX, IDC_EDITORFS_GPNT_FIRING_POINTS_LIST, m_FiringPoints_List);
	DDX_Control(pDX, IDC_EDITORFS_GPNT_CONVERT, m_Convert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorFS_GPNT, CFormView)
	//{{AFX_MSG_MAP(CEditorFS_GPNT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_EDITORFS_GPNT_GUNMODE, OnSelchangeGpntmode)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_EDITORFS_GPNT_LIST, OnSelchangeEditorfsGpntList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EDITORFS_GPNT_FIRING_POINTS_LIST, OnItemchangedEditorfsGpntFiringPointsList)
	ON_COMMAND_RANGE(ID_ELEMENTAPPLYER_APPLY, ID_ELEMENTAPPLYER_DISCARD, OnApplyerBarClick)
	ON_COMMAND_RANGE(ID_ELEMENTEDITOR_CREATE,ID_ELEMENTEDITOR_DELETE, OnEditorBarClick)
	ON_COMMAND_RANGE(ID_ELEMENTEDITOR2_CREATE,ID_ELEMENTEDITOR2_DELETE, OnEditor2BarClick)
	ON_EN_CHANGE(IDC_EDITORFS_GPNT_FIRINGPOINT_POSX, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_GPNT_FIRINGPOINT_POSY, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_GPNT_FIRINGPOINT_POSZ, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_GPNT_FIRINGPOINT_NORMX, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_GPNT_FIRINGPOINT_NORMY, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_GPNT_FIRINGPOINT_NORMZ, OnChangeData)
	ON_BN_CLICKED(IDC_EDITORFS_GPNT_CONVERT, OnConvert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_GPNT diagnostics

#ifdef _DEBUG
void CEditorFS_GPNT::AssertValid() const
{
	CFormView::AssertValid();
}

void CEditorFS_GPNT::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CMainFrame * CEditorFS_GPNT::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CEditorFS_GPNT::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CEditorFS_GPNT::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_GPNT message handlers

void CEditorFS_GPNT::AdaptEditorBox(int cx,CWnd *ed)
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

void CEditorFS_GPNT::AdaptVectorBox(int cx,CEdit *ed1,CEdit *ed2,CEdit *ed3)
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

void CEditorFS_GPNT::Init()
{
	InitGunMode();
}

BOOL CEditorFS_GPNT::UnInit()
{
	if(!FinishPendingEditing())
		return FALSE;
	GetMainView()->m_DisplayGun=-1;
	return TRUE;
}

BOOL CEditorFS_GPNT::FinishPendingEditing()
{
	if(m_AmEditing)
	{
		int a=MessageBox("Do you want to apply changes first?\n\nPress Yes to Apply, No to Discard, Cancel to do nothing)","Apply changes",MB_YESNOCANCEL);
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

void CEditorFS_GPNT::InitGunMode()
{
	int gunmode=m_GunMode.GetCurSel();
	ASSERT(gunmode==0 || gunmode==1);

	CString tp="Primary Gun";
	if(gunmode==1)
		tp="Secondary Gun";

	m_List.ResetContent();
	for(int i=0;i<GetDocument()->m_FS_RealGuns[gunmode].Num;i++)
	{
		CString x;
		x.Format("%s with %i slots",tp,GetDocument()->m_FS_RealGuns[gunmode].Slot[i].num_guns);
		m_List.AddString(x);
	}
	m_List.SetCurSel(0);
	OnSelchangeEditorfsGpntList();

	if(gunmode==0)
		m_Convert.SetWindowText("Convert gunpoint to Missile");
	else
		m_Convert.SetWindowText("Convert gunpoint to Gun");
	UpdateEditorDisplay(FALSE);
}

void CEditorFS_GPNT::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if(!::IsWindow(m_TBElements))
	{
		WINDOWPLACEMENT wndPlmnt;
		CStatic *placeHld;

		m_TBElements.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBElements.LoadToolBar(IDR_ELEMENTEDITOR);
		m_TBElements.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_GPNT_TOOLBARELEMENTS);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBElements.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		m_TBApplyer.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_AUTOSIZE, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_TBApplyer.LoadToolBar(IDR_ELEMENTAPPLYER);
		m_TBApplyer.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_TBApplyer.SetButtonText(0,"Apply");
		m_TBApplyer.SetButtonText(1,"Discard");
		GetMainFrame()->ResizeToolbar(&m_TBApplyer,TRUE);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_GPNT_TOOLBARAPPLYER);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBApplyer.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		m_TBFiringPoints.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBFiringPoints.LoadToolBar(IDR_ELEMENTEDITOR2);
		m_TBFiringPoints.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_GPNT_TOOLBARFIRINGPOINTS);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBFiringPoints.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

		m_TBApplyer.ShowWindow(FALSE);
	}

	m_GunMode.DeleteAllItems();
	m_GunMode.InsertItem(0,"Gun Slots");
	m_GunMode.InsertItem(1,"Missile Slots");

	m_FiringPoints_List.DeleteAllItems();
	for(int i=0;i<20;i++)
		m_FiringPoints_List.DeleteColumn(0);
	m_FiringPoints_List.InsertColumn(0,"#",LVCFMT_LEFT,15,0);
	m_FiringPoints_List.InsertColumn(1,"X",LVCFMT_RIGHT,60,1);
	m_FiringPoints_List.InsertColumn(2,"Y",LVCFMT_RIGHT,60,2);
	m_FiringPoints_List.InsertColumn(3,"Z",LVCFMT_RIGHT,60,3);
	m_FiringPoints_List.InsertColumn(4,"NormX",LVCFMT_RIGHT,60,1);
	m_FiringPoints_List.InsertColumn(5,"NormY",LVCFMT_RIGHT,60,2);
	m_FiringPoints_List.InsertColumn(6,"NormZ",LVCFMT_RIGHT,60,3);
	m_FiringPoints_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FLATSB);
}

void CEditorFS_GPNT::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (::IsWindow(m_hWnd))
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
		
	/*CStatic *head=(CStatic *)GetDlgItem(IDC_EDITORFS_GPNT_HEADER);
	if(head!=NULL)
	{
		if(::IsWindow(head->m_hWnd))
			head->SetWindowPos(NULL,1,2,cx-69,22,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}*/

	if(::IsWindow(m_TBElements.m_hWnd))
		m_TBElements.SetWindowPos(NULL,cx-69,0,0,0,SWP_NOSIZE | SWP_NOREDRAW | SWP_NOZORDER);
	if(::IsWindow(m_TBFiringPoints.m_hWnd))
		m_TBFiringPoints.SetWindowPos(NULL,cx-69,175,0,0,SWP_NOSIZE | SWP_NOREDRAW | SWP_NOZORDER);

	CTabCtrl *tc=(CTabCtrl *)GetDlgItem(IDC_EDITORFS_GPNT_GUNMODE);
	if(tc!=NULL)
	{
		if(::IsWindow(tc->m_hWnd))
			tc->SetWindowPos(NULL,1,2,cx-5,100/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	CListBox *lbox=(CListBox *)GetDlgItem(IDC_EDITORFS_GPNT_LIST);
	if(lbox!=NULL)
	{
		if(::IsWindow(lbox->m_hWnd))
			lbox->SetWindowPos(NULL,1,2,cx-16,70/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	CListCtrl *lviw=(CListCtrl *)GetDlgItem(IDC_EDITORFS_GPNT_FIRING_POINTS_LIST);
	if(lviw!=NULL)
	{
		if(::IsWindow(lviw->m_hWnd))
			lviw->SetWindowPos(NULL,1,2,cx-5,158/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	CStatic *cbtn=(CStatic *)GetDlgItem(IDC_EDITORFS_GPNT_CONVERT);
	if(cbtn!=NULL)
	{
		if(::IsWindow(cbtn->m_hWnd))
			cbtn->SetWindowPos(NULL,1,2,cx-5,24/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}

	AdaptVectorBox(cx,&m_FiringPoint_PosX,&m_FiringPoint_PosY,&m_FiringPoint_PosZ);
	AdaptVectorBox(cx,&m_FiringPoint_NormX,&m_FiringPoint_NormY,&m_FiringPoint_NormZ);
	InvalidateRect(NULL);
}

void CEditorFS_GPNT::OnSelchangeGpntmode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	InitGunMode();
	*pResult = 0;
}

void CEditorFS_GPNT::OnSelchangeEditorfsGpntList() 
{
	if(m_List.GetCount()==0)
	{
		GetMainView()->m_DisplayGun=-1;
		GetMainView()->InvalidateRect(NULL);
		return;
	}

	//Fill editor boxes
	m_DataLocked=TRUE;
	m_IsNewItem=FALSE;
	int m_CurrentItem=m_List.GetCurSel();
	m_DataLocked=FALSE;

	//Update display
	GetDocument()->m_EditorFS_RealGun=GetDocument()->m_FS_RealGuns[m_GunMode.GetCurSel()].Slot[m_CurrentItem];
	GetMainView()->m_DisplayGun=-4;
	UpdateFiringPointsList();
	GetMainView()->InvalidateRect(NULL);

	if(GetDocument()->m_EditorFS_RealGun.num_guns>0)
		m_FiringPoints_List.SetItemState(0,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);	
	OnSelchangeGpntFiringPointsList();
}

void CEditorFS_GPNT::OnSelchangeGpntFiringPointsList()
{
	m_DataLocked=TRUE;
	int i=GetCurrentFiringPointSelection();
	GetMainView()->m_EditorFS_GPNT_FiringPoint=i;

	CString fl;
	fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.point[i].x);
	m_FiringPoint_PosX.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.point[i].y);
	m_FiringPoint_PosY.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.point[i].z);
	m_FiringPoint_PosZ.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.normal[i].x);
	m_FiringPoint_NormX.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.normal[i].y);
	m_FiringPoint_NormY.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.normal[i].z);
	m_FiringPoint_NormZ.SetWindowText(fl);

	GetMainView()->InvalidateRect(NULL);
	m_DataLocked=FALSE;
}

int CEditorFS_GPNT::GetCurrentFiringPointSelection()
{
	POSITION pos=m_FiringPoints_List.GetFirstSelectedItemPosition();
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

void CEditorFS_GPNT::OnItemchangedEditorfsGpntFiringPointsList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	OnSelchangeGpntFiringPointsList();
	*pResult = 0;
}

void CEditorFS_GPNT::UpdateEditorDisplay(BOOL editing)
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

	m_FiringPoint_PosX.EnableWindow(ic);
	m_FiringPoint_PosY.EnableWindow(ic);
	m_FiringPoint_PosZ.EnableWindow(ic);
	m_FiringPoint_NormX.EnableWindow(ic);
	m_FiringPoint_NormY.EnableWindow(ic);
	m_FiringPoint_NormZ.EnableWindow(ic);
	m_FiringPoints_List.EnableWindow(ic);
}

void CEditorFS_GPNT::OnChangeData() 
{
	if(!m_DataLocked)
	{
		UpdateEditorDisplay(TRUE);
		
		//Read data

		//Read firing point data
		CString t;
		if(GetDocument()->m_EditorFS_RealGun.num_guns>0)
		{
			int f=GetCurrentFiringPointSelection();
			m_FiringPoint_PosX.GetWindowText(t);
			GetDocument()->m_EditorFS_RealGun.point[f].x=atof(t);
			m_FiringPoint_PosY.GetWindowText(t);
			GetDocument()->m_EditorFS_RealGun.point[f].y=atof(t);
			m_FiringPoint_PosZ.GetWindowText(t);
			GetDocument()->m_EditorFS_RealGun.point[f].z=atof(t);
			m_FiringPoint_NormX.GetWindowText(t);
			GetDocument()->m_EditorFS_RealGun.normal[f].x=atof(t);
			m_FiringPoint_NormY.GetWindowText(t);
			GetDocument()->m_EditorFS_RealGun.normal[f].y=atof(t);
			m_FiringPoint_NormZ.GetWindowText(t);
			GetDocument()->m_EditorFS_RealGun.normal[f].z=atof(t);
			//Update listview right away
			t.Format("%.4f",GetDocument()->m_EditorFS_RealGun.point[f].x);
			m_FiringPoints_List.SetItemText(f,1,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_RealGun.point[f].y);
			m_FiringPoints_List.SetItemText(f,2,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_RealGun.point[f].z);
			m_FiringPoints_List.SetItemText(f,3,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_RealGun.normal[f].x);
			m_FiringPoints_List.SetItemText(f,4,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_RealGun.normal[f].y);
			m_FiringPoints_List.SetItemText(f,5,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_RealGun.normal[f].z);
			m_FiringPoints_List.SetItemText(f,6,t);
		}

		GetMainView()->InvalidateRect(NULL);
	}
}

void CEditorFS_GPNT::OnApplyerBarClick(UINT nID)
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

void CEditorFS_GPNT::DoApply()
{
	int m_CurrentItem=m_List.GetCurSel();

	//Replace old item
	GetDocument()->m_FS_RealGuns[m_GunMode.GetCurSel()].Slot[m_CurrentItem]=GetDocument()->m_EditorFS_RealGun;
	CString x;
	CString tp="Primary Gun";
	if(m_GunMode.GetCurSel()==1)
		tp="Secondary Gun";
	x.Format("%s with %i slots",tp,GetDocument()->m_EditorFS_RealGun.num_guns);

	m_List.InsertString(m_CurrentItem,x);
	m_List.DeleteString(m_CurrentItem+1);
	m_List.SetCurSel(m_CurrentItem);

	//Handle new item
	if(m_IsNewItem)
		GetDocument()->m_FS_RealGuns[m_GunMode.GetCurSel()].Num++;

	m_IsNewItem=FALSE;
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	DoDiscard();
}

void CEditorFS_GPNT::DoDiscard()
{
	if(m_IsNewItem)
	{
		m_List.DeleteString(m_List.GetCount()-1);
		if(m_List.GetCount()!=0)
			m_List.SetCurSel(0);
	}
	UpdateEditorDisplay(FALSE);
	OnSelchangeEditorfsGpntList();
}

void CEditorFS_GPNT::OnEditorBarClick(UINT nID)
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

void CEditorFS_GPNT::OnEditor2BarClick(UINT nID)
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


void CEditorFS_GPNT::DoNewItem()
{
	//Prepare display
	m_List.AddString("<New item>");
	m_List.SetCurSel(m_List.GetCount()-1);
	UpdateEditorDisplay(TRUE);

	//Prepare Editor
	m_DataLocked=TRUE;
	GetDocument()->m_EditorFS_RealGun.num_guns=1;
	m_FiringPoint_PosX.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.point[0].x=0;
	m_FiringPoint_PosY.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.point[0].y=0;
	m_FiringPoint_PosZ.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.point[0].z=0;
	m_FiringPoint_NormX.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.normal[0].x=0;
	m_FiringPoint_NormY.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.normal[0].y=0;
	m_FiringPoint_NormZ.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.normal[0].z=0;
	UpdateFiringPointsList();
	m_FiringPoints_List.SetItemState(0,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
	m_DataLocked=FALSE;
	GetMainView()->m_DisplayGun=-4;	
	GetMainView()->InvalidateRect(NULL);
	m_IsNewItem=TRUE;
}

void CEditorFS_GPNT::DoDeleteItem()
{
	if(m_List.GetCurSel()<0 || m_List.GetCount()<=0)
		return;

	if(MessageBox("Are you sure you want to delete this item?","Confirmation",MB_YESNO)!=IDYES)
		return;

	int m_CurrentItem=m_List.GetCurSel();
	for(int i=m_CurrentItem+1;i<GetDocument()->m_FS_RealGuns[m_GunMode.GetCurSel()].Num;i++)
		GetDocument()->m_FS_RealGuns[m_GunMode.GetCurSel()].Slot[i-1]=GetDocument()->m_FS_RealGuns[m_GunMode.GetCurSel()].Slot[i];
	GetDocument()->m_FS_RealGuns[m_GunMode.GetCurSel()].Num--;

	//Remove from list
	m_List.DeleteString(m_CurrentItem);
	
	//Update display
	if(GetDocument()->m_FS_RealGuns[m_GunMode.GetCurSel()].Num==0)
	{
		m_FiringPoints_List.DeleteAllItems();
		m_FiringPoint_PosX.SetWindowText("");
		m_FiringPoint_PosY.SetWindowText("");
		m_FiringPoint_PosZ.SetWindowText("");
		m_FiringPoint_NormX.SetWindowText("");
		m_FiringPoint_NormY.SetWindowText("");
		m_FiringPoint_NormZ.SetWindowText("");
	}
	else if(GetDocument()->m_FS_RealGuns[m_GunMode.GetCurSel()].Num<=m_CurrentItem)
		m_List.SetCurSel(m_CurrentItem-1);
	else
		m_List.SetCurSel(m_CurrentItem);

	UpdateEditorDisplay(FALSE);
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	OnSelchangeEditorfsGpntList();
}

void CEditorFS_GPNT::DoCopyItem()
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

void CEditorFS_GPNT::OnConvert()
{
	if(m_List.GetCount()==0)
		return;

	int cgm=m_GunMode.GetCurSel();
	int agm=1-cgm;
	int ntm=GetDocument()->m_FS_RealGuns[agm].Num;

	//Create new item
	GetDocument()->m_FS_RealGuns[agm].Num++;
	GetDocument()->m_FS_RealGuns[agm].Slot[ntm]=GetDocument()->m_EditorFS_RealGun;

	//Delete old item
	int m_CurrentItem=m_List.GetCurSel();
	for(int i=m_CurrentItem+1;i<GetDocument()->m_FS_RealGuns[cgm].Num;i++)
		GetDocument()->m_FS_RealGuns[cgm].Slot[i-1]=GetDocument()->m_FS_RealGuns[cgm].Slot[i];
	GetDocument()->m_FS_RealGuns[cgm].Num--;

	//Update display
	m_GunMode.SetCurSel(agm);
	InitGunMode();
	m_List.SetCurSel(ntm);
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	OnSelchangeEditorfsGpntList();
}

void CEditorFS_GPNT::DoNewFiringPoint()
{
	if(GetDocument()->m_EditorFS_RealGun.num_guns>=MAX_FS_FIRING_POINTS)
	{
		MessageBox("Already too many firing points.","Error");
		return;
	}
	m_DataLocked=TRUE;
	int i=GetDocument()->m_EditorFS_RealGun.num_guns;
	GetDocument()->m_EditorFS_RealGun.num_guns++;
	m_FiringPoint_PosX.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.point[i].x=0.0;
	m_FiringPoint_PosY.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.point[i].y=0.0;
	m_FiringPoint_PosZ.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.point[i].z=0.0;
	m_FiringPoint_NormX.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.normal[i].x=0.0;
	m_FiringPoint_NormY.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.normal[i].y=0.0;
	m_FiringPoint_NormZ.SetWindowText("0.0");
	GetDocument()->m_EditorFS_RealGun.normal[i].z=0.0;
	m_DataLocked=FALSE;
	UpdateEditorDisplay(TRUE);
	UpdateFiringPointsList();
	m_FiringPoints_List.SetItemState(i,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
}

void CEditorFS_GPNT::DoDeleteFiringPoint()
{
	if(GetDocument()->m_EditorFS_RealGun.num_guns<2)
	{
		if(MessageBox("Cannot delete last gun slot. Do you want to delete the whole gun point instead?","Confirmation",MB_YESNO | MB_DEFBUTTON2)==IDYES)
			DoDeleteItem();
		return;
	}

	if(MessageBox("Are you sure you want to delete this firing point?","Confirmation",MB_YESNO)==IDNO)
		return;

	int i2=GetCurrentFiringPointSelection();
	for(int i=i2+1;i<GetDocument()->m_EditorFS_RealGun.num_guns;i++)
	{
		GetDocument()->m_EditorFS_RealGun.point[i-1]=GetDocument()->m_EditorFS_RealGun.point[i];
		GetDocument()->m_EditorFS_RealGun.normal[i-1]=GetDocument()->m_EditorFS_RealGun.normal[i];
	}
	GetDocument()->m_EditorFS_RealGun.num_guns--;
	UpdateEditorDisplay(TRUE);
	UpdateFiringPointsList();
	int sel=i2;
	if(GetDocument()->m_EditorFS_RealGun.num_guns<=sel)
		sel=GetDocument()->m_EditorFS_RealGun.num_guns-1;
	if(sel>=0)
		m_FiringPoints_List.SetItemState(sel,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);	
}

void CEditorFS_GPNT::DoCopyFiringPoint()
{
	if(GetDocument()->m_EditorFS_RealGun.num_guns>=MAX_FS_FIRING_POINTS)
	{
		MessageBox("Already too many firing points.","Error");
		return;
	}
	int i=GetDocument()->m_EditorFS_RealGun.num_guns;
	int i2=GetCurrentFiringPointSelection();
	GetDocument()->m_EditorFS_RealGun.num_guns++;
	GetDocument()->m_EditorFS_RealGun.point[i].x=GetDocument()->m_EditorFS_RealGun.point[i2].x;
	GetDocument()->m_EditorFS_RealGun.point[i].y=GetDocument()->m_EditorFS_RealGun.point[i2].y;
	GetDocument()->m_EditorFS_RealGun.point[i].z=GetDocument()->m_EditorFS_RealGun.point[i2].z;
	GetDocument()->m_EditorFS_RealGun.normal[i].x=GetDocument()->m_EditorFS_RealGun.normal[i2].x;
	GetDocument()->m_EditorFS_RealGun.normal[i].y=GetDocument()->m_EditorFS_RealGun.normal[i2].y;
	GetDocument()->m_EditorFS_RealGun.normal[i].z=GetDocument()->m_EditorFS_RealGun.normal[i2].z;
	UpdateEditorDisplay(TRUE);
	UpdateFiringPointsList();
	m_FiringPoints_List.SetItemState(i,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
}

void CEditorFS_GPNT::UpdateFiringPointsList()
{
	CString fl;
	m_FiringPoints_List.DeleteAllItems();
	for(int i=0;i<GetDocument()->m_EditorFS_RealGun.num_guns;i++)
	{
		char t2[256];
		itoa(i,t2,10);
		VERIFY(m_FiringPoints_List.InsertItem(i,t2)==i);
		fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.point[i].x);
		m_FiringPoints_List.SetItemText(i,1,fl);
		fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.point[i].y);
		m_FiringPoints_List.SetItemText(i,2,fl);
		fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.point[i].z);
		m_FiringPoints_List.SetItemText(i,3,fl);
		fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.normal[i].x);
		m_FiringPoints_List.SetItemText(i,4,fl);
		fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.normal[i].y);
		m_FiringPoints_List.SetItemText(i,5,fl);
		fl.Format("%.4f",GetDocument()->m_EditorFS_RealGun.normal[i].z);
		m_FiringPoints_List.SetItemText(i,6,fl);
	}
}




