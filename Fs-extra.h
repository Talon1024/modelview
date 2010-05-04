//
//
//  fs-extra.h  ....
//
//

#define MAX_FS_TEXTURE     60
#define MAX_FS_SOBJ        180
#define MAX_FS_THRUSTERS	16
#define MAX_FS_THRUSTERS_GLOWS	16
#define MAX_FS_SHIELDFACES	4800
#define MAX_FS_SHIELDVERTICES	2400
#define MAX_FS_SUBMODEL_POINTS	8000
#define MAX_FS_POLYGONS	32000
#define MAX_FS_VERTICES	5000
#define MAX_FS_NORMALS	16000
#define MAX_FS_CROSSSECTIONS 64
#define MAX_FS_LIGHTS 128
#define MAX_FS_BBOXS 4000
#define MAX_FS_FIRING_POINTS	64
#define MAX_FS_DOCKS	32
#define MAX_FS_SPLINE_PATHS	256
#define MAX_FS_DOCK_POINTS	256
#define MAX_FS_TURRETS_BANKS	64
#define MAX_FS_REALGUNS_SLOTS	64
#define MAX_FS_FILESINVP 4000
#define MAX_FS_POFFILESINVP 200
#define MAX_FS_SUBSYSTEMS 256

//typedef signed char    char;
//typedef signed short   short;
//typedef signed long    long;
//typedef unsigned short unsigned short;
//typedef int            int;

typedef struct vms_vector
{
	float x,y,z;
} VMS_VECTOR;

#define D_EOF         0  //eof
#define D_DEFPOINTS		1	 //defpoints
#define D_FLATPOLY		2	 //flat-shaded polygon
#define D_TMAPPOLY		3	 //texture-mapped polygon
#define D_SORTNORM		4	 //sort by normal
#define D_BOUNDBOX    5  //bounding box

#define ID_PSPO 0x4f505350L //pof file id
#define ID_OHDR 0x5244484fL	//Object header
#define ID_HDR2 0x32524448L	//Object header
#define ID_SOBJ 0x4a424f53L	//Subobject header        
#define ID_OBJ2 0x324a424fL	//Subobject header
#define ID_TXTR 0x52545854L	//Texture filename list

#define ID_PATH 0x48544150L	//Texture filename list
#define ID_SPCL 0x4C435053L	//Texture filename list
#define ID_SHLD 0x444C4853L	//Texture filename list
#define ID__EYE 0x45594520L	//Texture filename list
#define ID_GPNT 0x544E5047L	//Texture filename list
#define ID_MPNT 0x544E504DL	//Texture filename list
#define ID_TGUN 0x4E554754L	//Texture filename list
#define ID_TMIS 0x53494D54L	//Texture filename list
#define ID_DOCK 0x4B434F44L	//Texture filename list
#define ID_FUEL 0x4C455546L	//Texture filename list
#define ID_PINF 0x464E4950L

#define ID_EYE_ 0x20455945L
#define ID_INSG 0x47534E49L
#define ID_ACEN 0x4E454341L

#pragma pack(1)

#define FS_VPNT D3_VPNT	//FreeSpace 1/2 points = Descent 3 points
/*typedef struct FS_vpnt
{
	float  x,y,z;
} FS_VPNT;*/

typedef struct bbinfo
{
	FS_VPNT  min,max;
} BBINFO;

typedef struct FS_PINFO
{
	unsigned long Corners;
	unsigned long Colors;
	unsigned long Sobj;
	unsigned long Ptype;
	unsigned long detail;
	FS_VPNT Center;
	FS_VPNT Normal;
	short Vp[20];
	short Np[20];
	float U[20];
	float V[20];
} FS_PINFOtag;

typedef struct FS_shieldface
{
	FS_VPNT Normal;
	int Vface[3]; 
	int Nface[3];
} FS_SHIELDFACE;

typedef struct FS_shields
{
	unsigned long Vcount;
	float Vpoint[MAX_FS_SHIELDVERTICES][3];
	unsigned long Fcount;
	FS_SHIELDFACE Face[MAX_FS_SHIELDFACES];
} FS_SHIELDS;

typedef struct FS_thruster
{
	int num_glows;
	CString properties;
	FS_VPNT glow_pos[MAX_FS_THRUSTERS_GLOWS];
	FS_VPNT glow_norm[MAX_FS_THRUSTERS_GLOWS];
	float glow_radius[MAX_FS_THRUSTERS_GLOWS];
} FS_THRUSTER;

