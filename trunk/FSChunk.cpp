// FSChunk.cpp: implementation of the CFSChunk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "modview32.h"
#include "FSChunk.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFSChunk::CFSChunk()
{
	chunk_size=0;
}

CFSChunk::~CFSChunk()
{
	ClearChunk();
}

void CFSChunk::ClearChunk()
{
	if(chunk_size!=0)
	{
		delete(chunk_data);
		chunk_size=0;
	}
}

BOOL CFSChunk::ReadChunk(CFile *f,BOOL resetpos)
{
	ClearChunk();

	int pos = (int) f->GetPosition();
	if(resetpos)
		f->Seek(pos-8,SEEK_SET);	//Go back 8 steps first

	f->Read(&id,4);
	f->Read(&chunk_size,4);
	chunk_data=new unsigned char[chunk_size];
	f->Read(chunk_data,chunk_size);

	if(resetpos)
		f->Seek(pos,SEEK_SET);
	return TRUE; //TODO: check
}

BOOL CFSChunk::WriteChunk(CFile *f)
{
	if(chunk_size!=0)
	{
		f->Write(&id,4);
		f->Write(&chunk_size,4);
		f->Write(chunk_data,chunk_size);
	}
	return TRUE; //TODO: check
}

void CFSChunk::CreateChunk(int newid, int newsize, unsigned char *newdata)
{
	ClearChunk();
	id=newid;
	chunk_size=newsize;
	chunk_data=newdata;
}

void CFSChunk::CreateChunk(unsigned char *newdata,BOOL deletedata/*=TRUE*/)
{
	ClearChunk();
	if(newdata==NULL)
		return;
	memcpy(&id,newdata,4);
	memcpy(&chunk_size,newdata+4,4);
	chunk_data=new unsigned char[chunk_size];
	memcpy(chunk_data,newdata+8,chunk_size);
	if(deletedata)
		delete(newdata);
}
