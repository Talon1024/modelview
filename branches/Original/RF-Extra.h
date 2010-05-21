#include "TGA.h"

#define ID_MCFR	0x5246434d
#define ID_MBUS 0x5355424d

typedef struct RF_VPNT
{
	float xyz[3];
} RF_VPNTtag;

typedef struct RF_UVS
{
	float uv[2];
} RF_UVStag;

typedef struct RF_TRIS
{
	int indicies[3];
	int surface;
} RF_TRIStag;

typedef struct RF_MODELDATA
{
	RF_VPNT	*vertices;
	RF_VPNT	*normals;
	RF_UVS	*uvs;
	RF_TRIS	*triangles;
} RF_MODELDATAtag;

typedef struct RF_GEOUNIT
{
	int unk[6];
	int mIndex;
	int unk1;
	short mNumVerts;
	short mNumTris;
	short size_3float;
	short size_tris;
	short size_2float;
	short size_dwordblock;
	short size_shortblock;
	short unk2;
} RF_GEOUNITtag;

typedef struct RF_TEXTURE
{
	CString filename;
	GLuint texturedata;
	RGBA32_t *buffer;
	int height;
	int width;
} RF_TEXTUREtag;

typedef struct RF_MODEL
{
	CString name;
	CString name2;
	int num_surfaces;
	short num_geounits;
	int num_textures;
	float radius;
	float mins[3];
	float maxs[3];
	RF_GEOUNIT *geounit;
	RF_MODELDATA *modeldata;
	GLuint  model;
#define			MAX_RF_TEXTURES 32
	RF_TEXTURE texture[MAX_RF_TEXTURES];
} RF_MODELtag;


//VPP
typedef struct VPP_HEADER
{
	int signature;
	int version;
	int num_files;
	int filesize;
} VPP_HEADERtag;

typedef struct VPP_LOAD
{
	CString filename;
	int offset;
	int size;
} VPP_LOADtag;


