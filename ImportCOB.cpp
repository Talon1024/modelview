//
//--------------------------------------------------------------------------
// cob2fs.c
// 8-15-99
//
//--------------------------------------------------------------------------
//

//This is pretty much the WinCOB2FS V1.0 version of COB2FS by XXX.
//However it had a lot of bugs where vars where not initialized (set to 0).
//Seems like the compiler he used didn't care and set everything to 0 by
//default. MS Visual C++ and any usual ANSI-compiler however do not do this
//and force the manual initialisation. The "FixWinCOB2FS" tag marks these
//fixes.

#include "stdafx.h"
#include "ImportCOB.h"
#include "MODVIEW32.h"
#include "MainFrm.h"
#include "FSChunk.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//#define LOGTEXT TRACE
#define LOGTEXT { CString l; l.Format
#define LOGDOIT logstring+=l; }

//
//-------------------------
// prototypes
//-------------------------
//
/*void MakePof(char *);
void read_cob(FILE *file);
void ReduceToUnit(float vector[3]);
void calcNormal(float v[3][3], float out[3]);
void cob2model(void);*/

CMODVIEW32Doc * CImportCOB::GetDocument()
{
	//Get the document
	CMODVIEW32App *m_App=(CMODVIEW32App *)AfxGetApp();
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_App->m_pMainWnd);
	return (CMODVIEW32Doc *)viewFrame->GetDocument();
}


CImportCOB::CImportCOB()
{
	//FixWinCOB2FS
	for(int i=0;i<POLY_MAX;i++)
		Cmodel.Poly[i].matlindex=0;
	memset(&Cmodel,0,sizeof(cmodel));
	memset(&FSModel,0,sizeof(fsmodel));

	//Prepare log file
	errstring="";
	errdetails="";
	logstring="";
	LOGTEXT("COB2FS converter based on %s by Garry Knudson\n",COB2FS_VERSION); LOGDOIT;

}

CImportCOB::~CImportCOB()
{
	//Add errstring to logstring
	if(!errstring.IsEmpty())
		logstring+="\nCancelled due to an error:\n"+errstring+"\n";
	else
		logstring+="\nCompleted successfully.\n";
	
	CString filename=((CMODVIEW32App *)AfxGetApp())->m_LogFile;

	CStdioFile f;
	if(f.Open(filename,CFile::modeWrite | CFile::modeCreate))
	{
		f.WriteString(logstring);
		f.Close();
	}
}




//
//--------------------------------------------------------------------------
// main()
//
//
//--------------------------------------------------------------------------
//
/*int __fastcall run(char *input, char *output) 
{
	FILE *file;

file=fopen(input,"rb");
// file=fopen("d:\\sfighter.cob","rb");
if(!file)
{
	LOGTEXT(" File not found\n"); LOGDOIT;
} else {
	LOGTEXT("  %s",COB2FS_VERSION);
	LOGTEXT("  Processing %s\n\n",input);
//Application->ProcessMessages();
	read_cob(file);
	fclose(file);
	cob2model();
	MakePof( output );
}
LOGTEXT(" \n Done...\n"); LOGDOIT;
return(0);
}*/

//
//--------------------------------------------------------------------------
// read_cob()
//
//--------------------------------------------------------------------------
//
ERRORCODE CImportCOB::ReadCOB(CFile *f)
{
	unsigned int i,j,Chunkend,texture;
	COB_HEADER cobheader;
	CHUNK_HEADER chunkheader;
	unsigned short Polhname;
	unsigned short Polhn_string;
	char Polhstring[100];
	unsigned int position;
	short identifier;
	POLH_DATA Polhdata;
	unsigned short Grouname;
	unsigned short Groun_string;
	char Groustring[100];
	GROU_DATA Groudata[SOBJ_MAX];
	unsigned int Parentid[SOBJ_MAX];

	unsigned int rd_flag,matltflag,GroupFlagError;
	unsigned int n_vertix,n_uv,n_facehole;
	unsigned int VStartSet,UVStartSet;
	CVPNT Vin;
	char *pid;
	unsigned int red,blue,green;


	LOGTEXT("Source file: %s\n",f->GetFilePath()); LOGDOIT;
	LOGTEXT("\n"); LOGDOIT;
	LOGTEXT("Reading COB file ->\n"); LOGDOIT;
	//
	//-------------------------
	// read cob header
	//-------------------------
	//
	f->Read(&cobheader, sizeof(COB_HEADER));
	if(cobheader.Format!='B')
	{
		errstring=ERROR_COB_NOTBINFORMAT_TEXT;
		return ERROR_COB_NOTBINFORMAT;
	}

	//
	//-------------------------
	// read chunks...
	//-------------------------
	//
	Cmodel.Vcount=0;
	Cmodel.UVcount=0;
	Cmodel.Pcount=0;
	Cmodel.Scount=0;
	Chunkend=1;
	numtextures=0;
	Matlcount=0;
	chunknum=0;
	GroupFlagError=0;

	while(Chunkend==1)
	{
		f->Read(&chunkheader, sizeof(CHUNK_HEADER));
		pid=(char *)&chunkheader.Type;
		//LOGTEXT(" Reading chunk at %i (%c%c%c%c)\n", f->GetPosition(),*pid,*(pid+1),*(pid+2),*(pid+3)); LOGDOIT;
		switch(chunkheader.Type)
		{
		case D_END:
			Chunkend=0;
			break;

		case D_GROU:
			if(GroupFlagError!=0)
			{
				errstring=ERROR_COB_WRONGGROUPING_TEXT;
				return ERROR_COB_WRONGGROUPING;
			}
			f->Read(&Grouname, sizeof(short));
			f->Read(&Groun_string, sizeof(short));
			//LOGTEXT("  Grou name: "); LOGDOIT;
			for(i=0;i<Groun_string;i++)
			{
				f->Read(&Groustring[i], sizeof(char));
				//LOGTEXT(" %c",Groustring[i]); LOGDOIT;
			}
			Groustring[Groun_string]=0;
			//LOGTEXT(" ,%d -",Grouname); LOGDOIT;
			if(Groun_string!=0)
				strcpy_s(Cmodel.Sname[Cmodel.Scount],Groustring);
			else
				strcpy_s(Cmodel.Sname[Cmodel.Scount],"None");
			//LOGTEXT("  %s\n",Cmodel.Sname[Cmodel.Scount]); LOGDOIT;
			f->Read(&Groudata[Cmodel.Scount], sizeof(GROU_DATA));

			Cmodel.Soffset[Cmodel.Scount].x=Groudata[Cmodel.Scount].Center.x;
			Cmodel.Soffset[Cmodel.Scount].y=Groudata[Cmodel.Scount].Center.y;
			Cmodel.Soffset[Cmodel.Scount].z=Groudata[Cmodel.Scount].Center.z;
			Parentid[Cmodel.Scount]=chunkheader.Chunkid;
			for(i=0;i<Cmodel.Scount;i++)
			{
				if(chunkheader.Parentid==Parentid[i])
					Cmodel.parent[Cmodel.Scount]=i;
			}
			Cmodel.Scount++;
			if(Cmodel.Scount>SOBJ_MAX)
			{
				errstring=ERROR_COB_TOOMANYSECTIONS_TEXT;
				errdetails.Format("Should be <=%i",SOBJ_MAX);
				return ERROR_COB_TOOMANYSECTIONS;
			}
			break;

		case D_MAT1:
			position=(unsigned int)f->GetPosition();
			f->Read(&Matldata[Matlcount], sizeof(MATL_DATA));
			if(f->GetPosition()>=(position+chunkheader.Size))
			{ // flatpoly
				red=(unsigned int)(Matldata[Matlcount].red*0x00ff);
				green=(unsigned int)(Matldata[Matlcount].green*0x00ff);
				blue=(unsigned int)(Matldata[Matlcount].blue*0x00ff);
				//LOGTEXT("  Flatpoly: red=%d, green=%d, blue=%d\n",red,green,blue); LOGDOIT;
				green=green << 8;
				blue=blue << 16;

				for(i=0;i<Cmodel.Pcount;i++)
				{
					if(Cmodel.Poly[Cmodel.Pcount-1].chunk==Cmodel.Poly[i].chunk)
					{
						if(Cmodel.Poly[i].chunkmatl==Matldata[Matlcount].number)
						{
							Cmodel.Poly[i].material=red+green+blue;
							//LOGTEXT("  material number=%04x\n",Cmodel.Poly[i].material); LOGDOIT;
							Cmodel.Poly[i].matlindex=0;
							Cmodel.Poly[i].polytype=2;
							Cmodel.Poly[i].facet=Matldata[Matlcount].facet;
						}
					}
				}
			}

			while(f->GetPosition()<(position+chunkheader.Size))
			{
				f->Read(&identifier, sizeof(short));
				switch(identifier)
				{
				case 0x3a65: //:e
					//LOGTEXT("  Got an e: \n"); LOGDOIT;
					f->Read(&Polhn_string, sizeof(short));
					for(i=0;i<Polhn_string;i++)
						f->Read(&Polhstring[i], sizeof(char));
					break;

				case 0x3a74: //:t
					//LOGTEXT("  Got a t: "); LOGDOIT;
					matltflag=0; //FixWinCOB2FS: char->long
					f->Read(&matltflag, sizeof(char));
					f->Read(&Polhn_string, sizeof(short));
					for(i=0;i<Polhn_string;i++)
						f->Read(&Matlname[numtextures][i], sizeof(char));
					Matlname[numtextures][Polhn_string]=0;
					//LOGTEXT(" %s - ",Matlname[numtextures]); LOGDOIT;
					f->Read(&Matltdata[Matlcount], sizeof(MATLT_DATA));
					f->Seek(position+chunkheader.Size,CFile::begin);
					texture=0xff;
					for(i=0;i<numtextures;i++)
					if(strcmp(Matlname[i], Matlname[numtextures])==0)
						texture=i;
					if(texture==0xff)
					{
						texture=numtextures;
						LOGTEXT(" Texture #%d - %s\n",texture,Matlname[numtextures]); LOGDOIT;
						numtextures++;
						if(numtextures>TEXTURE_MAX)
						{
							errstring=ERROR_COB_TOOMANYTEXTURES_TEXT;
							errdetails.Format("Should be <=%i",TEXTURE_MAX);
							return ERROR_COB_TOOMANYTEXTURES;
						}
					}
					else
					{
						//LOGTEXT(" already added\n"); LOGDOIT;
					}

					for(i=0;i<Cmodel.Pcount;i++)
					{
						if(Cmodel.Poly[Cmodel.Pcount-1].chunk==Cmodel.Poly[i].chunk)
						{
							if(Cmodel.Poly[i].chunkmatl==Matldata[Matlcount].number)
							{
								Cmodel.Poly[i].material=texture;
								Cmodel.Poly[i].matlindex=Matlcount;
								Cmodel.Poly[i].facet=Matldata[Matlcount].facet;
							}
						}
					}
					// Matlcount++;
					break;

				case 0x3a62: //:b
					// LOGTEXT("  got a b:\n"); LOGDOIT;
					break;

				default:
					LOGTEXT("  big oops...\n"); LOGDOIT;
				}
			}
			Matlcount++;
			//LOGTEXT("  Matlcount=%d\n",Matlcount); LOGDOIT;
			if(Matlcount>MATL_MAX)
			{
				errstring=ERROR_COB_TOOMANYMATLS_TEXT;
				errdetails.Format("Should be <=%i",MATL_MAX);
				return ERROR_COB_TOOMANYMATLS;
			}
			// fseek(file,chunkheader.Size,SEEK_CUR);
			break;

		case D_POLH:
			position=(unsigned int) f->GetPosition();
			//fseek(file,chunkheader.Size,SEEK_CUR);
			if(Cmodel.Scount==0)
			{
				GroupFlagError=0xff;
				Cmodel.Scount=1;
			}
			VStartSet=Cmodel.Vcount;
			UVStartSet=Cmodel.UVcount;
			f->Read(&Polhname, sizeof(short));
			f->Read(&Polhn_string, sizeof(short));
			LOGTEXT(" Submodel: name="); LOGDOIT;
			for(i=0;i<Polhn_string;i++)
			{
				f->Read(&Polhstring[i], sizeof(char));
				LOGTEXT("%c",Polhstring[i]); LOGDOIT;
			}
			LOGTEXT(", id=%d, ",Polhname); LOGDOIT;

			f->Read(&Polhdata, sizeof(POLH_DATA));
			f->Read(&n_vertix, sizeof(long));
			LOGTEXT("n_vertix=%ld, ", n_vertix); LOGDOIT;
			for(i=0;i<n_vertix;i++)
			{
				//f->Read(&Cmodel.Vpoint[Cmodel.Vcount], sizeof(CVPNT));
				f->Read(&Vin, sizeof(CVPNT));
				Cmodel.Vpoint[Cmodel.Vcount].x=Vin.x*Polhdata.Matrix1.j+
											 Vin.y*Polhdata.Matrix1.k+
											 Vin.z*Polhdata.Matrix1.l+
											 Polhdata.Matrix1.m;

				Cmodel.Vpoint[Cmodel.Vcount].y=Vin.x*Polhdata.Matrix2.j+
											 Vin.y*Polhdata.Matrix2.k+
											 Vin.z*Polhdata.Matrix2.l+
											 Polhdata.Matrix2.m;

				Cmodel.Vpoint[Cmodel.Vcount].z=Vin.x*Polhdata.Matrix3.j+
											 Vin.y*Polhdata.Matrix3.k+
											 Vin.z*Polhdata.Matrix3.l+
											 Polhdata.Matrix3.m;

				Cmodel.Vpntseg[Cmodel.Vcount]=Cmodel.Scount-1;
				Cmodel.Vpntchunk[Cmodel.Vcount]=chunknum;
				Cmodel.Vcount++;
				if(Cmodel.Vcount>VEC_MAX)
				{
					errstring=ERROR_COB_TOOMANYVERTICES_TEXT;
					errdetails.Format("Should be <=%i",VEC_MAX);
					return ERROR_COB_TOOMANYVERTICES;
				}
			}
			f->Read(&n_uv, sizeof(long));
			// LOGTEXT("  n_uv=%ld\n", n_uv); LOGDOIT;
			for(i=0;i<n_uv;i++)
			{
				f->Read(&Cmodel.UVpoint[Cmodel.UVcount], sizeof(CUV));
				Cmodel.UVcount++;
				if(Cmodel.UVcount>VEC_MAX*2)
				{
					errstring=ERROR_COB_TOOMANYUVS_TEXT;
					errdetails.Format("Should be <=%i",VEC_MAX*2);
					return ERROR_COB_TOOMANYUVS;
				}
			}
			f->Read(&n_facehole, sizeof(long));
			LOGTEXT("n_facehole=%ld, ", n_facehole); LOGDOIT;
			for(i=0;i<n_facehole;i++)
			{
				rd_flag=0; //FixWinCOB2FS: char -> long
				f->Read(&rd_flag, sizeof(char));
				Cmodel.Poly[Cmodel.Pcount].n_index=0; //HH: FixWinCOB2FS-bad bug: we read a short into an unsigned uninitialized long, uhoh
				f->Read(&Cmodel.Poly[Cmodel.Pcount].n_index, sizeof(short));
				if(Cmodel.Poly[Cmodel.Pcount].n_index>20)
				{
					errstring=ERROR_COB_TOOMANYPOLYGONPOINTS_TEXT;
					errdetails.Format("Polygon #%i - Is: %i, Should be <=%i",Cmodel.Pcount,Cmodel.Poly[Cmodel.Pcount].n_index,20);
					return ERROR_COB_TOOMANYPOLYGONPOINTS;
				}
				if((rd_flag&0x08)==0)
				{
					Cmodel.Poly[Cmodel.Pcount].material=0; //HH: FixWinCOB2FS-bad bug: we read a short into an unsigned uninitialized long, uhoh
					f->Read(&Cmodel.Poly[Cmodel.Pcount].material, sizeof(short));
					Cmodel.Poly[Cmodel.Pcount].chunkmatl=Cmodel.Poly[Cmodel.Pcount].material;
				}
				for(j=0;j<Cmodel.Poly[Cmodel.Pcount].n_index;j++)
				{
					f->Read(&Cmodel.Poly[Cmodel.Pcount].index[j], sizeof(CINDEX));
					Cmodel.Poly[Cmodel.Pcount].index[j].v+=VStartSet;
					Cmodel.Poly[Cmodel.Pcount].index[j].uv+=UVStartSet;
				}
				Cmodel.Poly[Cmodel.Pcount].segment=Cmodel.Scount-1;
				Cmodel.Poly[Cmodel.Pcount].chunk=chunknum;
				Cmodel.Poly[Cmodel.Pcount].polytype=3;
				Cmodel.Pcount++;
				if(Cmodel.Pcount>POLY_MAX)
				{
					errstring=ERROR_COB_TOOMANYPOLYGONS_TEXT;
					errdetails.Format("Should be <=%i",POLY_MAX);
					return ERROR_COB_TOOMANYPOLYGONS;
				}
			}
			chunknum++;
			if(chunknum>SOBJ_MAX)
			{
				errstring=ERROR_COB_TOOMANYCHUNKS_TEXT;
				errdetails.Format("Should be <=%i",SOBJ_MAX);
				return ERROR_COB_TOOMANYCHUNKS;
			}
			LOGTEXT("V=%d, P=%d, UV=%d, S=%d\n",Cmodel.Vcount,Cmodel.Pcount,Cmodel.UVcount,Cmodel.Scount-1); LOGDOIT;
			f->Seek((position+chunkheader.Size),CFile::begin);
			break;

		default:
			//LOGTEXT("  (Skipping)\n"); LOGDOIT;
			f->Seek(chunkheader.Size,CFile::current);
		}
	}
	
	return ERROR_GEN_NOERROR;
}




