// GamePaneFS.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "GamePaneFS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGamePaneFS

IMPLEMENT_DYNCREATE(CGamePaneFS, CFormView)

CGamePaneFS::CGamePaneFS()
	: CFormView(CGamePaneFS::IDD)
{
	//{{AFX_DATA_INIT(CGamePaneFS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGamePaneFS::~CGamePaneFS()
{
}

void CGamePaneFS::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGamePaneFS)
	DDX_Control(pDX, IDC_FS_DETAILLEVEL, m_MyDetLvls);
	DDX_Control(pDX, IDC_FS_TEXTURELIST, m_MyListBox);
	DDX_Control(pDX, IDC_FS_DISPLAY, m_MyTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGamePaneFS, CFormView)
	//{{AFX_MSG_MAP(CGamePaneFS)
	ON_NOTIFY(NM_CLICK, IDC_DETAILLEVEL, OnClickDetaillevel)
	ON_BN_CLICKED(IDC_SHOWSHIELD, OnShowshield)
	ON_LBN_SELCHANGE(IDC_FS_TEXTURELIST, OnSelchangeFsTexturelist)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FS_DISPLAY, OnSelchangeFsDisplay)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGamePaneFS diagnostics

#ifdef _DEBUG
void CGamePaneFS::AssertValid() const
{
	CFormView::AssertValid();
}

void CGamePaneFS::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CMainFrame * CGamePaneFS::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CGamePaneFS::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CGamePaneFS::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CGamePaneFS message handlers

void CGamePaneFS::Init()
{
	CMODVIEW32Doc *pDoc=GetDocument();

	CListView *cl=(CListView *)GetDlgItem(IDC_FS_DETAILLEVEL);
	CListCtrl *ct=&cl->GetListCtrl();
	ct->DeleteAllItems();
	for(unsigned int i=0;i<10;i++)
		ct->DeleteColumn(0);
	ct->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	ct->InsertColumn(0,"Detail",LVCFMT_LEFT,40,0);
	ct->InsertColumn(1,"vert",LVCFMT_RIGHT,36,1);
	ct->InsertColumn(2,"poly",LVCFMT_RIGHT,36,2);
	ct->InsertColumn(3,"txtr",LVCFMT_RIGHT,36,3);
	ct->InsertColumn(4,"sobj",LVCFMT_RIGHT,36,4);

	int vert,poly,txtr,sobj;
	char tstr[256];

	for(unsigned int i=0; i<6; i++)
	{
		if(pDoc->m_FS_PMinfo.n_detail >= i+1)
		{
			sprintf(tstr,"Lvl %i",i+1);
			ct->InsertItem(i,tstr);
			GetPolyInfo(i,&vert,&poly,&txtr,&sobj);
			ct->SetItemText(i,1,_itoa(vert,tstr,10));
			ct->SetItemText(i,2,_itoa(poly,tstr,10));
			ct->SetItemText(i,3,_itoa(txtr,tstr,10));
			ct->SetItemText(i,4,_itoa(sobj,tstr,10));
		}
	}
	if(pDoc->m_FS_PMinfo.n_detail > 0)
	{
		GetPolyInfo(9,&vert,&poly,&txtr,&sobj);
		pDoc->m_FS_ModelHasDebris=FALSE;
		if(vert>0)
		{
			pDoc->m_FS_ModelHasDebris=TRUE;
			ct->InsertItem(6,"Debris");
			ct->SetItemText(pDoc->m_FS_PMinfo.n_detail,1,_itoa(vert,tstr,10));
			ct->SetItemText(pDoc->m_FS_PMinfo.n_detail,2,_itoa(poly,tstr,10));
			ct->SetItemText(pDoc->m_FS_PMinfo.n_detail,3,_itoa(txtr,tstr,10));
			ct->SetItemText(pDoc->m_FS_PMinfo.n_detail,4,_itoa(sobj,tstr,10));
		}
	}

	CStatic *cs=(CStatic *)GetDlgItem(IDC_GUNS);
	sprintf_s(tstr,"%i / %i",pDoc->m_FS_RealGuns[0].Num+pDoc->m_FS_RealGuns[1].Num,pDoc->m_FS_Turrets[0].Num+pDoc->m_FS_Turrets[1].Num);
	cs->SetWindowText(tstr);

	cs=(CStatic *)GetDlgItem(IDC_SHIPSIZE);
	sprintf_s(tstr,"%.0f|%.0f|%.0f",pDoc->m_FS_Model._maxbox.x-pDoc->m_FS_Model._minbox.x,pDoc->m_FS_Model._maxbox.y-pDoc->m_FS_Model._minbox.y,pDoc->m_FS_Model._maxbox.z-pDoc->m_FS_Model._minbox.z);
	cs->SetWindowText(tstr);

	CTabCtrl *ts=(CTabCtrl *)GetDlgItem(IDC_FS_DISPLAY);
	if(ts->GetItemCount()==0)
	{
		ts->DeleteAllItems();
		ts->InsertItem(0,"Textures");
		ts->InsertItem(1,"Submodels");
		ts->InsertItem(2,"Subsystems");
		ts->InsertItem(3,"Guns");
	}
	FillTab();
}

