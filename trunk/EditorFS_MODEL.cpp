// EditorFS_MODEL.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "EditorFS_MODEL.h"
#include "EditorFSDlg_MomentOfInertia.h"
#include "EditorFSDlg_Textures.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_MODEL

IMPLEMENT_DYNCREATE(CEditorFS_MODEL, CFormView)

CEditorFS_MODEL::CEditorFS_MODEL()
	: CFormView(CEditorFS_MODEL::IDD)
{
	//{{AFX_DATA_INIT(CEditorFS_MODEL)
	//}}AFX_DATA_INIT
	m_DataLocked=FALSE;
}

CEditorFS_MODEL::~CEditorFS_MODEL()
{
}

void CEditorFS_MODEL::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorFS_MODEL)
	DDX_Control(pDX, IDC_EDITORFS_MODEL_LIGHT_TYPE, m_Lights_Type);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_LIGHT_POSITION_Z, m_Lights_Position_Z);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_LIGHT_POSITION_Y, m_Lights_Position_Y);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_LIGHT_POSITION_X, m_Lights_Position_X);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_LIGHTS, m_List_Lights);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MASS_CENTER_Z, m_MassCenter_Z);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MASS_CENTER_Y, m_MassCenter_Y);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MASS_CENTER_X, m_MassCenter_X);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_MASS, m_Mass);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_ACEN_Z, m_ACenPoint_Z);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_ACEN_Y, m_ACenPoint_Y);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_ACEN_X, m_ACenPoint_X);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_EYEPOINT_POSITION_Z, m_Eye_Position_Z);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_EYEPOINT_POSITION_Y, m_Eye_Position_Y);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_EYEPOINT_POSITION_X, m_Eye_Position_X);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_EYEPOINT_NORMAL_Z, m_Eye_Normal_Z);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_EYEPOINT_NORMAL_Y, m_Eye_Normal_Y);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_EYEPOINT_NORMAL_X, m_Eye_Normal_X);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_EYEPOINT_SUBMODEL, m_Eye_Submodel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEditorFS_MODEL, CFormView)
	//{{AFX_MSG_MAP(CEditorFS_MODEL)
	ON_CBN_SELCHANGE(IDC_EDITORFS_MODEL_EYEPOINT_SUBMODEL, OnSelchangeEditorfsModelEyepointSubmodel)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_ACEN_X, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_ACEN_Y, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_ACEN_Z, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_LIGHT_POSITION_X, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_LIGHT_POSITION_Y, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_LIGHT_POSITION_Z, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_LIGHT_TYPE, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_MASS_CENTER_X, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_MASS_CENTER_Y, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_MASS_CENTER_Z, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_MASS, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_EYEPOINT_POSITION_X, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_EYEPOINT_POSITION_Y, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_EYEPOINT_POSITION_Z, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_EYEPOINT_NORMAL_X, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_EYEPOINT_NORMAL_Y, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_MODEL_EYEPOINT_NORMAL_Z, OnChangeData)
	ON_BN_CLICKED(IDC_EDITORFS_MODEL_BUTTON_MOMENTOFINERTIA, OnButtonMomentOfInertia)
	ON_BN_CLICKED(IDC_EDITORFS_MODEL_BUTTON_TEXTURENAMES, OnButtonTextureNames)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EDITORFS_MODEL_LIGHTS, OnItemchangedEditorfsModelLights)
	ON_COMMAND_RANGE(ID_ELEMENTAPPLYER_APPLY, ID_ELEMENTAPPLYER_DISCARD, OnApplyerBarClick)
	ON_COMMAND_RANGE(ID_ELEMENTEDITOR_CREATE,ID_ELEMENTEDITOR_DELETE, OnEditorBarClick)
	ON_COMMAND_RANGE(ID_ELEMENTEDITOR2_CREATE,ID_ELEMENTEDITOR2_DELETE, OnEditor2BarClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_MODEL diagnostics

#ifdef _DEBUG
void CEditorFS_MODEL::AssertValid() const
{
	CFormView::AssertValid();
}

void CEditorFS_MODEL::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CMainFrame * CEditorFS_MODEL::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CEditorFS_MODEL::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CEditorFS_MODEL::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_MODEL message handlers

void CEditorFS_MODEL::Init()
{
	GetDocument()->m_EditorFS_Model=GetDocument()->m_FS_ModelInfo;
	GetMainView()->m_EditorFS_ShowModelInfo=TRUE;
	m_Eye_Submodel.ResetContent();
	m_Eye_Submodel.AddString("<No eye point>");
	for(int i=0;i<GetDocument()->m_FS_NumSOBJ;i++)
		m_Eye_Submodel.AddString(GetDocument()->m_FS_SOBJ[i].submodel_name);
	m_AmEditing=FALSE;
	FillInfo();
	ChangeFSMode();
}

void CEditorFS_MODEL::FillInfo()
{
	m_DataLocked=TRUE;

	if(GetDocument()->m_EditorFS_Model.num_eye_points==0)
		m_Eye_Submodel.SetCurSel(0);
	else
	{
		m_Eye_Submodel.SetCurSel(GetDocument()->m_EditorFS_Model.eye_point_sobj_number+1);
		
		CString t;
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.eye_point_sobj_offset.x);
		m_Eye_Position_X.SetWindowText(t);
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.eye_point_sobj_offset.y);
		m_Eye_Position_Y.SetWindowText(t);
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.eye_point_sobj_offset.z);
		m_Eye_Position_Z.SetWindowText(t);
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.eye_point_normal.x);
		m_Eye_Normal_X.SetWindowText(t);
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.eye_point_normal.y);
		m_Eye_Normal_Y.SetWindowText(t);
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.eye_point_normal.z);
		m_Eye_Normal_Z.SetWindowText(t);
	}

	//AutoCenter point
	CString t;
	t.Format("%.2f",GetDocument()->m_EditorFS_Model.ACenPoint.x);
	m_ACenPoint_X.SetWindowText(t);
	t.Format("%.2f",GetDocument()->m_EditorFS_Model.ACenPoint.y);
	m_ACenPoint_Y.SetWindowText(t);
	t.Format("%.2f",GetDocument()->m_EditorFS_Model.ACenPoint.z);
	m_ACenPoint_Z.SetWindowText(t);

	//Mass
	t.Format("%.4f",GetDocument()->m_EditorFS_Model.mass);
	m_Mass.SetWindowText(t);
	t.Format("%.4f",GetDocument()->m_EditorFS_Model.mass_center.x);
	m_MassCenter_X.SetWindowText(t);
	t.Format("%.4f",GetDocument()->m_EditorFS_Model.mass_center.y);
	m_MassCenter_Y.SetWindowText(t);
	t.Format("%.4f",GetDocument()->m_EditorFS_Model.mass_center.z);
	m_MassCenter_Z.SetWindowText(t);
	/*t.Format("%.8f",GetDocument()->m_EditorFS_Model.moment_inertia[0][0]);
	m_MomInertia_00.SetWindowText(t);
	t.Format("%.8f",GetDocument()->m_EditorFS_Model.moment_inertia[0][1]);
	m_MomInertia_01.SetWindowText(t);
	t.Format("%.8f",GetDocument()->m_EditorFS_Model.moment_inertia[0][2]);
	m_MomInertia_02.SetWindowText(t);
	t.Format("%.8f",GetDocument()->m_EditorFS_Model.moment_inertia[1][0]);
	m_MomInertia_10.SetWindowText(t);
	t.Format("%.8f",GetDocument()->m_EditorFS_Model.moment_inertia[1][1]);
	m_MomInertia_11.SetWindowText(t);
	t.Format("%.8f",GetDocument()->m_EditorFS_Model.moment_inertia[1][2]);
	m_MomInertia_12.SetWindowText(t);
	t.Format("%.8f",GetDocument()->m_EditorFS_Model.moment_inertia[2][0]);
	m_MomInertia_20.SetWindowText(t);
	t.Format("%.8f",GetDocument()->m_EditorFS_Model.moment_inertia[2][1]);
	m_MomInertia_21.SetWindowText(t);
	t.Format("%.8f",GetDocument()->m_EditorFS_Model.moment_inertia[2][2]);
	m_MomInertia_22.SetWindowText(t);*/

	//Cross sections
	//m_List_CrossSections.DeleteAllItems();
	for(int i2=0;i2<GetDocument()->m_EditorFS_Model.num_cross_sections;i2++)
	{
		t.Format("Cross section: %i %.4f %.4f\n",i2,GetDocument()->m_EditorFS_Model.cross_section_depth[i2],GetDocument()->m_EditorFS_Model.cross_section_radius[i2]);
		OutputDebugString(t);
		/*t.Format("%i",i);
		m_List_CrossSections.InsertItem(i,t);
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.cross_section_depth[i]);
		m_List_CrossSections.SetItemText(i,1,t);
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.cross_section_radius[i]);
		m_List_CrossSections.SetItemText(i,2,t);*/
	}

	//Lights
	m_List_Lights.DeleteAllItems();
	for(int i=0;i<GetDocument()->m_EditorFS_Model.num_lights;i++)
	{
		t.Format("%i",i);
		m_List_Lights.InsertItem(i,t);
		t.Format("%i",GetDocument()->m_EditorFS_Model.light_type[i]);
		m_List_Lights.SetItemText(i,1,t);
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.light_location[i].x);
		m_List_Lights.SetItemText(i,2,t);
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.light_location[i].y);
		m_List_Lights.SetItemText(i,3,t);
		t.Format("%.2f",GetDocument()->m_EditorFS_Model.light_location[i].z);
		m_List_Lights.SetItemText(i,4,t);
	}
	if(GetDocument()->m_FS_ModelInfo.num_lights>0)
	{
		//Select first light
		GetMainView()->m_EditorFS_MODEL_CurLight=0;
		m_List_Lights.SetItemState(0,LVIS_SELECTED | LVIS_FOCUSED,LVIS_SELECTED | LVIS_FOCUSED);
	}
	else
		GetMainView()->m_EditorFS_MODEL_CurLight=-1;
	UpdateEditorDisplay();
	m_DataLocked=FALSE;
}

