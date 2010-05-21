// WelcomePane.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "WelcomePane.h"
#include "OptionsDlg.h"
#include "DM_Reg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWelcomePane

IMPLEMENT_DYNCREATE(CWelcomePane, CFormView)

CWelcomePane::CWelcomePane()
	: CFormView(CWelcomePane::IDD)
{
	//{{AFX_DATA_INIT(CWelcomePane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWelcomePane::~CWelcomePane()
{
}

void CWelcomePane::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcomePane)
	DDX_Control(pDX, IDC_WELCOMETEXT, m_WelcomeText);
	DDX_Control(pDX, IDC_NEXTTIPBUTTON, m_NextTipButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWelcomePane, CFormView)
	//{{AFX_MSG_MAP(CWelcomePane)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_NEXTTIP, OnNexttip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWelcomePane diagnostics

#ifdef _DEBUG
void CWelcomePane::AssertValid() const
{
	CFormView::AssertValid();
}

void CWelcomePane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWelcomePane message handlers

void CWelcomePane::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	m_NextTipButton.m_Centered=TRUE;
	ShowNextTip();
	OnSize(_OnSize_nType,_OnSize_cx,_OnSize_cy);
}

void CWelcomePane::OnSize(UINT nType, int cx, int cy) 
{
	_OnSize_nType=nType;
	_OnSize_cx=cx;
	_OnSize_cy=cy;

	CView::OnSize(nType, cx, cy);
	if (::IsWindow(m_hWnd))
		SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER);
		
	if(::IsWindow(m_WelcomeText.m_hWnd))
		m_WelcomeText.SetWindowPos(NULL,2,92,cx-2,cy-92,SWP_NOZORDER);	
}

void CWelcomePane::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	//White background
	RECT cRect;
	GetClientRect(&cRect);
	SetBkColor(dc,RGB(0,0,0));
	dc.FillSolidRect(&cRect,16777215);
}

void CWelcomePane::OnNexttip() 
{
	ShowNextTip();
}

void CWelcomePane::ShowNextTip()
{
	if(!::IsWindow(m_hWnd) || !::IsWindow(m_WelcomeText.m_hWnd))
		return;

	CString txt;

	COptionsDlg dlg;
	CString inifile=dlg.GetINIfile();
	int current_tip=DMReg_ReadHKCUint("TipOfTheDay",0);
	current_tip+=1;

	switch(current_tip)
	{
	case 2:
		txt="Model Comparison Table\n\nDo you want to know which of the original models are the most point/polygon/...-wasting ones?\n\nJust use the 'Model Comparison Table' in the Model menu whenever you have a .HAM, a .HOG or a .VP loaded and it will fetch statistics about every model in the current file.";
		break;

	case 3:
		txt="Guns and Submodels\n\nYou can let MODELVIEW show you the position of guns and submodels by using the upper toolbar.\n\n(Note that this functionality is currently disabled in Descent 3, since it is not yet completed for this game.)";
		break;

	case 4:
		txt="MODELVIEW is ONE tool for FOUR games.\n\nYou can view models from the games Descent 2, Descent 3, Conflict/Descent: FreeSpace and FreeSpace 2. Depending on the model type you are loading, MODELVIEW automatically goes into the mode for the correct game and adapts the toolbars to offer special game-specific features.";
		break;

	case 5:
		txt="Model out of sight?\n\nDid you get lost in the void while rotating, zooming and paning the model? You can quickly reset the view to have the model displayed in the center of the screen with default zoom level by using 'Reset viewpoint' in the Navigation menu or pressing Ctrl+R.\n";
		break;

	case 6:
		txt="Want to code your own OOF/POF editor?\n\nMODELVIEW can help you study the file formats so that you can start right ahead. The 'Models Details' dialog in the Model menu can show you the position and length of every file block in the currently loaded model. This will help you debug your program easier.\n\nAnd finally, you can directly jump to our developer pages with the HAM/HXM, OOF and POF file format specifications by going to the 'Internet' tab in the About dialog box in the Help menu.\n";
		break;

	case 7:
		txt="FreeSpace gun banks\n\nWant to know which guns are used when shooting? MODELVIEW can display you each gun bank and tell you what kind of weapons it shoots (laser or missiles).\n\nTo do this, click the 'Guns' tab in the tab control on the right part of the screen when a FreeSpace model is loaded. You will then see a list of available guns for the current model and can select which guns to mark in the view.\n";
		break;

	case 8:
		txt="Textures\n\nMODELVIEW shows you the list of textures in the right part of the screen, when the 'Textures' tab is active (which is the default).\n\nYou can easily identify which texture is used where on the ship by clicking its name in the list; this will cause MODELVIEW to hide all polygons except the ones that use the texture you have selected. Click on '<All textures>' to display all again.\n";
		break;

	case 9:
		txt="Submodels and positions\n\nSubmodels are part of the models which can be rotated. Descent 2/3 use this function to move the robot arms, in FreeSpace 1/2 the turrets on a ship are submodels.\n\nClicking the 'Show Segments' icon in the upper toolbar will draw red boxes around each submodel.\n\nYou can identify each submodel when you the 'Submodels' tab is active on the right part of the screen by clicking the submodel's name.\n\nPositions in Descent 2/3 are a predefined state how a robot's submodels are rotated, e.g. when it shoots. Just click on an item in the the Position listbox in the lower right of the screen and watch the model. When you have a Descent 2 model loaded, MODELVIEW even changes the position smoothly animated.";
		break;

	default: //the first one
		current_tip=1;
		txt="You can use your mouse to control your model:\n";
		txt+="\n";
		txt+="Rotate\n";
		txt+="You can rotate your model by pressing the left button of your mouse and then drag the model around.\n";
		txt+="\n";
		txt+="Zoom\n";
		txt+="To zoom, hold the right mouse button and move your mouse up to zoom in and down to zoom out.\n";
		txt+="\n";
		txt+="Pane\n";
		txt+="To pane, hold the Shift key and any mouse button, then drag the model.\n";
		txt+="\n";
		txt+="You can also reverse the meaning of the mouse buttons in the Options dialog (File menu), to make left mouse button zoom and right mouse button rotate.\n";
	}
	DMReg_WriteHKCU("TipOfTheDay",current_tip);

	m_WelcomeText.SetWindowText(txt);
	RedrawWindow();
	//InvalidateRect(NULL);
	

}
