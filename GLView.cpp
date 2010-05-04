// GLView.cpp : implementation of the CGLView class
//

#include "stdafx.h"
#include "mainfrm.h"

#include "GLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
// CGLView

IMPLEMENT_DYNCREATE(CGLView, CView)

BEGIN_MESSAGE_MAP(CGLView, CView)
	//{{AFX_MSG_MAP(CGLView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	// Palette handlers
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////
// CGLView construction/destruction

CGLView::CGLView() :
	m_hPalette(NULL)
{
	ASSERT(FALSE); //HeikoH: do we still need this code?
}

CGLView::~CGLView()
{
}

BOOL CGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Set up style to suit OpenGL
    cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS |
         CS_OWNDC,
        ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1), NULL);

    

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////
// CGLView printing

BOOL CGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////
// CGLView drawing

void CGLView::OnDraw(CDC* pDC)
{
		// Draw the OpenGL scene
    
}

/////////////////////////////////////////////////////////////////////
// CGLView diagnostics

#ifdef _DEBUG
void CGLView::AssertValid() const
{
	CView::AssertValid();
}

void CGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////
// CGLView helpers

void CGLView::SetupPalette()
{
	LOGICALPALETTE pal = { 0x300, 256 };
	BYTE baRed[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE baGreen[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE baBlue[] = {0, 85, 170, 255};

	// Fill our logical palette structure with color data
	for ( int iColor = 0; iColor < 256; ++iColor ) {
		pal.peaEntries[iColor].peRed =
								baRed[iColor & 0x07];
		pal.peaEntries[iColor].peGreen =
								baGreen[(iColor >> 0x03) & 0x07];
		pal.peaEntries[iColor].peBlue =
								baBlue[(iColor >> 0x06) & 0x03];
		pal.peaEntries[iColor].peFlags = 0;
	} // for

	// Build our palette
	m_hPalette = CreatePalette((LOGPALETTE*)&pal);
}

/////////////////////////////////////////////////////////////////////
// CGLView message handlers required by OpenGL

int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize OpenGL parameters
	PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), // Structure size
        1,                             // Version number
        PFD_DRAW_TO_WINDOW |           // Property flags
            PFD_SUPPORT_OPENGL |
            PFD_DOUBLEBUFFER,          // (remove if no double buf)
        PFD_TYPE_RGBA,                 // PixelType
        24,                            // 24-bit color
        0, 0, 0, 0, 0, 0,              // Color bits and shift
        0, 0, 0, 0, 0, 0, 0,           // Alpha and accum buffer bits
        32,                            // 32-bit depth buffer
        0, 0,                          // No stencil or aux buffer
        PFD_MAIN_PLANE,                // Layer type
        0,                             // Reserved
        0, 0, 0                        // Unsupported
    };

	// Tell GDI to convert device context from Win32 to
	// OpenGL.
    CClientDC dcClient(this);
    int iPixelFormat = ChoosePixelFormat(dcClient.m_hDC,&pfd);
    if ( !iPixelFormat ) {
		// This system cannot run OpenGL
		TRACE("Error retrieving pixel format index...\n");
		ASSERT(FALSE);
		AfxMessageBox("Cannot initialize OpenGL...quitting.",
						MB_OK | MB_ICONERROR);
		return -1; // will fail new document creation...
	} // if

    if ( !SetPixelFormat(dcClient.m_hDC,iPixelFormat,&pfd) ) {
		// This system cannot run OpenGL
		TRACE("Error setting new pixel format...\n");
		ASSERT(FALSE);
		AfxMessageBox("Cannot initialize OpenGL...quitting.",
						MB_OK | MB_ICONERROR);
		return -1; // will fail new document creation...
	} // if

	// Update the PIXELFORMATDESCRIPTOR structure once
	// the device context has been modified.
    DescribePixelFormat(dcClient.m_hDC,iPixelFormat,
						sizeof(pfd),&pfd);

	// The PIXELFORMATDESCRIPTOR has been updated, so we now
	// determine whether to create and manage a custom
	// palette.
    if ( pfd.dwFlags & PFD_NEED_PALETTE ) {
		// We do, so build a new palette...
        SetupPalette();
	} // if
	
	// Create the OpenGL rendering context
    m_hRC = wglCreateContext(dcClient.m_hDC);
    if ( m_hRC == NULL ) {
		// This system cannot run OpenGL
		TRACE("Error creating OpenGL rendering context...\n");
		ASSERT(FALSE);
		AfxMessageBox("Cannot initialize OpenGL...quitting.",
						MB_OK | MB_ICONERROR);
		return -1; // will fail new document creation...
	} // if

	// We now make it the current rendering context so
	// we might set our clear color
    wglMakeCurrent(dcClient.m_hDC,m_hRC);
	glClearColor(0.0f,0.0f,0.0f,0.0f); // black
    wglMakeCurrent(dcClient.m_hDC,NULL);

	return 0;
}