// Convert data to garrys stuff...

void CImportCOB::ReduceToUnit(float vector[3])
{
	float length;

	length=(float)sqrt((vector[0]*vector[0]) +
						 (vector[1]*vector[1]) +
						 (vector[2]*vector[2]));

	if(length==0.0f)
		length=1.0f;

	vector[0]/=length;
	vector[1]/=length;
	vector[2]/=length;
}


void CImportCOB::calcNormal(float v[3][3], float out[3])
{
	double v1[3],v2[3];
	static const int x=0;
	static const int y=1;
	static const int z=2;

	v1[x]=v[0][x]-v[1][x];
	v1[y]=v[0][y]-v[1][y];
	v1[z]=v[0][z]-v[1][z];

	v2[x]=v[1][x]-v[2][x];
	v2[y]=v[1][y]-v[2][y];
	v2[z]=v[1][z]-v[2][z];

	out[x]=(float)(v1[y]*v2[z]-v1[z]*v2[y]);
	out[y]=(float)(v1[z]*v2[x]-v1[x]*v2[z]);
	out[z]=(float)(v1[x]*v2[y]-v1[y]*v2[x]);

	ReduceToUnit(out);
}

//
//--------------------------------------------------------------------------
// copy cob data --> garry's
//
//--------------------------------------------------------------------------
//
void CImportCOB::ConvertCob2Model(void)
{
	unsigned int i,j,k,gotdetail,m,n,n_smooth;
	float v[3][3];
	float vout[3];
	unsigned char SNum[SOBJ_MAX];
	unsigned char CNum[SOBJ_MAX];
	float minx,miny,minz,maxx,maxy,maxz,Radius,cenx,ceny,cenz;
	float smoothx,smoothy,smoothz,smooth[3];
	double maxxyz;

	LOGTEXT("\n"); LOGDOIT;
	LOGTEXT("Converting to POF model data ->\n"); LOGDOIT;

	// remove extra points...

	for(i=0;i<Cmodel.Vcount;i++)		 // find the valid points...
		Cmodel.Vpntreal[i]=0xff;
	for(i=0;i<Cmodel.Pcount;i++)
	{
		for(j=0;j<Cmodel.Poly[i].n_index;j++)
			Cmodel.Vpntreal[Cmodel.Poly[i].index[j].v]=0;
	}

	//
	//-------------------------
	// Transfer data...
	//-------------------------
	//
	FSModel.Vcount=0;
	FSModel.Ncount=0;
	FSModel.BBcount=0;
	FSModel.Pcount=0;
	FSModel.Scount=0;

	FSModel.Scount=Cmodel.Scount;
	for(i=0;i<FSModel.Scount;i++)
	{
		FSModel.Soffset[i].x=(float)(-Cmodel.Soffset[i].y*20.0); // yzxsign change
		FSModel.Soffset[i].y=(float)(Cmodel.Soffset[i].z*20.0);
		FSModel.Soffset[i].z=(float)(Cmodel.Soffset[i].x*20.0);
		FSModel.Sparent[i]=Cmodel.parent[i];
		strcpy_s(FSModel.Sname[i],Cmodel.Sname[i]);
	}
	FSModel.Soffset[0].x=0;
	FSModel.Soffset[0].y=0;
	FSModel.Soffset[0].z=0;
	FSModel.Sparent[0]=-1;

	FSModel.Debriscount=0;

	for(i=0;i<FSModel.Scount;i++)
	{
		if(_strnicmp(FSModel.Sname[i],"debris",6)==0)
		{
			FSModel.Sdebris[FSModel.Debriscount]=i;
			FSModel.Debriscount++;
			FSModel.Sparent[i]=-1;
		}
	}

	FSModel.Sdetail[0]=0;
	FSModel.Detailcount=1;

	gotdetail=0;
	for(i=1;i<FSModel.Scount;i++)
	{
		if(_strnicmp(FSModel.Sname[i],"detail1",7)==0)
		{
			FSModel.Sdetail[FSModel.Detailcount]=i;
			FSModel.Detailcount++;
			FSModel.Sparent[i]=-1;
			gotdetail++;
			if(gotdetail>1)
			{
				LOGTEXT("  Error: two groups with 'detail1' name"); LOGDOIT;
				exit(0);
			}
		}
	}

	gotdetail=0;
	for(i=1;i<FSModel.Scount;i++)
	{
		if(_strnicmp(FSModel.Sname[i],"detail2",7)==0)
		{
			FSModel.Sdetail[FSModel.Detailcount]=i;
			FSModel.Detailcount++;
			FSModel.Sparent[i]=-1;
			gotdetail++;
			if(gotdetail>1)
			{
				LOGTEXT("  Error: two groups with 'detail2' name"); LOGDOIT;
				exit(0);
			}
		}
	}

	gotdetail=0;
	for(i=1;i<FSModel.Scount;i++)
	{
		if(_strnicmp(FSModel.Sname[i],"detail3",7)==0)
		{
			FSModel.Sdetail[FSModel.Detailcount]=i;
			FSModel.Detailcount++;
			FSModel.Sparent[i]=-1;
			gotdetail++;
			if(gotdetail>1)
			{
				LOGTEXT("  Error: two groups with 'detail3' name"); LOGDOIT;
				exit(0);
			}
		}
	}

	FSModel.shield=0;
	gotdetail=0;
	for(i=1;i<FSModel.Scount;i++)
	{
		if(_strnicmp(FSModel.Sname[i],"shield",6)==0)
		{
			FSModel.shield=i;
			FSModel.Sparent[i]=-1;
			gotdetail++;
			if(gotdetail>1)
			{
				LOGTEXT("  Error: two groups with 'shield' name"); LOGDOIT;
				exit(0);
			}
		}
	}

	LOGTEXT(" Statistics:\n"); LOGDOIT;
	LOGTEXT("  parent="); LOGDOIT;
	for(i=0;i<FSModel.Scount;i++)
		LOGTEXT(" %ld", Cmodel.parent[i]); LOGDOIT;
	LOGTEXT("\n"); LOGDOIT;
	for(i=0;i<SOBJ_MAX;i++)
		SNum[i]=0;
	for(i=0;i<SOBJ_MAX;i++)
		CNum[i]=0;
	for(i=0;i<Cmodel.Vcount;i++)
	{
		SNum[Cmodel.Vpntseg[i]]++;
		CNum[Cmodel.Vpntchunk[i]]++;
	}
	LOGTEXT("  Segments:"); LOGDOIT;
	for(i=0;i<Cmodel.Scount;i++)
		LOGTEXT(" %d",SNum[i]); LOGDOIT;
	LOGTEXT("\n"); LOGDOIT;
	LOGTEXT("  Chunks:"); LOGDOIT;
	for(i=0;i<chunknum;i++)
		LOGTEXT(" %d",CNum[i]); LOGDOIT;
	LOGTEXT("\n"); LOGDOIT;

	for(i=0;i<Cmodel.Vcount;i++)
	{
		if(Cmodel.Vpntreal[i]==0)
		{
			FSModel.Vpoint[FSModel.Vcount].x=(float)(-Cmodel.Vpoint[i].y*20.0);		// yzx sign change
			FSModel.Vpoint[FSModel.Vcount].y=(float)(Cmodel.Vpoint[i].z*20.0);
			FSModel.Vpoint[FSModel.Vcount].z=(float)(Cmodel.Vpoint[i].x*20.0);
			FSModel.Vpntseg[FSModel.Vcount]=Cmodel.Vpntseg[i];
			FSModel.Vpntchunk[FSModel.Vcount]=Cmodel.Vpntchunk[i];
			Cmodel.Vpntfinal[i]=FSModel.Vcount++;
		}
	}

	LOGTEXT("  Vertices count: cob file=%d, pof file=%d\n",Cmodel.Vcount,FSModel.Vcount); LOGDOIT;

	for(i=1;i<FSModel.Scount;i++)
	{
		LOGTEXT("  Soffset %d:",i); LOGDOIT;
		LOGTEXT("  %f,",FSModel.Soffset[i].x); LOGDOIT;
		LOGTEXT("  %f,",FSModel.Soffset[i].y); LOGDOIT;
		LOGTEXT("  %f\n",FSModel.Soffset[i].z); LOGDOIT;
		for(j=0;j<FSModel.Vcount;j++)
		{
			if(FSModel.Vpntseg[j]==i)
			{
				FSModel.Vpoint[j].x-=FSModel.Soffset[i].x;
				FSModel.Vpoint[j].y-=FSModel.Soffset[i].y;
				FSModel.Vpoint[j].z-=FSModel.Soffset[i].z;
			}
		}
	}

	FSModel.Pcount=Cmodel.Pcount;
	for(i=0;i<Cmodel.Pcount;i++)
	{
		FSModel.Poly[i].Corners=Cmodel.Poly[i].n_index;
		FSModel.Poly[i].Colors=Cmodel.Poly[i].material;
		FSModel.Poly[i].facet=Cmodel.Poly[i].facet;
		FSModel.Poly[i].Segment=Cmodel.Poly[i].segment;
		FSModel.Poly[i].Chunk=Cmodel.Poly[i].chunk;
		FSModel.Poly[i].Ptype=Cmodel.Poly[i].polytype;

		for(j=0;j<Cmodel.Poly[i].n_index;j++)
		{
			FSModel.Poly[i].Vp[j]= Cmodel.Vpntfinal[Cmodel.Poly[i].index[j].v];
			FSModel.Poly[i].U[j]=Cmodel.UVpoint[Cmodel.Poly[i].index[j].uv].u;
			FSModel.Poly[i].V[j]=-Cmodel.UVpoint[Cmodel.Poly[i].index[j].uv].v;

			ASSERT(Cmodel.Poly[i].matlindex!=0xcdcdcdcd); //Fix WinCOB2FS
			if(Matltdata[Cmodel.Poly[i].matlindex].urepeat!=0)
				FSModel.Poly[i].U[j]*=Matltdata[Cmodel.Poly[i].matlindex].urepeat;
			if(Matltdata[Cmodel.Poly[i].matlindex].vrepeat!=0)
				FSModel.Poly[i].V[j]*=Matltdata[Cmodel.Poly[i].matlindex].vrepeat;

			FSModel.Poly[i].U[j]-=Matltdata[Cmodel.Poly[i].matlindex].uoffset;
			FSModel.Poly[i].V[j]+=Matltdata[Cmodel.Poly[i].matlindex].voffset;
		}

		v[0][0]=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].x;
		v[0][1]=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].y;
		v[0][2]=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].z;
		v[1][0]=FSModel.Vpoint[FSModel.Poly[i].Vp[1]].x;
		v[1][1]=FSModel.Vpoint[FSModel.Poly[i].Vp[1]].y;
		v[1][2]=FSModel.Vpoint[FSModel.Poly[i].Vp[1]].z;
		v[2][0]=FSModel.Vpoint[FSModel.Poly[i].Vp[2]].x;
		v[2][1]=FSModel.Vpoint[FSModel.Poly[i].Vp[2]].y;
		v[2][2]=FSModel.Vpoint[FSModel.Poly[i].Vp[2]].z;
		calcNormal(v,vout);
		FSModel.Poly[i].Normal.x=vout[0];
		FSModel.Poly[i].Normal.y=vout[1];
		FSModel.Poly[i].Normal.z=vout[2];
	}


	for(i=0;i<FSModel.Pcount;i++)
	{
		cenx=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].x;
		ceny=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].y;
		cenz=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].z;
		maxx=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].x;
		maxy=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].y;
		maxz=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].z;
		minx=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].x;
		miny=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].y;
		minz=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].z;
		for(j=1;j<FSModel.Poly[i].Corners;j++)
		{
			cenx+=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x;
			ceny+=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y;
			cenz+=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z;
			if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x>maxx)
				maxx=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x;
			if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y>maxy)
				maxy=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y;
			if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z>maxz)
				maxz=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z;
			if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x<minx)
				minx=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x;
			if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y<miny)
				miny=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y;
			if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z<minz)
				minz=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z;
		}

		FSModel.Poly[i].Center.x=cenx/FSModel.Poly[i].Corners;
		FSModel.Poly[i].Center.y=ceny/FSModel.Poly[i].Corners;
		FSModel.Poly[i].Center.z=cenz/FSModel.Poly[i].Corners;

		maxx=FSModel.Poly[i].Center.x-FSModel.Vpoint[FSModel.Poly[i].Vp[0]].x;
		maxy=FSModel.Poly[i].Center.y-FSModel.Vpoint[FSModel.Poly[i].Vp[0]].y;
		maxz=FSModel.Poly[i].Center.z-FSModel.Vpoint[FSModel.Poly[i].Vp[0]].z;
		maxxyz=((maxx*maxx)+(maxy*maxy)+(maxz*maxz));
		FSModel.Poly[i].Radius=(float)sqrt(maxxyz);

		for(j=1;j<FSModel.Poly[i].Corners;j++)
		{
			maxx=FSModel.Poly[i].Center.x-FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x;
			maxy=FSModel.Poly[i].Center.y-FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y;
			maxz=FSModel.Poly[i].Center.z-FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z;
			maxxyz=((maxx*maxx)+(maxy*maxy)+(maxz*maxz));
			Radius=(float)sqrt(maxxyz);
			if(FSModel.Poly[i].Radius<Radius)	FSModel.Poly[i].Radius=Radius;
		}
	}

	for(i=0;i<FSModel.Vcount;i++)
	{
		FSModel.n_vn[i]=0;
		for(j=0;j<FSModel.Pcount;j++)
		{
			for(k=0;k<FSModel.Poly[j].Corners;k++)
			{
				if(FSModel.Poly[j].Vp[k]==i)
				{
					if(FSModel.Poly[j].facet!='s')
					{
						FSModel.Npoint[FSModel.Ncount].x=FSModel.Poly[j].Normal.x;
						FSModel.Npoint[FSModel.Ncount].y=FSModel.Poly[j].Normal.y;
						FSModel.Npoint[FSModel.Ncount].z=FSModel.Poly[j].Normal.z;
					} else {
						n_smooth=0;
						smoothx=0;
						smoothy=0;
						smoothz=0;
						for(m=0;m<FSModel.Pcount;m++)
						{
							if(FSModel.Poly[m].facet=='s')
							{
								for(n=0;n<FSModel.Poly[m].Corners;n++)
								{
									if(FSModel.Poly[m].Vp[n]==FSModel.Poly[j].Vp[k])
									{
										smoothx+=FSModel.Poly[m].Normal.x;
										smoothy+=FSModel.Poly[m].Normal.y;
										smoothz+=FSModel.Poly[m].Normal.z;
										n_smooth++;
									}
								}
							}
						}

						smooth[0]=smoothx/n_smooth; 
						smooth[1]=smoothy/n_smooth;
						smooth[2]=smoothz/n_smooth;
						ReduceToUnit(smooth);

						FSModel.Npoint[FSModel.Ncount].x=smooth[0];
						FSModel.Npoint[FSModel.Ncount].y=smooth[1];
						FSModel.Npoint[FSModel.Ncount].z=smooth[2];
					}
					FSModel.Poly[j].Np[k]=FSModel.Ncount;
					FSModel.Ncount++;
					FSModel.n_vn[i]++;
				}
			}
		}
	}
}