BOOL CEditorFS_MODEL::UnInit()
{
	if(!FinishPendingEditing())
		return FALSE;
	GetMainView()->m_EditorFS_ShowModelInfo=FALSE;
	return TRUE;
}

BOOL CEditorFS_MODEL::FinishPendingEditing()
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


void CEditorFS_MODEL::OnSelchangeEditorfsModelEyepointSubmodel() 
{
	int sel=m_Eye_Submodel.GetCurSel();
	if(sel==0)
		GetDocument()->m_EditorFS_Model.num_eye_points=0;
	else
	{
		GetDocument()->m_EditorFS_Model.num_eye_points=1;
		GetDocument()->m_EditorFS_Model.eye_point_sobj_number=sel-1;
	}
	FillInfo();
	OnChangeData();
}

void CEditorFS_MODEL::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if(!::IsWindow(m_TBLights))
	{
		WINDOWPLACEMENT wndPlmnt;
		CStatic *placeHld;

		/*m_TBCrossSections.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBCrossSections.LoadToolBar(IDR_ELEMENTEDITOR);
		m_TBCrossSections.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_MODEL_TOOLBAR_CROSSSECTIONS);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBCrossSections.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);*/

		m_TBLights.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBLights.LoadToolBar(IDR_ELEMENTEDITOR);
		m_TBLights.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_MODEL_TOOLBAR_LIGHTS);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBLights.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);

		m_TBApplyer.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_AUTOSIZE, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBApplyer.LoadToolBar(IDR_ELEMENTAPPLYER);
		m_TBApplyer.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_TBApplyer.SetButtonText(0,"Apply");
		m_TBApplyer.SetButtonText(1,"Discard");
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_MODEL_TOOLBAR_APPLYER);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBApplyer.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);
		m_TBApplyer.ShowWindow(FALSE);

		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();
	}

	//Cross sections COLUMNS
	/*m_List_CrossSections.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	for(int i=0;i<MAX_FS_CROSSSECTIONS;i++)
		m_List_CrossSections.DeleteColumn(0);
	m_List_CrossSections.InsertColumn(0,"#",LVCFMT_LEFT,30);
	m_List_CrossSections.InsertColumn(1,"Depth",LVCFMT_RIGHT,60);
	m_List_CrossSections.InsertColumn(2,"Radius",LVCFMT_RIGHT,60);*/

	//Lights COLUMNS
	m_List_Lights.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	for(int i=0;i<MAX_FS_LIGHTS;i++)
		m_List_Lights.DeleteColumn(0);
	m_List_Lights.InsertColumn(0,"#",LVCFMT_LEFT,18);
	m_List_Lights.InsertColumn(1,"T",LVCFMT_RIGHT,0);
	m_List_Lights.InsertColumn(2,"x",LVCFMT_RIGHT,48);
	m_List_Lights.InsertColumn(3,"y",LVCFMT_RIGHT,48);
	m_List_Lights.InsertColumn(4,"z",LVCFMT_RIGHT,48);
}

