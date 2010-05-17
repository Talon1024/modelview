// MODVIEW32Doc.h : interface of the CMODVIEW32Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODVIEW32DOC_H__1ECD613F_A743_11D3_9455_00105A9D6FFE__INCLUDED_)
#define AFX_MODVIEW32DOC_H__1ECD613F_A743_11D3_9455_00105A9D6FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define _WITHEDITOR		// enable support for editor (added in Beta 04)
#define _WITHTXVIEW		// enable support for texture pane (added in Beta 05)
#define _WITHHTVIEW		// enable support for HTML task view (added in Beta 05)

#include "FS-extra.h"
#include "FSChunk.h"

#define GAME_NONE 0
#define GAME_FS 1	//FreeSpace 1/2


#define MAX_TEXTURES	128
#define MAX_GUNS 256
#define MAX_GUNSGROUPS	128
#define MAX_FILENAMELEN 8192


#define ERRORCODE int

#define ERROR_GEN_NOERROR 0
#define ERROR_GEN_NOERROR_TEXT "No error. (c:p"
#define ERROR_GEN_OUTOFMEMORY 1
#define ERROR_GEN_OUTOFMEMORY_TEXT "Out of memory."
#define ERROR_GEN_NOMODELSELECTED 2
#define ERROR_GEN_NOMODELSELECTED_TEXT "No model selected."
#define ERROR_GEN_UNSUPPORTEDFILETYPE 3
#define ERROR_GEN_UNSUPPORTEDFILETYPE_TEXT "Unsupported file extension."
#define ERROR_GEN_INVALIDHOGFILE 4
#define ERROR_GEN_INVALIDHOGFILE_TEXT "Invalid HOG file (file was neither recognized as Descent 1/2 HOG nor as Descent 3 HOG2 file)."
#define ERROR_GEN_TOOMANYGUNS 5
#define ERROR_GEN_TOOMANYGUNS_TEXT "Too many gun points."
//
//#define ERROR_D2_INVALIDTEXTUREID 2003
//#define ERROR_D2_INVALIDTEXTUREID_TEXT "Cotains a polygon with an invalid texture ID."
//#define ERROR_D2_TOOMANYSEGMENTS 2005
//#define ERROR_D2_TOOMANYSEGMENTS_TEXT "Too many segments."
//#define ERROR_D2_TOOHIGHVCOUNT 2008
//#define ERROR_D2_TOOHIGHVCOUNT_TEXT "Too many vertices defined."
//#define ERROR_D2_TOOMANYPOINTSINPOLY 2010
//#define ERROR_D2_TOOMANYPOINTSINPOLY_TEXT "Contains a polygon with too many points."
//#define ERROR_D2_TOOMANYPOLYGONS 2011
//#define ERROR_D2_TOOMANYPOLYGONS_TEXT "Too many polygons."
//#define ERROR_D2_BADPOLYMODELDATA 2012
//#define ERROR_D2_BADPOLYMODELDATA_TEXT "Bad polymodel data."
//#define ERROR_D2_HOGUNSUPPORTED 2013
//#define ERROR_D2_HOGUNSUPPORTED_TEXT "Descent 1/2 HOG files are not yet supported."
//#define ERROR_D2_HOARDHAMUNSUPPORTED 2014
//#define ERROR_D2_HOARDHAMUNSUPPORTED_TEXT "HOARD.HAM (from Descent 2 Vertigo Series) does not contain any model data and is therefore not supported."

