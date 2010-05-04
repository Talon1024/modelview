// VPFileDialogSubWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MODVIEW32.h"
#include "VPFileDialog.h"
#include "VPFileDialogSubWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVPFileDialogSubWnd

CVPFileDialogSubWnd::CVPFileDialogSubWnd()
{
}

CVPFileDialogSubWnd::~CVPFileDialogSubWnd()
{
}


BEGIN_MESSAGE_MAP(CVPFileDialogSubWnd, CWnd)
	//{{AFX_MSG_MAP(CVPFileDialogSubWnd)
    ON_BN_CLICKED(IDC_GAMEBUTTON, OnGameButton)
	ON_COMMAND(ID_VPFILEDIALOG_JUMPTOREDFACTION1FULLVERSIONDIRECTORY, OnVpfiledialogJumptoredfaction1fullversiondirectory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVPFileDialogSubWnd message handlers

void CVPFileDialogSubWnd::OnGameButton()
{
	CPoint point=((CVPFileDialog *)GetParent())->GetPositionForMenu();

	m_PopupMenu.LoadMenu(IDR_VPFILEDIALOG);
	/*if(m_PopupMenu.m_XPMode==0)
		m_PopupMenu.LoadToolbar(IDR_MENU16);
	else*/
		m_PopupMenu.LoadToolbar(IDR_MENU20);
	ClientToScreen(&point);
	BCMenu *psub = (BCMenu *)m_PopupMenu.GetSubMenu(0);
	psub->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,AfxGetMainWnd());
	m_PopupMenu.DestroyMenu();

	//::MessageBox(NULL, "GameButton pressed", NULL, MB_OK);
}

void CVPFileDialogSubWnd::OnVpfiledialogJumptoredfaction1fullversiondirectory() 
{
	::MessageBox(NULL, "GameButton pressed", NULL, MB_OK);
	
}
