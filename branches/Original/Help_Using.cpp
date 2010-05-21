// Help_Using.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "Help_Using.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelp_Using dialog


CHelp_Using::CHelp_Using(CWnd* pParent /*=NULL*/)
	: CDialog(CHelp_Using::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelp_Using)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHelp_Using::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelp_Using)
	DDX_Control(pDX, IDC_HELP_USING_KEYBOARD, m_HelpUsing_Keyboard);
	DDX_Control(pDX, IDC_HELP_USING_MOUSE, m_HelpUsing_Mouse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelp_Using, CDialog)
	//{{AFX_MSG_MAP(CHelp_Using)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelp_Using message handlers

BOOL CHelp_Using::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString txt;
	txt="Rotate - ";
	txt+="You can rotate your model by pressing the left button of your mouse and then drag the model around.\n";
	txt+="\n";
	txt+="Zoom - ";
	txt+="To zoom, hold the right mouse button and move your mouse up to zoom in and down to zoom out.\n";
	txt+="\n";
	txt+="Pane - ";
	txt+="To pane, hold the shift key and any mouse button, then drag the model.\n";
	txt+="\n";
	txt+="You can also reverse the meaning of the mouse buttons in the Options dialog (File menu), to make left mouse button zoom and right mouse button rotate.\n";
	m_HelpUsing_Mouse.SetWindowText(txt);
	
	txt="Rotate - Ctrl+Cursor keys\n";
	txt+="Zoom - Page up/down\n";
	txt+="Pane - Shift+Cursor keys\n";
	txt+="\n";
	txt+="For hotkeys open the menus and you will see each function with its hot key listed.";
	m_HelpUsing_Keyboard.SetWindowText(txt);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