//#define ERROR_D3_TOOMANYTEXTURES 3003
//#define ERROR_D3_TOOMANYTEXTURES_TEXT "Too many textures."
//#define ERROR_D3_INVALIDTEXTURENAME 3004
//#define ERROR_D3_INVALIDTEXTURENAME_TEXT "Invalid texture name (too many characters)."
//#define ERROR_D3_TOOHIGHVCOUNT 3008
//#define ERROR_D3_TOOHIGHVCOUNT_TEXT "Too many vertices defined."
//#define ERROR_D3_TOOMANYPOINTSINPOLY 3010
//#define ERROR_D3_TOOMANYPOINTSINPOLY_TEXT "Too many points in polygon (>20)."
//#define ERROR_D3_TOOMANYPOLYGONS 3011
//#define ERROR_D3_TOOMANYPOLYGONS_TEXT "Too many polygons."
//#define ERROR_D3_HOGTOOMANYFILES 3013
//#define ERROR_D3_HOGTOOMANYFILES_TEXT "Too many files in HOG."
//#define ERROR_D3_NOFILESINHOG 3014
//#define ERROR_D3_NOFILESINHOG_TEXT "No .OOF model files in HOG."
//#define ERROR_D3_TOOMANYANINUMBERKEYSTHRUSTERS 3016
//#define ERROR_D3_TOOMANYANINUMBERKEYSTHRUSTERS_TEXT "Too many keys in animation block."
//#define ERROR_D3_TOOMANYATCHPOINTS 3017
//#define ERROR_D3_TOOMANYATCHPOINTS_TEXT "Too many attachment points."
//#define ERROR_D3_TOOMANYNATHPOINTS 3018
//#define ERROR_D3_TOOMANYNATHPOINTS_TEXT "Too many attachment normals."
//#define ERROR_D3_TOOMANYGRNDPOINTS 3019
//#define ERROR_D3_TOOMANYGRNDPOINTS_TEXT "Too many ground points."
//#define ERROR_D3_TOOMANYGPNTPOINTS 3020
//#define ERROR_D3_TOOMANYGPNTPOINTS_TEXT "Too many gun points."
//#define ERROR_D3_TOOMANYSPCLPOINTS 3021
//#define ERROR_D3_TOOMANYSPCLPOINTS_TEXT "Too many special points."
//#define ERROR_D3_TOOMANYWBATPOINTS 3022
//#define ERROR_D3_TOOMANYWBATPOINTS_TEXT "Too many weapon batteries."
//#define ERROR_D3_TOOMANYWBATNUMGUNPOINTS 3023
//#define ERROR_D3_TOOMANYWBATNUMGUNPOINTS_TEXT "Too many guns in weapon battery."
//#define ERROR_D3_TOOMANYWBATNUMEYEPOINTS 3024
//#define ERROR_D3_TOOMANYWBATNUMEYEPOINTS_TEXT "Too many eye points in weapon battery."
//#define ERROR_D3_PINFTEXTTOOLONG 3025
//#define ERROR_D3_PINFTEXTTOOLONG_TEXT "PINF block contains too long text."
//#define ERROR_D3_INVALIDCHUNK 3026
//#define ERROR_D3_INVALIDCHUNK_TEXT "Invalid chunk."
//#define ERROR_D3_INVALIDCHUNKLENGTH 3027 //(Unused)
//#define ERROR_D3_INVALIDCHUNKLENGTH_TEXT "Invalid chunk length."
//#define ERROR_D3_INVALIDOOFHEADER 3028
//#define ERROR_D3_INVALIDOOFHEADER_TEXT "Invalid file header."