void CGamePaneFS::FillTab()
{
	int i;
	CMODVIEW32Doc *pDoc=GetDocument();
	CMODVIEW32View *view=GetMainView();

	CListBox *lbox=(CListBox *)GetDlgItem(IDC_FS_TEXTURELIST);
	CTabCtrl *ts=(CTabCtrl *)GetDlgItem(IDC_FS_DISPLAY);

	//Reset settings
	view->m_DisplayTexture=-1;
	view->m_DisplaySubmodel=-1;
	view->m_FS_DisplaySubsystem=-1;
	view->m_DisplayGun=-1;

	int num_sobj=0;

	switch(ts->GetCurSel())
	{
	case 0:
		lbox->ResetContent();
		lbox->AddString("<All textures>");
		if(pDoc->m_FS_BitmapData.count > 0)
		{
			for(unsigned int m=pDoc->m_FS_PofDataL[0];(m<pDoc->m_FS_PofDataH[0]+1) && (m < 60);m++)
				lbox->AddString(pDoc->m_FS_BitmapData.pic[m].name);
			lbox->SetCurSel(0);
		}
		break;

	case 1:
		lbox->ResetContent();
		lbox->AddString("<All submodels>");
		
		for(int m=0;m<pDoc->m_FS_NumSOBJ;m++)
		{
			if(pDoc->m_FS_SOBJ[m].detail==view->m_Detaillevel)
			{
				CString tstr;
				tstr.Format("#%i: %s",num_sobj,pDoc->m_FS_SOBJ[m].submodel_name);
				lbox->AddString(tstr);
				num_sobj++;
			}
		}
		lbox->SetCurSel(0);
		break;

	case 2:
		lbox->ResetContent();
		lbox->AddString("<No subsystems>");
		for(i=0;i<pDoc->m_FS_NumSubsystems;i++)
			lbox->AddString(pDoc->m_FS_Subsystems[i].Desc);
		lbox->SetCurSel(0);
		break;

	case 3:
		lbox->ResetContent();
		pDoc->m_NumGunsGroups=0;
		for(i=0;i<pDoc->m_Guns.Num;i++)
		{
			int actgroup=-1;
			for(int j=0;j<pDoc->m_NumGunsGroups;j++)
			{
				if(pDoc->m_GunsGroup[j].GunType==pDoc->m_Guns.GunType[i] && pDoc->m_GunsGroup[j].GunBank==pDoc->m_Guns.GunBank[i])
					actgroup=j;
			}
			if(actgroup==-1)
			{
				pDoc->m_GunsGroup[pDoc->m_NumGunsGroups].GunType=pDoc->m_Guns.GunType[i];
				pDoc->m_GunsGroup[pDoc->m_NumGunsGroups].GunBank=pDoc->m_Guns.GunBank[i];
				pDoc->m_GunsGroup[pDoc->m_NumGunsGroups].Num=1;
				pDoc->m_NumGunsGroups++;
			}
			else
				pDoc->m_GunsGroup[actgroup].Num++;
		}
		for(i=0;i<pDoc->m_NumGunsGroups;i++)
		{
			CString entry,x;
			switch(pDoc->m_GunsGroup[i].GunType)
			{
			case GUNTYPE_PLAYER_GUN: x="Primary Gun"; break;
			case GUNTYPE_PLAYER_MIS: x="Secondary Gun"; break;
			case GUNTYPE_TURRET_GUN: x="Laser Turret"; break;
			case GUNTYPE_TURRET_MIS: x="Missile Launcher"; break;
			default: x="???";
			}

			entry.Format("%s (Bank %i) x%i",x,pDoc->m_GunsGroup[i].GunBank,pDoc->m_GunsGroup[i].Num);
			lbox->AddString(entry);
		}
		lbox->SetCurSel(0);
		OnSelchangeFsTexturelist();
		break;

	default:
		ASSERT(FALSE);
	}

	view->InvalidateRect(NULL);
}

