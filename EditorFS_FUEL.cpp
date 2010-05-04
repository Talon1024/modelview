// EditorFS_FUEL.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "EditorFS_FUEL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_FUEL

IMPLEMENT_DYNCREATE(CEditorFS_FUEL, CFormView)

CEditorFS_FUEL::CEditorFS_FUEL()
	: CFormView(CEditorFS_FUEL::IDD)
{
	//{{AFX_DATA_INIT(CEditorFS_FUEL)
	//}}AFX_DATA_INIT
}

CEditorFS_FUEL::~CEditorFS_FUEL()
{
}

void CEditorFS_FUEL::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorFS_FUEL)
	DDX_Control(pDX, IDC_EDITORFS_FUEL_LIST, m_List);
	DDX_Control(pDX, IDC_EDITORFS_FUEL_POSITION_Z, m_PositionZ);
	DDX_Control(pDX, IDC_EDITORFS_FUEL_POSITION_Y, m_PositionY);
	DDX_Control(pDX, IDC_EDITORFS_FUEL_POSITION_X, m_PositionX);
	DDX_Control(pDX, IDC_EDITORFS_FUEL_NORMAL_Z, m_NormalZ);
	DDX_Control(pDX, IDC_EDITORFS_FUEL_NORMAL_Y, m_NormalY);
	DDX_Control(pDX, IDC_EDITORFS_FUEL_NORMAL_X, m_NormalX);
	DDX_Control(pDX, IDC_EDITORFS_FUEL_RADIUS, m_Radius);
	DDX_Control(pDX, IDC_EDITORFS_FUEL_PROPERTIES, m_Properties);
	DDX_Control(pDX, IDC_EDITORFS_FUEL_GLOWLIST, m_GlowList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorFS_FUEL, CFormView)
	//{{AFX_MSG_MAP(CEditorFS_FUEL)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_EDITORFS_FUEL_LIST, OnSelchangeEditorfsFuelList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EDITORFS_FUEL_GLOWLIST, OnItemchangedEditorfsFuelGlowlist)
	ON_EN_CHANGE(IDC_EDITORFS_FUEL_PROPERTIES, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_FUEL_POSITION_X, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_FUEL_POSITION_Y, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_FUEL_POSITION_Z, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_FUEL_NORMAL_X, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_FUEL_NORMAL_Y, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_FUEL_NORMAL_Z, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_FUEL_RADIUS, OnChangeData)
	ON_COMMAND_RANGE(ID_ELEMENTAPPLYER_APPLY, ID_ELEMENTAPPLYER_DISCARD, OnApplyerBarClick)
	ON_COMMAND_RANGE(ID_ELEMENTEDITOR_CREATE,ID_ELEMENTEDITOR_PASTE, OnEditorBarClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_FUEL diagnostics

#ifdef _DEBUG
void CEditorFS_FUEL::AssertValid() const
{
	CFormView::AssertValid();
}

void CEditorFS_FUEL::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CMainFrame * CEditorFS_FUEL::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CEditorFS_FUEL::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CEditorFS_FUEL::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_FUEL message handlers

void CEditorFS_FUEL::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if(!::IsWindow(m_TBElements))
	{
		WINDOWPLACEMENT wndPlmnt;
		CStatic *placeHld;

		m_TBElements.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBElements.LoadToolBar(IDR_ELEMENTEDITOR);
		m_TBElements.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_FUEL_TOOLBARELEMENTS);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBElements.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		m_TBApplyer.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_AUTOSIZE, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_TBApplyer.LoadToolBar(IDR_ELEMENTAPPLYER);
		m_TBApplyer.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
		m_TBApplyer.SetButtonText(0,"Apply");
		m_TBApplyer.SetButtonText(1,"Discard");
		GetMainFrame()->ResizeToolbar(&m_TBApplyer,TRUE);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_FUEL_TOOLBARAPPLYER);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBApplyer.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		m_TBGlowPoints.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBGlowPoints.LoadToolBar(IDR_ELEMENTEDITOR2);
		m_TBGlowPoints.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_FUEL_TOOLBARGLOWPOINTS);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBGlowPoints.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();

		m_TBApplyer.ShowWindow(FALSE);

		m_GlowList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		m_GlowList.InsertColumn(0,"#",LVCFMT_LEFT,18);
		m_GlowList.InsertColumn(1,"X",LVCFMT_LEFT,60);
		m_GlowList.InsertColumn(2,"Y",LVCFMT_LEFT,60);
		m_GlowList.InsertColumn(3,"Z",LVCFMT_LEFT,60);
		m_GlowList.InsertColumn(4,"NormX",LVCFMT_LEFT,60);
		m_GlowList.InsertColumn(5,"NormY",LVCFMT_LEFT,60);
		m_GlowList.InsertColumn(6,"NormZ",LVCFMT_LEFT,60);
		m_GlowList.InsertColumn(7,"Rad",LVCFMT_LEFT,60);
	}
}

