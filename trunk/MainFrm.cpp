// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"

#include <direct.h>
#include <errno.h>

#include "DM_Reg.h"
#include "MODVIEW32.h"
#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"

#include "MainFrm.h"
#include "Explorer.h"
#include "ExplorerBar.h"
#include "OpenDlg.h"
#include "OptionsDlg.h"
#include "ImportDlg.h"
#include "Details.h"
#include "TableList.h"
#include "Help_Using.h"
#include "WelcomePane.h"
#include "GamePaneD2.h"
#include "GamePaneD3.h"
#include "GamePaneFS.h"
#include "GamePaneRF.h"

#ifdef _WITHHTVIEW
#include "HTMLTaskView.h"
#endif

#ifdef _WITHEDITOR
#include "EditorFS_MODEL.h"
#include "EditorFS_SPCL.h"
#include "EditorFS_TGUN.h"
#include "EditorFS_GPNT.h"
#include "EditorFS_SHLD.h"
#include "EditorFS_SOBJ.h"
#include "EditorFS_FUEL.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BOOL st=FALSE;


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_DETAILLEVEL_DEBRIS, OnViewDetaillevelDebris)
	ON_COMMAND(ID_VIEW_DETAILLEVEL_LEVEL1, OnViewDetaillevelLevel1)
	ON_COMMAND(ID_VIEW_DETAILLEVEL_LEVEL2, OnViewDetaillevelLevel2)
	ON_COMMAND(ID_MODEL_LEFT, OnModelLeft)
	ON_COMMAND(ID_MODEL_RIGHT, OnModelRight)
	ON_COMMAND(ID_MODEL_UP, OnModelUp)
	ON_COMMAND(ID_MODEL_ZOOMIN, OnModelZoomin)
	ON_COMMAND(ID_MODEL_ZOOMOUT, OnModelZoomout)
	ON_COMMAND(ID_VIEW_DETAILLEVEL_LEVEL3, OnViewDetaillevelLevel3)
	ON_COMMAND(ID_VIEW_DETAILLEVEL_LEVEL4, OnViewDetaillevelLevel4)
	ON_COMMAND(ID_VIEW_DETAILLEVEL_LEVEL5, OnViewDetaillevelLevel5)
	ON_COMMAND(ID_VIEW_DETAILLEVEL_LEVEL6, OnViewDetaillevelLevel6)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILLEVEL_LEVEL1, OnUpdateViewDetaillevelLevel1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILLEVEL_LEVEL2, OnUpdateViewDetaillevelLevel2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILLEVEL_LEVEL3, OnUpdateViewDetaillevelLevel3)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILLEVEL_LEVEL4, OnUpdateViewDetaillevelLevel4)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILLEVEL_LEVEL5, OnUpdateViewDetaillevelLevel5)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILLEVEL_LEVEL6, OnUpdateViewDetaillevelLevel6)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILLEVEL_DEBRIS, OnUpdateViewDetaillevelDebris)
	ON_COMMAND(ID_VIEW_RENDERMODE_WIREFRAME, OnViewRendermodeWireframe)
	ON_COMMAND(ID_VIEW_RENDERMODE_SURFACE, OnViewRendermodeSurface)
	ON_COMMAND(ID_VIEW_RENDERMODE_TEXTURED, OnViewRendermodeTextured)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERMODE_WIREFRAME, OnUpdateViewRendermodeWireframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERMODE_TEXTURED, OnUpdateViewRendermodeTextured)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERMODE_SURFACE, OnUpdateViewRendermodeSurface)
	ON_COMMAND(ID_VIEW_SHOWSHIELDS, OnViewShowshields)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWSHIELDS, OnUpdateViewShowshields)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTHRUSTER, OnUpdateViewShowthruster)
	ON_COMMAND(ID_VIEW_SHOWTHRUSTER, OnViewShowthruster)
	ON_COMMAND(ID_VIEW_FASTTEXTURELOAD, OnViewFasttextureload)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FASTTEXTURELOAD, OnUpdateViewFasttextureload)
	ON_COMMAND(ID_FILE_OPTIONSCONFIGURATION, OnFileOptionsconfiguration)
	ON_COMMAND(ID_FILE_OPENFILE, OnFileOpenfile)
	ON_COMMAND(ID_VIEW_RENDERSETTINGS_SMOOTH, OnViewRendersettingsSmooth)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERSETTINGS_SMOOTH, OnUpdateViewRendersettingsSmooth)
	ON_COMMAND(ID_VIEW_RENDERSETTINGS_CULLFACE, OnViewRendersettingsCullface)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERSETTINGS_CULLFACE, OnUpdateViewRendersettingsCullface)
	ON_COMMAND(ID_MODEL_DOWN, OnModelDown)
	ON_COMMAND(ID_VIEW_RENDERSETTINGS_ZBUFFERED, OnViewRendersettingsZbuffered)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERSETTINGS_ZBUFFERED, OnUpdateViewRendersettingsZbuffered)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_NAVIGATION_RESETZOOMLEVEL, OnNavigationResetzoomlevel)
	ON_COMMAND(ID_VIEW_SELECTBACKGROUNDCOLOR, OnViewSelectbackgroundcolor)
	ON_COMMAND(ID_VIEW_DOROTATION, OnViewDorotation)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DOROTATION, OnUpdateViewDorotation)
	ON_WM_TIMER()
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	ON_WM_INITMENUPOPUP()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_ZOOM, OnUpdateZoom)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_ROTATION, OnUpdateRotation)
	ON_COMMAND(ID_MODEL_NEXT, OnModelNext)
	ON_COMMAND(ID_MODEL_PREVIOUS, OnModelPrevious)
	ON_COMMAND(ID_MODEL_TEXTURELIST, OnModelTexturelist)
	ON_UPDATE_COMMAND_UI(ID_MODEL_TEXTURELIST, OnUpdateModelTexturelist)
	ON_WM_QUERYENDSESSION()
	ON_WM_CHILDACTIVATE()
	ON_WM_PARENTNOTIFY()
	ON_UPDATE_COMMAND_UI(ID_MODEL_NEXT, OnUpdateModelNext)
	ON_UPDATE_COMMAND_UI(ID_MODEL_PREVIOUS, OnUpdateModelPrevious)
	ON_UPDATE_COMMAND_UI(ID_MODEL_LEFT, OnUpdateNavigation)
	ON_COMMAND(ID_VIEW_SPECIALDESCENT3SETTINGS_NEXTPOSITION, OnViewSpecialdescent3settingsNextposition)
	ON_COMMAND(ID_VIEW_SPECIALDESCENT3SETTINGS_PREVIOUSPOSITION, OnViewSpecialdescent3settingsPreviousposition)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPECIALDESCENT3SETTINGS_NEXTPOSITION, OnUpdateViewSpecialdescent3settingsNextposition)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPECIALDESCENT3SETTINGS_PREVIOUSPOSITION, OnUpdateViewSpecialdescent3settingsPreviousposition)
	ON_COMMAND(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_GROUPA, OnViewSpecialdescent2settingsMaintexturesetGroupa)
	ON_COMMAND(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_WATER, OnViewSpecialdescent2settingsMaintexturesetWater)
	ON_COMMAND(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_ICE, OnViewSpecialdescent2settingsMaintexturesetIce)
	ON_COMMAND(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_FIRE, OnViewSpecialdescent2settingsMaintexturesetFire)
	ON_COMMAND(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_ALIEN2, OnViewSpecialdescent2settingsMaintexturesetAlien2)
	ON_COMMAND(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_ALIEN1, OnViewSpecialdescent2settingsMaintexturesetAlien1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_GROUPA, OnUpdateViewSpecialdescent2settingsMaintexturesetGroupa)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_FIRE, OnUpdateViewSpecialdescent2settingsMaintexturesetFire)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_ALIEN2, OnUpdateViewSpecialdescent2settingsMaintexturesetAlien2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_ALIEN1, OnUpdateViewSpecialdescent2settingsMaintexturesetAlien1)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_WATER, OnUpdateViewSpecialdescent2settingsMaintexturesetWater)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SPECIALDESCENT2SETTINGS_MAINTEXTURESET_ICE, OnUpdateViewSpecialdescent2settingsMaintexturesetIce)
	ON_COMMAND(ID_MODEL_MODELCOMPARISONTABLE, OnModelModelcomparisontable)
	ON_COMMAND(ID_VIEW_SHOWGUNS, OnViewShowguns)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWGUNS, OnUpdateViewShowguns)
	ON_COMMAND(ID_VIEW_SHOWSEGMENTS, OnViewShowsegments)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWSEGMENTS, OnUpdateViewShowsegments)
	ON_COMMAND(ID_VIEW_SHOWDOCKPOINTS, OnViewShowdockpoints)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWDOCKPOINTS, OnUpdateViewShowdockpoints)
	ON_COMMAND(ID_VIEW_SHOWTHRUSTERGLOWS, OnViewShowthrusterglows)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWTHRUSTERGLOWS, OnUpdateViewShowthrusterglows)
	ON_COMMAND(ID_FILE_RELOAD, OnFileReload)
	ON_UPDATE_COMMAND_UI(ID_MODEL_MODELCOMPARISONTABLE, OnUpdateModelModelcomparisontable)
	ON_UPDATE_COMMAND_UI(ID_FILE_RELOAD, OnUpdateFileReload)
	ON_BN_CLICKED(IDC_EDITOR, OnEditor)
	ON_COMMAND(ID_ELEMENTEDITOR_CREATE, OnElementeditorCreate)
	ON_COMMAND(ID_ELEMENTEDITOR_DELETE, OnElementeditorDelete)
	ON_COMMAND(ID_FILE_RETURNTOVIEWERMODE, OnBackToViewer)
	ON_COMMAND(ID_NAVIGATION_PANELEFT, OnNavigationPaneleft)
	ON_COMMAND(ID_NAVIGATION_PANERIGHT, OnNavigationPaneright)
	ON_COMMAND(ID_NAVIGATION_PANEDOWN, OnNavigationPanedown)
	ON_COMMAND(ID_NAVIGATION_PANEUP, OnNavigationPaneup)
	ON_WM_SIZE()
	ON_COMMAND(ID_ELEMENTAPPLYER_APPLY, OnElementapplyerApply)
	ON_COMMAND(ID_ELEMENTAPPLYER_DISCARD, OnElementapplyerDiscard)
	ON_UPDATE_COMMAND_UI(ID_EDITOR_OPENINPOFEDITOR, OnUpdateEditor)
	ON_COMMAND(ID_EDITOR_CLOSEANDGOBACKTOVIEWER, OnViewer)
	ON_UPDATE_COMMAND_UI(ID_EDITOR_CLOSEANDGOBACKTOVIEWER, OnUpdateViewer)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateEditorSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPTIONSCONFIGURATION, OnUpdateFileOptionsconfiguration)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDITOR_POFEDITORTOOGLE, OnEditorPofeditortoogle)
	ON_UPDATE_COMMAND_UI(ID_EDITOR_POFEDITORTOOGLE, OnUpdateEditorPofeditortoogle)
	ON_COMMAND(ID_ELEMENTEDITOR_COPY, OnElementeditorCopy)
	ON_COMMAND(ID_ELEMENTEDITOR2_CREATE, OnElementeditor2Create)
	ON_COMMAND(ID_ELEMENTEDITOR2_DELETE, OnElementeditor2Delete)
	ON_COMMAND(ID_ELEMENTEDITOR2_COPY, OnElementeditor2Copy)
	ON_COMMAND(ID_MODEL_COPY, OnModelCopy)
	ON_UPDATE_COMMAND_UI(ID_MODEL_COPY, OnUpdateModelCopy)
	ON_COMMAND(ID_EXPLORERVIEW_COMPARISONTABLE, OnExplorerviewComparisontable)
	ON_COMMAND(ID_EXPLORERBAR_NEXT, OnExplorerbarNext)
	ON_COMMAND(ID_EXPLORERBAR_FILTER, OnExplorerbarFilter)
	ON_COMMAND(ID_EXPLORERBAR_PREV, OnExplorerbarPrev)
	ON_COMMAND(ID_EXPLORERBAR_TREEVIEW, OnExplorerbarTreeview)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_FS1, OnUpdateFileSaveAsFs1)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS_FS2, OnUpdateFileSaveAsFs2)
	ON_COMMAND(ID_FILE_SAVE_AS_FS1, OnFileSaveAsFs1)
	ON_COMMAND(ID_FILE_SAVE_AS_FS2, OnFileSaveAsFs2)
	ON_COMMAND(ID_VIEW_ROTATE, OnViewRotate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE, OnUpdateViewRotate)
	ON_COMMAND(ID_HELP_USING, OnHelpUsing)
	ON_COMMAND(ID_EDITOR_IMPORTPOFFROM3DFILE, OnEditorImportpoffrom3dfile)
	ON_UPDATE_COMMAND_UI(ID_EDITOR_IMPORTPOFFROM3DFILE, OnUpdateEditorImportpoffrom3dfile)
	ON_UPDATE_COMMAND_UI(ID_MODEL_DOWN, OnUpdateNavigation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RIGHT, OnUpdateNavigation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_UP, OnUpdateNavigation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_ZOOMIN, OnUpdateNavigation)
	ON_UPDATE_COMMAND_UI(ID_MODEL_ZOOMOUT, OnUpdateNavigation)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_RESETZOOMLEVEL, OnUpdateNavigation)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_PANELEFT, OnUpdateNavigation)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_PANERIGHT, OnUpdateNavigation)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_PANEUP, OnUpdateNavigation)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATION_PANEDOWN, OnUpdateNavigation)
	ON_COMMAND(ID_EDITOR_OPENINPOFEDITOR, OnEditor)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateEditorSave)
	ON_COMMAND(ID_HELP_USING2, OnHelpUsing)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_COMMAND(ID_VPFILEDIALOG_JUMPTODESCENT2DIRECTORY, OnVpfiledialogJumptodescent2directory)
	ON_UPDATE_COMMAND_UI(ID_VPFILEDIALOG_JUMPTODESCENT2DIRECTORY, OnUpdateVpfiledialogJumptodescent2directory)
	ON_COMMAND(ID_VPFILEDIALOG_JUMPTODESCENT3DIRECTORY, OnVpfiledialogJumptodescent3directory)
	ON_UPDATE_COMMAND_UI(ID_VPFILEDIALOG_JUMPTODESCENT3DIRECTORY, OnUpdateVpfiledialogJumptodescent3directory)
	ON_COMMAND(ID_VPFILEDIALOG_JUMPTOFREESPACE1DIRECTORY, OnVpfiledialogJumptofreespace1directory)
	ON_UPDATE_COMMAND_UI(ID_VPFILEDIALOG_JUMPTOFREESPACE1DIRECTORY, OnUpdateVpfiledialogJumptofreespace1directory)
	ON_COMMAND(ID_VPFILEDIALOG_JUMPTOFREESPACE2DIRECTORY, OnVpfiledialogJumptofreespace2directory)
	ON_UPDATE_COMMAND_UI(ID_VPFILEDIALOG_JUMPTOFREESPACE2DIRECTORY, OnUpdateVpfiledialogJumptofreespace2directory)
	ON_COMMAND(ID_MODEL_SHOWMODELDETAILS_TEXTURES,TXShowTextures)
	ON_COMMAND(ID_MODEL_SHOWMODELDETAILS_BLOCKLIST,TXShowBlockList)
	ON_COMMAND(ID_MODEL_SHOWMODELDETAILS_LOGFILE,TXShowLogFile)
	ON_COMMAND(ID_MODEL_SHOWMODELDETAILS_NODETAILS,TXPaneHide)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWMODELDETAILS_TEXTURES,OnUpdateTXShowTextures)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWMODELDETAILS_BLOCKLIST,OnUpdateTXShowBlockList)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWMODELDETAILS_LOGFILE,OnUpdateTXShowLogFile)
	ON_UPDATE_COMMAND_UI(ID_MODEL_SHOWMODELDETAILS_NODETAILS,OnUpdateTXHide)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_EDITORFS_MODEL, ID_EDITORFS_FUEL, OnEditorFS_ToolbarClick)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EDITORFS_MODEL, ID_EDITORFS_FUEL, OnEditorFS_ToolbarClick_Update)
	ON_COMMAND_RANGE(ID_NAVIGATION_VIEWFROMFRONT, ID_NAVIGATION_VIEWFROMRIGHT,OnPredefinedView)
	ON_UPDATE_COMMAND_UI_RANGE(ID_NAVIGATION_VIEWFROMFRONT, ID_NAVIGATION_VIEWFROMRIGHT,OnPredefinedView_Update)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	//ID_INDICATOR_PAGE,
	ID_INDICATOR_STATUS,
	//ID_INDICATOR_ROTATION,
	//ID_INDICATOR_ZOOM,
	ID_INDICATOR_VERSION
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_VPFileDialog=NULL;
	m_ExplorerVisible=TRUE;
	m_ExplorerFilterLowLevel=FALSE;
	current_game=GAME_NONE;
	m_ZoomLevel=1.0;
	m_XRotation=30.00;
	m_YRotation=30.00;
	m_ToolBarTextMode=TOOLBAR_TEXT_RIGHT;
	m_ToolBarSize=TOOLBAR_SIZE_SMALL;
	m_SwapMouseButtons=FALSE;
	m_UseControlBar=TRUE;
	m_EditorMode=FALSE;
	m_TXCurrent=-1;

	CString ininame=AfxGetApp()->m_pszHelpFilePath;
	ininame.TrimRight(".HLP");
	ininame.TrimRight(".hlp");
	m_TypeRootsFile=ininame+".dir";
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	EnableDocking(CBRS_ALIGN_ANY);

	InitToolbars();

	return 0;
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
#ifdef _WITHTXVIEW
	m_wndSplitterTX.CreateStatic(this,2,1);
	m_wndSplitterTX.SetRowInfo(0,2000,0);
	m_wndSplitterTX.SetRowInfo(1,0,0);
	m_wndSplitterTX2.CreateStatic(&m_wndSplitterTX, 1, 2,WS_CHILD|WS_VISIBLE|WS_BORDER,m_wndSplitterTX.IdFromRowCol(1, 0));
	//m_wndSplitterTX2.CreateView(0,0,RUNTIME_CLASS(CTXList),CSize(400,100),pContext);
	m_wndSplitterTX2.CreateView(0,1,RUNTIME_CLASS(CTXView),CSize(400,100),pContext);
	m_wndSplitterTX3.CreateStatic(&m_wndSplitterTX2,1,2,WS_CHILD|WS_VISIBLE|WS_BORDER,m_wndSplitterTX2.IdFromRowCol(0, 0));
	m_wndSplitterTX3.CreateView(0,0,RUNTIME_CLASS(CTXViewModeToolbar),CSize(22,100),pContext);
	m_wndSplitterTX3.CreateView(0,1,RUNTIME_CLASS(CTXList),CSize(400,100),pContext);
	m_wndSplitter.CreateStatic(&m_wndSplitterTX, 1, 3,WS_CHILD|WS_VISIBLE|WS_BORDER,m_wndSplitterTX.IdFromRowCol(0, 0));
	m_wndSplitterTX2.SetColumnInfo(0,600,0);
	m_wndSplitterTX2.RecalcLayout();
	//OnSize2();

	CWnd *pWnd=m_wndSplitterTX3.GetPane(0, 0);
	CTXViewModeToolbar* pView = DYNAMIC_DOWNCAST(CTXViewModeToolbar, pWnd);
	pView->Init();	
