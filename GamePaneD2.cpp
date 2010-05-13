// GamePaneD2.cpp : implementation file
//

#include "stdafx.h"
#include "MODVIEW32.h"
#include "GamePaneD2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGamePaneD2

IMPLEMENT_DYNCREATE(CGamePaneD2, CFormView)

CGamePaneD2::CGamePaneD2()
	: CFormView(CGamePaneD2::IDD)
{
	//{{AFX_DATA_INIT(CGamePaneD2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGamePaneD2::~CGamePaneD2()
{
}

void CGamePaneD2::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGamePaneD2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGamePaneD2, CFormView)
	//{{AFX_MSG_MAP(CGamePaneD2)
	ON_NOTIFY(TCN_SELCHANGE, IDC_D2_DISPLAY, OnSelchangeD2Display)
	ON_LBN_SELCHANGE(IDC_D2_POSITIONS, OnSelchangeD2Positions)
	ON_LBN_SELCHANGE(IDC_D2_TEXTURELIST, OnSelchangeD2Texturelist)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGamePaneD2 diagnostics

#ifdef _DEBUG
void CGamePaneD2::AssertValid() const
{
	CFormView::AssertValid();
}

void CGamePaneD2::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


CMainFrame * CGamePaneD2::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CGamePaneD2::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CGamePaneD2::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}


/////////////////////////////////////////////////////////////////////////////
// CGamePaneD2 message handlers

