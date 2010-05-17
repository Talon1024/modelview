// Explorer.cpp : implementation file
//

#include "stdafx.h"
#include "MODVIEW32.h"
#include "Explorer.h"
#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExplorer

IMPLEMENT_DYNCREATE(CExplorer, CListView)

CExplorer::CExplorer()
{
	DoEventhandling=TRUE;
}

CExplorer::~CExplorer()
{
}


BEGIN_MESSAGE_MAP(CExplorer, CTreeView)
	//{{AFX_MSG_MAP(CExplorer)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExplorer drawing

void CExplorer::OnDraw(CDC* pDC)
{
	CDocument* pDoc=GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CExplorer diagnostics

#ifdef _DEBUG
void CExplorer::AssertValid() const
{
	CTreeView::AssertValid();
}

void CExplorer::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExplorer message handlers


void CExplorer::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if(DoEventhandling)
	{
		//GetTreeCtrl().ModifyStyle(0, TVS_SHOWSELALWAYS | TVS_FULLROWSELECT | TVS_HASLINES | TVS_HASLINES);

		HTREEITEM itm=GetTreeCtrl().GetSelectedItem();
		ASSERT(itm!=NULL);
		if (!itm) return;
		CString name=GetTreeCtrl().GetItemText(itm);

	
		CMainFrame *viewFrame=static_cast<CMainFrame*>(GetParentFrame());
		CMODVIEW32View *view=(CMODVIEW32View *)viewFrame->GetActiveView();
		//CMODVIEW32Doc *viewDoc=(CMODVIEW32Doc *)view->GetDocument();
		CMODVIEW32Doc *viewDoc=(CMODVIEW32Doc *)viewFrame->GetDocument();
	
		if (viewDoc->m_Game == GAME_FS)	{
			name=name+".pof";
			viewFrame->m_ZoomLevel=1.0;
			viewDoc->m_ErrorCode=viewDoc->FS_LoadVPContent(name);
		}
	}

	*pResult=0;
}

BOOL CExplorer::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL ret=CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	GetTreeCtrl().ModifyStyle(0, TVS_SHOWSELALWAYS | TVS_FULLROWSELECT | TVS_HASLINES | TVS_HASBUTTONS);
	return ret;
}

// GetNextItem	- Get next item as if outline was completely expanded
// Returns		- The item immediately below the reference item
// hItem		- The reference item
HTREEITEM CExplorer::GetNextItem(HTREEITEM hItem)
{
	HTREEITEM hti;

	if(GetTreeCtrl().ItemHasChildren(hItem))
		return GetTreeCtrl().GetChildItem(hItem);		// return first child
	else
	{
		// return next sibling item
		// Go up the tree to find a parent's sibling if needed.
		while((hti=GetTreeCtrl().GetNextSiblingItem(hItem))==NULL)
		{
			if((hItem=GetTreeCtrl().GetParentItem(hItem))==NULL)
				return NULL;
		}
	}
	return hti;
}

