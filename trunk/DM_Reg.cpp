#include "stdafx.h"
#include "DM_Reg.h"
#include "version.h"

//[HKEY_LOCAL_MACHINE\Software\Descent Network\Descent Manager Tools\<name of module>]
//"Path"= (incl. "\")
//"EXEName"= (without path)
//"EXE"= (incl. path)
//"Version"=
//"Build"=
//"VersionDate"=
//"Coder"=

//[HKEY_LOCAL_MACHINE\Software\Descent Network\Games Configuration\<name of game>]
//"Path"=

//[HKEY_CURRENT_USER\Software\Descent Network\<name of module>\Settings]


#define	DMREG_LM	"Software\\Descent Network"
#define DMREG_CU	"Software\\Descent Network"

CString _version;
CString _exepath;
int _build;


CString _GetPrivateProfileString(CString AppName,CString KeyName,CString FileName)
{
	char x[8192];
	GetPrivateProfileString(AppName,KeyName,"$",x,8191,FileName);
	if(strcmp(x,"$")==0)
		return "";
	return x;
}

CString _AddBackslash(CString dir)
{
	if(dir=="")
		return "";
	if(dir.Right(1)!="\\")
		return dir+"\\";
	return dir;
}

BOOL _FileExists(CString file)
{
	CFile f;
	return(f.Open(file,CFile::modeRead));
}

CString _ReadHKLM(CString path,CString key)
{
	HKEY hkey;
	LPDWORD sizedata=(LPDWORD)new LPDWORD;
	long ret;
	char value[8192];
    long valuesize;
	CString regkey=DMREG_LM+path;
	valuesize=8191;
	ret=RegOpenKeyEx(HKEY_CURRENT_USER,regkey,NULL,KEY_QUERY_VALUE,&hkey);
	ret=RegQueryValueEx(hkey,key,NULL,NULL,(LPBYTE)value,(LPDWORD)&valuesize);
	if(ret!=ERROR_SUCCESS)
		*value=0; //make sure value is empty, if an error occured
	ret=RegCloseKey(hkey);
	CString strValue=value;
	delete sizedata;
	return strValue;
}

int _ReadHKLMint(CString path,CString key,int def=0)
{
	HKEY hkey;
	LPDWORD sizedata=(LPDWORD)new LPDWORD;
	long ret;
	int value;
    long valuesize;
	CString regkey=DMREG_LM+path;
	valuesize=4;
	ret=RegOpenKeyEx(HKEY_CURRENT_USER,regkey,NULL,KEY_QUERY_VALUE,&hkey);
	ret=RegQueryValueEx(hkey,key,NULL,NULL,(LPBYTE)&value,(LPDWORD)&valuesize);
	if(ret!=ERROR_SUCCESS)
		value=def; //make sure value is 0, if an error occured
	ret=RegCloseKey(hkey);
	delete sizedata;
	return value;
}

int DMReg_ReadHKCUint(CString key,int def)
{
	HKEY hkey;
	LPDWORD sizedata=(LPDWORD)new LPDWORD;
	long ret;
	int value;
    long valuesize;
	CString path="\\Descent Manager MODELVIEW32\\Settings";
	CString regkey=DMREG_CU+path; //"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Descent 3 Demo.Exe";
	valuesize=4;
	ret=RegOpenKeyEx(HKEY_CURRENT_USER,regkey,NULL,KEY_QUERY_VALUE,&hkey);
	ret=RegQueryValueEx(hkey,key,NULL,NULL,(LPBYTE)&value,(LPDWORD)&valuesize);
	if(ret!=ERROR_SUCCESS)
		value=def; //make sure value is default, if an error occured
	ret=RegCloseKey(hkey);
	delete sizedata;
	return value;
}

CString DMReg_ReadHKCU(CString key,CString def)
{
	HKEY hkey;
	LPDWORD sizedata=(LPDWORD)new LPDWORD;
	long ret;
	char value[8192];
    long valuesize;
	CString path="\\Descent Manager MODELVIEW32\\Settings";
	CString regkey=DMREG_CU+path; //"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Descent 3 Demo.Exe";
	valuesize=8191;
	ret=RegOpenKeyEx(HKEY_CURRENT_USER,regkey,NULL,KEY_QUERY_VALUE,&hkey);
	ret=RegQueryValueEx(hkey,key,NULL,NULL,(LPBYTE)&value,(LPDWORD)&valuesize);
	if(ret!=ERROR_SUCCESS)
		strcpy_s(value,def); //make sure value is default, if an error occured
	ret=RegCloseKey(hkey);
	CString strValue=value;
	delete sizedata;
	return value;
}

