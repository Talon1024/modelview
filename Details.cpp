// Details.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "Details.h"
#include "MODVIEW32.h"
#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetails dialog


CDetails::CDetails()
	: CDialog(CDetails::IDD)
{
	texviewvisible=FALSE;
	m_TextureData=NULL;
	//{{AFX_DATA_INIT(CDetails)
	//}}AFX_DATA_INIT
}

CDetails::~CDetails()
{
	if(m_TextureData!=NULL)
	{
		delete(m_TextureData);
		m_TextureData=NULL;
	}
}

void CDetails::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetails)
	DDX_Control(pDX, IDC_TEXTUREVIEW, m_TextureView);
	DDX_Control(pDX, IDC_TEXTUREVIEWBUTTON, m_TexViewState);
	DDX_Control(pDX, IDC_TABS, m_Tabs);
	DDX_Control(pDX, IDC_LISTVIEW, m_ListView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetails, CDialog)
	//{{AFX_MSG_MAP(CDetails)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABS, OnSelchangeTabs)
	ON_BN_CLICKED(IDC_TEXTUREVIEWBUTTON, OnTextureview)
	ON_WM_PAINT()
	ON_NOTIFY(NM_CLICK, IDC_LISTVIEW, OnClickListview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetails message handlers

BOOL CDetails::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowPos(NULL,0,0,595,395,SWP_NOZORDER | SWP_NOMOVE);
	
	CMODVIEW32App *m_App=(CMODVIEW32App *)AfxGetApp();
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_App->m_pMainWnd);

	m_Tabs.InsertItem(0,"Texture information");
	//if(viewFrame->current_game!=GAME_D2)
		m_Tabs.InsertItem(1,"Block order");
	m_ListView.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	FillTextureInformation();

	return FALSE; //TRUE if there is an OK button!!
}

void CDetails::OnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	switch(m_Tabs.GetCurSel())
	{
	case 0:
		FillTextureInformation();
		break;

	case 1:
		FillBlockOrder();
		break;

	default:
		ASSERT(FALSE);
	}

	*pResult = 0;
}

void CDetails::DeleteAllColumns()
{
	for(int i=0;i<20;i++)
		m_ListView.DeleteColumn(0);
}

void CDetails::FillTextureInformation()
{
	m_ListView.DeleteAllItems();
	DeleteAllColumns();
	m_ListView.InsertColumn(0,"Name",LVCFMT_LEFT,140,0);
	m_ListView.InsertColumn(1,"Location",LVCFMT_LEFT,140,1);
	m_ListView.InsertColumn(2,"Type",LVCFMT_LEFT,90,1);
	m_ListView.InsertColumn(3,"Width",LVCFMT_RIGHT,50,2);
	m_ListView.InsertColumn(4,"Height",LVCFMT_RIGHT,50,3);
	m_ListView.InsertColumn(5,"Used",LVCFMT_RIGHT,60,3);

	CMODVIEW32App *m_App=(CMODVIEW32App *)AfxGetApp();
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_App->m_pMainWnd);
	//CMODVIEW32View *m_view=(CMODVIEW32View *)viewFrame->GetCurrentView(); //GetNextView(pos);
	CMODVIEW32Doc *viewDoc=(CMODVIEW32Doc *)viewFrame->GetDocument();

	for(int i=0; i<viewDoc->m_TexturesNum+1; i++)
	{
		CString w,h,u;
		w.Format("%i",viewDoc->m_Textures[i].Width);
		h.Format("%i",viewDoc->m_Textures[i].Height);
		u.Format("%ix",viewDoc->m_Textures[i].Used);
		if(viewDoc->m_Textures[i].Used==-1)
			u="<Not yet implemented for this game!>";
		if(viewDoc->m_Textures[i].Width==-1)
		{
			w="???";
			h="???";
			u="???";
		}

		int itm=m_ListView.InsertItem(i,viewDoc->m_Textures[i].Filename);
		m_ListView.SetItemText(itm,1,viewDoc->m_Textures[i].Location);
		m_ListView.SetItemText(itm,2,viewDoc->m_Textures[i].Type);
		m_ListView.SetItemText(itm,3,w);
		m_ListView.SetItemText(itm,4,h);
		m_ListView.SetItemText(itm,5,u);
	}
}