void CEditorFS_FUEL::OnSize(UINT nType, int cx, int cy) 
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
	if(::IsWindow(m_TBGlowPoints.m_hWnd))
		m_TBGlowPoints.SetWindowPos(NULL,cx-69,191,0,0,SWP_NOSIZE | SWP_NOREDRAW | SWP_NOZORDER);

	CListBox *lbox=(CListBox *)GetDlgItem(IDC_EDITORFS_FUEL_LIST);
	if(lbox!=NULL)
	{
		if(::IsWindow(lbox->m_hWnd))
			lbox->SetWindowPos(NULL,1,2,cx-2,100/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	
	CListCtrl *lctr=(CListCtrl *)GetDlgItem(IDC_EDITORFS_FUEL_GLOWLIST);
	if(lctr!=NULL)
	{
		if(::IsWindow(lctr->m_hWnd))
			lctr->SetWindowPos(NULL,1,2,cx-2,120/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}

	AdaptEditorBox(cx,&m_Properties);
	AdaptEditorBox(cx,&m_Radius);
	AdaptVectorBox(cx,&m_PositionX,&m_PositionY,&m_PositionZ);
	AdaptVectorBox(cx,&m_NormalX,&m_NormalY,&m_NormalZ);
	InvalidateRect(NULL);
}

void CEditorFS_FUEL::AdaptEditorBox(int cx,CWnd *ed)
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

void CEditorFS_FUEL::AdaptVectorBox(int cx,CEdit *ed1,CEdit *ed2,CEdit *ed3)
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

void CEditorFS_FUEL::Init()
{
	CMODVIEW32Doc *pDoc=GetDocument();
		
	m_List.ResetContent();
	for(int i=0;i<pDoc->m_FS_Model.num_thrusters;i++)
	{
		CString x;
		if(pDoc->m_FS_Model.thrusters[i].properties=="")
			x.Format("Thruster #%i",i);
		else
			x.Format("Thruster #%i: %s",i,pDoc->m_FS_Model.thrusters[i].properties);
		m_List.AddString(x);
	}
	m_List.SetCurSel(0);
	m_AmEditing=FALSE;
	OnSelchangeEditorfsFuelList();
	UpdateEditorDisplay(FALSE);
}

BOOL CEditorFS_FUEL::UnInit()
{
	if(!FinishPendingEditing())
		return FALSE;
	GetMainView()->m_FS_DisplayThruster=-1;
	return TRUE;
}

BOOL CEditorFS_FUEL::FinishPendingEditing()
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

void CEditorFS_FUEL::OnSelchangeEditorfsFuelList() 
{
	if(m_List.GetCount()==0)
	{
		//GetMainView()->m_DisplayGun=-1;
		GetMainView()->InvalidateRect(NULL);
		return;
	}

	//Fill editor boxes
	m_DataLocked=TRUE;
	m_IsNewItem=FALSE;
	int m_CurrentItem=m_List.GetCurSel();
	m_Properties.SetWindowText(GetDocument()->m_FS_Model.thrusters[m_CurrentItem].properties);
	m_DataLocked=FALSE;

	//Update display
	GetMainView()->m_FS_DisplayThruster=m_CurrentItem;
	GetDocument()->m_EditorFS_FUEL=GetDocument()->m_FS_Model.thrusters[m_CurrentItem];
	UpdateGlowPointsList();
	GetMainView()->InvalidateRect(NULL);
	if(GetDocument()->m_EditorFS_FUEL.num_glows>0)
		m_GlowList.SetItemState(0,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);	
	OnSelchangeEditorfsGlowList();
}

void CEditorFS_FUEL::OnItemchangedEditorfsFuelGlowlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	OnSelchangeEditorfsGlowList();
	*pResult = 0;
}

void CEditorFS_FUEL::OnSelchangeEditorfsGlowList()
{
	m_DataLocked=TRUE;
	int i=GetCurrentGlowListSelection();
	GetMainView()->m_EditorFS_GPNT_FiringPoint=i;

	CString fl;
	fl.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_pos[i].x);
	m_PositionX.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_pos[i].y);
	m_PositionY.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_pos[i].z);
	m_PositionZ.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_norm[i].x);
	m_NormalX.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_norm[i].y);
	m_NormalY.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_norm[i].z);
	m_NormalZ.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_radius[i]);
	m_Radius.SetWindowText(fl);

	GetMainView()->InvalidateRect(NULL);
	m_DataLocked=FALSE;

}