#define ERROR_FS_CANTOPENCURVP 6000
#define ERROR_FS_CANTOPENCURVP_TEXT "Can't open VP file."
#define ERROR_FS_CANTOPENFSVP 6001
#define ERROR_FS_CANTOPENFSVP_TEXT "Can't open FreeSpace VP file."
#define ERROR_FS_TEXTURETOOBIG 6002
#define ERROR_FS_TEXTURETOOBIG_TEXT "Texture resolution exceeds limits (1024*2048)."
#define ERROR_FS_TOOMANYTEXTURES 6003
#define ERROR_FS_TOOMANYTEXTURES_TEXT "Too many textures."
#define ERROR_FS_INVALIDTEXTURENAME 6004
#define ERROR_FS_INVALIDTEXTURENAME_TEXT "Invalid texture name (exceeds 30 characters)."
#define ERROR_FS_SHIELDTOOMANYVERTICIES 6005
#define ERROR_FS_SHIELDTOOMANYVERTICIES_TEXT "Shield data consists of too many verticies."
#define ERROR_FS_SHIELDTOOMANYFACES 6006
#define ERROR_FS_SHIELDTOOMANYFACES_TEXT "Shield data consists of too many faces."
#define ERROR_FS_TOOMANYSOBJS 6007
#define ERROR_FS_TOOMANYSOBJS_TEXT "Too many SOBJ chunks."
#define ERROR_FS_TOOHIGHVCOUNT 6008
#define ERROR_FS_TOOHIGHVCOUNT_TEXT "Too many vertices defined."
#define ERROR_FS_TOOHIGHNCOUNT 6009
#define ERROR_FS_TOOHIGHNCOUNT_TEXT "Too many normals defined."
#define ERROR_FS_TOOMANYPOINTSINPOLY 6010
#define ERROR_FS_TOOMANYPOINTSINPOLY_TEXT "Contains a polygon with too many points."
#define ERROR_FS_TOOMANYPOLYGONS 6011
#define ERROR_FS_TOOMANYPOLYGONS_TEXT "Too many polygons."
#define ERROR_FS_BADPOLYMODELDATA 6012
#define ERROR_FS_BADPOLYMODELDATA_TEXT "Bad polymodel data."
#define ERROR_FS_VPTOOMANYFILES 6013
#define ERROR_FS_VPTOOMANYFILES_TEXT "Too many files in VP."
#define ERROR_FS_NOFILESINVP 6014
#define ERROR_FS_NOFILESINVP_TEXT "No .POF model files in VP."
#define ERROR_FS_VPTOOMANYPOFS 6015
#define ERROR_FS_VPTOOMANYPOFS_TEXT "Too many POF files in VP."
#define ERROR_FS_TOOMANYTHRUSTERS 6016
#define ERROR_FS_TOOMANYTHRUSTERS_TEXT "Too many thrusters."
#define ERROR_FS_TOOMANYTHRUSTERGLOWS 6017
#define ERROR_FS_TOOMANYTHRUSTERGLOWS_TEXT "Contains a thruster with too many glowpoints."
#define ERROR_FS_TOOMANYDOCKINGPOINTS 6018
#define ERROR_FS_TOOMANYDOCKINGPOINTS_TEXT "Too many docking points."
#define ERROR_FS_TOOMANYCROSSSECTIONS 6019
#define ERROR_FS_TOOMANYCROSSSECTIONS_TEXT "Too many cross sections."
#define ERROR_FS_TOOMANYLIGHTS 6020
#define ERROR_FS_TOOMANYLIGHTS_TEXT "Too many lights."
#define ERROR_FS_NOOHDRCHUNKS 6021
#define ERROR_FS_NOOHDRCHUNKS_TEXT "No OHDR/HDR2 block in POF file."
#define ERROR_FS_NOSOBJCHUNKS 6022
#define ERROR_FS_NOSOBJCHUNKS_TEXT "No SOBJ/OBJ2 block in POF file."
#define ERROR_FS_CHUNKDATAMISSING	6023
#define ERROR_FS_CHUNKDATAMISSING_TEXT "Block ended unexpected."
#define ERROR_FS_CHUNKDATAOVERFLOW	6024
#define ERROR_FS_CHUNKDATAOVERFLOW_TEXT "Block exceeded end of file."
#define ERROR_FS_DOCKTOOMANYSPLINEPATHS 6025
#define ERROR_FS_DOCKTOOMANYSPLINEPATHS_TEXT "Contains a docking point with too many spline paths."
#define ERROR_FS_DOCKTOOMANYDOCKPOINTS 6026
#define ERROR_FS_DOCKTOOMANYDOCKPOINTS_TEXT "Contains a docking point with too many actual points defined."
#define ERROR_FS_TOOMANYBBOXS 6027
#define ERROR_FS_TOOMANYBBOXS_TEXT "Too many bounding boxes."
#define ERROR_FS_INVALIDPOFHEADER 6028
#define ERROR_FS_INVALIDPOFHEADER_TEXT "Invalid file header."
#define ERROR_FS_HUGESUBMODEL 6029
#define ERROR_FS_HUGESUBMODEL_TEXT "Too huge submodel, too many points defined."
#define ERROR_FS_TOOMANYTURRETBANKS 6030
#define ERROR_FS_TOOMANYTURRETBANKS_TEXT "Too many turret banks."
#define ERROR_FS_TOOMANYREALGUNSLOTS 6031
#define ERROR_FS_TOOMANYREALGUNSLOTS_TEXT "Too many gun slots."
#define ERROR_FS_TOOMANYFIRINGPOINTS 6032
#define ERROR_FS_TOOMANYFIRINGPOINTS_TEXT "Contains a turret bank with too many firing points."
#define ERROR_FS_INVALIDEYEPOINTSOBJ 6033
#define ERROR_FS_INVALIDEYEPOINTSOBJ_TEXT "Eye point is in invalid subobject."

