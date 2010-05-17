// MODVIEW32Doc.cpp : implementation of the CMODVIEW32Doc class
//

#include "stdafx.h"
#include "MODVIEW32.h"
#include "MainFrm.h"
#include "MODVIEW32Doc.h"
#include "MODVIEW32View.h"
#include "OptionsDlg.h"
#include "math.h" //for floor()
#include "matrix.h"
#include "stif32c.h"
#include "CompilerErrorDlg.h"
#include "TGA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

#define NEW_FILE  0
#define ANI_FILE  1
#define PCX_FILE  2
#define SKIP_FILE 3
#define OGF_FILE  4
#define OAF_FILE  5

#define VP_FS_TEXTURESET_NONE 0
#define VP_FS_TEXTURESET_FS1  1
#define VP_FS_TEXTURESET_FS2  2



/////////////////////////////////////////////////////////////////////////////
// CMODVIEW32Doc

IMPLEMENT_DYNCREATE(CMODVIEW32Doc, CDocument)

BEGIN_MESSAGE_MAP(CMODVIEW32Doc, CDocument)
	//{{AFX_MSG_MAP(CMODVIEW32Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMODVIEW32Doc construction/destruction


////////////////////////////////////////////////////////////////////////////////////////
// Initialization stuff
////////////////////////////////////////////////////////////////////////////////////////

CMODVIEW32Doc::CMODVIEW32Doc()
{
	m_TexturesNum=-1;
	for(int i=0;i<MAX_TEXTURES;i++)
	{
#ifdef _WITHTXVIEW
		m_Textures[i].Flags=0;
		m_Textures[i].Bitmap=NULL;
#endif
		m_Textures[i].FileType=TEXTUREINFOFILETYPE_UNKNOWN;
	}

	m_ArchiveStats_NumTotal=0;
	m_ArchiveStats_NumLores=0;
	m_ArchiveStats_NumHires=0;
	m_ArchiveStats_NumOther=0;

	m_NumVertices=0;
	m_NumPolygons=0;
	m_NumTextures=0;
	m_NumSubmodels=0;

	//m_D2_Marker_ModelNum=-1;
	//m_D2_TextureSet=D2_TEXTURESET_GROUPA;
	//m_D3_MainHOG_MainTextureSet=0;
	//m_D3_MainHOG_Loaded=FALSE;
	m_FS_MainVP_MainTextureSet=VP_FS_TEXTURESET_NONE;
	m_FS_MainVP_Loaded=FALSE;
	m_Game=GAME_NONE;

	m_InfoMode=FALSE;
	
	m_DoResetGeometry=FALSE;
	m_AutoZoom=-50.0f;
	bSize=TRUE;

	m_ModelLoaded=FALSE;

	//for(int i=0;i<MAX_D2_POLYGON_MODELS;i++)
	//{
	//	m_D2_PolygonModels[i].Type=D2_POLYMODEL_TYPE_UNKNOWN;
	//	m_D2_PolygonModels[i].Joints.Available=FALSE;
	//}

	//for(int i=0;i<MAX_D3_TEXTURE;i++)
	//{
	//	m_D3_Display.TextureFlag[i]=FALSE;
	//	m_D3_OffTexture[i]=FALSE;
	//}
	//m_D3_OffTXTR=FALSE;

	//for(int i=0;i<MAX_D3_SOBJ;i++)
	//	m_D3_HiLiteSobj[i]=FALSE;
	//m_D3_HiLiteCurSobj=FALSE;

	for(int i=0;i<MAX_FS_SOBJ;i++)
	{
		m_FS_SOBJ[i].bsp_data=NULL;
		m_FS_SOBJ[i].bsp_data_size=0;
	}

	//Get Main Path
//	char path_main[MAX_FILENAMELEN];
	COptionsDlg dlg;

	//Load Descent 2 Palette data
	//CFile fp;
	//dlg.GetMainPath((char *)&path_main);
	//strcat_s(path_main,"D2Pal.dat");
	//if(!fp.Open(path_main,CFile::typeBinary | CFile::modeRead))
	//{
	//	AfxMessageBox("Missing file D2Pal.dat.\n\nPlease reinstall the tool.");
	//	AfxAbort();
	//}
	//fp.Read(&m_D2_Palettes,4608);
	//fp.Close();

	//Load Descent 2 PolyModel descriptions
	//CStdioFile ft;
	//dlg.GetMainPath((char *)&path_main);
	//strcat_s(path_main,"D2Poly.dat");
	//if(!ft.Open(path_main,CFile::typeText | CFile::modeRead))
	//{
	//	AfxMessageBox("Missing file D2Poly.dat.\n\nPlease reinstall the tool.");
	//	AfxAbort();
	//}
	//for(int i=0; i<185; i++)
	//	ft.ReadString(m_D2_PM_Desc[i]);
	//ft.Close();

	//m_D2_MainHAMLoaded=FALSE;

	//Reset FS data
	m_Guns.Num=0;

	memset(&test1,123,256);
	memset(&test2,123,256);
}

CMODVIEW32Doc::~CMODVIEW32Doc()
{
	ClearDocument(FALSE);

	OutputDebugString("Testing memory...\n");
	for(int i=0;i<256;i++)
	{
		if(test1[i]!=123)
			OutputDebugString("Error in test1!\n");
		if(test2[i]!=123)
			OutputDebugString("Error in test2!\n");
	}
	if(test3!="qwertzuiop")
		OutputDebugString("Error in test3!\n");
	if(test4!="qwertzuiop")
		OutputDebugString("Error in test4!\n");
}

BOOL CMODVIEW32Doc::OnNewDocument()
{
	if(!CDocument::OnNewDocument())
		return FALSE;

	//m_D3_CurrHOG_OofNum=0;
	m_FS_CurrVP_PofNum=0;
	m_ModelLoaded=FALSE;
	m_Textures_Available=FALSE;
	//m_D3_CurrHOG_Header.dirnumber=0;
	m_FS_CurrVP_Header.dirnumber=0;
	ClearDocument();

	// (SDI documents will reuse this document)

	return TRUE;
}

void CMODVIEW32Doc::ClearDocument(BOOL RemoveGameBar/*=TRUE*/)
{
	m_ModelLoaded=FALSE;
	m_UntitledModel=FALSE;
	m_ErrorString="";
	m_ErrorDetails="";
	m_ErrorCode=ERROR_GEN_NOERROR;

	//Common stuff
	m_Guns.Num=0;
	m_Blocks_Num=0;
	m_TexturesNum=-1;
	for(int j=0;j<MAX_TEXTURES;j++)
	{
		m_Textures[j].FileType=TEXTUREINFOFILETYPE_UNKNOWN;
#ifdef _WITHTXVIEW
		if((m_Textures[j].Valid) && (m_Textures[j].Bitmap!=NULL))
		{
TRACE("Deleting texture %i from memory\n",j);
			delete(m_Textures[j].Bitmap);
			m_Textures[j].Bitmap=NULL;
		}
#endif
	}
	if(RemoveGameBar)
	{
		POSITION pos=GetFirstViewPosition();	
		CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
		CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
		/*CMainFrame *viewFrame=(CMainFrame *)((CMODVIEW32App *)AfxGetApp())->m_pMainWnd;
		if(viewFrame!=NULL)*/
		{
			viewFrame->GameBarNone();
			viewFrame->UpdateTextureBar();
			viewFrame->UpdateTextureBar();
			//viewFrame->UpdateMainDisplay();
		}
	}

	//D2 clear
	//for(int j=0;j<MAX_D2_SOBJ;j++)
	//{
	//	m_D2_Model.Soffset[j].x=0;
	//	m_D2_Model.Soffset[j].y=0;
	//	m_D2_Model.Soffset[j].z=0;
	//}
	//for(int j=0;j<MAX_D2_VERTICES;j++)
	//{
	//	m_D2_Model.Vpoint[j].x=0;
	//	m_D2_Model.Vpoint[j].y=0;
	//	m_D2_Model.Vpoint[j].z=0;
	//}

	//D3 clear
	//m_D3_CurSpcl=0;
	//m_D3_CurXpnt=0;
	//m_D3_CurSobj=0;
	//m_D3_CurWbat=0;
	//m_D3_CurTexture=0;
	//m_D3_Display.TotalKey=0;
	//m_D3_Display.PartsRotate=FALSE;
	//m_D3_Model.Vcount=0;
	//m_D3_Model.Pcount=0;
	//m_D3_Model.Scount=0;
	//m_D3_Model.ATCHnum=0;
	//m_D3_Model.NATHnum=0;
	//m_D3_Model.GRNDnum=0;
	//m_D3_Model.GPNTnum=0;
	//m_D3_Model.SPCLnum=0;
	//m_D3_Model.WBATnum=0;
	//m_D3_Model.PINF[0]=0;
	//for(int i=0;i<MAX_D3_SOBJ;i++)
	//{
	//	m_D3_HiLiteSobj[i]=FALSE;
	//	m_D3_Display.Facing[i]=FALSE;
	//	m_D3_Display.Rotate[i]=FALSE;
	//	m_D3_Display.Glow[i]=FALSE;
	//	m_D3_Display.Fov[i]=FALSE;
	//	m_D3_Rstep[i]=0;
	//	m_D3_Model.Name[i][0]=0;
	//	m_D3_Model.Prop[i][0]=0;
	//	m_D3_Display.pr_key[i]=FALSE;
	//	m_D3_Display.pr_active[i]=0;
	//	m_D3_Model.pindex[i]=0;
	//	m_D3_Model.pknum[i]=0;
	//	m_D3_Model.rindex[i]=0;
	//	m_D3_Model.rknum[i]=0;
	//	m_D3_Display.SpolyNum[i]=0;
	//	m_D3_Display.SvertNum[i]=0;
	//}
	//m_D3_HiLiteSobj[m_D3_CurSobj]=m_D3_HiLiteCurSobj;
	//for(int i=0;i<MAX_D3_TEXTURE;i++)
	//	m_D3_OffTexture[i]=FALSE;
	//m_D3_OffTexture[m_D3_CurTexture]=m_D3_OffTXTR;
	//for(int i=0;i<MAX_D3_ANI;i++)
	//{
	//	m_D3_Model.pani[i].x=0;
	//	m_D3_Model.pani[i].y=0;
	//	m_D3_Model.pani[i].z=0;
	//	m_D3_Model.pkey[i]=0;
	//	m_D3_Model.rani[i].x=0;
	//	m_D3_Model.rani[i].y=0;
	//	m_D3_Model.rani[i].z=0;
	//	m_D3_Model.rkey[i]=0;
	//	m_D3_Model.rangle[i]=0;
	//	m_D3_Display.pr_key[i]=FALSE;
	//}
	//for(int i=0; i<MAX_D3_POLY;i++)
	//	m_D3_Model.Poly[i].Color=0;

	//FreeSpace stuff
	m_FS_NumSubsystems=0;
	m_FS_NumSOBJ=0;
	m_FS_Model.Vcount=0;
	m_FS_Model.Pcount=0;
	m_FS_Model.Ncount=0;
	m_FS_Model.BBcount=0;
	m_FS_Model.shields.Fcount=0;
	m_FS_Model.shields.Vcount=0;
	m_FS_Model.num_thrusters=0;
	m_FS_NumDocks=0;
	m_FS_ModelInfo.ACenPoint.x=0.0;
	m_FS_ModelInfo.ACenPoint.y=0.0;
	m_FS_ModelInfo.ACenPoint.z=0.0;
	m_FS_ModelInfo.num_eye_points=0;
	m_FS_ModelInfo.num_cross_sections=0;
	m_FS_ModelInfo.num_lights=0;
	m_FS_ModelInfo.eye_point_normal.x=0.0;
	m_FS_ModelInfo.eye_point_normal.y=0.0;
	m_FS_ModelInfo.eye_point_normal.z=0.0;
	m_FS_ModelInfo.eye_point_sobj_number=0;
	m_FS_ModelInfo.eye_point_sobj_offset.x=0.0;
	m_FS_ModelInfo.eye_point_sobj_offset.y=0.0;
	m_FS_ModelInfo.eye_point_sobj_offset.z=0.0;
	m_FS_RealGuns[0].Num=0;
	m_FS_RealGuns[1].Num=0;
	m_FS_Turrets[0].Num=0;
	m_FS_Turrets[1].Num=0;
	m_FS_NumDocks=0;
	for(int i=0;i<MAX_FS_SOBJ;i++)
	{
		if(m_FS_SOBJ[i].bsp_data!=NULL)
		{
			delete(m_FS_SOBJ[i].bsp_data);
			m_FS_SOBJ[i].bsp_data=NULL;
			m_FS_SOBJ[i].bsp_data_size=0;
		}
	}
	
	//EditorFS stuff
	m_EditorFS_ShieldsBackup=m_FS_Model.shields;
	m_EditorFS_ChunkTXTR.ClearChunk();
	m_EditorFS_ChunkOHDR.ClearChunk();
	m_EditorFS_ChunkPINF.ClearChunk();
	m_EditorFS_ChunkPATH.ClearChunk();
	m_EditorFS_ChunkSHLD.ClearChunk();
	m_EditorFS_Chunk_EYE.ClearChunk();
	m_EditorFS_ChunkGPNT.ClearChunk();
	m_EditorFS_ChunkMPNT.ClearChunk();
	m_EditorFS_ChunkTGUN.ClearChunk();
	m_EditorFS_ChunkTMIS.ClearChunk();
	m_EditorFS_ChunkSPCL.ClearChunk();
	m_EditorFS_ChunkINSG.ClearChunk();
	m_EditorFS_ChunkACEN.ClearChunk();
	m_EditorFS_ChunkDOCK.ClearChunk();
	m_EditorFS_ChunkFUEL.ClearChunk();
	for(int i=0;i<MAX_FS_SOBJ;i++)
		m_EditorFS_ChunkSOBJ[i].ClearChunk();
}


/////////////////////////////////////////////////////////////////////////////
// CMODVIEW32Doc serialization

void CMODVIEW32Doc::Serialize(CArchive& ar)
{
	//Get other views
	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());

	m_view=viewFrame->GetCurrentView();
	CMODVIEW32App *app=(CMODVIEW32App *)AfxGetApp();
	app->showsplashlonger=FALSE;
	m_CurrentFile_FileName="";

	ASSERT(!ar.IsStoring()); //We do not support saving
	SetModifiedFlag(FALSE); //Just to be sure
	if(viewFrame->m_EditorMode)
		viewFrame->OnViewer2();

	CString an=ar.GetFile()->GetFileName();
	m_CurFile=ar.GetFile()->GetFilePath();
	an.MakeUpper();

	ExplorerClear();
//	m_D2_MainHAMLoaded=FALSE;

	m_TexturesNum=-1;

	BOOL supportedfiletype=FALSE;
	//if(an.Find(".POL")>=0)
	//{
	//	//Descent 2 code
	//	supportedfiletype=TRUE;
	//	viewFrame->ExplorerHide();
	//	m_ErrorCode=D2_ReadPOL(ar.GetFile(),1);
	//	if(m_ErrorCode==ERROR_GEN_NOERROR)
	//	{
	//		viewFrame->GameBarD2();
	//		D2_LoadPIGData(); //Load main .PIG file
	//	}
	//}

	//if(an.Find(".OOF")>=0)
	//{
	//	//Descent 3 code
	//	supportedfiletype=TRUE;
	//	viewFrame->ExplorerHide();
	//	m_D3_CurrHOG_OofNum=0;
	//	m_ErrorCode=D3_ReadOOF(ar.GetFile(),1);
	//	viewFrame->GameBarD3();
	//	D3_LoadOGFData();
	//}

	//if(an.Find(".V3C")>=0)
	//{
	//	//Red Faction code
	//	supportedfiletype=TRUE;
	//	viewFrame->ExplorerHide();
	//	m_ErrorCode=RF_ReadV3C(ar.GetFile());
	//	viewFrame->GameBarRF();
	//}


	if(an.Find(".POF")>=0)
	{
		//FreeSpace code
		supportedfiletype=TRUE;
		viewFrame->ExplorerHide();
		m_FS_CurrVP_PofNum=0;
		m_FS_CurrVP_Loaded=FALSE;
		m_ErrorCode=FS_ReadPOF(ar.GetFile(),1);
		if(m_ErrorCode==ERROR_GEN_NOERROR)
		{
			viewFrame->GameBarFS();
			FS_LoadPCXData2();	//Load textures from main .VP file
		}
	}

	if(an.Find(".VP")>=0 && an.Find(".VPP")<0)
	{
		//FreeSpace code
		supportedfiletype=TRUE;
		viewFrame->ExplorerShow();
		ClearDocument();
		m_Game=GAME_FS;
		m_ErrorCode=FS_InitVP(ar.GetFile(),&m_FS_CurrVP_Header,(VP_INFO *)&m_FS_CurrVP_Dir,TRUE);
		if(m_ErrorCode==ERROR_GEN_NOERROR)
		{
			m_FS_CurrVP_Loaded=TRUE; //if no error!!
			m_FS_CurrVP_Filename=ar.GetFile()->GetFilePath();
			FS_ReadFSVP(2);
			m_ErrorString=ERROR_GEN_NOMODELSELECTED_TEXT;
			m_ErrorCode=ERROR_GEN_NOMODELSELECTED;
		}
	}

	//if(an.Find(".VPP")>=0)
	//{
	//	//Red Faction code
	//	supportedfiletype=TRUE;
	//	viewFrame->ExplorerShow();
	//	ClearDocument();
	//	m_Game=GAME_RF;
	//	m_RF_CurrVPP_Filename=ar.GetFile()->GetFilePath();
	//	/*m_ErrorCode=*/RF_InitVPP(ar.GetFile(),&m_RF_CurrVPP_Header,&m_RF_CurrVPP_Load,TRUE);
	//	m_ErrorString=ERROR_GEN_NOMODELSELECTED_TEXT;
	//	m_ErrorCode=ERROR_GEN_NOMODELSELECTED;
	//}

	//if(an.Find(".HOG")>=0)
	//{
	//	supportedfiletype=TRUE;
	//	viewFrame->ExplorerHide();
	//	ClearDocument();

	//	//HOG or HOG2?
	//	CFile *f=ar.GetFile();
	//	int sig=0;
	//	f->Seek(0,CFile::begin);
	//	f->Read(&sig,3);
	//	f->Seek(0,CFile::begin);
	//	switch(sig)
	//	{
	//	case ID3_HOG: //Descent 2 code (unsupported)
	//		m_ErrorCode=ERROR_D2_HOGUNSUPPORTED;
	//		m_ErrorString=ERROR_D2_HOGUNSUPPORTED_TEXT;
	//		break;

	//	case ID3_HOG2: //Descent 3 code
	//		viewFrame->ExplorerShow();
	//		m_Game=GAME_D3;
	//		m_ErrorCode=D3_InitHOG(ar.GetFile(),&m_D3_CurrHOG_Header,(HOG2_LOAD *)&m_D3_CurrHOG_Dir,TRUE);
	//		if(m_ErrorCode==ERROR_GEN_NOERROR)
	//		{
	//			m_D3_CurrHOG_Filename=ar.GetFile()->GetFilePath();
	//			D3_ReadD3HOG();
	//			m_ErrorString=ERROR_GEN_NOMODELSELECTED_TEXT;
	//			m_ErrorCode=ERROR_GEN_NOMODELSELECTED;
	//		}
	//		break;

	//	default:
	//		m_ErrorString=ERROR_GEN_INVALIDHOGFILE_TEXT;
	//		m_ErrorCode=ERROR_GEN_INVALIDHOGFILE;
	//	}
	//}

	//if((an.Find(".HAM")>=0) || (an.Find(".HXM")>=0))
	//{
	//	supportedfiletype=TRUE;
	//	if(an.Find("HOARD.HAM")>=0) //HOARD.HAM from D2 Vertigo contains no model data
	//	{
	//		m_ErrorCode=ERROR_D2_HOARDHAMUNSUPPORTED;
	//		m_ErrorString=ERROR_D2_HOARDHAMUNSUPPORTED_TEXT;
	//	}
	//	else //Supported are DESCENT2.HAM, V-HAM and HXM formats
	//	{
	//		viewFrame->ExplorerShow();
	//		ClearDocument();		
	//		m_Game=GAME_D2;
	//		D2_ReadD2HAM();
	//		D2_InitHAM(ar.GetFile(),TRUE);
	//		strcpy_s(m_D2_CurrHAXM_Filename,ar.GetFile()->GetFilePath());
	//		m_ErrorString=ERROR_GEN_NOMODELSELECTED_TEXT;
	//		m_ErrorCode=ERROR_GEN_NOMODELSELECTED;
	//	}
	//}

	if(an.Find(".OPENPOF")>=0) //used by VPVIEW32
	{
		supportedfiletype=TRUE;
		char *data=new char[32768];
		CFile *poff=ar.GetFile();
		poff->SeekToBegin();
		poff->Read(data, (UINT)poff->GetLength());

		char *poffile,*pofoffset,*poftitle,*pofafter;

		//Get 1st parameter
		poffile=data;

		//Get 2nd parameter
		pofoffset=data;
		while(*pofoffset!=',' && *pofoffset!='\0')
			pofoffset++;
		if(*pofoffset!=',')
			return; //error
		*pofoffset=0;
		pofoffset++;
		
		//Get 3rd parameter
		poftitle=pofoffset;
		while(*poftitle!=',' && *poftitle!='\0')
			poftitle++;
		if(*poftitle!=',')
			return; //error
		*poftitle=0;
		poftitle++;

		//Cut rest
		pofafter=poftitle;
		while(*pofafter!=',' && *pofafter!='\0')
			pofafter++;
		*pofafter=0;
		pofafter++;

		//Open pof file in .VP
		int pofoffsetint=atoi(pofoffset);
		int pofoffsetsize=atoi(pofafter);
		CFile vp;
		vp.Open(poffile,CFile::modeRead);
		vp.Seek(pofoffsetint,CFile::begin);

		//Open .VP
		viewFrame->ExplorerShow();
		ClearDocument();
		m_Game=GAME_FS;
		FS_InitVP(&vp,&m_FS_CurrVP_Header,(VP_INFO *)&m_FS_CurrVP_Dir,TRUE);
		vp.Close();
		m_FS_CurrVP_Filename=poffile;
		viewFrame->ExplorerOpenSubFile(poftitle);
		
		delete[] data;
	}
	if(!supportedfiletype)
	{
		m_ErrorCode=ERROR_GEN_UNSUPPORTEDFILETYPE;
		m_ErrorString=ERROR_GEN_UNSUPPORTEDFILETYPE_TEXT;
	}

	SetStatusBarText("Ready");
	m_view->ChangeSize();
	viewFrame->UpdateTextureBar();
	viewFrame->UpdateMainDisplay();
	UpdateAllViews(NULL);
}

void CMODVIEW32Doc::SetStatusBarText(char *txt)
{
	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
	viewFrame->SetStatusBarText(txt);
}


/////////////////////////////////////////////////////////////////////////////
// CMODVIEW32Doc diagnostics

#ifdef _DEBUG
void CMODVIEW32Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMODVIEW32Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMODVIEW32Doc commands


////////////////////////////////////////////////////////////////////////////////////////
// Explorer pane stuff
////////////////////////////////////////////////////////////////////////////////////////

void CMODVIEW32Doc::ExplorerClear()
{
	//Get other views
	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());

	viewFrame->ExplorerClear();
}

void CMODVIEW32Doc::ExplorerAddRoot(char *text)
{
	//Get other views
	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());

	viewFrame->ExplorerAddRoot(text);
}

void CMODVIEW32Doc::ExplorerAddChild(char *filename, int ModelNum/*=-1*/)
{
	//Get other views
	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());

	//Get type nr
	int typenr=-1;
	char type[256];
	CString gamecaption;
	gamecaption="FreeSpace";
	GetPrivateProfileString(gamecaption,filename,"$",(char *)&type,255,viewFrame->m_TypeRootsFile);
	if(strcmp(type,"$")!=0)
		typenr=GetPrivateProfileInt(gamecaption,type,-1,viewFrame->m_TypeRootsFile);


	viewFrame->ExplorerAddChild(filename,typenr);
}

void CMODVIEW32Doc::ExplorerAddChild(CString filename, int ModelNum/*=-1*/)
{
	char *x=new char[filename.GetLength()+1];
	strcpy(x,filename);
	ExplorerAddChild(x,ModelNum);
	delete[] x;
}

void CMODVIEW32Doc::ExplorerGoTop()
{
	//Get other views
	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());

	viewFrame->ExplorerGoTop();
}


////////////////////////////////////////////////////////////////////////////////////////
// FreeSpace stuff
////////////////////////////////////////////////////////////////////////////////////////