//********************************
//
// how about a new sort routine...
//
//********************************

#define wc(walker)(*((unsigned char *) (walker)))
#define ws(walker)(*((unsigned short *) (walker)))
#define wi(walker)(*((unsigned int *) (walker)))
#define wf(walker)(*((float *) (walker)))
#define v(walker)(*((FS_VPNT *) (walker)))
#define wp(walker) ((unsigned short *) (walker))
#define vp(walker) ((FS_VPNT *) (walker))


//*************************************************************
// OHDR Routines*
//*************************************************************
void CImportCOB::ohdr(void)
{
	unsigned int i;
	FS_VPNT Max,Min;
	float minx,miny,minz,maxx,maxy,maxz,Radius,Radius2;
	float tmpx,tmpy,tmpz;
	double maxxyz;

	//FSModel.Soffset[0].x

	LOGTEXT(" Building chunk OHDR/HDR2\n"); LOGDOIT;
	tmpx=FSModel.Vpoint[0].x+FSModel.Soffset[FSModel.Vpntseg[0]].x;
	tmpy=FSModel.Vpoint[0].y+FSModel.Soffset[FSModel.Vpntseg[0]].y;
	tmpz=FSModel.Vpoint[0].z+FSModel.Soffset[FSModel.Vpntseg[0]].z;
	maxx=tmpx;
	maxy=tmpy;
	maxz=tmpz;
	minx=tmpx;
	miny=tmpy;
	minz=tmpz;
	maxxyz=((tmpx*tmpx)+(tmpy*tmpy)+(tmpz*tmpz));
	Radius=(float)sqrt(maxxyz);
	for(i=1;i<FSModel.Vcount;i++)
	{
		tmpx=FSModel.Vpoint[i].x+FSModel.Soffset[FSModel.Vpntseg[i]].x;
		tmpy=FSModel.Vpoint[i].y+FSModel.Soffset[FSModel.Vpntseg[i]].y;
		tmpz=FSModel.Vpoint[i].z+FSModel.Soffset[FSModel.Vpntseg[i]].z;
		if(tmpx>maxx)	maxx=tmpx;
		if(tmpy>maxy)	maxy=tmpy;
		if(tmpz>maxz)	maxz=tmpz;
		if(tmpx<minx)	minx=tmpx;
		if(tmpy<miny)	miny=tmpy;
		if(tmpz<minz)	minz=tmpz;
		maxxyz=((tmpx*tmpx)+(tmpy*tmpy)+(tmpz*tmpz));
		Radius2=(float)sqrt(maxxyz);
		if(Radius<Radius2) Radius=Radius2;
	}
	Max.x=maxx;
	Max.y=maxy;
	Max.z=maxz;
	Min.x=minx;
	Min.y=miny;
	Min.z=minz;


//Center.x=maxx-(maxx-minx)/2;
//Center.y=maxy-(maxy-miny)/2;
//Center.z=maxz-(maxz-minz)/2;

//maxx=Center.x-FSModel.Vpoint[0].x;
//maxy=Center.y-FSModel.Vpoint[0].y;
//maxz=Center.z-FSModel.Vpoint[0].z;
//maxxyz=((maxx*maxx)+(maxy*maxy)+(maxz*maxz));
//Radius=sqrt(maxxyz);

//for (i=1;i<FSModel.Vcount;i++)
//	{
//maxx=Center.x-FSModel.Vpoint[i].x;
//maxy=Center.y-FSModel.Vpoint[i].y;
//maxz=Center.z-FSModel.Vpoint[i].z;
//maxxyz=((maxx*maxx)+(maxy*maxy)+(maxz*maxz));
//Radius2=sqrt(maxxyz);
//if (Radius<Radius2) Radius=Radius2;
//}

	//Save to doc
	if(FSModel.shield!=0)
		GetDocument()->m_FS_PMinfo.n_models=FSModel.Scount-1;	// num sobj
	else
		GetDocument()->m_FS_PMinfo.n_models=FSModel.Scount;	// num sobj
	GetDocument()->m_FS_Model.radius=Radius;
	GetDocument()->m_FS_ModelInfo.obj_flags=1;
	GetDocument()->m_FS_Model._minbox.x=Min.x;
	GetDocument()->m_FS_Model._minbox.y=Min.y;
	GetDocument()->m_FS_Model._minbox.z=Min.z;
	GetDocument()->m_FS_Model._maxbox.x=Max.x;
	GetDocument()->m_FS_Model._maxbox.y=Max.y;
	GetDocument()->m_FS_Model._maxbox.z=Max.z;

	//Detail levels
	GetDocument()->m_FS_PMinfo.n_detail=FSModel.Detailcount;
	for(i=0;i<FSModel.Detailcount;i++)
	{
		if(FSModel.shield!=0)
		{
			if(FSModel.shield<(long)FSModel.Sdetail[i])
				GetDocument()->m_FS_PMinfo.detail[i]=FSModel.Sdetail[i]-1;
			else
				GetDocument()->m_FS_PMinfo.detail[i]=FSModel.Sdetail[i];
		} else
			GetDocument()->m_FS_PMinfo.detail[i]=FSModel.Sdetail[i];
	}

	//Debris count
	GetDocument()->m_FS_PMinfo.n_debris=FSModel.Debriscount;
	for(i=0;i<FSModel.Debriscount;i++)
	{
		if(FSModel.shield!=0)
		{
			if(FSModel.shield<(long)FSModel.Sdetail[i])
				GetDocument()->m_FS_PMinfo.debris[i]=FSModel.Sdebris[i]-1;
			else
				GetDocument()->m_FS_PMinfo.debris[i]=FSModel.Sdebris[i];
		} else
			GetDocument()->m_FS_PMinfo.debris[i]=FSModel.Sdebris[i];
	}

	GetDocument()->m_FS_ModelInfo.mass=10000;
	GetDocument()->m_FS_ModelInfo.mass_center.x=0;
	GetDocument()->m_FS_ModelInfo.mass_center.y=0;
	GetDocument()->m_FS_ModelInfo.mass_center.z=0;
	GetDocument()->m_FS_ModelInfo.moment_inertia[0][0]=0;
	GetDocument()->m_FS_ModelInfo.moment_inertia[0][1]=0;
	GetDocument()->m_FS_ModelInfo.moment_inertia[0][2]=0;
	GetDocument()->m_FS_ModelInfo.moment_inertia[1][0]=0;
	GetDocument()->m_FS_ModelInfo.moment_inertia[1][1]=0;
	GetDocument()->m_FS_ModelInfo.moment_inertia[1][2]=0;
	GetDocument()->m_FS_ModelInfo.moment_inertia[2][0]=0;
	GetDocument()->m_FS_ModelInfo.moment_inertia[2][1]=0;
	GetDocument()->m_FS_ModelInfo.moment_inertia[2][2]=0;

	GetDocument()->m_FS_ModelInfo.num_cross_sections=0;
	GetDocument()->m_FS_ModelInfo.num_lights=0;

	//Create chunk
	/*wi(walker+OutputLength)=ID_OHDR;
	OutputLength+=4;

	FS_VPNT test1,Center;
	unsigned int SizeAt,StartSize;
	unsigned char *walker=OutputData;

	wi(walker+OutputLength)=0x01234567;
	SizeAt=OutputLength;					// to fix the size at the end...
	OutputLength+=4;
	StartSize=OutputLength;				// start for the size stuff

	if(FSModel.shield!=0)
	{
		wi(walker+OutputLength)=FSModel.Scount-1;	// num sobj
	} else {
		wi(walker+OutputLength)=FSModel.Scount;	// num sobj
	}
	OutputLength+=4;

	wf(walker+OutputLength)=Radius;	 // radius
	OutputLength+=4;

	wi(walker+OutputLength)=1;	// obj flags
	OutputLength+=4;

	v(walker+OutputLength)=Min;		 // min
	OutputLength+=12;
	v(walker+OutputLength)=Max;		 // max
	OutputLength+=12;

	wi(walker+OutputLength)=FSModel.Detailcount;		// detail levels
	OutputLength+=4;
	for(i=0;i<FSModel.Detailcount;i++)
	{
		if(FSModel.shield!=0)
		{
			if(FSModel.shield<(long)FSModel.Sdetail[i])
				wi(walker+OutputLength)=FSModel.Sdetail[i]-1;
			else
				wi(walker+OutputLength)=FSModel.Sdetail[i];
		} else
			wi(walker+OutputLength)=FSModel.Sdetail[i];
		OutputLength+=4;
	}


	wi(walker+OutputLength)=FSModel.Debriscount;		// debris levels
	OutputLength+=4;
	for(i=0;i<FSModel.Debriscount;i++)
	{
		if(FSModel.shield!=0)
		{
			if(FSModel.shield<(long)FSModel.Sdetail[i])
			{
				wi(walker+OutputLength)=FSModel.Sdebris[i]-1;
			} else {
				wi(walker+OutputLength)=FSModel.Sdebris[i];
			}
		} else {
			wi(walker+OutputLength)=FSModel.Sdebris[i];
		}
		OutputLength+=4;
	}


	wf(walker+OutputLength)=10000;	// mass
	OutputLength+=4;

	Center.x=0;
	Center.y=0;
	Center.z=0;

	v(walker+OutputLength)=Center;		// center
	OutputLength+=12;

	test1.x=0;
	test1.y=0;
	test1.z=0;

	v(walker+OutputLength)=test1;		 // moment
	OutputLength+=12;
	v(walker+OutputLength)=test1;		 // moment2
	OutputLength+=12;
	v(walker+OutputLength)=test1;		 // moment3
	OutputLength+=12;

	wi(walker+OutputLength)=0;	// cross sections
	OutputLength+=4;

	wi(walker+OutputLength)=0;	// lights
	OutputLength+=4;

	wi(walker+SizeAt)=OutputLength-StartSize;*/
}


