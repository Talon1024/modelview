// DataSafe.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "DataSafe.h"
#include "MODVIEW32View.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSafe

CDataSafe::CDataSafe()
{
	
}

CDataSafe::~CDataSafe()
{
}

	int CDataSafe::m_LastWidth = 0;
	int CDataSafe::m_LastHeight = 0;

BEGIN_MESSAGE_MAP(CDataSafe, CWnd)
	//{{AFX_MSG_MAP(CDataSafe)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataSafe message handlers