#else
	m_wndSplitter.CreateStatic(this, 1, 3);
#endif

	//m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CExplorer), CSize(140, 100), pContext);
#ifdef _WITHHTVIEW
	m_paneMainView=m_wndSplitter.AddView(0, 1, RUNTIME_CLASS(CMODVIEW32View), pContext);
	m_paneMainHTML=m_wndSplitter.AddView(0, 1, RUNTIME_CLASS(CHTMLTaskView), pContext);
#else
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMODVIEW32View), CSize(100, 100), pContext);
#endif
	m_paneWelcome=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CWelcomePane), pContext);
	m_paneD2=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CGamePaneD2), pContext);
	m_paneD3=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CGamePaneD3), pContext);
	m_paneFS=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CGamePaneFS), pContext);
	m_paneRF=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CGamePaneRF), pContext);

#ifdef _WITHEDITOR
	m_paneEdFS_MODEL=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CEditorFS_MODEL), pContext);
	m_paneEdFS_SPCL=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CEditorFS_SPCL), pContext);
	m_paneEdFS_TGUN=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CEditorFS_TGUN), pContext);
	m_paneEdFS_GPNT=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CEditorFS_GPNT), pContext);
	m_paneEdFS_SHLD=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CEditorFS_SHLD), pContext);
	m_paneEdFS_SOBJ=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CEditorFS_SOBJ), pContext);
	m_paneEdFS_FUEL=m_wndSplitter.AddView(0, 2, RUNTIME_CLASS(CEditorFS_FUEL), pContext);
#endif

	m_barSplitter.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD|WS_VISIBLE|WS_BORDER,m_wndSplitter.IdFromRowCol(0, 0));
	m_barSplitter.CreateView(0, 0, RUNTIME_CLASS(CExplorerBar), CSize(25, 25), pContext);
	m_barSplitter.CreateView(1, 0, RUNTIME_CLASS(CExplorer), CSize(0, 0), pContext);
	m_wndSplitter.SetColumnInfo(1,500,0);
	m_wndSplitter.SetColumnInfo(2,140,0);
	m_wndSplitter.ShowView(m_paneWelcome);
#ifdef _WITHHTVIEW
	m_wndSplitter.ShowView(m_paneMainView);
#endif
	st=TRUE;
	ExplorerHide();
	m_wndSplitter.RecalcLayout();

	SetTimer(0,50,NULL);
	
	return TRUE;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style &= ~FWS_PREFIXTITLE;
	cs.cx=min(800,::GetSystemMetrics(SM_CXSCREEN));
	cs.cy=min(600,::GetSystemMetrics(SM_CYSCREEN));
	cs.x = (::GetSystemMetrics(SM_CXSCREEN) - cs.cx) / 2;
	cs.y = (::GetSystemMetrics(SM_CYSCREEN) - cs.cy) / 2;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::GameBarD2() 
{
	if(current_game!=GAME_D2)
	{
		GameBarNone();
		m_wndSplitter.ShowView(m_paneD2);
		ShowControlBar(&m_wndToolbarNoModel,FALSE,FALSE);
		ShowControlBar(&m_wndToolBarD2,TRUE,FALSE);
	}
	current_game=GAME_D2;
	((CGamePaneD2 *)GetGamePane())->Init();
}

void CMainFrame::GameBarD3()
{
	if(current_game!=GAME_D3)
	{
		GameBarNone();
		m_wndSplitter.ShowView(m_paneD3);
		ShowControlBar(&m_wndToolbarNoModel,FALSE,FALSE);
		ShowControlBar(&m_wndToolBarD3,TRUE,FALSE);
	}
	current_game=GAME_D3;
	((CGamePaneD3 *)GetGamePane())->Init();
}

void CMainFrame::GameBarFS() 
{
	if(current_game!=GAME_FS || m_EditorMode)
	{
		GameBarNone();
		m_wndSplitter.ShowView(m_paneFS);
		ShowControlBar(&m_wndToolbarNoModel,FALSE,FALSE);
		ShowControlBar(&m_wndToolBarFS,TRUE,FALSE);
	}
	current_game=GAME_FS;
	((CGamePaneFS *)GetGamePane())->Init();
}

void CMainFrame::GameBarRF()
{
	if(current_game!=GAME_RF)
	{
		GameBarNone();
		m_wndSplitter.ShowView(m_paneRF);
		//ShowControlBar(&m_wndToolbarNoModel,FALSE,FALSE);
		//ShowControlBar(&m_wndToolBarRF,TRUE,FALSE);
	}
	current_game=GAME_RF;
	((CGamePaneRF *)GetGamePane())->Init();
}

void CMainFrame::GameBarNone()
{
	current_game=GAME_NONE;

	ShowControlBar(&m_wndToolbarNoModel,TRUE,FALSE);
	ShowControlBar(&m_wndToolBarD2,FALSE,FALSE);
	ShowControlBar(&m_wndToolBarD3,FALSE,FALSE);
	ShowControlBar(&m_wndToolBarFS,FALSE,FALSE);

	m_wndSplitter.ShowView(m_paneWelcome);
	((CWelcomePane *)GetGamePane())->ShowNextTip();
}

void CMainFrame::ExplorerClear()
{
	ExplorerPane()->DoEventhandling=FALSE;
	CTreeCtrl *p=ExplorerCtrl();
	p->DeleteAllItems();
	for(int i=0;i<MAX_TYPEROOTS;i++)
	{
		ExplorerPane()->typeroot[i]=NULL;
		ExplorerPane()->typeroot_str[i]="";
		ExplorerPane()->typeroot_num[i]=0;
	}
	ExplorerPane()->DoEventhandling=TRUE;
}

void CMainFrame::ExplorerAddRoot(char *text)
{
	CTreeCtrl *p=ExplorerCtrl();
	m_ExplorerLastRoot=p->InsertItem(text);
	ExplorerPane()->m_NumModelsListed=0;
	ExplorerPane()->m_RootName=text;

	int curgame=GetDocument()->m_Game;
	if(curgame==GAME_D3 || curgame==GAME_FS)
	{
		CExplorer *e=ExplorerPane();
		CTreeCtrl *p=ExplorerCtrl();

		CString gamecaption;
		if(GetDocument()->m_Game==GAME_D3)
			gamecaption="Descent3";
		else
			gamecaption="FreeSpace";

		for(int typenr=0;typenr<MAX_TYPEROOTS;typenr++)
		{
			char capname[8192];
			char typestr[8192];
			sprintf(typestr,"%i",typenr);
			GetPrivateProfileString(gamecaption,typestr,"$",(char *)&capname,8191,m_TypeRootsFile);
			if(strcmp(capname,"$")!=0)
			{
				e->typeroot[typenr]=p->InsertItem(capname,m_ExplorerLastRoot);
				p->EnsureVisible(e->typeroot[typenr]);
				e->typeroot_str[typenr]=capname;
				e->typeroot_num[typenr]=0;
			}
		}

	}
}

