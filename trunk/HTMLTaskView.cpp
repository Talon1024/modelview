// HTMLTaskView.cpp : implementation file
//

#include "stdafx.h"
#include "modview32.h"
#include "modview32doc.h"
#include "MainFrm.h"
#include "HTMLTaskView.h"
#include "OptionsDlg.h"
#include "DM_Reg.h"
#include <afxadv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTMLTaskView

IMPLEMENT_DYNCREATE(CHTMLTaskView, CHtmlView)

CHTMLTaskView::CHTMLTaskView()
{
	//{{AFX_DATA_INIT(CHTMLTaskView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_HTMLFile_Open=DMReg_GetTEMPDir()+"open.html";
	m_HTMLFile_Archive=DMReg_GetTEMPDir()+"archive.html";
	m_HTMLFile_Error=DMReg_GetTEMPDir()+"error.html";
	m_HTMLFile_NoTextures=DMReg_GetTEMPDir()+"notextures.html";
}

CHTMLTaskView::~CHTMLTaskView()
{
}

void CHTMLTaskView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHTMLTaskView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHTMLTaskView, CHtmlView)
	//{{AFX_MSG_MAP(CHTMLTaskView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTMLTaskView diagnostics

#ifdef _DEBUG
void CHTMLTaskView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHTMLTaskView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHTMLTaskView message handlers

void CHTMLTaskView::OnInitialUpdate() 
{
}

void CHTMLTaskView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	TRACE("%s %i %s %s\n",lpszURL,nFlags,lpszTargetFrameName,lpszHeaders);
	
	CString url=lpszURL;

	if(url.Right(1)=="\\")
		url.TrimRight("\\");
	if(url.Right(1)=="/")
		url.TrimRight("/");
	if(url.Left(12)=="modelview://")
	{
		url=url.Mid(12);

		if(url.Left(4)=="Cmd:")
			((CMainFrame *)((CMODVIEW32App *)AfxGetApp()))->ParseCmd(url);
		else
		{
			url.Replace("%20"," ");
			AfxGetApp()->OpenDocumentFile(url);
		}
		*pbCancel=TRUE;
	}
	else
		CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CHTMLTaskView::GenerateHTML()
{
	CMainFrame *viewFrame=static_cast<CMainFrame*>(GetParentFrame());
	CMODVIEW32Doc *pDoc=viewFrame->GetDocument();

	CStdioFile ft;


	///////////////////////// Welcome page
	VERIFY(ft.Open(m_HTMLFile_Open,CFile::modeWrite | CFile::modeCreate));
	GenerateHTMLHeader(&ft);
	
	ft.WriteString("Welcome to Descent Manager MODELVIEW32!<br><br>");
	ft.WriteString("<b><u>Open document</u></b><br>");

	//Add recent file list files
	CRecentFileList mru(0,"Recent File List","File%d",10,100);
	mru.ReadList();
	for(int i=0;i<mru.GetSize();i++)
	{
		if(mru[i]!="")
			ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"modelview://"+mru[i]+"\">"+mru[i]+"</a><br>");
	}

	ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"modelview://Cmd:OpenDialog\">More...</a><br>");
	ft.WriteString("<br>");

	ft.WriteString("<b><u>Other options</u></b><br>");
	ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"modelview://Cmd:OptionsDialog\">Options & Configuration</a><br>");
#ifdef _WITHEDITOR
	ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"modelview://Cmd:ImportDialog\">Generate FreeSpace model from TrueSpace .COB file</a><br>");