#define ERROR_PCX_INVALIDVERSION 10000
#define ERROR_PCX_INVALIDVERSION_TEXT "PCX Version is not 5!"
#define ERROR_ANI_INVALIDVERSION 10100
#define ERROR_ANI_INVALIDVERSION_TEXT "ANI Version is not 2!"
#define ERROR_ANI_INVALIDID 10101
#define ERROR_ANI_INVALIDID_TEXT "ANI ID is not 0!"

#define ERROR_COB_NOTBINFORMAT 20000
#define ERROR_COB_NOTBINFORMAT_TEXT "Import failed: Input file is not in bin format."
#define ERROR_COB_WRONGGROUPING 20001
#define ERROR_COB_WRONGGROUPING_TEXT "Import failed: Group either all parts or none."
#define ERROR_COB_TOOMANYSECTIONS 20002
#define ERROR_COB_TOOMANYSECTIONS_TEXT "Import failed: Too many sections."
#define ERROR_COB_TOOMANYTEXTURES 20003
#define ERROR_COB_TOOMANYTEXTURES_TEXT "Import failed: Too many textures."
#define ERROR_COB_TOOMANYMATLS 20004
#define ERROR_COB_TOOMANYMATLS_TEXT "Import failed: Too many materials."
#define ERROR_COB_TOOMANYVERTICES 20005
#define ERROR_COB_TOOMANYVERTICES_TEXT "Import failed: Too many vertices."
#define ERROR_COB_TOOMANYUVS 20006
#define ERROR_COB_TOOMANYUVS_TEXT "Import failed: Too many UV coordinates."
#define ERROR_COB_TOOMANYPOLYGONPOINTS 20007
#define ERROR_COB_TOOMANYPOLYGONPOINTS_TEXT "Import failed: Too many polygon points."
#define ERROR_COB_TOOMANYPOLYGONS 20008
#define ERROR_COB_TOOMANYPOLYGONS_TEXT "Import failed: Too many polygons."
#define ERROR_COB_TOOMANYCHUNKS 20009
#define ERROR_COB_TOOMANYCHUNKS_TEXT "Import failed: Too many chunks."

#define BOXES_DISTANCE	2


#define TEXTUREINFOFLAG_TEXTURELOADED	1
#define TEXTUREINFOFLAG_TEXTUREIS24BIT	2 //Palette or RGBA-texture

#define TEXTUREINFOFILETYPE_UNKNOWN	0
//#define TEXTUREINFOFILETYPE_OGF		1 //Descent 3 texture
//#define TEXTUREINFOFILETYPE_OAF		2 //Descent 3 animation
#define TEXTUREINFOFILETYPE_PCX		1 //FreeSpace 1/2 texture
#define TEXTUREINFOFILETYPE_ANI		2 //FreeSpace 1/2 animation
//#define TEXTUREINFOFILETYPE_D2U		5 //Descent 2 texture (uncompressed)
//#define TEXTUREINFOFILETYPE_D2C		6 //Descent 2 texture (RLE-compressed)

