// Copyright(c) 2oo1 by Darkyoda and MENTAL VORTEX

#include "stdafx.h"

// 
// Description:
// 
// Black and White L3D File Loader
// 
//
// 
// 


//
// Comments :
//
// 10.April
// DY : Created this File
//
//
//




// Includes
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include			"tga.h"

void	 
ConvertUncompressedTGA(byte BitDepth,int width,int height,void *source,RGBA32_t *dest)
{
	int i,j;
	RGBA32_t	*d = dest;

	if(BitDepth == 32)
	{
		RGBA32_t	*PixelBuf = (RGBA32_t	*) source;
		
		for(i=0;i<width;i++)
		{
			d = &dest[(width-1-i)*height];
			for(j=0;j<height;j++)
			{
				d->r = PixelBuf->b;
				d->g = PixelBuf->g;
				d->b = PixelBuf->r;
				d->a = PixelBuf->a;

				d++;
				PixelBuf++;
			};
		};
	}
	else
	if(BitDepth == 24)
	{
		RGB_t	*PixelBuf = (RGB_t	*) source;
		for(i=0;i<width;i++)
		{
			d = &dest[(width-i-1)*height];
			for(j=0;j<height;j++)
			{
				d->r = PixelBuf->b;
				d->g = PixelBuf->g;
				d->b = PixelBuf->r;
				d->a = 255;

				d++;
				PixelBuf++;
			};
		};
	};
};

void		
TGA_LoadRGBA( char const* inName,RGBA32_t*& outData, int& outWidth, int& outHeight )
{
	TGAHeader_t		*Header;
	void			*Buffer;
	void			*ImageData;
	int				 FileSize;

	{
		FILE		*fp;

		fp = fopen(inName,"rb");

		if(!fp) 
		{
			printf("GetTargaImage failed : File (%s) not Found...\n",inName);
			return;
		}

		fseek(fp,0,SEEK_END);
		FileSize = ftell(fp);
		fseek(fp,0,SEEK_SET);

		if(!(Buffer = new char[FileSize]))
		{
			printf("GetTargaImage failed : Not enough Memory...\n");
			fclose(fp);
			return;
		};

		fread(Buffer,FileSize,1,fp);

		fclose(fp);
	}	
	{
		if(!_stricmp(((char *)Buffer+FileSize-16),"TRUEVISION-XFILE."))
		{
			if(Buffer)
				delete[]Buffer;
			return;
		};
		Header = (TGAHeader_t *) Buffer;

		if(Header->ColorMapType) 
		{
			printf("GetTargaImage failed : ColorMaps not supported\n");
			if(Buffer)
				delete[]Buffer;
			return;
		}

		ImageData = (void *)((char *)Buffer + sizeof(TGAHeader_t) + Header->ColorMapLength + Header->IDLength);

		outWidth = Header->XSize;
		outHeight = Header->YSize;
		
		if(Header->ImageType == TGA_IMAGETYPE_UNCOMPRESSED_TRUECOLOR) 
		{	
			outData = new RGBA32_t[Header->XSize*Header->YSize];
			ConvertUncompressedTGA(Header->PixelDepth,Header->XSize,Header->YSize,ImageData,outData);
		}
		else
		{
			printf("GetTargaImage failed : Currently just uncompressed True - Color Images Supported supported\n");
			if(Buffer)
				delete[]Buffer;
			return;
		}
	}

	if(Buffer)
		delete[]Buffer;

	return;
};