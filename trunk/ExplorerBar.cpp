// ExplorerBar.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "ExplorerBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExplorerBar

IMPLEMENT_DYNCREATE(CExplorerBar, CFormView)

CExplorerBar::CExplorerBar()
	: CFormView(CExplorerBar::IDD)
{
	alreadyinited=FALSE;
	//{{AFX_DATA_INIT(CExplorerBar)
	//}}AFX_DATA_INIT
}

CExplorerBar::~CExplorerBar()
{
}

void CExplorerBar::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExplorerBar)
	DDX_Control(pDX, IDC_TOOLBARPLACEHOLDER, m_ToolBarPlaceHolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExplorerBar, CFormView)
	//{{AFX_MSG_MAP(CExplorerBar)
	ON_COMMAND_RANGE(ID_EXPLORERBAR_TREEVIEW, ID_EXPLORERVIEW_COMPARISONTABLE, OnExplorerBarClick)
	ON_UPDATE_COMMAND_UI(ID_EXPLORERBAR_TREEVIEW, OnUpdateExplorerBarTreeView)
	ON_UPDATE_COMMAND_UI(ID_EXPLORERBAR_FILTER, OnUpdateExplorerBarFilter)
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPLORERVIEW_COMPARISONTABLE, OnExplorerviewComparisontable)
	ON_UPDATE_COMMAND_UI(ID_EXPLORERVIEW_COMPARISONTABLE, OnUpdateExplorerviewComparisontable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExplorerBar diagnostics

#ifdef _DEBUG
void CExplorerBar::AssertValid() const
{
	CFormView::AssertValid();
}

void CExplorerBar::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExplorerBar message handlers

void CExplorerBar::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	if(!alreadyinited) //OnInitialUpdate() is updated twice sometimes, so we need to protect ourselves against it
	{
		m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
		//m_toolBAR.Create(this);
		m_ToolBar.LoadToolBar(IDR_EXPLORER_BAR);

		// For right and left toolbars use CBRS_ALIGN_RIGHT or CBRS_ALIGN_LEFT
		// otherwise use CBRS_ALIGN_ANY. Whichever you use it must match the 
		// orientation of your toolbar which must match the correspondent CStatic 
		// control orientation. Remember though that each toolbar will be fixed to
		// to the location of the CStatic control.
		m_ToolBar.SetBarStyle(CBRS_ALIGN_ANY | CBRS_TOOLTIPS | CBRS_FLYBY);
	
		WINDOWPLACEMENT wndPlmnt;
	
		// Get the window placements of each toolbar holder
		// and set each toolbar to its correspondent area.
		m_ToolBarPlaceHolder.GetWindowPlacement(&wndPlmnt);
		m_ToolBar.GetToolBarCtrl().SetWindowPlacement(&wndPlmnt);
	
	
		m_ToolTip.Create(this, TTS_ALWAYSTIP);
	
		CRect rect;
	
		// Set tooltips for toolbar1
		m_ToolBar.GetToolBarCtrl().GetItemRect(0, rect);
		m_ToolTip.AddTool(&m_ToolBar, ID_EXPLORERBAR_TREEVIEW, rect, ID_EXPLORERBAR_TREEVIEW);
		m_ToolBar.GetToolBarCtrl().GetItemRect(1, rect);
		m_ToolTip.AddTool(&m_ToolBar, ID_EXPLORERBAR_FILTER, rect, ID_EXPLORERBAR_FILTER);
		m_ToolBar.GetToolBarCtrl().GetItemRect(2, rect);
		m_ToolTip.AddTool(&m_ToolBar, ID_EXPLORERBAR_PREV, rect, ID_EXPLORERBAR_PREV);
		m_ToolBar.GetToolBarCtrl().GetItemRect(3, rect);
		m_ToolTip.AddTool(&m_ToolBar, ID_EXPLORERBAR_NEXT, rect, ID_EXPLORERBAR_NEXT);
		m_ToolBar.GetToolBarCtrl().GetItemRect(4, rect);
		m_ToolTip.AddTool(&m_ToolBar, ID_EXPLORERVIEW_COMPARISONTABLE, rect, ID_EXPLORERVIEW_COMPARISONTABLE);
		m_ToolBar.GetToolBarCtrl().SetToolTips(&m_ToolTip);
	
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();
		alreadyinited=TRUE;
	}
	m_ToolBar.GetToolBarCtrl().SetToolTips(&m_ToolTip);
}

void CExplorerBar::OnExplorerBarClick(UINT nID)
{
	CMainFrame *viewFrame=static_cast<CMainFrame*>(GetParentFrame());
	viewFrame->OnExplorerBarClick2(nID);
}

void CExplorerBar::OnUpdateExplorerBarTreeView(CCmdUI *pCmdUI)
{
	CMainFrame *viewFrame=static_cast<CMainFrame*>(GetParentFrame());
	pCmdUI->SetCheck(viewFrame->m_ExplorerIndex);
}

void CExplorerBar::OnUpdateExplorerBarFilter(CCmdUI *pCmdUI)
{
	CMainFrame *viewFrame=static_cast<CMainFrame*>(GetParentFrame());
	pCmdUI->SetCheck(viewFrame->m_ExplorerFilterLowLevel);
	pCmdUI->Enable(viewFrame->GetDocument()->m_Game!=GAME_FS);
}

void CExplorerBar::OnSize(UINT nType, int cx, int cy) 
{
	//This code avoids scroll bars
	CView::OnSize(nType, cx, cy);
	if (::IsWindow(m_hWnd))
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
}

void CExplorerBar::OnExplorerviewComparisontable() 
{
	// TODO: Add your command handler code here
	
}

void CExplorerBar::OnUpdateExplorerviewComparisontable(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