typedef struct TEXTUREINFO
{
	CString Filename;
	CString Location;
	CString Type;
	int Width;
	int Height;
	int Used;
	CString InFilename;
	int InOffset;
	ULONGLONG InSize;
	BOOL Valid;
	CString Error;
	unsigned int PaletteLong[256]; //Unused if TEXTUREINFOFLAG_TEXTUREIS24BIT is set in Flags
	unsigned char Palette[3][256]; //Unused if TEXTUREINFOFLAG_TEXTUREIS24BIT is set in Flags
	int FileType;
#ifdef _WITHTXVIEW
	int Flags; 
	unsigned char *Bitmap;
#endif
} TEXTUREINFOtag;


class CMODVIEW32Doc : public CDocument
{
protected: // create from serialization only
	CMODVIEW32Doc();
	DECLARE_DYNCREATE(CMODVIEW32Doc)



#define GUNTYPE_PLAYER_GUN	1
#define GUNTYPE_PLAYER_MIS	2
#define GUNTYPE_TURRET_GUN	3
#define GUNTYPE_TURRET_MIS	4

typedef struct GUNS
{
	int Num;
	vms_vector Gun[MAX_GUNS];
	vms_vector GunDirection[MAX_GUNS];
	int InSubModel[MAX_GUNS];
	int GunType[MAX_GUNS];
	int GunBank[MAX_GUNS];
} guns;

typedef struct GUNSGROUP
{
	int Num;
	int GunType;
	int GunBank;
} gunsgroup;


typedef struct FS_TURRETS_BANK
{
	int sobj_parent;
	int sobj_par_phys;
	FS_VPNT turret_normal;
	int num_firing_points;
	FS_VPNT firing_point[MAX_FS_FIRING_POINTS];
} fs_turrets_bank;

typedef struct FS_TURRETS
{
	int Num;
	FS_TURRETS_BANK Bank[MAX_FS_TURRETS_BANKS];
} fs_turrets;

typedef struct FS_REALGUNS_SLOT
{
	int num_guns;
	FS_VPNT point[MAX_FS_FIRING_POINTS];
	FS_VPNT normal[MAX_FS_FIRING_POINTS];
} fs_realguns_slot;

typedef struct FS_REALGUNS
{
	int Num;
	FS_REALGUNS_SLOT Slot[MAX_FS_REALGUNS_SLOTS];
} fs_realguns;

typedef struct FS_DOCK
{
	CString properties;
	int num_spline_paths;
	int path_number[MAX_FS_SPLINE_PATHS];
	int num_points;
	vms_vector position[MAX_FS_DOCK_POINTS];
	vms_vector normal[MAX_FS_DOCK_POINTS];
} fs_dock;


#define MAX_BLOCKS 8192
typedef struct BLOCK
{
	CString name;
	int offset;
	int size;
	CString desc;
} BLOCKtag;


// Attributes
public:

	//Common attributes
	BOOL m_Textures_Available;
	GLuint  m_Model;
	BOOL m_ModelLoaded;
	BOOL bSize;
	BOOL bZoom;
	BOOL m_DoResetGeometry;
	float m_AutoZoom;
	int m_Game;
	BOOL m_NoTexturesFound;

	//Archive stats
	int m_ArchiveStats_NumTotal;
	int m_ArchiveStats_NumLores;
	int m_ArchiveStats_NumHires;
	int m_ArchiveStats_NumOther;

	//Texture data
	TEXTUREINFO m_Textures[MAX_TEXTURES];
	int m_TexturesNum;

	//Error data
	CString m_ErrorString;
	ERRORCODE m_ErrorCode;
	CString m_ErrorDetails;

	//Block data
	int m_Blocks_Num;
	BLOCK m_Blocks[MAX_BLOCKS];