//*************************************************************
// TXTR Routines*
//*************************************************************
void CImportCOB::txtr(void)
{
	unsigned int j = 0, k = 0;
	char tname[25];

	GetDocument()->m_FS_BitmapData.count=numtextures;
	if(numtextures>0)
	{
		LOGTEXT(" Building chunk TXTR\n"); LOGDOIT;
		
		/*
		unsigned int SizeAt,StartSize;
		unsigned char *walker=OutputData;
		wi(walker+OutputLength)=ID_TXTR;
		OutputLength+=4;

		wi(walker+OutputLength)=0x01234567;
		SizeAt=OutputLength;					// to fix the size at the end...
		OutputLength+=4;
		StartSize=OutputLength;				// start for the size stuff

		wi(walker+OutputLength)=numtextures;
		OutputLength+=4;*/

		for(unsigned int h=0;h<numtextures;h++)
		{
			for(unsigned int i=0;i<strlen(Matlname[h]);i++)
			{
				if(Matlname[h][i]=='\\') j=i+1;
				if(Matlname[h][i]=='.') k=i;
			}
			strncpy_s(tname, Matlname[h]+j, k-j);
			tname[k-j]='\0';

			if(strlen(tname)<30)
				strcpy_s(GetDocument()->m_FS_BitmapData.pic[h].name,tname);
			else
				ASSERT(FALSE);
			/*else
				HHTODO:ERROR!!*/

			/*k=strlen(tname);
			wi(walker+OutputLength)=k;
			OutputLength+=4;
			for(i=0;i<k;i++)
			{
				wc(walker+OutputLength)=tname[i];
				OutputLength+=1;
			}*/
		}
		//wi(walker+SizeAt)=OutputLength-StartSize;
	}
}