ERRORCODE CMODVIEW32Doc::FS_LoadPCXData(unsigned int ActivePM,BOOL bAniLoad,BOOL bFastload)
{
	unsigned int xsize,ysize;
	unsigned int xreal,yreal;         
	unsigned char XScale,YScale;
	GLint MaxTextureSize[4];
	unsigned int MaxSize;

	ASSERT(m_Game==GAME_FS);

	//Load Main FS texture set if not yet done
	FS_ReadFSVP(m_FS_CurrVP_FreeSpaceVersion);

	//Init
	glGetIntegerv(GL_MAX_TEXTURE_SIZE,MaxTextureSize);
	MaxSize=MaxTextureSize[0];
	m_TexturesNum=-1;
	
	//Delete textures
	for(int i=0;i<MAX_FS_TEXTURE;i++)
	{
		if(m_FS_TextureFlag[i])
		{
			glDeleteLists(m_FS_ModelTexture[i],1);
			m_FS_TextureFlag[i]=FALSE;
		}
		m_FS_BitmapData.pic[i].valid=0;
	}

	if(!m_ModelLoaded)
		return ERROR_GEN_NOERROR;

	if(m_FS_BitmapData.count>0)
	{
		for(unsigned int i=m_FS_PofDataL[ActivePM];(i<(m_FS_PofDataH[ActivePM]+1)) && (i < MAX_FS_TEXTURE);i++)
		{
			BOOL unknownfiletype=TRUE;
			m_FS_BitmapData.pic[i].valid=1;

			//Initialize m_Textures struct
			m_TexturesNum++;
			if(m_TexturesNum>MAX_TEXTURES)
			{
				m_ErrorString=ERROR_FS_TOOMANYTEXTURES_TEXT;
				m_ErrorDetails.Format("Is: %i, Should be <=%i",m_TexturesNum,MAX_FS_TEXTURE);
				return ERROR_FS_TOOMANYTEXTURES;
			}
			m_Textures[m_TexturesNum].Filename=m_FS_BitmapData.pic[i].name;
			m_Textures[m_TexturesNum].Width=-1;
			m_Textures[m_TexturesNum].Height=-1;
			m_Textures[m_TexturesNum].Used=0;
			m_Textures[m_TexturesNum].InFilename="";
			m_Textures[m_TexturesNum].InOffset=-1;
			m_Textures[m_TexturesNum].InSize=-1;
			m_Textures[m_TexturesNum].Error="???";
			m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_UNKNOWN;

			//Count polygons using the texture
			for(unsigned int j=0;j<m_FS_Model.Pcount;j++)
			{	// type 3 polygons
				if((m_FS_Model.Poly[j].Ptype==3) && (m_FS_Model.Poly[j].Colors==i)) //&(pDoc->m_FS_SOBJ[pDoc->m_FS_Model.Poly[j].Sobj].detail==(long)m_Detaillevel)
					m_Textures[m_TexturesNum].Used++;
			}

			// check first for a pcx file
			char Filename[50];
			m_FS_BitmapData.pic[i].name;
			strcpy_s(Filename,m_FS_BitmapData.pic[i].name);
			strcat_s(Filename,".pcx");
			TRACE("Trying to load %s",m_FS_BitmapData.pic[i].name);
			
			if(!m_InfoMode)
			{
				char str[256];
				sprintf_s(str,"Loading %s...",Filename);
				SetStatusBarText(str);
			}

			//Get local directory
			CString localdirectory="";
			{
				char x[FNAMELEN];
				strcpy_s(x,m_CurFile);
				removefilename((char *)&x);
				localdirectory=x;
			}

			//PCX Search process #1: Current directory
			{
				CString localfilename=localdirectory;
				localfilename+=Filename;
				CFile fcheck;
				if(fcheck.Open(localfilename,CFile::modeRead))
				{
					unknownfiletype=FALSE;
					TRACE(".pcx...Found in current directory!\n");
					m_Textures[m_TexturesNum].Location="Current directory";
					m_Textures[m_TexturesNum].Type="PCX Texture";
					m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_PCX;
					m_Textures[m_TexturesNum].InFilename=localfilename;
					m_Textures[m_TexturesNum].InOffset=0;
					m_Textures[m_TexturesNum].InSize=fcheck.GetLength();
					m_Textures[m_TexturesNum].Valid=TRUE;
				}
			}

			//PCX Search process #2: Current .VP file
			if(unknownfiletype && m_FS_CurrVP_Loaded)
			{
				unsigned int j=0;
				while((j<m_FS_CurrVP_Header.dirnumber)&(_strcmpi(Filename,m_FS_CurrVP_Dir[j].filename) !=0))
					j++;

				if(j<m_FS_CurrVP_Header.dirnumber)
				{
					CFile fcheck;
					if(!fcheck.Open(m_FS_CurrVP_Filename,CFile::modeRead))
					{
						m_ErrorString=ERROR_FS_CANTOPENCURVP_TEXT;
						return ERROR_FS_CANTOPENCURVP;
					}
					//File_end=m_FS_CurrVP_Dir[j].offset + m_FS_CurrVP_Dir[j].size;
					//File_start=m_FS_CurrVP_Dir[j].offset;
					unknownfiletype=FALSE;
					TRACE(".pcx...Found in current VP!\n");
					m_Textures[m_TexturesNum].Location="Current VP";
					m_Textures[m_TexturesNum].Type="PCX Texture";
					m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_PCX;
					m_Textures[m_TexturesNum].InFilename=m_FS_CurrVP_Filename;
					m_Textures[m_TexturesNum].InOffset=m_FS_CurrVP_Dir[j].offset;
					m_Textures[m_TexturesNum].InSize=m_FS_CurrVP_Dir[j].size;
					m_Textures[m_TexturesNum].Valid=TRUE;
				}
			}

			//PCX Search process #3: /data/maps
			if(unknownfiletype)
			{
				CString mapsdirectory;
				COptionsDlg dlg;
				if(m_FS_CurrVP_FreeSpaceVersion==1)
					mapsdirectory=dlg.GetF1Path();
				else
					mapsdirectory=dlg.GetF2Path();
				CString mapsfilename=mapsdirectory;
				if(mapsfilename.Right(1)!="\\")
					mapsfilename+="\\";
				mapsfilename+="data\\maps\\";
				mapsfilename+=Filename;
				
				CFile fcheck;
				if(fcheck.Open(mapsfilename,CFile::modeRead))
				{
					unknownfiletype=FALSE;
					TRACE(".pcx...Found in FreeSpace /data/maps directory!\n");
					m_Textures[m_TexturesNum].Location="/data/maps directory";
					m_Textures[m_TexturesNum].Type="PCX Texture";
					m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_PCX;
					m_Textures[m_TexturesNum].InFilename=mapsfilename;
					m_Textures[m_TexturesNum].InOffset=0;
					m_Textures[m_TexturesNum].InSize=fcheck.GetLength();
					m_Textures[m_TexturesNum].Valid=TRUE;
				}
			}

			//PCX Search process #4: Main .VP file
			if(unknownfiletype && m_FS_MainVP_Loaded)
			{
				unsigned int j=0;
				while((j<m_FS_MainVP_Header.dirnumber)&(_strcmpi(Filename,m_FS_MainVP_Dir[j].filename) !=0))
					j++;

				if(j<m_FS_MainVP_Header.dirnumber)
				{
					CFile fcheck;
					if(!fcheck.Open(m_FS_MainVP_Filename,CFile::modeRead))
						return ERROR_FS_CANTOPENFSVP;
					unknownfiletype=FALSE;
					TRACE(".pcx...Found in main VP!\n");
					m_Textures[m_TexturesNum].Location="Main FreeSpace VP";
					m_Textures[m_TexturesNum].Type="PCX Texture";
					m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_PCX;
					m_Textures[m_TexturesNum].InFilename=m_FS_MainVP_Filename;
					m_Textures[m_TexturesNum].InOffset=m_FS_MainVP_Dir[j].offset;
					m_Textures[m_TexturesNum].InSize=m_FS_MainVP_Dir[j].size;
					m_Textures[m_TexturesNum].Valid=TRUE;
				} 
			}

			//ANI Search process #1: Current directory
			if(unknownfiletype)
			{		   // no pcx file... check for ani file
				strcpy_s(Filename,m_FS_BitmapData.pic[i].name);
				strcat_s(Filename,".ani");

				CFile fcheck;
				if(fcheck.Open(Filename,CFile::modeRead))
				{
					unknownfiletype=FALSE;
					TRACE(".ani...Found in current directory!\n");
					m_Textures[m_TexturesNum].Location="Current Directory";
					m_Textures[m_TexturesNum].Type="ANI Animation";
					m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_ANI;
					m_Textures[m_TexturesNum].InFilename=Filename;
					m_Textures[m_TexturesNum].InOffset=0;
					m_Textures[m_TexturesNum].InSize=fcheck.GetLength();
					m_Textures[m_TexturesNum].Valid=TRUE;
				} 
			}
			
			//ANI Search process #2: Current .VP
			if(unknownfiletype && m_FS_CurrVP_Loaded)
			{
				unsigned int j=0;
				while((j<m_FS_CurrVP_Header.dirnumber)&(_strcmpi(Filename,m_FS_CurrVP_Dir[j].filename) !=0))
					j++;
				if(j<m_FS_CurrVP_Header.dirnumber)
				{
					CFile fcheck;
					if(!fcheck.Open(m_FS_CurrVP_Filename,CFile::modeRead))
						return ERROR_FS_CANTOPENCURVP;
					unknownfiletype=FALSE;
					TRACE(".ani...Found in current VP!\n");
					m_Textures[m_TexturesNum].Location="Current VP";
					m_Textures[m_TexturesNum].Type="ANI Animation";
					m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_ANI;
					m_Textures[m_TexturesNum].InFilename=m_FS_CurrVP_Filename;
					m_Textures[m_TexturesNum].InOffset=m_FS_CurrVP_Dir[j].offset;
					m_Textures[m_TexturesNum].InSize=m_FS_CurrVP_Dir[j].size;
					m_Textures[m_TexturesNum].Valid=TRUE;
				}
			}

			//ANI Search process #3: /data/maps
			if(unknownfiletype)
			{
				CString mapsdirectory;
				COptionsDlg dlg;
				if(m_FS_CurrVP_FreeSpaceVersion==1)
					mapsdirectory=dlg.GetF1Path();
				else
					mapsdirectory=dlg.GetF2Path();
				CString mapsfilename=mapsdirectory;
				if(mapsfilename.Right(1)!="\\")
					mapsfilename+="\\";
				mapsfilename+="data\\maps\\";
				mapsfilename+=Filename;

				CFile fcheck;
				if(fcheck.Open(mapsfilename,CFile::modeRead))
				{
					unknownfiletype=FALSE;
					TRACE(".ani...Found in FreeSpace /data/maps directory!\n");
					m_Textures[m_TexturesNum].Location="/data/maps directory";
					m_Textures[m_TexturesNum].Type="ANI Animation";
					m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_ANI;
					m_Textures[m_TexturesNum].InFilename=mapsfilename;
					m_Textures[m_TexturesNum].InOffset=0;
					m_Textures[m_TexturesNum].InSize=fcheck.GetLength();
					m_Textures[m_TexturesNum].Valid=TRUE;
				}
			}

			//ANI Search process #4: Main .VP file
			if(unknownfiletype && m_FS_MainVP_Loaded)
			{
				unsigned int j=0;
				while((j<m_FS_MainVP_Header.dirnumber)&(_strcmpi(Filename,m_FS_MainVP_Dir[j].filename) !=0))
					j++;
				if(j<m_FS_MainVP_Header.dirnumber)
				{
					CFile fcheck;
					if(!fcheck.Open(m_FS_MainVP_Filename,CFile::modeRead))
						return ERROR_FS_CANTOPENFSVP;
					unknownfiletype=FALSE;
					TRACE(".ani...Found in main VP!\n");
					m_Textures[m_TexturesNum].Location="Main FreeSpace VP";
					m_Textures[m_TexturesNum].Type="ANI Animation";
					m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_ANI;
					m_Textures[m_TexturesNum].InFilename=m_FS_MainVP_Filename;
					m_Textures[m_TexturesNum].InOffset=m_FS_MainVP_Dir[j].offset;
					m_Textures[m_TexturesNum].InSize=m_FS_MainVP_Dir[j].size;
					m_Textures[m_TexturesNum].Valid=TRUE;
				}
			}

			//ANI Search process #: Skip if turned off
			if((!bAniLoad) && (m_Textures[m_TexturesNum].FileType==TEXTUREINFOFILETYPE_ANI))
			{
				m_FS_BitmapData.pic[i].valid=0;    
				m_Textures[m_TexturesNum].Valid=FALSE;
				TRACE("Above ANI SKIPPED due to user setting!\n");
				/*TOREMOVE*/
			}

			//Search process FAILED!
			if(unknownfiletype)
			{
				m_FS_BitmapData.pic[i].valid=0;	// not ani or pcx...
				TRACE(".pcx/.ani...NOT FOUND!\n");
				m_Textures[m_TexturesNum].Location="<Not found>";
				m_Textures[m_TexturesNum].Type="<Unknown>";
				m_Textures[m_TexturesNum].Valid=FALSE;
				m_Textures[m_TexturesNum].Error="Not found";
			}

			//Load file
			if(m_Textures[m_TexturesNum].Valid)
			{
				CFile f;
				VERIFY(f.Open(m_Textures[m_TexturesNum].InFilename,CFile::modeRead));
				f.Seek(m_Textures[m_TexturesNum].InOffset,CFile::begin);
				switch(m_Textures[m_TexturesNum].FileType)
				{
				case TEXTUREINFOFILETYPE_PCX:
				{
					//Header
					PCXHEADER header;
					f.Read(&header,sizeof(PCXHEADER));
					if(header.Version!=5)
					{
						m_FS_BitmapData.pic[i].valid=0;
						m_Textures[m_TexturesNum].Valid=FALSE;
						m_Textures[m_TexturesNum].Error="Invalid PCX version";
						break;
					}

					xreal=header.Xmax-header.Xmin+1;
					yreal=header.Ymax-header.Ymin+1;
					m_Textures[m_TexturesNum].Width=xreal;
					m_Textures[m_TexturesNum].Height=yreal;

					unsigned int kb;
					kb=2; while(kb<xreal) kb*=2; xsize=kb;
					kb=2; while(kb<yreal) kb*=2; ysize=kb;

					//Palette
					if(header.Version==5)
					{
						unsigned char byte1;
						f.Seek(m_Textures[m_TexturesNum].InOffset+m_Textures[m_TexturesNum].InSize-769,CFile::begin);
						f.Read(&byte1,1);
						if(byte1==0x0C)
						{
							for(int j=0;j<256;j++)
							{
								f.Read(&m_Textures[m_TexturesNum].Palette[0][j],1); //Red
								f.Read(&m_Textures[m_TexturesNum].Palette[1][j],1); //Green
								f.Read(&m_Textures[m_TexturesNum].Palette[2][j],1); //Blue
								m_Textures[m_TexturesNum].PaletteLong[j]=m_Textures[m_TexturesNum].Palette[0][j]+(m_Textures[m_TexturesNum].Palette[1][j]<<8)+(m_Textures[m_TexturesNum].Palette[2][j]<<16)+(255<<24);
							}
						}
					}
					f.Seek(m_Textures[m_TexturesNum].InOffset+128,CFile::begin);

					if(xreal*yreal>1024*2048)
					{
						m_FS_BitmapData.pic[i].valid=0;
						m_Textures[m_TexturesNum].Valid=FALSE;
						m_Textures[m_TexturesNum].Error="PCX Texture too big (>2097152 pixels)";
						break;
					}
	
#ifdef _WITHTXVIEW
					int x_clusterize2=xreal;
					if(x_clusterize2%2!=0)
						x_clusterize2+=2-(xreal%2);
					int x_clusterize4=xreal;
					if(x_clusterize4%4!=0)
						x_clusterize4+=4-(xreal%4);

					m_Textures[m_TexturesNum].Flags=TEXTUREINFOFLAG_TEXTURELOADED;
					
					unsigned char *tmpbuffer;
					try {
						m_Textures[m_TexturesNum].Bitmap=new unsigned char[x_clusterize4*yreal];
						tmpbuffer=new unsigned char[x_clusterize2*yreal];
					}
					catch (CMemoryException e) {
						m_Textures[m_TexturesNum].Valid=FALSE;
						m_Textures[m_TexturesNum].Error="Out of Memory";
						m_FS_BitmapData.pic[m_TexturesNum].valid=0;
						break;
					}
#endif

					//Bitmap
					XScale=0;
					YScale=0;
					unsigned char XScaleTest=0;
					unsigned char YScaleTest=0;
					if(bFastload)
					{
						while(MaxSize<(xsize>>XScale))
						{
							XScale++;
							XScaleTest=XScaleTest<<1;
							XScaleTest++;
						}
						while(MaxSize<(ysize>>YScale))
						{
							YScale++;
							YScaleTest=YScaleTest<<1;
							YScaleTest++;
						}
					}
	
					unsigned char runcount=0;
					unsigned char runvalue=0;
					unsigned int runoffset=0;
					for(unsigned int j=0;j<yreal;j++)
					{
						unsigned int offset;
						if(bFastload)
							offset=(j>>YScale)*(xsize>>XScale);
						else
							offset=j*xreal;
						for(unsigned int k=0;k<(xreal+(xreal&1));k++)
						{
							if(runcount>0)
								runcount--;
							else {
								f.Read(&runvalue,1);
								if((runvalue & 0xC0)==0xC0)
								{
									runcount=(runvalue & 0x3F) - 1;
									f.Read(&runvalue,1);
								}
							}
							if(((j & YScaleTest)+(k & XScaleTest))==0)
								m_FS_RGBAtexture2[offset++]=m_Textures[m_TexturesNum].PaletteLong[runvalue];
							
#ifdef _WITHTXVIEW
							if(runoffset<x_clusterize4*yreal) //Just to be sure
								tmpbuffer[runoffset]=runvalue;
#endif
							runoffset++;
						}	
					}
#ifdef _WITHTXVIEW
					for(unsigned int j=0;j<yreal;j++)
						memcpy(&m_Textures[m_TexturesNum].Bitmap[j*x_clusterize4],&tmpbuffer[j*x_clusterize2],xreal);
					delete[] tmpbuffer;
#endif
					for(unsigned int j=0;j<yreal;j++)
					{
						if(bFastload)
						{
							unsigned int offset=(j>>YScale)*(xsize>>XScale) + (xreal>>XScale);
							for(unsigned int k=xreal;k<xsize;k++)
							{	// fill out the extra
								if((k&XScaleTest)==0)
								{
									//RGBAtexture2[offset]=0xff00ff00;
									m_FS_RGBAtexture2[offset]=m_FS_RGBAtexture2[offset-(xreal>>XScale)];
									offset++;
	
								}
							}
						}
					}

					if(bFastload)
					{
						for(unsigned int j=yreal;j<ysize;j++)
						{			// fill out the extra
							unsigned int offset=(j>>YScale)*(xsize>>XScale);
							for(unsigned int k=0;k<xsize;k++)
							{
								if((k&XScaleTest)==0)
								{
									//RGBAtexture2[offset]=0xff0000ff;
									m_FS_RGBAtexture2[offset]=m_FS_RGBAtexture2[offset-(yreal>>YScale)*(xsize>>XScale)];
									offset++;
								}
							}
						}
					}
					m_Textures[m_TexturesNum].Error="<ok>";
				}
				break;

				case TEXTUREINFOFILETYPE_ANI:
				{
					//Header
					ANIHEADER header;
					f.Read(&header,sizeof(ANIHEADER));
					if(header.version!=2)
					{
						m_FS_BitmapData.pic[i].valid=0;
						m_Textures[m_TexturesNum].Valid=FALSE;
						m_Textures[m_TexturesNum].Error="Invalid ANI version";
						break;
					}
					if(header.id!=0)
					{
						m_FS_BitmapData.pic[i].valid=0;
						m_Textures[m_TexturesNum].Valid=FALSE;
						m_Textures[m_TexturesNum].Error="Invalid ANI file id";
						break;
					}

					xreal=header.w;
					yreal=header.h;
					m_Textures[m_TexturesNum].Width=xreal;
					m_Textures[m_TexturesNum].Height=yreal;

					unsigned int jb;
					jb=2; while(jb<xreal) jb*=2; xsize=jb;
					jb=2; while(jb<yreal) jb*=2; ysize=jb;

					//Palette
					for(int j=0;j<256;j++)
					{
						m_Textures[m_TexturesNum].Palette[0][j]=header.palette[j*3+0]; //Red
						m_Textures[m_TexturesNum].Palette[1][j]=header.palette[j*3+1]; //Green
						m_Textures[m_TexturesNum].Palette[2][j]=header.palette[j*3+2]; //Blue
						m_Textures[m_TexturesNum].PaletteLong[j]=m_Textures[m_TexturesNum].Palette[0][j]+(m_Textures[m_TexturesNum].Palette[1][j]<<8)+(m_Textures[m_TexturesNum].Palette[2][j]<<16)+(255<<24);
					}

					if(xreal*yreal>1024*2048)
					{
						m_FS_BitmapData.pic[i].valid=0;
						m_Textures[m_TexturesNum].Valid=FALSE;
						m_Textures[m_TexturesNum].Error="PCX Texture too big (>2097152 pixels).";
						break;
					}
					
#ifdef _WITHTXVIEW
					int size=xreal*yreal;
					m_Textures[m_TexturesNum].Flags=TEXTUREINFOFLAG_TEXTURELOADED;
					m_Textures[m_TexturesNum].Bitmap=new unsigned char[size];
#endif

					//Bitmap
					XScale=0;
					YScale=0;
					unsigned char XScaleTest=0;
					unsigned char YScaleTest=0;
					if(bFastload)
					{
						while(MaxSize<(xsize>>XScale))
						{
							XScale++;
							XScaleTest=XScaleTest<<1;
							XScaleTest++;
						}
						while(MaxSize<(ysize>>YScale))
						{
							YScale++;
							YScaleTest=YScaleTest<<1;
							YScaleTest++;
						}
					}

					unsigned char byte1;
					unsigned short byte2;
					unsigned int byte4;
					f.Read(&byte2,2);
					f.Read(&byte4,4);
					f.Read(&byte4,4);
					f.Read(&byte1,1);

					//Read just the 1st frame... thats all we need
					unsigned char runcount=0;
					unsigned char runvalue=0;
					int runoffset=0;
					for(unsigned int j=0;j<yreal;j++)
					{
						for(unsigned int k=0;k<xreal;k++)
						{
							if(runcount>0)
								runcount--;
							else {
								f.Read(&runvalue,1);
								if(runvalue==header.packer_code)
								{
									f.Read(&runcount,1);
									f.Read(&runvalue,1);
								}
							}
							if(((j&YScaleTest)+(k&XScaleTest))==0)
							{
								unsigned int offset;
								if(bFastload)
									offset=(j>>YScale)*(xsize>>XScale)+(k>>XScale);
								else
									offset=j*xreal + k;
								m_FS_RGBAtexture2[offset]=m_Textures[m_TexturesNum].PaletteLong[runvalue];
							}
#ifdef _WITHTXVIEW
							m_Textures[m_TexturesNum].Bitmap[runoffset]=runvalue;
							runoffset++;
#endif
						}
						for(unsigned int k=xreal;k<xsize;k++)
						{	//fill out the extra
							if(((j&YScaleTest)+(k&XScaleTest))==0)
							{
								unsigned int offset;
								if(bFastload)
									offset=(j>>YScale)*(xsize>>XScale)+(k>>XScale);
								else
									offset=j*xreal+k;
								m_FS_RGBAtexture2[offset]=0xff00ff00;
							}
						}
					}               
					for(unsigned int j=yreal;j<ysize;j++)
					{			// fill out the extra
						for(unsigned int k=0;k<xsize;k++)
						{
							if(((j&YScaleTest)+(k&XScaleTest))==0)
							{
								unsigned int offset;
								if(bFastload)
									offset=(j>>YScale)*(xsize>>XScale)+(k>>XScale);
								else
									offset=j*xreal+k;
								m_FS_RGBAtexture2[offset]=0xff00ff00;
							}
						}
					}
					m_Textures[m_TexturesNum].Error="<ok>";
				}
				break;

				default:
					ASSERT(FALSE);
				}
			}

			//////////////////////////////////////////////////////////////////////////////
			//Load the image into OpenGL
			if(m_FS_BitmapData.pic[i].valid==1)
			{
				//Load the image into m_Textures
				int size=xsize*ysize*4;

				m_FS_BitmapData.pic[i].xreal=xreal;
				m_FS_BitmapData.pic[i].yreal=yreal;
				m_FS_BitmapData.pic[i].xsize=xsize;
				m_FS_BitmapData.pic[i].ysize=ysize;

TRACE("m_FS_LoadPCX[%i]=%i-%i (ActivePM)\n",i,i,m_FS_PofDataL[ActivePM],ActivePM);
				m_FS_LoadPCX[i]=i-m_FS_PofDataL[ActivePM];

				glNewList(m_FS_ModelTexture[i-m_FS_PofDataL[ActivePM]]=glGenLists(1),GL_COMPILE);
				m_FS_TextureFlag[i-m_FS_PofDataL[ActivePM]]=TRUE;

				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glPixelStorei(GL_UNPACK_ALIGNMENT,4);
				glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
				glPixelStorei(GL_UNPACK_SKIP_ROWS,0);
				glPixelStorei(GL_UNPACK_SKIP_PIXELS,0);
				if(bFastload)
					glTexImage2D(GL_TEXTURE_2D,0,4,xsize>>XScale,ysize>>YScale,0,GL_RGBA,GL_UNSIGNED_BYTE,m_FS_RGBAtexture2);
				else
				{
					if(xsize>MaxSize)
						xsize=MaxSize;
					if(ysize>MaxSize)
						ysize=MaxSize;

					gluScaleImage(GL_RGBA,xreal,yreal,GL_UNSIGNED_BYTE,m_FS_RGBAtexture2,xsize,ysize,GL_UNSIGNED_BYTE,m_FS_RGBAtexture);
					glTexImage2D(GL_TEXTURE_2D,0,4,xsize,ysize,0,GL_RGBA,GL_UNSIGNED_BYTE,m_FS_RGBAtexture);
				}
				glEndList();
				m_Textures_Available=TRUE;
			}
		}
	}

	m_NoTexturesFound=FALSE;
	if(!m_Textures_Available && (m_FS_BitmapData.count>0) && (m_FS_PofDataL[ActivePM]<(m_FS_PofDataH[ActivePM]+1)) && !m_InfoMode)
		m_NoTexturesFound=TRUE;

	if(!m_InfoMode)
		SetStatusBarText("Ready");

	return ERROR_GEN_NOERROR;
}

signed char read_INT8(CFile *fp)
{
	signed char value;
	fp->Read(&value,sizeof(signed char));
	return value;
}

signed short read_INT16(CFile *fp)
{
	signed short value;
	fp->Read(&value,sizeof(signed short));
	return value;
}

signed long read_INT32(CFile *fp)
{
	signed long value;
	fp->Read(&value,sizeof(signed long));
	return value;
}

//Without RETURN support
CString read_STRING(CFile *fp)
{
	int len=read_INT32(fp);
	CString ret="";
	for(int i=0; i<len; i++)
	{
		char x=(char)read_INT8(fp);
		if(x!=0 && x!=13 && x!=10)
			ret+=x;
	}
	return ret;
}

//With RETURN support
CString read_STRING13(CFile *fp)
{
	int len=read_INT32(fp);
	CString ret="";
	for(int i=0; i<len; i++)
	{
		char x=(char)read_INT8(fp);
		if(x!=0)
			ret+=x;
	}
	return ret;
}

//Fixed string
CString read_STRINGfixed(CFile *fp,int len)
{
	BOOL nullpassed=FALSE;
	CString ret="";
	for(int i=0; i<len; i++)
	{
		char x=(char)read_INT8(fp);
		if(x!=0 && !nullpassed)
			ret+=x;
		else
			nullpassed=TRUE;
	}
	return ret;
}

//0-terminated string
CString read_STRINGterminated(CFile *fp)
{
	CString ret;
	char x;
	do {
		x=(char)read_INT8(fp);
		if(x!=0)
			ret+=x;
	} while (x!=0);
	return ret;
}


unsigned int read_UINTW(CFile *fp)
{
	UINT32 value;
	fp->Read(&value,sizeof(UINT32));
//  assert(value <=0xffff);
	return (unsigned int)value;
}

FS_VPNT read_VECTOR(CFile *fp)
{
	FS_VPNT value;
	fp->Read(&value,sizeof(FS_VPNT));
	return value;
}

float read_float(CFile *fp)
{
	float value;
	fp->Read(&value,sizeof(float));
	return value;
}

ULONGLONG cfseek(CFile *fp,long offset,int whence)
{
//  printf("%#08lx\n",ftell(fp));
	return fp->Seek(offset,whence);
}


#define wc(walker)  (*((unsigned char *) (walker)))
#define ws(walker)  (*((unsigned short *) (walker)))
#define wi(walker)  (*((unsigned int *) (walker)))
#define wf(walker)  (*((float *) (walker)))
#define v(walker)  (*((FS_VPNT *) (walker)))
#define wp(walker) ((unsigned short *) (walker))
#define vp(walker) ((FS_VPNT *) (walker))


#define LOGTEXT	if(!m_InfoMode) logtext.Format
#define LOGDOIT if(!m_InfoMode) flog.WriteString(logtext)