void CEditorFS_MODEL::OnChangeData() 
{
	if(!m_DataLocked)
	{
		m_TBApplyer.ShowWindow(TRUE);
		m_AmEditing=TRUE;
		CString t;

		//Lights
		int curl=GetCurrentLightSelection();
		if(curl>=0)
		{
			m_Lights_Position_X.GetWindowText(t);
			GetDocument()->m_EditorFS_Model.light_location[curl].x=(float)atof(t);
			m_Lights_Position_Y.GetWindowText(t);
			GetDocument()->m_EditorFS_Model.light_location[curl].y=(float)atof(t);
			m_Lights_Position_Z.GetWindowText(t);
			GetDocument()->m_EditorFS_Model.light_location[curl].z=(float)atof(t);
			m_Lights_Type.GetWindowText(t);
			GetDocument()->m_EditorFS_Model.light_type[curl]=atoi(t);

			//Update list box
			t.Format("%i",GetDocument()->m_EditorFS_Model.light_type[curl]);
			m_List_Lights.SetItemText(curl,1,t);
			t.Format("%.2f",GetDocument()->m_EditorFS_Model.light_location[curl].x);
			m_List_Lights.SetItemText(curl,2,t);
			t.Format("%.2f",GetDocument()->m_EditorFS_Model.light_location[curl].y);
			m_List_Lights.SetItemText(curl,3,t);
			t.Format("%.2f",GetDocument()->m_EditorFS_Model.light_location[curl].z);
			m_List_Lights.SetItemText(curl,4,t);
		}

		//Mass
		m_Mass.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.mass=atof(t);
		m_MassCenter_X.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.mass_center.x=(float)atof(t);
		m_MassCenter_Y.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.mass_center.y=(float)atof(t);
		m_MassCenter_Z.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.mass_center.z=(float)atof(t);

		//AutoCenter
		m_ACenPoint_X.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.ACenPoint.x=(float)atof(t);
		m_ACenPoint_Y.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.ACenPoint.y=(float)atof(t);
		m_ACenPoint_Z.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.ACenPoint.z=(float)atof(t);

		//Eye Point
		m_Eye_Position_X.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.eye_point_sobj_offset.x=(float)atof(t);
		m_Eye_Position_Y.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.eye_point_sobj_offset.y=(float)atof(t);
		m_Eye_Position_Z.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.eye_point_sobj_offset.z=(float)atof(t);
		m_Eye_Normal_X.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.eye_point_normal.x=(float)atof(t);
		m_Eye_Normal_Y.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.eye_point_normal.y=(float)atof(t);
		m_Eye_Normal_Z.GetWindowText(t);
		GetDocument()->m_EditorFS_Model.eye_point_normal.z=(float)atof(t);

		GetMainView()->InvalidateRect(NULL);
	}
}