	//Descent 2 attributes
	//int			m_D2_Robots_Num;
	//ULONGLONG	m_D2_MO_RobotInfo[MAX_D2_ROBOT_TYPES];
	//int			m_D2_Models_Num;
	//char		m_D2_CurrHAXM_Filename[MAX_FILENAMELEN];
	//D2_MODEL		m_D2_Model;
	//D2_MODELBITMAPS	m_D2_BitmapData;
	//GLuint		m_D2_ModelTexture[MAX_D2_TEXTURE];
	//UINT16		m_D2_ObjBitmapPtr[625];
	//D2_BITMAP_INDEX		m_D2_ObjBitmap[625];
	//D2_BITMAP_INDEX		m_D2_HxmObjBitmap[625];
	//CString		m_D2_PM_Desc[MAX_D2_POLYGON_MODELS];
	//int			m_D2_PolType[MAX_D2_POLYGON_MODELS];
	//int			m_D2_MyRobot[MAX_D2_POLYGON_MODELS];
	////int			m_D2_MyReact[MAX_D2_POLYGON_MODELS];
	//BOOL		m_D2_AmLores[MAX_D2_POLYGON_MODELS];
	//D2_JOINTPOS				m_D2_JointPos[MAX_D2_ROBOT_JOINTS];
	//D2_ROBOT_INFO			m_D2_Robots[MAX_D2_ROBOT_TYPES];
	//D2_REACTOR				m_D2_Reactors[MAX_D2_REACTORS];
	//D2_PLAYER_SHIP			m_D2_PlayerShip;
	//int						m_D2_ModelLoadnr_to_ID[MAX_D2_POLYGON_MODELS];
	//byte m_D2_Palettes[4608];
	//D2_WEAPON_INFO m_D2_Weapons[MAX_D2_WEAPON_TYPES];
	//BOOL m_D2_MainHAMLoaded;
	//int m_D2_PIGNumTextures;
	//int m_D2_Marker_ModelNum;
	//D2_POLYMODELINFO m_D2_PolygonModels[MAX_D2_POLYGON_MODELS];
	//int m_D2_TextureSet;

	////Descent 3 attributes
	//CString		m_D3_MainHOG_Filename;
	//HOG2_HEADER	m_D3_MainHOG_Header;
	//HOG2_LOAD	m_D3_MainHOG_Dir[MAX_D3_FILESINHOG];
	//BOOL		m_D3_MainHOG_Loaded;
	//int			m_D3_MainHOG_MainTextureSet;
	///*int			m_D3_MainHOG_OofNum;
	//int			m_D3_MainHOG_OofList[MAX_OOFFILESINHOG2];*/
	//CString		m_D3_CurrHOG_Filename;
	//HOG2_HEADER	m_D3_CurrHOG_Header;
	//HOG2_LOAD	m_D3_CurrHOG_Dir[MAX_D3_FILESINHOG];
	//int			m_D3_CurrHOG_OofList[MAX_D3_OOFFILESINHOG2];
	//int			m_D3_CurrHOG_OofNum;
	//D3_MODEL	m_D3_Model;
	//D3_DISPLAY	m_D3_Display;
	//unsigned int m_D3_CurSpcl;
	//unsigned int m_D3_CurXpnt;
	//unsigned int m_D3_CurPwin;
	//unsigned int m_D3_CurWbat;
	//unsigned int m_D3_CurSobj;
	//unsigned int m_D3_CurTexture;
	//BOOL m_D3_HiLiteSobj[MAX_D3_SOBJ];
	//BOOL m_D3_HiLiteCurSobj;
	//BOOL m_D3_OffTexture[MAX_D3_TEXTURE];
	//BOOL m_D3_OffTXTR;
	//float m_D3_Rstep[MAX_D3_SOBJ];
	//unsigned int m_D3_TotalTexture;
	//char m_D3_TextureList[MAX_D3_TEXTURE][36];
	//GLuint m_D3_ModelTexture[MAX_D3_TEXTURE];
	