BOOL _WriteHKLM(CString path,CString key,CString value)
{
	HKEY hkey;
	LPDWORD sizedata=(LPDWORD)new LPDWORD;
	LPDWORD res=(LPDWORD)new LPDWORD;
	char kvalue[8192];
	
	try {
		memset(&kvalue,0,8192);
		strcpy_s(kvalue,value);
		long valuesize=value.GetLength()+1;
		CString regkey=DMREG_LM+path; //"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Descent 3 Demo.Exe";
		long ret=RegCreateKeyEx(HKEY_CURRENT_USER,regkey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hkey,res);
		if(ret==ERROR_SUCCESS)
			ret=RegSetValueEx(hkey,key,0,REG_SZ,(LPBYTE)kvalue,valuesize);
		if(ret==ERROR_SUCCESS)
			ret=RegCloseKey(hkey);
		delete sizedata;
		delete res;
		return ret==ERROR_SUCCESS;
	} catch (char* str) {
			delete sizedata;
			delete res;
			return ERROR_GEN_FAILURE;
	}

	delete sizedata;
}

BOOL _WriteHKLM(CString path,CString key,int value)
{
	HKEY hkey;
	LPDWORD sizedata=(LPDWORD)new LPDWORD;
	LPDWORD res=(LPDWORD)new LPDWORD;
	char kvalue[4];
	try {
		memset(&kvalue,0,4);
		memcpy(kvalue,&value,4);
		long valuesize=4;
		CString regkey=DMREG_LM+path; //"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Descent 3 Demo.Exe";
		long ret=RegCreateKeyEx(HKEY_CURRENT_USER,regkey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hkey,res);
		if(ret==ERROR_SUCCESS)
			ret=RegSetValueEx(hkey,key,0,REG_DWORD,(LPBYTE)kvalue,valuesize);
		if(ret==ERROR_SUCCESS)
			ret=RegCloseKey(hkey);
		delete sizedata;
		delete res;
		return ret==ERROR_SUCCESS;
	}
	catch (char* str) {
			delete sizedata;
			delete res;
			return ERROR_GEN_FAILURE;
	}

	delete sizedata;
}

BOOL DMReg_WriteHKCU(CString key,CString value)
{
	HKEY hkey;
	LPDWORD sizedata=(LPDWORD)new LPDWORD;
	LPDWORD res=(LPDWORD)new LPDWORD;
	char kvalue[8192];
	try {
		memset(&kvalue,0,8192);
		strcpy_s(kvalue,value);
	    long valuesize=value.GetLength()+1;
		CString path="\\Descent Manager MODELVIEW32\\Settings";
		CString regkey=DMREG_CU+path; //"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Descent 3 Demo.Exe";
		long ret=RegCreateKeyEx(HKEY_CURRENT_USER,regkey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hkey,res);
		if(ret==ERROR_SUCCESS)
			ret=RegSetValueEx(hkey,key,0,REG_SZ,(LPBYTE)kvalue,valuesize);
		if(ret==ERROR_SUCCESS)
			ret=RegCloseKey(hkey);
		delete sizedata;
		delete res;
		return ret==ERROR_SUCCESS; 
	} 
	catch (char* str) {
			delete sizedata;
			delete res;
			return ERROR_GEN_FAILURE;
	}

	delete sizedata;
}

BOOL DMReg_WriteHKCU(CString key,int value)
{
	HKEY hkey;
	LPDWORD sizedata=(LPDWORD)new LPDWORD;
	LPDWORD res=(LPDWORD)new LPDWORD;
	char kvalue[4];
	try {
		memset(&kvalue,0,4);
		memcpy(kvalue,&value,4);
	    long valuesize=4;
		CString path="\\Descent Manager MODELVIEW32\\Settings";
		CString regkey=DMREG_CU+path; //"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Descent 3 Demo.Exe";
		long ret=RegCreateKeyEx(HKEY_CURRENT_USER,regkey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&hkey,res);
		if(ret==ERROR_SUCCESS)
			ret=RegSetValueEx(hkey,key,0,REG_DWORD,(LPBYTE)kvalue,valuesize);
		if(ret==ERROR_SUCCESS)
			ret=RegCloseKey(hkey);
		delete sizedata;
		delete res;
		return ret==ERROR_SUCCESS;
	} 
	catch (char* str) {
			delete sizedata;
			delete res;
			return ERROR_GEN_FAILURE;
	}

	delete sizedata;
}