void CEditorFS_MODEL::OnItemchangedEditorfsModelLights(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	OnSelchangeLights();
	*pResult = 0;
}

void CEditorFS_MODEL::OnSelchangeLights()
{
	m_DataLocked=TRUE;
	int i=GetCurrentLightSelection();
	GetMainView()->m_EditorFS_MODEL_CurLight=i;
	CString fl;
	fl.Format("%i",GetDocument()->m_EditorFS_Model.light_type[i]);
	m_Lights_Type.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_Model.light_location[i].x);
	m_Lights_Position_X.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_Model.light_location[i].y);
	m_Lights_Position_Y.SetWindowText(fl);
	fl.Format("%.4f",GetDocument()->m_EditorFS_Model.light_location[i].z);
	m_Lights_Position_Z.SetWindowText(fl);

	GetMainView()->InvalidateRect(NULL);
	m_DataLocked=FALSE;
}

int CEditorFS_MODEL::GetCurrentLightSelection()
{
	if(GetDocument()->m_EditorFS_Model.num_lights<=0)
		return -1;
	POSITION pos=m_List_Lights.GetFirstSelectedItemPosition();
	int i=m_List_Lights.GetNextSelectedItem(pos);
	if(i==-1) //if nothing is selected we default to item #1
		i=0;
	if(i>MAX_FS_LIGHTS)
	{
		i=MAX_FS_LIGHTS-1;
		ASSERT(FALSE);
	}
	return i;
}

