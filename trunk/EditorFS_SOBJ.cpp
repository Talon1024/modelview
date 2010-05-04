// EditorFS_SOBJ.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "EditorFS_SOBJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SOBJ

IMPLEMENT_DYNCREATE(CEditorFS_SOBJ, CFormView)

CEditorFS_SOBJ::CEditorFS_SOBJ()
	: CFormView(CEditorFS_SOBJ::IDD)
{
	//{{AFX_DATA_INIT(CEditorFS_SOBJ)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_DataLocked=FALSE;
}

CEditorFS_SOBJ::~CEditorFS_SOBJ()
{
}

void CEditorFS_SOBJ::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditorFS_SOBJ)
	DDX_Control(pDX, IDC_EDITORFS_SOBJ_INFO, m_Info);
	DDX_Control(pDX, IDC_EDITORFS_SOBJ_NAME, m_Name);
	DDX_Control(pDX, IDC_EDITORFS_SOBJ_PROPERTIES, m_Properties);
	DDX_Control(pDX, IDC_EDITORFS_SOBJ_ROTATEAXIS, m_RotateAxis);
	DDX_Control(pDX, IDC_EDITORFS_SOBJ_ROTATEMODE, m_RotateMode);
	DDX_Control(pDX, IDC_EDITORFS_SOBJ_SUBMODELSLIST, m_ListSOBJ);
	DDX_Control(pDX, IDC_EDITORFS_SOBJ_DETAILLEVEL, m_TabSOBJ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditorFS_SOBJ, CFormView)
	//{{AFX_MSG_MAP(CEditorFS_SOBJ)
	ON_NOTIFY(TCN_SELCHANGE, IDC_EDITORFS_SOBJ_DETAILLEVEL, OnSelchangeEditorfsSobjDetaillevel)
	ON_LBN_SELCHANGE(IDC_EDITORFS_SOBJ_SUBMODELSLIST, OnSelchangeEditorfsSobjSubmodelslist)
	ON_BN_CLICKED(IDC_EDITORFS_SOBJ_ROTATEMODE, OnEditorfsSobjRotatemode)
	ON_EN_CHANGE(IDC_EDITORFS_SOBJ_NAME, OnChangeData)
	ON_EN_CHANGE(IDC_EDITORFS_SOBJ_PROPERTIES, OnChangeData)
	ON_CBN_SELCHANGE(IDC_EDITORFS_SOBJ_ROTATEAXIS, OnChangeData)
	ON_COMMAND_RANGE(ID_ELEMENTAPPLYER_APPLY, ID_ELEMENTAPPLYER_DISCARD, OnApplyerBarClick)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SOBJ diagnostics

#ifdef _DEBUG
void CEditorFS_SOBJ::AssertValid() const
{
	CFormView::AssertValid();
}

void CEditorFS_SOBJ::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CMainFrame * CEditorFS_SOBJ::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CEditorFS_SOBJ::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CEditorFS_SOBJ::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CEditorFS_SOBJ message handlers

void CEditorFS_SOBJ::Init()
{
	int it=0;

	m_TabSOBJ.DeleteAllItems();
	for(unsigned int i=0;i<6;i++)
	{
		if(GetDocument()->m_FS_PMinfo.n_detail>=i+1)
		{
			CString x;
			x.Format("Lvl %i",i+1);
			m_TabSOBJ.InsertItem(it,x);
			it++;
		}
	}
	if(GetDocument()->m_FS_PMinfo.n_detail > 0)
	{
		if(GetDocument()->m_FS_ModelHasDebris)
			m_TabSOBJ.InsertItem(it,"Debris");
	}
	InitSOBJMode();
}

BOOL CEditorFS_SOBJ::UnInit()
{
	if(!FinishPendingEditing())
		return FALSE;
	GetMainView()->m_EditorFS_HighLight_Segment_2=-1;
	return TRUE;
}

BOOL CEditorFS_SOBJ::FinishPendingEditing()
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

void CEditorFS_SOBJ::OnSelchangeEditorfsSobjDetaillevel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	InitSOBJMode();	
	*pResult = 0;
}