void CMainFrame::ExplorerAddChild(char *filename,int typenr/*=-1*/)
{
	CExplorer *e=ExplorerPane();
	CTreeCtrl *p=ExplorerCtrl();
	ASSERT(typenr<MAX_TYPEROOTS);
	if(typenr>=MAX_TYPEROOTS) //Overflow protection
		typenr=-1;
	if(!m_ExplorerIndex) //User wants no grouping
		typenr=-1;
	if(typenr==-1)
		p->EnsureVisible(p->InsertItem(filename,m_ExplorerLastRoot));
	else
	{
		if(e->typeroot[typenr]==NULL)
		{
			//Get name
			if(GetDocument()->m_Game==GAME_D2)
			{
				CString capname=GetDocument()->D2_GenerateModelTypeName(typenr);
				e->typeroot[typenr]=p->InsertItem(capname,m_ExplorerLastRoot);
				p->EnsureVisible(e->typeroot[typenr]);
				e->typeroot_str[typenr]=capname;
				e->typeroot_num[typenr]=0;
			}
			else
				ASSERT(FALSE);
			/*{
				char capname[8192];
				char typestr[8192];
				sprintf(typestr,"%i",typenr);
				
				CString gamecaption;
				if(GetDocument()->m_Game==GAME_D3)
					gamecaption="Descent3";
				else
					gamecaption="FreeSpace";
				GetPrivateProfileString(gamecaption,typestr,"<unknown type>",(char *)&capname,8191,m_TypeRootsFile);
				e->typeroot[typenr]=p->InsertItem(capname,m_ExplorerLastRoot);
				p->EnsureVisible(e->typeroot[typenr]);
				e->typeroot_str[typenr]=capname;
				e->typeroot_num[typenr]=0;
			}*/
		}
		
		//Update num items
		e->typeroot_num[typenr]++;
		CString captext;
		captext.Format("%s (%i)",e->typeroot_str[typenr],e->typeroot_num[typenr]);
		p->SetItemText(e->typeroot[typenr],captext);

		//Add item
		p->InsertItem(filename,e->typeroot[typenr]);
	}

	//Update total num items
	e->m_NumModelsListed++;
	CString text;
	text.Format("%s (%i)",e->m_RootName,e->m_NumModelsListed);
	p->SetItemText(p->GetRootItem(),text);
}

void CMainFrame::ExplorerGoTop()
{
	CExplorer *e=ExplorerPane();
	CTreeCtrl *p=ExplorerCtrl();
	p->EnsureVisible(p->GetRootItem());

	//Tidy up
	for(int i=0;i<MAX_TYPEROOTS;i++)
	{
		if(e->typeroot[i]!=NULL)
		{
			if(e->typeroot_num[i]==0)
				p->DeleteItem(e->typeroot[i]);
		}
	}
}

CExplorer *CMainFrame::ExplorerPane()
{
	if(!m_ExplorerVisible)
		ExplorerShow();
	ASSERT(m_ExplorerVisible);
	//CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CWnd *pWnd=m_barSplitter.GetPane(1,0);
	CExplorer* pView = DYNAMIC_DOWNCAST(CExplorer, pWnd);
	return pView;
}

CTreeCtrl *CMainFrame::ExplorerCtrl()
{
	CExplorer *pView=ExplorerPane();
	return &pView->GetTreeCtrl();
}

int CMainFrame::ExplorerOpenSubFile(char *fname)
{
	char *pathname=new char[8192];
	char *filename=pathname;
	char *p=filename;
	strcpy(pathname,fname);
	
	//Remove file path, if specified
	while(*p!=0)
	{
		if(*p=='\\')
		{
			filename=p;
			filename++;
		}
		p++;
	}

	//Remove file extension, if specified
	char *X=strrchr(filename,'.');
	if(X!=NULL)
		*X=0;

	CExplorer *e=ExplorerPane();
	CTreeCtrl *t=ExplorerCtrl();
	e->DoEventhandling=TRUE;

	HTREEITEM k=t->GetRootItem();
	while(k=e->GetNextItem(k))
	{
		if(_strcmpi(filename,t->GetItemText(k))==0)
		{
			t->EnsureVisible(k);
			t->SelectItem(k);
		}
	}
	delete[] pathname;
	return 0;
}




CMODVIEW32View *CMainFrame::GetCurrentView()
{
#ifdef _WITHHTVIEW
	//m_wndSplitter.ShowView(m_paneMainView);
	ASSERT(IsWindow(m_wndSplitter));
	return (CMODVIEW32View *)m_wndSplitter.GetView(m_paneMainView);
#else
	CWnd *pWnd;
	if(m_ExplorerVisible)
		pWnd=m_wndSplitter.GetPane(0, 1);
	else
		pWnd=m_wndSplitter.GetPane(0, 0);

	CMODVIEW32View* pView=DYNAMIC_DOWNCAST(CMODVIEW32View, pWnd);
	return pView;
#endif
}

CMODVIEW32Doc * CMainFrame::GetDocument()
{
	//CWnd* pWnd=GetCurrentView()
	//CMODVIEW32View* pView = DYNAMIC_DOWNCAST(CMODVIEW32View, pWnd);
	return GetCurrentView()->GetDocument();
	//CMODVIEW32Doc *doc=(CMODVIEW32Doc *)GetActiveDocument();
	//return doc;
}

CView * CMainFrame::GetGamePane()
{
	CWnd *pWnd;
	if(m_ExplorerVisible)
		pWnd=m_wndSplitter.GetPane(0, 2);
	else
		pWnd=m_wndSplitter.GetPane(0, 1);

	CView* pView = DYNAMIC_DOWNCAST(CView, pWnd);
	return pView;
}

void CMainFrame::OnViewDetaillevelDebris() 
{
	GetCurrentView()->FS_SetDetailLevel(9);
}

void CMainFrame::OnViewDetaillevelLevel1() 
{
	GetCurrentView()->FS_SetDetailLevel(0);
}

void CMainFrame::OnViewDetaillevelLevel2() 
{
	GetCurrentView()->FS_SetDetailLevel(1);
}

void CMainFrame::OnViewDetaillevelLevel3() 
{
	GetCurrentView()->FS_SetDetailLevel(2);
}

void CMainFrame::OnViewDetaillevelLevel4() 
{
	GetCurrentView()->FS_SetDetailLevel(3);
}

void CMainFrame::OnViewDetaillevelLevel5() 
{
	GetCurrentView()->FS_SetDetailLevel(4);
}

void CMainFrame::OnViewDetaillevelLevel6() 
{
	GetCurrentView()->FS_SetDetailLevel(5);
}


#define ROTSPEED 5
#define ZOOMSPEED 1.2f

void CMainFrame::OnModelLeft() 
{
	GetCurrentView()->OnKeyDown2(VK_LEFT,1,0);

	GetCurrentView()->m_yRotation-=ROTSPEED;
	if(GetCurrentView()->m_yRotation < 0) GetCurrentView()->m_yRotation = GetCurrentView()->m_yRotation + 360;
	if(GetCurrentView()->m_yRotation >= 360) GetCurrentView()->m_yRotation = GetCurrentView()->m_yRotation - 360;
	GetCurrentView()->m_SkipBuild=TRUE;
	GetCurrentView()->InvalidateRect(NULL);
	m_YRotation = GetCurrentView()->m_yRotation;
}


void CMainFrame::OnModelRight() 
{
	GetCurrentView()->OnKeyDown2(VK_RIGHT,1,0);

	GetCurrentView()->m_yRotation+=ROTSPEED;
	if(GetCurrentView()->m_yRotation < 0) GetCurrentView()->m_yRotation = GetCurrentView()->m_yRotation + 360;
	if(GetCurrentView()->m_yRotation >= 360) GetCurrentView()->m_yRotation = GetCurrentView()->m_yRotation - 360;
	GetCurrentView()->m_SkipBuild=TRUE;
	GetCurrentView()->InvalidateRect(NULL);
	m_YRotation = GetCurrentView()->m_yRotation;
}

void CMainFrame::OnModelUp() 
{
	GetCurrentView()->OnKeyDown2(VK_UP,1,0);

	GetCurrentView()->m_xRotation-=ROTSPEED;
	if(GetCurrentView()->m_xRotation < 0) GetCurrentView()->m_xRotation = GetCurrentView()->m_xRotation + 360;
	if(GetCurrentView()->m_xRotation >= 360) GetCurrentView()->m_xRotation = GetCurrentView()->m_xRotation - 360;
	GetCurrentView()->m_SkipBuild=TRUE;
	GetCurrentView()->InvalidateRect(NULL);
	m_XRotation = GetCurrentView()->m_xRotation;
}

void CMainFrame::OnModelDown() 
{
	GetCurrentView()->OnKeyDown2(VK_DOWN,1,0);

	GetCurrentView()->m_xRotation+=ROTSPEED;
	if(GetCurrentView()->m_xRotation < 0) GetCurrentView()->m_xRotation = GetCurrentView()->m_xRotation + 360;
	if(GetCurrentView()->m_xRotation >= 360) GetCurrentView()->m_xRotation = GetCurrentView()->m_xRotation - 360;
	GetCurrentView()->m_SkipBuild=TRUE;
	GetCurrentView()->InvalidateRect(NULL);
	m_XRotation = GetCurrentView()->m_xRotation;
}

void CMainFrame::OnModelZoomin() 
{
	GetCurrentView()->OnKeyDown2('A',1,0);

	GetCurrentView()->m_xScaling*=ZOOMSPEED;
	GetCurrentView()->m_yScaling*=ZOOMSPEED;
	GetCurrentView()->m_zScaling*=ZOOMSPEED;
	GetCurrentView()->m_SkipBuild=TRUE;
	GetCurrentView()->InvalidateRect(NULL);
	m_ZoomLevel = GetCurrentView()->m_xScaling;
}

void CMainFrame::OnModelZoomout() 
{
	GetCurrentView()->OnKeyDown2('Y',1,0);

	if(GetCurrentView()->m_xScaling>0.01f)
	{
		GetCurrentView()->m_xScaling/=ZOOMSPEED;
		GetCurrentView()->m_yScaling/=ZOOMSPEED;
		GetCurrentView()->m_zScaling/=ZOOMSPEED;
		GetCurrentView()->m_SkipBuild=TRUE;
		GetCurrentView()->InvalidateRect(NULL);
	}
	m_ZoomLevel = GetCurrentView()->m_xScaling;
}

#define PANESPEED 30.0f
#define PANEDIV 3.0f

float CMainFrame::CalcPaneStep()
{
	switch(current_game)
	{
	case GAME_D2:	return GetDocument()->m_D2_Model.MaxXYZ/PANEDIV;
	case GAME_D3:	return GetDocument()->m_D3_Display.MaxXYZ/PANEDIV;
	case GAME_FS:	return GetDocument()->m_FS_Model.MaxXYZ/PANEDIV;
	default:		return PANESPEED;
	}
}

float CMainFrame::CalcPaneDrag()
{
	switch(current_game)
	{
	case GAME_D2:	return GetDocument()->m_D2_Model.MaxXYZ/100;
	case GAME_D3:	return GetDocument()->m_D3_Display.MaxXYZ/50;
	case GAME_FS:	return GetDocument()->m_FS_Model.MaxXYZ/100;
	default:		return (float)1.0;
	}
}
	


void CMainFrame::OnNavigationPaneleft() 
{
	GetCurrentView()->m_xTranslation+=CalcPaneStep();
	GetCurrentView()->m_SkipBuild=TRUE;
	GetCurrentView()->InvalidateRect(NULL);	//GetCurrentView()->m_xScaling*=ZOOMSPEED;	
}

void CMainFrame::OnNavigationPaneright() 
{
	GetCurrentView()->m_xTranslation-=CalcPaneStep();
	GetCurrentView()->m_SkipBuild=TRUE;
	GetCurrentView()->InvalidateRect(NULL);	//GetCurrentView()->m_xScaling*=ZOOMSPEED;	
}


void CMainFrame::OnNavigationPanedown() 
{
	GetCurrentView()->m_yTranslation+=CalcPaneStep();
	GetCurrentView()->m_SkipBuild=TRUE;
	GetCurrentView()->InvalidateRect(NULL);	//GetCurrentView()->m_xScaling*=ZOOMSPEED;	
}

void CMainFrame::OnNavigationPaneup() 
{
	GetCurrentView()->m_yTranslation-=CalcPaneStep();
	GetCurrentView()->m_SkipBuild=TRUE;
	GetCurrentView()->InvalidateRect(NULL);	//GetCurrentView()->m_xScaling*=ZOOMSPEED;	
}


////////////////////////////

void CMainFrame::OnUpdateViewDetaillevelLevel1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS && GetDocument()->m_FS_PMinfo.n_detail>=1);
	pCmdUI->SetCheck(GetCurrentView()->m_Detaillevel==0);
}

void CMainFrame::OnUpdateViewDetaillevelLevel2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS && GetDocument()->m_FS_PMinfo.n_detail>=2);
	pCmdUI->SetCheck(GetCurrentView()->m_Detaillevel==1);
}

void CMainFrame::OnUpdateViewDetaillevelLevel3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS && GetDocument()->m_FS_PMinfo.n_detail>=3);
	pCmdUI->SetCheck(GetCurrentView()->m_Detaillevel==2);
}

void CMainFrame::OnUpdateViewDetaillevelLevel4(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS && GetDocument()->m_FS_PMinfo.n_detail>=4);
	pCmdUI->SetCheck(GetCurrentView()->m_Detaillevel==3);
}

void CMainFrame::OnUpdateViewDetaillevelLevel5(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS && GetDocument()->m_FS_PMinfo.n_detail>=5);
	pCmdUI->SetCheck(GetCurrentView()->m_Detaillevel==4);
}

