// GamePaneD3.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "GamePaneD3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGamePaneD3

IMPLEMENT_DYNCREATE(CGamePaneD3, CFormView)

CGamePaneD3::CGamePaneD3()
	: CFormView(CGamePaneD3::IDD)
{
	//{{AFX_DATA_INIT(CGamePaneD3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGamePaneD3::~CGamePaneD3()
{
}

void CGamePaneD3::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGamePaneD3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGamePaneD3, CFormView)
	//{{AFX_MSG_MAP(CGamePaneD3)
	ON_NOTIFY(TCN_SELCHANGE, IDC_D3_DISPLAY, OnSelchangeD3Display)
	ON_LBN_SELCHANGE(IDC_D3_POSITIONS1, OnSelchangeD3Positions)
	ON_LBN_SELCHANGE(IDC_D3_TEXTURELIST, OnSelchangeD3Texturelist)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGamePaneD3 diagnostics

#ifdef _DEBUG
void CGamePaneD3::AssertValid() const
{
	CFormView::AssertValid();
}

void CGamePaneD3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CMainFrame * CGamePaneD3::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CGamePaneD3::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CGamePaneD3::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CGamePaneD3 message handlers

void CGamePaneD3::Init()
{
	CMODVIEW32Doc *pDoc=GetDocument();

	CStatic *ctrl;
	char *temp=new char[256];
	CString sep=" ";
	ctrl=(CStatic *)GetDlgItem(IDC_D3_VERTICIES);
	ctrl->SetWindowText(sep+itoa(pDoc->m_D3_Model.Vcount,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_D3_POLYGONS);
	ctrl->SetWindowText(sep+itoa(pDoc->m_D3_Model.Pcount,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_D3_TEXTURES);
	ctrl->SetWindowText(sep+itoa(pDoc->m_D3_TotalTexture,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_D3_SOBJ);
	ctrl->SetWindowText(sep+itoa(pDoc->m_D3_Model.Scount,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_D3_GUNS);
	ctrl->SetWindowText(sep+itoa(pDoc->m_Guns.Num,temp,10));

	CTabCtrl *ts=(CTabCtrl *)GetDlgItem(IDC_D3_DISPLAY);
	if(ts->GetItemCount()==0)
	{
		ts->DeleteAllItems();
		ts->InsertItem(0,"Textures");
		ts->InsertItem(1,"Submodels");
	}
	FillTab();

	CListBox *lbox=(CListBox *)GetDlgItem(IDC_D3_POSITIONS);
	lbox->ResetContent();
	lbox->AddString("<default>");
	for(unsigned long i=0;i<pDoc->m_D3_Display.TotalKey;i++)
	{
		sprintf(temp,"Position #%i",i);
		lbox->AddString(temp);
	}
	lbox->SetCurSel(0);


	delete(temp);
}

void CGamePaneD3::FillTab()
{
	unsigned long i;
	CMODVIEW32Doc *pDoc=GetDocument();
	CMODVIEW32View *view=GetMainView();

	CListBox *lbox=(CListBox *)GetDlgItem(IDC_D3_TEXTURELIST);
	CTabCtrl *ts=(CTabCtrl *)GetDlgItem(IDC_D3_DISPLAY);

	//Reset settings
	view->m_DisplayTexture=-1;
	view->m_DisplaySubmodel=-1;
	view->m_DisplayGun=-1;

	switch(ts->GetCurSel())
	{
	case 0:
		lbox->ResetContent();
		lbox->AddString("<All textures>");
		{
			int n_texs=0;
			for(i=0;i<pDoc->m_D3_TotalTexture;i++)
			{
				CString texname=pDoc->m_D3_TextureList[i];
				if(pDoc->m_Textures[i].Used!=0)
				{
					ASSERT(n_texs<MAX_N_TEXS);
					lbox->AddString(texname);
					m_sel2id[n_texs]=i;
					n_texs++;
				}
				else
					OutputDebugString("Skipping unused texture "+texname+"\n");
			}
		}
		lbox->SetCurSel(0);
		break;

	case 1:
		lbox->ResetContent();
		lbox->AddString("<All submodels>");
		for(i=0;i<pDoc->m_D3_Model.Scount;i++)
		{
			CString tstr;
			tstr.Format("#%i: %s",i,pDoc->m_D3_Model.Name[i]);
			lbox->AddString(tstr);
		}
		lbox->SetCurSel(0);
		break;

	default:
		ASSERT(FALSE);
	}

	view->InvalidateRect(NULL);
}

void CGamePaneD3::OnSelchangeD3Display(NMHDR* pNMHDR, LRESULT* pResult) 
{
	FillTab();
	*pResult = 0;
}

void CGamePaneD3::OnSelchangeD3Positions() 
{
	CMODVIEW32View *view=GetMainView();
	CListBox *lbox=(CListBox *)GetDlgItem(IDC_D3_POSITIONS);
	view->D3_SetPosition(lbox->GetCurSel());
}

void CGamePaneD3::OnSelchangeD3Texturelist() 
{
	CMODVIEW32View *view=GetMainView();

	CListBox *lbox=(CListBox *)GetDlgItem(IDC_D3_TEXTURELIST);
	CTabCtrl *ts=(CTabCtrl *)GetDlgItem(IDC_D3_DISPLAY);

	switch(ts->GetCurSel())
	{
	case 0:
		if(lbox->GetCurSel()!=0)
			view->m_DisplayTexture=m_sel2id[lbox->GetCurSel()-1];
		else
			view->m_DisplayTexture=-1;
		break;
		
	case 1:
		view->m_DisplaySubmodel=lbox->GetCurSel()-1;
		break;

	default:
		ASSERT(FALSE);
	}

	view->RedrawWindow();	
}

void CGamePaneD3::OnPositionChange()
{
	OnSelchangeD3Positions();
}

void CGamePaneD3::OnSize(UINT nType, int cx, int cy) 
{
	_OnSize_nType=nType;
	_OnSize_cx=cx;
	_OnSize_cy=cy;

	CView::OnSize(nType, cx, cy);

	if (::IsWindow(m_hWnd))
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	
	//Polymodelinfo groupbox
	CButton *grpctrl=(CButton *)GetDlgItem(IDC_D3_POLYMODELINFO_GROUP);
	int grpw=cx-14; if(grpw<0) grpw=0;
	if(grpctrl!=NULL && ::IsWindow(grpctrl->m_hWnd))
		grpctrl->SetWindowPos(NULL,0,0,grpw,107,SWP_NOZORDER | SWP_NOMOVE);

	//Polymodel infos
	CStatic *ctrl;
	int plyw=cx-100; if(plyw<0) plyw=0;
	ctrl=(CStatic *)GetDlgItem(IDC_D3_VERTICIES);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);
	ctrl=(CStatic *)GetDlgItem(IDC_D3_POLYGONS);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);
	ctrl=(CStatic *)GetDlgItem(IDC_D3_TEXTURES);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);
	ctrl=(CStatic *)GetDlgItem(IDC_D3_SOBJ);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);
	ctrl=(CStatic *)GetDlgItem(IDC_D3_GUNS);
	if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
		ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);
	//ctrl=(CStatic *)GetDlgItem(IDC_D3_TYPEDESC);
	//if(ctrl!=NULL && ::IsWindow(ctrl->m_hWnd))
	//	ctrl->SetWindowPos(NULL,0,0,plyw,16,SWP_NOZORDER | SWP_NOMOVE);

	//Tab control
	CTabCtrl *tabctrl=(CTabCtrl *)GetDlgItem(IDC_D3_DISPLAY);
	int tabw=cx-14; if(tabw<0) tabw=0;
	int tabh=cy-233; if(tabh<0) tabh=0;
	if(tabctrl!=NULL && ::IsWindow(tabctrl->m_hWnd))
		tabctrl->SetWindowPos(NULL,0,0,tabw,tabh,SWP_NOZORDER | SWP_NOMOVE);	

	//List control
	CListCtrl *lstctrl=(CListCtrl *)GetDlgItem(IDC_D3_TEXTURELIST);
	int lstw=cx-28; if(lstw<0) lstw=0;
	int lsth=cy-286; if(lsth<0) lsth=0;
	if(lstctrl!=NULL && ::IsWindow(lstctrl->m_hWnd))
		lstctrl->SetWindowPos(NULL,0,0,lstw,lsth,SWP_NOZORDER | SWP_NOMOVE);	

	//Positions
	CListBox *lbox=(CListBox *)GetDlgItem(IDC_D3_POSITIONS);
	int posw=cx-14; if(posw<0) posw=0;
	int posy=cy-89;
	if(lbox!=NULL && ::IsWindow(lbox->m_hWnd))
		lbox->SetWindowPos(NULL,7,posy,posw,82,SWP_NOZORDER);	

	//Positions label
	CStatic *labl=(CStatic *)GetDlgItem(IDC_D3_POSITIONS_LABEL);
	int poly=cy-104;
	if(labl!=NULL && ::IsWindow(labl->m_hWnd))
		labl->SetWindowPos(NULL,7,poly,0,0,SWP_NOZORDER | SWP_NOSIZE);	
	
}

void CGamePaneD3::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	OnSize(_OnSize_nType,_OnSize_cx,_OnSize_cy);
}
