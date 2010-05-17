// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1ECD613D_A743_11D3_9455_00105A9D6FFE__INCLUDED_)
#define AFX_MAINFRM_H__1ECD613D_A743_11D3_9455_00105A9D6FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "explorer.h"
#include "modview32doc.h"
#include "modview32view.h"
#include "TXList.h"
#include "TXView.h"
#include "TXViewModeToolbar.h"
#include "SilentSplitterWnd.h"
#include "splitterex.h"
#include "BCMenu.h"
#include "AW_CMultiViewSplitter.h"
#include "VPFileDialog.h"

#define TOOLBAR_TEXT_NONE 0
#define TOOLBAR_TEXT_RIGHT 1
#define TOOLBAR_TEXT_BOTTOM 2

#define TOOLBAR_SIZE_LARGE 0
#define TOOLBAR_SIZE_SMALL 1

#define TOV_NODOCUMENT	0
#define TOV_ARCHIVELOADED	1
#define TOV_MODELVIEW	2
#define TOV_MODELVIEWBUTNOTEXTURES	3
#define TOV_MODELVIEWBUTFAILED	4


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnExplorerBarClick(UINT nID);
	void OnExplorerBarClick2(UINT nID);
	BOOL m_ExplorerIndex;
	BOOL m_ExplorerFilterLowLevel;
	CString m_TypeRootsFile;
	BOOL m_UseControlBar;
	BOOL m_SwapMouseButtons;
	BOOL m_QuickRendering;
	int m_ToolBarSize;
	int m_ToolBarTextMode;
	void SetButton(CToolBar *tb,int buttonnr,CString RightText,CString BottomText);
	void InitToolbars();
	void LoadOptions();
	void FS_SetDetailLevel(int mode);
	void FS_ShowShield();
	void FS_ShowThruster();
	void DockControlBarLeftOf(CToolBar *Bar, CToolBar *LeftOf);
	void ResizeToolbar(CToolBar *tb,BOOL alwayssmall=FALSE);
//	void GameBarD2();
	int ExplorerOpenSubFile(char *filename);
	//BCMenu
	HMENU NewMenu(int menu=0);
	BCMenu m_menu;

	int m_ExplorerVisible;
	float m_ZoomLevel;
	float m_XRotation;
	float m_YRotation;
	void ExplorerShow();
	void ExplorerHide();
	void ExplorerGoTop();
	HTREEITEM m_ExplorerLastRoot;
	void ExplorerAddRoot(char *text);
	void ExplorerClear();
	void SetStatusBarText(char *txt);
	int current_game;
	CMODVIEW32Doc * GetDocument();
	CMODVIEW32View * GetCurrentView();
	CView * GetGamePane();
	CTreeCtrl * ExplorerCtrl();
	CExplorer *ExplorerPane();
	void ExplorerAddChild(char *filename,int typenr=-1);
	virtual ~CMainFrame();
	void GameBarNone();
//	void GameBarD2();
//	void GameBarD3();
	void GameBarFS();
	float CalcPaneStep();
	float CalcPaneDrag();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar	m_wndToolbarNoModel;
//	CToolBar    m_wndToolBarD2;
//	CToolBar    m_wndToolBarD3;
	CToolBar    m_wndToolBarFS;
	CToolBar	m_wndToolBarEditorFS;
	CToolBar	m_wndToolBarNavigation;
	//CGameBarD2	m_wndDlgBarD2;
	//CGameBarD3	m_wndDlgBarD3;
	//CGameBarFS	m_wndDlgBarFS;
	int m_paneFS,
		m_paneWelcome,
		m_paneEdFS_MODEL,
		m_paneEdFS_SPCL,
		m_paneEdFS_TGUN,
		m_paneEdFS_GPNT,
		m_paneEdFS_SHLD,
		m_paneEdFS_SOBJ,
		m_paneEdFS_FUEL;
#ifdef _WITHHTVIEW
	int m_paneMainHTML,m_paneMainView;
#endif
	//CEditBarFS	m_wndDlgBarFSeditor;
	AW_CMultiViewSplitter	m_wndSplitter;
#ifdef _WITHTXVIEW
	AW_CMultiViewSplitter	m_wndSplitterTX;
	AW_CMultiViewSplitter	m_wndSplitterTX2;
	CSilentSplitterWnd	m_wndSplitterTX3;
#endif
	//CSplitterWndEx m_wndSplitter;
	CSilentSplitterWnd m_barSplitter;
	CSilentSplitterWnd m_edtSplitter;

public:
	void UpdateMainDisplay();