void CGamePaneFS::GetPolyInfo(int detaillevel, int *vert, int *poly, int *txtr, int *sobj)
{
	CMODVIEW32Doc *pDoc=GetDocument();

	int num_vert=0;
	for(unsigned int m=0;m<pDoc->m_FS_Model.Vcount;m++)
	{
		if(pDoc->m_FS_SOBJ[pDoc->m_FS_Model.VSobj[m]].detail==(long)detaillevel)
			num_vert++;
	}

	int num_poly=0;
	for(unsigned int m=0;m<pDoc->m_FS_Model.Pcount;m++)
	{
		if(pDoc->m_FS_SOBJ[pDoc->m_FS_Model.Poly[m].Sobj].detail==(long)detaillevel)
			num_poly++;
	}

	int num_txtr=0;
	for(unsigned int m=0;m<pDoc->m_FS_BitmapData.count;m++)
	{
		if(pDoc->m_FS_BitmapData.pic[m].valid==1)
			num_txtr++;
	}

	int num_sobj=0;
	for(int m=0;m<pDoc->m_FS_NumSOBJ;m++)
	{
		if(pDoc->m_FS_SOBJ[m].detail==(long)detaillevel)
			num_sobj++;
	}
	*vert=num_vert;
	*poly=num_poly;
	*txtr=num_txtr;
	*sobj=num_sobj;
}

void CGamePaneFS::OnClickDetaillevel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMODVIEW32View *view=GetMainView();

	CListView *cl=(CListView *)GetDlgItem(IDC_DETAILLEVEL);
	CListCtrl *ct=&cl->GetListCtrl();
	int nItem=-1;
	POSITION pos=ct->GetFirstSelectedItemPosition();
	while((pos!=NULL) && (nItem==-1))
	{
		nItem=ct->GetNextSelectedItem(pos);
		if(ct->GetItemState(nItem,LVIS_FOCUSED)!=LVIS_FOCUSED)
			nItem=-1;
	}

	if(nItem==ct->GetItemCount()-1 && GetDocument()->m_FS_ModelHasDebris)
		view->FS_SetDetailLevel(9);
	else
	{
		switch(nItem)
		{
		case 0:
			view->FS_SetDetailLevel(0);
			break;
		case 1:
			view->FS_SetDetailLevel(1);
			break;
		case 2:
			view->FS_SetDetailLevel(2);
			break;
		case 3:
			view->FS_SetDetailLevel(3);
			break;
		case 4:
			view->FS_SetDetailLevel(4);
			break;
		case 5:
			view->FS_SetDetailLevel(5);
			break;
		}
	}
	*pResult = 0;
}

void CGamePaneFS::OnShowshield() 
{
	GetMainFrame()->FS_ShowShield();
}