int CEditorFS_FUEL::GetCurrentGlowListSelection()
{
	POSITION pos=m_GlowList.GetFirstSelectedItemPosition();
	int i=m_GlowList.GetNextSelectedItem(pos);
	if(i==-1) //if nothing is selected we default to item #1
		i=0;
	if(i>MAX_FS_FIRING_POINTS)
	{
		i=MAX_FS_FIRING_POINTS-1;
		ASSERT(FALSE);
	}
	return i;
}

void CEditorFS_FUEL::UpdateGlowPointsList()
{
	int sel=m_List.GetCurSel();

	m_GlowList.DeleteAllItems();
	for(int i=0;i<GetDocument()->m_FS_Model.thrusters[sel].num_glows;i++)
	{
		CString x;
		x.Format("%i",i);
		VERIFY(m_GlowList.InsertItem(i,x)==i);
		x.Format("%.4f",GetDocument()->m_FS_Model.thrusters[sel].glow_pos[i].x);
		m_GlowList.SetItemText(i,1,x);
		x.Format("%.4f",GetDocument()->m_FS_Model.thrusters[sel].glow_pos[i].y);
		m_GlowList.SetItemText(i,2,x);
		x.Format("%.4f",GetDocument()->m_FS_Model.thrusters[sel].glow_pos[i].z);
		m_GlowList.SetItemText(i,3,x);
		x.Format("%.4f",GetDocument()->m_FS_Model.thrusters[sel].glow_norm[i].x);
		m_GlowList.SetItemText(i,4,x);
		x.Format("%.4f",GetDocument()->m_FS_Model.thrusters[sel].glow_norm[i].y);
		m_GlowList.SetItemText(i,5,x);
		x.Format("%.4f",GetDocument()->m_FS_Model.thrusters[sel].glow_norm[i].z);
		m_GlowList.SetItemText(i,6,x);
		x.Format("%.4f",GetDocument()->m_FS_Model.thrusters[sel].glow_radius[i]);
		m_GlowList.SetItemText(i,7,x);
	}

}

