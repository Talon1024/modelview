#include "stdafx.h"
#include "direct.h"
#include "errno.h"
#include "stif32c.h"


//----
//StIf for C++
//Written by Heiko Herrmann alias HH-Software Images

//Copies a file. Complete path statements are needed
//for both source and destination.
//RETURNS: 0 if success
//        -1 if source could not be found/opened
//        -2 if destination could not be opened
int filecopy(char *source, char *target)
{
	FILE *file1,*file2;
	int filesize;

	file1=fopen(source,"rb");
	if(file1==NULL)
		return -1;

	file2=fopen(target,"wb");
	if(file2==NULL)
	{
		fclose(file1);
		return -2;
	}
	
	fseek(file1,0,SEEK_END);
	filesize=ftell(file1);
	fseek(file1,0,SEEK_SET);

	int *buffer=(int *)malloc(filesize);
	
	fread(buffer,1,filesize,file1);
	fwrite(buffer,1,filesize,file2);

	free(buffer);
	fclose(file2);
	fclose(file1);

	return 0;
}

//Copies a file from one dir to another.
//RETURNS: (see values in function filecopy)
int filecopy2(char *source, char *target, char *filename)
{
	char s[8192];
	char t[8192];

	buildfilename(s,source,filename);
	buildfilename(t,target,filename);
	return filecopy((char *)&s,(char *)&t);
}
		


//Adds a backslash at the end of a path statement.
void iaddbs(char *path)
{
	int len=strlen(path);
	char *p=path;

	if(len<3)
		return;
	for(int i=0;i<len-1;i++)
		p++;
	if(*p!='\\')
		strcat(path,"\\");
}

//Checks if a file exists.
//RETURNS: 0 if file does not exist (or cannot be open)
//         1 if the file exists and is openable
int fileexist(char *filename)
{
	FILE *file=fopen(filename,"rb");
	if(file==NULL)
		return 0;
	fclose(file);
	return 1;
}

//Builds a complete path statement out of a directory
//and a filename.
void buildfilename(char *res,char *path,char *fname)
{
	strcpy(res,path);
	iaddbs(res);
	strcat(res,fname);
}

//Removes a filename from a full path statement,
//so that only the directory stays.
//RETURNS: 1 if a "\" was found
//         0 if no "\" was found (path is not modified then)

int removefilename(char *path)
{
	char *p=path;
	char *d=path;
	int len=strlen(path);
	for(int i=0;i<len;i++)
	{
		if(*p=='\\')
			d=p;
		p++;
	}

	if(d!=path)
	{
		d++;
		*d='\0';
		return 1;
	}
	else
		return 0;
}

//Removes an extension from a filename or full path statement.
//RETURNS: 1 if a "." was found, or the last "." was even before a "\"
//         0 if no "." was found (no extension was provided then)
int removeextension(char *path)
{
	char *p=path;
	char *d=path;
	int len=strlen(path);
	for(int i=0;i<len;i++)
	{
		if(*p=='.')
			d=p;
		if(*p=='\\')
			d=path;	//security
		p++;
	}

	if(d!=path)
	{
		*d='\0';
		return 1;
	}
	else
		return 0;
}



BOOL checkvalidpath(char *pname,BOOL rootallowed)
{
	char *p=pname;
	
	if(strlen(pname)<3)
		return FALSE;
	if(!rootallowed && strlen(pname)<4)
		return FALSE;
	p++;
	if(*p!=':')
		return FALSE;
	p++;
	if(*p!='\\')
		return FALSE;

	return TRUE;
}

BOOL makedirs(char *pname)
{
	char tp[8192];
	unsigned int pos=3;
	char *p=pname;
	p++; p++; p++;

	memset(tp,0,8192);
	while(pos<=strlen(pname))
	{
		if((*p=='\\') || (*p=='\0'))
		{
			strncpy_s(tp,pname,pos);
			if(_mkdir(tp)==-1)
				if(errno==ENOENT)
					return FALSE;
		}
		pos++;
		p++;
	}	
	
	return TRUE;
}

void path_main(char *path)
{
	strcpy(path,AfxGetApp()->m_pszHelpFilePath);
	removefilename(path);
}

void path_stif(char *path)
{
	strcpy(path,"");
	
	//Look if StIf is installed;
	if(fileexist("c:\\stif.cfg"))
		GetPrivateProfileString("Directories","StIf3","",path,8000,"c:\\stif.cfg");
}

void fname_stifini(char *fname)
{
	strcpy(fname,"");
	char stifdir[8192];
	path_stif((char *)&stifdir);
	if(strlen(stifdir))
		buildfilename(fname,(char *)&stifdir,"stif.ini");
}

void fname_descentini(char *fname)
{
	strcpy(fname,"");
	char stifdir[8192];
	path_stif((char *)&stifdir);
	if(strlen(stifdir))
		buildfilename(fname,(char *)&stifdir,"descent.ini");
}

//----
/*void httpdownload(char *url, char *dest)
{
#define HTTPDOWNLOAD_BUFFER	256
	CInternetSession m_inet;
	CHttpFile *m_file;
	CFile m_local;
	DWORD m_receivedbytes;
	char *m_filedata;

	m_file=(CHttpFile *)m_inet.OpenURL(url, 1,INTERNET_FLAG_TRANSFER_BINARY);
	
	m_filedata=new char[HTTPDOWNLOAD_BUFFER];
      
	m_local.Open(dest,CFile::modeCreate | CFile::modeWrite);

	m_receivedbytes=1;
	while(m_receivedbytes!=0)
	{
		m_receivedbytes=m_file->Read(m_filedata,HTTPDOWNLOAD_BUFFER);
		m_local.Write(m_filedata,m_receivedbytes);
	}

	m_file->Close();
	m_inet.Close();

	m_local.Close();

	delete(m_filedata);
	delete(m_file);
}*/

CString getappini()
{
	CString ininame=AfxGetApp()->m_pszHelpFilePath;
	ininame.MakeUpper();
	ininame.TrimRight(".HLP");
	ininame=ininame+".INI";
	return ininame;
}