BOOL CGLView::DestroyWindow() 
{
	// Shut down OpenGL by deleting the rendering context
    wglDeleteContext(m_hRC);

	// Delete our custom palette, if any was created
	if ( m_hPalette != NULL ) {
	    DeleteObject(m_hPalette);
	} // if
	
	return CView::DestroyWindow();
}

BOOL CGLView::OnEraseBkgnd(CDC* pDC) 
{
	// Don't erase view...let OpenGL handle
	// that for us...
	return TRUE;
}

void CGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	m_ClientRect.right = cx;
		m_ClientRect.bottom = cy;
	// Change OpenGL's rendering context size and
	// viewing frustrum.
	CClientDC dcClient(this);
	//double aspect;
	CSize size(cx,cy);
	VERIFY(wglMakeCurrent(dcClient.m_hDC,m_hRC));
/*	aspect = (cy == 0) ? (double)size.cx : (double)size.cx/(double)size.cy;

	glViewport(0,0,size.cx,size.cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-10.0,10.0,-10.0,10.0,18.0,70.0);
	gluPerspective(30,aspect,1,500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-65.0f);
	wglMakeCurrent(NULL,NULL);*/

	nRange = 50.0f;
	if(cy == 0)
		cy = 1;
	//lastWidth = cx;
	//lastHeight = cy;
	// Set Viewport to window dimensions
    glViewport(0, 0, cx, cy);

	glMatrixMode(GL_PROJECTION);

	// Reset coordinate system
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
    if (cx <= cy) 
		glOrtho (-nRange, nRange, -nRange*cy/cx, nRange*cy/cx, -nRange*5, nRange*5);
    else 
		glOrtho (-nRange*cx/cy, nRange*cx/cy, -nRange, nRange, -nRange*5, nRange*5);

	//glScalef(60.0f, 60.0f, 60.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void CGLView::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CView::OnPaletteChanged(pFocusWnd);
	
	// Manage our palette
	if ( m_hPalette ) {
		CDC* pDC = GetDC();
	    SelectPalette(pDC->m_hDC,m_hPalette,FALSE);
	    RealizePalette(pDC->m_hDC);
		ReleaseDC(pDC);

		// Redraw
		Invalidate();
	} // if
}

BOOL CGLView::OnQueryNewPalette() 
{
	// Manage our palette
	if ( m_hPalette ) {
		CDC* pDC = GetDC();
	    SelectPalette(pDC->m_hDC,m_hPalette,FALSE);
	    RealizePalette(pDC->m_hDC);
		ReleaseDC(pDC);

		// Redraw
		Invalidate();
	} // if

	return CView::OnQueryNewPalette();
}

/////////////////////////////////////////////////////////////////////
// Non-OpenGL CGLView message handlers

