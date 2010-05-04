// FSChunk.h: interface for the CFSChunk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FSCHUNK_H__EDDCB269_670D_4C95_AA95_546675B4F912__INCLUDED_)
#define AFX_FSCHUNK_H__EDDCB269_670D_4C95_AA95_546675B4F912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFSChunk  
{
public:
	void CreateChunk(int newid, int newsize, unsigned char *newdata);
	void CreateChunk(unsigned char *newdata,BOOL deletedata=TRUE);
	CFSChunk();
	virtual ~CFSChunk();

	int id;
	int chunk_size;
	unsigned char *chunk_data;

	void ClearChunk();
	BOOL ReadChunk(CFile *f,BOOL resetpos);
	BOOL WriteChunk(CFile *f);
};

#endif // !defined(AFX_FSCHUNK_H__EDDCB269_670D_4C95_AA95_546675B4F912__INCLUDED_)
