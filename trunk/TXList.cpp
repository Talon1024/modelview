// TXList.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "TXList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTXList

IMPLEMENT_DYNCREATE(CTXList, CListView)

CTXList::CTXList()
{
}

CTXList::~CTXList()
{
}


BEGIN_MESSAGE_MAP(CTXList, CListView)
	//{{AFX_MSG_MAP(CTXList)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTXList drawing

void CTXList::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTXList diagnostics

#ifdef _DEBUG
void CTXList::AssertValid() const
{
	CListView::AssertValid();
}

void CTXList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTXList message handlers

CMainFrame * CTXList::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CTXList::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CTXList::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}

void CTXList::DeleteAllColumns()
{
	for(int i=0;i<20;i++)
		GetListCtrl().DeleteColumn(0);
}

void CTXList::FillTextureInfo(int mode)
{
	current_mode=mode;
	GetMainFrame()->m_TXCurrent=-1; //Nothing selected for now

	GetListCtrl().ModifyStyle(LVS_TYPEMASK,LVS_REPORT | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER,0);
	GetListCtrl().SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	GetListCtrl().DeleteAllItems();
	DeleteAllColumns();

	switch(mode)
	{
	case 0: //Textures
		{
			GetListCtrl().InsertColumn(0,"Name",LVCFMT_LEFT,100/*140*/,0);
			GetListCtrl().InsertColumn(1,"Type",LVCFMT_LEFT,36,0);
			GetListCtrl().InsertColumn(2,"x",LVCFMT_RIGHT,30,2);
			GetListCtrl().InsertColumn(3,"y",LVCFMT_RIGHT,30,3);
			GetListCtrl().InsertColumn(4,"Used",LVCFMT_RIGHT,36,3);
			GetListCtrl().InsertColumn(5,"Filesize",LVCFMT_RIGHT,50,3);
			GetListCtrl().InsertColumn(6,"Status",LVCFMT_LEFT,65,3);
			GetListCtrl().InsertColumn(7,"Location",LVCFMT_LEFT,120,1);
			GetListCtrl().InsertColumn(8,"Type",LVCFMT_LEFT,90,1);

			if(GetDocument()==NULL)
				return;
			for(int i=0; i<GetDocument()->m_TexturesNum+1; i++)
			{
				CString w,h,u,s;
				w.Format("%i",GetDocument()->m_Textures[i].Width);
				h.Format("%i",GetDocument()->m_Textures[i].Height);
				u.Format("%ix",GetDocument()->m_Textures[i].Used);
				s.Format("%i",GetDocument()->m_Textures[i].InSize);
				if(GetDocument()->m_Textures[i].Used==-1)
					u="<Not yet implemented for this game!>";
				if(GetDocument()->m_Textures[i].Width==-1)
				{
					w="???";
					h="???";
					u="???";
					s="???";
				}

				CString t;
				switch(GetDocument()->m_Textures[i].FileType)
				{
				case TEXTUREINFOFILETYPE_UNKNOWN:	t="???"; break;
				//case TEXTUREINFOFILETYPE_OGF:		t="OGF"; break;
				//case TEXTUREINFOFILETYPE_OAF:		t="OAF"; break;
				case TEXTUREINFOFILETYPE_PCX:		t="PCX"; break;
				case TEXTUREINFOFILETYPE_ANI:		t="ANI"; break;
				//case TEXTUREINFOFILETYPE_D2U:		t="D2U"; break;
				//case TEXTUREINFOFILETYPE_D2C:		t="D2C"; break;
				default:	/*ASSERT(FALSE);*/		t="???";
				}

				int itm=GetListCtrl().InsertItem(i,GetDocument()->m_Textures[i].Filename);
				GetListCtrl().SetItemText(itm,1,t);
				GetListCtrl().SetItemText(itm,2,w);
				GetListCtrl().SetItemText(itm,3,h);
				GetListCtrl().SetItemText(itm,4,u);
				GetListCtrl().SetItemText(itm,5,s);
				GetListCtrl().SetItemText(itm,6,GetDocument()->m_Textures[i].Error);
				GetListCtrl().SetItemText(itm,7,GetDocument()->m_Textures[i].Location);
				GetListCtrl().SetItemText(itm,8,GetDocument()->m_Textures[i].Type);
			}
		}
		break;

	case 1: //Block order
		{
			GetListCtrl().InsertColumn(0,"Block",LVCFMT_LEFT,50,0);
			GetListCtrl().InsertColumn(1,"Offset",LVCFMT_RIGHT,60,1);
			GetListCtrl().InsertColumn(2,"Size",LVCFMT_RIGHT,60,1);
			GetListCtrl().InsertColumn(3,"Description",LVCFMT_LEFT,300,1);

			for(int i=0; i<GetDocument()->m_Blocks_Num; i++)
			{
				CString x;

				int itm=GetListCtrl().InsertItem(i,GetDocument()->m_Blocks[i].name);
				x.Format("%i",GetDocument()->m_Blocks[i].offset);
				GetListCtrl().SetItemText(itm,1,x);
				x.Format("%i",GetDocument()->m_Blocks[i].size);
				GetListCtrl().SetItemText(itm,2,x);
				GetListCtrl().SetItemText(itm,3,GetDocument()->m_Blocks[i].desc);
			}
		}
		break;

	case 2: //Log
		{
			GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT);
			GetListCtrl().InsertColumn(0,"Log",LVCFMT_LEFT,550,0);

			CStdioFile ft;
			if(ft.Open(((CMODVIEW32App *)AfxGetApp())->m_LogFile,CFile::modeRead))
			{
				CString x;
				BOOL ret=TRUE;
				int itm=0;
				while(ret)
				{
					ret=ft.ReadString(x);
					GetListCtrl().InsertItem(itm,x);
					itm++;
				}
			}
		}
		break;
	}

	if(GetListCtrl().GetItemCount()>=0)
	{
		//Select first item
		GetMainFrame()->m_TXCurrent=0;
		GetListCtrl().SetItemState(0,LVIS_SELECTED | LVIS_FOCUSED,LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CTXList::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	switch(current_mode)
	{
	case 0: //Textures
		{
			POSITION pos=GetListCtrl().GetFirstSelectedItemPosition();
			if(pos!=NULL)
			{
				GetMainFrame()->m_TXCurrent=GetListCtrl().GetNextSelectedItem(pos);
				GetMainFrame()->RedrawWindow(); /*TODO*/
			}
		}
		break;
	}

	*pResult = 0;
}