void CGLView::DrawCartAxis()
{

	glPushMatrix();
	glNewList(Axis = glGenLists(1), GL_COMPILE);	
	glBegin(GL_LINES);
		// light red x axis arrow
		glColor3f(1.f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		// light green y axis arrow
		glColor3f(0.0f,1.f,0.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		// light blue z axis arrow
		glColor3f(0.0f,0.0f,1.f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,1.0f);
	/*glEnd();
	glBegin(GL_LINES);*/
		// x letter & arrowhead
		glColor3f(1.f,0.0f,0.0f);
		glVertex3f(1.1f,0.1f,0.0f);
		glVertex3f(1.3f,-0.1f,0.0f);
		glVertex3f(1.3f,0.1f,0.0f);
		glVertex3f(1.1f,-0.1f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(0.9f,0.1f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(0.9f,-0.1f,0.0f);
		// y letter & arrowhead
		glColor3f(.0f,1.f,0.0f);
		glVertex3f(-0.1f,1.3f,0.0f);
		glVertex3f(0.f,1.2f,0.0f);
		glVertex3f(0.1f,1.3f,0.0f);
		glVertex3f(0.f,1.2f,0.0f);
		glVertex3f(0.f,1.2f,0.0f);
		glVertex3f(0.f,1.1f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(0.1f,0.9f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glVertex3f(-0.1f,0.9f,0.0f);
		// z letter & arrowhead
		glColor3f(.0f,.0f,1.f);
		
		glVertex3f(0.0f,-0.1f,1.3f);
		glVertex3f(0.0f,-0.1f,1.1f);
		glVertex3f(0.0f,-0.1f,1.1f);
		glVertex3f(0.0f,0.1f,1.3f);
		glVertex3f(0.0f,0.1f,1.3f);
		glVertex3f(0.0f,0.1f,1.1f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,0.1f,0.9f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,-0.1f,0.9f);
	glEnd();

	glEndList();
	//glPushMatrix();
}

void CGLView::DrawGrid()
{

// Draw the 3 center grid axis lines
	glColor3d(0.3,0.3,0.3);
	glLineWidth(2.0);
	glBegin(GL_LINES); 

	glVertex3f(0.0,21.0,0.0);
	glVertex3f(0.0,-21.0,0.0);

	glVertex3f(21.0,0.0,0.0);
	glVertex3f(-21.0,0.0,0.0);

	glVertex3f(0.0,0.0,21.0);
	glVertex3f(0.0,0.0,-21.0);

	glEnd();


// Draw the grid lines
	glColor3d(0.2,0.2,0.2);
	glLineWidth(1.0);

	for(float f = 1.0;f<=20.0;f++)
	{
		glBegin(GL_LINES);

		glVertex3f(20.0,f,0.0); //XY Grid
		glVertex3f(-20.0,f,0.0);
	
		glVertex3f(20.0,-f,0.0);
		glVertex3f(-20.0,-f,0.0);

		glVertex3f(f,20.0,0.0);
		glVertex3f(f,-20.0,0.0);

		glVertex3f(-f,20.0,0.0);
		glVertex3f(-f,-20.0,0.0);
	
		glVertex3f(f,0.0,20.0); //XZ Grid
		glVertex3f(f,0.0,-20.0);
	
		glVertex3f(-f,0.0,20.0);
		glVertex3f(-f,0.0,-20.0);
	
		glVertex3f(20.0,0.0,f);
		glVertex3f(-20.0,0.0,f);
		
		glVertex3f(20.0,0.0,-f);
		glVertex3f(-20.0,0.0,-f);
		
		glVertex3f(0.0,f,20.0); //yz grid
		glVertex3f(0.0,f,-20.0);
		
		glVertex3f(0.0,-f,20.0);
		glVertex3f(0.0,-f,-20.0);
		
		glVertex3f(0.0,20.0,f);
		glVertex3f(0.0,-20.0,f);
	
		glVertex3f(0.0,20.0,-f);
		glVertex3f(0.0,-20.0,-f);


		glEnd();


	}


	glLineWidth(1.0); //resets the line width
	///////////////
}


void CGLView::RenderIndicator()
{
	glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	glRotatef(m_yRotation, 0.0, 1.0, 0.0);
	glPushMatrix();
	
	//"superimpose" the RGB-carthesian-axes in a small area near the bottom-left corner of the view
	glViewport(m_ClientRect.right - (m_ClientRect.right/6),0,m_ClientRect.right/6,m_ClientRect.bottom/6);
	//
	glPopMatrix();

	glScalef(30.0,30.0,30.0);

	glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glCallList(Axis);
	glPopAttrib();
	glViewport(0,0,m_ClientRect.right,m_ClientRect.bottom);
	glPopMatrix();
}



void CGLView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_RightButtonDown = TRUE;
	m_RightDownPos = point;
	if(nFlags == 10)
	{
		m_RBControl = TRUE;
	}
	
	CView::OnRButtonDown(nFlags, point);
}

void CGLView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_RightButtonDown = FALSE;
	m_RBControl = FALSE;
	
	CView::OnRButtonUp(nFlags, point);
}

void CGLView::OnMouseMove(UINT nFlags, CPoint point) 
{
	static int last_x = 0;
	static int last_y = 0;
	float ZoomFactor = 0.0;
	//int  delta_x = point.x-last_x;
	//int  delta_y = point.y-last_y;
	CMainFrame *viewFrame=static_cast<CMainFrame*>(GetParentFrame());

	if(m_LeftButtonDown) // Zoom
	{
	
		ZoomFactor = (float)(m_LeftDownPos.y - point.y)/100.0f;
		
		m_xScaling+=ZoomFactor;
		m_yScaling+=ZoomFactor;
		m_zScaling+=ZoomFactor;
		if(m_xScaling < (float)0.01) m_xScaling = (float)0.01;
		if(m_yScaling < (float)0.01) m_yScaling = (float)0.01;
		if(m_zScaling < (float)0.01) m_zScaling = (float)0.01;

		m_LeftDownPos = point;
		InvalidateRect(NULL,FALSE);

		
		viewFrame->m_ZoomLevel = m_xScaling; //for the status of the zoom level
		
		
	}

	if(m_RightButtonDown && !m_RBControl) //Rotation of the scene
	{
		m_yRotation -= (float)(m_RightDownPos.x - point.x)/2.0f;
		m_xRotation -= (float)(m_RightDownPos.y - point.y)/2.0f;
		m_RightDownPos = point;
		if(m_yRotation < 0) m_yRotation = m_yRotation + 360;
		if(m_yRotation >= 360) m_yRotation = m_yRotation - 360;
		if(m_xRotation < 0) m_xRotation = m_xRotation + 360;
		if(m_xRotation >= 360) m_xRotation = m_xRotation - 360;
		
		InvalidateRect(NULL,FALSE);
		viewFrame->m_YRotation = m_yRotation; // for the status of the rotation.
		viewFrame->m_XRotation = m_xRotation;
	}

	if(m_RBControl) //panning
	{
	
		if(last_x > point.x)
		{
			m_xTranslation += (float)(point.x - last_x)/(float)7.5;
		}
		else
		{
			m_xTranslation -= (float)(last_x - point.x)/(float)7.5;
		}

		if(last_y > point.y)
		{
			m_yTranslation -= (float)(point.y - last_y)/(float)7.5;
		}
		else
		{
			m_yTranslation += (float)(last_y - point.y)/(float)7.5;
		}
		
		InvalidateRect(NULL,FALSE);
		viewFrame->m_YTranslation = m_yTranslation; // for the status of the panning.
		viewFrame->m_XTranslation = m_xTranslation;
	}
	
	last_x = point.x;
	last_y = point.y;
	
	CView::OnMouseMove(nFlags, point);
}

void CGLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;
	CView::OnLButtonDown(nFlags, point);
}

void CGLView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_LeftButtonDown = FALSE;
	
	CView::OnLButtonUp(nFlags, point);
}