	//FreeSpace attributes
	FS_MODELBITMAPS		m_FS_BitmapData;
	BOOL				m_FS_TextureFlag[MAX_FS_TEXTURE];	
	FS_MODEL			m_FS_Model;
	BOOL				m_FS_ModelHasDebris;
	int					m_FS_NumSubsystems;
	FS_SUBSYSTEM		m_FS_Subsystems[MAX_FS_SUBSYSTEMS];
	int					m_FS_NumSOBJ;
	FS_SOBJ				m_FS_SOBJ[MAX_FS_SOBJ];
	CString				m_FS_MainVP_Filename;
	VP_HEADER			m_FS_MainVP_Header;
	VP_INFO				m_FS_MainVP_Dir[MAX_FS_FILESINVP];
	BOOL				m_FS_MainVP_Loaded;
	int					m_FS_MainVP_MainTextureSet;
	CString				m_FS_CurrVP_Filename;
	VP_HEADER			m_FS_CurrVP_Header;
	VP_INFO				m_FS_CurrVP_Dir[MAX_FS_FILESINVP];
	BOOL				m_FS_CurrVP_Loaded;
	int					m_FS_CurrVP_FreeSpaceVersion;
	int					m_FS_CurrVP_PofList[MAX_FS_POFFILESINVP];
	int					m_FS_CurrVP_PofNum;
	GLuint				m_FS_ModelTexture[MAX_FS_TEXTURE];
	unsigned int		m_FS_PofDataL[MAX_FS_SOBJ],m_FS_PofDataH[MAX_FS_SOBJ];
	unsigned int		m_FS_RGBAtexture[1024*1024];   
	unsigned int		m_FS_RGBAtexture2[1024*2048];
	unsigned int		m_FS_LoadPCX[MAX_FS_TEXTURE];
	PMINFO				m_FS_PMinfo;   
	FS_REALGUNS			m_FS_RealGuns[2]; //0=GPNT, 1=MPNT
	FS_TURRETS			m_FS_Turrets[2]; //0=TGUN, 1=TMIS
	int					m_FS_NumDocks;
	FS_DOCK				m_FS_Docks[MAX_FS_DOCKS];
	FS_MODELINFO		m_FS_ModelInfo;

	//Red Faction
	//RF_MODEL	m_RF_Model;
	//VPP_HEADER	m_RF_CurrVPP_Header;
	//VPP_LOAD	*m_RF_CurrVPP_Load;
	//CString m_RF_CurrVPP_Filename;

	//EditorFS
	char test1[256];
	CString test3;
	FS_MODELINFO		m_EditorFS_Model;
	FS_REALGUNS_SLOT	m_EditorFS_RealGun;
	FS_SUBSYSTEM		m_EditorFS_Subsystem;
	FS_TURRETS_BANK		m_EditorFS_Turret;
	FS_SOBJ				m_EditorFS_SOBJ;
	FS_THRUSTER			m_EditorFS_FUEL;
	char test2[256];
	CString test4;
	CFSChunk m_EditorFS_ChunkTXTR;
	CFSChunk m_EditorFS_ChunkOHDR;
	CFSChunk m_EditorFS_ChunkSOBJ[MAX_FS_SOBJ];
	CFSChunk m_EditorFS_ChunkTGUN;
	CFSChunk m_EditorFS_ChunkTMIS;
	CFSChunk m_EditorFS_ChunkPATH;
	CFSChunk m_EditorFS_ChunkPINF;
	CFSChunk m_EditorFS_ChunkSHLD;
	CFSChunk m_EditorFS_Chunk_EYE;
	CFSChunk m_EditorFS_ChunkGPNT;
	CFSChunk m_EditorFS_ChunkMPNT;
	CFSChunk m_EditorFS_ChunkDOCK;
	CFSChunk m_EditorFS_ChunkFUEL;
	CFSChunk m_EditorFS_ChunkINSG;
	CFSChunk m_EditorFS_ChunkACEN;
	CFSChunk m_EditorFS_ChunkSPCL;
	FS_SHIELDS m_EditorFS_ShieldsBackup;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMODVIEW32Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_UntitledModel;
	ERRORCODE RF_LoadVPPContent(CString filename);
//	void RF_InitVPP(CFile *fp,VPP_HEADER *vpp_header,VPP_LOAD **vpp_load,BOOL fillexplorer);
	//ERRORCODE ReadRFVPP();
	//ERRORCODE RF_ReadV3C(CFile *fp);
	void OnFileSave2();
	BOOL SaveModified2();
	CString m_CurrentFile_FileName;
	ULONGLONG m_CurrentFile_Size;
	int m_CurrentFile_Offset;
	CString m_CurFile;
	CString m_ModelType_Desc;
	int m_FileVersion;
	int m_DisplayTexture;
	GUNS m_Guns;
	GUNSGROUP m_GunsGroup[MAX_GUNSGROUPS];
	int m_NumGunsGroups;
	int m_NumPositions;
	int m_NumFilesize;
	BOOL m_InfoMode;
	int m_NumPolygons;
	int m_NumVertices;
	int m_NumSubmodels;
	int m_NumTextures;
	BOOL m_ModelJustLoaded;

