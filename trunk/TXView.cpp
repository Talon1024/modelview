// TXView.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "TXView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTXView

IMPLEMENT_DYNCREATE(CTXView, CScrollView)

CTXView::CTXView()
{
}

CTXView::~CTXView()
{
}


BEGIN_MESSAGE_MAP(CTXView, CScrollView)
	//{{AFX_MSG_MAP(CTXView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTXView drawing

void CTXView::OnDraw(CDC* pDC)
{
#ifdef _WITHTXVIEW
	CDocument* pDoc = GetDocument();
	CString err="";

	pDC->SetBkMode(OPAQUE);
	pDC->SetBkColor((COLORREF)0x000000);
	pDC->SetTextColor((COLORREF)0xFFFFFF);

	CBrush brush;
	brush.CreateSolidBrush((COLORREF)0x000000);

	int txid=GetMainFrame()->m_TXCurrent;
	if(txid!=-1)
	{
		TEXTUREINFO *tx=&GetDocument()->m_Textures[txid];
		if((tx->Flags && TEXTUREINFOFLAG_TEXTURELOADED) && tx->Width!=-1)
		{
			SetScrollSizes(MM_TEXT,CSize(tx->Width,tx->Height));
			CRect rect(tx->Width,0,2000,2000);
			pDC->FillRect(&rect,&brush);
			CRect rect2(0,tx->Height,2000,2000);
			pDC->FillRect(&rect2,&brush);

			if((tx->Flags & TEXTUREINFOFLAG_TEXTUREIS24BIT)==0) 
			{
				BITMAPINFO *psBmpInfo;
				psBmpInfo=(BITMAPINFO *)new BYTE[sizeof(BITMAPINFOHEADER)+(sizeof(RGBQUAD)*256)];
				psBmpInfo->bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
				psBmpInfo->bmiHeader.biWidth        = tx->Width;
				psBmpInfo->bmiHeader.biHeight       = -tx->Height;
				psBmpInfo->bmiHeader.biPlanes       = 1;
				psBmpInfo->bmiHeader.biBitCount     = 8;
				psBmpInfo->bmiHeader.biCompression  = BI_RGB;
				psBmpInfo->bmiHeader.biSizeImage    = 0;
				psBmpInfo->bmiHeader.biXPelsPerMeter= 0;
				psBmpInfo->bmiHeader.biYPelsPerMeter= 0;
				psBmpInfo->bmiHeader.biClrUsed      = 0;
				psBmpInfo->bmiHeader.biClrImportant = 0;

				for (short Entry=0;Entry<256;Entry++)
				{
					psBmpInfo->bmiColors[Entry].rgbRed     =tx->Palette[0][Entry];
					psBmpInfo->bmiColors[Entry].rgbGreen   =tx->Palette[1][Entry];
					psBmpInfo->bmiColors[Entry].rgbBlue    =tx->Palette[2][Entry];
					psBmpInfo->bmiColors[Entry].rgbReserved=0;
				}

				SetDIBitsToDevice(*pDC,0,0,tx->Width,tx->Height,0,0,0,tx->Height,tx->Bitmap,psBmpInfo,DIB_RGB_COLORS);
				delete[] psBmpInfo;
			}
			else
			{
				BITMAPINFO     *psBmpInfo;
				psBmpInfo = (BITMAPINFO *)new BYTE[ sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD)*256) ];
				psBmpInfo->bmiHeader.biSize           = sizeof(BITMAPINFOHEADER);
				psBmpInfo->bmiHeader.biWidth          = tx->Width;
				psBmpInfo->bmiHeader.biHeight         = -tx->Height;
				psBmpInfo->bmiHeader.biPlanes         = 1;
				psBmpInfo->bmiHeader.biBitCount       = 32;
				psBmpInfo->bmiHeader.biCompression    = BI_RGB;
				psBmpInfo->bmiHeader.biSizeImage      = 0;
				psBmpInfo->bmiHeader.biXPelsPerMeter  = 0;
				psBmpInfo->bmiHeader.biYPelsPerMeter  = 0;
				psBmpInfo->bmiHeader.biClrUsed        = 0;
				psBmpInfo->bmiHeader.biClrImportant   = 0;

				SetDIBitsToDevice(*pDC,0,0,tx->Width,tx->Height,0,0,0,tx->Height,tx->Bitmap,psBmpInfo,DIB_PAL_COLORS);
				delete[] psBmpInfo;
			}

		}
		else
			err="Texture not loaded or not available.";
	}
	else
		err="No model with textures loaded.";

	if(err!="")
	{
		SetScrollSizes(MM_TEXT,CSize(200,30));
		CRect rect2(0,0,2000,2000);
		pDC->FillRect(&rect2,&brush);
		pDC->TextOut(0,0,err);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CTXView diagnostics

#ifdef _DEBUG
void CTXView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTXView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTXView message handlers

CMainFrame * CTXView::GetMainFrame()
{
	return static_cast<CMainFrame*>(GetParentFrame());
}

CMODVIEW32View * CTXView::GetMainView()
{
	return (CMODVIEW32View *)GetMainFrame()->GetCurrentView();
}

CMODVIEW32Doc * CTXView::GetDocument()
{
	return (CMODVIEW32Doc *)GetMainFrame()->GetDocument();
}


void CTXView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 1;
	SetScrollSizes(MM_TEXT, sizeTotal);	
}