ERRORCODE CMODVIEW32Doc::FS_ReadPOF(CFile *fp,int VpNum)
{
	ERRORCODE ret;
	CString logtext;
	CStdioFile flog;

	ClearDocument(FALSE);
	m_FS_CurrVP_FreeSpaceVersion=0;
	m_Textures_Available=FALSE;

	if(!m_InfoMode)
		SetStatusBarText("Loading POF...");

	memset(&m_FS_PMinfo.debris,255,80);
	flog.Open(((CMODVIEW32App *)AfxGetApp())->m_LogFile,CFile::modeWrite | CFile::modeCreate);
	
	if(m_FS_CurrVP_PofNum==0)
	{
		fp->Seek(0L,SEEK_END);
		m_CurrentFile_Offset=0;
		m_CurrentFile_Size=fp->GetPosition();
		LOGTEXT("Location: %s, filesize: %i bytes\n",fp->GetFilePath(),m_CurrentFile_Size); LOGDOIT;
	} else {
		m_CurrentFile_Offset=m_FS_CurrVP_Dir[m_FS_CurrVP_PofList[VpNum]].offset;
		m_CurrentFile_Size=/*m_FS_CurrVP_Dir[m_FS_CurrVP_PofList[VpNum]].offset + */m_FS_CurrVP_Dir[m_FS_CurrVP_PofList[VpNum]].size;
		LOGTEXT("Location: %s at offset %i, filesize: %i bytes\n",fp->GetFilePath(),m_CurrentFile_Offset,m_CurrentFile_Size); LOGDOIT;
	}
	fp->Seek(m_CurrentFile_Offset, SEEK_SET);

	unsigned int long len,next_chunk;
	unsigned int long temp,pos_start; 
	unsigned int version;
	int StartByte;
	unsigned int id=read_INT32(fp);
	if(id!=0x4f505350)
	{
		m_ErrorString=ERROR_FS_INVALIDPOFHEADER_TEXT;
		return ERROR_FS_INVALIDPOFHEADER;
	}

	char *pid=(char *)&id;
	version=read_INT32(fp);
	m_FileVersion=version;
	LOGTEXT("File version %i\n",m_FileVersion); LOGDOIT;

	BOOL hasheader=FALSE;
	BOOL hasobjcts=FALSE;

	while(m_CurrentFile_Offset+m_CurrentFile_Size>fp->GetPosition())
	{
		id=read_INT32(fp);
		len=read_INT32(fp);
		next_chunk = (unsigned long) fp->GetPosition() + len;
		pos_start = (unsigned long) fp->GetPosition();

		pid=(char *)&id;
		m_Blocks_Num++;
		m_Blocks[m_Blocks_Num-1].name.Format("%c%c%c%c",*pid,*(pid+1),*(pid+2),*(pid+3));
		m_Blocks[m_Blocks_Num-1].offset=pos_start-m_CurrentFile_Offset;
		m_Blocks[m_Blocks_Num-1].size=len;
		m_Blocks[m_Blocks_Num-1].desc="??? <Unknown>";

		if(next_chunk>m_CurrentFile_Offset+m_CurrentFile_Size)
		{
			ASSERT(FALSE);
			m_ErrorString=ERROR_FS_CHUNKDATAOVERFLOW_TEXT;
			return ERROR_FS_CHUNKDATAOVERFLOW;
		}

#ifdef _DEBUG
	TRACE("%i <%c%c%c%c> %i %d\n",pos_start,*pid,*(pid+1),*(pid+2),*(pid+3),len,m_FS_NumSOBJ);
#endif

		switch(id)
		{

		case ID_OHDR:
		case ID_HDR2:
			{		//Object header
				m_Blocks[m_Blocks_Num-1].desc="Model header";

				//EditorFS
				m_EditorFS_ChunkOHDR.ReadChunk(fp,TRUE);

				hasheader=TRUE;

				if(id==ID_HDR2)
				{
					m_FS_CurrVP_FreeSpaceVersion=2;
					m_FS_Model.radius=read_float(fp);		   //rad
					m_FS_ModelInfo.obj_flags=read_INT32(fp);		   //obj_flags
				}

				m_FS_PMinfo.n_models=read_INT32(fp);	 //n_subobj
				if(m_FS_PMinfo.n_models>=MAX_FS_SOBJ)
				{
					m_ErrorString=ERROR_FS_TOOMANYSOBJS_TEXT;
					return ERROR_FS_TOOMANYSOBJS;
				}

				for(unsigned int i=0;i<m_FS_PMinfo.n_models;i++)
				{
					m_FS_SOBJ[i].real_offset.x=0;
					m_FS_SOBJ[i].real_offset.y=0;
					m_FS_SOBJ[i].real_offset.z=0;
					m_FS_SOBJ[i].offset.x=0;
					m_FS_SOBJ[i].offset.y=0;
					m_FS_SOBJ[i].offset.z=0;
				}
				if(id==ID_OHDR)
				{
					m_FS_CurrVP_FreeSpaceVersion=1;
					m_FS_Model.radius=read_float(fp);        //rad
					m_FS_ModelInfo.obj_flags=read_INT32(fp);         //obj_flags
				}
				m_FS_Model._minbox=read_VECTOR(fp);	  //pmmin
				m_FS_Model._maxbox=read_VECTOR(fp);	  //pmmax
				m_FS_PMinfo.n_detail=read_INT32(fp);
				LOGTEXT("Number of submodels: %i\nRadius: %.2f\nNumber of detail versions: %i\n",m_FS_PMinfo.n_models,m_FS_Model.radius,m_FS_PMinfo.n_detail);
				LOGDOIT;

				for(unsigned int j=0;j<m_FS_PMinfo.n_detail;j++)
					m_FS_PMinfo.detail[j]=read_INT32(fp);
				m_FS_PMinfo.n_debris=read_INT32(fp);
				for(unsigned int j=0;j<m_FS_PMinfo.n_debris;j++)
					m_FS_PMinfo.debris[j]=read_INT32(fp);

				//Mass
				if(m_FileVersion>=1903)
				{
					float scale=1.0f;
					m_FS_ModelInfo.mass=read_float(fp);
					m_FS_ModelInfo.mass_center=read_VECTOR(fp);
					if(m_FileVersion<2009)
					{
						//vol_mass -> area_mass conversion
						double vol_mass=m_FS_ModelInfo.mass;
						double areamass=4.65*pow(m_FS_ModelInfo.mass,2/3);
						scale=(float) (vol_mass/areamass);
						m_FS_ModelInfo.mass=areamass;
					}
					for(int j1=0;j1<3;j1++)
					{
						for(int j2=0;j2<3;j2++)
							m_FS_ModelInfo.moment_inertia[j2][j1]=(float)read_float(fp)*scale;
					}
				} else {
					m_FS_ModelInfo.mass=1.0;
					m_FS_ModelInfo.mass_center.x=0.0;
					m_FS_ModelInfo.mass_center.y=0.0;
					m_FS_ModelInfo.mass_center.z=0.0;
					for(int j1=0;j1<3;j1++)
					{
						for(int j2=0;j2<3;j2++)
							m_FS_ModelInfo.moment_inertia[j2][j1]=0.0; /**/
					}
				}

				//Cross sections
				if(m_FileVersion>=2014)
				{
					m_FS_ModelInfo.num_cross_sections=read_INT32(fp);
					if(m_FS_ModelInfo.num_cross_sections>MAX_FS_CROSSSECTIONS)
					{
						m_ErrorString=ERROR_FS_TOOMANYCROSSSECTIONS_TEXT;
						m_ErrorDetails.Format("Is: %i, Should be <=%i",m_FS_ModelInfo.num_cross_sections,MAX_FS_CROSSSECTIONS);
						return ERROR_FS_TOOMANYCROSSSECTIONS;
					}
					for(int i=0;i<m_FS_ModelInfo.num_cross_sections;i++)
					{
						m_FS_ModelInfo.cross_section_depth[i]=read_float(fp);
						m_FS_ModelInfo.cross_section_radius[i]=read_float(fp);
					}
				}
				else
					m_FS_ModelInfo.num_cross_sections=0;

				//Lights
				if(m_FileVersion>=2007)
				{
					m_FS_ModelInfo.num_lights=read_INT32(fp);
					if(m_FS_ModelInfo.num_lights==ID_SOBJ || m_FS_ModelInfo.num_lights==ID_OBJ2)
					{
						m_FS_ModelInfo.num_lights=0;
						fp->Seek(next_chunk,CFile::begin);
						AfxMessageBox("This file contains an invalid HDR2 data chunk, which is missing lights specification. This might have been caused by a bug in the utility Segeltuch by Frank Krueger. MODELVIEW can fix this file, if you go into POF-Editor now and save it from there.");
						break; //Fix a Segeltuch bug
					}
					if(m_FS_ModelInfo.num_lights>MAX_FS_LIGHTS)
					{
						m_ErrorString=ERROR_FS_TOOMANYLIGHTS_TEXT;
						m_ErrorDetails.Format("Is: %i, Should be <=%i",m_FS_ModelInfo.num_lights,MAX_FS_LIGHTS);
						return ERROR_FS_TOOMANYLIGHTS;
					}
					for(int i=0;i<m_FS_ModelInfo.num_lights;i++)
					{
						m_FS_ModelInfo.light_location[i]=read_VECTOR(fp);
						m_FS_ModelInfo.light_type[i]=read_INT32(fp);
						ASSERT(m_FS_ModelInfo.light_type[i]>=1);
						ASSERT(m_FS_ModelInfo.light_type[i]<=2);
					}
				}

				break;
			}

		case ID_TXTR:
			{		//Texture filename list
				m_Blocks[m_Blocks_Num-1].desc="Texture reference list";
				m_EditorFS_ChunkTXTR.ReadChunk(fp,TRUE);

				unsigned int charcount;
				unsigned int i,j;
				char inputbuf[30];

				m_FS_BitmapData.count=read_INT32(fp);
				if(m_FS_BitmapData.count>MAX_FS_TEXTURE)
				{
					m_ErrorString=ERROR_FS_TOOMANYTEXTURES_TEXT;
					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_FS_BitmapData.count,MAX_FS_TEXTURE);
					return ERROR_FS_TOOMANYTEXTURES;
				}

				for(i=0;i<m_FS_BitmapData.count;i++)
				{
					charcount=read_INT32(fp);
					if(charcount>29)
					{
						m_ErrorString=ERROR_FS_INVALIDTEXTURENAME_TEXT;
						return ERROR_FS_INVALIDTEXTURENAME;
					}
					for(j=0;j<charcount;j++)
						inputbuf[j]=read_INT8(fp);
					inputbuf[charcount]=0;
					strcpy_s(m_FS_BitmapData.pic[i].name,inputbuf);
				}
				break;
			}

			//*********
		case ID_SHLD: {		//shield data
				m_Blocks[m_Blocks_Num-1].desc="Shield data";
				m_EditorFS_ChunkSHLD.ReadChunk(fp,TRUE);

				unsigned int i;

				//Shield vertices
				m_FS_Model.shields.Vcount=read_INT32(fp);
				if(m_FS_Model.shields.Vcount>MAX_FS_SHIELDVERTICES)
				{
					m_ErrorString=ERROR_FS_SHIELDTOOMANYVERTICIES_TEXT;
					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_FS_Model.shields.Vcount,MAX_FS_SHIELDVERTICES);
					return ERROR_FS_SHIELDTOOMANYVERTICIES;
				}
				for(i=0;i<m_FS_Model.shields.Vcount;i++)
				{
					m_FS_Model.shields.Vpoint[i][0]=read_float(fp);
					m_FS_Model.shields.Vpoint[i][1]=read_float(fp);
					m_FS_Model.shields.Vpoint[i][2]=read_float(fp);
				}

				//Shield faces
				m_FS_Model.shields.Fcount=read_INT32(fp);
				if(m_FS_Model.shields.Fcount>MAX_FS_SHIELDFACES)
				{
					m_ErrorString=ERROR_FS_SHIELDTOOMANYFACES_TEXT;
					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_FS_Model.shields.Fcount,MAX_FS_SHIELDFACES);
					return ERROR_FS_SHIELDTOOMANYFACES;
				}
				for(i=0;i<m_FS_Model.shields.Fcount;i++)
				{
					m_FS_Model.shields.Face[i].Normal=read_VECTOR(fp);
					m_FS_Model.shields.Face[i].Vface[0]=read_INT32(fp);
					m_FS_Model.shields.Face[i].Vface[1]=read_INT32(fp);
					m_FS_Model.shields.Face[i].Vface[2]=read_INT32(fp);
					m_FS_Model.shields.Face[i].Nface[0]=read_INT32(fp);
					m_FS_Model.shields.Face[i].Nface[1]=read_INT32(fp);
					m_FS_Model.shields.Face[i].Nface[2]=read_INT32(fp);
				}

				m_EditorFS_ShieldsBackup=m_FS_Model.shields;
				break;
			}


			//*********
		case ID_SOBJ:
		case ID_OBJ2: {		//Subobject header
				hasobjcts=TRUE;

				if(m_FS_NumSOBJ>=MAX_FS_SOBJ)
				{
					m_ErrorString=ERROR_FS_TOOMANYSOBJS_TEXT;
					m_ErrorDetails.Format("Should be <=%i",MAX_FS_SOBJ);
					return ERROR_FS_TOOMANYSOBJS;
				}

				if(m_FS_NumSOBJ==0)
				{
					LOGTEXT("\nSubobjects -->\n"); LOGDOIT;
				}

				//EditorFS
				m_EditorFS_ChunkSOBJ[m_FS_NumSOBJ].ReadChunk(fp,TRUE);

				StartByte=m_FS_Model.Vcount;

				m_FS_SOBJ[m_FS_NumSOBJ].submodel_number=read_INT32(fp);		 //submodel      
				if(id==ID_OBJ2)
					m_FS_SOBJ[m_FS_NumSOBJ].radius=read_float(fp);		 //rad 	fs2
				m_FS_SOBJ[m_FS_NumSOBJ].submodel_parent=read_INT32(fp);	   //parent
				m_FS_SOBJ[m_FS_NumSOBJ].real_offset=read_VECTOR(fp);	  //offset
				m_FS_SOBJ[m_FS_NumSOBJ].offset=m_FS_SOBJ[m_FS_NumSOBJ].real_offset;
				if(m_FS_SOBJ[m_FS_NumSOBJ].submodel_parent!=-1)
				{
					m_FS_SOBJ[m_FS_NumSOBJ].offset.x+=m_FS_SOBJ[m_FS_SOBJ[m_FS_NumSOBJ].submodel_parent].offset.x; 
					m_FS_SOBJ[m_FS_NumSOBJ].offset.y+=m_FS_SOBJ[m_FS_SOBJ[m_FS_NumSOBJ].submodel_parent].offset.y;
					m_FS_SOBJ[m_FS_NumSOBJ].offset.z+=m_FS_SOBJ[m_FS_SOBJ[m_FS_NumSOBJ].submodel_parent].offset.z;
				}
				if(id==ID_SOBJ)
					m_FS_SOBJ[m_FS_NumSOBJ].radius=read_float(fp);      //rad
				m_FS_SOBJ[m_FS_NumSOBJ].geometric_center=read_VECTOR(fp);	 //center
				m_FS_SOBJ[m_FS_NumSOBJ].bounding_box_min_point=read_VECTOR(fp);	 //objmin
				m_FS_SOBJ[m_FS_NumSOBJ].bounding_box_max_point=read_VECTOR(fp);	 //objmax
				ULONGLONG dummy1=fp->GetPosition();
				m_FS_SOBJ[m_FS_NumSOBJ].submodel_name=read_STRING(fp);
				ULONGLONG dummy2=fp->GetPosition();
				m_FS_SOBJ[m_FS_NumSOBJ].properties=read_STRING13(fp);
				ULONGLONG dummy3=fp->GetPosition();
				m_FS_SOBJ[m_FS_NumSOBJ].movement_type=read_INT32(fp);
				m_FS_SOBJ[m_FS_NumSOBJ].movement_axis=read_INT32(fp);
				m_FS_SOBJ[m_FS_NumSOBJ].reserved=read_INT32(fp); ASSERT(m_FS_SOBJ[m_FS_NumSOBJ].reserved==0);	   //reserved (always 0)
				m_FS_SOBJ[m_FS_NumSOBJ].bsp_data_size=read_INT32(fp);		 //model data size
				m_FS_SOBJ[m_FS_NumSOBJ].bsp_data=new unsigned char[m_FS_SOBJ[m_FS_NumSOBJ].bsp_data_size];		 //model data size
				if(m_FS_SOBJ[m_FS_NumSOBJ].bsp_data==NULL)
				{
					m_ErrorString=ERROR_GEN_OUTOFMEMORY_TEXT;
					return ERROR_GEN_OUTOFMEMORY;
				}
				fp->Read(m_FS_SOBJ[m_FS_NumSOBJ].bsp_data,m_FS_SOBJ[m_FS_NumSOBJ].bsp_data_size);
				ret=FS_Walk(m_FS_SOBJ[m_FS_NumSOBJ].bsp_data,m_FS_NumSOBJ,StartByte);
				if(ret!=ERROR_GEN_NOERROR)
					return ret;

				LOGTEXT("Subobject %i: %s, parent: %i\n",m_FS_NumSOBJ,m_FS_SOBJ[m_FS_NumSOBJ].submodel_name,m_FS_SOBJ[m_FS_NumSOBJ].submodel_parent); LOGDOIT;
				m_Blocks[m_Blocks_Num-1].desc="Submodel \""+m_FS_SOBJ[m_FS_NumSOBJ].submodel_name+"\"";
				m_FS_NumSOBJ++;
				
				ULONGLONG dummy=fp->GetPosition();
				ASSERT(fp->GetPosition()==next_chunk);
				break;
			}
			
		case ID_GPNT:
		case ID_MPNT:
			{
				m_Blocks[m_Blocks_Num-1].desc="Gun points";
				int guntype=0; //GPNT
				if(id==ID_MPNT)
				{
					guntype=1; //MPNT
					m_Blocks[m_Blocks_Num-1].desc="Missile points";
				}

				if(m_FS_RealGuns[guntype].Num!=0)
				{
					LOGTEXT("WARNING! More than one \"%s\" block. MODELVIEW32 will ignore all except the last.\n",m_Blocks[m_Blocks_Num-1].desc); LOGDOIT;
				}
				m_FS_RealGuns[guntype].Num=read_INT32(fp);
				if(m_FS_RealGuns[guntype].Num>=MAX_FS_REALGUNS_SLOTS)
				{
					m_ErrorString=ERROR_FS_TOOMANYREALGUNSLOTS_TEXT;
					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_FS_RealGuns[guntype].Num,MAX_FS_REALGUNS_SLOTS);
					return ERROR_FS_TOOMANYREALGUNSLOTS;
				}
				for(int i=0;i<m_FS_RealGuns[guntype].Num;i++)
				{
					m_FS_RealGuns[guntype].Slot[i].num_guns=read_INT32(fp);
					for(int j=0;j<m_FS_RealGuns[guntype].Slot[i].num_guns;j++)
					{
						if(m_Guns.Num>=MAX_GUNS)
						{
							m_ErrorString=ERROR_GEN_TOOMANYGUNS_TEXT;
							m_ErrorDetails.Format("Exceeded %i.",MAX_GUNS);
							return ERROR_GEN_TOOMANYGUNS;
						}
						m_FS_RealGuns[guntype].Slot[i].point[j]=read_VECTOR(fp);
						m_FS_RealGuns[guntype].Slot[i].normal[j]=read_VECTOR(fp);
						m_Guns.Gun[m_Guns.Num]=m_FS_RealGuns[guntype].Slot[i].point[j];
						m_Guns.GunDirection[m_Guns.Num]=m_FS_RealGuns[guntype].Slot[i].normal[j];
						m_Guns.InSubModel[m_Guns.Num]=0;
						m_Guns.GunBank[m_Guns.Num]=i;
						if(id==ID_GPNT)
							m_Guns.GunType[m_Guns.Num]=GUNTYPE_PLAYER_GUN;
						else
							m_Guns.GunType[m_Guns.Num]=GUNTYPE_PLAYER_MIS;
						m_Guns.Num++;
					}
				}
			}
			break;
		
		case ID_TGUN:
		case ID_TMIS:
			{
				m_Blocks[m_Blocks_Num-1].desc="Gun turrets";
				int guntype=0; //TGUN
				if(id==ID_TMIS)
				{
					guntype=1; //TMIS
					m_Blocks[m_Blocks_Num-1].desc="Missile turrets";
				}

				if(m_FS_Turrets[guntype].Num!=0)
				{
					LOGTEXT("WARNING! More than one \"%s\" block. MODELVIEW32 will ignore all except the last.\n",m_Blocks[m_Blocks_Num-1].desc); LOGDOIT;
				}
				m_FS_Turrets[guntype].Num=read_INT32(fp);
				if(m_FS_Turrets[guntype].Num>=MAX_FS_TURRETS_BANKS)
				{
					m_ErrorString=ERROR_FS_TOOMANYTURRETBANKS_TEXT;
					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_FS_Turrets[guntype].Num,MAX_FS_TURRETS_BANKS);
					return ERROR_FS_TOOMANYTURRETBANKS;
				}
				for(int i=0;i<m_FS_Turrets[guntype].Num;i++)
				{
					m_FS_Turrets[guntype].Bank[i].sobj_parent=read_INT32(fp);
					m_FS_Turrets[guntype].Bank[i].sobj_par_phys=read_INT32(fp);
					m_FS_Turrets[guntype].Bank[i].turret_normal=read_VECTOR(fp);
					m_FS_Turrets[guntype].Bank[i].num_firing_points=read_INT32(fp);
					if(m_FS_Turrets[guntype].Bank[i].num_firing_points>=MAX_FS_FIRING_POINTS)
					{
						m_ErrorString=ERROR_FS_TOOMANYFIRINGPOINTS_TEXT;
						m_ErrorDetails.Format("Turret bank #%i - Is: %i, Should be <=%i",i,m_FS_Turrets[guntype].Bank[i].num_firing_points,MAX_FS_FIRING_POINTS);
						return ERROR_FS_TOOMANYFIRINGPOINTS;
					}
					for(int j=0;j<m_FS_Turrets[guntype].Bank[i].num_firing_points;j++)
					{
						m_FS_Turrets[guntype].Bank[i].firing_point[j]=read_VECTOR(fp);
						if(m_Guns.Num>=MAX_GUNS)
						{
							m_ErrorString=ERROR_GEN_TOOMANYGUNS_TEXT;
							m_ErrorDetails.Format("Exceeded %i.",MAX_GUNS);
							return ERROR_GEN_TOOMANYGUNS;
						}
						m_Guns.Gun[m_Guns.Num]=m_FS_Turrets[guntype].Bank[i].firing_point[j];
						m_Guns.GunDirection[m_Guns.Num]=m_FS_Turrets[guntype].Bank[i].turret_normal;
						m_Guns.InSubModel[m_Guns.Num]=m_FS_Turrets[guntype].Bank[i].sobj_parent;
						m_Guns.GunBank[m_Guns.Num]=i;
						if(id==ID_TGUN)
							m_Guns.GunType[m_Guns.Num]=GUNTYPE_TURRET_GUN;
						else
							m_Guns.GunType[m_Guns.Num]=GUNTYPE_TURRET_MIS;
						m_Guns.Num++;
					}
				}
			}
			break;

		case ID_SPCL:
			{
				m_Blocks[m_Blocks_Num-1].desc="Subsystems";
				m_FS_NumSubsystems=read_INT32(fp);
				LOGTEXT("\nSubsystems -->\n"); LOGDOIT;
				for(int i=0;i<m_FS_NumSubsystems;i++)
				{
					m_FS_Subsystems[i].Name=read_STRING(fp);
					m_FS_Subsystems[i].Properties=read_STRING(fp);
					m_FS_Subsystems[i].Desc=FS_MakeSubsystemDesc(m_FS_Subsystems[i].Name,m_FS_Subsystems[i].Properties);
					m_FS_Subsystems[i].Center=read_VECTOR(fp);
					m_FS_Subsystems[i].Radius=read_float(fp);
					LOGTEXT("Subsystem #%i: %s, properties: %s\n",i,m_FS_Subsystems[i].Name,m_FS_Subsystems[i].Properties); LOGDOIT;
				}
				ULONGLONG dummy=fp->GetPosition();
				ASSERT(fp->GetPosition()==next_chunk);
			}
			break;

		case ID_DOCK:
			{
				m_Blocks[m_Blocks_Num-1].desc="Docking points";
				m_EditorFS_ChunkDOCK.ReadChunk(fp,TRUE);

				LOGTEXT("\nDocking points -->\n"); LOGDOIT;
				ASSERT(m_FS_NumDocks==0); //No 2 DOCK blocks in one POF file
				m_FS_NumDocks=read_INT32(fp);
				if(m_FS_NumDocks>=MAX_FS_DOCKS)
				{
					m_ErrorString=ERROR_FS_TOOMANYDOCKINGPOINTS_TEXT;
					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_FS_NumDocks,MAX_FS_DOCKS);
					return ERROR_FS_TOOMANYDOCKINGPOINTS;
				}
				for(int i=0;i<m_FS_NumDocks;i++)
				{
					m_FS_Docks[i].properties=read_STRING(fp);
					m_FS_Docks[i].num_spline_paths=read_INT32(fp);
					if(m_FS_Docks[i].num_spline_paths>MAX_FS_SPLINE_PATHS)
					{
						m_ErrorString=ERROR_FS_DOCKTOOMANYSPLINEPATHS_TEXT;
						m_ErrorDetails.Format("Docking point #%i - Is: %i, Should be <=%i",i,m_FS_Docks[i].num_spline_paths,MAX_FS_SPLINE_PATHS);
						return ERROR_FS_DOCKTOOMANYSPLINEPATHS;
					}
					for(int j=0;j<m_FS_Docks[i].num_spline_paths;j++)
						m_FS_Docks[i].path_number[j]=read_INT32(fp);

					m_FS_Docks[i].num_points=read_INT32(fp);
					if(m_FS_Docks[i].num_points>MAX_FS_DOCK_POINTS)
					{
						m_ErrorString=ERROR_FS_DOCKTOOMANYDOCKPOINTS_TEXT;
						m_ErrorDetails.Format("Docking point #%i - Is: %i, Should be <=%i",i,m_FS_Docks[i].num_points,MAX_FS_DOCK_POINTS);
						return ERROR_FS_DOCKTOOMANYDOCKPOINTS;
					}
					for(int j=0;j<m_FS_Docks[i].num_points;j++)
					{
						m_FS_Docks[i].position[j]=read_VECTOR(fp);
						m_FS_Docks[i].normal[j]=read_VECTOR(fp);
					}
					LOGTEXT("Dock #%i: %i spline paths, %i points, properties: %s\n",i,m_FS_Docks[i].num_spline_paths,m_FS_Docks[i].num_points,m_FS_Docks[i].properties); LOGDOIT;
				}
			}
			break;

		case ID_PATH:
			{
				m_Blocks[m_Blocks_Num-1].desc="Path information";
				m_EditorFS_ChunkPATH.ReadChunk(fp,TRUE);
				LOGTEXT("\nPaths -->\n"); LOGDOIT;
				int num_paths=read_INT32(fp);
				for(int i=0;i<num_paths;i++)
				{
					CString y,z;
					y=read_STRING(fp);
					z=read_STRING(fp);
					int num_verts=read_INT32(fp);
					for(int j=0;j<num_verts;j++)
					{
						read_VECTOR(fp);
						read_float(fp);
						int num_turrets=read_INT32(fp);
						for(int k=0;k<num_turrets;k++)
							read_INT32(fp);
					}
					LOGTEXT("Path #%i: %i verts, name: %s, parent: %s\n",i,num_verts,y,z); LOGDOIT;
				}
			}
			break;

		case ID_FUEL:
			{
				m_Blocks[m_Blocks_Num-1].desc="Thrusters";
				m_EditorFS_ChunkFUEL.ReadChunk(fp,TRUE);

				LOGTEXT("\nThruster glows -->\n"); LOGDOIT;
				m_FS_Model.num_thrusters=read_INT32(fp);
				if(m_FS_Model.num_thrusters>MAX_FS_THRUSTERS)
				{
					m_ErrorString=ERROR_FS_TOOMANYTHRUSTERS_TEXT;
					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_FS_Model.num_thrusters,MAX_FS_THRUSTERS);
					return ERROR_FS_TOOMANYTHRUSTERS;
				}
				for(int i=0;i<m_FS_Model.num_thrusters;i++)
				{
					m_FS_Model.thrusters[i].num_glows=read_INT32(fp);
					if(m_FS_Model.thrusters[i].num_glows>MAX_FS_THRUSTERS_GLOWS)
					{
						m_ErrorString=ERROR_FS_TOOMANYTHRUSTERGLOWS_TEXT;
						m_ErrorDetails.Format("Thruster #%i - Is: %i, Should be <=%i",i,m_FS_Model.thrusters[i].num_glows,MAX_FS_THRUSTERS_GLOWS);
						return ERROR_FS_TOOMANYTHRUSTERGLOWS;
					}
					if(m_FS_CurrVP_FreeSpaceVersion==2 && m_FileVersion>=2117)
						m_FS_Model.thrusters[i].properties=read_STRING(fp);
					else
						m_FS_Model.thrusters[i].properties="";

					for(int j=0;j<m_FS_Model.thrusters[i].num_glows;j++)
					{
						m_FS_Model.thrusters[i].glow_pos[j]=read_VECTOR(fp);
						m_FS_Model.thrusters[i].glow_norm[j]=read_VECTOR(fp);
						m_FS_Model.thrusters[i].glow_radius[j]=read_float(fp);
						LOGTEXT("Thruster %i glow %i: Name: %s, Radius: %.2f\n",i,j,m_FS_Model.thrusters[i].properties,m_FS_Model.thrusters[i].glow_radius[j]); LOGDOIT;
					}
				}

			}
			break;

		case ID_PINF:
			{
				m_Blocks[m_Blocks_Num-1].desc="Model compiler information (unused)";
				//EditorFS
				m_EditorFS_ChunkPINF.ReadChunk(fp,TRUE);

				CString text;
				while(fp->GetPosition()!=next_chunk)
				{
					char ch;
					fp->Read(&ch,1);
					if(ch==0)
						ch='\n';
					text+=ch;
				}
				LOGTEXT("\nPOF Compiler information ->\n"); LOGDOIT;
				LOGTEXT("%s\n",text); LOGDOIT;
			}
			break;
					

		case ID_EYE_:
			m_Blocks[m_Blocks_Num-1].desc="Eye points";
			m_EditorFS_Chunk_EYE.ReadChunk(fp,TRUE);
			m_FS_ModelInfo.num_eye_points=read_INT32(fp);
			if(m_FS_ModelInfo.num_eye_points>1)
			{
				LOGTEXT("WARNING! More than one eye point: %i. MODELVIEW32 will ignore all except the first.\n",m_FS_ModelInfo.num_eye_points); LOGDOIT;
			}
			if(m_FS_ModelInfo.num_eye_points>0)
			{
				m_FS_ModelInfo.eye_point_sobj_number=read_INT32(fp);
				ASSERT(m_FS_ModelInfo.eye_point_sobj_number<m_FS_NumSOBJ);
				if(m_FS_ModelInfo.eye_point_sobj_number>=m_FS_NumSOBJ)
				{
					m_ErrorString=ERROR_FS_INVALIDEYEPOINTSOBJ_TEXT;
					m_ErrorDetails.Format("Valid subobjects id's: 0-%i, eye point specified: %i.",m_FS_NumSOBJ-1,m_FS_ModelInfo.eye_point_sobj_number);
					return ERROR_FS_INVALIDEYEPOINTSOBJ;
				}
				m_FS_ModelInfo.eye_point_sobj_offset=read_VECTOR(fp);
				m_FS_ModelInfo.eye_point_normal=read_VECTOR(fp);
			}
			break;

		case ID_INSG:
			m_Blocks[m_Blocks_Num-1].desc="Squad logo information";
			temp=read_INT32(fp);
			fp->Seek(-4,CFile::current);
			m_EditorFS_ChunkINSG.ReadChunk(fp,TRUE);
			
			LOGTEXT("\nSquad logo information ->\n"); LOGDOIT;
			LOGTEXT("Number of insignias: %i\n",temp); LOGDOIT;
			break;

		case ID_ACEN:
			m_Blocks[m_Blocks_Num-1].desc="AutoCenter point";
			m_EditorFS_ChunkACEN.ReadChunk(fp,TRUE);
			ASSERT(len==12);
			m_FS_ModelInfo.ACenPoint=read_VECTOR(fp);
			LOGTEXT("\nAutoCenter point ->\n"); LOGDOIT;
			LOGTEXT("Pos %.2f | %.2f | %.2f\n",m_FS_ModelInfo.ACenPoint.x,m_FS_ModelInfo.ACenPoint.y,m_FS_ModelInfo.ACenPoint.z); LOGDOIT;
			break;

		default:
			{
				char outputbuf[256];
				sprintf_s(outputbuf,"%i <%c%c%c%c> %d\n",(int)pos_start,*pid,*(pid+1),*(pid+2),*(pid+3),m_FS_NumSOBJ);
				TRACE(outputbuf);
			}
			break;

		}

		temp = (unsigned int) fp->GetPosition();
		if(temp>next_chunk)
		{
			ASSERT(FALSE);
			m_ErrorString=ERROR_FS_CHUNKDATAMISSING_TEXT;
			return ERROR_FS_CHUNKDATAMISSING;
		}
		if(next_chunk !=temp)
			fp->Seek(next_chunk,SEEK_SET);
	}

	//Check if valid
	if(!hasheader)
	{
		m_ErrorString=ERROR_FS_NOOHDRCHUNKS_TEXT;
		return ERROR_FS_NOOHDRCHUNKS;
	}
	if(!hasobjcts)
	{
		m_ErrorString=ERROR_FS_NOSOBJCHUNKS_TEXT;
		return ERROR_FS_NOSOBJCHUNKS;
	}

	TRACE("POF reading ok - Number of normals: %i\n",m_FS_Model.Ncount);

	FS_PreparePOF();

	return ERROR_GEN_NOERROR;
}

void CMODVIEW32Doc::FS_PreparePOF()
{
	//unsigned int i,j;

	m_ModelLoaded=TRUE;
	
	//Enumerate submodels and assign them to their detail mode
	for(int i=0;i<m_FS_NumSOBJ;i++)
		m_FS_SOBJ[i].detail=-1;
	for(unsigned int i=0;i<m_FS_PMinfo.n_detail;i++)
		m_FS_SOBJ[m_FS_PMinfo.detail[i]].detail=i;
	for(unsigned int i=0;i<m_FS_PMinfo.n_debris;i++)
		m_FS_SOBJ[m_FS_PMinfo.debris[i]].detail=9;
	for(int i=0;i<m_FS_NumSOBJ;i++)
	{
		if(m_FS_SOBJ[i].submodel_parent!=-1)
		{
			int j=i;
			while(m_FS_SOBJ[j].submodel_parent!=-1)
				j=m_FS_SOBJ[j].submodel_parent;
			m_FS_SOBJ[i].detail=m_FS_SOBJ[j].detail;
		}
	}

	//Prepare everything else
	for(int i=0;i<MAX_FS_SOBJ;i++)
	{
		m_FS_PofDataL[i]=0xFFFFFFFF;
		m_FS_PofDataH[i]=0;
	}
	for(unsigned int i=0;i<m_FS_Model.Pcount;i++)
	{
		if(m_FS_Model.Poly[i].Ptype==3)
		{
			if(m_FS_PofDataL[m_FS_SOBJ[m_FS_Model.Poly[i].Sobj].detail]>m_FS_Model.Poly[i].Colors)
				m_FS_PofDataL[m_FS_SOBJ[m_FS_Model.Poly[i].Sobj].detail]=m_FS_Model.Poly[i].Colors;
			if(m_FS_PofDataH[m_FS_SOBJ[m_FS_Model.Poly[i].Sobj].detail]<m_FS_Model.Poly[i].Colors)
				m_FS_PofDataH[m_FS_SOBJ[m_FS_Model.Poly[i].Sobj].detail]=m_FS_Model.Poly[i].Colors;
		}
	}
	//Count elements
	m_NumFilesize=(unsigned int) m_CurrentFile_Size; //HHTOTESTm_FS_CurrVP_Dir[m_FS_CurrVP_PofList[VpNum]].size;
	m_NumVertices=0;
	m_NumPolygons=0;
	m_NumTextures=0;
	m_NumSubmodels=0;

	int detaillevel=0; 
	for(unsigned int i=0;i<m_FS_Model.Vcount;i++)
	{
		if(m_FS_SOBJ[m_FS_Model.VSobj[i]].detail==detaillevel)
			m_NumVertices++;
	}
	for(unsigned int i=0;i<m_FS_Model.Pcount;i++)
	{
		if(m_FS_SOBJ[m_FS_Model.Poly[i].Sobj].detail==detaillevel)
			m_NumPolygons++;
	}
	for(unsigned int i=m_FS_PofDataL[0];i<(m_FS_PofDataH[0]+1);i++)
	{
		if(i<m_FS_BitmapData.count)
		{
			//for(m=0;m<m_FS_BitmapData.count;m++)
			//	if(m_FS_BitmapData.pic[m].valid == 1)
			m_NumTextures++;
		}
	}
	for(int i=0;i<m_FS_NumSOBJ;i++)
	{
		if(m_FS_SOBJ[i].detail==detaillevel)
			m_NumSubmodels++;
	}

	//Update display
	m_Game=GAME_FS;
	m_DoResetGeometry=TRUE;

	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
	viewFrame->SetTypeOfView(TOV_MODELVIEW);

	if(!m_InfoMode)
	{
		SetStatusBarText("Ready");
		FS_CalcDisplay();
	}
}

ERRORCODE CMODVIEW32Doc::FS_Walk(void *StartAt, int module, int StartByte)
{
	unsigned char *walker=(unsigned char *)StartAt;
	unsigned int i,j;
	ERRORCODE ret;

	while(wi(walker) !=D_EOF)
	{
		switch(wi(walker))
		{
		case D_EOF:					   // 00
			break;

		case D_DEFPOINTS:
			{			// 01
				unsigned int size=wi(walker+4);	  //block size
				unsigned char n_vn[MAX_FS_SUBMODEL_POINTS];
				unsigned int n=wi(walker+8);		//vertices
				unsigned int p=wi(walker+12);	//normals
				unsigned char *stepper=walker;
				stepper+=wi(walker+16);	 //offset   
				if(n>MAX_FS_SUBMODEL_POINTS)
				{
					m_ErrorString=ERROR_FS_HUGESUBMODEL_TEXT;
					m_ErrorDetails.Format("Is: %i, Should be <=%i",n,MAX_FS_SUBMODEL_POINTS);
					return ERROR_FS_HUGESUBMODEL;
				}

				for(i=0;i<n;i++)
					n_vn[i]=wc(walker+20+i);
				if((m_FS_Model.Vcount+n)>MAX_FS_VERTICES)
				{
					m_ErrorString=ERROR_FS_TOOHIGHVCOUNT_TEXT;
					m_ErrorDetails.Format("Should be <=%i",MAX_FS_VERTICES);
					return ERROR_FS_TOOHIGHVCOUNT;
				}
				if((m_FS_Model.Ncount+p)>MAX_FS_NORMALS)
				{
					m_ErrorString=ERROR_FS_TOOHIGHNCOUNT_TEXT;
					m_ErrorDetails.Format("Should be <=%i",MAX_FS_NORMALS);
					return ERROR_FS_TOOHIGHNCOUNT;
				}
				for(i=0;i<n;i++)
				{
					m_FS_Model.Vpoint[m_FS_Model.Vcount]=v(stepper);
					m_FS_Model.VSobj[m_FS_Model.Vcount]=module;
					m_FS_Model.Vpoint[m_FS_Model.Vcount].x+=m_FS_SOBJ[module].offset.x;
					m_FS_Model.Vpoint[m_FS_Model.Vcount].y+=m_FS_SOBJ[module].offset.y;
					m_FS_Model.Vpoint[m_FS_Model.Vcount].z+=m_FS_SOBJ[module].offset.z;
					m_FS_Model.Vcount++;
					stepper+=12;
					for(j=0;j<n_vn[i];j++)
					{
						m_FS_Model.Npoint[m_FS_Model.Ncount++]=v(stepper);
						stepper+=12;
					}
				}
				walker+=size;
				break;
			}

		case D_FLATPOLY:   {			// 02
				unsigned int n=wi(walker+36);
				if(n>20)
				{
					m_ErrorString=ERROR_FS_TOOMANYPOINTSINPOLY_TEXT;
					m_ErrorDetails.Format("Is: %i, Should be <=%i",n,20);
					return ERROR_FS_TOOMANYPOINTSINPOLY;
				}
				if(m_FS_Model.Pcount>=MAX_FS_POLYGONS)
				{
					m_ErrorString=ERROR_FS_TOOMANYPOLYGONS_TEXT;
					m_ErrorDetails.Format("Should be <=%i",MAX_FS_POLYGONS);
					return ERROR_FS_TOOMANYPOLYGONS;
				}

				m_FS_Model.Poly[m_FS_Model.Pcount].Ptype=2;
				m_FS_Model.Poly[m_FS_Model.Pcount].Corners=(short)n;
				m_FS_Model.Poly[m_FS_Model.Pcount].Normal=v(walker+8);
				m_FS_Model.Poly[m_FS_Model.Pcount].Colors=wi(walker + 40);
				m_FS_Model.Poly[m_FS_Model.Pcount].Sobj=module;
				for(i=0;i<n;i++)
				{
					m_FS_Model.Poly[m_FS_Model.Pcount].Vp[i]=ws(walker + 44 + (i*4));
					m_FS_Model.Poly[m_FS_Model.Pcount].Vp[i]+=StartByte;
					m_FS_Model.Poly[m_FS_Model.Pcount].Np[i]=ws(walker + 46 + (i*4));
				}
				m_FS_Model.Pcount++;
				walker+=44+n*4;
				m_Textures_Available=TRUE;
				break;
			}

		case D_TMAPPOLY:   {			// 03
				unsigned int n=wi(walker+36);
				if(n>20)
				{
					m_ErrorString=ERROR_FS_TOOMANYPOINTSINPOLY_TEXT;
					return ERROR_FS_TOOMANYPOINTSINPOLY;
				}
//TRACE(".%i.",	m_FS_Model.Pcount);
				if(m_FS_Model.Pcount>=MAX_FS_POLYGONS)
				{
					m_ErrorString=ERROR_FS_TOOMANYPOLYGONS_TEXT;
					return ERROR_FS_TOOMANYPOLYGONS;
				}

				m_FS_Model.Poly[m_FS_Model.Pcount].Ptype=3;
				m_FS_Model.Poly[m_FS_Model.Pcount].Corners=n;
				m_FS_Model.Poly[m_FS_Model.Pcount].Normal=v(walker+8);
				m_FS_Model.Poly[m_FS_Model.Pcount].Colors=wi(walker + 40);
				m_FS_Model.Poly[m_FS_Model.Pcount].Sobj=module;
				for(i=0;i<n;i++)
				{
					m_FS_Model.Poly[m_FS_Model.Pcount].Vp[i]=ws(walker + 44 + (i*12));
					m_FS_Model.Poly[m_FS_Model.Pcount].Vp[i]+=StartByte;
					m_FS_Model.Poly[m_FS_Model.Pcount].Np[i]=ws(walker + 46 + (i*12));
					m_FS_Model.Poly[m_FS_Model.Pcount].U[i]=wf(walker + 48 + (i*12));
					m_FS_Model.Poly[m_FS_Model.Pcount].V[i]=wf(walker + 52 + (i*12));
				}
				m_FS_Model.Pcount++;
				walker+=44 + n*12;
				break;
			}

		case D_SORTNORM:
			{			// 04
				ret=FS_Walk(walker+wi(walker+36),module,StartByte);
				if(ret==ERROR_GEN_NOERROR)
					ret=FS_Walk(walker+wi(walker+40),module,StartByte);
				if(ret!=ERROR_GEN_NOERROR)
					return ret;
				walker+=80;
				break;
			}

		case D_BOUNDBOX:  
			{			// 05
				if(m_FS_Model.BBcount>=MAX_FS_BBOXS)
				{
					m_ErrorString=ERROR_FS_TOOMANYBBOXS_TEXT;
					m_ErrorDetails.Format("Should be <=%i",ERROR_FS_TOOMANYBBOXS_TEXT);
					return ERROR_FS_TOOMANYBBOXS;
				}
				m_FS_Model.Bbox[m_FS_Model.BBcount].min=v(walker+8);
				m_FS_Model.Bbox[m_FS_Model.BBcount].min=v(walker+20);
				m_FS_Model.BBcount++;
				walker+=32;
				break;
			}

		default:
			m_ErrorString=ERROR_FS_BADPOLYMODELDATA_TEXT;
			return ERROR_FS_BADPOLYMODELDATA;
		}
	}
	return ERROR_GEN_NOERROR;
}


