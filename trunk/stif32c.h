#include <afxinet.h>

#ifndef _STIF
#define _STIF

#define FNAMELEN 8192

int filecopy(char *source, char *target);
int filecopy2(char *source, char *target, char *filename);
void iaddbs(char *path);
int fileexist(char *filename);
int removefilename(char *path);
int removeextension(char *path);
void buildfilename(char *res,char *dir,char *fname);
BOOL checkvalidpath(char *pname,BOOL rootallowed);
BOOL makedirs(char *pname);
void path_main(char *path);
void path_stif(char *path);
void fname_stifini(char *fname);
void fname_descentini(char *fname);
//void httpdownload(char *url, char *dest);
CString getappini();

#endif