//Cross sections
void CEditorFS_MODEL::OnEditorBarClick(UINT nID)
{
	switch(nID)
	{
	case ID_ELEMENTEDITOR_CREATE:
		DoNewLight();
		break;

	case ID_ELEMENTEDITOR_DELETE:
		DoDeleteLight();
		break;

	case ID_ELEMENTEDITOR_COPY:
		DoCopyLight();
		break;
	
	default:
		ASSERT(FALSE);
		AfxAbort(); //"OnEditorBarClick with unknown nID");
	}
}

//Lights
void CEditorFS_MODEL::OnEditor2BarClick(UINT nID)
{
	switch(nID)
	{
	case ID_ELEMENTEDITOR2_CREATE:
		//DoNewFiringPoint();
		break;

	case ID_ELEMENTEDITOR2_DELETE:
		//DoDeleteFiringPoint();
		break;

	case ID_ELEMENTEDITOR2_COPY:
		//DoCopyFiringPoint();
		break;
	
	default:
		ASSERT(FALSE);
		AfxAbort(); //"OnEditor2BarClick with unknown nID");
	}
}

void CEditorFS_MODEL::OnApplyerBarClick(UINT nID)
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

void CEditorFS_MODEL::OnButtonMomentOfInertia()
{
	int i1,i2;
	CEditorFSDlg_MomentOfInertia dlg;
	for(i1=0;i1<3;i1++)
	{
		for(i2=0;i2<3;i2++)
		{
			dlg.moment_inertia[i1][i2]=GetDocument()->m_EditorFS_Model.moment_inertia[i1][i2];
			float f=GetDocument()->m_EditorFS_Model.moment_inertia[i1][i2];
			OutputDebugString("");
		}
	}
	if(dlg.DoModal()!=IDOK)
		return;

	for(i1=0;i1<3;i1++)
	{
		for(i2=0;i2<3;i2++)
			GetDocument()->m_EditorFS_Model.moment_inertia[i1][i2]=dlg.moment_inertia[i1][i2];
	}
	OnChangeData(); //data has changed
}

void CEditorFS_MODEL::OnButtonTextureNames()
{
	CEditorFSDlg_Textures dlg;
	dlg.DoModal();

	//Reload textures
	if(GetMainView()->m_RenderMode == RENDER_TEXTURED)
		GetDocument()->FS_LoadPCXData(GetMainView()->m_Detaillevel,GetMainView()->m_ShowThruster,GetMainView()->m_FastTextureLoad);
	GetMainView()->RedrawWindow();
}


/*
	DDX_Control(pDX, IDC_EDITORFS_MODEL_CROSSSECTIONS, m_List_CrossSections);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_CROSSSECTIONS_DEPTH, m_CrossSections_Depth);
	DDX_Control(pDX, IDC_EDITORFS_MODEL_CROSSSECTIONS_RADIUS, m_CrossSections_Radius);
*/


void CEditorFS_MODEL::UpdateEditorDisplay()
{
	BOOL ic=(GetDocument()->m_EditorFS_Model.num_lights>0);
	m_Lights_Position_X.EnableWindow(ic);
	m_Lights_Position_Y.EnableWindow(ic);
	m_Lights_Position_Z.EnableWindow(ic);
	m_Lights_Type.EnableWindow(ic);

	ic=(GetDocument()->m_EditorFS_Model.num_eye_points>0);
	m_Eye_Position_X.EnableWindow(ic);
	m_Eye_Position_Y.EnableWindow(ic);
	m_Eye_Position_Z.EnableWindow(ic);
	m_Eye_Normal_X.EnableWindow(ic);
	m_Eye_Normal_Y.EnableWindow(ic);
	m_Eye_Normal_Z.EnableWindow(ic);

}