ERRORCODE CMODVIEW32Doc::FS_InitVP(CFile *fp,VP_HEADER *vp_header,VP_INFO *vp_info,BOOL fillexplorer)
{
	unsigned int i;   
	char *pntptr;
	ERRORCODE errret=ERROR_GEN_NOERROR;

	fp->SeekToBegin();
	fp->Read(vp_header,sizeof(VP_HEADER));

	if(!m_InfoMode)
	{
		char str[256];
		sprintf_s(str,"Listing %i files...",vp_header->dirnumber);
		SetStatusBarText(str);
	}

	fp->Seek(vp_header->diroffset,SEEK_SET);
	if(vp_header->dirnumber>MAX_FS_FILESINVP)
	{
		m_ErrorString=ERROR_FS_VPTOOMANYFILES_TEXT;
		m_ErrorDetails.Format("Is: %i, Should be <=%i",vp_header->dirnumber,MAX_FS_FILESINVP);
		return ERROR_FS_VPTOOMANYFILES;
	}

	VP_INFO temp[MAX_FS_FILESINVP];
	for(i=0;i<vp_header->dirnumber;i++)
		fp->Read(&temp[i], sizeof(VP_INFO));

	if(fillexplorer)
	{
		m_ArchiveStats_NumTotal=0;
		m_ArchiveStats_NumLores=-1;
		m_ArchiveStats_NumHires=-1;
		m_ArchiveStats_NumOther=0;

		char tempfname[8192];
		strcpy_s(tempfname,fp->GetFileName());
		ExplorerClear();
		ExplorerAddRoot((char *)&tempfname);

		m_FS_CurrVP_PofNum=0;
		for(i=0;i<vp_header->dirnumber;i++)
		{
			if((pntptr=strrchr(temp[i].filename,'.')) !=NULL)
			{
				if(_strcmpi(pntptr,".pof")==0)
				{
					m_ArchiveStats_NumTotal++;
					m_FS_CurrVP_PofList[m_FS_CurrVP_PofNum]=i;
					if(m_FS_CurrVP_PofNum<MAX_FS_POFFILESINVP)
					{
						m_FS_CurrVP_PofNum++;
						memset(tempfname,0,36);
						strncpy_s(tempfname,temp[i].filename,strlen(temp[i].filename)-4); //drop the ".pof"
						ExplorerAddChild(tempfname);
					}
					else
					{
						errret=ERROR_FS_VPTOOMANYPOFS;
						m_ErrorDetails.Format("Should be <=%i",MAX_FS_POFFILESINVP);
						m_ErrorString=ERROR_FS_VPTOOMANYPOFS_TEXT;
					}
				}
				else
					m_ArchiveStats_NumOther++;
			}
		}
		ExplorerGoTop();
	}

	memcpy(vp_info,temp,sizeof(VP_INFO)*4000);
	return errret;
}

void CMODVIEW32Doc::FS_CalcDisplay()
{
	float Min=0,Max=0;
	for(unsigned int i=0;i<m_FS_Model.Vcount;i++) 
	{
		if(m_FS_Model.Vpoint[i].x>Max) Max=m_FS_Model.Vpoint[i].x;
		if(m_FS_Model.Vpoint[i].x<Min) Min=m_FS_Model.Vpoint[i].x;
		if(m_FS_Model.Vpoint[i].y>Max) Max=m_FS_Model.Vpoint[i].y;
		if(m_FS_Model.Vpoint[i].y<Min) Min=m_FS_Model.Vpoint[i].y;
		if(m_FS_Model.Vpoint[i].z>Max) Max=m_FS_Model.Vpoint[i].z;
		if(m_FS_Model.Vpoint[i].z<Min) Min=m_FS_Model.Vpoint[i].z;
	}
	Min=(float)abs((int)Min);
	if(Min>Max) Max=Min;
	m_FS_Model.MaxXYZ=Max;
}

ERRORCODE CMODVIEW32Doc::FS_LoadVPContent(CString filename)
{
	int VpPof=-1;
	char *pntptr;
	unsigned int i;   
	unsigned int VpPofNum=0;

	m_TexturesNum=-1;
	m_ModelJustLoaded=TRUE;

	for(i=0;i<m_FS_CurrVP_Header.dirnumber;i++)
	{
		if((pntptr=strrchr(m_FS_CurrVP_Dir[i].filename,'.')) !=NULL)
		{
			if(_strcmpi(pntptr,".pof")==0)
			{
				if(_strcmpi(m_FS_CurrVP_Dir[i].filename,filename)==0) 
					VpPof=VpPofNum++;
				VpPofNum++;
			}
		}
	}

	if(VpPof==-1)
	{
		ClearDocument();
		m_ErrorString=ERROR_GEN_NOMODELSELECTED_TEXT;
		return ERROR_GEN_NOMODELSELECTED;
	}

	m_CurrentFile_FileName=filename;
	CFile fp;
	fp.Open(m_FS_CurrVP_Filename,CFile::modeRead);
	m_ErrorCode=FS_ReadPOF(&fp,VpPof);
	fp.Close();
	
	if(m_ErrorCode==ERROR_GEN_NOERROR)
		FS_LoadPCXData2();

	if(!m_InfoMode)
	{
		POSITION pos=GetFirstViewPosition();	
		CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
		CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
		m_view=viewFrame->GetCurrentView();
		
		m_view->ChangeSize();
		viewFrame->GameBarFS();
		viewFrame->UpdateTextureBar();
		viewFrame->UpdateMainDisplay();
		UpdateAllViews(NULL);
	}
	
	return m_ErrorCode;
}

void CMODVIEW32Doc::FS_LoadPCXData2()
{
	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
	CMODVIEW32View* pView=viewFrame->GetCurrentView();

	pView->FS_SetDetailLevel(0);
}

//Read main VP into memory (game=1 for FS1, game=2 for FS2)
void CMODVIEW32Doc::FS_ReadFSVP(int game)
{
	COptionsDlg dlg; //we do not actually show or even create the OptionsDlg, we just use some routines from it (GetF1Path and GetF2Path)
	
	if(!m_FS_MainVP_Loaded)
		m_FS_MainVP_MainTextureSet=VP_FS_TEXTURESET_NONE;

	switch(game)
	{
	case 1:
		if(m_FS_MainVP_MainTextureSet==VP_FS_TEXTURESET_FS1)
			return; //Already in memory

		if(!m_InfoMode)
			SetStatusBarText("Listing FreeSpace 1 VP content...");
		m_FS_MainVP_Filename=dlg.GetF1Path();
		if(m_FS_MainVP_Filename!="")
			m_FS_MainVP_Filename+="DATA\\FREESPACE.VP";
		m_FS_MainVP_MainTextureSet=VP_FS_TEXTURESET_FS1;
		break;

	case 2:
		if(m_FS_MainVP_MainTextureSet==VP_FS_TEXTURESET_FS2)
			return; //Already in memory

		if(!m_InfoMode)
			SetStatusBarText("Listing FreeSpace 2 VP content...");
		m_FS_MainVP_Filename=dlg.GetF2Path();
		if(m_FS_MainVP_Filename!="")
			m_FS_MainVP_Filename+="SPARKY_FS2.VP";
		m_FS_MainVP_MainTextureSet=VP_FS_TEXTURESET_FS2;
		break;

	default:
		TRACE("CMODVIEW32Doc::ReadFSVP: Unknown FreeSpace version!\n");
		return;
	}
	
	m_FS_MainVP_Loaded=FALSE;
	if(strlen(m_FS_MainVP_Filename)>0)
	{
		CFile fp;
		if(fp.Open(m_FS_MainVP_Filename,CFile::modeRead | CFile::shareDenyNone))
		{
			FS_InitVP(&fp,&m_FS_MainVP_Header,(VP_INFO *)&m_FS_MainVP_Dir,FALSE);
			m_FS_MainVP_Loaded=TRUE;
			fp.Close();
		}
		else
		{
			//Oooops, the file actually did not exist
			m_FS_MainVP_MainTextureSet=VP_FS_TEXTURESET_NONE;
			m_FS_MainVP_Filename="";
		}
	}

	if(!m_InfoMode)
		SetStatusBarText("Ready");		
}

CString CMODVIEW32Doc::FS_MakeSubsystemDesc(CString x1, CString x2)
{
	CString x3;
	if(x1.Left(1)=="$")
		x1=x1.Mid(1);
	x3=x1.Left(1);
	x3.MakeUpper();
	x3+=x1.Mid(1);
	if(x2.Find("subsystem")==-1)
		x3+=" (hidden)";
	return x3;
}

void CMODVIEW32Doc::FS_BuildGunGroups()
{
	m_NumGunsGroups=0;
	for(int i=0;i<m_Guns.Num;i++)
	{
		int actgroup=-1;
		for(int j=0;j<m_NumGunsGroups;j++)
		{
			if(m_GunsGroup[j].GunType==m_Guns.GunType[i] && m_GunsGroup[j].GunBank==m_Guns.GunBank[i])
				actgroup=j;
		}
		if(actgroup==-1)
		{
			m_GunsGroup[m_NumGunsGroups].GunType=m_Guns.GunType[i];
			m_GunsGroup[m_NumGunsGroups].GunBank=m_Guns.GunBank[i];
			m_GunsGroup[m_NumGunsGroups].Num=1;
			m_NumGunsGroups++;
		}
		else
			m_GunsGroup[actgroup].Num++;
	}
}


////////////////////////////////////////////////////////////////////////////////////////
// Descent 3 stuff
////////////////////////////////////////////////////////////////////////////////////////