//*************************************************************
// SOBJ Routines*
//*************************************************************
//
// SOBJ type 0 data
//
void CImportCOB::Dump0Data(void)
{
	unsigned char *walker=OutputData;

	//LOGTEXT("  doing 0: eof\n"); LOGDOIT;

	wi(walker+OutputLength)=D_EOF;
	OutputLength+=4;

	wi(walker+OutputLength)=0;
	OutputLength+=4;
}

//
// SOBJ type 1 data
//
void CImportCOB::Dump1Data(unsigned int Segment)
{
	unsigned int i,j,k,SizeAt,StartSize;
//	VPNT Vtemp;
	unsigned char *walker=OutputData;
	unsigned int n_verts,n_norms;

	n_verts=0;
	n_norms=0;
	for(i=0;i<FSModel.Vcount;i++)
	{
		if(FSModel.Vpntseg[i]==Segment)
		{
			n_verts++;
			n_norms+=FSModel.n_vn[i];
		}
	}

	LOGTEXT("n_verts=%d, ",n_verts); LOGDOIT;
	LOGTEXT("n_norms=%d\n",n_norms); LOGDOIT;


	StartSize=OutputLength;				// start for the size stuff

	wi(walker+OutputLength)=D_DEFPOINTS;		// type 1 data...
	OutputLength+=4;
	wi(walker+OutputLength)=0x01234567;
	SizeAt=OutputLength;					// to fix the size at the end...
	OutputLength+=4;
	wi(walker+OutputLength)=n_verts;
	OutputLength+=4;
	wi(walker+OutputLength)=n_norms;
	OutputLength+=4;

	if((n_verts&3)==0)
		wi(walker+OutputLength)=n_verts+20;				//offset
	else
		wi(walker+OutputLength)=((n_verts+4)&~3)+20;		 //offset
	OutputLength+=4;


	for(i=0;i<FSModel.Vcount;i++)
	{
		if(FSModel.Vpntseg[i]==Segment)
		{
			wc(walker+OutputLength)=FSModel.n_vn[i];
			OutputLength+=1;
		}
	}
	if((n_verts&3)!=0)
	{
		for(i=(n_verts&3);i<4;i++)
		{
			wc(walker+OutputLength)=FSModel.n_vn[i];
			OutputLength+=1;
		}
	}

	FSModel.NScount=0;
	FSModel.VScount=0;
	for(i=0;i<FSModel.Vcount;i++)
	{
		if(FSModel.Vpntseg[i]==Segment)
		{
			v(walker+OutputLength)=FSModel.Vpoint[i];
			OutputLength+=12;
			FSModel.VSobj[i]=FSModel.VScount++;
			k=0;
			for(j=0;j<i;j++)
			{
				k+=FSModel.n_vn[j];
			}
			for(j=k;j<k+FSModel.n_vn[i];j++)
			{
				v(walker+OutputLength)=FSModel.Npoint[j];
				OutputLength+=12; 
				FSModel.NSobj[j]=FSModel.NScount++;
			}
		}
	}
	wi(walker+SizeAt)=OutputLength-StartSize;
}


//
// SOBJ type 2&3 data
//
void CImportCOB::Dump23Data(int Polygon)
{
	unsigned int i,j;
	unsigned int SizeAt,StartSize;
	unsigned char *walker=OutputData;

	i=Polygon;

	StartSize=OutputLength;				// start for the size stuff

	if(FSModel.Poly[i].Ptype==2)
	{
		wi(walker+OutputLength)=D_FLATPOLY;
		//LOGTEXT("  doing 2: Polygon=%d\n",Polygon); LOGDOIT;
	}
	else
	{
		wi(walker+OutputLength)=D_TMAPPOLY;
		//LOGTEXT("  doing 3: Polygon=%d\n",Polygon); LOGDOIT;
	}
	OutputLength+=4;

	wi(walker+OutputLength)=0x01234567;
	SizeAt=OutputLength;					// to fix the size at the end...
	OutputLength+=4;

	v(walker+OutputLength)=FSModel.Poly[i].Normal;
	OutputLength+=12;
	v(walker+OutputLength)=FSModel.Poly[i].Center;
	OutputLength+=12;

	wf(walker+OutputLength)=FSModel.Poly[i].Radius;
	OutputLength+=4;

	wi(walker+OutputLength)=FSModel.Poly[i].Corners;
	OutputLength+=4;
	wi(walker+OutputLength)=FSModel.Poly[i].Colors;
	OutputLength+=4;

	//FSModel.VSobj[i]=FSModel.VScount++;
	for(j=0;j<FSModel.Poly[i].Corners;j++)
	{
		ws(walker+OutputLength)=FSModel.VSobj[FSModel.Poly[i].Vp[j]];
		OutputLength+=2;
		ws(walker+OutputLength)=FSModel.NSobj[FSModel.Poly[i].Np[j]];
		OutputLength+=2;
		if(FSModel.Poly[i].Ptype==3)
		{
			wf(walker+OutputLength)=FSModel.Poly[i].U[j];
			OutputLength+=4;
			wf(walker+OutputLength)=FSModel.Poly[i].V[j];
			OutputLength+=4;
		}
	}
	wi(walker+SizeAt)=OutputLength-StartSize;
}

//
// SOBJ type 4 data
//
void CImportCOB::Dump4Data(FS_VPNT Normal, FS_VPNT Point, FS_VPNT Min, FS_VPNT Max)
{
	unsigned int SizeAt,StartSize;
	unsigned char *walker=OutputData;

	//LOGTEXT("  doing 4: splitting\n"); LOGDOIT;

	StartSize=OutputLength;				// start for the size stuff

	wi(walker+OutputLength)=D_SORTNORM;
	OutputLength+=4;

	wi(walker+OutputLength)=0x01234567;
	SizeAt=OutputLength;					// to fix the size at the end...
	OutputLength+=4;

	v(walker+OutputLength)=Normal;
	OutputLength+=12;
	v(walker+OutputLength)=Point;
	OutputLength+=12;

	wi(walker+OutputLength)=0;	// reserved=0
	OutputLength+=4;
	wi(walker+OutputLength)=0;	// front
	OutputLength+=4;
	wi(walker+OutputLength)=0;	// back
	OutputLength+=4;
	wi(walker+OutputLength)=0;	// prelist
	OutputLength+=4;
	wi(walker+OutputLength)=0;	// postlist
	OutputLength+=4;
	wi(walker+OutputLength)=0;	// online
	OutputLength+=4;

	v(walker+OutputLength)=Min;
	OutputLength+=12;
	v(walker+OutputLength)=Max;
	OutputLength+=12;

	wi(walker+SizeAt)=OutputLength-StartSize;
}

//
// SOBJ type 5 data
//
void CImportCOB::Dump5Data(FS_VPNT Min, FS_VPNT Max)
{
	unsigned int SizeAt,StartSize;
	unsigned char *walker=OutputData;

	//LOGTEXT("  doing 5: BoundBox\n"); LOGDOIT;
	Max.x+=DELTA;
	Max.y+=DELTA;
	Max.z+=DELTA;
	Min.x-=DELTA;
	Min.y-=DELTA;
	Min.z-=DELTA;

	StartSize=OutputLength;				// start for the size stuff

	wi(walker+OutputLength)=D_BOUNDBOX;
	OutputLength+=4;

	wi(walker+OutputLength)=0x01234567;
	SizeAt=OutputLength;					// to fix the size at the end...
	OutputLength+=4;

	v(walker+OutputLength)=Min;
	OutputLength+=12;
	v(walker+OutputLength)=Max;
	OutputLength+=12;

	wi(walker+SizeAt)=OutputLength-StartSize;
}