typedef struct FS_MODEL
{
	unsigned long Vcount;
	FS_VPNT Vpoint[MAX_FS_VERTICES];
	short VSobj[MAX_FS_VERTICES];
	unsigned long Ncount;
	FS_VPNT Npoint[MAX_FS_NORMALS];
	unsigned long BBcount;
	BBINFO Bbox[MAX_FS_BBOXS];
	unsigned long Pcount;
	FS_PINFO Poly[MAX_FS_POLYGONS];

	int num_thrusters;
	FS_THRUSTER thrusters[MAX_FS_THRUSTERS];
	
	FS_SHIELDS shields;

	float MaxXYZ;
	FS_VPNT _minbox;
	FS_VPNT _maxbox;
	float radius;
} FS_MODELtag;

typedef struct FS_SOBJ
{
	FS_VPNT geometric_center;
	FS_VPNT real_offset;
	FS_VPNT offset;
	FS_VPNT bounding_box_min_point;
	FS_VPNT bounding_box_max_point;
	CString submodel_name;
	CString properties;
	int movement_type;
	int movement_axis;
	int submodel_parent;
	int detail;

	int reserved;
	int bsp_data_size;
	unsigned char *bsp_data;
	float radius;
	int submodel_number;
} FS_SOBJtag;

typedef struct FS_SUBSYSTEM
{
	CString Desc;
	CString Name;
	CString Properties;
	FS_VPNT Center;
	float Radius;
} FS_SUBSYSTEMtag;

typedef struct FS_MODELINFO
{
	FS_VPNT ACenPoint;
	
	float mass;
	FS_VPNT mass_center;
	float moment_inertia[3][3];

	int num_cross_sections;
	float cross_section_depth[MAX_FS_CROSSSECTIONS];
	float cross_section_radius[MAX_FS_CROSSSECTIONS];

	int num_lights;
	FS_VPNT light_location[MAX_FS_LIGHTS];
	int light_type[MAX_FS_LIGHTS];

	int num_eye_points; //only first one is used by MODELVIEW
	int eye_point_sobj_number;
	FS_VPNT eye_point_sobj_offset;
	FS_VPNT eye_point_normal;

	int obj_flags;
} FS_MODELINFOtag;

typedef struct pminfo {
	unsigned long n_models;
	unsigned long n_detail,detail[6];
	unsigned long n_debris,debris[64];
} PMINFO;

//
//  The PCX header format.
//
typedef struct pcxheader
{								// Offset   Description
	char   Id;					//  00h     Manufacturer ID
	char   Version;				//  01h     Version
	char   Format;				//  02h     Encoding Scheme
	char   BitsPixelPlane;		//  03h     Bits/Pixel/Plane
	unsigned short  Xmin;		//  04h     X Start (upper left)
	unsigned short  Ymin;		//  06h     Y Start (top)
	unsigned short  Xmax;		//  08h     X End (lower right)
	unsigned short  Ymax;		//  0Ah     Y End (bottom)
	short  Hdpi;				//  0Ch     Horizontal Resolution
	short  Vdpi;				//  0Eh     Vertical Resolution
	char   EgaPalette[48];		//  10h     16-Color EGA Palette
	char   Reserved;			//  40h     Reserved
	char   NumberOfPlanes;		//  41h     Number of Color Planes
	short  BytesLinePlane;		//  42h     Bytes/Line/Plane
	short  PaletteInfo;			//  44h     Palette Interpretation
	short  HScreenSize;			//  46h     Horizontal Screen Size
	short  VScreenSize;			//  48h     Vertical Screen Size
	char   Filler[54];			//  4Ah     Reserved
} PCXHEADER;

typedef struct
{
	unsigned char valid;
	unsigned long xsize,ysize;
	unsigned long xreal,yreal;
	char name[30];
//  unsigned char bitmap[1024*1024];
//  unsigned long *bitmap;
//  unsigned char PcxVgaPal[256*3];
//  unsigned long PcxPal[256];
	short detail;
} FS_BITMAPDATA;

typedef struct
{
	unsigned long count;
	FS_BITMAPDATA pic[MAX_FS_TEXTURE];
} FS_MODELBITMAPS;


typedef struct
{
	unsigned long signature;
	unsigned long version;
	unsigned long diroffset;
	unsigned long dirnumber;
} VP_HEADER;

typedef struct
{
	unsigned long offset;
	unsigned long size;
	char filename[32];
	unsigned long timestamp;
} VP_INFO;

//
//  The ANI header format.
//
typedef struct aniheader
{
	unsigned short   id;
	unsigned short   version;
	unsigned short   fps;
	char    r,g,b;
	unsigned short  w,h;
	unsigned short nframes;
	unsigned char   packer_code;
	char palette[256*3];
	unsigned short  numkeys;
} ANIHEADER;