#endif
	ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"modelview://Cmd:HelpControls\">Keyboard & Mouse Help</a><br>");

	GenerateHTMLFooter(&ft);
	ft.Close();


	///////////////////////// Archive page
	VERIFY(ft.Open(m_HTMLFile_Archive,CFile::modeWrite | CFile::modeCreate));
	GenerateHTMLHeader(&ft);
	ft.WriteString("You have opened an archive. Select a model on the left to view it.<br><br>");

	ft.WriteString("<b><u>Statistics:</u></b><br>");

	CString x;
	if(pDoc->m_ArchiveStats_NumOther!=-1)
	{
		x.Format("&nbsp;&nbsp;&nbsp;Number of files in this archive: <b>%i</b><br>",pDoc->m_ArchiveStats_NumOther+pDoc->m_ArchiveStats_NumTotal);
		ft.WriteString(x);
	}
	if(pDoc->m_ArchiveStats_NumTotal!=-1)
	{
		x.Format("&nbsp;&nbsp;&nbsp;Number of models in this archive: <b>%i</b><br>",pDoc->m_ArchiveStats_NumTotal);
		ft.WriteString(x);
	}
	if(pDoc->m_ArchiveStats_NumHires!=-1)
	{
		x.Format("&nbsp;&nbsp;&nbsp;Number of hi-resolution models: <b>%i</b><br>",pDoc->m_ArchiveStats_NumHires);
		ft.WriteString(x);
	}
	if(pDoc->m_ArchiveStats_NumLores!=-1)
	{
		x.Format("&nbsp;&nbsp;&nbsp;Number of low-resolution models: <b>%i</b><br>",pDoc->m_ArchiveStats_NumLores);
		ft.WriteString(x);
	}
	ft.WriteString("<br>");

	ft.WriteString("<b><u>Other options:</u></b><br>");
	ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"modelview://Cmd:ModelComparisonTable\">Generate Model Comparison Table with all models in this archive</a><br>");
	ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"open.html\">Open a different archive or model</a><br>");
	
	GenerateHTMLFooter(&ft);
	ft.Close();


	///////////////////////// Error page
	VERIFY(ft.Open(m_HTMLFile_Error,CFile::modeWrite | CFile::modeCreate));
	GenerateHTMLHeader(&ft);
	//ft.WriteString("<b><u>Error occured</u></b><br>");
	//Display error details
	{
		char path_main[MAX_FILENAMELEN];
		COptionsDlg dlg;
		dlg.GetMainPath((char *)&path_main);
		CString x1=path_main;
		x.Format("<img src=\""+x1+"ht_error.gif\" height=32 width=32 border=0>&nbsp;&nbsp;&nbsp;An Error Occured!<br><br>");
		ft.WriteString(x);
	}
	x.Format("Error #%i:<br>",pDoc->m_ErrorCode);
	ft.WriteString(x);
	ft.WriteString(pDoc->m_ErrorString+"<br>");
	ft.WriteString(pDoc->m_ErrorDetails+"<br>");
	ft.WriteString("<br>&nbsp;&nbsp;&nbsp;<a href=\"open.html\">Open a different model or archive</a><br>");
	GenerateHTMLFooter(&ft);
	ft.Close();


	///////////////////////// No textures found page
	VERIFY(ft.Open(m_HTMLFile_NoTextures,CFile::modeWrite | CFile::modeCreate));
	GenerateHTMLHeader(&ft);
	ft.WriteString("<b><u>No textures available</u></b><br>");
	//Display error details
	ft.WriteString("The model has been successfully loaded, however no textures needed to render this model were found. To see something switch to wireframe or surface rendering mode.<br><br>");
	ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"modelview://Cmd:ViewWireframe\">Switch to Wireframe mode</a><br>");
	ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"modelview://Cmd:ViewSurface\">Switch to Surface mode</a><br>");
	ft.WriteString("<br><br>If you are sure the model only uses standard textures, make sure you have configured the correct game directory so that the standard texture files can be located:<br><br>");
	ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"modelview://Cmd:OptionsDialog\">Options & Configuration</a><br>");
	ft.WriteString("&nbsp;&nbsp;&nbsp;<a href=\"open.html\">Open a different archive or model</a><br>");
	GenerateHTMLFooter(&ft);
	ft.Close();

}

void CHTMLTaskView::GenerateHTMLHeader(CStdioFile *ft)
{
	//Get main path
	char path_main[MAX_FILENAMELEN];
	COptionsDlg dlg;
	dlg.GetMainPath((char *)&path_main);
	strcat_s(path_main,"ht_logo.jpg");
	CString x=path_main;
	
	ft->WriteString("<html>");
	ft->WriteString("<head>");
	ft->WriteString("</head>");
	ft->WriteString("<body topmargin=0 leftmargin=0>");
	ft->WriteString("<table border=\"0\" width=\"100%\" bgcolor=\"black\" cellspacing=0 cellpadding=0>");
	ft->WriteString("<tr>");
	ft->WriteString("<td>");
	ft->WriteString("<img src=\""+x+"\" border=0>");
	ft->WriteString("</td>");
	ft->WriteString("</tr>");
	ft->WriteString("</table>");
	ft->WriteString("<table border=\"0\" width=\"100%\" bgcolor=\"white\" cellspacing=2 cellpadding=2>");
	ft->WriteString("<tr>");
	ft->WriteString("<td>");
	ft->WriteString("<font face=\"Tahoma,Arial\" size=\"2\"><br>");
}

void CHTMLTaskView::GenerateHTMLFooter(CStdioFile *ft)
{
	ft->WriteString("</font>");
	ft->WriteString("</td>");
	ft->WriteString("</tr>");
	ft->WriteString("</table>");
	ft->WriteString("</body>");
	ft->WriteString("</html>");
}

void CHTMLTaskView::ShowHTML(int type)
{
	switch(type)
	{
	case TOV_NODOCUMENT:
TRACE("NODOCUMENT--------------------------------------\n");
		GenerateHTML();
		Navigate2(_T(m_HTMLFile_Open),NULL,NULL);
		InvalidateRect(NULL);
		break;

	case TOV_ARCHIVELOADED:
TRACE("ARCHIVELOADED--------------------------------------\n");
		GenerateHTML();
		Navigate2(_T(m_HTMLFile_Archive));
		InvalidateRect(NULL);
		break;

	case TOV_MODELVIEW:
		break;

	case TOV_MODELVIEWBUTNOTEXTURES:
TRACE("MODELVIEWBUTNOTEXTURES--------------------------------------\n");
		GenerateHTML();
		Navigate2(_T(m_HTMLFile_NoTextures));
		InvalidateRect(NULL);
		break;
	
	case TOV_MODELVIEWBUTFAILED:
TRACE("MODELVIEWBUTFAILED--------------------------------------\n");
		GenerateHTML();
		Navigate2(_T(m_HTMLFile_Error));
		InvalidateRect(NULL);
		break;
	
	default:
		ASSERT(FALSE);
	}
}