//FillSubmodelInformation
//m_ListView.InsertColumn(0,"#",LVCFMT_LEFT,140,0);
//m_ListView.InsertColumn(1,"Name",LVCFMT_LEFT,140,0);
//m_ListView.InsertColumn(0,"Polygons",LVCFMT_LEFT,140,0);
//m_ListView.InsertColumn(0,"Size of bounding points",LVCFMT_LEFT,140,0);

void CDetails::FillBlockOrder()
{
	m_ListView.DeleteAllItems();
	DeleteAllColumns();
	m_ListView.InsertColumn(0,"Block",LVCFMT_LEFT,50,0);
	m_ListView.InsertColumn(1,"Offset",LVCFMT_RIGHT,60,1);
	m_ListView.InsertColumn(2,"Size",LVCFMT_RIGHT,60,1);
	m_ListView.InsertColumn(3,"Description",LVCFMT_LEFT,300,1);

	CMODVIEW32App *m_App=(CMODVIEW32App *)AfxGetApp();
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_App->m_pMainWnd);
	//CMODVIEW32View *m_view=(CMODVIEW32View *)viewFrame->GetCurrentView(); //GetNextView(pos);
	CMODVIEW32Doc *viewDoc=(CMODVIEW32Doc *)viewFrame->GetDocument();
	for(int i=0; i<viewDoc->m_Blocks_Num; i++)
	{
		CString x;

		int itm=m_ListView.InsertItem(i,viewDoc->m_Blocks[i].name);
		x.Format("%i",viewDoc->m_Blocks[i].offset);
		m_ListView.SetItemText(itm,1,x);
		x.Format("%i",viewDoc->m_Blocks[i].size);
		m_ListView.SetItemText(itm,2,x);
		m_ListView.SetItemText(itm,3,viewDoc->m_Blocks[i].desc);
	}
		
}







//Texture view

void CDetails::ShowTextureView(BOOL show)
{
	if(show)
		SetWindowPos(NULL,0,0,595,595,SWP_NOZORDER | SWP_NOMOVE);
	else
		SetWindowPos(NULL,0,0,595,395,SWP_NOZORDER | SWP_NOMOVE);
	texviewvisible=show;

/*	if(show && !texviewvisible)
	{
		texviewvisible=TRUE;
		m_TexView.CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,NULL,NULL,WS_VISIBLE | WS_POPUPWINDOW | WS_CHILD,0,0,300,300,m_hWnd,NULL);

		m_TexView.ShowWindow(TRUE);
		m_TexView.SetScrollSizes(MM_TEXT,CSize(100,100));
		m_TexView.RedrawWindow();
	}
	
	if(!show && texviewvisible)
	{
		texviewvisible=FALSE;
		m_TexView.DestroyWindow();
	}*/
}

void CDetails::OnTextureview() 
{
	ShowTextureView(m_TexViewState.GetCheck()==1);
	//if(m_Tabs.GetCurSel()==0)
	//	GetTexture();
}

void CDetails::OnClickListview(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//if(m_Tabs.GetCurSel()==0)
	//	GetTexture();
	*pResult = 0;
}

void CDetails::GetTexture()
{
	//Get name of the texture and index
	int index=m_ListView.GetSelectionMark();
	if(index==-1)
		index=0;


	if(m_TextureData!=NULL)
	{
		delete(m_TextureData);
		m_TextureData=NULL;
	}

	CMODVIEW32App *m_App=(CMODVIEW32App *)AfxGetApp();
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_App->m_pMainWnd);
	CMODVIEW32Doc *viewDoc=(CMODVIEW32Doc *)viewFrame->GetDocument();

	m_TextureResX=viewDoc->m_Textures[index].Width;
	m_TextureResY=viewDoc->m_Textures[index].Height;
	m_TextureResL=m_TextureResX*m_TextureResY;
	m_TextureData=new unsigned char[m_TextureResL];