//
// split
//
void CImportCOB::split(FS_VPNT Min, FS_VPNT Max,unsigned int SobjNum)
{
	unsigned int i,j,total,total1,total2,Polygon1,Polygon2;
	FS_VPNT Max1,Min1,Max2,Min2,Max3,Min3,Max4,Min4;
	FS_VPNT Normal,Point;
	float testx,testy,testz;
	unsigned int Type4At,Type5At,Front,Back,Prelist,Postlist,Online;
	unsigned char *walker=OutputData;


	Min1=Min;
	Max1=Max;
	Min2=Min;
	Max2=Max;
	Min3=Min;
	Max3=Max;
	Min4=Min;
	Max4=Max;
	testx=Max.x-Min.x;
	testy=Max.y-Min.y;
	testz=Max.z-Min.z;
	Point.x=testx/2+Min.x;
	Point.y=testy/2+Min.y;
	Point.z=testz/2+Min.z;
	Normal.x=0;
	Normal.y=0;
	Normal.z=0;
	if((testx >= testy) & (testx >= testz))
	{
		Normal.x=1;
		Min1.x=Point.x;
		Max2.x=Point.x;
	} else {
		if(testy >= testz)
		{
			Normal.y=1;
			Min1.y=Point.y;
			Max2.y=Point.y;
		} else {
			Normal.z=1;
			Min1.z=Point.z;
			Max2.z=Point.z;
		}
	}

	total1=0;
	i=0;
	while((i<FSModel.Pcount) & (total1<2))
	{
		if(FSModel.Poly[i].Segment==SobjNum)
			if((FSModel.Poly[i].Center.x >= Min1.x) & (FSModel.Poly[i].Center.x<Max1.x))
				if((FSModel.Poly[i].Center.y >= Min1.y) & (FSModel.Poly[i].Center.y<Max1.y))
					if((FSModel.Poly[i].Center.z >= Min1.z) & (FSModel.Poly[i].Center.z<Max1.z))
					{
						total1++;
						Polygon1=i;
					}
		i++;
	}
	total2=0;
	i=0;
	while((i<FSModel.Pcount) & (total2<2))
	{
		if(FSModel.Poly[i].Segment==SobjNum)
			if((FSModel.Poly[i].Center.x >= Min2.x) & (FSModel.Poly[i].Center.x<Max2.x))
				if((FSModel.Poly[i].Center.y >= Min2.y) & (FSModel.Poly[i].Center.y<Max2.y))
					if((FSModel.Poly[i].Center.z >= Min2.z) & (FSModel.Poly[i].Center.z<Max2.z))
					{
						total2++;
						Polygon2=i;
					}
		i++;
	}


		total=0;
		for(i=0;i<FSModel.Pcount;i++)
		{
			if(FSModel.Poly[i].Segment==SobjNum)
				if((FSModel.Poly[i].Center.x >= Min.x) & (FSModel.Poly[i].Center.x<Max.x))
					if((FSModel.Poly[i].Center.y >= Min.y) & (FSModel.Poly[i].Center.y<Max.y))
						if((FSModel.Poly[i].Center.z >= Min.z) & (FSModel.Poly[i].Center.z<Max.z))
						{
							total++;
							if(total==1)
							{
								Max4.x=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].x;
								Max4.y=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].y;
								Max4.z=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].z;
								Min4.x=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].x;
								Min4.y=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].y;
								Min4.z=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].z;
							}
							for(j=0;j<FSModel.Poly[i].Corners;j++)
							{
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x>Max4.x)
										Max4.x=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x;
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y>Max4.y)
										Max4.y=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y;
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z>Max4.z)
										Max4.z=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z;
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x<Min4.x)
										Min4.x=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x;
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y<Min4.y)
										Min4.y=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y;
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z<Min4.z)
										Min4.z=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z;
							}
						}
		}


	if((total1>0) & (total2>0))
	{
		Type4At=OutputLength;
		Dump4Data(Normal, Point, Min4, Max4);
		Dump0Data();
		Front=OutputLength;
		if(total1>1)
			split(Min1, Max1,SobjNum);
		else {
			Max3.x=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[0]].x;
			Max3.y=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[0]].y;
			Max3.z=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[0]].z;
			Min3.x=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[0]].x;
			Min3.y=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[0]].y;
			Min3.z=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[0]].z;
			for(i=1;i<FSModel.Poly[Polygon1].Corners;i++)
			{
				if(FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].x>Max3.x)
					Max3.x=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].x;
				if(FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].y>Max3.y)
					Max3.y=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].y;
				if(FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].z>Max3.z)
					Max3.z=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].z;
				if(FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].x<Min3.x)
					Min3.x=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].x;
				if(FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].y<Min3.y)
					Min3.y=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].y;
				if(FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].z<Min3.z)
					Min3.z=FSModel.Vpoint[FSModel.Poly[Polygon1].Vp[i]].z;
			}
			Dump5Data(Min3,Max3);
			Dump23Data(Polygon1);
			Dump0Data();
		}
		wi(walker+Type4At+36)=Front-Type4At;	 // Front

		Back=OutputLength;
		if(total2>1)
			split(Min2, Max2,SobjNum);
		else {
			Max3.x=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[0]].x;
			Max3.y=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[0]].y;
			Max3.z=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[0]].z;
			Min3.x=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[0]].x;
			Min3.y=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[0]].y;
			Min3.z=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[0]].z;
			for(i=1;i<FSModel.Poly[Polygon2].Corners;i++)
			{
				if(FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].x>Max3.x)
					Max3.x=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].x;
				if(FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].y>Max3.y)
					Max3.y=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].y;
				if(FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].z>Max3.z)
					Max3.z=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].z;
				if(FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].x<Min3.x)
					Min3.x=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].x;
				if(FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].y<Min3.y)
					Min3.y=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].y;
				if(FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].z<Min3.z)
					Min3.z=FSModel.Vpoint[FSModel.Poly[Polygon2].Vp[i]].z;
			}
			Dump5Data(Min3,Max3);
			Dump23Data(Polygon2);
			Dump0Data();
		}
		wi(walker+Type4At+40)=Back-Type4At;	 // Back

		Prelist=OutputLength;
		Dump0Data();
		wi(walker+Type4At+44)=Prelist-Type4At;	// Prelist

		Postlist=OutputLength;
		Dump0Data();
		wi(walker+Type4At+48)=Postlist-Type4At;	 // Postlist

		Online=OutputLength;
		Dump0Data();
		wi(walker+Type4At+52)=Online-Type4At;		 // Online
	} else {
		Type5At=OutputLength;				//this block dumps all polys into 1
		Dump5Data(Min3,Max3);				//boundbox....
		total=0;
		for(i=0;i<FSModel.Pcount;i++)
		{
			if(FSModel.Poly[i].Segment==SobjNum)
				if((FSModel.Poly[i].Center.x >= Min.x) & (FSModel.Poly[i].Center.x<Max.x))
					if((FSModel.Poly[i].Center.y >= Min.y) & (FSModel.Poly[i].Center.y<Max.y))
						if((FSModel.Poly[i].Center.z >= Min.z) & (FSModel.Poly[i].Center.z<Max.z))
						{
							total++;
							if(total==1)
							{
								Max3.x=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].x;
								Max3.y=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].y;
								Max3.z=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].z;
								Min3.x=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].x;
								Min3.y=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].y;
								Min3.z=FSModel.Vpoint[FSModel.Poly[i].Vp[0]].z;
							}
							for(j=0;j<FSModel.Poly[i].Corners;j++)
							{
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x>Max3.x)
										Max3.x=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x;
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y>Max3.y)
										Max3.y=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y;
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z>Max3.z)
										Max3.z=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z;
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x<Min3.x)
										Min3.x=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].x;
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y<Min3.y)
										Min3.y=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].y;
								if(FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z<Min3.z)
										Min3.z=FSModel.Vpoint[FSModel.Poly[i].Vp[j]].z;
							}
							Dump23Data(i);
						}
		}
		v(walker+Type5At+8)=Min3;		 // min
		v(walker+Type5At+20)=Max3;	// min
		Dump0Data();
	}
}


//**********************************/ 