//Read main D3 HOG into memory
//void CMODVIEW32Doc::D3_ReadD3HOG()
//{
//	COptionsDlg dlg; //we do not actually show or even create the OptionsDlg, we just use some routines from it (GetD3Path)
//
//	if(m_D3_MainHOG_MainTextureSet==1)
//		return; //Already in memory
//
//	if(!m_InfoMode)
//		SetStatusBarText("Listing Descent 3 HOG content...");
//	m_D3_MainHOG_Filename=dlg.GetD3Path();
//	if(m_D3_MainHOG_Filename!="")
//		m_D3_MainHOG_Filename+="D3.HOG";
//	m_D3_MainHOG_MainTextureSet=1;
//	
//	m_D3_MainHOG_Loaded=FALSE;
//	if(m_D3_MainHOG_Filename!="")
//	{
//		CFile fp;
//
//		if(fp.Open(m_D3_MainHOG_Filename,CFile::modeRead | CFile::shareDenyNone))
//		{
//			D3_InitHOG(&fp,&m_D3_MainHOG_Header,(HOG2_LOAD *)&m_D3_MainHOG_Dir,FALSE);
//			m_D3_MainHOG_Loaded=TRUE;
//			fp.Close();   
//		}
//	}
//
//	if(!m_InfoMode)
//		SetStatusBarText("Ready");		
//}
//
//
////Read OOF file
//ERRORCODE CMODVIEW32Doc::D3_ReadOOF(CFile *fp,int OofNum)
//{
//	unsigned int long id,len,next_chunk,btemp;
//	DWORD ooflength;
//	unsigned int long version;
//	float ftemp;
//	char *pid;
//	int OffsetStart;
//
//	ClearDocument(FALSE);
//
//	//Prepare log file
//	CStdioFile flog;
//	CString logtext;
//	flog.Open(((CMODVIEW32App *)AfxGetApp())->m_LogFile,CFile::modeWrite | CFile::modeCreate);
//
//	if(!m_InfoMode)
//		SetStatusBarText("Loading OOF...");
//
//	if(m_D3_CurrHOG_OofNum==0)
//	{
//		//Plain OOF file
//		fp->Seek(0L, SEEK_END);
//		ooflength=(DWORD)fp->GetPosition();
//		OffsetStart=0;
//	} else {
//		//Inside a HOG
//		OffsetStart=m_D3_CurrHOG_Dir[m_D3_CurrHOG_OofList[OofNum]].offset;
//		ooflength=m_D3_CurrHOG_Dir[m_D3_CurrHOG_OofList[OofNum]].offset + m_D3_CurrHOG_Dir[m_D3_CurrHOG_OofList[OofNum]].size;
//	}
//	fp->Seek(OffsetStart, SEEK_SET);
//
//
//	id=read_INT32(fp);
//	if(id !=ID_PSPO)
//	{
//		m_ErrorString=ERROR_D3_INVALIDOOFHEADER_TEXT;
//		return ERROR_D3_INVALIDOOFHEADER;
//	}
//	version=read_INT32(fp);
//	m_FileVersion=(int)version;
//
//	LOGTEXT("File %s at offset %i\n",fp->GetFilePath(),OffsetStart); LOGDOIT;
//	LOGTEXT("File version %i\n",version); LOGDOIT;
//
//	while(ooflength>fp->GetPosition())
//	{
//		id=read_INT32(fp);
//		len=read_INT32(fp);
//		next_chunk = (unsigned long) fp->GetPosition() + len;
//
//		pid=(char *)&id;
//		m_Blocks_Num++;
//		m_Blocks[m_Blocks_Num-1].name.Format("%c%c%c%c",*pid,*(pid+1),*(pid+2),*(pid+3));
//		m_Blocks[m_Blocks_Num-1].offset=(int)fp->GetPosition()-OffsetStart;
//		m_Blocks[m_Blocks_Num-1].size=len;
//		m_Blocks[m_Blocks_Num-1].desc="??? <Unknown>";
//
//		switch(id)
//		{
//		
//		case ID_TXTR: {		//Texture filename list
//				m_Blocks[m_Blocks_Num-1].desc="Texture information";
//				unsigned int charcount;
//				unsigned char data;
//
//				m_D3_TotalTexture=read_INT32(fp);
//				if(m_D3_TotalTexture>MAX_D3_TEXTURE)
//				{
//					m_ErrorString=ERROR_D3_TOOMANYTEXTURES_TEXT;
//					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_D3_TotalTexture,MAX_D3_TEXTURE);
//					return ERROR_D3_TOOMANYTEXTURES;
//				}
//				for(unsigned int i=0;i<m_D3_TotalTexture;i++)
//				{
//					m_D3_Display.Used_texture[i]=FALSE;
//					charcount=read_INT32(fp);    
//					if(charcount>36)
//					{
//						m_ErrorString=ERROR_D3_INVALIDTEXTURENAME_TEXT;
//						m_ErrorDetails.Format("Is: %i, Should be <=%i",charcount,36);
//						return ERROR_D3_INVALIDTEXTURENAME;
//					}
//					for(unsigned int j=0;j<charcount;j++)
//					{
//						data=read_INT8(fp);
//						m_D3_TextureList[i][j]=data;
//					}
//				}
//				break;
//			}
//
//		case ID_OHDR: {		//Object header
//				m_Blocks[m_Blocks_Num-1].desc="Object header";
//				btemp=read_INT32(fp);       //n_subobj
//				ftemp=read_float(fp);          //rad
//				m_D3_Model._minbox=(vms_vector)read_VECTOR(fp); //pmmin
//				m_D3_Model._maxbox=read_VECTOR(fp); //pmmax
//				ftemp=read_float(fp);            //unknown
//				break;
//			}
//
//		case ID_SOBJ: {		//Subobject header
//				unsigned int n_verts,n_txtr,n_pnts;
//				int n_faces;
//				char input_char;
//				unsigned int Vstart,StrTotal;
//
//				Vstart=m_D3_Model.Vcount;
//
//				btemp=read_INT32(fp);	   //submodel
//				m_D3_Model.Sparent[m_D3_Model.Scount]=read_INT32(fp);	 //parent
//
//				ftemp=read_float(fp);	   //unknown1
//				ftemp=read_float(fp);	   //unknown2
//				ftemp=read_float(fp);	   //unknown3
//				ftemp=read_float(fp);	   //unknown4
//				ftemp=read_float(fp);	   //unknown5
//				ftemp=read_float(fp);	   //unknown6
//				ftemp=read_float(fp);	   //unknown7
//
//				m_D3_Model.Soffset[m_D3_Model.Scount]=read_VECTOR(fp);	//Offset
//
//				ftemp=read_float(fp);	   //unknown1
//				ftemp=read_float(fp);	   //unknown2
//				ftemp=read_float(fp);	   //unknown3
//				ftemp=read_float(fp);	   //unknown4
//				ftemp=read_float(fp);	   //unknown5
//				ftemp=read_float(fp);	   //unknown6
//
//				long subobj_name = read_INT32(fp);		   //subobj name
//				StrTotal=0;
//				for(long j=0;j<subobj_name;j++)
//				{
//					input_char=read_INT8(fp);
//					m_D3_Model.Name[m_D3_Model.Scount][StrTotal++]=input_char;
//				}
//				m_D3_Model.Name[m_D3_Model.Scount][StrTotal]=0;
//				m_Blocks[m_Blocks_Num-1].desc.Format("Subobject \"%s\"",m_D3_Model.Name[m_D3_Model.Scount]);
//
//				long subobj_property = read_INT32(fp);		   //subobj properites
//				StrTotal=0;
//				for(long j=0;j<subobj_property;j++)
//				{
//					input_char=read_INT8(fp);
//					m_D3_Model.Prop[m_D3_Model.Scount][StrTotal++]=input_char;
//				}
//				m_D3_Model.Prop[m_D3_Model.Scount][StrTotal]=0;
//
//
//				btemp=read_INT32(fp);	  //unknown
//				btemp=read_INT32(fp);	  //unknown
//				btemp=read_INT32(fp);	  //reserve 0
//
//				n_verts=read_INT32(fp); //n_verts/norms
//				m_D3_Display.SvertNum[m_D3_Model.Scount]=n_verts;
//				if((m_D3_Model.Vcount+n_verts)>MAX_D3_VECT)
//				{
//						m_ErrorString=ERROR_D3_TOOHIGHVCOUNT_TEXT;
//						m_ErrorDetails.Format("Is: %i, Should be <=%i",m_D3_Model.Vcount+n_verts,MAX_D3_VECT);
//						return ERROR_D3_TOOHIGHVCOUNT;
//				}
//				btemp=m_D3_Model.Vcount;		//save Vcount..., use same index for normals and alphas
//				for(unsigned int i=0;i<n_verts;i++)
//				{	 //vector
//					m_D3_Model.Vpoint[m_D3_Model.Vcount]=read_VECTOR(fp);
//					m_D3_Model.VSobj[m_D3_Model.Vcount]=(unsigned short)m_D3_Model.Scount;
//					m_D3_Model.Vcount++;
//				}
//				for(unsigned int i=0;i<n_verts;i++) //normal
//					m_D3_Model.Vnormal[btemp+i]=read_VECTOR(fp);
//				if(version==2300)
//				{
//					for(unsigned int i=0;i<n_verts;i++) //unknown
//						m_D3_Model.Valpha[btemp+i]=read_float(fp);	  
//				}
//
//				n_faces=read_INT32(fp);	  //n_faces
//				if(m_D3_Model.Pcount>MAX_D3_POLY)
//				{
//					m_ErrorString=ERROR_D3_TOOMANYPOLYGONS_TEXT;
//					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_D3_Model.Pcount,MAX_D3_POLY);
//					return ERROR_D3_TOOMANYPOLYGONS;
//				}
//
//				m_D3_Display.SpolyNum[m_D3_Model.Scount]=n_faces;
//
//				for(int i=0;(i<n_faces) && (i < MAX_D3_POLY);i++)
//				{
//					if (m_D3_Model.Pcount < MAX_D3_POLY) {
//						m_D3_Model.Poly[m_D3_Model.Pcount].Sobj=m_D3_Model.Scount;
//						m_D3_Model.Poly[m_D3_Model.Pcount].Normal=read_VECTOR(fp);	   //normal
//
//						n_pnts=read_INT32(fp);	  //n_pnts
//						m_D3_Model.Poly[m_D3_Model.Pcount].Corners=n_pnts;
//						if(n_pnts>25)
//						{
//							m_ErrorString=ERROR_D3_TOOMANYPOINTSINPOLY_TEXT;
//							m_ErrorDetails.Format("Is: %i, Should be <=%i",n_pnts,25);
//							return ERROR_D3_TOOMANYPOINTSINPOLY;
//						}
//
//						n_txtr=read_INT32(fp);	  //n_txtr
//					//if(n_txtr>1)  //The following line was commented out which caused the next line
//									// to become part of the if clause. As  a result the Poly[].type
//									// was not initialized.  So this line was meant as an error catch
//									// so I commented it out...(SteveKlinger)
//						//AfxMessageBox("n_txtr>1...","INFO",MB_OK);
//
//						m_D3_Model.Poly[m_D3_Model.Pcount].Type=n_txtr;
//						if(n_txtr==0)
//						{
//							m_D3_Model.Poly[m_D3_Model.Pcount].Red=read_INT8(fp);
//							m_D3_Model.Poly[m_D3_Model.Pcount].Green=read_INT8(fp);
//							m_D3_Model.Poly[m_D3_Model.Pcount].Blue=read_INT8(fp);
//						} else {
//							m_D3_Model.Poly[m_D3_Model.Pcount].Color=read_INT32(fp);                    
//							m_D3_Display.Used_texture[m_D3_Model.Poly[m_D3_Model.Pcount].Color]=TRUE;
//						}
//
//						for(unsigned int j=0;j<n_pnts;j++)
//						{
//							m_D3_Model.Poly[m_D3_Model.Pcount].Vp[j]=read_INT32(fp);	   //index
//							m_D3_Model.Poly[m_D3_Model.Pcount].Vp[j]+=Vstart;     
//							m_D3_Model.Poly[m_D3_Model.Pcount].U[j]=read_float(fp);		  //u
//							m_D3_Model.Poly[m_D3_Model.Pcount].V[j]=read_float(fp);		  //v
//						}
//					}
//					if(version !=1907)
//					{
//						ftemp=read_float(fp);		   //b length?
//						ftemp=read_float(fp);		   //b width?
//					}
//					m_D3_Model.Pcount++;
//				}
//				m_D3_Model.Scount++;
//				break;
//			}
//
//
//		case ID_PANI:
//			{
//				m_Blocks[m_Blocks_Num-1].desc="Poly animation frames";
//				unsigned int k_count,total;
//
//				total=0;
//				for(unsigned int i=0;i<m_D3_Model.Scount;i++)
//				{
//					k_count=read_INT32(fp);
//					m_D3_Model.pknum[i]=k_count;
//					m_D3_Model.pindex[i]=total;
//					btemp=read_INT32(fp);
//					btemp=read_INT32(fp);
//					total=total + k_count;
//					if(total>MAX_D3_ANI)
//					{
//						m_ErrorString=ERROR_D3_TOOMANYANINUMBERKEYSTHRUSTERS_TEXT;
//						m_ErrorDetails.Format("Block \"PANI\" - Is: %i, Should be <=%i",total,MAX_D3_ANI);
//						return ERROR_D3_TOOMANYANINUMBERKEYSTHRUSTERS;
//					}
//					for(unsigned int j=0;j<k_count;j++)
//					{
//						btemp=read_INT32(fp);
//						if ((m_D3_Model.pindex[i] + j) < MAX_D3_ANI) {
//							m_D3_Model.pkey[m_D3_Model.pindex[i] + j]=btemp;
//							m_D3_Model.pani[m_D3_Model.pindex[i] + j]=read_VECTOR(fp);
//						}
//						m_D3_Display.pr_key[btemp]=TRUE;
//					}
//				}
//				int j=0;
//				for(int i=0;i<MAX_D3_ANI;i++)
//				{
//					if(m_D3_Display.pr_key[i])
//						m_D3_Display.pr_active[j++]=i;
//				}
//				m_D3_Display.TotalKey=j;
//				break;
//			}
//
//		case ID_RANI: {
//				m_Blocks[m_Blocks_Num-1].desc="Rotational animation frames";
//				unsigned int k_count,total;
//
//				total=0;
//				for(unsigned int i=0;i<m_D3_Model.Scount;i++)
//				{
//					k_count=read_INT32(fp);
//					m_D3_Model.rknum[i]=k_count;
//					m_D3_Model.rindex[i]=total;
//					btemp=read_INT32(fp);
//					btemp=read_INT32(fp);
//					total=total + k_count;
//					if(total>MAX_D3_ANI)
//					{
//						m_ErrorString=ERROR_D3_TOOMANYANINUMBERKEYSTHRUSTERS_TEXT;
//						m_ErrorDetails.Format("Block \"RANI\" - Is: %i, Should be <=%i",total,MAX_D3_ANI);
//						return ERROR_D3_TOOMANYANINUMBERKEYSTHRUSTERS;
//					}
//					for(unsigned int j=0;j<k_count;j++)
//					{
//						btemp=read_INT32(fp);
//						if (m_D3_Model.rindex[i] + j < MAX_D3_ANI) {
//							m_D3_Model.rkey[m_D3_Model.rindex[i] + j]=btemp;
//							m_D3_Model.rani[m_D3_Model.rindex[i] + j]=read_VECTOR(fp);
//							m_D3_Model.rangle[m_D3_Model.rindex[i] + j]=read_INT32(fp);
//						}
//						m_D3_Display.pr_key[btemp]=TRUE;
//					}
//				}
//				int j=0;
//				for(int i=0;i<MAX_D3_ANI;i++)
//				{
//					if(m_D3_Display.pr_key[i])
//						m_D3_Display.pr_active[j++]=i;
//				}
//				m_D3_Display.TotalKey=j;
//				break;
//			}
//
//
//		case ID_ATCH: {
//				m_Blocks[m_Blocks_Num-1].desc="Attachment points (used for flags in CTF)";
//
//				m_D3_Model.ATCHnum=read_INT32(fp);
//				LOGTEXT("ATCH --> %i attachment points\n",m_D3_Model.ATCHnum); LOGDOIT;
//				if(m_D3_Model.ATCHnum>MAX_D3_PNT)
//				{
//					m_ErrorString=ERROR_D3_TOOMANYATCHPOINTS_TEXT;
//					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_D3_Model.ATCHnum,MAX_D3_PNT);
//					return ERROR_D3_TOOMANYATCHPOINTS;
//				}
//				for(unsigned int i=0;i<m_D3_Model.ATCHnum;i++)
//				{
//					m_D3_Model.ATCHsobj[i]=read_INT32(fp);	  //goes with sobj...
//					m_D3_Model.ATCHpos[i]=read_VECTOR(fp);	  //Position
//					m_D3_Model.ATCHnorm[i]=read_VECTOR(fp);	  //Normal
//				}
//				break;
//			}
//
//		case ID_NATH: {
//				m_Blocks[m_Blocks_Num-1].desc="Normals for attachment points";
//
//				m_D3_Model.NATHnum=read_INT32(fp);
//				LOGTEXT("NATH --> %i attachment normals\n",m_D3_Model.NATHnum); LOGDOIT;
//				if(m_D3_Model.NATHnum>MAX_D3_PNT)
//				{
//					m_ErrorString=ERROR_D3_TOOMANYNATHPOINTS_TEXT;
//					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_D3_Model.NATHnum,MAX_D3_PNT);
//					return ERROR_D3_TOOMANYNATHPOINTS;
//				}
//				for(unsigned int i=0;i<m_D3_Model.NATHnum;i++)
//				{
//					m_D3_Model.NATHsobj[i]=read_INT32(fp);	  //goes with sobj...
//					m_D3_Model.NATHpos[i]=read_VECTOR(fp);	  //Position
//					m_D3_Model.NATHnorm[i]=read_VECTOR(fp);	  //Normal
//				}
//				break;
//			}
//
//		case ID_GRND: {
//				m_Blocks[m_Blocks_Num-1].desc="Grounding points";
//
//				m_D3_Model.GRNDnum=read_INT32(fp);
//				LOGTEXT("GRND --> %i grounding points\n",m_D3_Model.GRNDnum); LOGDOIT;
//				if(m_D3_Model.GRNDnum>MAX_D3_PNT)
//				{
//					m_ErrorString=ERROR_D3_TOOMANYGRNDPOINTS_TEXT;
//					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_D3_Model.GRNDnum,MAX_D3_PNT);
//					return ERROR_D3_TOOMANYGRNDPOINTS;
//				}
//				for(unsigned int i=0;i<m_D3_Model.GRNDnum;i++)
//				{
//					m_D3_Model.GRNDsobj[i]=read_INT32(fp);	  //goes with sobj...
//					m_D3_Model.GRNDpos[i]=read_VECTOR(fp);	  //Position
//					m_D3_Model.GRNDnorm[i]=read_VECTOR(fp);	  //Normal
//				}
//				break;
//			}
//
//		case ID_GPNT: {
//				m_Blocks[m_Blocks_Num-1].desc="Gun points";
//
//				m_D3_Model.GPNTnum=read_INT32(fp);
//				LOGTEXT("GPNT --> %i gun points\n",m_D3_Model.ATCHnum); LOGDOIT;
//				if(m_D3_Model.GPNTnum>MAX_D3_PNT)
//				{
//					m_ErrorString=ERROR_D3_TOOMANYGPNTPOINTS_TEXT;
//					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_D3_Model.GPNTnum,MAX_D3_PNT);
//					return ERROR_D3_TOOMANYGPNTPOINTS;
//				}
//
//				ASSERT(MAX_GUNS>=MAX_D3_PNT);
//				m_Guns.Num=m_D3_Model.GPNTnum;
//				if(m_Guns.Num>=MAX_GUNS)
//				{
//					m_ErrorString=ERROR_GEN_TOOMANYGUNS_TEXT;
//					m_ErrorDetails.Format("Exceeded %i.",MAX_GUNS);
//					return ERROR_GEN_TOOMANYGUNS;
//				}
//				for(unsigned int i=0;i<m_D3_Model.GPNTnum;i++)
//				{
//					m_D3_Model.GPNTsobj[i]=read_INT32(fp);	  //goes with sobj...
//					m_D3_Model.GPNTpos[i]=read_VECTOR(fp);	  //Position
//					m_D3_Model.GPNTnorm[i]=read_VECTOR(fp);	  //Normal
//
//					m_Guns.InSubModel[i]=m_D3_Model.GPNTsobj[i];
//					m_Guns.Gun[i]=m_D3_Model.GPNTpos[i];
//					m_Guns.GunDirection[i]=m_D3_Model.GPNTnorm[i];
//				}
//				break;
//			}
//
//
//
//		case ID_SPCL: {
//				m_Blocks[m_Blocks_Num-1].desc="Special points";
//				unsigned int charcount,StrTotal;
//				char input_char;
//
//				m_D3_Model.SPCLnum=read_INT32(fp);
//				LOGTEXT("SPCL --> %i special points\n",m_D3_Model.SPCLnum); LOGDOIT;
//				if(m_D3_Model.SPCLnum>MAX_D3_PNT)
//				{
//					m_ErrorString=ERROR_D3_TOOMANYSPCLPOINTS_TEXT;
//					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_D3_Model.SPCLnum,MAX_D3_PNT);
//					return ERROR_D3_TOOMANYSPCLPOINTS;
//				}
//				for(unsigned int i=0;i<m_D3_Model.SPCLnum;i++)
//				{
//					charcount=read_INT32(fp);
//					StrTotal=0;
//					for(unsigned int j=0;j<charcount;j++)
//					{
//						input_char=read_INT8(fp);
//						m_D3_Model.SPCLname[i][StrTotal++]=input_char;
//					}
//					m_D3_Model.SPCLname[i][StrTotal]=0;					
//					TRACE("Special point: %s, ",m_D3_Model.SPCLname[i]);
//
//					charcount=read_INT32(fp);
//					StrTotal=0;
//					for(unsigned int j=0;j<charcount;j++)
//					{
//						input_char=read_INT8(fp);
//						m_D3_Model.SPCLprop[i][StrTotal++]=input_char;
//					}
//					m_D3_Model.SPCLprop[i][StrTotal]=0;
//					TRACE("parameter: %s\n",m_D3_Model.SPCLprop[i]);
//					LOGTEXT(" Special point #%i: name=%s, properties=%s\n",i,m_D3_Model.SPCLname[i],m_D3_Model.SPCLprop[i]); LOGDOIT;
//
//
//					m_D3_Model.SPCLpos[i]=read_VECTOR(fp);
//					m_D3_Model.SPCLrad[i]=read_float(fp);
//				}
//				break;
//			}
//
//
//
//		case ID_WBAT: {
//				m_Blocks[m_Blocks_Num-1].desc="Weapon batteries";
//
//				m_D3_Model.WBATnum=read_INT32(fp);
//				LOGTEXT("WBAT --> %i weapon batteries\n",m_D3_Model.WBATnum); LOGDOIT;
//				if(m_D3_Model.WBATnum>MAX_D3_PNT)
//				{
//					m_ErrorString=ERROR_D3_TOOMANYWBATPOINTS_TEXT;
//					m_ErrorDetails.Format("Is: %i, Should be <=%i",m_D3_Model.WBATnum,MAX_D3_PNT);
//					return ERROR_D3_TOOMANYWBATPOINTS;
//				}
//				for(unsigned int i=0;i<m_D3_Model.WBATnum;i++)
//				{
//					m_D3_Model.WBATnumgun[i]=read_INT32(fp);
//					if(m_D3_Model.WBATnumgun[i]>MAX_D3_PNT)
//					{
//						m_ErrorString=ERROR_D3_TOOMANYWBATNUMGUNPOINTS_TEXT;
//						m_ErrorDetails.Format("Weapon battery #%i - Is: %i, Should be <=%i",i,m_D3_Model.WBATnumgun[i],MAX_D3_PNT);
//						return ERROR_D3_TOOMANYWBATNUMGUNPOINTS;
//					}
//					for(unsigned int j=0;j<m_D3_Model.WBATnumgun[i];j++)
//						m_D3_Model.WBATgun[i][j]=read_INT32(fp);		//gun
//					m_D3_Model.WBATnumeye[i]=read_INT32(fp);
//					if(m_D3_Model.WBATnumeye[i]>MAX_D3_PNT)
//					{
//						m_ErrorString=ERROR_D3_TOOMANYWBATNUMEYEPOINTS_TEXT;
//						m_ErrorDetails.Format("Weapon battery #%i - Is: %i, Should be <=%i",i,m_D3_Model.WBATnumeye[i],MAX_D3_PNT);
//						return ERROR_D3_TOOMANYWBATNUMEYEPOINTS;
//					}
//					for(unsigned int j=0;j<m_D3_Model.WBATnumeye[i];j++)
//						m_D3_Model.WBATeye[i][j]=read_INT32(fp);		//eye sobj
//				}
//				break;
//			}
//
//		case ID_PINF: {
//				m_Blocks[m_Blocks_Num-1].desc="OOF compiler information (not used)";
//				if(len>MAX_D3_PINF)
//				{
//					m_ErrorString=ERROR_D3_PINFTEXTTOOLONG_TEXT;
//					m_ErrorDetails.Format("Is: %i, Should be <=%i",len,MAX_D3_PINF);
//					return ERROR_D3_PINFTEXTTOOLONG;
//				}
//				fp->Read(&m_D3_Model.PINF, (UINT)len);
//				m_D3_Model.PINF[len]=0;
//				LOGTEXT("--------------------------------------------------\nPINF --> OOF compiler information (not used):\n"); LOGDOIT;
//				LOGTEXT("%s",m_D3_Model.PINF); LOGDOIT;
//				break;
//			}
//
//
//		default: {
//				pid=(char *)&id;
//				m_ErrorDetails.Format("Unknown block \"%c%c%c%c\"",*pid,*(pid+1),*(pid+2),*(pid+3));
//				m_ErrorString=ERROR_D3_INVALIDCHUNK_TEXT;
//				return ERROR_D3_INVALIDCHUNK;
//			}
//
//		}
//
//
//		if(next_chunk !=(DWORD)fp->GetPosition())
//			fp->Seek(next_chunk,CFile::begin);
//	}
//	if(!m_InfoMode)
//		D3_CalcDisplay();
//	m_ModelLoaded=TRUE;
//	m_Game=GAME_D3;
//
//	m_DoResetGeometry=TRUE;
//
//	m_NumFilesize=m_D3_CurrHOG_Dir[m_D3_CurrHOG_OofList[OofNum]].size;
//	m_NumVertices=m_D3_Model.Vcount;
//	m_NumPolygons=m_D3_Model.Pcount;
//	m_NumTextures=m_D3_TotalTexture;
//	m_NumSubmodels=m_D3_Model.Scount;
//	m_NumPositions=m_D3_Display.TotalKey;
//	flog.Close();
//	return ERROR_GEN_NOERROR;
//}
//
//void CMODVIEW32Doc::D3_LoadOGFData()
//{
//	FILE *file;
//	unsigned short Color;
//	unsigned char InputChar,runcount;
//	unsigned int Red,Green,Blue;
//	unsigned int RGBAColor;
//
//	OGF_HEADER1 header1;
//	OGF_HEADER2 header2;
//
//	OAF_HEADER1 h1;
//	OAF_HEADER2 h2;
//
//	unsigned int offset;
//	char Filename[50];
//	unsigned char FileType;
//
//	m_TexturesNum=-1;
//	//Load Main D3 texture set if not yet done
//	D3_ReadD3HOG();
//	
//	for(int i=0;i<MAX_FS_TEXTURE;i++)
//	{
//		if(m_D3_Display.TextureFlag[i])
//			glDeleteLists(m_D3_ModelTexture[i],1);
//		m_D3_Display.TextureFlag[i]=FALSE; 
//		m_D3_Display.ClearFlag[i]=FALSE;
//	}
//
//
//	for(unsigned int i=0;i<m_D3_TotalTexture;i++)
//	{
//		FileType=NEW_FILE;
//
//		/*if(!m_D3_Display.Used_texture[i])
//			FileType=SKIP_FILE;*/
//
//		TRACE("Trying to load %s",m_D3_TextureList[i]);
//
//		if(!m_InfoMode)
//		{
//			char str[256];
//			sprintf_s(str,"Loading %s...",m_D3_TextureList[i]);
//			SetStatusBarText(str);
//		}
//
//		m_TexturesNum++;
//		m_Textures[m_TexturesNum].Filename=m_D3_TextureList[i];
//		m_Textures[m_TexturesNum].Location="<Not found>";
//		m_Textures[m_TexturesNum].Type="<Unknown>";
//		m_Textures[m_TexturesNum].Width=-1;
//		m_Textures[m_TexturesNum].Height=-1;
//		m_Textures[m_TexturesNum].Used=-1;
//		m_Textures[m_TexturesNum].InFilename="";
//		m_Textures[m_TexturesNum].InOffset=-1;
//		m_Textures[m_TexturesNum].InSize=-1;
//		m_Textures[m_TexturesNum].Error="???";
//
//
//		if(FileType==NEW_FILE)
//		{
//			strcpy_s(Filename,m_D3_TextureList[i]);
//			strcat_s(Filename, ".ogf");
//			
//			//OGF Search process #1: Current directory
//			if((file=fopen(Filename, "rb")) !=(FILE *) NULL)
//			{
//				FileType=OGF_FILE;
//				TRACE(".ogf...Found as OGF in current directory!\n");
//				m_Textures[m_TexturesNum].Location="Current directory";
//				m_Textures[m_TexturesNum].Type="OGF Texture";
//				m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_OGF;
//				m_Textures[m_TexturesNum].InFilename=Filename;
//				m_Textures[m_TexturesNum].InOffset=0;
//				m_Textures[m_TexturesNum].InSize=-1;
//			}
//			
//			//OGF Search process #2: Current .HOG file
//			if(FileType==NEW_FILE)
//			{
//				unsigned int j=0;
//				while((j<m_D3_CurrHOG_Header.dirnumber)&(_strcmpi(Filename,m_D3_CurrHOG_Dir[j].filename)!=0))
//					j++;
//
//				if(j<m_D3_CurrHOG_Header.dirnumber)
//				{
//					if((file=fopen(m_D3_CurrHOG_Filename,"rb"))!=(FILE *)NULL)
//					{
//						fseek(file, m_D3_CurrHOG_Dir[j].offset, SEEK_SET);
//						FileType=OGF_FILE;
//						TRACE(".ogf...Found as OGF in current HOG!\n");
//						m_Textures[m_TexturesNum].Location="Current HOG";
//						m_Textures[m_TexturesNum].Type="OGF Texture";
//						m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_OGF;
//						m_Textures[m_TexturesNum].InFilename=m_D3_CurrHOG_Filename;
//						m_Textures[m_TexturesNum].InOffset=m_D3_CurrHOG_Dir[j].offset;
//						m_Textures[m_TexturesNum].InSize=-1;
//					}
//				}
//			}
//
//			//OGF Search process #3: Main .HOG file
//			if((FileType==NEW_FILE)&(m_D3_MainHOG_Loaded))
//			{
//				unsigned int j=0;
//				while((j<m_D3_MainHOG_Header.dirnumber)&(_strcmpi(Filename,m_D3_MainHOG_Dir[j].filename)!=0))
//					j++;
//
//				if(j<m_D3_MainHOG_Header.dirnumber)
//				{
//					if((file=fopen(m_D3_MainHOG_Filename,"rb"))!=(FILE *)NULL)
//					{
//						fseek(file, m_D3_MainHOG_Dir[j].offset, SEEK_SET);
//						FileType=OGF_FILE;
//						TRACE(".ogf...Found as OGF in main HOG!\n");
//						m_Textures[m_TexturesNum].Location="Main Descent 3 HOG";
//						m_Textures[m_TexturesNum].Type="OGF Texture";
//						m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_OGF;
//						m_Textures[m_TexturesNum].InFilename=m_D3_MainHOG_Filename;
//						m_Textures[m_TexturesNum].InOffset=m_D3_MainHOG_Dir[j].offset;
//						m_Textures[m_TexturesNum].InSize=-1;
//					}
//				}
//			}
//		}
//
//		if(FileType==NEW_FILE)
//		{
//			strcpy_s(Filename,m_D3_TextureList[i]);
//			strcat_s(Filename, ".oaf");
//
//			//OAF Search process #1: Current directory
//			if((file=fopen(Filename, "rb")) !=(FILE *) NULL)
//			{
//				FileType=OAF_FILE;
//				TRACE(".oaf...Found as OAF in current directory!\n");
//				m_Textures[m_TexturesNum].Location="Current directory";
//				m_Textures[m_TexturesNum].Type="OAF Animation";
//				m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_OAF;
//				m_Textures[m_TexturesNum].InFilename=Filename;
//				m_Textures[m_TexturesNum].InOffset=0;
//				m_Textures[m_TexturesNum].InSize=-1;
//			}
//			
//			//OAF Search process #2: Current .HOG file
//			if(FileType==NEW_FILE)
//			{
//				unsigned int j=0;
//				while((j<m_D3_CurrHOG_Header.dirnumber)&(_strcmpi(Filename,m_D3_CurrHOG_Dir[j].filename)!=0))
//					j++;
//
//				if(j<m_D3_CurrHOG_Header.dirnumber)
//				{
//					if((file=fopen(m_D3_CurrHOG_Filename,"rb"))!=(FILE *)NULL)
//					{
//						fseek(file, m_D3_CurrHOG_Dir[j].offset, SEEK_SET);
//						FileType=OAF_FILE;
//						TRACE(".oaf...Found as OAF in current HOG!\n");
//						m_Textures[m_TexturesNum].Location="Current HOG";
//						m_Textures[m_TexturesNum].Type="OAF Animation";
//						m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_OAF;
//						m_Textures[m_TexturesNum].InFilename=m_D3_CurrHOG_Filename;
//						m_Textures[m_TexturesNum].InOffset=m_D3_CurrHOG_Dir[j].offset;
//						m_Textures[m_TexturesNum].InSize=-1;
//					}
//				}
//			}
//
//			//OAF Search process #3: Main .HOG file
//			if(FileType==NEW_FILE)
//			{
//				unsigned int j=0;
//				while((j<m_D3_MainHOG_Header.dirnumber)&(_strcmpi(Filename,m_D3_MainHOG_Dir[j].filename)!=0))
//					j++;
//
//				if(j<m_D3_MainHOG_Header.dirnumber)
//				{
//					if((file=fopen(m_D3_MainHOG_Filename,"rb"))!=(FILE *)NULL)
//					{
//						fseek(file, m_D3_MainHOG_Dir[j].offset, SEEK_SET);
//						FileType=OAF_FILE;
//						TRACE(".oaf...Found as OAF in main HOG!\n");
//						m_Textures[m_TexturesNum].Location="Main Descent 3 HOG";
//						m_Textures[m_TexturesNum].Type="OAF Animation";
//						m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_OAF;
//						m_Textures[m_TexturesNum].InFilename=m_D3_MainHOG_Filename;
//						m_Textures[m_TexturesNum].InOffset=m_D3_MainHOG_Dir[j].offset;
//						m_Textures[m_TexturesNum].InSize=-1;
//					}
//				}
//			}
//
//
//			if(FileType==OAF_FILE)
//			{
//				fread(&h1, sizeof(OAF_HEADER1), 1, file);
//				if(h1.id==0x5C29)
//					fread(&h2, sizeof(OAF_HEADER2), 1, file);
//			}
//		}
//
//
//
//		/*(FileType==NEW_FILE)
//		{	   // check the alt name list
//			strcpy(Filename,m_D3_TextureList[i]);
//			strcat(Filename, ".ogf");
//
//			k=0;
//			while((k<CROSSLIST_TOTAL)&(strcmpi(Filename,CrossList[k].OgfName) !=0))
//				k++;
//			if(k<CROSSLIST_TOTAL)
//			{
//				strcpy(Filename,CrossList[k].FileName);
//
//				if((file=fopen(Filename, "rb")) !=(FILE *) NULL)
//					FileType=OGF_FILE;
//				else {
//					j=0;
//					while((j<HogOgfNum)&(strcmpi(Filename,hog2_load[OgfList[j]].filename) !=0))
//						j++;
//
//					if(j<HogOgfNum)
//					{
//						if((file=fopen(m_D3_CurrHOG_Filename,"rb"))==(FILE *) NULL)
//						{
//							MessageBox(hMainSave,"Ogf: Can't open the hog file","ERROR",MB_OK);
//							exit(-2);
//						}
//						fseek(file, hog2_load[OgfList[j]].offset, SEEK_SET);
//						FileType=OGF_FILE;
//					}
//				}
//			}
//		}
//
//
//		if(FileType==NEW_FILE)
//		{	   // check the alt name list
//			strcpy(Filename,m_D3_TextureList[i]);
//			strcat(Filename, ".oaf");
//
//			k=0;
//			while((k<CROSSLIST_TOTAL)&(strcmpi(Filename,CrossList[k].OgfName) !=0))
//				k++;
//			if(k<CROSSLIST_TOTAL)
//			{
//				strcpy(Filename,CrossList[k].FileName);
//				if((file=fopen(Filename, "rb")) !=(FILE *) NULL)
//					FileType=OAF_FILE;
//				else {
//					j=0;
//					while((j<HogOafNum)&(strcmpi(Filename,hog2_load[OafList[j]].filename) !=0))
//						j++;
//
//					if(j<HogOafNum)
//					{
//						if((file=fopen(m_D3_CurrHOG_Filename,"rb"))==(FILE *) NULL)
//						{
//							MessageBox(hMainSave,"Oaf: Can't open the hog file","ERROR",MB_OK);
//							exit(-2);
//						}
//						fseek(file, hog2_load[OafList[j]].offset, SEEK_SET);
//						FileType=OAF_FILE;
//					}
//				}
//				if(FileType==OAF_FILE)
//				{
//					fread(&h1, sizeof(OAF_HEADER1), 1, file);
//					if(h1.id==0x5C29)
//						fread(&h2, sizeof(OAF_HEADER2), 1, file);
//				}
//			}
//		}*/
//
//
//		if((FileType==OGF_FILE)||(FileType==OAF_FILE))
//		{
//			fread(&header1, sizeof(OGF_HEADER1), 1, file);
//			InputChar=1;
//			while(InputChar !=0)
//				fread(&InputChar, sizeof(unsigned char), 1, file);
//			fread(&header2, sizeof(OGF_HEADER2), 1, file);
//			
//			m_Textures[m_TexturesNum].Width=header2.width;
//			m_Textures[m_TexturesNum].Height=header2.height;
//
//			if(header2.height>128)
//			{
//				m_Textures[m_TexturesNum].Error="Height>128 - Unsupported in MODELVIEW32";
//				FileType=SKIP_FILE;
//			}
//			if(header2.width>128)
//			{
//				m_Textures[m_TexturesNum].Error="Width>128 - Unsupported in MODELVIEW32";
//				FileType=SKIP_FILE;
//			}
//			if(FileType==SKIP_FILE)
//				fclose(file);
//		}
//
//		if((FileType==OGF_FILE)||(FileType==OAF_FILE))
//		{
//			if(!m_InfoMode)
//			{
//				char str[256];
//				sprintf_s(str,"Loading %s...",Filename);
//				SetStatusBarText(str);
//			}
//
//			unsigned int RGBAtexture[128*128];
//
//			offset=0;
//			runcount=0;
//			int lastpos=ftell(file);
//			for(unsigned short j=0;j<header2.height;j++)
//			{
//				for(unsigned short k=0;k<header2.width;k++)
//				{
//					if(runcount>0)
//						runcount--;
//					else
//					{
//						fread(&runcount, sizeof(unsigned char), 1, file);
//						if(runcount>0)
//							runcount--;
//						fread(&Color, sizeof(unsigned short), 1, file);
//
//						if(header1.hdr[2]==0x7A)
//						{
//							Blue=Color&0x1F;
//							Green=(Color>>5)&0x1F;
//							Red=(Color>>10)&0x1F;
//
//							RGBAColor=Red<<3;
//							RGBAColor+=Green<<11;
//							RGBAColor+=Blue<<19;
//
//							if(Color&0x8000)
//								RGBAColor+=255<<24;
//							else
//							{
//								if(Color&0x7FFF)
//									RGBAColor+=255<<24;
//								m_D3_Display.ClearFlag[i]=TRUE;
//							}
//						}
//						else
//						{
//							Blue=Color&0x0F;
//							Green=(Color>>4)&0x0F;
//							Red=(Color>>8)&0x0F;
//
//							RGBAColor=Red<<4;
//							RGBAColor+=Green<<12;
//							RGBAColor+=Blue<<20;
//							if((Color&0xEEE)==0)
//								m_D3_Display.ClearFlag[i]=TRUE;
//							else
//								RGBAColor+=255<<24;
//						}
//					}
//					RGBAtexture[offset]=RGBAColor;
//					offset++;
//				}
//			}
//
//#ifdef _WITHTXVIEW
//			int size=128*128*4;
//			m_Textures[m_TexturesNum].Flags=TEXTUREINFOFLAG_TEXTURELOADED | TEXTUREINFOFLAG_TEXTUREIS24BIT;
//			m_Textures[m_TexturesNum].InSize=ftell(file)-lastpos;
//			m_Textures[m_TexturesNum].Error="<ok>";
//			try {
//				m_Textures[m_TexturesNum].Bitmap=new unsigned char[size];
//				int temp,revtemp;
//				byte *r,*g,*b;
//				for(int j=0;j<size/4;j++)
//				{
//					temp=RGBAtexture[j];
//					r=(byte *)(&temp)+2;
//					g=(byte *)(&temp)+1;
//					b=(byte *)(&temp);
//					revtemp=(*r)+(*g)*256+(*b)*65536;
//					memcpy(m_Textures[m_TexturesNum].Bitmap+4*i,&revtemp,4);
//
//					//memcpy(m_Textures[m_TexturesNum].Bitmap,&RGBAtexture,size);
//				}
//			} catch (CMemoryException e) {
//				m_Textures[m_TexturesNum].Valid=FALSE;
//				m_Textures[m_TexturesNum].Error="Out of Memory";
//				break;
//			}
//#endif
//
//			fclose(file);
//
//			m_D3_Display.TextureFlag[i]=TRUE;
//			glNewList(m_D3_ModelTexture[i]=glGenLists(1), GL_COMPILE);
//			glTexImage2D(GL_TEXTURE_2D,0,4,header2.width,header2.height,0,GL_RGBA,
//						 GL_UNSIGNED_BYTE, RGBAtexture);
//			glEndList();
//		}
//		else
//		{
//			TRACE("...Failed!\n");
//			m_Textures[m_TexturesNum].Error="Not found";
//		}
//		if(m_D3_Display.TextureFlag[i])
//			strcpy_s(m_D3_Display.LoadName[i],Filename);
//		else
//			m_D3_Display.LoadName[i][0]=0;
//
//		m_Textures[m_TexturesNum].Used=0;
//		for(unsigned int k=0;k<m_D3_Model.Pcount;k++)
//		{
//			if(m_D3_Model.Poly[k].Color==i)
//				m_Textures[m_TexturesNum].Used++;
//		}
//	}
//
//	if(!m_InfoMode)
//	{
//		SetStatusBarText("Ready");
//		POSITION pos=GetFirstViewPosition();	
//		CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
//		CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
//		viewFrame->SetTypeOfView(TOV_MODELVIEW);
//	}
//}
//
//ERRORCODE CMODVIEW32Doc::D3_InitHOG(CFile *fp,HOG2_HEADER *hog2_header,HOG2_LOAD *hog2_load,BOOL fillexplorer)
//{
//	unsigned int offset;
//	HOG2_INFO hog2_info;
//	HOG2_LOAD hog2_ltmp[MAX_D3_FILESINHOG];
//
//	/*if((file=fopen(HogName,"rb"))==(FILE *) NULL)
//	{
//		MessageBox(hMainSave,"Can't open the hog file","ERROR",MB_OK);
//		exit(-2);
//	}*/
//	fp->Read(hog2_header, sizeof(HOG2_HEADER));
//	if(hog2_header->signature !=ID_HOG2)
//	{
//		m_ErrorString=ERROR_GEN_INVALIDHOGFILE_TEXT;
//		return ERROR_GEN_INVALIDHOGFILE;
//	}
//
//	if(hog2_header->dirnumber>MAX_D3_FILESINHOG)
//	{
//		m_ErrorString=ERROR_D3_HOGTOOMANYFILES_TEXT;
//		m_ErrorDetails.Format("Is: %i, Should be <=%i",hog2_header->dirnumber,MAX_D3_FILESINHOG);
//		return ERROR_D3_HOGTOOMANYFILES;
//	}
//
//		//Hog2Num=hog2_header->dirnumber;
//	offset=hog2_header->diroffset;
//	for(unsigned int i=0;i<hog2_header->dirnumber;i++)
//	{
//		fp->Read(&hog2_info, sizeof(HOG2_INFO));
//		strncpy_s(hog2_ltmp[i].filename, hog2_info.filename, 36);
//		hog2_ltmp[i].size=hog2_info.size;
//		hog2_ltmp[i].offset=offset;
//		offset+=hog2_info.size;
//	}
//
//	char *pntptr;
//
//	if(fillexplorer)
//	{
//		m_ArchiveStats_NumTotal=0;
//		m_ArchiveStats_NumLores=0;
//		m_ArchiveStats_NumHires=0;
//		m_ArchiveStats_NumOther=0;
//		POSITION pos=GetFirstViewPosition();	
//		CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
//		CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
//
//		char tempfname[8192];
//		strcpy_s(tempfname,fp->GetFileName());
//		ExplorerClear();
//		ExplorerAddRoot((char *)&tempfname);
//
//		m_D3_CurrHOG_OofNum=0;
//		for(unsigned int i=0;i<hog2_header->dirnumber;i++)
//		{
//			if((pntptr=strrchr(hog2_ltmp[i].filename,'.')) !=NULL)
//			{
//				if(_strcmpi(pntptr,".oof")==0)
//				{
//					m_D3_CurrHOG_OofList[m_D3_CurrHOG_OofNum]=i;
//					m_D3_CurrHOG_OofNum++;
//					//check if >MAX_OOFFILESINHOG2
//					
//					memset(tempfname,0,36);
//					strncpy_s(tempfname,hog2_ltmp[i].filename,strlen(hog2_ltmp[i].filename)-4); //drop the ".oof"
//					if (tempfname[strlen(tempfname)-1] != 0)
//						tempfname[strlen(tempfname)-1] = 0;
//					//Apply Filter
//					m_ArchiveStats_NumTotal++;
//					CString x=tempfname;
//					x.MakeLower();
//					if(x.Right(2)=="lo" || x.Right(3)=="low" || x.Right(7)=="lowpoly" || x.Right(3)=="med" || x.Right(6)=="medium" || x.Right(7)=="medpoly" || x.Right(7)=="medploy" || x.Left(2)=="lo" || x.Left(3)=="med")
//					{
//						m_ArchiveStats_NumLores++;
//						if(viewFrame->m_ExplorerFilterLowLevel)
//							TRACE("Skipping %s...\n",x);
//						else
//							ExplorerAddChild(tempfname);
//					}
//					else
//					{
//						m_ArchiveStats_NumHires++;
//						ExplorerAddChild(tempfname);
//					}
//				}
//				else
//					m_ArchiveStats_NumOther++;
//			}
//		}
//		ExplorerGoTop();
//	}
//
//	memcpy(hog2_load,hog2_ltmp,sizeof(HOG2_LOAD)*4500);
//	/*
//	HogOafNum=0;
//	HogOgfNum=0;
//	HogOofNum=0;
//	HogGamNum=0;
//	GameTableNum=0x8000;
//	for(i=0;i<Hog2Num;i++)
//	{
//		if((pntptr=strrchr(hog2_load[i].filename,'.')) !=NULL)
//		{
//			if(strcmpi(pntptr,".ogf")==0)
//			{
//				OgfList[HogOgfNum]=i;
//				HogOgfNum++;
//			}
//			if(strcmpi(pntptr,".oof")==0)
//			{
//				OofList[HogOofNum]=i;
//				HogOofNum++;
//			}
//			if(strcmpi(pntptr,".oaf")==0)
//			{
//				OafList[HogOafNum]=i;
//				HogOafNum++;
//			}
//			if(strcmpi(pntptr,".gam")==0)
//			{
//				GamList[HogGamNum]=i;
//				HogGamNum++;
//				GameTableNum=i;
//			}
//		}
//	}
//
//	if(D3Hog)
//	{
//		D3Hog2Num=Hog2Num;
//		D3HogOafNum=HogOafNum;
//		D3HogOgfNum=HogOgfNum;
//		D3HogOofNum=HogOofNum;
//		D3HogGamNum=HogGamNum;
//		for(i=0;i<HogOgfNum;i++)
//		{
//			D3OgfList[i]=OgfList[i];
//		}
//		for(i=0;i<HogOafNum;i++) {
//			D3OafList[i]=OafList[i];
//		}
//		for(i=0;i<HogOofNum;i++) {
//			D3OofList[i]=OofList[i];
//		}
//		for(i=0;i<HogGamNum;i++) {
//			D3GamList[i]=GamList[i];
//		}
//		for(i=0;i<Hog2Num;i++) {
//			D3hog2_load[i]=hog2_load[i];
//		}
//	}*/
//
//	return ERROR_GEN_NOERROR;
//}
//
//ERRORCODE CMODVIEW32Doc::D3_LoadHOGContent(CString filename)
//{
//	int HogOof=-1;
//	char *pntptr; 
//	unsigned int HogOofNum=0;
//	
//	m_ModelJustLoaded=TRUE;
//	m_TexturesNum=-1;
//
//	for(unsigned int i=0;i<m_D3_CurrHOG_Header.dirnumber;i++)
//	{
//		if((pntptr=strrchr(m_D3_CurrHOG_Dir[i].filename,'.')) !=NULL)
//		{
//			if(_strcmpi(pntptr,".oof")==0)
//			{
//				if(_strcmpi(m_D3_CurrHOG_Dir[i].filename,filename)==0) 
//					HogOof=HogOofNum++;
//				HogOofNum++;
//			}
//		}
//	}
//
//	if(HogOof==-1)
//	{
//		ClearDocument();
//		m_ErrorString=ERROR_GEN_NOMODELSELECTED_TEXT;
//		return ERROR_GEN_NOMODELSELECTED;
//	}
//
//	CFile fp;
//	fp.Open(m_D3_CurrHOG_Filename,CFile::modeRead);
//	m_ErrorCode=D3_ReadOOF(&fp,HogOof);
//	fp.Close();
//	
//	if(m_ErrorCode==ERROR_GEN_NOERROR && !m_InfoMode)
//		D3_LoadOGFData();
//
//	if(!m_InfoMode)
//	{
//		POSITION pos=GetFirstViewPosition();	
//		CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
//		CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
//		m_view=viewFrame->GetCurrentView();
//	
//		viewFrame->GameBarD3();
//		m_view->ChangeSize();
//		viewFrame->UpdateTextureBar();
//		viewFrame->UpdateMainDisplay();
//		UpdateAllViews(NULL);
//	}
//
//	return m_ErrorCode;
//}
//
//void CMODVIEW32Doc::D3_CalcDisplay()
//{
//	vms_vector Spos[MAX_D3_SOBJ];//,Testpos;
//
//	for(unsigned int i=0;i<m_D3_Model.Scount;i++)
//	{
//		Spos[i]=m_D3_Model.Soffset[i];
//		if(m_D3_Model.Sparent[i] !=-1)
//		{
//			Spos[i].x+=Spos[m_D3_Model.Sparent[i]].x;
//			Spos[i].y+=Spos[m_D3_Model.Sparent[i]].y;
//			Spos[i].z+=Spos[m_D3_Model.Sparent[i]].z;
//		}
//	}
//
//	float Min=0.0;
//	float Max=0.0;
//	for(unsigned int i=0;i<m_D3_Model.Vcount;i++)
//	{
//		vms_vector Testpos=m_D3_Model.Vpoint[i];
//		Testpos.x+=Spos[m_D3_Model.VSobj[i]].x;
//		Testpos.y+=Spos[m_D3_Model.VSobj[i]].y;
//		Testpos.z+=Spos[m_D3_Model.VSobj[i]].z;
//
//		if(Testpos.x>Max) Max=Testpos.x;
//		if(Testpos.x<Min) Min=Testpos.x;
//		if(Testpos.y>Max) Max=Testpos.y;
//		if(Testpos.y<Min) Min=Testpos.y;
//		if(Testpos.z>Max) Max=Testpos.z;
//		if(Testpos.z<Min) Min=Testpos.z;
//	}
//	Min=(float)abs((int)Min);
//	if(Min>Max) Max=Min;
//	m_D3_Display.MaxXYZ=Max;
//
///*	m_D3_Display.PartsRotate=FALSE;
//	for(i=0;i<MAX_SOBJ;i++)
//	{
//		m_D3_Display.Facing[i]=FALSE;
//		m_D3_Display.Rotate[i]=FALSE;
//		m_D3_Display.Glow[i]=FALSE;
//		m_D3_Display.Fov[i]=FALSE;
//		m_D3_Rstep[i]=0;
//	}*/
//
///*	for(i=0;i<m_D3_Model.Scount;i++)
//{
//		if((pntptr=strrchr(m_D3_Model.Prop[i],'$')) !=NULL)
//		{
//
//			TRACE("%s\n",m_D3_Model.Prop[i]);
//
//			if(strnicmp(pntptr,"$facing",7)==0) {
//				m_D3_Display.Facing[i]=TRUE;
////					MessageBox(hMainSave,"$facing","INFO",MB_OK);
//				NumPoly=0;
//				for(Poly=0;Poly<m_D3_Model.Pcount;Poly++) {
//					if(m_D3_Model.Poly[Poly].Sobj==i) {
//						NumPoly++;
//						v1[0]=(float)((floor(m_D3_Model.Poly[Poly].Normal.x*1000.0))/1000.0);
//						v1[1]=(float)((floor(m_D3_Model.Poly[Poly].Normal.y*1000.0))/1000.0);
//						v1[2]=(float)((floor(m_D3_Model.Poly[Poly].Normal.z*1000.0))/1000.0);
//					}
//				}*/
//				/*HHif(NumPoly>1)
//					MessageBox(hMainSave,"$facing... NumPoly>1","INFO",MB_OK);*/
//		/*		v2[0]=0.0;
//				v2[1]=0.0;
//				v2[2]=1.0;
//				Angle=(float)((floor(Vangle(v1,v2)*1000.0))/1000.0);
//				m_D3_Display.Foffset[i][0]=Angle;
//				if((Angle==0.0)|(Angle==180.0)) {
//					m_D3_Display.Foffset[i][1]=0.0;
//					m_D3_Display.Foffset[i][2]=1.0;
//					m_D3_Display.Foffset[i][3]=0.0;
//				} else {
//					n=Vcross(v1,v2);
//					m_D3_Display.Foffset[i][1]=n.x;
//					m_D3_Display.Foffset[i][2]=n.y;
//					m_D3_Display.Foffset[i][3]=n.z;
//
//				}
//			}*/
//
//		/*	if(strnicmp(pntptr,"$rotate=",8)==0) {
//				m_D3_Display.Rotate[i]=TRUE;
//				sscanf(pntptr+8, "%f", &m_D3_Display.Rspeed[i] );
//				m_D3_Display.PartsRotate=TRUE;
////					MessageBox(hMainSave,"$rotate=","INFO",MB_OK);
//			}*/
//
//		/*	if(strnicmp(pntptr,"$glow=",6)==0) {
//				m_D3_Display.Glow[i]=TRUE;
//				sscanf(pntptr+6, "%f,%f,%f,%f", &m_D3_Display.Gset[i][0],&m_D3_Display.Gset[i][1],
//					   &m_D3_Display.Gset[i][2],&m_D3_Display.Gset[i][3] );
//				m_D3_Display.Gset[i][3] *=10;
////					MessageBox(hMainSave,"$glow=","INFO",MB_OK);
//
//
//				for(Poly=0;Poly<m_D3_Model.Pcount;Poly++) {
//					if(m_D3_Model.Poly[Poly].Sobj==i) {
//						NumPoly=Poly;
//						v1[0]=(float)((floor(m_D3_Model.Poly[Poly].Normal.x*1000.0))/1000.0);
//						v1[1]=(float)((floor(m_D3_Model.Poly[Poly].Normal.y*1000.0))/1000.0);
//						v1[2]=(float)((floor(m_D3_Model.Poly[Poly].Normal.z*1000.0))/1000.0);
//					}
//				}
//				if(m_D3_Model.Poly[NumPoly].Type==0) {
//					m_D3_Display.Facing[i]=TRUE;
//					v2[0]=0.0;
//					v2[1]=0.0;
//					v2[2]=1.0;
//					Angle=(float)((floor(Vangle(v1,v2)*1000.0))/1000.0);
//					m_D3_Display.Foffset[i][0]=Angle;
//					if((Angle==0.0)|(Angle==180.0)) {
//						m_D3_Display.Foffset[i][1]=1.0;
//						m_D3_Display.Foffset[i][2]=0.0;
//						m_D3_Display.Foffset[i][3]=0.0;
//					} else {
//						n=Vcross(v1,v2);
//						m_D3_Display.Foffset[i][1]=n.x;
//						m_D3_Display.Foffset[i][2]=n.y;
//						m_D3_Display.Foffset[i][3]=n.z;
//					}
//				}
//
//			}
//
//
//			if(strnicmp(pntptr,"$fov=",5)==0) {
//				m_D3_Display.Fov[i]=TRUE;
////					MessageBox(hMainSave,"$Fov=","INFO",MB_OK);
//			}
//
//		}
//	}
//	TRACE("DONE!\n");*/
//}