{
CString out;
out.Format("Index %i, %i x %i",index,m_TextureResX,m_TextureResY);
OutputDebugString(out);
}

	switch(viewFrame->current_game)
	{
	/*case GAME_D2:
		{
			memcpy(m_TextureData,viewDoc->m_D2_BitmapData.bitmap[index].bitmap_array,m_TextureResL);
			memcpy(&m_TexturePale,&viewDoc->m_D2_Palettes[viewDoc->m_D2_TextureSet],768);
			for(int i=0;i<768;i++)
			{
				ASSERT(m_TexturePale[i]<64);
				m_TexturePale[i]*=4;
			}
		}
		break;*/

	case GAME_FS:
		{
			memcpy(m_TextureData,&viewDoc->m_FS_ModelTexture[index],m_TextureResL);
			for(int i=0;i<768;i++)
				m_TexturePale[i]=i/3;
		}
	}

	//Blah
	RECT rect;
	rect.left=0;
	rect.top=370;
	rect.right=800;
	rect.bottom=800;
	InvalidateRect(&rect,TRUE);
}

void CDetails::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(texviewvisible)
	{
		CRect mr;
		WINDOWPLACEMENT pl;
		m_TextureView.GetWindowPlacement(&pl);
		m_TextureView.GetClientRect(mr);
		int x=pl.rcNormalPosition.left;
		int y=pl.rcNormalPosition.top;
		int cx=mr.Width();
		int xy=mr.Height();


	/*{
		CString dbg;
		dbg.Format("%i,%i %ix%i\n",x,y,cx,xy);
		OutputDebugString(dbg);
	}*/


		//Paint
		/*BITMAPINFO     *psBmpInfo;
		psBmpInfo = (BITMAPINFO *)new BYTE[ sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD)*256) ];
		psBmpInfo->bmiHeader.biSize           = sizeof(BITMAPINFOHEADER);
		psBmpInfo->bmiHeader.biWidth          = m_TextureResX;
		psBmpInfo->bmiHeader.biHeight         = -m_TextureResY;
		psBmpInfo->bmiHeader.biPlanes         = 1;
		psBmpInfo->bmiHeader.biBitCount       = 8;
		psBmpInfo->bmiHeader.biCompression    = BI_RGB;
		psBmpInfo->bmiHeader.biSizeImage      = 0;
		psBmpInfo->bmiHeader.biXPelsPerMeter  = 0;
		psBmpInfo->bmiHeader.biYPelsPerMeter  = 0;
		psBmpInfo->bmiHeader.biClrUsed        = 0;
		psBmpInfo->bmiHeader.biClrImportant   = 0;

		for (short Entry=0;Entry<256;Entry++)
		{
			psBmpInfo->bmiColors[Entry].rgbRed       = m_TexturePale[Entry*3+0];
			psBmpInfo->bmiColors[Entry].rgbGreen     = m_TexturePale[Entry*3+1];
			psBmpInfo->bmiColors[Entry].rgbBlue      = m_TexturePale[Entry*3+2];
			psBmpInfo->bmiColors[Entry].rgbReserved  = 0;
		}*/

		//Manual draw
		int maxx=m_TextureResX;
		int maxy=m_TextureResY;
		for(int cury=0; cury<maxy; cury++)
		{
			for(int curx=0; curx<maxx; curx++)
			{
				unsigned char pii=*(m_TextureData+curx+cury*m_TextureResX);
				int pix=pii;
				COLORREF col=m_TexturePale[pix*3+0]+m_TexturePale[pix*3+1]*256+m_TexturePale[pix*3+2]*65536;
				dc.SetPixel(x+curx,y+cury,col);
			}
		}


		//SetDIBitsToDevice(dc,0,0,m_TextureResX,m_TextureResY,0,0,0,m_TextureResY,m_TextureData,psBmpInfo,DIB_RGB_COLORS);
		//DrawBitmap(&dc,(CBitmap &)m_TextureData,&m_TexturePale);
		//delete(psBmpInfo);
	}

/*	dc.MoveTo(CPoint(0,0));
	dc.LineTo(CPoint(x,y));*/
	// Do not call CDialog::OnPaint() for painting messages
}




void CDetails::DrawBitmap(CDC *pDC, CBitmap &bitmap, CPalette *pPal)
{
	// Create a compatible memory DC
	CDC memDC;
	memDC.CreateCompatibleDC( pDC );
	memDC.SelectObject( &bitmap );

	// Select and realize the palette
	if( pPal != NULL && pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		pDC->SelectPalette( pPal, FALSE );
		pDC->RealizePalette();
	}

	BITMAP bm;
	bitmap.GetBitmap( &bm );

	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0,SRCCOPY);
}