void CEditorFS_SOBJ::InitSOBJMode()
{
	m_ListSOBJ.ResetContent();
	int sobjmode=m_TabSOBJ.GetCurSel();
	int m_Detaillevel=sobjmode;
	if((sobjmode+1==m_TabSOBJ.GetItemCount()) && GetDocument()->m_FS_ModelHasDebris)
		m_Detaillevel=9;

	GetMainView()->FS_SetDetailLevel(m_Detaillevel);
	for(int i=0;i<GetDocument()->m_FS_NumSOBJ;i++)
	{
		if(GetDocument()->m_FS_SOBJ[i].detail==m_Detaillevel/*(long)view->m_Detaillevel*/)
		{
			CString tstr;
			tstr.Format("%s",GetDocument()->m_FS_SOBJ[i].submodel_name);
			m_ListSOBJ.AddString(tstr);
		}
	}
	m_ListSOBJ.SetCurSel(0);
	OnSelchangeEditorfsSobjSubmodelslist();
}

void CEditorFS_SOBJ::OnSelchangeEditorfsSobjSubmodelslist() 
{
	m_DataLocked=TRUE;
	int sel=GetMainView()->FS_CalcRealSubmodelNumber(m_ListSOBJ.GetCurSel());
	GetMainView()->m_EditorFS_HighLight_Segment_2=sel;
	GetMainView()->InvalidateRect(NULL);

	GetDocument()->m_EditorFS_SOBJ=GetDocument()->m_FS_SOBJ[sel];

	//Rotate mode
	m_RotateMode.SetCheck(GetDocument()->m_EditorFS_SOBJ.movement_type==1);
	if(GetDocument()->m_EditorFS_SOBJ.movement_axis>=0)
		m_RotateAxis.SetCurSel(GetDocument()->m_EditorFS_SOBJ.movement_axis);
	
	m_Name.SetWindowText(GetDocument()->m_EditorFS_SOBJ.submodel_name);
	m_Properties.SetWindowText(GetDocument()->m_EditorFS_SOBJ.properties);
	OnEditorfsSobjRotatemode();
	m_DataLocked=FALSE;

	//Info
	CString info,line;
	FS_VPNT bbox,offs;
	int parent=GetDocument()->m_FS_SOBJ[sel].submodel_parent;
	offs=GetDocument()->m_FS_SOBJ[sel].offset;
	bbox=GetDocument()->m_FS_SOBJ[sel].bounding_box_min_point; 
	line.Format("Boundingbox min: %.2f|%.2f|%.2f\r\n",bbox.x+offs.x,bbox.y+offs.y,bbox.z+offs.z); info+=line;
	bbox=GetDocument()->m_FS_SOBJ[sel].bounding_box_max_point;
	line.Format("Boundingbox max: %.2f|%.2f|%.2f\r\n",bbox.x+offs.x,bbox.y+offs.y,bbox.z+offs.z); info+=line;
	line.Format("Radius: %.2f\r\n\r\n",GetDocument()->m_FS_SOBJ[sel].radius); info+=line;
	if(parent>=0)
		line.Format("Parent submodel: %s\r\n",GetDocument()->m_FS_SOBJ[parent].submodel_name);
	else
		line.Format("Parent submodel: <None>\r\n");
	info+=line;
	line.Format("Offset: %.2f|%.2f|%.2f",offs.x,offs.y,offs.z); info+=line;
	m_Info.SetWindowText(info);
}

void CEditorFS_SOBJ::OnEditorfsSobjRotatemode()
{
	OnChangeData();
	if(m_RotateAxis.GetCurSel()==-1)
		m_RotateAxis.SetCurSel(0);
	m_RotateAxis.EnableWindow(m_RotateMode.GetCheck());
}

void CEditorFS_SOBJ::OnInitialUpdate() 
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
		placeHld=(CStatic *)GetDlgItem(IDC_EDITORFS_SOBJ_TOOLBARAPPLYER);
		placeHld->GetWindowPlacement(&wndPlmnt);
		m_TBApplyer.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);	
		m_TBApplyer.ShowWindow(FALSE);
		m_AmEditing=FALSE;
	}
}

