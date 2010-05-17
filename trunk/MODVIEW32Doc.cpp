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

	m_FS_MainVP_MainTextureSet=VP_FS_TEXTURESET_NONE;
	m_FS_MainVP_Loaded=FALSE;
	m_Game=GAME_NONE;

	m_InfoMode=FALSE;
	
	m_DoResetGeometry=FALSE;
	m_AutoZoom=-50.0f;
	bSize=TRUE;

	m_ModelLoaded=FALSE;

	for(int i=0;i<MAX_FS_SOBJ;i++)
	{
		m_FS_SOBJ[i].bsp_data=NULL;
		m_FS_SOBJ[i].bsp_data_size=0;
	}

	//Get Main Path
//	char path_main[MAX_FILENAMELEN];
	COptionsDlg dlg;

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

	m_FS_CurrVP_PofNum=0;
	m_ModelLoaded=FALSE;
	m_Textures_Available=FALSE;
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
		viewFrame->GameBarNone();
		viewFrame->UpdateTextureBar();
		viewFrame->UpdateTextureBar();
	}

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

	m_TexturesNum=-1;

	BOOL supportedfiletype=FALSE;

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
			FS_LoadTextureData2();	//Load textures from main .VP file
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

unsigned int xsize,ysize;
unsigned int xreal,yreal; 
unsigned char XScale,YScale;
unsigned int MaxSize;

ERRORCODE CMODVIEW32Doc::locatetexture(char* texturename, int texturenum, int current_tex) {

	// check first for a pcx file
	char Filename[50];
	char* extension;
	char* type;
	bool unknownfiletype = true;
	
	for (int filetype = 1; filetype < MAXTEXTURETYPES; filetype++) {
	
		switch(filetype) {
			case TEXTUREINFOFILETYPE_DDS:
				extension = ".dds";
				type = "DDS Texture";
				break;
			case TEXTUREINFOFILETYPE_TGA:
				extension = ".tga";
				type = "TGA Texture";
				break;
			case TEXTUREINFOFILETYPE_PCX:
				extension = ".pcx";
				type = "PCX Texture";
				break;
			case TEXTUREINFOFILETYPE_EFF:
				extension = ".eff";
				type = "EFF animation";
				break;
			case TEXTUREINFOFILETYPE_ANI:
				extension = ".ani";
				type = "ANI animation";
				break;
		}
		
		strcpy_s(Filename, texturename);
		strcat_s(Filename, extension);
		TRACE("Trying to load %s",texturename);
		
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

		//Search process #1: Current directory
		{
			CString localfilename=localdirectory;
			localfilename+=Filename;
			CFile fcheck;
			if(fcheck.Open(localfilename,CFile::modeRead))
			{
				unknownfiletype=FALSE;
				TRACE(/*extension +*/ "...Found in current directory!\n");
				m_Textures[current_tex].Location="Current directory";
				m_Textures[current_tex].Type= type;
				m_Textures[current_tex].FileType=filetype;
				m_Textures[current_tex].InFilename=localfilename;
				m_Textures[current_tex].InOffset=0;
				m_Textures[current_tex].InSize=fcheck.GetLength();
				m_Textures[current_tex].Valid=TRUE;
			}
		}

		//Search process #2: Current .VP file
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
				TRACE(/*extension +*/ "...Found in current VP!\n");
				m_Textures[current_tex].Location= "Current VP";
				m_Textures[current_tex].Type= type;
				m_Textures[current_tex].FileType=filetype;
				m_Textures[current_tex].InFilename=m_FS_CurrVP_Filename;
				m_Textures[current_tex].InOffset=m_FS_CurrVP_Dir[j].offset;
				m_Textures[current_tex].InSize=m_FS_CurrVP_Dir[j].size;
				m_Textures[current_tex].Valid=TRUE;
			}
		}

		//Search process #3: /data/maps
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
				TRACE(/*extension +*/ "...Found in FreeSpace /data/maps directory!\n");
				m_Textures[current_tex].Location="/data/maps directory";
				m_Textures[current_tex].Type=type;
				m_Textures[current_tex].FileType=filetype;
				m_Textures[current_tex].InFilename=mapsfilename;
				m_Textures[current_tex].InOffset=0;
				m_Textures[current_tex].InSize=fcheck.GetLength();
				m_Textures[current_tex].Valid=TRUE;
			}
		}

		//Search process #4: Main .VP file
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
				TRACE(/*extension + */"...Found in main VP!\n");
				m_Textures[current_tex].Location = "Main FreeSpace VP";
				m_Textures[current_tex].Type = type;
				m_Textures[current_tex].FileType = filetype;
				m_Textures[current_tex].InFilename=m_FS_MainVP_Filename;
				m_Textures[current_tex].InOffset=m_FS_MainVP_Dir[j].offset;
				m_Textures[current_tex].InSize=m_FS_MainVP_Dir[j].size;
				m_Textures[current_tex].Valid=TRUE;
			} 
		}

		//Search process FAILED!
		if(unknownfiletype)
		{
			m_FS_BitmapData.pic[texturenum].valid=0;	// not ani or pcx...
			TRACE("No Texture found\n");
			m_Textures[current_tex].Location="<Not found>";
			m_Textures[current_tex].Type="<Unknown>";
			m_Textures[current_tex].Valid=FALSE;
			m_Textures[current_tex].Error="Not found";
		}
	}
	
	return ERROR_GEN_NOERROR;
}

