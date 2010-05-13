// GamePaneRF.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "GamePaneRF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGamePaneRF

IMPLEMENT_DYNCREATE(CGamePaneRF, CFormView)

CGamePaneRF::CGamePaneRF()
	: CFormView(CGamePaneRF::IDD)
{
	//{{AFX_DATA_INIT(CGamePaneRF)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGamePaneRF::~CGamePaneRF()
{
}

void CGamePaneRF::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGamePaneRF)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGamePaneRF, CFormView)
	//{{AFX_MSG_MAP(CGamePaneRF)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGamePaneRF diagnostics

#ifdef _DEBUG
void CGamePaneRF::AssertValid() const
{
	CFormView::AssertValid();
}

void CGamePaneRF::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

CMODVIEW32Doc * CGamePaneRF::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}

CMainFrame * CGamePaneRF::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CGamePaneRF::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

/////////////////////////////////////////////////////////////////////////////
// CGamePaneRF message handlers

void CGamePaneRF::Init()
{
	CMODVIEW32Doc *pDoc=GetDocument();

	int num_vertices=0;
	for(int i=0;i<pDoc->m_RF_Model.num_geounits;i++)
		num_vertices+=pDoc->m_RF_Model.geounit[i].mNumVerts;

	int num_polygons=0;
	for(int i=0;i<pDoc->m_RF_Model.num_geounits;i++)
		num_polygons+=pDoc->m_RF_Model.geounit[i].mNumTris;
	

	CStatic *ctrl;
	char *temp=new char[256];
	CString sep=" ";
	ctrl=(CStatic *)GetDlgItem(IDC_RF_VERTICIES);
	ctrl->SetWindowText(sep+_itoa(num_vertices,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_RF_POLYGONS);
	ctrl->SetWindowText(sep+_itoa(num_polygons,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_RF_TEXTURES);
	ctrl->SetWindowText(sep+_itoa(pDoc->m_RF_Model.num_textures,temp,10));
	ctrl=(CStatic *)GetDlgItem(IDC_RF_SOBJ);
	ctrl->SetWindowText(sep+_itoa(pDoc->m_RF_Model.num_geounits,temp,10));

	delete[] temp;
}

