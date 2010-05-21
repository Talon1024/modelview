// SplashNew.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "SplashNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashNew dialog


CSplashNew::CSplashNew(CWnd* pParent /*=NULL*/)
	: CDialog(CSplashNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplashNew)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSplashNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplashNew)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplashNew, CDialog)
	//{{AFX_MSG_MAP(CSplashNew)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashNew message handlers

BOOL CSplashNew::Create(CWnd *pParent)
{
	//{{AFX_DATA_INIT(CSplashWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	if (!CDialog::Create(CSplashNew::IDD, pParent))
	{
		TRACE0("Warning: creation of CSplashNew dialog failed\n");
		return FALSE;
	}

	return TRUE;
}
