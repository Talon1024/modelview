//
//
//  d3-extra.h  ....
//
//

#define MAX_D3_TEXTURE	35
#define MAX_D3_SOBJ	60
#define MAX_D3_ANI		15000
#define MAX_D3_PNT		10
#define MAX_D3_PINF	1000
#define MAX_D3_VECT	4000
#define MAX_D3_POLY	4000
#define MAX_D3_FILESINHOG 4500
#define MAX_D3_OOFFILESINHOG2 1000

//#define D3_CROSSLIST_TOTAL		73
//#define D3_ORFLIST_TOTAL		2085

#define ID_HOG2 0x32474F48L   //hog2 file id
#define ID3_HOG2 0x474F48L   //hog file id
#define ID_PSPO 0x4f505350L   //pof file id

#define ID_TXTR 0x52545854L	//Texture filename list
#define ID_OHDR 0x5244484fL	//Object header
#define ID_SOBJ 0x4a424f53L	//Subobject header
#define ID_SPCL 0x4C435053L	//Texture filename list    
#define ID_GPNT 0x544E5047L	//Texture filename list
#define ID_GRND 0x444E5247L	//Texture filename list
#define ID_ATCH 0x48435441L	//Texture filename list
#define ID_NATH 0x4854414EL	//Texture filename list
#define ID_WBAT 0x54414257L	//Texture filename list
#define ID_PANI 0x494E4150L	//Texture filename list
#define ID_RANI 0x494E4152L	//Texture filename list
#define ID_PINF 0x464E4950L   //Texture filename list


typedef struct D3_vpnt
{
	float  x,y,z;
} D3_VPNT;

typedef struct D3_posangle
{
	BOOL ratav;
	float angle;
	D3_VPNT pnt;
	BOOL patav;
	D3_VPNT pat;
} D3_POSANGLE;

typedef struct pinfo_d3
{
	unsigned long Corners;
	unsigned long Color;
	unsigned char Red,Green,Blue;
	unsigned long Type;
	unsigned long Sobj;
	D3_VPNT Normal;
	unsigned long Vp[25];
	float U[25];
	float V[25];
} D3_PINFO;

typedef struct D3_MODEL
{		// data loaded from pof file.

	D3_VPNT _minbox;
	D3_VPNT _maxbox;

	unsigned long Vcount;
	D3_VPNT Vpoint[MAX_D3_VECT];
	D3_VPNT Vnormal[MAX_D3_VECT];
	float Valpha[MAX_D3_VECT];
	unsigned short VSobj[MAX_D3_VECT];
	unsigned long Pcount;
	D3_PINFO Poly[MAX_D3_POLY];

	unsigned long Scount;
	long Sparent[MAX_D3_SOBJ];
	D3_VPNT Soffset[MAX_D3_SOBJ];
	char Name[MAX_D3_SOBJ][50];
	char Prop[MAX_D3_SOBJ][50];

	unsigned long pindex[MAX_D3_SOBJ];
	unsigned long pknum[MAX_D3_SOBJ];
	unsigned long pkey[MAX_D3_ANI];
	D3_VPNT pani[MAX_D3_ANI];

	unsigned long rindex[MAX_D3_SOBJ];
	unsigned long rknum[MAX_D3_SOBJ];
	unsigned long rkey[MAX_D3_ANI];
	D3_VPNT rani[MAX_D3_ANI];
	unsigned long rangle[MAX_D3_ANI];
	
	unsigned long ATCHnum;
	unsigned long ATCHsobj[MAX_D3_PNT];
	D3_VPNT ATCHpos[MAX_D3_PNT];
	D3_VPNT ATCHnorm[MAX_D3_PNT];
	
	unsigned long NATHnum;
	unsigned long NATHsobj[MAX_D3_PNT];
	D3_VPNT NATHpos[MAX_D3_PNT];
	D3_VPNT NATHnorm[MAX_D3_PNT];
	
	unsigned long GRNDnum;
	unsigned long GRNDsobj[MAX_D3_PNT];
	D3_VPNT GRNDpos[MAX_D3_PNT];
	D3_VPNT GRNDnorm[MAX_D3_PNT];

	unsigned long GPNTnum;
	unsigned long GPNTsobj[MAX_D3_PNT];
	D3_VPNT GPNTpos[MAX_D3_PNT];
	D3_VPNT GPNTnorm[MAX_D3_PNT];
	
	unsigned long SPCLnum;
	char SPCLname[MAX_D3_PNT][50];
	char SPCLprop[MAX_D3_PNT][50];
	D3_VPNT SPCLpos[MAX_D3_PNT];
	float SPCLrad[MAX_D3_PNT];
	
	unsigned long WBATnum;
	unsigned long WBATnumgun[MAX_D3_PNT];
	unsigned long WBATgun[MAX_D3_PNT][MAX_D3_PNT];
	unsigned long WBATnumeye[MAX_D3_PNT];
	unsigned long WBATeye[MAX_D3_PNT][MAX_D3_PNT];
	
	char PINF[MAX_D3_PINF];
} D3_MODELtag;


typedef struct D3_DISPLAY
{				// data needed to display the pof file.
	float MaxXYZ;

	long SpolyNum[MAX_D3_SOBJ];
	long SvertNum[MAX_D3_SOBJ];
	BOOL Facing[MAX_D3_SOBJ];
	float Foffset[MAX_D3_SOBJ][4];
	BOOL PartsRotate;
	BOOL Rotate[MAX_D3_SOBJ];
	float Rspeed[MAX_D3_SOBJ];
	BOOL Glow[MAX_D3_SOBJ];
	float Gset[MAX_D3_SOBJ][4];
	BOOL Fov[MAX_D3_SOBJ];

	unsigned long TotalKey;
	BOOL pr_key[MAX_D3_ANI];
	unsigned long pr_active[MAX_D3_ANI];

	BOOL TextureFlag[MAX_D3_TEXTURE];
	BOOL ClearFlag[MAX_D3_TEXTURE];
	BOOL Used_texture[MAX_D3_TEXTURE];
	char LoadName[MAX_D3_TEXTURE][50];
} D3_DISPLAYtag;

typedef struct D3_DISPLAYPOLY
{
	unsigned char flag,red,green,blue;
	unsigned long startpnt, numpnt;
} D3_DISPLAYPOLYtag;

typedef struct D3_DISPLAYSOBJ
{
	unsigned long startpoly, numpoly;
} D3_DISPLAYSOBJtag;

#pragma pack(1)

typedef struct
{
	unsigned long signature;
	unsigned long dirnumber;
	unsigned long diroffset;
	unsigned char fill[56];
} HOG2_HEADER;

typedef struct
{
	char filename[36];
	unsigned long unknown;
	unsigned long size;
	unsigned long timestamp;
} HOG2_INFO;

typedef struct
{
	char filename[36];
	unsigned long size;
	unsigned long offset;
} HOG2_LOAD;

typedef struct
{
	unsigned char hdr[3];
} OGF_HEADER1;

typedef struct
{
	unsigned char miplevel;
	unsigned char filler[9];
	unsigned short width;
	unsigned short height;
	unsigned short unknown;
} OGF_HEADER2;

typedef struct
{
	unsigned char hdr[5];
	unsigned short id;
} OAF_HEADER1;

typedef struct
{
	unsigned char hdr[10];
} OAF_HEADER2;

typedef struct
{
	char FileName[36];
	char OgfName[36];
} D3_CROSSLIST;


typedef struct
{
	unsigned long number;
	char name[30];
} D3_DATALIST;