float CMODVIEW32Doc::Vmag(float v[3])
{
	return((float)sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]));
}

FS_VPNT CMODVIEW32Doc::Vnorm(float v[3])
{
	float value;
	FS_VPNT n;
	value=Vmag(v);
	if(value==0)
	{
		n.x=v[0];
		n.y=v[1];
		n.z=v[2];
	} else {
		n.x=v[0]/value;
		n.y=v[1]/value;
		n.z=v[2]/value;
	}
	return n;
}

float CMODVIEW32Doc::Vdot(float v1[3],float v2[3])
{
	return(v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2]);
}

FS_VPNT CMODVIEW32Doc::Vcross(float v1[3],float v2[3])
{
	float nv[3];
	nv[0]= ((float)(v1[1]*v2[2])-(float)(v1[2]*v2[1]));
	nv[1]=-((float)(v1[0]*v2[2])-(float)(v1[2]*v2[0]));
	nv[2]= ((float)(v1[0]*v2[1])-(float)(v1[1]*v2[0]));
	return Vnorm(nv);
}

float CMODVIEW32Doc::Vangle(float v1[3],float v2[3])
{
	float value;
	value=Vdot(v1,v2);
	value/=Vmag(v1);
	value/=Vmag(v2);
	value=(float)acos(value);
	value*=(float)57.2958;  //combine 180/3.14159
	return(value);
}


////////////////////////////////////////////////////////////////////////////////////////
// Descent 2 stuff
////////////////////////////////////////////////////////////////////////////////////////
//
//int CMODVIEW32Doc::D2_InitHAM(CFile *fp, BOOL fillexplorer)
//{
//	UINTW	n_textures=0,n_sounds=0,n_vclips=0,n_eclips=0,n_wclips=0;
//	UINTW	n_robots=0,n_robot_joints=0,n_weapons=0,n_powerups=0;
//	UINTW	n_polygon_models=0,n_gagues=0,n_object_bitmaps=0;
//	UINTW	n_bitmap_ptrs=0,n_cockpits=0,first_multi_bitmap=0;
//	UINTW	n_reactors=0,s_robots=0,s_robot_joints=0,s_object_bitmaps=0;
//
//	for(int i=0;i<625;i++)
//		m_D2_HxmObjBitmap[i].index=0xffff;
//
//	for(int i=0;i<MAX_D2_POLYGON_MODELS;i++)
//	{
//		m_D2_PolType[i]=D2_POLYMODEL_TYPE_UNKNOWN;
//		m_D2_AmLores[i]=FALSE;
//	}
//
//	//Detect file type
//	switch(read_INT32(fp))
//	{
//	case ID_HAM:
//		{
//			m_FileVersion=read_UINTW(fp);
//			n_textures=read_UINTW(fp);             // read tmap_info
//			for(unsigned int i=0;i<n_textures;i++)
//				cfseek(fp,sizeof(D2_BITMAP_INDEX),SEEK_CUR);
//			for(unsigned int i=0;i<n_textures;i++)
//				cfseek(fp,sizeof(D2_TMAP_INFO),SEEK_CUR);
//			n_sounds=read_UINTW(fp);                  // read sounds
//			for(unsigned int i=0;i<n_sounds;i++)
//				cfseek(fp,sizeof(UINT8),SEEK_CUR);
//			for(unsigned int i=0;i<n_sounds;i++)
//				cfseek(fp,sizeof(UINT8),SEEK_CUR);
//			n_vclips=read_UINTW(fp);              // read video clips
//			for(unsigned int i=0;i<n_vclips;i++)
//				cfseek(fp,sizeof(D2_VCLIP),SEEK_CUR);
//			n_eclips=read_UINTW(fp);                 // read effect clips
//			for(unsigned int i=0;i<n_eclips;i++)
//				cfseek(fp,sizeof(D2_ECLIP),SEEK_CUR);
//			n_wclips=read_UINTW(fp);               // read weapon clips
//			for(unsigned int i=0;i<n_wclips;i++)
//				cfseek(fp,sizeof(D2_WCLIP),SEEK_CUR);
//			n_robots=read_UINTW(fp);                // read robot info
//			for(unsigned int i=0;i<n_robots;i++)
//			{
//				m_D2_MO_RobotInfo[i]=fp->GetPosition();
//				D2_ReadROB(fp,i);
//				fp->Seek(m_D2_MO_RobotInfo[i],SEEK_SET);
//				fp->Read(&m_D2_Robots[i],sizeof(D2_ROBOT_INFO));
//				m_D2_MyRobot[m_D2_Model.RobotInfo.model_num]=i;
//				m_D2_PolType[m_D2_Model.RobotInfo.model_num]=D2_POLYMODEL_TYPE_ROBOT;
//			}
//			n_robot_joints=read_UINTW(fp);          // read robot joints
//			for(unsigned int i=0;i<n_robot_joints;i++)
//			{
//				D2_JOINTPOS temp;
//				fp->Read(&temp,sizeof(D2_JOINTPOS));
//				ASSERT(temp.jointnum<MAX_D2_ROBOT_JOINTS);
//				//m_D2_JointPos[temp.jointnum].angles=temp.angles;
//				m_D2_JointPos[i]=temp;
//			}
//
//			n_weapons=read_UINTW(fp);                 // read weapon info
//			for(unsigned int i=0;i<n_weapons;i++)
//				fp->Read(&m_D2_Weapons[i],sizeof(D2_WEAPON_INFO));
//			n_powerups=read_UINTW(fp);                 // read powerup info
//			for(unsigned int i=0;i<n_powerups;i++)
//				cfseek(fp,sizeof(D2_POWERUP_TYPE_INFO),SEEK_CUR);
//			n_polygon_models=read_UINTW(fp);              // read polygon models
//			//ERROR CHECK!
//			for(unsigned int i=0;i<n_polygon_models;i++)
//			{
//				m_D2_ModelLoadnr_to_ID[i]=i;
//				m_D2_PolygonModels[i].offsetPolyModel=(int) fp->GetPosition();
//				fp->Read(&m_D2_PolygonModels[i].dataPolyModel, sizeof(D2_POLYMODEL));
//				m_D2_PolygonModels[i].sizeModelData=m_D2_PolygonModels[i].dataPolyModel.model_data_size;
//				/*int j=polygon_model.simpler_model;
//				if(j!=0)
//				{
//					m_D2_PolType[j-1]=m_D2_PolType[i];
//					m_D2_MyRobot[j-1]=m_D2_MyRobot[i];
//					m_D2_AmLores[j-1]=TRUE;
//				}*/
//			}
//			for(unsigned int i=0;i<n_polygon_models;i++)
//			{
//				m_D2_PolygonModels[i].offsetModelData=(int) fp->GetPosition();
//				//m_D2_MO_ModelData[i]=model_data_cpos[i];
//				cfseek(fp,m_D2_PolygonModels[i].sizeModelData,SEEK_CUR);
//			}
//			for(unsigned int i=0;i<n_polygon_models;i++)
//			{
//				m_D2_PolygonModels[i].offsetDyingModel=(int) fp->GetPosition();
//				fp->Read(&m_D2_PolygonModels[i].dying_modelnum,sizeof(UINT32));
//			}
//			for(unsigned int i=0;i<n_polygon_models;i++)
//			{
//				m_D2_PolygonModels[i].offsetDestroyedModel=(int) fp->GetPosition();
//				fp->Read(&m_D2_PolygonModels[i].destroyed_modelnum,sizeof(UINT32));
//			}
//			n_gagues=read_UINTW(fp);                 // read gagues
//			for(unsigned int i=0;i<n_gagues;i++)
//				cfseek(fp,sizeof(D2_BITMAP_INDEX),SEEK_CUR);
//			for(unsigned int i=0;i<n_gagues;i++)
//				cfseek(fp,sizeof(D2_BITMAP_INDEX),SEEK_CUR);
//			n_object_bitmaps=read_UINTW(fp);            // read object bitmaps
//			for(unsigned int i=0;i<n_object_bitmaps;i++)
//				fp->Read(&m_D2_ObjBitmap[i],sizeof(D2_BITMAP_INDEX));
//			for(unsigned int i=0;i<n_object_bitmaps;i++)
//				fp->Read(&m_D2_ObjBitmapPtr[i],sizeof(D2_BITMAP_INDEX));
//			fp->Read(&m_D2_PlayerShip,sizeof(D2_PLAYER_SHIP));
//			n_cockpits=read_UINTW(fp);               // read cockpit data
//			for(unsigned int i=0;i<n_cockpits;i++)
//				cfseek(fp,sizeof(UINT16),SEEK_CUR);
//			first_multi_bitmap=read_UINTW(fp);         // read first multi bitmap
//			n_reactors=read_UINTW(fp);                // read reactor data
//			for(unsigned int i=0;i<n_reactors;i++)
//				fp->Read(&m_D2_Reactors[i],sizeof(D2_REACTOR));
//			m_D2_Marker_ModelNum=read_UINTW(fp);      // read marker model number
//			cfseek(fp,sizeof(UINT16)*2600,SEEK_CUR);  // read game bitmap translate
//		}
//		break;
//
//	case ID_V_HAM:
//		{
//			m_FileVersion=read_UINTW(fp)+100;       // read version
//			n_weapons=read_UINTW(fp);         // read weapon info
//			for(unsigned int i=0;i<n_weapons;i++)
//				cfseek(fp,sizeof(D2_WEAPON_INFO),SEEK_CUR);
//
//			n_robots=read_UINTW(fp);         // read robot info
//			for(unsigned int i=0;i<n_robots;i++)
//			{
//			/*	m_D2_MO_RobotInfo[i]=fp->GetPosition();
//				D2_ReadROB(fp,i);
//				m_D2_MyRobot[m_D2_Model.RobotInfo.model_num]=i;
//				m_D2_PolType[m_D2_Model.RobotInfo.model_num]=D2_POLYMODEL_TYPE_ROBOT;*/
//				cfseek(fp,sizeof(D2_ROBOT_INFO),SEEK_CUR);
//			}
//
//			n_robot_joints=read_UINTW(fp);      // read robot joints
//			for(unsigned int i=0;i<n_robot_joints;i++)
//				cfseek(fp,sizeof(D2_JOINTPOS),SEEK_CUR);
//
//			n_polygon_models=read_UINTW(fp);      // read polygon models
//			//ERROR CHECK!
//			for(unsigned int i=0;i<n_polygon_models;i++)
//			{
//				m_D2_ModelLoadnr_to_ID[i]=i+D2_VHAM_IDFIRSTMODEL;
//				m_D2_PolygonModels[i].offsetPolyModel=(int) fp->GetPosition();
//				fp->Read(&m_D2_PolygonModels[i].dataPolyModel, sizeof(D2_POLYMODEL));
//				m_D2_PolygonModels[i].sizeModelData=m_D2_PolygonModels[i].dataPolyModel.model_data_size;
//
//				//
//				/*polymodel_pgm[i]=i;
//				s_polygon_models[i]=0;
//				polymodel_cpos[i]=fp->GetPosition();
//				fp->Read(&polygon_model, sizeof(POLYMODEL));
//				model_data_size[i]=polygon_model.model_data_size;
//				m_D2_MO_PolygonModel[i]=polymodel_cpos[i];
//				m_D2_MS_ModelData[i]=model_data_size[i];
//				int j=polygon_model.simpler_model;
//				if(j!=0)
//				{
//					m_D2_PolType[j-1]=m_D2_PolType[i];
//					m_D2_MyRobot[j-1]=m_D2_MyRobot[i];
//					m_D2_AmLores[j-1]=TRUE;
//				}*/
//			}
//			for(unsigned int i=0;i<n_polygon_models;i++)
//			{
//				m_D2_PolygonModels[i].offsetModelData=(int) fp->GetPosition();
//				cfseek(fp,m_D2_PolygonModels[i].sizeModelData,SEEK_CUR);
//			}
//			for(unsigned int i=0;i<n_polygon_models;i++)
//			{
//				m_D2_PolygonModels[i].offsetDyingModel=(int) fp->GetPosition();
//				fp->Read(&m_D2_PolygonModels[i].dying_modelnum,sizeof(UINT32));
//				//cfseek(fp,sizeof(UINT32),SEEK_CUR);
//			}
//			for(unsigned int i=0;i<n_polygon_models;i++)
//			{
//				m_D2_PolygonModels[i].offsetDestroyedModel=(int) fp->GetPosition();
//				fp->Read(&m_D2_PolygonModels[i].destroyed_modelnum,sizeof(UINT32));
//				//cfseek(fp,sizeof(UINT32),SEEK_CUR);
//			}
//
//			for(int i=422;i<600;i++)
//				m_D2_ObjBitmap[i].index=0;                    // clear vertigo bitmaps
//
//			for(int i=502;i<600;i++)
//				m_D2_ObjBitmapPtr[i]=0;                       // clear vertigo pointers
//
//			n_object_bitmaps=read_UINTW(fp);          // read object bitmaps
//			for(unsigned int i=0;i<n_object_bitmaps;i++)
//			{
//				D2_BITMAP_INDEX	new_bitmap;
//				fp->Read(&new_bitmap, sizeof(D2_BITMAP_INDEX));
//				m_D2_ObjBitmap[i+422]=new_bitmap;
//			}
//
//			n_bitmap_ptrs=read_UINTW(fp);       // read bitmap pointers
//			for(unsigned int i=0;i<n_bitmap_ptrs;i++)
//			{
//				UINT16 new_pointer;
//				fp->Read(&new_pointer, sizeof(UINT16));
//				m_D2_ObjBitmapPtr[i+502]=new_pointer;
//			}
//			for(unsigned int i=0;i<n_bitmap_ptrs;i++)            // don't know what this is...
//				cfseek(fp,sizeof(UINT16),SEEK_CUR);
//		}
//		break;
//
//	case ID_HXM:
//		{
//			m_FileVersion=read_UINTW(fp)+200;      // read version
//			n_robots=read_UINTW(fp);      // read robot info
//			for(unsigned int i=0;i<n_robots;i++)
//			{
//				s_robots=read_UINTW(fp);
//				cfseek(fp,sizeof(D2_ROBOT_INFO),SEEK_CUR);
//			}
//			n_robot_joints=read_UINTW(fp);   // read robot joints
//			for(unsigned int i=0;i<n_robot_joints;i++)
//			{
//				s_robot_joints=read_UINTW(fp);
//				cfseek(fp,sizeof(D2_JOINTPOS),SEEK_CUR);
//			}
//			n_polygon_models=read_UINTW(fp);       // read polygon models
//			/*model_count=n_polygon_models;*/
//			//ERROR CHECK!
//			for(unsigned int i=0;i<n_polygon_models;i++)
//			{
//				m_D2_ModelLoadnr_to_ID[i]=read_UINTW(fp);
//				m_D2_PolygonModels[i].offsetPolyModel=(int) fp->GetPosition();
//				fp->Read(&m_D2_PolygonModels[i].dataPolyModel, sizeof(D2_POLYMODEL));
//				m_D2_PolygonModels[i].sizeModelData=m_D2_PolygonModels[i].dataPolyModel.model_data_size;
//				m_D2_PolygonModels[i].offsetModelData=(int) fp->GetPosition();
//				cfseek(fp,m_D2_PolygonModels[i].sizeModelData,SEEK_CUR);
//				m_D2_PolygonModels[i].offsetDyingModel=(int) fp->GetPosition();
//				fp->Read(&m_D2_PolygonModels[i].dying_modelnum,sizeof(UINT32));
//				m_D2_PolygonModels[i].offsetDestroyedModel=(int) fp->GetPosition();
//				fp->Read(&m_D2_PolygonModels[i].destroyed_modelnum,sizeof(UINT32));
//			}
//			n_object_bitmaps=read_UINTW(fp);             // read object bitmaps
//			for(unsigned int i=0;i<n_object_bitmaps;i++)
//			{
//				s_object_bitmaps=read_UINTW(fp);
//				fp->Read(&m_D2_HxmObjBitmap[s_object_bitmaps], sizeof(D2_BITMAP_INDEX));
//			}
//		 }
//	/*	 // how about a pog file?
//
//			i=strlen(PogName) - 3;
//			PogName[i]='\0';
//			strncat(PogName, "pog\0\0", 5);
//
//			if(bAutopog & bPog) {
//			  pogfile=fopen(PogName,"rb");
//			  if(pogfile)  PogActive=TRUE;
//				 fclose(pogfile);
//			}*/
//			break;
//		 case ID_HMEL:
//			 {
//
//			m_FileVersion=read_UINTW(fp)+300;      // read version
//			ASSERT(m_FileVersion==1);
//
//			n_polygon_models=1;       // read polygon models
//			//model_count=n_polygon_models;
//			int i=0;
//			m_D2_ModelLoadnr_to_ID[i]=read_UINTW(fp);
//			m_D2_PolygonModels[i].offsetPolyModel=(int) fp->GetPosition();
//			fp->Read(&m_D2_PolygonModels[i].dataPolyModel, sizeof(D2_POLYMODEL));
//			m_D2_PolygonModels[i].sizeModelData=m_D2_PolygonModels[i].dataPolyModel.model_data_size;
//			m_D2_PolygonModels[i].offsetModelData=(int) fp->GetPosition();
//			cfseek(fp,m_D2_PolygonModels[i].sizeModelData,SEEK_CUR);
//			m_D2_PolygonModels[i].offsetDyingModel=(int) fp->GetPosition();
//			fp->Read(&m_D2_PolygonModels[i].dying_modelnum,sizeof(UINT32));
//			m_D2_PolygonModels[i].offsetDestroyedModel=(int) fp->GetPosition();
//			fp->Read(&m_D2_PolygonModels[i].destroyed_modelnum,sizeof(UINT32));
//		}
//	/*	 // how about a pog file?
//
//			i=strlen(PogName) - 3;
//			PogName[i]='\0';
//			strncat(PogName, "pog\0\0", 5);
//
//			if(bAutopog & bPog) {
//			  pogfile=fopen(PogName,"rb");
//			  if(pogfile)  PogActive=TRUE;
//				 fclose(pogfile);
//			}*/
//		break;
//
//
//	default:
//		return -1; //unexpected end?
//	}
//
//	//*******************************************
//	//Enumerate polygon models - ROBOTS
//	for(unsigned int i=0;i<n_robots;i++)
//	{
//		int m=m_D2_Robots[i].model_num;
//		//ERROR CHECK m
//		m_D2_PolygonModels[m].Type=D2_POLYMODEL_TYPE_ROBOT;
//		m_D2_PolygonModels[m].GunNum=m_D2_Robots[i].n_guns;
//		for(int j=0;j<m_D2_Robots[i].n_guns;j++)
//		{
//			m_D2_PolygonModels[m].Gun[j]=m_D2_Robots[i].gun_points[j];
//			//m_D2_Model[m].ExInfo.GunDirection[j]=0/0/1;
//			m_D2_PolygonModels[m].GunInSubModel[j]=m_D2_Robots[i].gun_submodels[j];
//		}
//
//		m_D2_PolygonModels[m].Joints.Available=TRUE;
//
//		for(int j=0;j<MAX_D2_GUNS+1;j++)
//		{
//			for(int k=0;k<N_ANIM_STATES;k++)
//				m_D2_PolygonModels[m].Joints.Anim_states[j][k]=m_D2_Robots[i].anim_states[j][k];
//		}
//	}
//
//	//Enumerate polygon models - REACTORS
//	for(unsigned int i=0;i<n_reactors;i++)
//	{
//		int m=m_D2_Reactors[i].model_num;
//		//ERROR CHECK m
//		m_D2_PolygonModels[m].Type=D2_POLYMODEL_TYPE_REACTOR;
//		m_D2_PolygonModels[m].GunNum=m_D2_Reactors[i].n_guns;
//		for(unsigned int j=0;j<m_D2_Reactors[i].n_guns;j++)
//		{
//			m_D2_PolygonModels[m].Gun[j]=m_D2_Reactors[i].gun_points[j];
//			m_D2_PolygonModels[m].GunDirection[j]=m_D2_Reactors[i].gun_dirs[j];
//			m_D2_PolygonModels[m].GunInSubModel[j]=0;
//		}
//		m_D2_PolygonModels[m].Joints.Available=FALSE;
//	}
//
//	//Enumerate polygon models - MARKER
//	int m=m_D2_Marker_ModelNum;
//	if(m>=0 && m<MAX_D2_POLYGON_MODELS) //Just to be sure
//	{
//		m_D2_PolygonModels[m].Type=D2_POLYMODEL_TYPE_MARKER;
//		m_D2_PolygonModels[m].GunNum=0;
//		m_D2_PolygonModels[m].Joints.Available=FALSE;
//	}
//
//	//Enumerate polygon models - WEAPONS
//	for(unsigned int i=0;i<n_weapons;i++)
//	{
//		if(m_D2_Weapons[i].render_type!=1)
//		{
//			int n=m_D2_Weapons[i].model_num;
//			if((n>0) && (m<256))
//			{
//				m_D2_PolygonModels[n].Type=D2_POLYMODEL_TYPE_WEAPON;
//				m_D2_PolygonModels[n].GunNum=0;
//			}
//			m_D2_PolygonModels[n].Joints.Available=FALSE;
//
//			n=m_D2_Weapons[i].model_num_inner;
//			if((n>0) && (n<256))
//			{
//				m_D2_PolygonModels[n].Type=D2_POLYMODEL_TYPE_WEAPONINNER;
//				m_D2_PolygonModels[n].GunNum=0;
//			}
//			m_D2_PolygonModels[n].Joints.Available=FALSE;
//		}
//	}
//
//	//Enumerate polygon models - SHIP
//	m=m_D2_PlayerShip.model_num;
//	//ERROR CHECK m
//	m_D2_PolygonModels[m].Type=D2_POLYMODEL_TYPE_PLAYERSHIP;
//	m_D2_PolygonModels[m].GunNum=N_PLAYER_GUNS;
//	for(unsigned int i=0;i<N_PLAYER_GUNS;i++)
//	{
//		m_D2_PolygonModels[m].Gun[i]=m_D2_PlayerShip.gun_points[i];
//		//m_D2_Model[m].ExInfo.GunDirection[i]=0/0/1;
//		m_D2_PolygonModels[m].GunInSubModel[i]=0;
//	}
//	m_D2_PolygonModels[m].Joints.Available=FALSE;
//
//	//Enumerate polygon models - LORES flag
//	for(unsigned int i=0;i<n_polygon_models;i++)
//	{
//		m=m_D2_PolygonModels[i].dataPolyModel.simpler_model-1;
//		if(m!=-1)
//		{
//			m_D2_PolygonModels[m].Type=m_D2_PolygonModels[i].Type | D2_POLYMODEL_TYPE_FLAG_LORES;
//			m_D2_PolygonModels[m].GunNum=m_D2_PolygonModels[i].GunNum;
//			for(int j=0;j<m_D2_PolygonModels[i].GunNum;j++)
//			{
//				m_D2_PolygonModels[m].Gun[j]=m_D2_PolygonModels[i].Gun[j];
//				m_D2_PolygonModels[m].GunDirection[j]=m_D2_PolygonModels[i].GunDirection[j];
//				m_D2_PolygonModels[m].GunInSubModel[j]=0; //LORES versions have only 1 submodel
//			}
//		}
//	}
//
//	//Enumerate polygon models - DESTROYED flag
//	for(unsigned int i=0;i<n_polygon_models;i++)
//	{
//		m=m_D2_PolygonModels[i].destroyed_modelnum;
//		if(m!=-1)
//		{
//			m_D2_PolygonModels[m].Type=m_D2_PolygonModels[i].Type | D2_POLYMODEL_TYPE_FLAG_DESTROYED;
//			m_D2_PolygonModels[m].GunNum=m_D2_PolygonModels[i].GunNum;
//			for(int j=0;j<m_D2_PolygonModels[i].GunNum;j++)
//			{
//				m_D2_PolygonModels[m].Gun[j]=m_D2_PolygonModels[i].Gun[j];
//				m_D2_PolygonModels[m].GunDirection[j]=m_D2_PolygonModels[i].GunDirection[j];
//				m_D2_PolygonModels[m].GunInSubModel[j]=0; //LORES versions have only 1 submodel
//			}
//		}
//	}
//
//	//Enumerate polygon models - DYING flag
//	for(unsigned int i=0;i<n_polygon_models;i++)
//	{
//		m=m_D2_PolygonModels[i].dying_modelnum;
//		if(m!=-1)
//		{
//			m_D2_PolygonModels[m].Type=m_D2_PolygonModels[i].Type | D2_POLYMODEL_TYPE_FLAG_DYING;
//			m_D2_PolygonModels[m].GunNum=m_D2_PolygonModels[i].GunNum;
//			for(int j=0;j<m_D2_PolygonModels[i].GunNum;j++)
//			{
//				m_D2_PolygonModels[m].Gun[j]=m_D2_PolygonModels[i].Gun[j];
//				m_D2_PolygonModels[m].GunDirection[j]=m_D2_PolygonModels[i].GunDirection[j];
//				m_D2_PolygonModels[m].GunInSubModel[j]=0; //LORES versions have only 1 submodel
//			}
//		}
//	}
//
//
//	//*******************************************
//	
//	if(fillexplorer)
//	{
//		m_ArchiveStats_NumTotal=0;
//		m_ArchiveStats_NumLores=0;
//		m_ArchiveStats_NumHires=0;
//		m_ArchiveStats_NumOther=-1;
//
//		POSITION pos=GetFirstViewPosition();	
//		CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
//		CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
//
//		char tempfname[8192];
//		strcpy_s(tempfname,fp->GetFileName());
//		ExplorerClear();
//		ExplorerAddRoot((char *)&tempfname);
//
//		for(unsigned int i=0;i<n_polygon_models;i++)
//		{
//			m_ArchiveStats_NumTotal++;
//
//			//Do string conversion to shut the analyzer up
//			const size_t newsizea = (m_D2_PM_Desc[m_D2_ModelLoadnr_to_ID[i]].GetLength() + 1);
//			char *nstringa = new char[newsizea];
//			strcpy_s(nstringa, newsizea, m_D2_PM_Desc[m_D2_ModelLoadnr_to_ID[i]]);
//
//			sprintf(tempfname,"Model #%i: %s [ID %i]",i,nstringa,m_D2_ModelLoadnr_to_ID[i]);
//
//			if(m_D2_PolygonModels[i].Type & D2_POLYMODEL_TYPE_FLAG_LORES)
//			{
//				m_ArchiveStats_NumLores++; //Count lores models
//				if(!viewFrame->m_ExplorerFilterLowLevel)
//					ExplorerAddChild(tempfname,i);
//			}
//			else
//			{
//				m_ArchiveStats_NumHires++;
//				ExplorerAddChild(tempfname,i);
//			}
//		}
//		ExplorerGoTop();
//		m_D2_Models_Num=n_polygon_models;
//	}
//
//	return 0;  //expected end
//}
//
////Get the data... the right way
//#define D2_w(walker)  (*((UINT16 *) (walker)))
//#define D2_v(walker)  (*((D2_VPNT *) (walker)))
//#define D2_wp(walker) ((UINT16 *) (walker))
//#define D2_vp(walker) ((D2_VPNT *) (walker))*/
//
//ERRORCODE CMODVIEW32Doc::D2_Walk(void *StartAt,int module)
//{
//	UINT8 *walker=(UINT8 *)StartAt;
//	int i;
//
//	while(D2_w(walker)!=D_EOF)
//	{
//		switch (D2_w(walker))
//		{
//		case D_EOF: //00
//			break;
//
//		case D_DEFPOINTS: //01
//			{
//				int n=D2_w(walker+2);
//				walker+=n*12 + 4;
//				break;
//			}
//
//		case D_FLATPOLY: //02
//			{
//				int n=D2_w(walker+2);
//				if(n>20)
//				{
//					m_ErrorString=ERROR_D2_TOOMANYPOINTSINPOLY_TEXT;
//					m_ErrorDetails.Format("Polygon #%i - Is: %i, Should be <=%i",m_D2_Model.Pcount,n,20);
//					return ERROR_D2_TOOMANYPOINTSINPOLY;
//				}
//				m_D2_Model.Pcount++;
//				if(m_D2_Model.Pcount>=400)
//				{
//					m_ErrorString=ERROR_D2_TOOMANYPOLYGONS_TEXT;
//					m_ErrorDetails.Format("Should be <=%i",401);
//					return ERROR_D2_TOOMANYPOLYGONS;
//				}
//				m_D2_Model.Poly[m_D2_Model.Pcount].Polytype=2;
//				m_D2_Model.Poly[m_D2_Model.Pcount].Corners=n;
//				m_D2_Model.Poly[m_D2_Model.Pcount].Normal=D2_v(walker+16);
//				m_D2_Model.Poly[m_D2_Model.Pcount].Colors=D2_w(walker + 28);
//				m_D2_Model.Poly[m_D2_Model.Pcount].Segment=module;
//				for(i=0;i<n;i++)
//					m_D2_Model.Poly[m_D2_Model.Pcount].Pointindex[i]=D2_w(walker+30+(i*2));
//				walker+=30+((n&~1)+1)*2;
//				break;
//			}
//
//		case D_TMAPPOLY: //03
//			{
//				int n=D2_w(walker+2);
//				if(n>20)
//				{
//					m_ErrorString=ERROR_D2_TOOMANYPOINTSINPOLY_TEXT;
//					m_ErrorDetails.Format("Polygon #%i - Is: %i, Should be <=%i",m_D2_Model.Pcount,n,20);
//					return ERROR_D2_TOOMANYPOINTSINPOLY;
//				}
//				m_D2_Model.Pcount++;
//				if(m_D2_Model.Pcount>=400)
//				{
//					m_ErrorString=ERROR_D2_TOOMANYPOLYGONS_TEXT;
//					m_ErrorDetails.Format("Should be <=%i",401);
//					return ERROR_D2_TOOMANYPOLYGONS;
//				}
//				m_D2_Model.Poly[m_D2_Model.Pcount].Polytype=3;
//				m_D2_Model.Poly[m_D2_Model.Pcount].Corners=n;
//				m_D2_Model.Poly[m_D2_Model.Pcount].Normal=D2_v(walker+16);
//				m_D2_Model.Poly[m_D2_Model.Pcount].Colors=D2_w(walker + 28);
//				if(m_D2_Model.Poly[m_D2_Model.Pcount].Colors>19)
//				{
//					m_ErrorString=ERROR_D2_INVALIDTEXTUREID_TEXT;
//					m_ErrorDetails.Format("Polygon #%i - Is: %i, Should be <=%i",m_D2_Model.Pcount,m_D2_Model.Poly[m_D2_Model.Pcount].Colors,19);
//					return ERROR_D2_INVALIDTEXTUREID;
//				}
//				m_D2_Model.Poly[m_D2_Model.Pcount].Segment=module;
//				for(i=0;i<n;i++)
//					m_D2_Model.Poly[m_D2_Model.Pcount].Pointindex[i]=D2_w(walker + 30 + (i*2));
//				for(i=0;i<n;i++)
//					m_D2_Model.Poly[m_D2_Model.Pcount].Bitmap[i]=D2_v(walker+30 +((n&~1)+1)*2 +i*12);
//				walker+=30 + ((n&~1)+1)*2 + n*12;
//				break;
//			}
//
//		case D_SORTNORM: //04
//			{
//				ERRORCODE ret=D2_Walk(walker+D2_w(walker+28),module);
//				if(ret!=ERROR_GEN_NOERROR)
//					return ret;
//				ret=D2_Walk(walker+D2_w(walker+30),module);
//				if(ret!=ERROR_GEN_NOERROR)
//					return ret;
//				walker+=32;
//				break;
//			}
//
//		case D_RODBM: //05
//			walker+=36;
//			break;
//
//		case D_SUBCALL: //06
//			{
//				int n=D2_w(walker+2);
//				m_D2_Model.Scount++;
//				if(m_D2_Model.Scount>9)
//				{
//					m_ErrorString=ERROR_D2_TOOMANYSEGMENTS_TEXT;
//					m_ErrorDetails.Format("Should be <=%i",9);
//					return ERROR_D2_TOOMANYSEGMENTS;
//				}
//				m_D2_Model.Soffset[n]=D2_v(walker+4);
//
//				m_D2_Model.Sbackup[n]=m_D2_Model.Soffset[n];
//				m_D2_Model.Soffset[n].x+=m_D2_Model.Soffset[module].x;
//				m_D2_Model.Soffset[n].y+=m_D2_Model.Soffset[module].y;
//				m_D2_Model.Soffset[n].z+=m_D2_Model.Soffset[module].z;
//
//				ERRORCODE ret=D2_Walk(walker+D2_w(walker+16),n);
//				if(ret!=ERROR_GEN_NOERROR)
//					return ret;
//				walker+=20;
//				break;
//			}
//
//		case D_DEFP_START: //07
//			{
//				int n=D2_w(walker+2);
//				int p=D2_w(walker+4);
//				m_D2_Model.Vcount+=n;
//				if(m_D2_Model.Vcount>800)
//				{
//					m_ErrorString=ERROR_D2_TOOHIGHVCOUNT_TEXT;
//					m_ErrorDetails.Format("Should be <=%i",800);
//					return ERROR_D2_TOOHIGHVCOUNT;
//				}
//				for(i=0;i<n;i++)
//				{
//					m_D2_Model.Vpoint[p+i]=D2_v(walker+8+i*12);
//					m_D2_Model.Vpoint[p+i].x+=m_D2_Model.Soffset[module].x;
//					m_D2_Model.Vpoint[p+i].y+=m_D2_Model.Soffset[module].y;
//					m_D2_Model.Vpoint[p+i].z+=m_D2_Model.Soffset[module].z;
//					m_D2_Model.Vbackup[p+i]=m_D2_Model.Vpoint[p+i];
//					m_D2_Model.Vsegment[p+i]=module;
//				}
//				walker+=n*12 + 8;
//				break;
//			}
//
//		case D_GLOW: //08
//			walker+=4;
//			break;
//
//		default:
//			m_ErrorString=ERROR_D2_BADPOLYMODELDATA_TEXT;
//			return ERROR_D2_BADPOLYMODELDATA;
//		}
//	}
//	return ERROR_GEN_NOERROR;
//}
//
////Read a polymodel
//ERRORCODE CMODVIEW32Doc::D2_ReadPOL(CFile *fp, int ModelNum)
//{
//	ClearDocument(FALSE);
//
//	int modellen=m_D2_PolygonModels[ModelNum].sizeModelData;
//	UINT8 *ModelData=(UINT8 *) malloc(modellen);
//	
//	fp->Seek(m_D2_PolygonModels[ModelNum].offsetPolyModel, SEEK_SET);
//	fp->Read(&m_D2_Model.PolyModel,sizeof(D2_POLYMODEL));
//	fp->Seek(m_D2_PolygonModels[ModelNum].offsetModelData, SEEK_SET);
//	fp->Read(ModelData,modellen);
//
//	m_D2_Model.Vcount=0;
//	m_D2_Model.Pcount=0;
//	m_D2_Model.Scount=0;
//
//	ERRORCODE ret=D2_Walk(ModelData,0);
//	if(ret!=ERROR_GEN_NOERROR)
//	{
//		free(ModelData);
//		return ret;
//	}
//
//	m_NumVertices=m_D2_Model.Vcount;
//	m_NumPolygons=m_D2_Model.Pcount;
//	m_NumTextures=m_D2_Model.PolyModel.n_textures;
//	m_NumSubmodels=m_D2_Model.Scount;
//	m_NumFilesize=m_D2_PolygonModels[ModelNum].sizeModelData;
//	m_D2_Model.Joints=m_D2_PolygonModels[ModelNum].Joints;
//	m_ModelType_Desc=D2_GenerateModelTypeName(m_D2_PolygonModels[ModelNum].Type);
//	D2_CalcDisplay();
//
//	//OBSOLETE
//	m_D2_Model.PolType=m_D2_PolType[ModelNum];
//	m_D2_Model.MyRobot=m_D2_MyRobot[ModelNum];
//	m_D2_Model.AmLores=m_D2_AmLores[ModelNum];
//	//OBSOLETE
//	
//	if(m_D2_Model.PolType==D2_POLYMODEL_TYPE_ROBOT)
//		ret=D2_ReadROB(fp,m_D2_Model.MyRobot);
//
//	free(ModelData);
//
//	if(ret!=ERROR_GEN_NOERROR)
//		return ret;
//
//	//Since there are no blocks in D2, we only add four fixed ones containing information
//	m_Blocks_Num=4;
//	m_Blocks[0].name="MODEL";
//	m_Blocks[0].size=sizeof(D2_POLYMODEL);
//	m_Blocks[0].offset=m_D2_PolygonModels[ModelNum].offsetPolyModel;
//	m_Blocks[0].desc="Header";
//	m_Blocks[1].name="IDTA";
//	m_Blocks[1].offset=m_D2_PolygonModels[ModelNum].offsetModelData;
//	m_Blocks[1].size=modellen;
//	m_Blocks[1].desc="IDTA 3D model data";
//	m_Blocks[2].name="DYING";
//	m_Blocks[2].size=4;
//	m_Blocks[2].offset=m_D2_PolygonModels[ModelNum].offsetDyingModel;
//	m_Blocks[2].desc="Link to dying model version (used in reactors)";
//	m_Blocks[3].name="DEAD";
//	m_Blocks[3].offset=m_D2_PolygonModels[ModelNum].offsetDestroyedModel;
//	m_Blocks[3].size=4;
//	m_Blocks[3].desc="Link to destroyed model version (used in player ship)";
//
//	//Log
//	int k=m_D2_Model.PolyModel.first_texture;
//	int l=m_D2_Model.PolyModel.first_texture+m_D2_Model.PolyModel.n_textures-1;
//	CStdioFile flog;
//	CString logtext;
//	flog.Open(((CMODVIEW32App *)AfxGetApp())->m_LogFile,CFile::modeWrite | CFile::modeCreate);
//	LOGTEXT("Number of submodels: %i\nRadius: %.2f\nMaximum diameter: %.2f\nTextures: #%i-#%i\n\n",m_D2_Model.PolyModel.n_models,((float)m_D2_Model.PolyModel.rad)/65536,m_D2_Model.MaxXYZ,k,l); LOGDOIT;
//
//	int tmp=m_D2_Model.PolyModel.simpler_model-1;
//	if(tmp==-1)
//	{
//		LOGTEXT("No lo-res version of this model available.\n"); LOGDOIT;
//	}
//	else
//	{
//		LOGTEXT("Lo-res version of this model available: #%i\n",tmp); LOGDOIT;
//	}
//	tmp=m_D2_PolygonModels[ModelNum].dying_modelnum;
//	if(tmp==-1)
//	{
//		LOGTEXT("No \"dying\" version of this model available.\n"); LOGDOIT;
//	}
//	else
//	{
//		LOGTEXT("\"Dying\" version of this model available: #%i\n",tmp); LOGDOIT;
//	}
//	tmp=m_D2_PolygonModels[ModelNum].destroyed_modelnum;
//	if(tmp==-1)
//	{
//		LOGTEXT("No \"dead\" version of this model available.\n"); LOGDOIT;
//	}
//	else
//	{
//		LOGTEXT("\"Dead\" version of this model available: #%i\n",tmp); LOGDOIT;
//	}
//
//	flog.Close();
//
//	if(!m_InfoMode)
//	{
//		SetStatusBarText("Ready");
//		POSITION pos=GetFirstViewPosition();	
//		CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
//		CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
//		viewFrame->SetTypeOfView(TOV_MODELVIEW);
//	}
//
//	return ERROR_GEN_NOERROR;
//}
//
//ERRORCODE CMODVIEW32Doc::D2_ReadROB(CFile *fp, int RobotNum)
//{
//	fp->Seek(m_D2_MO_RobotInfo[RobotNum],SEEK_SET);
//	fp->Read(&m_D2_Model.RobotInfo,sizeof(D2_ROBOT_INFO));
//
//	m_Guns.Num=m_D2_Model.RobotInfo.n_guns;
//	if(m_Guns.Num>=MAX_GUNS)
//	{
//		m_ErrorString=ERROR_GEN_TOOMANYGUNS_TEXT;
//		m_ErrorDetails.Format("Exceeded %i.",MAX_GUNS);
//		return ERROR_GEN_TOOMANYGUNS;
//	}
//	for(int i=0;i<m_Guns.Num;i++)
//	{
//		m_Guns.Gun[i].x=(float)m_D2_Model.RobotInfo.gun_points[i].x;
//		m_Guns.Gun[i].y=(float)m_D2_Model.RobotInfo.gun_points[i].y;
//		m_Guns.Gun[i].z=(float)m_D2_Model.RobotInfo.gun_points[i].z;
//		m_Guns.InSubModel[i]=m_D2_Model.RobotInfo.gun_submodels[i];
//	}
//	return ERROR_GEN_NOERROR;
//}

