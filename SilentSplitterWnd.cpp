#include "stdafx.h"
#include "SilentSplitterWnd.h"

// class implementation
CSilentSplitterWnd::CSilentSplitterWnd()
{
	m_cxSplitter=3;      // put your own values here, to make the splitter fit your needs
	m_cySplitter=3;
	m_cxBorderShare=0;
	m_cyBorderShare=0;
	m_cxSplitterGap=3;
	m_cySplitterGap=3;
}

CSilentSplitterWnd::~CSilentSplitterWnd()
{
}

BEGIN_MESSAGE_MAP(CSilentSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CSilentSplitterWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CSilentSplitterWnd::HitTest(CPoint pt)const
{
	ASSERT_VALID(this);
	// do not allow caller to see mouse hits
	return 0;
}