void CMainFrame::OnUpdateViewDetaillevelLevel6(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS && GetDocument()->m_FS_PMinfo.n_detail>=6);
	pCmdUI->SetCheck(GetCurrentView()->m_Detaillevel==5);
}

void CMainFrame::OnUpdateViewDetaillevelDebris(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS && GetDocument()->m_FS_ModelHasDebris);
	pCmdUI->SetCheck(GetCurrentView()->m_Detaillevel==9);
}

void CMainFrame::OnUpdateViewRendermodeWireframe(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCurrentView()->m_RenderMode==RENDER_WIREFRAME);
}

void CMainFrame::OnUpdateViewRendermodeSurface(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCurrentView()->m_RenderMode==RENDER_SURFACE);
}

void CMainFrame::OnUpdateViewRendermodeTextured(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCurrentView()->m_RenderMode==RENDER_TEXTURED);
}

void CMainFrame::OnUpdateViewRendersettingsSmooth(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCurrentView()->m_RenderSmooth);
}

void CMainFrame::OnUpdateViewRendersettingsCullface(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCurrentView()->m_RenderCull);
}

void CMainFrame::OnUpdateViewRendersettingsZbuffered(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCurrentView()->m_RenderZbuffer);
}
void CMainFrame::OnViewRendermodeWireframe() 
{
	GetCurrentView()->m_RenderMode=RENDER_WIREFRAME;
	UpdateMainDisplay();
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnViewRendermodeSurface() 
{
	GetCurrentView()->m_RenderMode=RENDER_SURFACE;
	UpdateMainDisplay();
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnViewRendermodeTextured() 
{
	GetCurrentView()->m_RenderMode=RENDER_TEXTURED;
	if(GetDocument()->m_Game==GAME_FS)
		GetDocument()->FS_LoadPCXData(GetCurrentView()->m_Detaillevel,GetCurrentView()->m_ShowThruster,GetCurrentView()->m_FastTextureLoad);
	UpdateMainDisplay();
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnViewRendersettingsSmooth() 
{
	GetCurrentView()->m_RenderSmooth=!GetCurrentView()->m_RenderSmooth;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnViewRendersettingsCullface() 
{
	GetCurrentView()->m_RenderCull=!GetCurrentView()->m_RenderCull;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnViewRendersettingsZbuffered() 
{
	GetCurrentView()->m_RenderZbuffer=!GetCurrentView()->m_RenderZbuffer;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnViewShowshields() 
{
	GetCurrentView()->m_ShowShield=!GetCurrentView()->m_ShowShield;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnUpdateViewShowshields(CCmdUI* pCmdUI) 
{
	if(!m_EditorMode || m_EditorFS_CurrentView!=ID_EDITORFS_SHLD)
	{
		pCmdUI->Enable(current_game==GAME_FS);
		pCmdUI->SetCheck(GetCurrentView()->m_ShowShield);
		if((current_game==GAME_FS) && (m_UseControlBar) && (!m_EditorMode))
			((CGamePaneFS *)GetGamePane())->UpdateIt(GetCurrentView()->m_ShowShield,GetCurrentView()->m_ShowThruster);
	}
	else
		pCmdUI->Enable(FALSE); //User may not disable shields while in Edit Shields mode
}

void CMainFrame::OnViewShowdockpoints() 
{
	GetCurrentView()->m_FS_ShowDockingPoints=!GetCurrentView()->m_FS_ShowDockingPoints;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnUpdateViewShowdockpoints(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS);
	pCmdUI->SetCheck(GetCurrentView()->m_FS_ShowDockingPoints);
}



void CMainFrame::OnViewShowthrusterglows() 
{
	GetCurrentView()->m_FS_ShowThrusterGlows=!GetCurrentView()->m_FS_ShowThrusterGlows;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnUpdateViewShowthrusterglows(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS && GetDocument()->m_FS_Model.num_thrusters>0);
	pCmdUI->SetCheck(GetCurrentView()->m_FS_ShowThrusterGlows);
}

void CMainFrame::OnUpdateViewShowthruster(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS);
	pCmdUI->SetCheck(GetCurrentView()->m_ShowThruster);
	if(current_game==GAME_FS)
		((CGamePaneFS *)GetGamePane())->UpdateIt(GetCurrentView()->m_ShowShield,GetCurrentView()->m_ShowThruster);
}

void CMainFrame::OnViewShowthruster() 
{
	GetCurrentView()->m_ShowThruster=!GetCurrentView()->m_ShowThruster;
	GetDocument()->FS_LoadPCXData(GetCurrentView()->m_Detaillevel,GetCurrentView()->m_ShowThruster,GetCurrentView()->m_FastTextureLoad);
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnViewFasttextureload() 
{
	GetCurrentView()->m_FastTextureLoad=!GetCurrentView()->m_FastTextureLoad;
	GetDocument()->FS_LoadPCXData(GetCurrentView()->m_Detaillevel,GetCurrentView()->m_ShowThruster,GetCurrentView()->m_FastTextureLoad);
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnUpdateViewFasttextureload(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS);
	pCmdUI->SetCheck(GetCurrentView()->m_FastTextureLoad);
}

void CMainFrame::SetStatusBarText(char *txt)
{
	m_wndStatusBar.SetPaneText(0,txt);
	m_wndStatusBar.RedrawWindow();
}

void CMainFrame::OnFileOptionsconfiguration() 
{
	COptionsDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_GameConfigChanged)
		{
			//Restart the tool
			PostQuitMessage(-1);
			CString exename=AfxGetApp()->m_pszHelpFilePath;
			exename.TrimRight(".HLP");
			exename.TrimRight(".hlp");
			exename=exename+".exe";
			ShellExecute(NULL,"open",exename,NULL,NULL,SW_SHOWNORMAL);
		} else {
			//Reload options
			LoadOptions();
			GetCurrentView()->InvalidateRect(NULL);
		}

	}
}


void CMainFrame::OnFileOpenfile() 
{
	/*COpenDlg dlg;
	dlg.DoModal();*/
	ExplorerOpenSubFile("c:\\cxc\\ast01.pof");
}



void CMainFrame::OnFileOpen() 
{
	//OnFileOpennewversion();
	
	CMODVIEW32App*	pApp = (CMODVIEW32App*)AfxGetApp();

	m_VPFileDialog=new CVPFileDialog(TRUE,"",NULL,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		"All supported model and archive files|*.oof;*.hog;*.pof;*.vp;*.ham;*.hxm|Descent 2 model file (*.pol)|*.pol|Descent 2 archive (*.ham, *.hxm)|*.ham;*.hxm|Descent 3 model files (*.oof)|*.oof|Descent 3 archive (*.hog)|*.hog|FreeSpace 1/2 model files (*.pof)|*.pof|FreeSpace 1/2 archive (*.vp)|*.vp|All Files (*.*)|*.*||",(CWnd*)this);	
	//pFileDialog->m_ofn.lpstrInitialDir = pApp->m_sD3Path;

	if(m_VPFileDialog->DoModal()==IDOK)
		pApp->OpenDocumentFile(m_VPFileDialog->GetPathName());
		
	delete(m_VPFileDialog);
	m_VPFileDialog=NULL;

	return;
}

void CMainFrame::OnViewDorotation() 
{
	GetCurrentView()->m_D3_Rotate=!GetCurrentView()->m_D3_Rotate;
	SetTimer(1,50,NULL);
}

void CMainFrame::OnUpdateViewDorotation(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE); //current_game==GAME_D3);
	pCmdUI->SetCheck(GetCurrentView()->m_D3_Rotate);
}

void CMainFrame::OnUpdateModelTexturelist(CCmdUI* pCmdUI) 
{
	CMODVIEW32Doc* pDoc=GetDocument();
	pCmdUI->Enable(pDoc->m_ModelLoaded);
#ifdef _WITHTXVIEW
	pCmdUI->SetCheck(TXGetCurrent()!=-1);
#endif
}

void CMainFrame::OnNavigationResetzoomlevel() 
{
	GetCurrentView()->ResetGeometry();
	m_ZoomLevel = 1.0;
	m_XRotation = 30.00;
	m_YRotation = 30.00;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnViewSelectbackgroundcolor() 
{
	int oldR=(int)(GetCurrentView()->m_ClearColorRed*256);
	int oldG=(int)(GetCurrentView()->m_ClearColorGreen*256);
	int oldB=(int)(GetCurrentView()->m_ClearColorBlue*256);
	COLORREF old=oldR+oldG*256+oldB*65536;

	CColorDialog dlg(old,CC_FULLOPEN);
	dlg.DoModal();
	
	COLORREF col=dlg.GetColor();
	int colR=col%256;
	int colG=(col/256)%256;
	int colB=(col/65536)%256;
	GetCurrentView()->m_ClearColorRed=(float)colR;
	GetCurrentView()->m_ClearColorRed/=256;
	GetCurrentView()->m_ClearColorGreen=(float)colG;
	GetCurrentView()->m_ClearColorGreen/=256;
	GetCurrentView()->m_ClearColorBlue=(float)colB;
	GetCurrentView()->m_ClearColorBlue/=256;
	//GetCurrentView()->InvalidateRect(NULL);
	//GetCurrentView()->InvalidateRect(NULL);
	//GetCurrentView()->InvalidateRect(NULL,TRUE);
	//GetCurrentView()->InvalidateRect(NULL);
	RedrawWindow();
	RedrawWindow(); //need to do it 2x, otherwise it will not take the change until next redraw
}


void CMainFrame::ExplorerShow()
{
	m_ExplorerVisible=TRUE;
//	m_wndSplitter.ShowColumn();
	m_wndSplitter.SetColumnInfo(0,140,0);
	m_wndSplitter.RecalcLayout();
	OnSize2();
}

void CMainFrame::ExplorerHide()
{
//	m_ExplorerVisible=FALSE;
	m_wndSplitter.SetColumnInfo(0,0,10000);
//	m_wndSplitter.HideColumn(0);
	m_wndSplitter.RecalcLayout();
	OnSize2();
}



void CMainFrame::OnTimer(UINT nIDEvent) 
{
	CMODVIEW32Doc* pDoc=GetDocument();
	CMODVIEW32View* pView=GetCurrentView();

	switch(nIDEvent)
	{
	case 0:
		if(current_game==GAME_D2 /* or game_d3*/)
		{
			int redraw=FALSE;
			for(int i=0;i<MAX_D2_SUBMODELS;i++)
			{
				redraw|=pView->D2_ParsePosAngle(&pView->m_D2_PosAngle_Current[i].p,&pView->m_D2_PosAngle_ShallBe[i].p,&pView->m_D2_PosAngle_StepBit[i].p);
				redraw|=pView->D2_ParsePosAngle(&pView->m_D2_PosAngle_Current[i].h,&pView->m_D2_PosAngle_ShallBe[i].h,&pView->m_D2_PosAngle_StepBit[i].h);
				redraw|=pView->D2_ParsePosAngle(&pView->m_D2_PosAngle_Current[i].b,&pView->m_D2_PosAngle_ShallBe[i].b,&pView->m_D2_PosAngle_StepBit[i].b);
			}
			if(redraw)
				pView->InvalidateRect(NULL);
		}
		break;

	case 1:
		if(current_game==GAME_D3)
		{
			OutputDebugString("D3\n");
			if((pView->m_D3_Rotate)&(pDoc->m_D3_Display.PartsRotate))
			{
				for(unsigned int i=0;i<pDoc->m_D3_Model.Scount;i++)
				{
					if(pDoc->m_D3_Display.Rotate[i])
						pDoc->m_D3_Rstep[i] = (float)((const int)(pDoc->m_D3_Rstep[i] + (18.0/pDoc->m_D3_Display.Rspeed[i]))%360);
				}
				pView->InvalidateRect(NULL);
			}
		}
		break;
	}	
	CFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnUpdateZoom(CCmdUI *pCmdUI) //Stinger
{
	CMODVIEW32View* pView=GetCurrentView();
    pCmdUI->Enable(); 
	CString strZoom;
    strZoom.Format(" ZoomLevel: %2.3f", m_ZoomLevel);
    pCmdUI->SetText(strZoom); 
}

void CMainFrame::OnUpdateRotation(CCmdUI *pCmdUI)//Stinger
{
	CMODVIEW32View* pView=GetCurrentView();
    pCmdUI->Enable(); 
	CString strRot;
    strRot.Format(" XRot: %2.2f  YRot: %2.2f", m_XRotation, m_YRotation); 
    pCmdUI->SetText(strRot); 
}

void CMainFrame::OnModelNext() 
{
	ExplorerPane()->SendMessage(WM_KEYDOWN,VK_ADD,1);
	ExplorerPane()->SendMessage(WM_KEYDOWN,VK_DOWN,1);
}

void CMainFrame::OnModelPrevious() 
{
	ExplorerPane()->SendMessage(WM_KEYDOWN,VK_UP,1);
}


//************** BCMENU stuff START ****************
HMENU CMainFrame::NewMenu(int menu/*=0*/)
{
	// Load the menu from the resources
	if(menu==0)
		m_menu.LoadMenu(IDR_MAINFRAME);  // ****replace IDR_MAINFRAME with your menu ID****
	else
		m_menu.LoadMenu(IDR_EDITOR);

  // Use ModifyODMenu to add a bitmap to a menu options.The first parameter
  // is the menu option text string.If it's NULL, keep the current text
  // string.The second option is the ID of the menu option to change.
  // The third option is the resource ID of the bitmap.This can also be a
  // toolbar ID in which case the class searches the toolbar for the
  // appropriate bitmap.Only Bitmap and Toolbar resources are supported.
  //m_menu.ModifyODMenu(NULL,ID,IDB_ZOOM);

  // Another method for adding bitmaps to menu options is through the
  // LoadToolbar member function.This method allows you to add all
  // the bitmaps in a toolbar object to menu options (if they exist).
  // The function parameter is an the toolbar id.
  // There is also a function called LoadToolbars that takes an
  // array of id's.
  m_menu.LoadToolbar(IDR_SMALL);

  return(m_menu.Detach());
}

//This handler ensure that the popup menu items are drawn correctly
void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
  BOOL setflag=FALSE;
  if(lpMeasureItemStruct->CtlType==ODT_MENU){
    if(IsMenu((HMENU)lpMeasureItemStruct->itemID)){
      CMenu* cmenu=CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);
      if(m_menu.IsMenu(cmenu)){
        m_menu.MeasureItem(lpMeasureItemStruct);
        setflag=TRUE;
      }
    }
  }
  if(!setflag)
	  CFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


//This handler ensures that keyboard shortcuts work
LRESULT CMainFrame::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
  LRESULT lresult;
  if(m_menu.IsMenu(pMenu))
    lresult=BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
  else
    lresult=CFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
  return(lresult);
}


//This handler updates the menus from time to time
void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
  CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
  if(!bSysMenu){
    if(m_menu.IsMenu(pPopupMenu))
      BCMenu::UpdateMenu(pPopupMenu);
  }
}
//************** BCMENU stuff END ****************

void CMainFrame::OnModelTexturelist() 
{
#ifdef _WITHTXVIEW
	int cur,min;
	m_wndSplitterTX.GetRowInfo(1,cur,min);
	if(cur<=0)
	
		TXPaneShow(); //Show
	else
		TXPaneHide(); //Hide
#else
	CDetails dlg;
	dlg.DoModal();
#endif
}



BOOL CMainFrame::OnQueryEndSession() 
{
	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;

	OutputDebugString("QES");
	
	return TRUE;
}

void CMainFrame::OnChildActivate() 
{
	CFrameWnd::OnChildActivate();
	OutputDebugString("OCA");
}

void CMainFrame::OnParentNotify(UINT message, LPARAM lParam) 
{
	CString x;
	x.Format("OnParentNotify: Message=%i\n");
//	OutputDebugString(x);
	CFrameWnd::OnParentNotify(message, lParam);
}

void CMainFrame::ResizeToolbar(CToolBar *tb,BOOL alwayssmall/*=FALSE*/)
{
	DWORD size1=22;
	DWORD size2=40;
	if(m_ToolBarSize==TOOLBAR_SIZE_SMALL || alwayssmall)
	{
		size1=16;
		size2=36;
	}
	
	if(m_ToolBarTextMode==TOOLBAR_TEXT_BOTTOM)
		tb->SetSizes(CSize(size2,size2), CSize(size1,size1));

	if(m_ToolBarTextMode==TOOLBAR_TEXT_RIGHT)
	{
		tb->GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
		tb->GetToolBarCtrl().AutoSize();
	
		// Resize the buttons so that the text will fit.
		CRect rc(0, 0, 0, 0);
		CSize sizeMax(0, 0);

		for (int nIndex = tb->GetToolBarCtrl().GetButtonCount() - 1; nIndex >= 0; nIndex--)
		{
			if(tb->GetButtonStyle(nIndex)!=TBBS_SEPARATOR)
			{
				tb->SetButtonStyle(nIndex,TBSTYLE_AUTOSIZE);
				tb->GetToolBarCtrl().GetItemRect(nIndex, rc);
	
				//rc.NormalizeRect();
				sizeMax.cx = __max(rc.Size().cx, sizeMax.cx);
				sizeMax.cy = __max(rc.Size().cy, sizeMax.cy);
			}
		}

		//To prevent the assertion in SetSizes
		CSize sizeBtn=CSize(size1,size1);
		if(sizeMax.cx<sizeBtn.cx+7)
			sizeMax.cx=sizeBtn.cx+7;
		if(sizeMax.cy<sizeBtn.cy+6)
			sizeMax.cy=sizeBtn.cy+6;

		tb->SetSizes(sizeMax,sizeBtn);
		tb->CalcDynamicLayout(50,LM_HORZ);
		tb->CalcFixedLayout(FALSE,TRUE);
	}
}

//UNUSED
void CMainFrame::DockControlBarLeftOf(CToolBar *Bar, CToolBar *LeftOf)
{
	ASSERT(FALSE);

	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

void CMainFrame::OnUpdateNavigation(CCmdUI* pCmdUI) 
{
	CMODVIEW32Doc* pDoc=GetDocument();
	pCmdUI->Enable(pDoc->m_ModelLoaded);	
}

void CMainFrame::OnUpdateModelPrevious(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(ExplorerCtrl()->GetCount()!=0);//( m_ExplorerVisible);
}

void CMainFrame::OnUpdateModelNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(ExplorerCtrl()->GetCount()!=0);
}

void CMainFrame::OnViewSpecialdescent3settingsNextposition() 
{
	CListBox *lbox=(CListBox *)((CGamePaneD3 *)GetGamePane())->GetDlgItem(IDC_D3_POSITIONS);
	int x=lbox->GetCurSel();
	if(x<lbox->GetCount()-1)
		x++;
	lbox->SetCurSel(x);
	((CGamePaneD3 *)GetGamePane())->OnPositionChange();
}

void CMainFrame::OnViewSpecialdescent3settingsPreviousposition() 
{
	CListBox *lbox=(CListBox *)((CGamePaneD3 *)GetGamePane())->GetDlgItem(IDC_D3_POSITIONS);
	int x=lbox->GetCurSel();
	if(x>0)
		x--;
	lbox->SetCurSel(x);
	((CGamePaneD3 *)GetGamePane())->OnPositionChange();
}

void CMainFrame::OnUpdateViewSpecialdescent3settingsNextposition(CCmdUI* pCmdUI) 
{
	if(current_game==GAME_D3)
	{
		CListBox *lbox=(CListBox *)((CGamePaneD3 *)GetGamePane())->GetDlgItem(IDC_D3_POSITIONS);
		int x=lbox->GetCurSel();
		pCmdUI->Enable(x<lbox->GetCount()-1);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateViewSpecialdescent3settingsPreviousposition(CCmdUI* pCmdUI) 
{
	if(current_game==GAME_D3)
	{
		CListBox *lbox=(CListBox *)((CGamePaneD3 *)GetGamePane())->GetDlgItem(IDC_D3_POSITIONS);
		int x=lbox->GetCurSel();
		pCmdUI->Enable(x>0);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::FS_ShowShield()
{
	OnViewShowshields();
}

void CMainFrame::FS_ShowThruster()
{
	OnViewShowthruster();
}

void CMainFrame::OnViewSpecialdescent2settingsMaintexturesetGroupa() 
{
	GetDocument()->m_D2_TextureSet=D2_TEXTURESET_GROUPA;
	GetDocument()->D2_LoadPIGData();
	GetCurrentView()->InvalidateRect(NULL);
	UpdateTextureBar();
}

void CMainFrame::OnViewSpecialdescent2settingsMaintexturesetWater() 
{
	GetDocument()->m_D2_TextureSet=D2_TEXTURESET_WATER;
	GetDocument()->D2_LoadPIGData();
	GetCurrentView()->InvalidateRect(NULL);
	UpdateTextureBar();
}

void CMainFrame::OnViewSpecialdescent2settingsMaintexturesetIce() 
{
	GetDocument()->m_D2_TextureSet=D2_TEXTURESET_ICE;
	GetDocument()->D2_LoadPIGData();
	GetCurrentView()->InvalidateRect(NULL);
	UpdateTextureBar();
}

void CMainFrame::OnViewSpecialdescent2settingsMaintexturesetFire() 
{
	GetDocument()->m_D2_TextureSet=D2_TEXTURESET_FIRE;
	GetDocument()->D2_LoadPIGData();
	GetCurrentView()->InvalidateRect(NULL);
	UpdateTextureBar();
}

void CMainFrame::OnViewSpecialdescent2settingsMaintexturesetAlien2() 
{
	GetDocument()->m_D2_TextureSet=D2_TEXTURESET_ALIEN2;
	GetDocument()->D2_LoadPIGData();
	GetCurrentView()->InvalidateRect(NULL);
	UpdateTextureBar();
}

void CMainFrame::OnViewSpecialdescent2settingsMaintexturesetAlien1() 
{
	GetDocument()->m_D2_TextureSet=D2_TEXTURESET_ALIEN1;
	GetDocument()->D2_LoadPIGData();
	GetCurrentView()->InvalidateRect(NULL);
	UpdateTextureBar();
}

void CMainFrame::OnUpdateViewSpecialdescent2settingsMaintexturesetGroupa(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_D2);
	pCmdUI->SetCheck(GetDocument()->m_D2_TextureSet==D2_TEXTURESET_GROUPA);
}

void CMainFrame::OnUpdateViewSpecialdescent2settingsMaintexturesetFire(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_D2);
	pCmdUI->SetCheck(GetDocument()->m_D2_TextureSet==D2_TEXTURESET_FIRE);
}

void CMainFrame::OnUpdateViewSpecialdescent2settingsMaintexturesetAlien2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_D2);
	pCmdUI->SetCheck(GetDocument()->m_D2_TextureSet==D2_TEXTURESET_ALIEN2);
}

void CMainFrame::OnUpdateViewSpecialdescent2settingsMaintexturesetAlien1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_D2);
	pCmdUI->SetCheck(GetDocument()->m_D2_TextureSet==D2_TEXTURESET_ALIEN1);
}

void CMainFrame::OnUpdateViewSpecialdescent2settingsMaintexturesetWater(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_D2);
	pCmdUI->SetCheck(GetDocument()->m_D2_TextureSet==D2_TEXTURESET_WATER);
}

