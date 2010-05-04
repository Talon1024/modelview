// VPFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include <dlgs.h>
#include "MODVIEW32.h"
#include "VPFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVPFileDialog

IMPLEMENT_DYNAMIC(CVPFileDialog, CFileDialog)

CVPFileDialog::CVPFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CVPFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CVPFileDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CVPFileDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();

	// We need to enlarge standard CFileDialog to make space for our controls
	// idea from Christian Skovdal Andersen article - Customizing CFileDialog
	// This variable should be changed acording to your wishes
	const UINT iExtraSize=50;
	CWnd *wndDlg=GetParent(); // Get a pointer to the original dialog box.
	RECT Rect;
    VERIFY(m_DlgWnd.SubclassWindow(wndDlg->m_hWnd)); // Create a new CMySubClassWnd so we can catch dialog control notifications
	wndDlg->GetWindowRect(&Rect);
	wndDlg->SetWindowPos(NULL,0,0,Rect.right-Rect.left,Rect.bottom-Rect.top+iExtraSize,SWP_NOMOVE); // Change the size of FileOpen dialog

	// Standard CFileDialog control ID's are defined in <dlgs.h>
	CWnd *wndComboCtrl = wndDlg->GetDlgItem(cmb1);	// cmb1 - standard file name combo box control
	wndComboCtrl->GetWindowRect(&Rect);
	wndDlg->ScreenToClient(&Rect); // Remember it is child controls

	// Put our control(s) somewhere below HIDDEN checkbox
	Rect.top+=60;
	Rect.bottom+=60;

	// IMPORTANT: We must put wndDlg here as hWndParents, NOT "this" as
	// written in Microsoft documentation example
	m_GameButton.Create("Quick jump to game directory >", WS_CHILD | WS_VISIBLE, Rect, wndDlg, IDC_GAMEBUTTON);
	m_GameButton.SetFont(wndComboCtrl->GetFont(), TRUE);
	((CMODVIEW32App *)AfxGetApp())->m_GameButton=&m_GameButton;
	((CMODVIEW32App *)AfxGetApp())->m_GameButtonRect=Rect;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE}
}

void CVPFileDialog::OnVpfiledialogJumptoredfaction1fullversiondirectory() 
{
	::MessageBox(NULL, "GameButton1 pressed", NULL, MB_OK);
	
}

void CVPFileDialog::SetPath(CString path)
{
	CWnd* pWnd = GetParent()->GetDlgItem(edt1);
	CWnd* pWnd1 = GetParent()->GetDlgItem(IDOK);
	CListCtrl* pLCtrl = (CListCtrl*)GetParent()->GetDlgItem(lst1);
	
	// Simulate a refresh to clear the current file name
	// .. this can take a LONG time if the history folder is selected!
	//
	// If you find a better way of doing this, please let me know.
	//pLCtrl->PostMessage(WM_KEYDOWN,VK_F5,0x0020001);
	//pLCtrl->PostMessage(WM_KEYUP,VK_F5,0xC0020001);
	
	// Add selection to edit box
	pWnd->SetWindowText(path);
	
	// Simulate a 'Return' key to make selection
	pWnd1->SetFocus();
	pWnd1->PostMessage(WM_KEYDOWN,VK_RETURN,0x0020001);
	pWnd1->PostMessage(WM_KEYUP,VK_RETURN,0xC0020001);
	pWnd->SetFocus();
}

void CVPFileDialog::OnFolderChange()
{
	TRACE("OnFolderChange()\n");
}

CPoint CVPFileDialog::GetPositionForMenu()
{
	RECT Rect;
	//((CVPPBuilder32App *)AfxGetApp())->m_GameButton->GetWindowRect(&Rect);
	Rect=((CMODVIEW32App *)AfxGetApp())->m_GameButtonRect;
	//ScreenToClient(&Rect); // Remember it is child controls
	CPoint ret(Rect.left,Rect.bottom);
TRACE("CPoint %i|%i\n",Rect.left,Rect.bottom);
	
	return ret;
}