void _Init_StIfMigrate()
{
	if(_ReadHKLMint("\\Games Configuration","StIfMigration")!=0)
		return;

	_WriteHKLM("\\Games Configuration","StIfMigration",1);
	CFile f;

	//Check for StIf
	if(!_FileExists("c:\\stif.cfg"))
		return; //End here

	//Get StIf dirs and files
	CString stifdir=_GetPrivateProfileString("Directories","StIf3","c:\\stif.cfg");
	if(stifdir=="")
		return; //End here
	CString stifini=_AddBackslash(stifdir)+"stif.ini";
	CString descini=_AddBackslash(stifdir)+"descent.ini";
	if(!_FileExists(stifini))
		return; //End here

	//Read games dirs
	if(_FileExists(descini))
	{
		/*VERIFY(_WriteHKLM("\\Games Configuration\\Descent 1 Demo","Path",_GetPrivateProfileString("Descent","D1Full",descini)));
		VERIFY(_WriteHKLM("\\Games Configuration\\Descent 1 OEM","Path",_GetPrivateProfileString("Descent","D1OEM",descini)));
		VERIFY(_WriteHKLM("\\Games Configuration\\Descent 1","Path",_GetPrivateProfileString("Descent","D1Full",descini)));
		VERIFY(_WriteHKLM("\\Games Configuration\\Descent 2 Demo","Path",_GetPrivateProfileString("Descent","D2Full",descini)));
		VERIFY(_WriteHKLM("\\Games Configuration\\Descent 2 OEM","Path",_GetPrivateProfileString("Descent","D2OEM",descini)));
		VERIFY(_WriteHKLM("\\Games Configuration\\Descent 2","Path",_GetPrivateProfileString("Descent","D2Full",descini)));
		VERIFY(_WriteHKLM("\\Games Configuration\\Descent 3 Demo","Path",_GetPrivateProfileString("Descent","D3Full",descini)));
		VERIFY(_WriteHKLM("\\Games Configuration\\Descent 3 OEM","Path",_GetPrivateProfileString("Descent","D3OEM",descini)));
		VERIFY(_WriteHKLM("\\Games Configuration\\Descent 3","Path",_GetPrivateProfileString("Descent","D3Full",descini)));*/
		//VERIFY(_WriteHKLM("\\Games Configuration\\FreeSpace 1 Demo","Path",_GetPrivateProfileString("FreeSpace","FS1Demo",descini)));
		//VERIFY(_WriteHKLM("\\Games Configuration\\FreeSpace 1 OEM","Path",_GetPrivateProfileString("FreeSpace","FS1OEM",descini)));
		//VERIFY(_WriteHKLM("\\Games Configuration\\FreeSpace 1","Path",_GetPrivateProfileString("FreeSpace","FS1Full",descini)));
		//VERIFY(_WriteHKLM("\\Games Configuration\\FreeSpace 2 Demo","Path",_GetPrivateProfileString("FreeSpace","FS2Demo",descini)));
		//VERIFY(_WriteHKLM("\\Games Configuration\\FreeSpace 2 OEM","Path",_GetPrivateProfileString("FreeSpace","FS2OEM",descini)));
		VERIFY(_WriteHKLM("\\Games Configuration\\FreeSpace 2","Path",_GetPrivateProfileString("FreeSpace","FS2Full",descini)));
		//VERIFY(_WriteHKLM("\\Games Configuration\\Summoner 1 Demo","Path",_GetPrivateProfileString("Summoner","S1Demo",descini)));
		//VERIFY(_WriteHKLM("\\Games Configuration\\Summoner 1 OEM","Path",_GetPrivateProfileString("Summoner","S1OEM",descini)));
		//VERIFY(_WriteHKLM("\\Games Configuration\\Summoner 1","Path",_GetPrivateProfileString("Summoner","S1Full",descini)));
	}
}