void CGamePaneFS::UpdateIt(BOOL ShowShield, BOOL ShowThruster, BOOL UpdateDisplayList)
{
	/*CButton *cb=(CButton *)GetDlgItem(IDC_SHOWSHIELDS);
	cb->SetCheck(ShowShield);
	cb=(CButton *)GetDlgItem(IDC_SHOWTHRUSTERS);
	cb->SetCheck(ShowThruster);*/
	
	CMODVIEW32View *view=GetMainView();
	
	CListView *cl=(CListView *)GetDlgItem(IDC_FS_DETAILLEVEL);
	CListCtrl *ct=&cl->GetListCtrl();
	ASSERT(cl);
	ASSERT(::IsWindow(cl->m_hWnd));
	
	int tomark=(int)view->m_Detaillevel;
	if(view->m_Detaillevel==9)
		tomark=ct->GetItemCount()-1;

	for(int i=0;i<ct->GetItemCount();i++)
	{
		UINT smask=0;

		/*LVITEM pItem;
		pItem.mask=0;
		pItem.iItem=i;
		pItem.iSubItem=0;
		ASSERT(ct->GetItem(&pItem));*/
		if(i==tomark)
			smask=LVIS_SELECTED | LVIS_FOCUSED;

		//ct->SetItem(i,0,LVIF_STATE,"",0,pItem.state,smask,0);
		ct->SetItemState(i,smask,LVIS_SELECTED | LVIS_FOCUSED);
	}
		
	if(UpdateDisplayList)
		FillTab();
}

void CGamePaneFS::OnSelchangeFsTexturelist() 
{
	CMODVIEW32View *view=GetMainView();

	CListBox *lbox=(CListBox *)GetDlgItem(IDC_FS_TEXTURELIST);
	CTabCtrl *ts=(CTabCtrl *)GetDlgItem(IDC_FS_DISPLAY);

	switch(ts->GetCurSel())
	{
	case 0:
		view->m_DisplayTexture=lbox->GetCurSel()-1;
		break;
		
	case 1:
		view->m_DisplaySubmodel=lbox->GetCurSel()-1;
		break;

	case 2:
		view->m_FS_DisplaySubsystem=lbox->GetCurSel()-1;
		break;

	case 3:
		view->m_DisplayGun=lbox->GetCurSel();
		break;

	default:
		ASSERT(FALSE);
	}

	view->InvalidateRect(NULL);
}

void CGamePaneFS::OnSelchangeFsDisplay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	FillTab();	
	*pResult=0;
}

void CGamePaneFS::OnSize(UINT nType, int cx, int cy) 
{
	_OnSize_nType=nType;
	_OnSize_cx=cx;
	_OnSize_cy=cy;

	CView::OnSize(nType, cx, cy);

	if (::IsWindow(m_hWnd))
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	
	//Detaillevels
	int dtlw=cx-14; if(dtlw<0) dtlw=0;
	if(::IsWindow(m_MyDetLvls.m_hWnd))
		m_MyDetLvls.SetWindowPos(NULL,0,0,dtlw,124,SWP_NOZORDER | SWP_NOMOVE);	
	
	//Tab
	int tabw=cx-14; if(tabw<0) tabw=0;
	int tabh=cy-180; if(tabh<0)	tabh=0;
	if(::IsWindow(m_MyTab.m_hWnd))
		m_MyTab.SetWindowPos(NULL,0,0,tabw,tabh,SWP_NOZORDER | SWP_NOMOVE);	
	
	//Listbox
	int lstw=cx-28; if(lstw<0) lstw=0;
	int lsth=cy-244; if(lsth<0)	lsth=0;
	if(::IsWindow(m_MyListBox.m_hWnd))
		m_MyListBox.SetWindowPos(NULL,0,0,lstw,lsth,SWP_NOZORDER | SWP_NOMOVE);
}


void CGamePaneFS::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	OnSize(_OnSize_nType,_OnSize_cx,_OnSize_cy);
}
