// StaticWithColor.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "StaticWithColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticWithColor

CStaticWithColor::CStaticWithColor()
{
	m_Centered=FALSE;
	m_Font = new CFont;

	m_Font->CreateFont(	-11,				//nHeight
						0,					//nWidth
						0,					//nEscapement
						0,					//nOrientation
						400,				//nWeight
						0,					//bItalic
						0,					//bUnderline
						0,					//cStrikeOut
						0,					//nCharSet
						1,					//nOutPrecision
						2,					//nClipPrecision
						1,					//nQuality
						DEFAULT_PITCH | FF_SWISS | TMPF_TRUETYPE, //34, 				//nPitchAndFamily
						"Tahoma"//"Arial"	//		//lpszFacename  "Arial"	//
						);
	m_HeaderMode=FALSE;
}

CStaticWithColor::~CStaticWithColor()
{
	m_Font->DeleteObject();
	delete(m_Font);
}


BEGIN_MESSAGE_MAP(CStaticWithColor, CStatic)
	//{{AFX_MSG_MAP(CStaticWithColor)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticWithColor message handlers

void CStaticWithColor::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);

	CString t;
	GetWindowText(t);

	dc.SetBkColor(16777215);
	dc.FillSolidRect(rect,16777215);
	dc.SetTextColor(0);

	CFont *pOldfont = NULL;
	pOldfont = (CFont *)dc.SelectObject(m_Font);
	//dc.ExtTextOut( rect.left, 0, ETO_OPAQUE | ETO_CLIPPED, rect, t, NULL );
	//dc.TextOut( rect.left, 0,  t );

	int par=DT_WORDBREAK;
	if(m_Centered)
		par+=(DT_CENTER + DT_VCENTER + DT_SINGLELINE);
	dc.DrawText(t,rect,par);

	dc.SelectObject(pOldfont);
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CStaticWithColor::SetHeaderMode()
{
	m_Font->DeleteObject();
	m_Font->CreateFont(	-15,				//nHeight
						0,					//nWidth
						0,					//nEscapement
						0,					//nOrientation
						700,				//nWeight
						0,					//bItalic
						0,					//bUnderline
						0,					//cStrikeOut
						0,					//nCharSet
						1,					//nOutPrecision
						2,					//nClipPrecision
						1,					//nQuality
						DEFAULT_PITCH | FF_SWISS | TMPF_TRUETYPE, //34, 				//nPitchAndFamily
						"Tahoma"//"Arial"	//		//lpszFacename  "Arial"	//
						);
	m_HeaderMode=TRUE;
}