//Called to load the Polymodel
//ERRORCODE CMODVIEW32Doc::D2_LoadHAXMContent(int ModelNum)
//{
//	POSITION pos=GetFirstViewPosition();	
//	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
//	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
//
//	CFile fp;
//	fp.Open(m_D2_CurrHAXM_Filename,CFile::modeRead);
//	m_ErrorCode=D2_ReadPOL(&fp,ModelNum);
//	fp.Close();
//	if(m_ErrorCode!=ERROR_GEN_NOERROR)
//	{
//		viewFrame->UpdateMainDisplay();
//		return m_ErrorCode;
//	}
//	if(!m_InfoMode)
//		D2_LoadPIGData();
//	m_ModelLoaded=TRUE;
//	m_DoResetGeometry=TRUE;
//	m_ModelJustLoaded=TRUE;
//
//	if(!m_InfoMode)
//	{
//		m_view=viewFrame->GetCurrentView();
//
//		viewFrame->GameBarD2();
//		m_view->D2_PreparePosition();
//		m_view->ChangeSize();
//		viewFrame->UpdateTextureBar();
//		viewFrame->UpdateMainDisplay();
//		UpdateAllViews(NULL);
//	}
//	return ERROR_GEN_NOERROR;
//}
//
////Generates the rgba texture by using the palette and texture data
//void CMODVIEW32Doc::D2_Index2RGBA(UINT32 i)
//{
//	UINT32 RGBAtexture[64*64];
//
//	m_D2_BitmapData.bitmap[i].transparent=FALSE;
//	for(int j=0;j<64*64;j++)
//	{
//		int index=m_D2_BitmapData.bitmap[i].bitmap_array[j] * 3;
//		RGBAtexture[j]=(m_D2_Palettes[index+m_D2_TextureSet]*4);
//		RGBAtexture[j]+=(m_D2_Palettes[index+1+m_D2_TextureSet]*4)<<8;
//		RGBAtexture[j]+=(m_D2_Palettes[index+2+m_D2_TextureSet]*4)<<16;
//		RGBAtexture[j]+=255<<24;
//		if(m_D2_BitmapData.bitmap[i].bitmap_array[j]>254)
//		{
//			RGBAtexture[j]&=0x00ffffff;
//			m_D2_BitmapData.bitmap[i].transparent=TRUE;
//		}
//	}
//
//	glNewList(m_D2_ModelTexture[i]=glGenLists(1),GL_COMPILE);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
//
//	//Define the 2D texture image.
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	// Force 4-byte alignment
//	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
//	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
//	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
//	glTexImage2D(GL_TEXTURE_2D, 0, 4, 64, 64, 0,GL_RGBA, GL_UNSIGNED_BYTE, RGBAtexture);
//	glEndList();
//}
//
//// Reads and decodes a D2 RLE-compressed file
//int CMODVIEW32Doc::D2_ReadRLE(FILE *file,UINT32 i)
//{
//	UINT8 xsize[64];
//	UINT8 line[256];
//	UINT8 *line_ptr;
//	UINTW x,y,j;
//	INT32 size;
//	UINT8 runcount,runvalue,byte;
//
//	fread(&size,1,sizeof(INT32),file);	// read total size of data
//	fread(xsize,64,1,file);				// read size of each of the rows of data
//	for(y=0;y<64;y++)	// loop on each row
//	{
//		fread(line,xsize[y],1,file);	// read one row of data into buffer
//		line_ptr=line;					// point to beginning of row
//		for(x=0;x<64;)	// loop on column
//		{
//			byte=*line_ptr++;              // get data
//			if((byte & 0xe0)==0xe0)		// if upper 3 bits set, then the low 5 bits
//			{	
//				runcount=byte & 0x1f;		// are the # of times to repeat the next
//				runvalue=*line_ptr++;		// color index
//				for(j=0;j<runcount;j++)
//				{     // set run of pixels
//					if(x<64)	// safety check in case exceeds row
//					{
//						m_D2_BitmapData.bitmap[i].bitmap_array[y*64+x]=runvalue;
//						x++;				// increment column counter
//					}
//				}
//			} else {
//				m_D2_BitmapData.bitmap[i].bitmap_array[y*64+x]=byte;	// otherwise this is just a pixel
//				x++;						// increment column counter
//			}
//		}
//	}
//	return size;
//}
//
////Load D2 textures
//void CMODVIEW32Doc::D2_LoadPIGData(void)
//{
//UINT16 AnimatedIndex[600]=
//{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,1842,   0,1741,   0,   0,   0,   0,
//    0,   0,   0,   0,1735,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,1869,1884,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,1903,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// 1930,   0,   0,   0,   0,   0,   0,   0,   0,1941,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,2105,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// 2110,   0,   0,   0,   0,   0,1853,1861,   0,1864,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,2006,
//    0,   0,   0,   0,   0,   0,   0,1993,   0,   0,
//    0,   0,   0,   0,2019,   0,   0,   0,   0,   0,
//    0,   0,2014,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,1758,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//    0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };
//
//	D2_PIG_HEADER  header_info;
//	//D2_PIG_HEADER	pogheader_info;
//	D2_PIG_TEXTURE texture_info[20];
//
//	D2_PIG_TEXTURE texture_tmp;
//	UINT32 readflag;
//	int i,j;
//	long offset_base,read_location;
//	FILE *file;
//	//UINT16 PogList[600],ChangeList[600];
//	//char dummySTRING[256];
//
//	if(m_D2_Model.PolyModel.n_textures>20)
//	{
//		return; /**/
//		//MessageBox(hMainSave,"N_Textures>20... ouch","ERROR",MB_OK);
//		//exit(3);
//	}
//	m_D2_BitmapData.count=m_D2_Model.PolyModel.n_textures;
//  
//  //if(PaletteLoaded==FALSE) return;
//
//	CString PigName;
//	COptionsDlg dlg;
//	PigName=dlg.GetD2Path();
//	if(PigName=="")
//		return; //ERRORCODE
//
//	switch(m_D2_TextureSet)
//	{
//		case D2_TEXTURESET_GROUPA:
//			PigName+="GROUPA.PIG";
//			break;
//
//		case D2_TEXTURESET_WATER:
//			PigName+="WATER.PIG";
//			break;
//
//		case D2_TEXTURESET_FIRE:
//			PigName+="FIRE.PIG";
//			break;
//
//		case D2_TEXTURESET_ICE:
//			PigName+="ICE.PIG";
//			break;
//
//		case D2_TEXTURESET_ALIEN1:
//			PigName+="ALIEN1.PIG";
//			break;
//
//		case D2_TEXTURESET_ALIEN2:
//			PigName+="ALIEN2.PIG";
//			break;
//
//		default:
//			ASSERT(FALSE);
//	}
//
//	file=fopen(PigName,"rb");
//	if(!file)
//		return;
//		
//	
//	m_TexturesNum=-1;
//	
//	m_D2_BitmapData.count=m_D2_Model.PolyModel.n_textures;
//	for(i=0;i<m_D2_BitmapData.count;i++)
//	{
//		if(m_D2_HxmObjBitmap[m_D2_ObjBitmapPtr[m_D2_Model.PolyModel.first_texture+i]].index !=0xffff)
//		{
//			if(AnimatedIndex[m_D2_ObjBitmapPtr[m_D2_Model.PolyModel.first_texture+i]] !=0)
//				m_D2_HxmObjBitmap[m_D2_ObjBitmapPtr[m_D2_Model.PolyModel.first_texture+i]].index=0xffff;
//		}
//	}
//	for(i=0;i<m_D2_BitmapData.count;i++)
//	{
//		if(m_D2_HxmObjBitmap[m_D2_ObjBitmapPtr[m_D2_Model.PolyModel.first_texture+i]].index==0xffff)
//			j=m_D2_ObjBitmap[m_D2_ObjBitmapPtr[m_D2_Model.PolyModel.first_texture+i]].index;
//		else
//			j=m_D2_HxmObjBitmap[m_D2_ObjBitmapPtr[m_D2_Model.PolyModel.first_texture+i]].index;
//		m_D2_BitmapData.bitmap[i].number=j-1;
//	}
//
//	//Read PIG header
//	fread(&header_info, sizeof(D2_PIG_HEADER), 1, file);
//	if(header_info.signature !=0x47495050L)
//	{
//	//	 printf("Not a PIG file");
//		return;
//	}
//
//	//Read PIG directory
//	TRACE("D2_LoadPIGData: Reading %i textures\n",header_info.num_textures);
//
//	m_D2_PIGNumTextures=header_info.num_textures;
//	int j_to_m_TexturesNum[MAX_D2_TEXTURE];
//	for(i=0;i<MAX_D2_TEXTURE;i++)
//		j_to_m_TexturesNum[i]=-1;
//	for(i=0;i<m_D2_PIGNumTextures;i++)
//	{
//		readflag=0;
//		for(j=0;j<m_D2_BitmapData.count;j++)
//		{
//			if(i==m_D2_BitmapData.bitmap[j].number)
//			{
//				if(readflag==0)
//				{
//					readflag=1;
//					read_location=ftell(file);
//				} else
//					fseek(file, read_location, SEEK_SET);
//				fread(&texture_info[j], sizeof(D2_PIG_TEXTURE), 1, file);
//				
//				//Texture names are not necessarily 0-terminated in D2, so we need to make sure it is
//				char tmp[9];
//				memset(&tmp,0,9);
//				memcpy(&tmp,&texture_info[j].name,8);
//				
//				m_TexturesNum++;
//				j_to_m_TexturesNum[j]=m_TexturesNum;
//				TRACE("m_TexturesNum=%i, i=%i, j=%i || m_D2_PIGNumTextures=%i, m_D2_BitmapData.count=%i\n",m_TexturesNum,i,j,m_D2_PIGNumTextures,m_D2_BitmapData.count);
//				m_Textures[m_TexturesNum].Filename=tmp;
//				m_Textures[m_TexturesNum].Filename=m_Textures[m_TexturesNum].Filename.Left(8);
//				m_Textures[m_TexturesNum].Location="Main PIG file (GROUPA)";
//				m_Textures[m_TexturesNum].Type="PIG texture";
//				m_Textures[m_TexturesNum].Width=64;
//				m_Textures[m_TexturesNum].Height=64;
//				m_Textures[m_TexturesNum].Used=0;
//				m_Textures[m_TexturesNum].Valid=TRUE;
//
//				for(int k=0;k<m_D2_Model.Pcount;k++)
//				{
//					if(m_D2_Model.Poly[k].Colors==j)
//						m_Textures[m_TexturesNum].Used++;
//				}
//			}
//		}
//		if(readflag==0)
//			fread(&texture_tmp,sizeof(D2_PIG_TEXTURE),1,file);
//	}
//
//	//Read PIG texture data
//	offset_base=ftell(file);
//	for(j=0;j<m_D2_BitmapData.count;j++)
//	{
//		read_location=offset_base+texture_info[j].offset;
//		fseek(file, read_location, SEEK_SET);
//		if ((j_to_m_TexturesNum[j] < 128) && (j_to_m_TexturesNum[j] != -1)) {
//			if((texture_info[j].flag & BM_FLAG_RLE)==0)
//			{
//				m_Textures[j_to_m_TexturesNum[j]].InSize=64*64;
//				m_Textures[j_to_m_TexturesNum[j]].Type="PIG texture (Uncompressed)";
//				m_Textures[j_to_m_TexturesNum[j]].FileType=TEXTUREINFOFILETYPE_D2U;
//				fread(&m_D2_BitmapData.bitmap[j].bitmap_array, sizeof(m_D2_BitmapData.bitmap[j].bitmap_array), 1, file);
//			}
//			else
//			{
//				m_Textures[j_to_m_TexturesNum[j]].Type="PIG texture (RLE-compressed)";
//				m_Textures[j_to_m_TexturesNum[j]].InSize=D2_ReadRLE(file,j);
//				m_Textures[j_to_m_TexturesNum[j]].FileType=TEXTUREINFOFILETYPE_D2C;
//			}
//#ifdef _WITHTXVIEW
//			m_Textures[j_to_m_TexturesNum[j]].Flags=TEXTUREINFOFLAG_TEXTURELOADED;
//			m_Textures[j_to_m_TexturesNum[j]].Bitmap=new unsigned char[64*64];
//			memcpy(m_Textures[j_to_m_TexturesNum[j]].Bitmap,&m_D2_BitmapData.bitmap[j].bitmap_array,64*64);
//
//		//Copy palette
//			for(i=0;i<256;i++)
//			{
//				m_Textures[j_to_m_TexturesNum[j]].Palette[0][i]=m_D2_Palettes[i*3+m_D2_TextureSet]*4;
//				m_Textures[j_to_m_TexturesNum[j]].Palette[1][i]=m_D2_Palettes[i*3+1+m_D2_TextureSet]*4;
//				m_Textures[j_to_m_TexturesNum[j]].Palette[2][i]=m_D2_Palettes[i*3+2+m_D2_TextureSet]*4;
//			}
//#endif
//		}
//	}
//	/*////
//	for(int j=0;j<255;j++)
//	{
//
//		int index=m_D2_BitmapData.bitmap[i].bitmap_array[j] * 3;
//		RGBAtexture[j]=(m_D2_Palettes[index+m_D2_TextureSet]*4);
//		RGBAtexture[j]+=(m_D2_Palettes[index+1+m_D2_TextureSet]*4)<<8;
//		RGBAtexture[j]+=(m_D2_Palettes[index+2+m_D2_TextureSet]*4)<<16;
//		RGBAtexture[j]+=255<<24;
//		if(m_D2_BitmapData.bitmap[i].bitmap_array[j]>254)
//		{
//			RGBAtexture[j]&=0x00ffffff;
//			m_D2_BitmapData.bitmap[i].transparent=TRUE;
//		}
//	}*/
//
//	fclose(file);


	/////////////////////////////////////
	//HH: We do not read POG textures yet
/*
 ===============================
*/

/*  if(PogActive & bPog)
{
	 file=fopen(PogName,"rb");
	 if(!file) {
		return;
	 }

  //
  //-------------------------
  // read header info
  //-------------------------
  //
  fread(&pogheader_info, sizeof(D2_PIG_HEADER), 1, file);
  if(pogheader_info.signature !=0x474f5044L) {
//	 printf("Not a POG file");
	 return;
  }

  //
  //-------------------------
  // read texture info
  //-------------------------
  //                 
  for(i=0;i<bitmapdata.count;i++) {
	 ChangeList[i]=0;
  }

  for(i=0;i<pogheader_info.num_textures;i++) {
	 fread(&PogList[i], sizeof(UINT16), 1, file);
  }

  for(i=0;i<pogheader_info.num_textures;i++) {
	 readflag=0;
	 for(j=0;j<bitmapdata.count;j++) {
		if(PogList[i]==(bitmapdata.bitmap[j].number+1)) {
		  ChangeList[j]=1;
		  if(readflag==0) {
			 readflag=1;
			 read_location=ftell(file);
		  } else {
			 fseek(file, read_location, SEEK_SET);
		  }
		  fread(&texture_info[j], sizeof(D2_PIG_TEXTURE), 1, file);
		}
	 }
	 if(readflag==0)
		fread(&texture_tmp, sizeof(D2_PIG_TEXTURE), 1, file);
  }

  //
  //-------------------------
  // read texture data base
  //-------------------------
  //

  offset_base=ftell(file);
  for(j=0;j<bitmapdata.count;j++) {

	 if(ChangeList[j] !=0) {
		read_location=offset_base + texture_info[j].offset;
		fseek(file, read_location, SEEK_SET);

		if((texture_info[j].flag & BM_FLAG_RLE)==0) {
		  fread(&m_D2_BitmapData.bitmap[j].bitmap_array, sizeof(m_D2_BitmapData.bitmap[j].bitmap_array), 1, file);
		} else {
		  read_rle(file,j);
	   }
	 }
  }

fclose(file);
  }*/

//	for(i=0;i<m_D2_BitmapData.count;i++)
//		D2_Index2RGBA(i);
//	//PigLoaded=TRUE;
//}



//void CMODVIEW32Doc::D2_CalcDisplay()
//{
//	long Min=0,Max=0;
//	for(int i=0;i<m_D2_Model.Vcount;i++) 
//	{
//		if(m_D2_Model.Vpoint[i].x>Max) Max=m_D2_Model.Vpoint[i].x;
//		if(m_D2_Model.Vpoint[i].x<Min) Min=m_D2_Model.Vpoint[i].x;
//		if(m_D2_Model.Vpoint[i].y>Max) Max=m_D2_Model.Vpoint[i].y;
//		if(m_D2_Model.Vpoint[i].y<Min) Min=m_D2_Model.Vpoint[i].y;
//		if(m_D2_Model.Vpoint[i].z>Max) Max=m_D2_Model.Vpoint[i].z;
//		if(m_D2_Model.Vpoint[i].z<Min) Min=m_D2_Model.Vpoint[i].z;
//	}
//	Min=(long)abs((int)Min);
//	if(Min>Max) Max=Min;
//	m_D2_Model.MaxXYZ=(float)Max;
//	m_D2_Model.MaxXYZ/=4096;
//}
//
//
//void CMODVIEW32Doc::D2_ReadD2HAM()
//{
//	if(!m_D2_MainHAMLoaded)
//	{
//		CFile fx;
//		CString tstr;
//
//		SetStatusBarText("Loading D2 main tables...");
//		COptionsDlg dlg;
//		tstr=dlg.GetD2Path();
//		tstr+="DESCENT2.HAM";
//		if(!fx.Open(tstr,CFile::modeRead | CFile::typeBinary))
//			return;
//		D2_InitHAM(&fx,FALSE);
//		fx.Close();
//		//ERROR CHECK!
//		m_D2_MainHAMLoaded=TRUE;
//		SetStatusBarText("Loading model...");
//	}
//}
//
//CString CMODVIEW32Doc::D2_GenerateModelTypeName(int type)
//{
//	CString ret;
//	switch(type & D2_POLYMODEL_TYPE_TYPESBITS)
//	{
//	case D2_POLYMODEL_TYPE_ROBOT:
//		ret.Format("Robot");
//		break;
//
//	case D2_POLYMODEL_TYPE_REACTOR:
//		ret.Format("Reactor");
//		break;
//
//	case D2_POLYMODEL_TYPE_PLAYERSHIP:
//		ret.Format("Player Ship");
//		break;
//
//	case D2_POLYMODEL_TYPE_MARKER:
//		ret.Format("Marker");
//		break;
//
//	case D2_POLYMODEL_TYPE_WEAPON:
//		ret.Format("Weapon");
//		break;
//
//	case D2_POLYMODEL_TYPE_WEAPONINNER:
//		ret.Format("Weapon - INNER");
//		break;
//
//	default:
//		ret.Format("<Unknown>");
//	}
//
//	if(type & D2_POLYMODEL_TYPE_FLAG_LORES)
//		ret+=" - LORES";
//	if(type & D2_POLYMODEL_TYPE_FLAG_DESTROYED)
//		ret+=" - DESTROYED";
//	if(type & D2_POLYMODEL_TYPE_FLAG_DYING)
//		ret+=" - DYING";
//	return ret;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////////////
//// Red Faction stuff
//////////////////////////////////////////////////////////////////////////////////////////
//
//ERRORCODE CMODVIEW32Doc::RF_ReadV3C(CFile *fp)
//{
//	int m_RF__unknown;
//	VERIFY(read_INT32(fp)==ID_MCFR);
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown=read_INT32(fp);
//	m_RF_Model.num_surfaces=read_INT32(fp);
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown=read_INT32(fp);
//
//	//RFV3_SubMeshBlock1
//	CString m_RF_Name,m_RF_None;
//	int m_RF__unknown_B;
//	VERIFY(read_INT32(fp)==ID_MBUS);
//	m_RF__unknown=read_INT32(fp);
//	m_RF_Name=read_STRINGfixed(fp,24);
//	m_RF_None=read_STRINGfixed(fp,24);
//TRACE("%s - %s\n",m_RF_Name,m_RF_None);
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown_B=read_INT32(fp);
//	if(m_RF__unknown_B==2)
//		m_RF__unknown=read_INT32(fp);
//	if(m_RF__unknown_B==3)
//	{
//		m_RF__unknown=read_INT32(fp);
//		m_RF__unknown=read_INT32(fp);
//	}
//
//	//RFV3_SubMeshBlock2
//	int m_RF__unknown_A,m_RF__unknown_C;
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown=read_INT32(fp);
//	m_RF_Model.radius=read_float(fp);
//	m_RF_Model.mins[0]=read_float(fp);
//	m_RF_Model.mins[1]=read_float(fp);
//	m_RF_Model.mins[2]=read_float(fp);
//	m_RF_Model.maxs[0]=read_float(fp);
//	m_RF_Model.maxs[1]=read_float(fp);
//	m_RF_Model.maxs[2]=read_float(fp);
//	m_RF__unknown=read_INT32(fp);
//	m_RF__unknown_A=read_INT32(fp);
//	m_RF_Model.num_geounits=read_INT16(fp);
//	m_RF__unknown_C=read_INT32(fp);
//
//
//	//RFV3_GeoUnit *geounits=new RFV3_GeoUnit[subheader2.mNumGeoUnits];
//	//fread( geounits, subheader2.mNumGeoUnits, sizeof(RFV3_GeoUnit), fp );
//	//segsize += subheader2.mNumGeoUnits * sizeof(RFV3_GeoUnit);
//	//printf( "      unk[0]          : %10i (0x%08X)\n", geounits[i].unk[0], geounits[i].unk[0] );
//
//	int segsize=0;
//	m_RF_Model.geounit=new RF_GEOUNIT[m_RF_Model.num_geounits];
//	segsize += m_RF_Model.num_geounits * sizeof(RF_GEOUNIT);
//
//	for(int i=0;i<m_RF_Model.num_geounits;i++)
//	{
//		fp->Read(&m_RF_Model.geounit[i],sizeof(RF_GEOUNIT));
//
//		TRACE( "   GeoUnit %i Info\n", i );
//
//		// 17344  125290556
//		// 125307900
//		// 125308716
//
//		// Index Position
//		TRACE( "      unk[0]          : %10i (0x%08X)\n", m_RF_Model.geounit[i].unk[0], m_RF_Model.geounit[i].unk[0] );
//		// Index Normals
//		TRACE( "      unk[1]          : %10i (0x%08X)\n", m_RF_Model.geounit[i].unk[1], m_RF_Model.geounit[i].unk[1] );
//		// Index UV's
//		TRACE( "      unk[2]          : %10i (0x%08X)\n", m_RF_Model.geounit[i].unk[2], m_RF_Model.geounit[i].unk[2] );
//		// Index Tris
//		TRACE( "      unk[3]          : %10i (0x%08X)\n", m_RF_Model.geounit[i].unk[3], m_RF_Model.geounit[i].unk[3] );
//		// Index ShortBlock+8
//		TRACE( "      unk[4]          : %10i (0x%08X)\n", m_RF_Model.geounit[i].unk[4], m_RF_Model.geounit[i].unk[4] );
//		// Index DwordBlock
//		TRACE( "      unk[5]          : %10i (0x%08X)\n", m_RF_Model.geounit[i].unk[5], m_RF_Model.geounit[i].unk[5] );
//		TRACE( "      mIndex          : %10i (0x%08X)\n", m_RF_Model.geounit[i].mIndex, m_RF_Model.geounit[i].mIndex );
//		TRACE( "      unk1            : %10i (0x%08X)\n", m_RF_Model.geounit[i].unk1, m_RF_Model.geounit[i].unk1 );
//		TRACE( "      NumTris         : %10i (0x%08X)\n", m_RF_Model.geounit[i].mNumTris, m_RF_Model.geounit[i].mNumTris );
//		TRACE( "      NumVerts        : %10i (0x%08X)\n", m_RF_Model.geounit[i].mNumVerts, m_RF_Model.geounit[i].mNumVerts );
//		// Size of the Normal and Position Block
//		TRACE( "      size_3float     : %10i (0x%08X)\n", m_RF_Model.geounit[i].size_3float, m_RF_Model.geounit[i].size_3float );
//		// Size of the Triangle Block
//		TRACE( "      size_tris       : %10i (0x%08X)\n", m_RF_Model.geounit[i].size_tris, m_RF_Model.geounit[i].size_tris );
//		// Size of the UV Block
//		TRACE( "      size_2float     : %10i (0x%08X)\n", m_RF_Model.geounit[i].size_2float, m_RF_Model.geounit[i].size_2float );
//		// Size of the Dword block
//		TRACE( "      size_dwordblock : %10i (0x%08X)\n", m_RF_Model.geounit[i].size_dwordblock, m_RF_Model.geounit[i].size_dwordblock );
//		// sizeof the short block
//		TRACE( "      size_shortblock : %10i (0x%08X)\n", m_RF_Model.geounit[i].size_shortblock, m_RF_Model.geounit[i].size_shortblock );
//		TRACE( "      unk2            : %10i (0x%08X)\n", m_RF_Model.geounit[i].unk2, m_RF_Model.geounit[i].unk2 );
//	}
//
//	m_RF_Model.modeldata=(RF_MODELDATA *)new RF_MODELDATA[m_RF_Model.num_geounits];
//	for(int i=0;i<m_RF_Model.num_geounits;i++)
//	{
//		m_RF_Model.modeldata[i].vertices=(RF_VPNT *)new char[m_RF_Model.geounit[i].size_3float];
//		m_RF_Model.modeldata[i].normals=(RF_VPNT *)new char[m_RF_Model.geounit[i].size_3float];
//		m_RF_Model.modeldata[i].uvs=(RF_UVS *)new char[m_RF_Model.geounit[i].size_2float];
//		m_RF_Model.modeldata[i].triangles=(RF_TRIS *)new char[m_RF_Model.geounit[i].size_tris];
//
//		fp->Read(m_RF_Model.modeldata[i].vertices,m_RF_Model.geounit[i].size_3float);
//		fp->Read(m_RF_Model.modeldata[i].normals,m_RF_Model.geounit[i].size_3float);
//		fp->Read(m_RF_Model.modeldata[i].uvs,m_RF_Model.geounit[i].size_2float);
//		fp->Read(m_RF_Model.modeldata[i].triangles,m_RF_Model.geounit[i].size_tris);
//		fp->Seek(m_RF_Model.geounit[i].size_shortblock,CFile::current);
//		fp->Seek(m_RF_Model.geounit[i].size_dwordblock,CFile::current);
//		//delete[] vertices;
//		//delete[] normals;
//		//delete[] uvs;
//		//delete[] triangles;
//
//		//Skip parts
//		int size=m_RF_Model.geounit[i].size_2float + 2*m_RF_Model.geounit[i].size_3float + m_RF_Model.geounit[i].size_dwordblock + m_RF_Model.geounit[i].size_shortblock + m_RF_Model.geounit[i].size_tris;
//		if( m_RF_Model.geounit[i].unk1 )
//		{
//			int bsize=((2*m_RF__unknown_A+15)>>4)<<4 ;
//			size+=bsize;
//			fp->Seek(bsize, CFile::current);
//		}
//
//		int fsize=((size+15)>>4)<<4;
//		if(fsize!=size)
//		{
//			fp->Seek(fsize-size, CFile::current);
//		}
//
//		segsize += fsize;
//	}
//	//delete[] m_RF_Model.geounit;
//
//	if(segsize!=m_RF__unknown_C)
//	{
//		fp->Seek(m_RF__unknown_C-segsize,CFile::current);
//	}
//
//
//	//Textures
//	fp->Seek(8+m_RF_Model.num_geounits*14,CFile::current);
//
//	m_RF_Model.num_textures=read_INT32(fp);
////TRACE("numtextures=%i\n",m_RF_Model.num_textures);
//
//	for(int i=0 ; i<m_RF_Model.num_textures; i++ )
//	{
//		char textureid=read_INT8(fp);
//		m_RF_Model.texture[i].filename=read_STRINGterminated(fp);
////TRACE("texture num=%i id=%i name=%s\n",i,textureid,texname);
//
//		//int			width, height;
//
//
//		//glBindTexture(GL_TEXTURE_2D,m_RF_Model.texturedata[i]);
//
//		CString fname="D:\\Games\\Red Faction International Demo\\maps\\"+m_RF_Model.texture[i].filename;
//		//RGBA32_t	*texbuf=NULL;
//		TGA_LoadRGBA(fname, m_RF_Model.texture[i].buffer,m_RF_Model.texture[i].width,m_RF_Model.texture[i].height);
//		//delete[] texbuf;
//	}
//
//	m_Game=GAME_RF;
//	m_DoResetGeometry=TRUE;
//	m_ModelLoaded=TRUE;
//
//	return 0; 
//}
//
////Placeholder TODO
//ERRORCODE CMODVIEW32Doc::ReadRFVPP()
//{
//	return 0;
//}