//
// SOBJ Setup
//
void CImportCOB::sobj(int SobjNum)
{
	ASSERT(SobjNum < MAX_FS_SOBJ);
	unsigned int i;
	FS_VPNT test1,test2;
	//FS_VPNT Offset,Center;
	FS_VPNT Max,Min;
	//unsigned int SizeAt,StartSize,SizeAt2,StartSize2;
	unsigned char *walker=OutputData;
	float minx,miny,minz,maxx,maxy,maxz,Radius,Radius2;
	double maxxyz;

	if (SobjNum < MAX_FS_SOBJ) {
		LOGTEXT(" Building chunk SOBJ #%i - ",SobjNum); LOGDOIT;
		i=0;
		while((i<FSModel.Vcount) & (FSModel.Vpntseg[i]!=SobjNum))
			i++;

		//Find bounding box
		maxx=FSModel.Vpoint[i].x;
		maxy=FSModel.Vpoint[i].y;	
		maxz=FSModel.Vpoint[i].z;
		minx=FSModel.Vpoint[i].x;
		miny=FSModel.Vpoint[i].y;
		minz=FSModel.Vpoint[i].z;
		maxxyz=((FSModel.Vpoint[i].x*FSModel.Vpoint[i].x)+
				(FSModel.Vpoint[i].y*FSModel.Vpoint[i].y)+
				(FSModel.Vpoint[i].z*FSModel.Vpoint[i].z));
		Radius=(float)sqrt(maxxyz);
		for(i=0;i<FSModel.Vcount;i++)
		{
			if(FSModel.Vpntseg[i]==SobjNum)
			{
				if(FSModel.Vpoint[i].x>maxx)	 maxx=FSModel.Vpoint[i].x;
				if(FSModel.Vpoint[i].y>maxy)	 maxy=FSModel.Vpoint[i].y;
				if(FSModel.Vpoint[i].z>maxz)	 maxz=FSModel.Vpoint[i].z;
				if(FSModel.Vpoint[i].x<minx)	 minx=FSModel.Vpoint[i].x;
				if(FSModel.Vpoint[i].y<miny)	 miny=FSModel.Vpoint[i].y;
				if(FSModel.Vpoint[i].z<minz)	 minz=FSModel.Vpoint[i].z;
				maxxyz=((FSModel.Vpoint[i].x*FSModel.Vpoint[i].x)+
						(FSModel.Vpoint[i].y*FSModel.Vpoint[i].y)+
						(FSModel.Vpoint[i].z*FSModel.Vpoint[i].z));	
				Radius2=(float)sqrt(maxxyz);
				if(Radius<Radius2) Radius=Radius2;
			}
		}
		Max.x=maxx;
		Max.y=maxy;
		Max.z=maxz;
		Min.x=minx;
		Min.y=miny;
		Min.z=minz;

//if (SobjNum==0)
//	{
//Center.x=0;
//Center.y=0;
//Center.z=0;
//} else {
//Center.x=FSModel.Soffset[SobjNum].x;
//Center.y=FSModel.Soffset[SobjNum].y;
//Center.z=FSModel.Soffset[SobjNum].z;
//}



//Center.x=maxx-(maxx-minx)/2;
//Center.y=maxy-(maxy-miny)/2;
//Center.z=maxz-(maxz-minz)/2;
//
//maxx=Center.x-FSModel.Vpoint[0].x;
//maxy=Center.y-FSModel.Vpoint[0].y;
//maxz=Center.z-FSModel.Vpoint[0].z;
//maxxyz=((maxx*maxx)+(maxy*maxy)+(maxz*maxz));
//Radius=sqrt(maxxyz);
//
//for (i=1;i<FSModel.Vcount;i++)
//	{
//maxx=Center.x-FSModel.Vpoint[i].x;
//maxy=Center.y-FSModel.Vpoint[i].y;
//maxz=Center.z-FSModel.Vpoint[i].z;
//maxxyz=((maxx*maxx)+(maxy*maxy)+(maxz*maxz));
//Radius2=sqrt(maxxyz);
//if (Radius<Radius2) Radius=Radius2;
//}

	//Save to doc
		if(FSModel.shield!=0)
		{
			if(FSModel.shield<(long)SobjNum)
				GetDocument()->m_FS_SOBJ[SobjNum].submodel_number=SobjNum-1;
			else
				GetDocument()->m_FS_SOBJ[SobjNum].submodel_number=SobjNum;
		} else
			GetDocument()->m_FS_SOBJ[SobjNum].submodel_number=SobjNum;		// sobj 0
	
		if(FSModel.shield!=0)
		{
			if(FSModel.shield<FSModel.Sparent[SobjNum])
				GetDocument()->m_FS_SOBJ[SobjNum].submodel_parent=FSModel.Sparent[SobjNum]-1;
			else
				GetDocument()->m_FS_SOBJ[SobjNum].submodel_parent=FSModel.Sparent[SobjNum];
		} else
			GetDocument()->m_FS_SOBJ[SobjNum].submodel_parent=FSModel.Sparent[SobjNum];		 // parent
	
		if(SobjNum==0)
		{
			GetDocument()->m_FS_SOBJ[SobjNum].real_offset.x=0;
			GetDocument()->m_FS_SOBJ[SobjNum].real_offset.y=0;
			GetDocument()->m_FS_SOBJ[SobjNum].real_offset.z=0;
		} else {										 // 7-30-99 didn't subtract the parent... ouch
			GetDocument()->m_FS_SOBJ[SobjNum].real_offset.x=FSModel.Soffset[SobjNum].x-FSModel.Soffset[FSModel.Sparent[SobjNum]].x;
			GetDocument()->m_FS_SOBJ[SobjNum].real_offset.y=FSModel.Soffset[SobjNum].y-FSModel.Soffset[FSModel.Sparent[SobjNum]].y;
			GetDocument()->m_FS_SOBJ[SobjNum].real_offset.z=FSModel.Soffset[SobjNum].z-FSModel.Soffset[FSModel.Sparent[SobjNum]].z;
		}
		if(GetDocument()->m_FS_SOBJ[SobjNum].submodel_parent!=-1)
		{
			GetDocument()->m_FS_SOBJ[SobjNum].offset.x+=GetDocument()->m_FS_SOBJ[GetDocument()->m_FS_SOBJ[SobjNum].submodel_parent].offset.x; 
			GetDocument()->m_FS_SOBJ[SobjNum].offset.y+=GetDocument()->m_FS_SOBJ[GetDocument()->m_FS_SOBJ[SobjNum].submodel_parent].offset.y;
			GetDocument()->m_FS_SOBJ[SobjNum].offset.z+=GetDocument()->m_FS_SOBJ[GetDocument()->m_FS_SOBJ[SobjNum].submodel_parent].offset.z;
		}
		else
		{
			GetDocument()->m_FS_SOBJ[SobjNum].offset.x=0;
			GetDocument()->m_FS_SOBJ[SobjNum].offset.y=0;
			GetDocument()->m_FS_SOBJ[SobjNum].offset.z=0;
		}
		GetDocument()->m_FS_SOBJ[SobjNum].radius=Radius;
		GetDocument()->m_FS_SOBJ[SobjNum].geometric_center.x=0;
		GetDocument()->m_FS_SOBJ[SobjNum].geometric_center.y=0;
		GetDocument()->m_FS_SOBJ[SobjNum].geometric_center.z=0;
		GetDocument()->m_FS_SOBJ[SobjNum].bounding_box_min_point.x=Min.x;
		GetDocument()->m_FS_SOBJ[SobjNum].bounding_box_min_point.y=Min.y;
		GetDocument()->m_FS_SOBJ[SobjNum].bounding_box_min_point.z=Min.z;
		GetDocument()->m_FS_SOBJ[SobjNum].bounding_box_max_point.x=Max.x;
		GetDocument()->m_FS_SOBJ[SobjNum].bounding_box_max_point.y=Max.y;
		GetDocument()->m_FS_SOBJ[SobjNum].bounding_box_max_point.z=Max.z;

		GetDocument()->m_FS_SOBJ[SobjNum].submodel_name=FSModel.Sname[SobjNum];
		GetDocument()->m_FS_SOBJ[SobjNum].properties="";
		GetDocument()->m_FS_SOBJ[SobjNum].movement_type=-1;
		GetDocument()->m_FS_SOBJ[SobjNum].movement_axis=-1;
		GetDocument()->m_FS_SOBJ[SobjNum].reserved=0;

	//Create chunk
	/*
	wi(walker+OutputLength)=ID_SOBJ;
	OutputLength+=4;

	wi(walker+OutputLength)=0x01234567;
	SizeAt=OutputLength;					// to fix the size at the end...
	OutputLength+=4;
	StartSize=OutputLength;				// start for the size stuff


	if(FSModel.shield!=0)
	{
		if(FSModel.shield<(long)SobjNum)
			wi(walker+OutputLength)=SobjNum-1;
		else
			wi(walker+OutputLength)=SobjNum;
	} else
		wi(walker+OutputLength)=SobjNum;		// sobj 0
	OutputLength+=4;

	if(FSModel.shield!=0)
	{
		if(FSModel.shield<FSModel.Sparent[SobjNum])
		{
			wi(walker+OutputLength)=FSModel.Sparent[SobjNum]-1;
		} else {
			wi(walker+OutputLength)=FSModel.Sparent[SobjNum];
		}
	} else {
		wi(walker+OutputLength)=FSModel.Sparent[SobjNum];		 // parent
	}
	OutputLength+=4;

	if(SobjNum==0)
	{
		Offset.x=0;
		Offset.y=0;
		Offset.z=0;
	} else {										 // 7-30-99 didn't subtract the parent... ouch
		Offset.x=FSModel.Soffset[SobjNum].x-FSModel.Soffset[FSModel.Sparent[SobjNum]].x;
		Offset.y=FSModel.Soffset[SobjNum].y-FSModel.Soffset[FSModel.Sparent[SobjNum]].y;
		Offset.z=FSModel.Soffset[SobjNum].z-FSModel.Soffset[FSModel.Sparent[SobjNum]].z;
	}

	v(walker+OutputLength)=Offset;
	OutputLength+=12;

	wf(walker+OutputLength)=Radius;	 // radius
	OutputLength+=4;

	Center.x=0;
	Center.y=0;
	Center.z=0;
	v(walker+OutputLength)=Center;		// center
	OutputLength+=12;
	v(walker+OutputLength)=Min;		 // min
	OutputLength+=12;
	v(walker+OutputLength)=Max;		 // max
	OutputLength+=12;

	wi(walker+OutputLength)=strlen(FSModel.Sname[SobjNum]);	 // reserved=0
	OutputLength+=4;
	for(i=0;i<strlen(FSModel.Sname[SobjNum]);i++)
	{
		wc(walker+OutputLength)=FSModel.Sname[SobjNum][i];// name
		OutputLength+=1;
	}


	wi(walker+OutputLength)=4;	// reserved=0
	OutputLength+=4;
	wi(walker+OutputLength)=0;	// reserved=0
	OutputLength+=4;

	wi(walker+OutputLength)=-1;	 // reserved=0
	OutputLength+=4;
	wi(walker+OutputLength)=-1;	 // reserved=0
	OutputLength+=4;
	wi(walker+OutputLength)=0;	// reserved=0
	OutputLength+=4;

	wi(walker+OutputLength)=0x01234567;
	SizeAt2=OutputLength;					 // to fix the size at the end...
	OutputLength+=4;

	StartSize2=OutputLength;				 // start for the size stuff*/

		OutputLength=0;

		test1.x=1;
		test1.y=1;
		test1.z=1;
		test2.x=2;
		test2.y=2;
		test2.z=2;

		Dump1Data(SobjNum);

		Max.x+=DELTA;
		Max.y+=DELTA;
		Max.z+=DELTA;
		Min.x-=DELTA;
		Min.y-=DELTA;
		Min.z-=DELTA;

		split(Min,Max,SobjNum);
		Dump0Data();

		GetDocument()->m_FS_SOBJ[SobjNum].bsp_data_size=OutputLength;
		GetDocument()->m_FS_SOBJ[SobjNum].bsp_data=new unsigned char[GetDocument()->m_FS_SOBJ[SobjNum].bsp_data_size];		 //model data size
		memcpy(GetDocument()->m_FS_SOBJ[SobjNum].bsp_data,OutputData,OutputLength);

		VERIFY(GetDocument()->FS_Walk(GetDocument()->m_FS_SOBJ[SobjNum].bsp_data,SobjNum,GetDocument()->m_FS_Model.Vcount)==ERROR_GEN_NOERROR);
		ASSERT(GetDocument()->m_FS_NumSOBJ<SobjNum+1);
		GetDocument()->m_FS_NumSOBJ=SobjNum+1;

/*CFile f;
CString x1; x1.Format("c:\\import%i",SobjNum);
f.Open(x1,CFile::modeWrite | CFile::modeCreate);
f.Write(GetDocument()->m_FS_SOBJ[SobjNum].bsp_data,GetDocument()->m_FS_SOBJ[SobjNum].bsp_data_size);
f.Close();*/

	/*wi(walker+SizeAt)=OutputLength-StartSize;
	wi(walker+SizeAt2)=OutputLength-StartSize2;*/
	}
}



