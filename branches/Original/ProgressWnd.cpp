// ProgressWnd.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "ProgressWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressWnd dialog


CProgressWnd::CProgressWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgressWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressWnd)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress1);
	DDX_Control(pDX, IDC_FRAME, m_Frame);
	DDX_Control(pDX, IDC_ACTION, m_Action);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressWnd, CDialog)
	//{{AFX_MSG_MAP(CProgressWnd)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressWnd message handlers

int CProgressWnd::Create(CWnd *pParent)
{	
	//{{AFX_DATA_INIT(CSplashWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	if (!CDialog::Create(CProgressWnd::IDD, pParent))
	{
		TRACE0("Warning: creation of CSplash dialog failed\n");
		return FALSE;
	}

	return TRUE;
}