void CEditorFS_FUEL::OnChangeData() 
{
	if(!m_DataLocked)
	{
		UpdateEditorDisplay(TRUE);

		CString t;
		m_Properties.GetWindowText(t);
		GetDocument()->m_EditorFS_FUEL.properties=t;
		
		//Read glow point data
		if(GetDocument()->m_EditorFS_FUEL.num_glows>0)
		{
			int f=GetCurrentGlowListSelection();
			m_PositionX.GetWindowText(t);
			GetDocument()->m_EditorFS_FUEL.glow_pos[f].x=(float)atof(t);
			m_PositionY.GetWindowText(t);
			GetDocument()->m_EditorFS_FUEL.glow_pos[f].y=(float)atof(t);
			m_PositionZ.GetWindowText(t);
			GetDocument()->m_EditorFS_FUEL.glow_pos[f].z=(float)atof(t);
			m_NormalX.GetWindowText(t);
			GetDocument()->m_EditorFS_FUEL.glow_norm[f].x=(float)atof(t);
			m_NormalY.GetWindowText(t);
			GetDocument()->m_EditorFS_FUEL.glow_norm[f].y=(float)atof(t);
			m_NormalZ.GetWindowText(t);
			GetDocument()->m_EditorFS_FUEL.glow_norm[f].z=(float)atof(t);
			m_Radius.GetWindowText(t);
			GetDocument()->m_EditorFS_FUEL.glow_radius[f]=(float)atof(t);
			//Update listview right away
			t.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_pos[f].x);
			m_GlowList.SetItemText(f,1,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_pos[f].y);
			m_GlowList.SetItemText(f,2,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_pos[f].z);
			m_GlowList.SetItemText(f,3,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_norm[f].x);
			m_GlowList.SetItemText(f,4,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_norm[f].y);
			m_GlowList.SetItemText(f,5,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_norm[f].z);
			m_GlowList.SetItemText(f,6,t);
			t.Format("%.4f",GetDocument()->m_EditorFS_FUEL.glow_radius[f]);
			m_GlowList.SetItemText(f,7,t);
		}
		
		GetMainView()->InvalidateRect(NULL);
	}

}

void CEditorFS_FUEL::UpdateEditorDisplay(BOOL editing)
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

	m_Properties.EnableWindow(ic);
	m_PositionX.EnableWindow(ic);
	m_PositionY.EnableWindow(ic);
	m_PositionZ.EnableWindow(ic);
	m_NormalX.EnableWindow(ic);
	m_NormalY.EnableWindow(ic);
	m_NormalZ.EnableWindow(ic);
	m_Radius.EnableWindow(ic);
}

BOOL CEditorFS_FUEL::DoApply()
{
	int m_CurrentItem=m_List.GetCurSel();
	CString x1=GetDocument()->m_EditorFS_Subsystem.Name;
	x1.MakeLower();
	if(x1.Left(1)!="$")
		x1="$"+x1;
	GetDocument()->m_EditorFS_Subsystem.Name=x1;

	//Check whether there is a NULL normal vector
	for(int i=0;i<GetDocument()->m_EditorFS_FUEL.num_glows;i++)
	{
		FS_VPNT p=GetDocument()->m_EditorFS_FUEL.glow_norm[i];
		if(p.x==0 && p.y==0 && p.z==0)
		{
			MessageBox("At least one normal of the glow points is a NULL vector!","Error applying item");
			return FALSE;
		}
	}

	//Replace old item
	CString x;
	if(GetDocument()->m_EditorFS_FUEL.properties=="")
		x.Format("Thruster #%i",m_CurrentItem);
	else
		x.Format("Thruster #%i: %s",m_CurrentItem,GetDocument()->m_EditorFS_FUEL.properties);
	GetDocument()->m_FS_Model.thrusters[m_CurrentItem]=GetDocument()->m_EditorFS_FUEL;
	m_List.InsertString(m_CurrentItem,x);
	m_List.DeleteString(m_CurrentItem+1);
	m_List.SetCurSel(m_CurrentItem);

	//Handle new item
	if(m_IsNewItem)
		GetDocument()->m_FS_Model.num_thrusters++;

	m_IsNewItem=FALSE;
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	DoDiscard();
	return TRUE;
}

void CEditorFS_FUEL::DoDiscard()
{
	if(m_IsNewItem)
	{
		m_List.DeleteString(m_List.GetCount()-1);
		if(m_List.GetCount()!=0)
			m_List.SetCurSel(0);
	}
	UpdateEditorDisplay(FALSE);
	OnSelchangeEditorfsFuelList();
}

void CEditorFS_FUEL::OnApplyerBarClick(UINT nID)
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

void CEditorFS_FUEL::OnEditorBarClick(UINT nID)
{
/*	switch(nID)
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
	}*/
}