//*************************************************************
// SHLD Routines*
//*************************************************************
void CImportCOB::shld(void)
{
	unsigned int i,j,k; 
	unsigned char *walker=OutputData;
	unsigned int n_verts,n_faces;
	SINFO Sinfo;
	short back[3]={2,0,1};

	if(FSModel.shield!=0)
	{
		LOGTEXT(" Building chunk SHLD - Using segment %d, ",FSModel.shield); LOGDOIT;

		FSModel.VScount=0;
		Sinfo.VScount=0;
		n_verts=0;
		for(i=0;i<FSModel.Vcount;i++)
		{
			if(FSModel.Vpntseg[i]==FSModel.shield)
			{
				n_verts++;
				FSModel.VSobj[i]=FSModel.VScount++;
				Sinfo.VSlist[Sinfo.VScount]=i;
				Sinfo.VScount++;
			}
		}
		LOGTEXT("n_verts=%d, ",n_verts); LOGDOIT;

		Sinfo.PScount=0;
		n_faces=0;
		for(i=0;i<FSModel.Pcount;i++)
		{
			if(FSModel.Poly[i].Segment==FSModel.shield)
			{
				n_faces++;
				Sinfo.PSlist[Sinfo.PScount]=i;
				Sinfo.PScount++;
				if(Sinfo.PScount>SHIELD_MAX)
				{
					LOGTEXT(" Shield polys>SHIELD_MAX \n"); LOGDOIT;
					exit(0);
				}
				if(FSModel.Poly[i].Corners!=3)
				{
					LOGTEXT(" Shield error: corners!=3 \n"); LOGDOIT;
					exit(0);
				}
			}
		}
		LOGTEXT(" n_faces=%d\n",n_faces); LOGDOIT;

		for(i=0;i<Sinfo.PScount;i++)
		{
			for(j=1;j<Sinfo.PScount;j++)
			{
				for(k=0;k<3;k++)
				{
					if(FSModel.Poly[Sinfo.PSlist[i]].Vp[0]==FSModel.Poly[Sinfo.PSlist[j]].Vp[k])
					{
						if(FSModel.Poly[Sinfo.PSlist[i]].Vp[1]==FSModel.Poly[Sinfo.PSlist[j]].Vp[back[k]])
							Sinfo.PSnext[i][0]=j;
					}
					if(FSModel.Poly[Sinfo.PSlist[i]].Vp[1]==FSModel.Poly[Sinfo.PSlist[j]].Vp[k])
					{
						if(FSModel.Poly[Sinfo.PSlist[i]].Vp[2]==FSModel.Poly[Sinfo.PSlist[j]].Vp[back[k]])
							Sinfo.PSnext[i][1]=j;
					}
					if(FSModel.Poly[Sinfo.PSlist[i]].Vp[2]==FSModel.Poly[Sinfo.PSlist[j]].Vp[k])
					{
						if(FSModel.Poly[Sinfo.PSlist[i]].Vp[0]==FSModel.Poly[Sinfo.PSlist[j]].Vp[back[k]])
							Sinfo.PSnext[i][2]=j;
					}
				}
			}
		}

		//Save to doc
		GetDocument()->m_FS_Model.shields.Vcount=n_verts;
		for(i=0;i<FSModel.Vcount;i++)
		{
			GetDocument()->m_FS_Model.shields.Vpoint[i][0]=FSModel.Vpoint[i].x;
			GetDocument()->m_FS_Model.shields.Vpoint[i][1]=FSModel.Vpoint[i].y;
			GetDocument()->m_FS_Model.shields.Vpoint[i][2]=FSModel.Vpoint[i].z;
		}
		GetDocument()->m_FS_Model.shields.Fcount=n_faces;
		for(i=0;i<Sinfo.PScount;i++)
		{
			GetDocument()->m_FS_Model.shields.Face[i].Normal=FSModel.Poly[Sinfo.PSlist[i]].Normal;
			if (FSModel.Poly[Sinfo.PSlist[i]].Vp[0] >= VEC_MAX || FSModel.Poly[Sinfo.PSlist[i]].Vp[1] >= VEC_MAX || FSModel.Poly[Sinfo.PSlist[i]].Vp[2] >= VEC_MAX) return;
			GetDocument()->m_FS_Model.shields.Face[i].Vface[0]=FSModel.VSobj[FSModel.Poly[Sinfo.PSlist[i]].Vp[0]];
			GetDocument()->m_FS_Model.shields.Face[i].Vface[1]=FSModel.VSobj[FSModel.Poly[Sinfo.PSlist[i]].Vp[1]];
			GetDocument()->m_FS_Model.shields.Face[i].Vface[2]=FSModel.VSobj[FSModel.Poly[Sinfo.PSlist[i]].Vp[2]];
			GetDocument()->m_FS_Model.shields.Face[i].Nface[0]=Sinfo.PSnext[i][0];
			GetDocument()->m_FS_Model.shields.Face[i].Nface[1]=Sinfo.PSnext[i][1];
			GetDocument()->m_FS_Model.shields.Face[i].Nface[2]=Sinfo.PSnext[i][2];
		}

		//Create Chunk
		/*wi(walker+OutputLength)=ID_SHLD;
		OutputLength+=4;

		unsigned int SizeAt,StartSize;

		wi(walker+OutputLength)=0x01234567;
		SizeAt=OutputLength;					// to fix the size at the end...
		OutputLength+=4;
		StartSize=OutputLength;				// start for the size stuff

		wi(walker+OutputLength)=n_verts;
		OutputLength+=4;

		for(i=0;i<FSModel.Vcount;i++)
		{
			if(FSModel.Vpntseg[i]==FSModel.shield)
			{
				v(walker+OutputLength)=FSModel.Vpoint[i];
				OutputLength+=12;
			}
		}

		wi(walker+OutputLength)=n_faces;
		OutputLength+=4;

		for(i=0;i<Sinfo.PScount;i++)
		{
			v(walker+OutputLength)=FSModel.Poly[Sinfo.PSlist[i]].Normal;
			OutputLength+=12;
			wi(walker+OutputLength)=FSModel.VSobj[FSModel.Poly[Sinfo.PSlist[i]].Vp[0]];
			OutputLength+=4;
			wi(walker+OutputLength)=FSModel.VSobj[FSModel.Poly[Sinfo.PSlist[i]].Vp[1]];
			OutputLength+=4;
			wi(walker+OutputLength)=FSModel.VSobj[FSModel.Poly[Sinfo.PSlist[i]].Vp[2]];
			OutputLength+=4;
			wi(walker+OutputLength)=Sinfo.PSnext[i][0];
			OutputLength+=4;
			wi(walker+OutputLength)=Sinfo.PSnext[i][1];
			OutputLength+=4;
			wi(walker+OutputLength)=Sinfo.PSnext[i][2];
			OutputLength+=4;
		}

		wi(walker+SizeAt)=OutputLength-StartSize;
		*/
	}
}


//*************************************************************
// PINF Routines*
//*************************************************************
void CImportCOB::pinf(void)
{
	unsigned int SizeAt,StartSize;
	unsigned char *walker=OutputData;

	LOGTEXT(" Building chunk PINF\n"); LOGDOIT;
	wi(walker+OutputLength)=ID_PINF;
	OutputLength+=4;

	wi(walker+OutputLength)=0x01234567;
	SizeAt=OutputLength;					// to fix the size at the end...
	OutputLength+=4;
	StartSize=OutputLength;				// start for the size stuff

	CString Version;
	Version.Format("MODELVIEW32 COB2FS engine-Based on %s by Garry Knudson",COB2FS_VERSION);

	for(int i=0;i<Version.GetLength();i++)
	{
		wc(walker+OutputLength)=Version.GetAt(i);
		OutputLength+=1;
	}

	wi(walker+SizeAt)=OutputLength-StartSize;
}




//*************************************************************
// MakePof*
//*************************************************************
void CImportCOB::MakePof(/*char *output*/)
{
	unsigned int i;

	LOGTEXT("\n"); LOGDOIT;
	LOGTEXT("Building POF file chunks ->\n"); LOGDOIT;

	LOGTEXT(" Statistics:\n"); LOGDOIT;
	LOGTEXT("  Vcount=%d\n",FSModel.Vcount); LOGDOIT;
	LOGTEXT("  Pcount=%d\n",FSModel.Pcount); LOGDOIT;
	LOGTEXT("  Scount=%d\n",FSModel.Scount); LOGDOIT;
	LOGTEXT("  chunknum=%d\n",chunknum); LOGDOIT;

	OutputLength=0;

	if((OutputData=(unsigned char *) malloc(768000))==NULL)
	{	 //256000
		LOGTEXT(" Not enough memory to allocate buffer\n"); LOGDOIT;
		exit(1);// terminate program if out of memory
	}

	//Write POF
	/*
	FILE *PMfile;
	unsigned char *walker;
	
	walker=OutputData;
	wi(walker+OutputLength)=ID_PSPO;		// File Id
	OutputLength+=4;
	wi(walker+OutputLength)=2014;			// Version
	OutputLength+=4;
	txtr();
	ohdr();
	for(i=0;i<FSModel.Scount;i++)
	{
		if(FSModel.shield==0)
			sobj(i);
		else
		{
			if(FSModel.shield!=i)
				sobj(i);
		}
	}
	shld();
	pinf();

	PMfile=fopen(output,"wb");
	fwrite(OutputData, OutputLength, 1, PMfile);
	fclose(PMfile);*/

	//Chunks
	txtr();
	ohdr();
	shld();

	//Internal SOBJ
	for(i=0;i<FSModel.Scount;i++)
	{
		if(FSModel.shield==0)
			sobj(i);
		else
		{
			if(FSModel.shield!=i)
				sobj(i);
		}
	}

	//Internal PINF
	OutputLength=0;
	pinf();
	if(OutputLength==0)
		GetDocument()->m_EditorFS_ChunkPINF.CreateChunk(OutputData,FALSE);
	else
		GetDocument()->m_EditorFS_ChunkPINF.ClearChunk();
	
	//Tell the viewer about the new model
	CMODVIEW32App *m_App=(CMODVIEW32App *)AfxGetApp();
	CMainFrame *viewFrame=static_cast<CMainFrame*>(m_App->m_pMainWnd);
	GetDocument()->FS_PreparePOF();
	viewFrame->GameBarFS();
	GetDocument()->SetPathName("Untitled");
	GetDocument()->m_CurFile="";
	ASSERT(m_Game==1 || m_Game==2);
	if(m_Game==1)
	{
		GetDocument()->m_FS_CurrVP_FreeSpaceVersion=1;
		GetDocument()->m_FileVersion=2016;
	}
	else
	{
		GetDocument()->m_FS_CurrVP_FreeSpaceVersion=2;
		GetDocument()->m_FileVersion=2117;
	}

	GetDocument()->FS_LoadTextureData2();
	GetDocument()->m_UntitledModel=TRUE;
	GetDocument()->SetModifiedFlag(TRUE); //Document has been changed
	GetDocument()->m_CurrentFile_Offset=0;

	viewFrame->LoadEditor();

	//GetDocument()->m_ModelLoaded=TRUE;
	//GetDocument()->m_Game=GAME_FS;

	free(OutputData);
}



/*void CImportCOB::LOGTEXT2(char *fmt, ...)
{
	CString x;

	va_list argptr;
	va_start(argptr, fmt);
	x.Format(fmt, argptr);
	va_end(argptr);

	OutputDebugString(x);
}*/