void CMODVIEW32Doc::loadPCXtexture(TEXTUREINFO* t_info, FS_BITMAPDATA* bitmap_data, BOOL bFastload) {

	GLint MaxTextureSize[4];

	glGetIntegerv(GL_MAX_TEXTURE_SIZE,MaxTextureSize);
	MaxSize=MaxTextureSize[0];

	CFile f;
	//Header
	PCXHEADER header;
	f.Read(&header,sizeof(PCXHEADER));
	if(header.Version!=5)
	{
		bitmap_data->valid=0;
		t_info->Valid=FALSE;
		t_info->Error="Invalid PCX version";
		return;
	}

	xreal=header.Xmax-header.Xmin+1;
	yreal=header.Ymax-header.Ymin+1;
	t_info->Width=xreal;
	t_info->Height=yreal;

	unsigned int kb;
	kb=2; while(kb<xreal) kb*=2; xsize=kb;
	kb=2; while(kb<yreal) kb*=2; ysize=kb;

	//Palette
	if(header.Version==5)
	{
		unsigned char byte1;
		f.Seek(t_info->InOffset + t_info->InSize-769,CFile::begin);
		f.Read(&byte1,1);
		if(byte1==0x0C)
		{
			for(int j=0;j<256;j++)
			{
				f.Read(&t_info->Palette[0][j],1); //Red
				f.Read(&t_info->Palette[1][j],1); //Green
				f.Read(&t_info->Palette[2][j],1); //Blue
				t_info->PaletteLong[j] = t_info->Palette[0][j]+(t_info->Palette[1][j]<<8)+(t_info->Palette[2][j]<<16)+(255<<24);
			}
		}
	}
	f.Seek(t_info->InOffset+128,CFile::begin);

	if(xreal*yreal>1024*2048)
	{
		bitmap_data->valid=0;
		t_info->Valid=FALSE;
		t_info->Error="PCX Texture too big (>2097152 pixels)";
		return;
	}

	#ifdef _WITHTXVIEW
	int x_clusterize2=xreal;
	if(x_clusterize2%2!=0)
		x_clusterize2+=2-(xreal%2);
	int x_clusterize4=xreal;
	if(x_clusterize4%4!=0)
		x_clusterize4+=4-(xreal%4);

	t_info->Flags=TEXTUREINFOFLAG_TEXTURELOADED;

	unsigned char *tmpbuffer;
	try {
		t_info->Bitmap=new unsigned char[x_clusterize4*yreal];
		tmpbuffer=new unsigned char[x_clusterize2*yreal];
	}
	catch (CMemoryException e) {
		t_info->Valid=FALSE;
		t_info->Error="Out of Memory";
		bitmap_data->valid=0;
		return;
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
				m_FS_RGBAtexture2[offset++]=t_info->PaletteLong[runvalue];
			
	#ifdef _WITHTXVIEW
			if(runoffset<x_clusterize4*yreal) //Just to be sure
				tmpbuffer[runoffset]=runvalue;
	#endif
			runoffset++;
		}	
	}
	#ifdef _WITHTXVIEW
	for(unsigned int j=0;j<yreal;j++)
		memcpy(&t_info->Bitmap[j*x_clusterize4],&tmpbuffer[j*x_clusterize2],xreal);
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
	t_info->Error="<ok>";
}