void CMainFrame::OnUpdateViewSpecialdescent2settingsMaintexturesetIce(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_D2);
	pCmdUI->SetCheck(GetDocument()->m_D2_TextureSet==D2_TEXTURESET_ICE);
}

void CMainFrame::OnModelModelcomparisontable() 
{
	CExplorer *e=ExplorerPane();
	CTreeCtrl *t=ExplorerCtrl();
	int rm=GetCurrentView()->m_RenderMode;
	CTableList dlg;
	dlg.DoModal();
	GetDocument()->m_ModelLoaded=FALSE;
	HTREEITEM k=t->GetRootItem();
	GetCurrentView()->m_RenderMode=rm;
	e->DoEventhandling=TRUE;
	t->SelectItem(k);
	GetDocument()->m_ErrorString=ERROR_GEN_NOMODELSELECTED_TEXT;
	GetDocument()->m_ErrorCode=ERROR_GEN_NOMODELSELECTED;
	UpdateMainDisplay();
}

void CMainFrame::OnViewShowguns() 
{
	GetCurrentView()->m_ShowGuns=!GetCurrentView()->m_ShowGuns;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnUpdateViewShowguns(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCurrentView()->m_ShowGuns);
	pCmdUI->Enable(GetCurrentView()->m_DisplayGun==-1 && current_game!=GAME_D3);
}

void CMainFrame::OnViewShowsegments() 
{
	GetCurrentView()->m_ShowSegments=!GetCurrentView()->m_ShowSegments;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnUpdateViewShowsegments(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCurrentView()->m_ShowSegments);
	pCmdUI->Enable(current_game!=GAME_D3);
}

void CMainFrame::FS_SetDetailLevel(int mode)
{
	UpdateMainDisplay();
	UpdateTextureBar();
	if(m_EditorMode)
		return;
	if((current_game==GAME_FS) && (m_UseControlBar))
		((CGamePaneFS *)GetGamePane())->UpdateIt(GetCurrentView()->m_ShowShield,GetCurrentView()->m_ShowThruster,TRUE);
}

void CMainFrame::LoadOptions()
{
	//Load settings
	COptionsDlg dlg;
	char *options=new char[256];
	dlg.LoadOptions(options);
	m_ToolBarTextMode=*(options+0);
	m_ToolBarSize=*(options+1);
	m_SwapMouseButtons=*(options+2);
	m_ExplorerIndex=*(options+3);
	m_QuickRendering=*(options+5);
	delete[] options;
}

