// TXViewModeToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "modview32.h"
#include "TXViewModeToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTXViewModeToolbar

IMPLEMENT_DYNCREATE(CTXViewModeToolbar, CFormView)

CTXViewModeToolbar::CTXViewModeToolbar()
	: CFormView(CTXViewModeToolbar::IDD)
{
	//{{AFX_DATA_INIT(CTXViewModeToolbar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTXViewModeToolbar::~CTXViewModeToolbar()
{
}

void CTXViewModeToolbar::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTXViewModeToolbar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTXViewModeToolbar, CFormView)
	//{{AFX_MSG_MAP(CTXViewModeToolbar)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTXViewModeToolbar diagnostics

#ifdef _DEBUG
void CTXViewModeToolbar::AssertValid() const
{
	CFormView::AssertValid();
}

void CTXViewModeToolbar::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTXViewModeToolbar message handlers

void CTXViewModeToolbar::Init()
{
	CTabCtrl *c=(CTabCtrl *)GetDlgItem(IDC_TAB1);
	c->InsertItem(0,"Textures");
	c->InsertItem(1,"Blocks");
	c->InsertItem(2,"LogFile");
}

void CTXViewModeToolbar::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (::IsWindow(m_hWnd))
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	CTabCtrl *tc=(CTabCtrl *)GetDlgItem(IDC_TAB1);
	if(tc!=NULL)
	{
		if(::IsWindow(tc->m_hWnd))
			tc->SetWindowPos(NULL,1,2,cx,cy/*cy-2*/,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
	}
	InvalidateRect(NULL);
}

int CTXViewModeToolbar::GetCurrent()
{
	CTabCtrl *c=(CTabCtrl *)GetDlgItem(IDC_TAB1);
	return c->GetCurSel();
}

void CTXViewModeToolbar::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMainFrame *m=static_cast<CMainFrame*>(GetParentFrame());
	m->OnSelchangeTab1(pNMHDR, pResult);
	*pResult = 0;
}

void CTXViewModeToolbar::ShowTextures()
{
	CTabCtrl *c=(CTabCtrl *)GetDlgItem(IDC_TAB1);
	c->SetCurSel(0);

	CMainFrame *m=static_cast<CMainFrame*>(GetParentFrame());
	m->OnSelchangeTab1b();
}

void CTXViewModeToolbar::ShowBlockList()
{
	CTabCtrl *c=(CTabCtrl *)GetDlgItem(IDC_TAB1);
	c->SetCurSel(1);

	CMainFrame *m=static_cast<CMainFrame*>(GetParentFrame());
	m->OnSelchangeTab1b();
}

void CTXViewModeToolbar::ShowLogFile()
{
	CTabCtrl *c=(CTabCtrl *)GetDlgItem(IDC_TAB1);
	c->SetCurSel(2);

	CMainFrame *m=static_cast<CMainFrame*>(GetParentFrame());
	m->OnSelchangeTab1b();
}