void CMODVIEW32Doc::loadANItexture(TEXTUREINFO* t_info, FS_BITMAPDATA* bitmap_data, BOOL bFastload) 
{
	GLint MaxTextureSize[4];

	glGetIntegerv(GL_MAX_TEXTURE_SIZE,MaxTextureSize);
	MaxSize=MaxTextureSize[0];

	CFile f;

	//Header
	ANIHEADER header;
	f.Read(&header,sizeof(ANIHEADER));
	if(header.version!=2)
	{
		bitmap_data->valid=0;
		t_info->Valid=FALSE;
		t_info->Error="Invalid ANI version";
		return;
	}
	if(header.id!=0)
	{
		bitmap_data->valid=0;
		t_info->Valid=FALSE;
		t_info->Error="Invalid ANI file id";
		return;
	}

	xreal=header.w;
	yreal=header.h;
	t_info->Width=xreal;
	t_info->Height=yreal;

	unsigned int jb;
	jb=2; while(jb<xreal) jb*=2; xsize=jb;
	jb=2; while(jb<yreal) jb*=2; ysize=jb;

	//Palette
	for(int j=0;j<256;j++)
	{
		t_info->Palette[0][j]=header.palette[j*3+0]; //Red
		t_info->Palette[1][j]=header.palette[j*3+1]; //Green
		t_info->Palette[2][j]=header.palette[j*3+2]; //Blue
		t_info->PaletteLong[j]=m_Textures[m_TexturesNum].Palette[0][j]+(m_Textures[m_TexturesNum].Palette[1][j]<<8)+(m_Textures[m_TexturesNum].Palette[2][j]<<16)+(255<<24);
	}

	if(xreal*yreal>1024*2048)
	{
		bitmap_data->valid=0;
		t_info->Valid=FALSE;
		t_info->Error="PCX Texture too big (>2097152 pixels).";
		return;
	}

	#ifdef _WITHTXVIEW
	int size=xreal*yreal;
	t_info->Flags=TEXTUREINFOFLAG_TEXTURELOADED;
	t_info->Bitmap=new unsigned char[size];
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
				m_FS_RGBAtexture2[offset]=t_info->PaletteLong[runvalue];
			}
	#ifdef _WITHTXVIEW
			t_info->Bitmap[runoffset]=runvalue;
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
	t_info->Error="<ok>";
}