void _Init_RegisterModule()
{
	//Get complete path
	CString exe=AfxGetApp()->m_pszHelpFilePath;
	exe.TrimRight(".HLP");
	exe.TrimRight(".hlp");
	exe+=".exe";

	//Split into path and exe filename
	CString exename;
	CString path;
	int pos=-1;
	for(int i=0;i<exe.GetLength();i++)
	{
		if(exe[i]=='\\')
			pos=i;
	}
	ASSERT(pos!=-1);
	if(pos!=-1)
	{
		path=exe.Left(pos+1);
		exename=exe.Mid(pos+1);
	}
	else
	{
		path="";
		exename=exe;
	}

	//Get version, build and date
	CString version;
	CString date;
	CString coder;
	int buildb = 0;

	version = VERSION;
	date = BUILDDATE;
	coder = CODER;
	buildb = BUILDNUM;
#ifdef DEBUG
	exename = "MODVIEW32d.exe";
#else
	exename = "MODVIEW32.exe";
#endif

	//Write everything into the registry
	CString keyname="\\Descent Manager Tools\\Descent Manager MODELVIEW32";
	_WriteHKLM(keyname,"Path",path);
	_WriteHKLM(keyname,"EXE",exe);
	_WriteHKLM(keyname,"EXEName",exename);
	_WriteHKLM(keyname,"Version",version);
	_WriteHKLM(keyname,"Build",buildb);
	_WriteHKLM(keyname,"Date",date);
	_WriteHKLM(keyname,"Coder",coder);

	_exepath=path;
	_version=version;
	_build=buildb;
}


/*{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL,ret,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0,NULL);
	MessageBox(NULL,(LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
	LocalFree(lpMsgBuf); // Free the buffer.
}*/




//Checks for old StIf files and migrates its information
void DMReg_Init()
{
	_Init_RegisterModule();
	_Init_StIfMigrate();
	//_Init_AutoSetGameDirs();
}

//Get game dir
CString DMReg_GetGameDir(int GameType,int DistributionType)
{
	CString key="\\Games Configuration\\";

	switch(GameType)
	{
	case DMREG_GAMETYPE_FREESPACE1:	key+="FreeSpace 1"; break;
	case DMREG_GAMETYPE_FREESPACE2:	key+="FreeSpace 2"; break;
	default: ASSERT(FALSE);	return "<Internal error: Invalid GameType>";
	}

	switch(DistributionType)
	{
	case DMREG_DISTRIBUTIONTYPE_FULL:	break;
	case DMREG_DISTRIBUTIONTYPE_DEMO:	key+=" Demo"; break;
	case DMREG_DISTRIBUTIONTYPE_OEM:	key+=" OEM"; break;
	default: ASSERT(FALSE);	return "<Internal error: Invalid DistributionType>";
	}

	return _AddBackslash(_ReadHKLM(key,"Path"));
}

CString DMReg_GetGameDir(CString id)
{
	CString key="\\Games Configuration\\"+id;
	return _AddBackslash(_ReadHKLM(key,"Path"));
}

BOOL DMReg_SetGameDir(int GameType,int DistributionType,CString path)
{
	CString key="\\Games Configuration\\";

	switch(GameType)
	{
	case DMREG_GAMETYPE_FREESPACE1:	key+="FreeSpace 1"; break;
	case DMREG_GAMETYPE_FREESPACE2:	key+="FreeSpace 2"; break;
	default: ASSERT(FALSE);	return FALSE;
	}

	switch(DistributionType)
	{
	case DMREG_DISTRIBUTIONTYPE_FULL:	break;
	case DMREG_DISTRIBUTIONTYPE_DEMO:	key+=" Demo"; break;
	case DMREG_DISTRIBUTIONTYPE_OEM:	key+=" OEM"; break;
	default: ASSERT(FALSE);	return FALSE;
	}

	return _WriteHKLM(key,"Path",_AddBackslash(path));
}

BOOL DMReg_WriteHKLM(CString key,int value)
{
	return _WriteHKLM("\\Descent Manager Tools\\Descent Manager MODELVIEW32",key,value);
}

int DMReg_ReadHKLMint(CString key,int def)
{
	return _ReadHKLMint("\\Descent Manager Tools\\Descent Manager MODELVIEW32",key,def);
}

CString DMReg_GetTEMPDir()
{
	return _AddBackslash(getenv("TEMP"));
}
/*	char *libvar;
   libvar = getenv( "LIB" );*/



CString DMReg_GetEXEDir() {	return _exepath; }
CString DMReg_GetVersion(){ return _version; }
int     DMReg_GetBuild()  { return _build;   }