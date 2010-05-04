// MODVIEW32.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MODVIEW32.h"

#include "MainFrm.h"
#include "OptionsDlg.h"

#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "Explorer.h"
#include "CoolControlsManager.h"
#include "dm_tools.h"
#include "SplashNew.h"
#include "welcome.h"
#include "DM_Reg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMODVIEW32App

BEGIN_MESSAGE_MAP(CMODVIEW32App, CWinApp)
	//{{AFX_MSG_MAP(CMODVIEW32App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_UPDATE_COMMAND_UI(ID_APP_ABOUT, OnUpdateAppAbout)
	ON_COMMAND(ID_HELP_CHECKFORNEWVERSION, OnHelpCheckfornewversion)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMODVIEW32App construction

CMODVIEW32App::CMODVIEW32App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMODVIEW32App object

CMODVIEW32App theApp;

/////////////////////////////////////////////////////////////////////////////
// CMODVIEW32App initialization

BOOL CMODVIEW32App::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Descent Network"));

	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

	//Show splash window
	CSplashNew m_splash;
	splashisdisplayed=TRUE;
	m_splash.Create(m_pMainWnd);
	m_splash.ShowWindow(SW_SHOW);
	m_splash.UpdateWindow();
	m_splash.SetTimer(1, 500, NULL);
	showsplashlonger=TRUE;

	DMReg_Init();
	m_LogFile=DMReg_GetTEMPDir()+"MODVIW32.LOG";
TRY
{
    CFile::Remove(m_LogFile);
}
CATCH( CFileException, e )
{
    #ifdef _DEBUG
        afxDump << "File " << m_LogFile << " cannot be removed\n";
    #endif
}
END_CATCH
	
	GetCtrlManager().InstallHook();

	//OpenGL warning message
	if(DMReg_ReadHKCUint("OpenGL_WarningMessage",0)==0)
	{
		CString bm;
		bm+="Please note that on some systems the tool might crash when loading models, e.g. on most ATI graphic cards loading Descent 3 models might crash the program. This usually happens due to incompatible OpenGL drivers of the graphic card.\n\n";
		bm+="If you experience this problem, there is one workaround: Install software OpenGL in the MODELVIEW32 directory:\n\n";
		bm+="- Go to the Help|About menu before you load a model, click on the 'Internet' tab and select the OpenGL drivers download. Click 'Go' and they will be downloaded.\n";
		bm+="- Extract the downloaded file to your MODELVIEW32 directory. This way other OpenGL programs and games will not be influenced and still use your hardware accelerated drivers.\n";
		bm+="- After that restart the tool. It should run a lot more stable now.\n\n";
		bm+="This is a one-time message and won't be displayed again. Have fun (c:.";

		AfxMessageBox(bm);
		DMReg_WriteHKCU("OpenGL_WarningMessage",1);
	}

	//If 1st startup, ask if *.model files should be associated with MODELVIEW32 TODO!
	if(Check1stStartup())
		m_splash.RedrawWindow();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMODVIEW32Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CExplorer));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if(!ProcessShellCommand(cmdInfo))
		return FALSE;

	//Sleep a bit for the splash screen, when no file has been loaded at startup
#ifndef _DEBUG
	if(showsplashlonger)
	{	//HACK!
		m_pMainWnd->ShowWindow(SW_HIDE);
		m_splash.UpdateWindow();
		Sleep(2000);
	}
#endif

	//Initialize BCMenu (menus with icons)
	CMenu* pMenu = m_pMainWnd->GetMenu();
	if (pMenu)pMenu->DestroyMenu();
	HMENU hMenu = ((CMainFrame*) m_pMainWnd)->NewMenu();
	pMenu = CMenu::FromHandle( hMenu );
	m_pMainWnd->SetMenu(pMenu);
	((CMainFrame*)m_pMainWnd)->m_hMenuDefault = hMenu;
	
	// The one and only window has been initialized, so show and update it.
	::SendMessage(m_pMainWnd->m_hWnd,WM_SYSCOMMAND,SC_MAXIMIZE,0); //Maximize main window
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	//Initialisation has finished, so hide the splash screen
	m_splash.DestroyWindow();
	splashisdisplayed=FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMODVIEW32App::OnAppAbout()
{
	DoAbout();
}

/////////////////////////////////////////////////////////////////////////////
// CMODVIEW32App message handlers


void CMODVIEW32App::OnUpdateAppAbout(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(FALSE);	
}






// Progress bar routine
void CMODVIEW32App::DisplayProgressWnd(char *action)
{
	progWnd.Create(m_pMainWnd);
	progWnd.ShowWindow(TRUE);
	progWnd.EnableWindow(FALSE);
	if(splashisdisplayed)
		progWnd.SetWindowPos(NULL,(::GetSystemMetrics(SM_CXSCREEN) - 422) / 2,(::GetSystemMetrics(SM_CYSCREEN) - 114) / 2+148,100,100,SWP_NOSIZE);
	progWnd.m_Action.SetWindowText(action);
	progWnd.RedrawWindow();
	showsplashlonger=FALSE;
}
 
// Progress bar routine
void CMODVIEW32App::UpdateProgressWnd(int now, int all)
{
	char x[256];
	sprintf_s(x,"Model #%i of %i (%i%%)",now,all,(now*100/all));
	progWnd.m_Frame.SetWindowText(x);
	progWnd.m_Progress1.SetRange32(0,all);
	progWnd.m_Progress1.SetPos(now);
	progWnd.m_Frame.RedrawWindow();
	progWnd.m_Progress1.RedrawWindow();
}


// Progress bar routine
void CMODVIEW32App::StatusProgressWnd(char *x)
{
	progWnd.m_Frame.SetWindowText(x);
	progWnd.m_Frame.RedrawWindow();
}

// Progress bar routine
void CMODVIEW32App::RedrawProgressWnd()
{
	progWnd.RedrawWindow();
}

// Progress bar routine
void CMODVIEW32App::CloseProgressWnd()
{
	progWnd.DestroyWindow();
}

BOOL CMODVIEW32App::Check1stStartup()
{
	if(DMReg_ReadHKLMint("Configured",0)==0)
	{
		//CWelcome dlg;
		//dlg.DoModal();
		DoGamesConfig();
		return TRUE;
	}
	return FALSE;
}

void CMODVIEW32App::OnHelpCheckfornewversion() 
{
	DoDMVC();
}

void CMODVIEW32App::ReloadFile(CString fname)
{
	OnFileNew();
	OpenDocumentFile(fname);
}