//	void GameBarRF();
	void ParseCmd(CString cmd);
	void SetTypeOfView(int type);
	int m_TXCurrent;
	void UpdateTextureBar();
	BOOL EditorFS_FinishPendingEditing();
	BOOL m_Check_ToolbarClick_Sucess;
	BOOL m_Check_SavePOF_Sucess;
	void OnViewer2();
	int m_EditorFS_CurrentView;
	afx_msg void OnEditorFS_ToolbarClick(UINT nID);
	BOOL m_EditorMode;
	void LoadEditor();
	void UpdateNavStatus();
	int m_LastWidth;
	int m_LastHeight;
	void OnSize2();
	void OnSelchangeTab1b();
	BOOL AskForFileName();
/*#ifdef _WITHEDITOR
	CCreateContext * m_CreateContext;
	void EditorModeSelect(int mode);
	CSplitterWnd	m_ed2Splitter;
#endif*/

// Generated message map functions
public:
	afx_msg void TXShowTextures();
	afx_msg void TXShowBlockList();
	afx_msg void TXShowLogFile();
	afx_msg void TXPaneShow();
	afx_msg void TXPaneHide();
	afx_msg void OnUpdateTXShowTextures(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTXShowBlockList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTXShowLogFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTXHide(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeTab1(NMHDR* _pnmtb, LRESULT *plr);
	int TXGetCurrent();
	void VPFileDialogJump(CString path);
	CVPFileDialog* m_VPFileDialog;

protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewDetaillevelDebris();
	afx_msg void OnViewDetaillevelLevel1();
	afx_msg void OnViewDetaillevelLevel2();
	afx_msg void OnModelLeft();
	afx_msg void OnModelRight();
	afx_msg void OnModelUp();
	afx_msg void OnModelZoomin();
	afx_msg void OnModelZoomout();
	afx_msg void OnViewDetaillevelLevel3();
	afx_msg void OnViewDetaillevelLevel4();
	afx_msg void OnViewDetaillevelLevel5();
	afx_msg void OnViewDetaillevelLevel6();
	afx_msg void OnUpdateViewDetaillevelLevel1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDetaillevelLevel2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDetaillevelLevel3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDetaillevelLevel4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDetaillevelLevel5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDetaillevelLevel6(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDetaillevelDebris(CCmdUI* pCmdUI);
	afx_msg void OnViewRendermodeWireframe();
	afx_msg void OnViewRendermodeSurface();
	afx_msg void OnViewRendermodeTextured();
	afx_msg void OnUpdateViewRendermodeWireframe(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewRendermodeTextured(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewRendermodeSurface(CCmdUI* pCmdUI);
	afx_msg void OnViewShowshields();
	afx_msg void OnUpdateViewShowshields(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewShowthruster(CCmdUI* pCmdUI);
	afx_msg void OnViewShowthruster();
	afx_msg void OnViewFasttextureload();
	afx_msg void OnUpdateViewFasttextureload(CCmdUI* pCmdUI);
	afx_msg void OnFileOptionsconfiguration();
	afx_msg void OnFileOpenfile();
	afx_msg void OnViewRendersettingsSmooth();
	afx_msg void OnUpdateViewRendersettingsSmooth(CCmdUI* pCmdUI);
	afx_msg void OnViewRendersettingsCullface();
	afx_msg void OnUpdateViewRendersettingsCullface(CCmdUI* pCmdUI);
	afx_msg void OnModelDown();
	afx_msg void OnViewRendersettingsZbuffered();
	afx_msg void OnUpdateViewRendersettingsZbuffered(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnNavigationResetzoomlevel();
	afx_msg void OnViewSelectbackgroundcolor();
	afx_msg void OnViewDorotation();
	afx_msg void OnUpdateViewDorotation(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnUpdateZoom(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRotation(CCmdUI *pCmdUI);
	afx_msg void OnModelNext();
	afx_msg void OnModelPrevious();
	afx_msg void OnModelTexturelist();
	afx_msg void OnUpdateModelTexturelist(CCmdUI* pCmdUI);
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnChildActivate();
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnUpdateModelNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelPrevious(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNavigation(CCmdUI* pCmdUI);
	//afx_msg void OnViewSpecialdescent3settingsNextposition();
	//afx_msg void OnViewSpecialdescent3settingsPreviousposition();
	//afx_msg void OnUpdateViewSpecialdescent3settingsNextposition(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateViewSpecialdescent3settingsPreviousposition(CCmdUI* pCmdUI);
	//afx_msg void OnViewSpecialdescent2settingsMaintexturesetGroupa();
	//afx_msg void OnViewSpecialdescent2settingsMaintexturesetWater();
	//afx_msg void OnViewSpecialdescent2settingsMaintexturesetIce();
	//afx_msg void OnViewSpecialdescent2settingsMaintexturesetFire();
	//afx_msg void OnViewSpecialdescent2settingsMaintexturesetAlien2();
	//afx_msg void OnViewSpecialdescent2settingsMaintexturesetAlien1();
	//afx_msg void OnUpdateViewSpecialdescent2settingsMaintexturesetGroupa(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateViewSpecialdescent2settingsMaintexturesetFire(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateViewSpecialdescent2settingsMaintexturesetAlien2(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateViewSpecialdescent2settingsMaintexturesetAlien1(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateViewSpecialdescent2settingsMaintexturesetWater(CCmdUI* pCmdUI);
	//afx_msg void OnUpdateViewSpecialdescent2settingsMaintexturesetIce(CCmdUI* pCmdUI);
	afx_msg void OnModelModelcomparisontable();
	afx_msg void OnViewShowguns();
	afx_msg void OnUpdateViewShowguns(CCmdUI* pCmdUI);
	afx_msg void OnViewShowsegments();
	afx_msg void OnUpdateViewShowsegments(CCmdUI* pCmdUI);
	afx_msg void OnViewShowdockpoints();
	afx_msg void OnUpdateViewShowdockpoints(CCmdUI* pCmdUI);
	afx_msg void OnViewShowthrusterglows();
	afx_msg void OnUpdateViewShowthrusterglows(CCmdUI* pCmdUI);
	afx_msg void OnFileReload();
	afx_msg void OnUpdateModelModelcomparisontable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileReload(CCmdUI* pCmdUI);
	afx_msg void OnEditor();
	afx_msg void OnElementeditorCreate();
	afx_msg void OnElementeditorDelete();
	afx_msg void OnBackToViewer();
	afx_msg void OnNavigationPaneleft();
	afx_msg void OnNavigationPaneright();
	afx_msg void OnNavigationPanedown();
	afx_msg void OnNavigationPaneup();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnElementapplyerApply();
	afx_msg void OnElementapplyerDiscard();
	afx_msg void OnUpdateEditor(CCmdUI* pCmdUI);
	afx_msg void OnViewer();
	afx_msg void OnUpdateViewer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditorSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOptionsconfiguration(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditorPofeditortoogle();
	afx_msg void OnUpdateEditorPofeditortoogle(CCmdUI* pCmdUI);
	afx_msg void OnElementeditorCopy();
	afx_msg void OnElementeditor2Create();
	afx_msg void OnElementeditor2Delete();
	afx_msg void OnElementeditor2Copy();
	afx_msg void OnModelCopy();
	afx_msg void OnUpdateModelCopy(CCmdUI* pCmdUI);
	afx_msg void OnExplorerviewComparisontable();
	afx_msg void OnExplorerbarNext();
	afx_msg void OnExplorerbarFilter();
	afx_msg void OnExplorerbarPrev();
	afx_msg void OnExplorerbarTreeview();
	afx_msg void OnUpdateFileSaveAsFs1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAsFs2(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAsFs1();
	afx_msg void OnFileSaveAsFs2();
	afx_msg void OnViewRotate();
	afx_msg void OnUpdateViewRotate(CCmdUI* pCmdUI);
	afx_msg void OnHelpUsing();
	afx_msg void OnEditorImportpoffrom3dfile();
	afx_msg void OnFileOpennewversion();
	afx_msg void OnUpdateEditorImportpoffrom3dfile(CCmdUI* pCmdUI);
//	afx_msg void OnVpfiledialogJumptodescent2directory();
	//afx_msg void OnUpdateVpfiledialogJumptodescent2directory(CCmdUI* pCmdUI);
	//afx_msg void OnVpfiledialogJumptodescent3directory();
	//afx_msg void OnUpdateVpfiledialogJumptodescent3directory(CCmdUI* pCmdUI);
	//afx_msg void OnVpfiledialogJumptofreespace1directory();
	//afx_msg void OnUpdateVpfiledialogJumptofreespace1directory(CCmdUI* pCmdUI);
	afx_msg void OnVpfiledialogJumptofreespace2directory();
	afx_msg void OnUpdateVpfiledialogJumptofreespace2directory(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnEditorFS_ToolbarClick_Update(CCmdUI *pCmdUI);
	afx_msg void OnPredefinedView_Update(CCmdUI* pCmdUI);
	afx_msg void OnPredefinedView(UINT nID);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1ECD613D_A743_11D3_9455_00105A9D6FFE__INCLUDED_)