int _2048isize(int x)
{
	if(x%2048==0)
		return x;
	x-=x%2048;
	x+=2048;
	return x;
}

void _2048imove(CFile *fp)
{
	fp->Seek(_2048isize((int) fp->GetPosition()),CFile::begin);
}

//void CMODVIEW32Doc::RF_InitVPP(CFile *fp, VPP_HEADER *vpp_header, VPP_LOAD **vpp_load, BOOL fillexplorer)
//{
//	VPP_LOAD *vpp_load_=*vpp_load;
//	if(fillexplorer)
//	{
//		char tempfname[8192];
//		strcpy_s(tempfname,fp->GetFileName());
//		ExplorerClear();
//		ExplorerAddRoot((char *)&tempfname);
//	}
//
//	vpp_header->signature=read_INT32(fp);
//	vpp_header->version=read_INT32(fp);
//	vpp_header->num_files=read_INT32(fp);
//	vpp_header->filesize=read_INT32(fp);
//	_2048imove(fp);
//	int filepos=2048+_2048isize(vpp_header->num_files*64);
//	vpp_load_=new VPP_LOAD[vpp_header->num_files];
//	*vpp_load=vpp_load_;
//
//	for(int i=0;i<vpp_header->num_files;i++)
//	{
//		vpp_load_[i].filename=read_STRINGfixed(fp,60);
//		vpp_load_[i].size=read_INT32(fp);
//		vpp_load_[i].offset=filepos;
//		filepos+=_2048isize(vpp_load_[i].size);
//
//		if(fillexplorer)
//		{
//			CString x=vpp_load_[i].filename;
//			x.MakeUpper();
//			if(x.Right(4)==".V3C")
//				ExplorerAddChild(vpp_load_[i].filename);
//		}
//	}
//	if(fillexplorer)
//		ExplorerGoTop();
//}
//
//ERRORCODE CMODVIEW32Doc::RF_LoadVPPContent(CString filename)
//{
//	int size=-1;
//	int offset=-1;
//	for(int i=0;i<m_RF_CurrVPP_Header.num_files;i++)
//	{
//		if(filename==m_RF_CurrVPP_Load[i].filename)
//		{
//			size=m_RF_CurrVPP_Load[i].size;
//			offset=m_RF_CurrVPP_Load[i].offset;
//		}
//	}
//
//	if(offset==-1)
//		return -1; //TODO: Assign an error code
//	else
//	{
//		CFile f;
//		f.Open(m_RF_CurrVPP_Filename,CFile::modeRead);
//		f.Seek(offset,CFile::begin);
//		/*return */RF_ReadV3C(&f);
//		return 0;
//	}
//}


////////////////////////////////////////////////////////////////////////////////////////
// EditorFS stuff
////////////////////////////////////////////////////////////////////////////////////////

//Save file
BOOL CMODVIEW32Doc::EditorFS_SavePOF(CString filename, int fileversion)
{
	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());

	//Check if valid
	CString err="";
	CString newerr;
	FS_VPNT t;
	//Check #1: Gun with NULL normal
	for(int guntype=0;guntype<2;guntype++)
	{
		for(int gun=0;gun<m_FS_RealGuns[guntype].Num;gun++)
		{
			for(int i=0;i<m_FS_RealGuns[guntype].Slot[gun].num_guns;i++)
			{
				t=m_FS_RealGuns[guntype].Slot[gun].normal[i];
				if(t.x==0 && t.y==0 && t.z==0)
				{
					if(guntype==0)
						newerr.Format("Laser Gun %i Slot %i has a NULL normal vector.\r\n",gun,i);
					else
						newerr.Format("Missile Gun %i Slot %i has a NULL normal vector.\r\n",gun,i);
					err+=newerr;
				}
			}
		}
	}
	//Check #2: Turrets with NULL normal
	for(int guntype=0;guntype<2;guntype++)
	{
		for(int gun=0;gun<m_FS_Turrets[guntype].Num;gun++)
		{
			t=m_FS_Turrets[guntype].Bank[gun].turret_normal;
			if(t.x==0 && t.y==0 && t.z==0)
			{
				if(guntype==0)
					newerr.Format("Gun Turret %i has a NULL normal vector.\r\n",gun);
				else
					newerr.Format("Missile Turret %i has a NULL normal vector.\r\n",gun);
				err+=newerr;
			}
		}
	}
	//Check #3: Eye point normal NULL
	if(m_FS_ModelInfo.num_eye_points>0)
	{
		t=m_FS_ModelInfo.eye_point_normal;
		if(t.x==0 && t.y==0 && t.z==0)
		{
			newerr.Format("Eye Point normal is a NULL vector.\r\n");
			err+=newerr;
		}
	}
	//Check #4: AutoCenter point in FreeSpace 1 file
	t=m_FS_ModelInfo.ACenPoint;
	if((t.x!=0 || t.y!=0 || t.z!=0) && (m_FS_CurrVP_FreeSpaceVersion==1))
	{
		newerr.Format("AutoCenter point is not supported in FreeSpace 1. It will be removed if you continue.\r\n");
		err+=newerr;
	}
	//Check #5: Insignia in FreeSpace 1 file
	if(m_EditorFS_ChunkINSG.chunk_size>0 && m_FS_CurrVP_FreeSpaceVersion==1)
	{
		newerr.Format("Insignias (for Squad logos) are not supported in FreeSpace 1. It will be removed if you continue.\r\n");
		err+=newerr;
	}
	//Check #6: Thruster properties
	for(int thr=0;thr<m_FS_Model.num_thrusters;thr++)
	{
		if(m_FS_Model.thrusters[thr].properties!="" && m_FS_CurrVP_FreeSpaceVersion==1)
		{
			newerr.Format("Properties for thruster #%i will be removed (\"%s\")",thr,m_FS_Model.thrusters[thr].properties);
			err+=newerr;
		}
	}

	//Now output result
	if(err!="")
	{
		CCompilerErrorDlg dlg;
		dlg.errstring=err;
		if(dlg.DoModal()==IDCANCEL)
			return FALSE;
	}

	//Continue with saving
	CFile f;
	if(!f.Open(filename,CFile::modeCreate | CFile::modeWrite))
	{
		viewFrame->MessageBox("Could not open file for writing!","Failed");
		return FALSE;
	}

	//Update shields backup data in memory
	m_EditorFS_ShieldsBackup=m_FS_Model.shields;

	//In FreeSpace 1 mode remove ACEN and INSG and thruster properties
	if(m_FS_CurrVP_FreeSpaceVersion==1)
	{
		m_EditorFS_ChunkINSG.ClearChunk();
		//Remove INSG!
		for(int thr=0;thr<m_FS_Model.num_thrusters;thr++)
			m_FS_Model.thrusters[thr].properties="";
	}

	//Set FS Version (we always set to the newest version available for the game, converting older versions to this one)
	if(m_FS_CurrVP_FreeSpaceVersion==1)
		m_FileVersion=2014;
	else
		m_FileVersion=2117;

	//Generate new chunks
	m_EditorFS_ChunkTXTR.CreateChunk(EditorFS_GenerateTextures());
	m_EditorFS_ChunkOHDR.CreateChunk(EditorFS_GenerateModelHeader());
	m_EditorFS_ChunkSPCL.CreateChunk(EditorFS_GenerateSubsystems());
	m_EditorFS_ChunkSHLD.CreateChunk(EditorFS_GenerateShields());
	m_EditorFS_ChunkACEN.CreateChunk(EditorFS_GenerateACEN());
	m_EditorFS_ChunkFUEL.CreateChunk(EditorFS_GenerateThrusters());
	m_EditorFS_ChunkTGUN.CreateChunk(EditorFS_GenerateTurrets(ID_TGUN));
	m_EditorFS_ChunkTMIS.CreateChunk(EditorFS_GenerateTurrets(ID_TMIS));
	m_EditorFS_ChunkGPNT.CreateChunk(EditorFS_GenerateRealGuns(ID_GPNT));
	m_EditorFS_ChunkMPNT.CreateChunk(EditorFS_GenerateRealGuns(ID_MPNT));
	for(int i=0;i<m_FS_NumSOBJ;i++)
		m_EditorFS_ChunkSOBJ[i].CreateChunk(EditorFS_GenerateSOBJ(i));

	//Write file signature
	{
		int sig=0x4f505350;
		f.Write(&sig,4);
		f.Write(&m_FileVersion,4);
	}

	//Write chunks
	m_EditorFS_ChunkTXTR.WriteChunk(&f);
	m_EditorFS_ChunkOHDR.WriteChunk(&f);
	for(int i=0;i<m_FS_NumSOBJ;i++)
		m_EditorFS_ChunkSOBJ[i].WriteChunk(&f);
	m_EditorFS_ChunkSPCL.WriteChunk(&f);
	m_EditorFS_ChunkGPNT.WriteChunk(&f);
	m_EditorFS_ChunkMPNT.WriteChunk(&f);
	m_EditorFS_ChunkTGUN.WriteChunk(&f);
	m_EditorFS_ChunkTMIS.WriteChunk(&f);
	m_EditorFS_ChunkDOCK.WriteChunk(&f);
	m_EditorFS_ChunkFUEL.WriteChunk(&f);
	m_EditorFS_ChunkSHLD.WriteChunk(&f);
	m_EditorFS_Chunk_EYE.WriteChunk(&f);
	m_EditorFS_ChunkINSG.WriteChunk(&f);
	m_EditorFS_ChunkACEN.WriteChunk(&f);
	m_EditorFS_ChunkPATH.WriteChunk(&f);
	m_EditorFS_ChunkPINF.WriteChunk(&f);

	f.Close();
	return TRUE;
}

unsigned char * CMODVIEW32Doc::EditorFS_GenerateSubsystems()
{
	if(m_FS_NumSubsystems==0)
		return NULL;

	unsigned char *buffer=new unsigned char[32768];
	int ptr=0;
	int type=ID_SPCL;
	memcpy(buffer+ptr,&type,4); ptr+=4;
	int size=0;
	memcpy(buffer+ptr,&size,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_NumSubsystems,4); ptr+=4;
	int slen,si; unsigned char ch;
	for(int subsys=0;subsys<m_FS_NumSubsystems;subsys++)
	{
		slen=m_FS_Subsystems[subsys].Name.GetLength();
		memcpy(buffer+ptr,&slen,4); ptr+=4;
		for(si=0;si<slen;si++)
		{
			ch=m_FS_Subsystems[subsys].Name[si];
			memcpy(buffer+ptr,&ch,1); ptr+=1;
		}
		slen=m_FS_Subsystems[subsys].Properties.GetLength();
		memcpy(buffer+ptr,&slen,4); ptr+=4;
		for(si=0;si<slen;si++)
		{
			ch=m_FS_Subsystems[subsys].Properties[si];
			memcpy(buffer+ptr,&ch,1); ptr+=1;
		}
		memcpy(buffer+ptr,&m_FS_Subsystems[subsys].Center.x,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Subsystems[subsys].Center.y,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Subsystems[subsys].Center.z,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Subsystems[subsys].Radius,4); ptr+=4;
	}
	size=ptr-8;
	memcpy(buffer+4,&size,4); //size
	return buffer;
}

unsigned char * CMODVIEW32Doc::EditorFS_GenerateThrusters()
{
	if(m_FS_Model.num_thrusters==0)
		return NULL;

	unsigned char *buffer=new unsigned char[32768];
	int ptr=0;
	int type=ID_FUEL;
	memcpy(buffer+ptr,&type,4); ptr+=4;
	int size=0;
	memcpy(buffer+ptr,&size,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_Model.num_thrusters,4); ptr+=4;
	int slen,si; unsigned char ch;
	for(int thr=0;thr<m_FS_Model.num_thrusters;thr++)
	{
		memcpy(buffer+ptr,&m_FS_Model.thrusters[thr].num_glows,4); ptr+=4;
		if(m_FileVersion>=2117)
		{
			slen=m_FS_Model.thrusters[thr].properties.GetLength();
			memcpy(buffer+ptr,&slen,4); ptr+=4;
			for(si=0;si<slen;si++)
			{
				ch=m_FS_Model.thrusters[thr].properties[si];
				memcpy(buffer+ptr,&ch,1); ptr+=1;
			}
		}
		for(int glow=0;glow<m_FS_Model.thrusters[thr].num_glows;glow++)
		{
			memcpy(buffer+ptr,&m_FS_Model.thrusters[thr].glow_pos[glow].x,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_Model.thrusters[thr].glow_pos[glow].y,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_Model.thrusters[thr].glow_pos[glow].z,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_Model.thrusters[thr].glow_norm[glow].x,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_Model.thrusters[thr].glow_norm[glow].y,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_Model.thrusters[thr].glow_norm[glow].z,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_Model.thrusters[thr].glow_radius[glow],4); ptr+=4;
		}
	}
	size=ptr-8;
	memcpy(buffer+4,&size,4); //size
	return buffer;
}

unsigned char * CMODVIEW32Doc::EditorFS_GenerateTurrets(int type)
{
	ASSERT(type==ID_TGUN || type==ID_TMIS);
	int tt=0; //turrettype
	if(type==ID_TMIS)
		tt=1;

	if(m_FS_Turrets[tt].Num==0)
		return NULL;

	unsigned char *buffer=new unsigned char[32768];
	int ptr=0;
	memcpy(buffer+ptr,&type,4); ptr+=4;
	int size=0;
	memcpy(buffer+ptr,&size,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_Turrets[tt].Num,4); ptr+=4;
	for(int bank=0;bank<m_FS_Turrets[tt].Num;bank++)
	{
		memcpy(buffer+ptr,&m_FS_Turrets[tt].Bank[bank].sobj_parent,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Turrets[tt].Bank[bank].sobj_par_phys,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Turrets[tt].Bank[bank].turret_normal.x,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Turrets[tt].Bank[bank].turret_normal.y,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Turrets[tt].Bank[bank].turret_normal.z,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Turrets[tt].Bank[bank].num_firing_points,4); ptr+=4;
		for(int firpnt=0;firpnt<m_FS_Turrets[tt].Bank[bank].num_firing_points;firpnt++)
		{
			memcpy(buffer+ptr,&m_FS_Turrets[tt].Bank[bank].firing_point[firpnt].x,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_Turrets[tt].Bank[bank].firing_point[firpnt].y,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_Turrets[tt].Bank[bank].firing_point[firpnt].z,4); ptr+=4;
		}
	}
	size=ptr-8;
	memcpy(buffer+4,&size,4); //size
	return buffer;
}

unsigned char * CMODVIEW32Doc::EditorFS_GenerateRealGuns(int type)
{
	ASSERT(type==ID_GPNT || type==ID_MPNT);
	int rg=0; //realguntype
	if(type==ID_MPNT)
		rg=1;

	if(m_FS_RealGuns[rg].Num==0)
		return NULL;

	unsigned char *buffer=new unsigned char[32768];
	int ptr=0;
	memcpy(buffer+ptr,&type,4); ptr+=4;
	int size=0;
	memcpy(buffer+ptr,&size,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_RealGuns[rg].Num,4); ptr+=4;
	for(int slot=0;slot<m_FS_RealGuns[rg].Num;slot++)
	{
		memcpy(buffer+ptr,&m_FS_RealGuns[rg].Slot[slot].num_guns,4); ptr+=4;
		for(int firpnt=0;firpnt<m_FS_RealGuns[rg].Slot[slot].num_guns;firpnt++)
		{
			memcpy(buffer+ptr,&m_FS_RealGuns[rg].Slot[slot].point[firpnt].x,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_RealGuns[rg].Slot[slot].point[firpnt].y,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_RealGuns[rg].Slot[slot].point[firpnt].z,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_RealGuns[rg].Slot[slot].normal[firpnt].x,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_RealGuns[rg].Slot[slot].normal[firpnt].y,4); ptr+=4;
			memcpy(buffer+ptr,&m_FS_RealGuns[rg].Slot[slot].normal[firpnt].z,4); ptr+=4;
		}
	}
	size=ptr-8;
	memcpy(buffer+4,&size,4); //size
	return buffer;
}

unsigned char * CMODVIEW32Doc::EditorFS_GenerateSOBJ(int subm)
{
	ASSERT(subm<m_FS_NumSOBJ);
	if (subm >= MAX_FS_SOBJ) return NULL;
	ASSERT(m_FS_CurrVP_FreeSpaceVersion==1 || m_FS_CurrVP_FreeSpaceVersion==2);
	ASSERT(m_FS_SOBJ[subm].bsp_data_size>=0);
	ASSERT(m_FS_SOBJ[subm].reserved==0);

	int buffersize=1024+m_FS_SOBJ[subm].bsp_data_size;
	unsigned char *buffer=new unsigned char[buffersize];
	int ptr=0;
	int type;
	if(m_FS_CurrVP_FreeSpaceVersion==1)
		type=ID_SOBJ;
	else
		type=ID_OBJ2;
	memcpy(buffer+ptr,&type,4); ptr+=4;
	int size=0;
	memcpy(buffer+ptr,&size,4); ptr+=4;

	memcpy(buffer+ptr,&m_FS_SOBJ[subm].submodel_number,4); ptr+=4;
	if(m_FS_CurrVP_FreeSpaceVersion==1)
	{
		memcpy(buffer+ptr,&m_FS_SOBJ[subm].submodel_parent,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_SOBJ[subm].real_offset.x,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_SOBJ[subm].real_offset.y,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_SOBJ[subm].real_offset.z,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_SOBJ[subm].radius,4); ptr+=4;
	} else {
		memcpy(buffer+ptr,&m_FS_SOBJ[subm].radius,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_SOBJ[subm].submodel_parent,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_SOBJ[subm].real_offset.x,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_SOBJ[subm].real_offset.y,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_SOBJ[subm].real_offset.z,4); ptr+=4;
	}
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].geometric_center.x,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].geometric_center.y,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].geometric_center.z,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].bounding_box_min_point.x,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].bounding_box_min_point.y,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].bounding_box_min_point.z,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].bounding_box_max_point.x,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].bounding_box_max_point.y,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].bounding_box_max_point.z,4); ptr+=4;

	int slen,si; unsigned char ch;
	slen=m_FS_SOBJ[subm].submodel_name.GetLength();
	memcpy(buffer+ptr,&slen,4); ptr+=4;
	for(si=0;si<slen;si++)
	{
		ch=m_FS_SOBJ[subm].submodel_name[si];
		memcpy(buffer+ptr,&ch,1); ptr+=1;
	}
	slen=m_FS_SOBJ[subm].properties.GetLength();
	memcpy(buffer+ptr,&slen,4); ptr+=4;
	for(si=0;si<slen;si++)
	{
		ch=m_FS_SOBJ[subm].properties[si];
		memcpy(buffer+ptr,&ch,1); ptr+=1;
	}

	memcpy(buffer+ptr,&m_FS_SOBJ[subm].movement_type,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].movement_axis,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].reserved,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_SOBJ[subm].bsp_data_size,4); ptr+=4;
	memcpy(buffer+ptr,m_FS_SOBJ[subm].bsp_data,m_FS_SOBJ[subm].bsp_data_size); ptr+=m_FS_SOBJ[subm].bsp_data_size;

	size=ptr-8;
	memcpy(buffer+4,&size,4); //size
	return buffer;
}

unsigned char * CMODVIEW32Doc::EditorFS_GenerateTextures()
{
	ASSERT(m_FS_BitmapData.count<MAX_FS_TEXTURE);

	int buffersize=1024;
	unsigned char *buffer=new unsigned char[buffersize];
	int ptr=0;
	int type=ID_TXTR;
	memcpy(buffer+ptr,&type,4); ptr+=4;
	int size=0;
	memcpy(buffer+ptr,&size,4); ptr+=4;

	memcpy(buffer+ptr,&m_FS_BitmapData.count,4); ptr+=4;
	for(unsigned int i=0;i<m_FS_BitmapData.count;i++)
	{
		int slen,si; unsigned char ch; CString tmp;
		tmp=m_FS_BitmapData.pic[i].name;
		slen=tmp.GetLength();
		memcpy(buffer+ptr,&slen,4); ptr+=4;
		for(si=0;si<slen;si++)
		{
			ch=tmp[si];
			memcpy(buffer+ptr,&ch,1); ptr+=1;
		}
	}

	size=ptr-8;
	memcpy(buffer+4,&size,4); //size
	return buffer;
}

unsigned char * CMODVIEW32Doc::EditorFS_GenerateModelHeader()
{
	int type;
	if(m_FS_CurrVP_FreeSpaceVersion==1)
		type=ID_OHDR;
	else
		type=ID_HDR2;

	unsigned char *buffer=new unsigned char[32768];
	int ptr=0;
	memcpy(buffer+ptr,&type,4); ptr+=4;
	int size=0;
	memcpy(buffer+ptr,&size,4); ptr+=4;

	if(m_FS_CurrVP_FreeSpaceVersion==1)
	{	memcpy(buffer+ptr,&m_FS_NumSOBJ,4); ptr+=4;	}
	memcpy(buffer+ptr,&m_FS_Model.radius,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_ModelInfo.obj_flags,4); ptr+=4;
	if(m_FS_CurrVP_FreeSpaceVersion==2)
	{	memcpy(buffer+ptr,&m_FS_NumSOBJ,4); ptr+=4;	}
	memcpy(buffer+ptr,&m_FS_Model._minbox.x,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_Model._minbox.y,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_Model._minbox.z,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_Model._maxbox.x,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_Model._maxbox.y,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_Model._maxbox.z,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_PMinfo.n_detail,4); ptr+=4;
	for(unsigned int i=0;i<m_FS_PMinfo.n_detail;i++)
	{	memcpy(buffer+ptr,&m_FS_PMinfo.detail[i],4); ptr+=4;	}
	memcpy(buffer+ptr,&m_FS_PMinfo.n_debris,4); ptr+=4;
	for(unsigned int i=0;i<m_FS_PMinfo.n_debris;i++)
	{	memcpy(buffer+ptr,&m_FS_PMinfo.debris[i],4); ptr+=4;	}
	memcpy(buffer+ptr,&m_FS_ModelInfo.mass,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_ModelInfo.mass_center.x,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_ModelInfo.mass_center.y,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_ModelInfo.mass_center.z,4); ptr+=4;
	for(int i=0;i<3;i++) { for(int j=0;j<3;j++)
	{	memcpy(buffer+ptr,&m_FS_ModelInfo.moment_inertia[j][i],4); ptr+=4;	}	}
	memcpy(buffer+ptr,&m_FS_ModelInfo.num_cross_sections,4); ptr+=4;
	for(int i=0;i<m_FS_ModelInfo.num_cross_sections;i++)
	{	memcpy(buffer+ptr,&m_FS_ModelInfo.cross_section_depth[i],4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_ModelInfo.cross_section_radius[i],4); ptr+=4;	}
	memcpy(buffer+ptr,&m_FS_ModelInfo.num_lights,4); ptr+=4;
	for(int i=0;i<m_FS_ModelInfo.num_lights;i++)
	{	memcpy(buffer+ptr,&m_FS_ModelInfo.light_location[i].x,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_ModelInfo.light_location[i].y,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_ModelInfo.light_location[i].z,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_ModelInfo.light_type[i],4); ptr+=4;	}

	size=ptr-8;
	memcpy(buffer+4,&size,4); //size
	return buffer;
}

unsigned char * CMODVIEW32Doc::EditorFS_GenerateShields()
{
	if(m_FS_Model.shields.Fcount==0 && m_FS_Model.shields.Vcount==0)
		return NULL;

	unsigned char *buffer=new unsigned char[32768];
	int ptr=0;
	int type=ID_SHLD;
	memcpy(buffer+ptr,&type,4); ptr+=4;
	int size=0;
	memcpy(buffer+ptr,&size,4); ptr+=4;
	memcpy(buffer+ptr,&m_FS_Model.shields.Vcount,4); ptr+=4;
	for(unsigned int vert=0;vert<m_FS_Model.shields.Vcount;vert++)
	{	memcpy(buffer+ptr,&m_FS_Model.shields.Vpoint[vert][0],4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Model.shields.Vpoint[vert][1],4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Model.shields.Vpoint[vert][2],4); ptr+=4;	}
	memcpy(buffer+ptr,&m_FS_Model.shields.Fcount,4); ptr+=4;
	for(unsigned int face=0;face<m_FS_Model.shields.Fcount;face++)
	{	memcpy(buffer+ptr,&m_FS_Model.shields.Face[face].Normal.x,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Model.shields.Face[face].Normal.y,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Model.shields.Face[face].Normal.z,4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Model.shields.Face[face].Vface[0],4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Model.shields.Face[face].Vface[1],4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Model.shields.Face[face].Vface[2],4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Model.shields.Face[face].Nface[0],4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Model.shields.Face[face].Nface[1],4); ptr+=4;
		memcpy(buffer+ptr,&m_FS_Model.shields.Face[face].Nface[2],4); ptr+=4;	}

	size=ptr-8;
	memcpy(buffer+4,&size,4); //size
	return buffer;
}

unsigned char * CMODVIEW32Doc::EditorFS_GenerateACEN()
{
	if(m_FS_CurrVP_FreeSpaceVersion==1)
		return NULL;
	FS_VPNT t=m_FS_ModelInfo.ACenPoint;
	if(t.x==0.0 && t.y==0 && t.z==0)
		return NULL;

	unsigned char *buffer=new unsigned char[128];
	int ptr=0;
	int type=ID_ACEN;
	memcpy(buffer+ptr,&type,4); ptr+=4;
	int size=0;
	memcpy(buffer+ptr,&size,4); ptr+=4;
	memcpy(buffer+ptr,&t.x,4); ptr+=4;
	memcpy(buffer+ptr,&t.y,4); ptr+=4;
	memcpy(buffer+ptr,&t.z,4); ptr+=4;

	size=ptr-8;
	memcpy(buffer+4,&size,4); //size
	return buffer;
}

BOOL CMODVIEW32Doc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
	if(!viewFrame->EditorFS_FinishPendingEditing())
		return FALSE;

	BOOL ret=EditorFS_SavePOF(lpszPathName,-1);
	if(ret)
		SetModifiedFlag(FALSE);
	return ret;
}

BOOL CMODVIEW32Doc::SaveModified() 
{
	POSITION pos=GetFirstViewPosition();	
	CMODVIEW32View *m_view=(CMODVIEW32View *)GetNextView(pos);
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_view->GetParentFrame());
	if(!viewFrame->EditorFS_FinishPendingEditing())
		return FALSE;
	
	return CDocument::SaveModified();
}

BOOL CMODVIEW32Doc::SaveModified2() 
{
	return SaveModified();
}

void CMODVIEW32Doc::OnFileSave2()
{
	OnFileSave();
}