void CMainFrame::InitToolbars()
{
	DWORD style;
	DWORD ctrlstyle;

#define DEFAULTSTYLE WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC
	ctrlstyle=DEFAULTSTYLE;
	
	//Load settings
	LoadOptions();

	switch(m_ToolBarTextMode)
	{
	case TOOLBAR_TEXT_NONE:
		style=TBSTYLE_FLAT;
		break;

	case TOOLBAR_TEXT_BOTTOM:
		style=TBSTYLE_FLAT;
		ctrlstyle|=CBRS_GRIPPER;
		break;

	case TOOLBAR_TEXT_RIGHT:
		style=TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_AUTOSIZE;
		break;

	default:
		ASSERT(FALSE);
	}

	//Main toolbar
	m_wndToolBar.CreateEx(this, style, ctrlstyle);
	if(m_ToolBarSize==TOOLBAR_SIZE_SMALL)
		m_wndToolBar.LoadToolBar(IDR_MAINFRAME_SMALL);
	else
		m_wndToolBar.LoadToolBar(IDR_MAINFRAME);
	m_wndToolBar.SetWindowText("Main toolbar");
	SetButton(&m_wndToolBar,0,"Open","Open file");
	SetButton(&m_wndToolBar,1,"Reload","Reload");
	SetButton(&m_wndToolBar,2,"","Details");
	SetButton(&m_wndToolBar,4,"","Wireframe");
	SetButton(&m_wndToolBar,5,"","Surface");
	SetButton(&m_wndToolBar,6,"","Textured");
	SetButton(&m_wndToolBar,8,"Anti-Alias","Anti-Alias");
	SetButton(&m_wndToolBar,9,"Background","Background");
	SetButton(&m_wndToolBar,11,"Guns","Guns");
	SetButton(&m_wndToolBar,12,"Segments","Segments");
	//SetButton(&m_wndToolBar,14,"","Background");
	//SetButton(&m_wndToolBar,12,"","Prev. Model");
	//SetButton(&m_wndToolBar,13,"","Next Model");
	/*SetButton(&m_wndToolBar,14,"","Reset view");
	SetButton(&m_wndToolBar,15,"","Turn left");
	SetButton(&m_wndToolBar,16,"","Turn right");
	SetButton(&m_wndToolBar,17,"","Pitch up");
	SetButton(&m_wndToolBar,18,"","Pitch down");
	SetButton(&m_wndToolBar,19,"","Zoom in");
	SetButton(&m_wndToolBar,20,"","Zoom out");*/
	ResizeToolbar(&m_wndToolBar);

	//Descent 2 toolbar
	m_wndToolBarD2.CreateEx(this, style, ctrlstyle);
	if(m_ToolBarSize==TOOLBAR_SIZE_SMALL)
		m_wndToolBarD2.LoadToolBar(IDR_DESCENT2_SMALL);
	else
		m_wndToolBarD2.LoadToolBar(IDR_DESCENT2);
	m_wndToolBarD2.SetWindowText("Descent 2 toolbar");
	SetButton(&m_wndToolBarD2,0,"","Open POG");
	SetButton(&m_wndToolBarD2,1,"","Unload POG");
	SetButton(&m_wndToolBarD2,3,"GROUPA","GROUPA.PIG");
	SetButton(&m_wndToolBarD2,4,"WATER","WATER.PIG");
	SetButton(&m_wndToolBarD2,5,"FIRE","FIRE.PIG");
	SetButton(&m_wndToolBarD2,6,"ICE","ICE.PIG");
	SetButton(&m_wndToolBarD2,7,"ALIEN1","ALIEN1.PIG");
	SetButton(&m_wndToolBarD2,8,"ALIEN2","ALIEN2.PIG");
	ResizeToolbar(&m_wndToolBarD2);
	ShowControlBar(&m_wndToolBarD2,FALSE,FALSE);

	//Descent 3 toolbar
	m_wndToolBarD3.CreateEx(this, style, ctrlstyle);
	if(m_ToolBarSize==TOOLBAR_SIZE_SMALL)
		m_wndToolBarD3.LoadToolBar(IDR_DESCENT3_SMALL);
	else
		m_wndToolBarD3.LoadToolBar(IDR_DESCENT3);
	m_wndToolBarD3.SetWindowText("Descent 3 toolbar");
	SetButton(&m_wndToolBarD3,0,"Previous position","Prev. pos");
	SetButton(&m_wndToolBarD3,1,"Next position","Next pos");
	ResizeToolbar(&m_wndToolBarD3);
	ShowControlBar(&m_wndToolBarD3,FALSE,FALSE);

	//FreeSpace toolbar
	m_wndToolBarFS.CreateEx(this,style, ctrlstyle);
	if(m_ToolBarSize==TOOLBAR_SIZE_SMALL)
		m_wndToolBarFS.LoadToolBar(IDR_FREESPACE_SMALL);
	else
		m_wndToolBarFS.LoadToolBar(IDR_FREESPACE);
	m_wndToolBarFS.SetWindowText("FreeSpace 1/2 toolbar");
	SetButton(&m_wndToolBarFS,0,"","Detail 1");
	SetButton(&m_wndToolBarFS,1,"","Detail 2");
	SetButton(&m_wndToolBarFS,2,"","Detail 3");
	SetButton(&m_wndToolBarFS,3,"","Detail 4");
	SetButton(&m_wndToolBarFS,4,"","Detail 5");
	SetButton(&m_wndToolBarFS,5,"","Detail 6");
	SetButton(&m_wndToolBarFS,6,"","Debris");
	SetButton(&m_wndToolBarFS,8,"Shields","Show Shields");
	SetButton(&m_wndToolBarFS,9,"Thruster","Thruster");
	SetButton(&m_wndToolBarFS,10,"Docking Points","Docking Points");
	SetButton(&m_wndToolBarFS,12,"Rotate Parts","Rotate Parts");
	SetButton(&m_wndToolBarFS,14,"POF-Editor","POF-Editor");
	ResizeToolbar(&m_wndToolBarFS);
	ShowControlBar(&m_wndToolBarFS,FALSE,FALSE);

	//EditorFS toolbar
	m_wndToolBarEditorFS.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_AUTOSIZE /*| CBRS_ALIGN_BOTTOM*/, DEFAULTSTYLE);
	m_wndToolBarEditorFS.LoadToolBar(IDR_EDITORFS);
	m_wndToolBarFS.SetWindowText("FreeSpace 1/2 Editor toolbar");
	SetButton(&m_wndToolBarEditorFS,2,"Model","Model");
	SetButton(&m_wndToolBarEditorFS,3,"Submodels","Submodels");
	SetButton(&m_wndToolBarEditorFS,4,"Subsystems","Subsystems");
	SetButton(&m_wndToolBarEditorFS,5,"Shields","Shields");
	SetButton(&m_wndToolBarEditorFS,6,"Guns","Guns");
	SetButton(&m_wndToolBarEditorFS,7,"Turrets","Turrets");
	SetButton(&m_wndToolBarEditorFS,8,"Thrusters","Thrusters");
	SetButton(&m_wndToolBarEditorFS,9,"Dockpoints","Dockpoints");
	SetButton(&m_wndToolBarEditorFS,10,"PathInfo","PathInfo");
	ResizeToolbar(&m_wndToolBarEditorFS,TRUE);
	ShowControlBar(&m_wndToolBarEditorFS,FALSE,FALSE);

	//No model Toolbar
	m_wndToolbarNoModel.CreateEx(this, style, ctrlstyle);
	if(m_ToolBarSize==TOOLBAR_SIZE_SMALL)
		m_wndToolbarNoModel.LoadToolBar(IDR_NOMODEL_SMALL);
	else
		m_wndToolbarNoModel.LoadToolBar(IDR_NOMODEL);
	m_wndToolbarNoModel.SetWindowText("No model loaded toolbar");
	SetButton(&m_wndToolbarNoModel,0,"Options && Configuration","Options");
	SetButton(&m_wndToolbarNoModel,2,"About...","About");
	ResizeToolbar(&m_wndToolbarNoModel);
	ShowControlBar(&m_wndToolbarNoModel,TRUE,FALSE);

	//Navigation
	m_wndToolBarNavigation.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBarNavigation.LoadToolBar(IDR_NAVIGATION);

	//Status bar
	m_wndStatusBar.CreateEx(this,/*SBARS_SIZEGRIP |*/ SBT_TOOLTIPS);
	m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(0,0,SBPS_STRETCH,150);
	m_wndStatusBar.SetPaneInfo(1,1,SBPS_STRETCH,150);
	m_wndStatusBar.SetPaneInfo(2,2,0,150);
	//m_wndStatusBar.SetPaneInfo(3,1,0,150);

	EnableDocking(CBRS_ALIGN_ANY);
}

void CMainFrame::SetButton(CToolBar *tb, int buttonnr, CString RightText, CString BottomText)
{
	if(m_ToolBarTextMode==TOOLBAR_TEXT_BOTTOM)
		tb->SetButtonText(buttonnr,BottomText);
	if(m_ToolBarTextMode==TOOLBAR_TEXT_RIGHT)
		tb->SetButtonText(buttonnr,RightText);
}

void CMainFrame::OnFileReload() 
{
	CMODVIEW32App *app=(CMODVIEW32App *)AfxGetApp();
	app->ReloadFile(GetDocument()->GetPathName());
}

void CMainFrame::OnUpdateFileReload(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetPathName().IsEmpty() && !m_EditorMode);
}

void CMainFrame::OnExplorerBarClick(UINT nID)
{
	switch(nID)
	{
	case ID_EXPLORERBAR_TREEVIEW:
		m_ExplorerIndex=!m_ExplorerIndex;
		OnFileReload();
		break;

	case ID_EXPLORERBAR_FILTER:
		m_ExplorerFilterLowLevel=!m_ExplorerFilterLowLevel;
		OnFileReload();
		break;

	case ID_EXPLORERBAR_PREV:
		OnModelPrevious();
		break;

	case ID_EXPLORERBAR_NEXT:
		OnModelNext();
		break;

	case ID_EXPLORERVIEW_COMPARISONTABLE:
		OnModelModelcomparisontable();
	
	}
}

void CMainFrame::OnExplorerBarClick2(UINT nID)
{
	OnExplorerBarClick(nID);
}

void CMainFrame::OnUpdateModelModelcomparisontable(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(ExplorerCtrl()->GetCount()!=0);//( m_ExplorerVisible);	
}


void CMainFrame::OnEditor() 
{
	LoadEditor();
/*#ifdef _WITHEDITOR
	GameBarNone();

	m_wndSplitter.SetColumnInfo(1,500,0);
	m_wndSplitter.SetColumnInfo(2,140,0);

	CWnd *pWnd=m_ed2Splitter.GetPane(0,0);
	CEditorFS_Subsystems_1* pView = DYNAMIC_DOWNCAST(CEditorFS_Subsystems_1, pWnd);
	pView->Init();
	
	CMenu* pMenu = GetMenu();
	if(pMenu)
		pMenu->DestroyMenu();
	HMENU hMenu = NewMenu(1);
	pMenu = CMenu::FromHandle( hMenu );
	SetMenu(pMenu);
	m_hMenuDefault = hMenu;
	::SetMenu(m_hWnd,hMenu);
	RedrawWindow();

	//TODO: Modify acceleration keys

	//TODO: Set new toolbar
	//m_wndToolBar.

	m_wndSplitter.RecalcLayout();
#else
	ASSERT(FALSE);
	AfxAbort();
#endif*/
}


void CMainFrame::OnBackToViewer() 
{
	//UNUSED!!
	ASSERT(FALSE);
#ifdef _WITHEDITOR
	//TODO: Check if save is needed

	//Hide Editor panes
	m_wndSplitter.SetColumnInfo(2,0,10000);
	m_wndSplitter.RecalcLayout();

	//Restore menu
	CMenu* pMenu = GetMenu();
	if (pMenu)pMenu->DestroyMenu();
	HMENU hMenu = NewMenu(0);
	pMenu = CMenu::FromHandle( hMenu );
	SetMenu(pMenu);
	m_hMenuDefault = hMenu;
	::SetMenu(m_hWnd,hMenu);
	RedrawWindow();

	//TODO: Restore acceleration keys

	//Restore GameBar + FS ToolBar
	GameBarFS();

#else
	ASSERT(FALSE);
	AfxAbort();
#endif	

	
}

void CMainFrame::OnElementeditorCreate() 
{
}

void CMainFrame::OnElementeditorDelete() 
{
}

void CMainFrame::OnElementeditorCopy() 
{
}

void CMainFrame::OnElementeditor2Create() 
{
}

void CMainFrame::OnElementeditor2Delete() 
{
}

void CMainFrame::OnElementeditor2Copy() 
{
}


void CMainFrame::OnElementapplyerApply() 
{
#ifdef _WITHEDITOR
	ASSERT(FALSE);
	ASSERT(m_EditorMode);
	CEditorFS_SPCL *edv=(CEditorFS_SPCL *)GetGamePane();
	edv->DoApply();	
#endif
}

void CMainFrame::OnElementapplyerDiscard() 
{
#ifdef _WITHEDITOR
	ASSERT(FALSE);
	ASSERT(m_EditorMode);
	CEditorFS_SPCL *edv=(CEditorFS_SPCL *)GetGamePane();
	edv->DoDiscard();
#endif
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	m_LastWidth=cx;
	m_LastHeight=cy;
	OnSize2();
}

void CMainFrame::OnSize2()
{
#ifdef _DEBUG
	if(!IsWindow(m_wndSplitter.m_hWnd))
		OutputDebugString("#NoWnd#");
	if(m_wndSplitter.GetColumnCount()!=3)
		OutputDebugString("#NoThr#");
	if(!st)
		OutputDebugString("#NoOCC#");
#endif

	if(st && IsWindow(m_wndSplitter.m_hWnd) && m_wndSplitter.GetColumnCount()==3)
	{
		int c1,m1;
		m_wndSplitter.GetColumnInfo(0,c1,m1);
		int w3=m_LastWidth-c1-256/*232*/;
		if(w3<0)
			w3=0;
		m_wndSplitter.SetColumnInfo(2,200,140/*200*/);
		m_wndSplitter.SetColumnInfo(1,w3,0);
		m_wndSplitter.RecalcLayout();
	}
}