void CEditorFS_SOBJ::OnChangeData() 
{
	if(!m_DataLocked)
	{
		m_ListSOBJ.EnableWindow(FALSE);
		m_TabSOBJ.EnableWindow(FALSE);
		m_TBApplyer.ShowWindow(TRUE);
		m_AmEditing=TRUE;

		//Read data
		m_Name.GetWindowText(GetDocument()->m_EditorFS_SOBJ.submodel_name);
		m_Properties.GetWindowText(GetDocument()->m_EditorFS_SOBJ.properties);
		if(m_RotateMode.GetCheck())
		{
			GetDocument()->m_EditorFS_SOBJ.movement_type=1;
			GetDocument()->m_EditorFS_SOBJ.movement_axis=m_RotateAxis.GetCurSel();
		} else {
			GetDocument()->m_EditorFS_SOBJ.movement_type=-1;
			GetDocument()->m_EditorFS_SOBJ.movement_axis=-1;
		}
	}
}

void CEditorFS_SOBJ::OnApplyerBarClick(UINT nID)
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

void CEditorFS_SOBJ::DoApply()
{
	int sel=GetMainView()->FS_CalcRealSubmodelNumber(m_ListSOBJ.GetCurSel());
	GetDocument()->m_FS_SOBJ[sel]=GetDocument()->m_EditorFS_SOBJ;
	m_ListSOBJ.InsertString(sel,GetDocument()->m_EditorFS_SOBJ.submodel_name);
	m_ListSOBJ.DeleteString(sel+1);
	m_ListSOBJ.SetCurSel(sel);
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	DoDiscard();
}

void CEditorFS_SOBJ::DoDiscard()
{
	m_ListSOBJ.EnableWindow(TRUE);
	m_TabSOBJ.EnableWindow(TRUE);
	m_TBApplyer.ShowWindow(FALSE);
	m_AmEditing=FALSE;
	
	//Reset data
	OnSelchangeEditorfsSobjSubmodelslist();
}

void CEditorFS_SOBJ::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (::IsWindow(m_hWnd))
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
		
	CTabCtrl *tc=(CTabCtrl *)GetDlgItem(IDC_EDITORFS_SOBJ_DETAILLEVEL);
	if(tc!=NULL)
	{
		if(::IsWindow(tc->m_hWnd))
			tc->SetWindowPos(NULL,1,2,cx-5,190,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	CListBox *lbox=(CListBox *)GetDlgItem(IDC_EDITORFS_SOBJ_SUBMODELSLIST);
	if(lbox!=NULL)
	{
		if(::IsWindow(lbox->m_hWnd))
			lbox->SetWindowPos(NULL,1,2,cx-16,150,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}

	CEdit *eded=(CEdit *)GetDlgItem(IDC_EDITORFS_SOBJ_NAME);
	if(eded!=NULL)
	{
		if(::IsWindow(eded->m_hWnd))
			eded->SetWindowPos(NULL,1,2,cx-79,22,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	CComboBox *comb=(CComboBox *)GetDlgItem(IDC_EDITORFS_SOBJ_ROTATEAXIS);
	if(comb!=NULL)
	{
		if(::IsWindow(comb->m_hWnd))
			comb->SetWindowPos(NULL,1,2,cx-79,82,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	CEdit *edt2=(CEdit *)GetDlgItem(IDC_EDITORFS_SOBJ_PROPERTIES);
	if(edt2!=NULL)
	{
		if(::IsWindow(edt2->m_hWnd))
			edt2->SetWindowPos(NULL,1,2,cx-5,75,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}

	CEdit *edt3=(CEdit *)GetDlgItem(IDC_EDITORFS_SOBJ_INFO);
	if(edt3!=NULL)
	{
		if(::IsWindow(edt3->m_hWnd))
			edt3->SetWindowPos(NULL,1,2,cx-5,90,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}

	/*CStatic *head=(CStatic *)GetDlgItem(IDC_EDITORFS_GPNT_HEADER);
	if(head!=NULL)
	{
		if(::IsWindow(head->m_hWnd))
			head->SetWindowPos(NULL,1,2,cx-69,22,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}*/

	InvalidateRect(NULL);
}