void CGamePaneD2::Init()
{
	CMODVIEW32Doc *pDoc=GetDocument();

	CStatic *ctrl;
	char *temp=new char[256];
	CString sep=" ";
	ctrl=(CStatic *)GetDlgItem(IDC_D2_VERTICIES);
	ctrl->SetWindowText(sep+_itoa(pDoc->m_D2_Model.Vcount,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_D2_POLYGONS);
	ctrl->SetWindowText(sep+_itoa(pDoc->m_D2_Model.Pcount,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_D2_TEXTURES);
	ctrl->SetWindowText(sep+_itoa(pDoc->m_D2_Model.PolyModel.n_textures,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_D2_SOBJ);
	ctrl->SetWindowText(sep+_itoa(pDoc->m_D2_Model.Scount,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_D2_GUNS);
	ctrl->SetWindowText(sep+_itoa(pDoc->m_Guns.Num,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_D2_TYPEDESC);
	ctrl->SetWindowText(sep+pDoc->m_ModelType_Desc);

	CTabCtrl *ts=(CTabCtrl *)GetDlgItem(IDC_D2_DISPLAY);
	if(ts->GetItemCount()==0)
	{
		ts->DeleteAllItems();
		ts->InsertItem(0,"Textures");
		ts->InsertItem(1,"Submodels");
	}
	FillTab();
	
	CListBox *lbox=(CListBox *)GetDlgItem(IDC_D2_POSITIONS);
	lbox->ResetContent();
	lbox->AddString("<default>");
	if(pDoc->m_ModelType_Desc.Compare("Robot")==0)
	{
		lbox->AddString("State 0 - Inactive");
		lbox->AddString("State 1 - Normal");
		lbox->AddString("State 2 - Attack");
		lbox->AddString("State 3 - Shoot/hit motion");
		lbox->AddString("State 4 - Getting hit");
	}
	lbox->SetCurSel(0);

	delete[] temp;
}

void CGamePaneD2::FillTab()
{
	int i;
	CMODVIEW32Doc *pDoc=GetDocument();
	CMODVIEW32View *view=GetMainView();

	CListBox *lbox=(CListBox *)GetDlgItem(IDC_D2_TEXTURELIST);
	CTabCtrl *ts=(CTabCtrl *)GetDlgItem(IDC_D2_DISPLAY);

	//Reset settings
	view->m_DisplayTexture=-1;
	view->m_DisplaySubmodel=-1;
	view->m_DisplayGun=-1;

	switch(ts->GetCurSel())
	{
	case 0:
		lbox->ResetContent();
		lbox->AddString("<All textures>");
		for(i=0;i<pDoc->m_TexturesNum+1;i++)
			lbox->AddString(pDoc->m_Textures[i].Filename);
		lbox->SetCurSel(0);
		break;

	case 1:
		lbox->ResetContent();
		lbox->AddString("<All submodels>");
		for(i=0;i<pDoc->m_D2_Model.PolyModel.n_models;i++)
		{
			CString tstr;
			tstr.Format("#%i",i);
			lbox->AddString(tstr);
		}
		lbox->SetCurSel(0);
		break;

	default:
		ASSERT(FALSE);
	}

	view->RedrawWindow();

}

void CGamePaneD2::OnSelchangeD2Display(NMHDR* pNMHDR, LRESULT* pResult) 
{
	FillTab();	
	*pResult = 0;
}

void CGamePaneD2::OnSelchangeD2Positions() 
{
	CMODVIEW32View *view=GetMainView();
	CListBox *lbox=(CListBox *)GetDlgItem(IDC_D2_POSITIONS);
	view->D2_SetPosition(lbox->GetCurSel());
}

void CGamePaneD2::OnSelchangeD2Texturelist() 
{
	CMODVIEW32View *view=GetMainView();

	CListBox *lbox=(CListBox *)GetDlgItem(IDC_D2_TEXTURELIST);
	CTabCtrl *ts=(CTabCtrl *)GetDlgItem(IDC_D2_DISPLAY);

	switch(ts->GetCurSel())
	{
	case 0:
		view->m_DisplayTexture=lbox->GetCurSel()-1;
		break;
		
	case 1:
		view->m_DisplaySubmodel=lbox->GetCurSel()-1;
		break;

	default:
		ASSERT(FALSE);
	}

	view->InvalidateRect(NULL);
}

void CGamePaneD2::OnSize(UINT nType, int cx, int cy) 
{
	_OnSize_nType=nType;
	_OnSize_cx=cx;
	_OnSize_cy=cy;

	CView::OnSize(nType, cx, cy);
	
	if (::IsWindow(m_hWnd))
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	
	//Polymodelinfo groupbox
	CButton *grpctrl=(CButton *)GetDlgItem(IDC_D2_POLYMODELINFO_GROUP);
	int grpw=cx-14; if(grpw<0) grpw=0;
	if(grpctrl!=NULL && ::IsWindow(grpctrl->m_hWnd))
		grpctrl->SetWindowPos(NULL,0,0,grpw,124,SWP_NOZORDER | SWP_NOMOVE);

	//Polymodel infos
	CStatic *ctrl;
	int plyw=cx-100; if(plyw<0) plyw=0;
	ctrl=(CStatic *)GetDlgItem(IDC_D2_VERTICIES);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);
	ctrl=(CStatic *)GetDlgItem(IDC_D2_POLYGONS);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);
	ctrl=(CStatic *)GetDlgItem(IDC_D2_TEXTURES);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);
	ctrl=(CStatic *)GetDlgItem(IDC_D2_SOBJ);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);
	ctrl=(CStatic *)GetDlgItem(IDC_D2_GUNS);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);
	ctrl=(CStatic *)GetDlgItem(IDC_D2_TYPEDESC);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);

	//Tab control
	CTabCtrl *tabctrl=(CTabCtrl *)GetDlgItem(IDC_D2_DISPLAY);
	int tabw=cx-14; if(tabw<0) tabw=0;
	int tabh=cy-250; if(tabh<0) tabh=0;
	if(tabctrl!=NULL && ::IsWindow(tabctrl->m_hWnd))
		tabctrl->SetWindowPos(NULL,0,0,tabw,tabh,SWP_NOZORDER | SWP_NOMOVE);	

	//List control
	CListCtrl *lstctrl=(CListCtrl *)GetDlgItem(IDC_D2_TEXTURELIST);
	int lstw=cx-28; if(lstw<0) lstw=0;
	int lsth=cy-303; if(lsth<0) lsth=0;
	if(lstctrl!=NULL && ::IsWindow(lstctrl->m_hWnd))
		lstctrl->SetWindowPos(NULL,0,0,lstw,lsth,SWP_NOZORDER | SWP_NOMOVE);	

	//Positions
	CListBox *lbox=(CListBox *)GetDlgItem(IDC_D2_POSITIONS);
	int posw=cx-14; if(posw<0) posw=0;
	int posy=cy-89;
	if(lbox!=NULL && ::IsWindow(lbox->m_hWnd))
		lbox->SetWindowPos(NULL,7,posy,posw,82,SWP_NOZORDER);	

	//Positions label
	CStatic *labl=(CStatic *)GetDlgItem(IDC_D2_POSITIONS_LABEL);
	int poly=cy-104;
	if(labl!=NULL && ::IsWindow(labl->m_hWnd))
		labl->SetWindowPos(NULL,7,poly,0,0,SWP_NOZORDER | SWP_NOSIZE);
}

void CGamePaneD2::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	OnSize(_OnSize_nType,_OnSize_cx,_OnSize_cy);
}
