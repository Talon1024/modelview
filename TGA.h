// Copyright(c) 2oo1 by Mental Vortex
#ifndef _TGA_H_
#define _TGA_H_
// 
// Description:
// 
// Truvision Targa Format
// 
// 
// 
// 



//
// Includes
//



//
// Structures
//
#pragma pack(1)
typedef struct _TGAHeader
{
	byte	IDLength;
	byte    ColorMapType;
	byte	ImageType;

	// Color Map Specification
	short	ColorMapFirstEntryIndex;
	short	ColorMapLength;
	byte	ColorMapEntrySize;

	// ImageSpecification
	short		XOrigin;
	short		YOrigin;
	short		XSize;
	short		YSize;
	byte	PixelDepth;
	byte	ImageDescriptor;		// 0-3 Alpha Channel Bits
									// 5+4 Origin Alignment
									// 6+7 Must be zero
} TGAHeader_t;

typedef struct 
{
	byte r,g,b;
} RGB_t;

typedef struct
{
	unsigned char r,g,b,a;
} RGBA32_t;		
#pragma pack()

#define TGA_IMAGETYPE_NOIMAGE						0		// NOT SUPPORTED
#define TGA_IMAGETYPE_UNCOMPRESSED_COLORMAP			1		// NOT SUPPORTED
#define TGA_IMAGETYPE_UNCOMPRESSED_TRUECOLOR		2
#define TGA_IMAGETYPE_UNCOMPRESSED_BLACKANDWHITE	3		// NOT SUPPORTED
#define TGA_IMAGETYPE_RLE_COLORMAP					9		// NOT SUPPORTED
#define TGA_IMAGETYPE_RLE_TRUECOLOR					10		// NOT SUPPORTED
#define TGA_IMAGETYPE_RLE_BLACKANDWHITE				11		// NOT SUPPORTED



//
// Functions
//

//
// Writes a TGA directly from L3D skinbuffer
//
void		TGA_LoadRGBA			( char const* inName,RGBA32_t*& outData, int& outWidth, int& outHeight );

#endif //_TGA_H_