ERRORCODE CMODVIEW32Doc::FS_LoadTextureData(unsigned int ActivePM,BOOL bAniLoad,BOOL bFastload)
{
	GLint MaxTextureSize[4];

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

			locatetexture(m_FS_BitmapData.pic[i].name, i, m_TexturesNum);

			//// check first for a pcx file
			//char Filename[50];
			///*m_FS_BitmapData.pic[i].name;*/
			//strcpy_s(Filename,m_FS_BitmapData.pic[i].name);
			//strcat_s(Filename,".pcx");
			//TRACE("Trying to load %s",m_FS_BitmapData.pic[i].name);
			//
			//if(!m_InfoMode)
			//{
			//	char str[256];
			//	sprintf_s(str,"Loading %s...",Filename);
			//	SetStatusBarText(str);
			//}

			////Get local directory
			//CString localdirectory="";
			//{
			//	char x[FNAMELEN];
			//	strcpy_s(x,m_CurFile);
			//	removefilename((char *)&x);
			//	localdirectory=x;
			//}

			////PCX Search process #1: Current directory
			//{
			//	CString localfilename=localdirectory;
			//	localfilename+=Filename;
			//	CFile fcheck;
			//	if(fcheck.Open(localfilename,CFile::modeRead))
			//	{
			//		unknownfiletype=FALSE;
			//		TRACE(".pcx...Found in current directory!\n");
			//		m_Textures[m_TexturesNum].Location="Current directory";
			//		m_Textures[m_TexturesNum].Type="PCX Texture";
			//		m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_PCX;
			//		m_Textures[m_TexturesNum].InFilename=localfilename;
			//		m_Textures[m_TexturesNum].InOffset=0;
			//		m_Textures[m_TexturesNum].InSize=fcheck.GetLength();
			//		m_Textures[m_TexturesNum].Valid=TRUE;
			//	}
			//}

			////PCX Search process #2: Current .VP file
			//if(unknownfiletype && m_FS_CurrVP_Loaded)
			//{
			//	unsigned int j=0;
			//	while((j<m_FS_CurrVP_Header.dirnumber)&(_strcmpi(Filename,m_FS_CurrVP_Dir[j].filename) !=0))
			//		j++;

			//	if(j<m_FS_CurrVP_Header.dirnumber)
			//	{
			//		CFile fcheck;
			//		if(!fcheck.Open(m_FS_CurrVP_Filename,CFile::modeRead))
			//		{
			//			m_ErrorString=ERROR_FS_CANTOPENCURVP_TEXT;
			//			return ERROR_FS_CANTOPENCURVP;
			//		}
			//		//File_end=m_FS_CurrVP_Dir[j].offset + m_FS_CurrVP_Dir[j].size;
			//		//File_start=m_FS_CurrVP_Dir[j].offset;
			//		unknownfiletype=FALSE;
			//		TRACE(".pcx...Found in current VP!\n");
			//		m_Textures[m_TexturesNum].Location="Current VP";
			//		m_Textures[m_TexturesNum].Type="PCX Texture";
			//		m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_PCX;
			//		m_Textures[m_TexturesNum].InFilename=m_FS_CurrVP_Filename;
			//		m_Textures[m_TexturesNum].InOffset=m_FS_CurrVP_Dir[j].offset;
			//		m_Textures[m_TexturesNum].InSize=m_FS_CurrVP_Dir[j].size;
			//		m_Textures[m_TexturesNum].Valid=TRUE;
			//	}
			//}

			////PCX Search process #3: /data/maps
			//if(unknownfiletype)
			//{
			//	CString mapsdirectory;
			//	COptionsDlg dlg;
			//	if(m_FS_CurrVP_FreeSpaceVersion==1)
			//		mapsdirectory=dlg.GetF1Path();
			//	else
			//		mapsdirectory=dlg.GetF2Path();
			//	CString mapsfilename=mapsdirectory;
			//	if(mapsfilename.Right(1)!="\\")
			//		mapsfilename+="\\";
			//	mapsfilename+="data\\maps\\";
			//	mapsfilename+=Filename;
			//	
			//	CFile fcheck;
			//	if(fcheck.Open(mapsfilename,CFile::modeRead))
			//	{
			//		unknownfiletype=FALSE;
			//		TRACE(".pcx...Found in FreeSpace /data/maps directory!\n");
			//		m_Textures[m_TexturesNum].Location="/data/maps directory";
			//		m_Textures[m_TexturesNum].Type="PCX Texture";
			//		m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_PCX;
			//		m_Textures[m_TexturesNum].InFilename=mapsfilename;
			//		m_Textures[m_TexturesNum].InOffset=0;
			//		m_Textures[m_TexturesNum].InSize=fcheck.GetLength();
			//		m_Textures[m_TexturesNum].Valid=TRUE;
			//	}
			//}

			////PCX Search process #4: Main .VP file
			//if(unknownfiletype && m_FS_MainVP_Loaded)
			//{
			//	unsigned int j=0;
			//	while((j<m_FS_MainVP_Header.dirnumber)&(_strcmpi(Filename,m_FS_MainVP_Dir[j].filename) !=0))
			//		j++;

			//	if(j<m_FS_MainVP_Header.dirnumber)
			//	{
			//		CFile fcheck;
			//		if(!fcheck.Open(m_FS_MainVP_Filename,CFile::modeRead))
			//			return ERROR_FS_CANTOPENFSVP;
			//		unknownfiletype=FALSE;
			//		TRACE(".pcx...Found in main VP!\n");
			//		m_Textures[m_TexturesNum].Location="Main FreeSpace VP";
			//		m_Textures[m_TexturesNum].Type="PCX Texture";
			//		m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_PCX;
			//		m_Textures[m_TexturesNum].InFilename=m_FS_MainVP_Filename;
			//		m_Textures[m_TexturesNum].InOffset=m_FS_MainVP_Dir[j].offset;
			//		m_Textures[m_TexturesNum].InSize=m_FS_MainVP_Dir[j].size;
			//		m_Textures[m_TexturesNum].Valid=TRUE;
			//	} 
			//}

			////ANI Search process #1: Current directory
			//if(unknownfiletype)
			//{		   // no pcx file... check for ani file
			//	strcpy_s(Filename,m_FS_BitmapData.pic[i].name);
			//	strcat_s(Filename,".ani");

			//	CFile fcheck;
			//	if(fcheck.Open(Filename,CFile::modeRead))
			//	{
			//		unknownfiletype=FALSE;
			//		TRACE(".ani...Found in current directory!\n");
			//		m_Textures[m_TexturesNum].Location="Current Directory";
			//		m_Textures[m_TexturesNum].Type="ANI Animation";
			//		m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_ANI;
			//		m_Textures[m_TexturesNum].InFilename=Filename;
			//		m_Textures[m_TexturesNum].InOffset=0;
			//		m_Textures[m_TexturesNum].InSize=fcheck.GetLength();
			//		m_Textures[m_TexturesNum].Valid=TRUE;
			//	} 
			//}
			//
			////ANI Search process #2: Current .VP
			//if(unknownfiletype && m_FS_CurrVP_Loaded)
			//{
			//	unsigned int j=0;
			//	while((j<m_FS_CurrVP_Header.dirnumber)&(_strcmpi(Filename,m_FS_CurrVP_Dir[j].filename) !=0))
			//		j++;
			//	if(j<m_FS_CurrVP_Header.dirnumber)
			//	{
			//		CFile fcheck;
			//		if(!fcheck.Open(m_FS_CurrVP_Filename,CFile::modeRead))
			//			return ERROR_FS_CANTOPENCURVP;
			//		unknownfiletype=FALSE;
			//		TRACE(".ani...Found in current VP!\n");
			//		m_Textures[m_TexturesNum].Location="Current VP";
			//		m_Textures[m_TexturesNum].Type="ANI Animation";
			//		m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_ANI;
			//		m_Textures[m_TexturesNum].InFilename=m_FS_CurrVP_Filename;
			//		m_Textures[m_TexturesNum].InOffset=m_FS_CurrVP_Dir[j].offset;
			//		m_Textures[m_TexturesNum].InSize=m_FS_CurrVP_Dir[j].size;
			//		m_Textures[m_TexturesNum].Valid=TRUE;
			//	}
			//}

			////ANI Search process #3: /data/maps
			//if(unknownfiletype)
			//{
			//	CString mapsdirectory;
			//	COptionsDlg dlg;
			//	if(m_FS_CurrVP_FreeSpaceVersion==1)
			//		mapsdirectory=dlg.GetF1Path();
			//	else
			//		mapsdirectory=dlg.GetF2Path();
			//	CString mapsfilename=mapsdirectory;
			//	if(mapsfilename.Right(1)!="\\")
			//		mapsfilename+="\\";
			//	mapsfilename+="data\\maps\\";
			//	mapsfilename+=Filename;

			//	CFile fcheck;
			//	if(fcheck.Open(mapsfilename,CFile::modeRead))
			//	{
			//		unknownfiletype=FALSE;
			//		TRACE(".ani...Found in FreeSpace /data/maps directory!\n");
			//		m_Textures[m_TexturesNum].Location="/data/maps directory";
			//		m_Textures[m_TexturesNum].Type="ANI Animation";
			//		m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_ANI;
			//		m_Textures[m_TexturesNum].InFilename=mapsfilename;
			//		m_Textures[m_TexturesNum].InOffset=0;
			//		m_Textures[m_TexturesNum].InSize=fcheck.GetLength();
			//		m_Textures[m_TexturesNum].Valid=TRUE;
			//	}
			//}

			////ANI Search process #4: Main .VP file
			//if(unknownfiletype && m_FS_MainVP_Loaded)
			//{
			//	unsigned int j=0;
			//	while((j<m_FS_MainVP_Header.dirnumber)&(_strcmpi(Filename,m_FS_MainVP_Dir[j].filename) !=0))
			//		j++;
			//	if(j<m_FS_MainVP_Header.dirnumber)
			//	{
			//		CFile fcheck;
			//		if(!fcheck.Open(m_FS_MainVP_Filename,CFile::modeRead))
			//			return ERROR_FS_CANTOPENFSVP;
			//		unknownfiletype=FALSE;
			//		TRACE(".ani...Found in main VP!\n");
			//		m_Textures[m_TexturesNum].Location="Main FreeSpace VP";
			//		m_Textures[m_TexturesNum].Type="ANI Animation";
			//		m_Textures[m_TexturesNum].FileType=TEXTUREINFOFILETYPE_ANI;
			//		m_Textures[m_TexturesNum].InFilename=m_FS_MainVP_Filename;
			//		m_Textures[m_TexturesNum].InOffset=m_FS_MainVP_Dir[j].offset;
			//		m_Textures[m_TexturesNum].InSize=m_FS_MainVP_Dir[j].size;
			//		m_Textures[m_TexturesNum].Valid=TRUE;
			//	}
			//}

			////ANI Search process #: Skip if turned off
			//if((!bAniLoad) && (m_Textures[m_TexturesNum].FileType==TEXTUREINFOFILETYPE_ANI))
			//{
			//	m_FS_BitmapData.pic[i].valid=0;    
			//	m_Textures[m_TexturesNum].Valid=FALSE;
			//	TRACE("Above ANI SKIPPED due to user setting!\n");
			//	/*TOREMOVE*/
			//}

			////Search process FAILED!
			//if(unknownfiletype)
			//{
			//	m_FS_BitmapData.pic[i].valid=0;	// not ani or pcx...
			//	TRACE(".pcx/.ani...NOT FOUND!\n");
			//	m_Textures[m_TexturesNum].Location="<Not found>";
			//	m_Textures[m_TexturesNum].Type="<Unknown>";
			//	m_Textures[m_TexturesNum].Valid=FALSE;
			//	m_Textures[m_TexturesNum].Error="Not found";
			//}

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
					loadPCXtexture(&m_Textures[m_TexturesNum], &m_FS_BitmapData.pic[i], bFastload);
					break; 
				}
				case TEXTUREINFOFILETYPE_ANI:
				{
					loadANItexture(&m_Textures[m_TexturesNum], &m_FS_BitmapData.pic[i], bFastload);
					break; 
				}
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
		FS_LoadTextureData2();

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

void CMODVIEW32Doc::FS_LoadTextureData2()
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






