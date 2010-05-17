// TableList.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "TableList.h"
#include "Explorer.h"
#include "MainFrm.h"
#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "LVSort.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableList dialog


CTableList::CTableList()
	: CDialog(CTableList::IDD)
{
	//{{AFX_DATA_INIT(CTableList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTableList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableList)
	DDX_Control(pDX, IDC_LIST1, m_ListView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTableList, CDialog)
	//{{AFX_MSG_MAP(CTableList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableList message handlers

BOOL CTableList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CMODVIEW32App *m_App=(CMODVIEW32App *)AfxGetApp();
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_App->m_pMainWnd);
	CMODVIEW32View* pView=viewFrame->GetCurrentView();
	CMODVIEW32Doc* pDoc=viewFrame->GetDocument();
	pView->m_RenderMode=RENDER_WIREFRAME;

	m_ListView.InsertColumn(0,"Name",LVCFMT_LEFT,150,0);
	m_ListView.InsertColumn(1,"FileVer",LVCFMT_RIGHT,65,1);
	m_ListView.InsertColumn(2,"Filesize",LVCFMT_RIGHT,65,1);
	m_ListView.InsertColumn(3,"Vertices",LVCFMT_RIGHT,55,1);
	m_ListView.InsertColumn(4,"Polygons",LVCFMT_RIGHT,60,1);
	m_ListView.InsertColumn(5,"Textures",LVCFMT_RIGHT,60,2);
	m_ListView.InsertColumn(6,"Submodels",LVCFMT_RIGHT,65,3);
	m_ListView.InsertColumn(7,"Guns",LVCFMT_RIGHT,40,3);
	//if(pDoc->m_Game==GAME_D2)
	//	m_ListView.InsertColumn(8,"Type",LVCFMT_LEFT,150,3);
	//if(pDoc->m_Game==GAME_D3)
	//	m_ListView.InsertColumn(8,"Positions",LVCFMT_RIGHT,65,3);

	m_ListView.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	modelentry me[1024];
	
	CExplorer *e=viewFrame->ExplorerPane();
	CTreeCtrl *t=viewFrame->ExplorerCtrl();
	e->DoEventhandling=TRUE;

	int i=0;
	int j=0;
	int max=t->GetCount();

	m_App->DisplayProgressWnd("Collecting model information");

	BOOL isamodel;
	pDoc->m_InfoMode=TRUE;
	HTREEITEM k=t->GetRootItem();
	while(k=e->GetNextItem(k))
	{
		isamodel=TRUE;
		CString name=t->GetItemText(k);
		
		if (pDoc->m_Game == GAME_FS)
		{
			name=name+".pof";
			ERRORCODE err=pDoc->FS_LoadVPContent(name);
			if(err!=ERROR_GEN_NOERROR)
			{
#ifdef _DEBUG
				if(err!=ERROR_GEN_NOMODELSELECTED)
				{
					CString x;
					x.Format("ERROR in model %s: Error %i - %s\n",name,err,pDoc->m_ErrorString);
					OutputDebugString(x);
					ASSERT(FALSE);
				}
#endif
				isamodel=FALSE;
			}
		}
		
		if(isamodel)
		{
			me[i].name=t->GetItemText(k);
			me[i].filevers=pDoc->m_FileVersion;
			me[i].filesize=pDoc->m_NumFilesize;
			me[i].vertices=pDoc->m_NumVertices;
			me[i].polygons=pDoc->m_NumPolygons;
			me[i].textures=pDoc->m_NumTextures;
			me[i].submodels=pDoc->m_NumSubmodels;
			me[i].positions=pDoc->m_NumPositions;
			me[i].guns=pDoc->m_Guns.Num;
			me[i].desc=pDoc->m_ModelType_Desc;
			
			/*{
				CString c;
				c.Format("%s: %i, %i, %i, %i\n",me[i].name,me[i].vertices,me[i].polygons,me[i].textures,me[i].submodels);
				OutputDebugString(c);
			}*/
			i++;
		}
		j++;
		m_App->UpdateProgressWnd(j,max);
	}

	m_App->StatusProgressWnd("Building list...");

	CString tS;
	for(j=0;j<i;j++)
	{
		int itm=m_ListView.InsertItem(i,me[j].name);
		tS.Format("%.2f",(float)(me[j].filevers)/100);
		m_ListView.SetItemText(itm,1,tS);
		tS.Format("%i",me[j].filesize);
		m_ListView.SetItemText(itm,2,tS);
		tS.Format("%i",me[j].vertices);
		m_ListView.SetItemText(itm,3,tS);
		tS.Format("%i",me[j].polygons);
		m_ListView.SetItemText(itm,4,tS);
		tS.Format("%i",me[j].textures);
		m_ListView.SetItemText(itm,5,tS);
		tS.Format("%i",me[j].submodels);
		m_ListView.SetItemText(itm,6,tS);
		tS.Format("%i",me[j].guns);
		m_ListView.SetItemText(itm,7,tS);
	}

	m_App->CloseProgressWnd();

	pDoc->m_InfoMode=FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


CListCtrl * CTableList::GetListControl()
{
	return &m_ListView;
}

void CTableList::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	// bAscending will be use to order from lower to higher or higher to lower
	bool bAscending = true;
	
	CSortClass csc(&m_ListView, pNMListView->iSubItem, bAscending);
	//CSortClass csc(&m_ListView, pNMListView->iSubItem, FALSE);
	csc.Sort(FALSE);
	
	*pResult = 0;
}