void CMainFrame::UpdateNavStatus()
{
	CMODVIEW32Doc* pDoc=GetDocument();

	if(pDoc->m_ModelLoaded)
	{
		CMODVIEW32View* view=GetCurrentView();
		CString x;
		x.Format(" XRot: %2.2f  YRot: %2.2f  Zoom: %2.2f  Center: %2.1f|%2.1f", view->m_xRotation,view->m_yRotation,view->m_xScaling,view->m_xTranslation,view->m_yTranslation);
		m_wndStatusBar.SetPaneText(1,x);
	}
	else
		m_wndStatusBar.SetPaneText(1,"No model loaded.");
	m_wndStatusBar.SetPaneText(2,"V"+DMReg_GetVersion());
}



void CMainFrame::LoadEditor()
{
	m_EditorFS_CurrentView=0;
	if(GetDocument()->m_FS_ModelInfo.num_eye_points>1)
	{
		if(MessageBox("WARNING! The POF file you have loaded has more than one eye points defined. The model editor however only supports one eye point. If you continue, all eye points except the first one are lost (except you save as a different file name). Are you sure you want to continue?","Confirmation",MB_YESNO)==IDNO)
			return;
		GetDocument()->m_FS_ModelInfo.num_eye_points=1;
	}

	if(GetDocument()->m_CurrentFile_Offset!=0)
	{
		if(MessageBox("You cannot edit the POF file while it is inside a VP. Do you want to extract the file first? If you click 'Yes' you will be prompted for a file path and name next.","Extract POF",MB_YESNO)==IDNO)
			return;
				
		/*//Get modelsdir
		char mapsdirectory[8192];
		COptionsDlg dlgo;
		if(GetDocument()->m_FS_CurrVP_FreeSpaceVersion==1)
			dlgo.GetF1Path((char *)&mapsdirectory);
		else
			dlgo.GetF2Path((char *)&mapsdirectory);
		CString mapsfilename=mapsdirectory;
		if(mapsfilename.Right(1)!="\\")
			mapsfilename+="\\";
		mapsfilename+="data\\models\\";
		mapsfilename+=GetDocument()->m_CurrentFile_FileName;

		//Prompt for POF dir/file
		CFileDialog dlg(FALSE,"pof",mapsfilename,OFN_EXPLORER | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT,"POF files (*.pof)|*.pof||",NULL);
		if(dlg.DoModal()==IDCANCEL)
			return;*/

		CString vpname=GetDocument()->GetPathName();
		if(!AskForFileName())
			return;
		CFile fvp,fpof;
		if(!fvp.Open(vpname,CFile::modeRead))
		{
			MessageBox("Could not open VP.","Error");
			return;
		}
		if(!fpof.Open(GetDocument()->GetPathName(),CFile::modeCreate | CFile::modeWrite))
		{
			MessageBox("Could not write POF.","Error");
			return;
		}
		fvp.Seek(GetDocument()->m_CurrentFile_Offset,CFile::begin);
		unsigned char *buffer=new unsigned char[GetDocument()->m_CurrentFile_Size];
		fvp.Read(buffer,(UINT)GetDocument()->m_CurrentFile_Size);
		fpof.Write(buffer,(UINT)GetDocument()->m_CurrentFile_Size);
		delete[] buffer;
		fvp.Close();
		fpof.Close();
		GetDocument()->m_FS_CurrVP_Loaded=FALSE;
		GetDocument()->m_CurrentFile_Offset=0;
		//GetDocument()->SetPathName(dlg.GetPathName(),TRUE);
		ExplorerClear();
		ExplorerHide();
	}
	else
		GetDocument()->m_CurrentFile_FileName=GetDocument()->GetTitle();

	m_EditorMode=TRUE;
	ShowControlBar(&m_wndToolBarEditorFS,TRUE,FALSE);
	OnEditorFS_ToolbarClick(ID_EDITORFS_MODEL);
}


void CMainFrame::OnEditorFS_ToolbarClick(UINT nID)
{
#ifdef _WITHEDITOR
	m_Check_ToolbarClick_Sucess=FALSE;
	switch(m_EditorFS_CurrentView)
	{
	case ID_EDITORFS_MODEL: { CEditorFS_MODEL *s=(CEditorFS_MODEL *)GetGamePane(); if(!s->UnInit()) return; } break;
	case ID_EDITORFS_SPCL:  { CEditorFS_SPCL  *s=(CEditorFS_SPCL  *)GetGamePane(); if(!s->UnInit()) return; } break;
	case ID_EDITORFS_TGUN:  { CEditorFS_TGUN  *s=(CEditorFS_TGUN  *)GetGamePane(); if(!s->UnInit()) return; } break;
	case ID_EDITORFS_GPNT:  { CEditorFS_GPNT  *s=(CEditorFS_GPNT  *)GetGamePane(); if(!s->UnInit()) return; } break;
	case ID_EDITORFS_SHLD:  { CEditorFS_SHLD  *s=(CEditorFS_SHLD  *)GetGamePane(); if(!s->UnInit()) return; } break;
	case ID_EDITORFS_SOBJ:  { CEditorFS_SOBJ  *s=(CEditorFS_SOBJ  *)GetGamePane(); if(!s->UnInit()) return; } break;
	case ID_EDITORFS_FUEL:  { CEditorFS_FUEL  *s=(CEditorFS_FUEL  *)GetGamePane(); if(!s->UnInit()) return; } break;
	default: break;
	}	

	InvalidateRect(NULL);

	m_Check_ToolbarClick_Sucess=TRUE;
	if(nID==-1)
		return;

	m_EditorFS_CurrentView=nID;
	switch(nID)
	{
	case ID_EDITORFS_MODEL: { m_wndSplitter.ShowView(m_paneEdFS_MODEL); CEditorFS_MODEL *s=(CEditorFS_MODEL *)GetGamePane(); s->Init(); break; }
	case ID_EDITORFS_SPCL: { m_wndSplitter.ShowView(m_paneEdFS_SPCL); CEditorFS_SPCL *s=(CEditorFS_SPCL *)GetGamePane(); s->Init(); break; }
	case ID_EDITORFS_TGUN: { m_wndSplitter.ShowView(m_paneEdFS_TGUN); CEditorFS_TGUN *s=(CEditorFS_TGUN *)GetGamePane(); s->Init(); break; }
	case ID_EDITORFS_GPNT: { m_wndSplitter.ShowView(m_paneEdFS_GPNT); CEditorFS_GPNT *s=(CEditorFS_GPNT *)GetGamePane(); s->Init(); break; }
	case ID_EDITORFS_SHLD: { m_wndSplitter.ShowView(m_paneEdFS_SHLD); CEditorFS_SHLD *s=(CEditorFS_SHLD *)GetGamePane(); s->Init(); break; }
	case ID_EDITORFS_SOBJ: { m_wndSplitter.ShowView(m_paneEdFS_SOBJ); CEditorFS_SOBJ *s=(CEditorFS_SOBJ *)GetGamePane(); s->Init(); break; }
	case ID_EDITORFS_FUEL: { m_wndSplitter.ShowView(m_paneEdFS_FUEL); CEditorFS_FUEL *s=(CEditorFS_FUEL *)GetGamePane(); s->Init(); break; }
	}	
	InvalidateRect(NULL);
#endif
}

void CMainFrame::OnEditorFS_ToolbarClick_Update(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_EditorMode);
	pCmdUI->SetCheck(pCmdUI->m_nID==m_EditorFS_CurrentView);
}

//Menu item
void CMainFrame::OnUpdateEditor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_EditorMode && current_game==GAME_FS);
}

//Toolbar button
void CMainFrame::OnUpdateEditorPofeditortoogle(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(current_game==GAME_FS);
	pCmdUI->SetCheck(m_EditorMode);
}

void CMainFrame::OnEditorPofeditortoogle() 
{
	ASSERT(current_game==GAME_FS);
	if(!m_EditorMode)
		OnEditor();
	else
		OnViewer();
}

void CMainFrame::OnViewer() 
{
	//Save and if failed abort
	if(!GetDocument()->SaveModified2())
		return; // user canceled save
	OnEditorFS_ToolbarClick(-1);

	GetDocument()->SetModifiedFlag(FALSE); //just to be sure

	//Everything alright, go back to viewer mode
	ShowControlBar(&m_wndToolBarEditorFS,FALSE,FALSE);
	m_EditorMode=FALSE;

	m_wndSplitter.ShowView(m_paneFS);
	CGamePaneFS *s=(CGamePaneFS *)GetGamePane();
	s->Init();

	//If new model then loose model completely
	if(GetDocument()->m_UntitledModel)
		GetDocument()->ClearDocument();
}

void CMainFrame::OnUpdateViewer(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EditorMode);
}

void CMainFrame::OnUpdateEditorSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EditorMode);	
}


void CMainFrame::OnViewer2()
{
	OnViewer();
}

void CMainFrame::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE /*!m_EditorMode*/);
}

void CMainFrame::OnUpdateFileOptionsconfiguration(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_EditorMode);
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString x;
	x.Format("OnKeyDown: %i %i %i\n",nChar,nRepCnt,nFlags);
	OutputDebugString(x);
	
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnPredefinedView(UINT nID)
{
	float rx,ry;
	switch(nID)
	{
	case ID_NAVIGATION_VIEWFROMFRONT:	rx=  0.0;	ry=  0.0;	break;
	case ID_NAVIGATION_VIEWFROMBACK:	rx=  0.0;	ry=180.0;	break;
	case ID_NAVIGATION_VIEWFROMLEFT:	rx=  0.0;	ry= 90.0;	break;
	case ID_NAVIGATION_VIEWFROMRIGHT:	rx=  0.0;	ry=270.0;	break;
	case ID_NAVIGATION_VIEWFROMTOP:		rx= 90.0;	ry=  0.0;	break;
	case ID_NAVIGATION_VIEWFROMBOTTOM:	rx=270.0;	ry=  0.0;	break;
	default:							ASSERT(FALSE);			return;
	}
	
	GetCurrentView()->m_xRotation=rx;
	GetCurrentView()->m_yRotation=ry;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnPredefinedView_Update(CCmdUI* pCmdUI) 
{
	CMODVIEW32Doc* pDoc=GetDocument();
	pCmdUI->Enable(pDoc->m_ModelLoaded);	
}

void CMainFrame::OnModelCopy() 
{
	GetCurrentView()->CopyToClipboard();
}

void CMainFrame::OnUpdateModelCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_ModelLoaded/*AND TEXTURES SHOWN!TODO*/);	
}

BOOL CMainFrame::EditorFS_FinishPendingEditing()
{
#ifdef _WITHEDITOR
	if(!m_EditorMode)
		return TRUE;
	switch(m_EditorFS_CurrentView)
	{
	case ID_EDITORFS_MODEL: { CEditorFS_MODEL *s=(CEditorFS_MODEL *)GetGamePane(); if(!s->FinishPendingEditing()) return FALSE; } break;
	case ID_EDITORFS_SPCL:  { CEditorFS_SPCL  *s=(CEditorFS_SPCL  *)GetGamePane(); if(!s->FinishPendingEditing()) return FALSE; } break;
	case ID_EDITORFS_TGUN:  { CEditorFS_TGUN  *s=(CEditorFS_TGUN  *)GetGamePane(); if(!s->FinishPendingEditing()) return FALSE; } break;
	case ID_EDITORFS_GPNT:  { CEditorFS_GPNT  *s=(CEditorFS_GPNT  *)GetGamePane(); if(!s->FinishPendingEditing()) return FALSE; } break;
	case ID_EDITORFS_SHLD:  { CEditorFS_SHLD  *s=(CEditorFS_SHLD  *)GetGamePane(); if(!s->FinishPendingEditing()) return FALSE; } break;
	case ID_EDITORFS_SOBJ:  { CEditorFS_SOBJ  *s=(CEditorFS_SOBJ  *)GetGamePane(); if(!s->FinishPendingEditing()) return FALSE; } break;
	default: break;
	}	
#endif
	return TRUE;
}

void CMainFrame::OnExplorerviewComparisontable() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnExplorerbarNext() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnExplorerbarFilter() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnExplorerbarPrev() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnExplorerbarTreeview() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnUpdateFileSaveAsFs1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EditorMode && GetDocument()->m_FS_CurrVP_FreeSpaceVersion==2);
}

void CMainFrame::OnUpdateFileSaveAsFs2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EditorMode && GetDocument()->m_FS_CurrVP_FreeSpaceVersion==1);
}

void CMainFrame::OnFileSaveAsFs1() 
{
#ifdef _WITHEDITOR
	GetDocument()->m_FS_CurrVP_FreeSpaceVersion=1;
	if(!AskForFileName())
	{
		GetDocument()->m_FS_CurrVP_FreeSpaceVersion=2;
		return;
	}
	//Generate dummy file -> HACK to prevent OnFileSave not asking for filename again if file does not exist yet
	CFile f;
	f.Open(GetDocument()->GetPathName(),CFile::modeWrite | CFile::modeCreate);
	f.Close();

	GetDocument()->OnFileSave2();
	if(m_EditorFS_CurrentView==ID_EDITORFS_MODEL)
	{
		CEditorFS_MODEL *s=(CEditorFS_MODEL *)GetGamePane();
		s->ChangeFSMode();
	}
	GetCurrentView()->InvalidateRect(NULL);
#endif
}