void CEditorFS_MODEL::DoNewLight()
{
	if(GetDocument()->m_EditorFS_Model.num_lights>=MAX_FS_LIGHTS)
	{
		MessageBox("Already too many lights.","Error");
		return;
	}
	m_DataLocked=TRUE;
	int i=GetDocument()->m_EditorFS_Model.num_lights;
	GetDocument()->m_EditorFS_Model.num_lights++;
	GetDocument()->m_EditorFS_Model.light_type[i]=1;
	m_Lights_Type.SetWindowText("1");
	GetDocument()->m_EditorFS_Model.light_location[i].x=0.0;
	GetDocument()->m_EditorFS_Model.light_location[i].y=0.0;
	GetDocument()->m_EditorFS_Model.light_location[i].z=0.0;
	FillInfo();
	m_List_Lights.SetItemState(i,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
	m_DataLocked=FALSE;
	OnChangeData();
}

void CEditorFS_MODEL::DoCopyLight()
{
	if(GetDocument()->m_EditorFS_Model.num_lights>=MAX_FS_LIGHTS)
	{
		MessageBox("Already too many lights.","Error");
		return;
	}
	m_DataLocked=TRUE;
	int i=GetDocument()->m_EditorFS_Model.num_lights;
	int i2=GetCurrentLightSelection();
	if(i==0)
	{
		DoNewLight(); //if no light existed yet, use this routine instead
		return;
	}
	GetDocument()->m_EditorFS_Model.num_lights++;
	GetDocument()->m_EditorFS_Model.light_type[i]=GetDocument()->m_EditorFS_Model.light_type[i2];
	GetDocument()->m_EditorFS_Model.light_location[i].x=GetDocument()->m_EditorFS_Model.light_location[i2].x;
	GetDocument()->m_EditorFS_Model.light_location[i].y=GetDocument()->m_EditorFS_Model.light_location[i2].y;
	GetDocument()->m_EditorFS_Model.light_location[i].z=GetDocument()->m_EditorFS_Model.light_location[i2].z;
	FillInfo();
	m_List_Lights.SetItemState(i,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
	m_DataLocked=FALSE;
	OnChangeData();
}

void CEditorFS_MODEL::DoDeleteLight()
{
	if(GetDocument()->m_EditorFS_Model.num_lights<=0)
		return;
	if(MessageBox("Are you sure you want to delete this light?","Confirmation",MB_YESNO)==IDNO)
		return;

	int i2=GetCurrentLightSelection();
	for(int i=i2+1;i<GetDocument()->m_EditorFS_Model.num_lights;i++)
	{
		GetDocument()->m_EditorFS_Model.light_type[i-1]=GetDocument()->m_EditorFS_Model.light_type[i];
		GetDocument()->m_EditorFS_Model.light_location[i-1].x=GetDocument()->m_EditorFS_Model.light_location[i].x;
		GetDocument()->m_EditorFS_Model.light_location[i-1].y=GetDocument()->m_EditorFS_Model.light_location[i].y;
		GetDocument()->m_EditorFS_Model.light_location[i-1].z=GetDocument()->m_EditorFS_Model.light_location[i].z;
	}
	GetDocument()->m_EditorFS_Model.num_lights--;
	FillInfo();
	int sel=i2;
	if(GetDocument()->m_EditorFS_Model.num_lights<=sel)
		sel=GetDocument()->m_EditorFS_Model.num_lights-1;
	if(sel>=0)
		m_List_Lights.SetItemState(sel,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);	
	OnChangeData();
}


void CEditorFS_MODEL::DoApply()
{
	GetDocument()->m_FS_ModelInfo=GetDocument()->m_EditorFS_Model;
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	DoDiscard();
}

void CEditorFS_MODEL::DoDiscard()
{
	GetDocument()->m_EditorFS_Model=GetDocument()->m_FS_ModelInfo;
	m_AmEditing=FALSE;
	m_TBApplyer.ShowWindow(FALSE);
}


void CEditorFS_MODEL::ChangeFSMode()
{
	m_DataLocked=TRUE;
	int acenav=TRUE;
	if(GetDocument()->m_FS_CurrVP_FreeSpaceVersion==1)
	{
		m_ACenPoint_X.SetWindowText("0.00");
		m_ACenPoint_Y.SetWindowText("0.00");
		m_ACenPoint_Z.SetWindowText("0.00");
		acenav=FALSE;
	}
	m_ACenPoint_X.EnableWindow(acenav);
	m_ACenPoint_Y.EnableWindow(acenav);
	m_ACenPoint_Z.EnableWindow(acenav);
	m_DataLocked=FALSE;
}