	//User interface control routines
	void ClearDocument(BOOL RemoveGameBar=TRUE);
	void ExplorerClear();
	void ExplorerAddRoot(char *text);
	void ExplorerAddChild(char *filename, int ModelNum=-1);
	void ExplorerAddChild(CString filename, int ModelNum=-1);
	void ExplorerGoTop();
	void SetStatusBarText(char *txt);

	//Descent 2 routines
	//void D2_ReadD2HAM();
	//int D2_InitHAM(CFile *fp, BOOL fillexplorer);
	//ERRORCODE D2_Walk(void *StartAt,int module);
	//ERRORCODE D2_LoadHAXMContent(int ModelNum);
	//ERRORCODE D2_ReadPOL(CFile *fp, int ModelNum);
	//int D2_ReadRLE(FILE *file,UINT32 i);
	//void D2_Index2RGBA(UINT32 i);
	//void D2_LoadPIGData(void);
	//CString D2_GenerateModelTypeName(int type);
	//ERRORCODE D2_ReadROB(CFile *fp, int RobotNum);
	//void D2_CalcDisplay();

	//Descent 3 routines
	//void D3_ReadD3HOG();
	//void D3_CalcDisplay();
	//ERRORCODE D3_InitHOG(CFile *fp,HOG2_HEADER *hog2_header,HOG2_LOAD *hog2_load,BOOL fillexplorer);
	//ERRORCODE D3_ReadOOF(CFile *fp,int OofNum);
	//void D3_LoadOGFData();
	//ERRORCODE D3_LoadHOGContent(CString filename);

	//FreeSpace routines
	void FS_CalcDisplay();
	ERRORCODE FS_InitVP(CFile *fp,VP_HEADER *vp_header,VP_INFO *vp_info,BOOL fillexplorer);
	void FS_ReadFSVP(int game);
	void FS_LoadPCXData2();
	ERRORCODE FS_LoadVPContent(CString filename);
	ERRORCODE FS_Walk(void *StartAt,int module,int StartByte);
	ERRORCODE FS_ReadPOF(CFile *fp,int VpNum);
	ERRORCODE FS_LoadPCXData(unsigned int ActivePM,BOOL bAniLoad,BOOL bFastload);
	void FS_PreparePOF();
	void FS_BuildGunGroups();
	CString FS_MakeSubsystemDesc(CString x1,CString x2);

	//EditorFS routines
	unsigned char * EditorFS_GenerateTextures();
	unsigned char * EditorFS_GenerateACEN();
	unsigned char * EditorFS_GenerateShields();
	unsigned char * EditorFS_GenerateModelHeader();
	unsigned char * EditorFS_GenerateSOBJ(int subm);
	unsigned char * EditorFS_GenerateSubsystems();
	unsigned char * EditorFS_GenerateThrusters();
	unsigned char * EditorFS_GenerateTurrets(int type);
	unsigned char * EditorFS_GenerateRealGuns(int type);
	BOOL EditorFS_SavePOF(CString filename, int fileversion);

	//Math routines
	float Vmag(float v[3]);
	FS_VPNT Vnorm(float v[3]);
	float Vdot(float v1[3],float v2[3]);
	FS_VPNT Vcross(float v1[3],float v2[3]);
	float Vangle(float v1[3],float v2[3]);

	virtual ~CMODVIEW32Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMODVIEW32Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODVIEW32DOC_H__1ECD613F_A743_11D3_9455_00105A9D6FFE__INCLUDED_)