void CMainFrame::OnFileSaveAsFs2() 
{
#ifdef _WITHEDITOR
	GetDocument()->m_FS_CurrVP_FreeSpaceVersion=2;
	if(!AskForFileName())
	{
		GetDocument()->m_FS_CurrVP_FreeSpaceVersion=1;
		return;
	}
	//Generate dummy file -> HACK to prevent OnFileSave not asking for filename again if file does not exist yet
	CFile f;
	f.Open(GetDocument()->GetPathName(),CFile::modeWrite | CFile::modeCreate);
	f.Close();

	GetDocument()->OnFileSave2();
	if(m_EditorFS_CurrentView==ID_EDITORFS_MODEL)
	{
		CEditorFS_MODEL *s=(CEditorFS_MODEL *)GetGamePane();
		s->ChangeFSMode();
	}
	GetCurrentView()->InvalidateRect(NULL);
#endif
}

void CMainFrame::OnViewRotate() 
{
	GetCurrentView()->m_Rotation=0;
	GetCurrentView()->m_DoRotation=!GetCurrentView()->m_DoRotation;
	GetCurrentView()->InvalidateRect(NULL);
}

void CMainFrame::OnUpdateViewRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetCurrentView()->m_DoRotation);
}

void CMainFrame::OnHelpUsing() 
{
	CHelp_Using dlg;
	dlg.DoModal();
}

BOOL CMainFrame::AskForFileName()
{
	//Get modelsdir
	CString mapsdirectory;
	COptionsDlg dlgo;
	if(GetDocument()->m_FS_CurrVP_FreeSpaceVersion==1)
		mapsdirectory=dlgo.GetF1Path();
	else
		mapsdirectory=dlgo.GetF2Path();
	CString mapsfilename=mapsdirectory;
	if(mapsfilename.Right(1)!="\\")
		mapsfilename+="\\";
	mapsfilename+="data\\models\\";

	//Create dirs if not available
	for(int i=0;i<mapsfilename.GetLength();i++)
	{
		if(mapsfilename[i]=='\\')
		{
			if(_mkdir(mapsfilename.Left(i))!=0)
			{
				TRACE("errno=%i\n",errno);
				if(errno!=EEXIST)
				{
					//mapsdirectory="c:\\";
					//mapsfilename="c:\\";
					break;
				}
			}
		}
	}

	mapsfilename+=GetDocument()->m_CurrentFile_FileName;
	
	//Prompt for POF dir/file
	m_VPFileDialog=new CVPFileDialog(FALSE,"pof",mapsfilename,OFN_EXPLORER | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT,"POF files (*.pof)|*.pof||",NULL);
	int ret=(m_VPFileDialog->DoModal()==IDOK);
	if(ret)
		GetDocument()->SetPathName(m_VPFileDialog->GetPathName(),TRUE);

	//Tidy up and return
	delete(m_VPFileDialog);
	m_VPFileDialog=NULL;
	return ret;
}

void CMainFrame::UpdateTextureBar()
{
#ifdef _WITHTXVIEW
	CWnd *pWnd1;
	pWnd1=m_wndSplitterTX3.GetPane(0, 0);
	CTXViewModeToolbar* pView1 = DYNAMIC_DOWNCAST(CTXViewModeToolbar, pWnd1);
	CWnd *pWnd;
	pWnd=m_wndSplitterTX3.GetPane(0, 1);
	CTXList* pView = DYNAMIC_DOWNCAST(CTXList, pWnd);
	pView->FillTextureInfo(pView1->GetCurrent());

	int width=600;
	int minww=0;
	if(pView1->GetCurrent()!=0)
	{
		width=30000;
		minww=30000;
	}
	m_wndSplitterTX2.SetColumnInfo(0,width,0);
	m_wndSplitterTX2.SetColumnInfo(1,width,minww);
	m_wndSplitterTX2.RecalcLayout();
#endif
}

void CMainFrame::OnEditorImportpoffrom3dfile() 
{
#ifdef _WITHEDITOR
	if(m_EditorMode)
		OnViewer();
	if(m_EditorMode)
		return; //User cancelled save

	//Import
	GetDocument()->ClearDocument();
	//GetDocument()->SetPathName("",FALSE);
	CImportDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		GetDocument()->m_ErrorCode=dlg.errnum;
		GetDocument()->m_ErrorString=dlg.errstring;
#ifdef _WITHTXVIEW
		TXShowLogFile();
		//OnEditor();
#endif
	}
#endif
}

void CMainFrame::OnUpdateEditorImportpoffrom3dfile(CCmdUI* pCmdUI) 
{
#ifdef _WITHEDITOR
	pCmdUI->Enable(TRUE);
#else
	pCmdUI->Enable(FALSE);
#endif
}



#include "OpenDlg.h"
void CMainFrame::OnFileOpennewversion() 
{
	CMODVIEW32App*	pApp = (CMODVIEW32App*)AfxGetApp();
	COpenDlg dlg;	
	if(dlg.DoModal()==IDOK)
		pApp->OpenDocumentFile(dlg.m_OpenFileName);
}

//Called by CMODVIEW32View to set type of view and so let CMainFrame set
//whether to show CMODVIEW32View or CHTMLTaskView
void CMainFrame::SetTypeOfView(int type)
{
	switch(type)
	{
	case TOV_NODOCUMENT:
	case TOV_ARCHIVELOADED:
	case TOV_MODELVIEWBUTFAILED:
	case TOV_MODELVIEWBUTNOTEXTURES:
#ifdef _WITHHTVIEW
		((CHTMLTaskView *)m_wndSplitter.GetView(m_paneMainHTML))->ShowHTML(type);
		m_wndSplitter.ShowView(m_paneMainHTML);
#endif
		break;

	case TOV_MODELVIEW:
#ifdef _WITHHTVIEW
		TRACE("m_paneMainView=%i\n",m_paneMainView);
		m_wndSplitter.ShowView(m_paneMainView);
#endif
		break;

	default:
		ASSERT(FALSE);
	}
}

//User clicked a link within CHTMLTaskView
void CMainFrame::ParseCmd(CString cmd)
{
	//Note: We can't directly call the menu handling routines directly
	//      since it will crash when it tries to get the (inactive) opengl view window
	//      so we are using ::SendMessage whereever it is needed
	HWND wnd=AfxGetApp()->m_pMainWnd->m_hWnd;
	if(cmd=="Cmd:OpenDialog")
		::SendMessage(wnd,WM_COMMAND,ID_FILE_OPEN,0);
	if(cmd=="Cmd:ImportDialog") 
		::SendMessage(wnd,WM_COMMAND,ID_EDITOR_IMPORTPOFFROM3DFILE,0);
	if(cmd=="Cmd:OptionsDialog")
		OnFileOptionsconfiguration();
	if(cmd=="Cmd:HelpControls")
		OnHelpUsing();
	if(cmd=="Cmd:ModelComparisonTable")
		::SendMessage(wnd,WM_COMMAND,ID_MODEL_MODELCOMPARISONTABLE,0);
	if(cmd=="Cmd:ViewWireframe")
		::SendMessage(wnd,WM_COMMAND,ID_VIEW_RENDERMODE_WIREFRAME,0);
	if(cmd=="Cmd:ViewSurface")
		::SendMessage(wnd,WM_COMMAND,ID_VIEW_RENDERMODE_SURFACE,0);
}


void CMainFrame::UpdateMainDisplay()
{
#ifdef _WITHHTVIEW
	CString err;
	switch(GetDocument()->m_ErrorCode)
	{
	case ERROR_GEN_NOERROR:	
		if(GetDocument()->m_ModelLoaded)
			SetTypeOfView(TOV_MODELVIEW);
		else
			SetTypeOfView(TOV_NODOCUMENT);
		break;
	
	case ERROR_GEN_NOMODELSELECTED:
		SetTypeOfView(TOV_ARCHIVELOADED);
		break;

	default:	
		SetTypeOfView(TOV_MODELVIEWBUTFAILED);
	}
#endif
}


//Texture pane stuff

void CMainFrame::OnSelchangeTab1(NMHDR* _pnmtb, LRESULT *plr) 
{
	NMTOOLBAR *pnmtb = (NMTOOLBAR*)_pnmtb;
	UpdateTextureBar();
}

void CMainFrame::OnSelchangeTab1b() 
{
	UpdateTextureBar();
}

void CMainFrame::TXPaneShow()
{
#ifdef _WITHTXVIEW
	int w3=m_LastHeight-244;
	if(m_EditorMode)
		w3-=24;
	if(w3<0)
		w3=0;
	m_wndSplitterTX.SetRowInfo(1,200,0);
	m_wndSplitterTX.SetRowInfo(0,w3,0);
	m_wndSplitterTX.RecalcLayout();
#endif
}

void CMainFrame::TXPaneHide()
{
#ifdef _WITHTXVIEW
	int w3=m_LastHeight;
	if(w3<0)
		w3=0;
	m_wndSplitterTX.SetRowInfo(1,0,0);
	m_wndSplitterTX.SetRowInfo(0,w3,0);
	m_wndSplitterTX.RecalcLayout();
#endif
}

int CMainFrame::TXGetCurrent()
{
#ifdef _WITHTXVIEW
	int cur,min;
	m_wndSplitterTX.GetRowInfo(1,cur,min);
	if(cur<=0)
		return -1; //Hidden

	CWnd *pWnd=m_wndSplitterTX3.GetPane(0, 0);
	CTXViewModeToolbar* pView = DYNAMIC_DOWNCAST(CTXViewModeToolbar, pWnd);
	return pView->GetCurrent();		
#else
	return -1;
#endif
}

void CMainFrame::TXShowTextures()
{
#ifdef _WITHTXVIEW
	TXPaneShow();
	CWnd *pWnd=m_wndSplitterTX3.GetPane(0, 0);
	CTXViewModeToolbar* pView = DYNAMIC_DOWNCAST(CTXViewModeToolbar, pWnd);
	pView->ShowTextures();		
#else
	OnModelTexturelist();
#endif
}

void CMainFrame::TXShowBlockList()
{
#ifdef _WITHTXVIEW
	TXPaneShow();
	CWnd *pWnd=m_wndSplitterTX3.GetPane(0, 0);
	CTXViewModeToolbar* pView = DYNAMIC_DOWNCAST(CTXViewModeToolbar, pWnd);
	pView->ShowBlockList();		
#endif
}

void CMainFrame::TXShowLogFile()
{
#ifdef _WITHTXVIEW
	TXPaneShow();
	CWnd *pWnd=m_wndSplitterTX3.GetPane(0, 0);
	CTXViewModeToolbar* pView = DYNAMIC_DOWNCAST(CTXViewModeToolbar, pWnd);
	pView->ShowLogFile();		
#else
	//Important for COB import
	CMODVIEW32App *app=(CMODVIEW32App *)AfxGetApp();
	ShellExecute(NULL,"open","notepad",app->m_LogFile,"",SW_SHOWNORMAL);
#endif
}

void CMainFrame::OnUpdateTXShowTextures(CCmdUI* pCmdUI)
{
#ifdef _WITHTXVIEW
	pCmdUI->SetCheck(TXGetCurrent()==0);
#else
	//Old version is only supported when a model is loaded.
	pCmdUI->Enable(GetDocument()->m_ModelLoaded);
#endif
}

void CMainFrame::OnUpdateTXShowBlockList(CCmdUI* pCmdUI)
{
#ifdef _WITHTXVIEW
	pCmdUI->SetCheck(TXGetCurrent()==1);
#else
	//Never available directly
	pCmdUI->Enable(FALSE);
#endif
}

void CMainFrame::OnUpdateTXShowLogFile(CCmdUI* pCmdUI)
{
#ifdef _WITHTXVIEW
	pCmdUI->SetCheck(TXGetCurrent()==2);
#else
	//Always enabled
	pCmdUI->Enable(GetDocument()->m_ModelLoaded);
#endif
}

void CMainFrame::OnUpdateTXHide(CCmdUI* pCmdUI)
{
#ifdef _WITHTXVIEW
	pCmdUI->SetCheck(TXGetCurrent()==-1);
#else
	//Never available
	pCmdUI->Enable(FALSE);
#endif
}



//Open/Save dialog helper functions

void CMainFrame::OnVpfiledialogJumptodescent2directory() 
{
	VPFileDialogJump(DMReg_GetGameDir("Descent 2"));
}

void CMainFrame::OnUpdateVpfiledialogJumptodescent2directory(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(DMReg_GetGameDir("Descent 2")!="");		
}

void CMainFrame::OnVpfiledialogJumptodescent3directory() 
{
	VPFileDialogJump(DMReg_GetGameDir("Descent 3"));
}

void CMainFrame::OnUpdateVpfiledialogJumptodescent3directory(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(DMReg_GetGameDir("Descent 3")!="");		
}

void CMainFrame::OnVpfiledialogJumptofreespace1directory() 
{
	VPFileDialogJump(DMReg_GetGameDir("FreeSpace 1"));
}

void CMainFrame::OnUpdateVpfiledialogJumptofreespace1directory(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(DMReg_GetGameDir("FreeSpace 1")!="");		
}

void CMainFrame::OnVpfiledialogJumptofreespace2directory() 
{
	VPFileDialogJump(DMReg_GetGameDir("FreeSpace 2"));
}

void CMainFrame::OnUpdateVpfiledialogJumptofreespace2directory(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(DMReg_GetGameDir("FreeSpace 2")!="");
}

void CMainFrame::VPFileDialogJump(CString path)
{
	ASSERT(m_VPFileDialog!=NULL);
	ASSERT(path!="");
	if(m_VPFileDialog==NULL)
		return;
	if(path=="")
		return;
	m_VPFileDialog->SetPath(path);
}
