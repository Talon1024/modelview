#ifndef _DMREG_H
#define _DMREG_H

#define DMREG_GAMETYPE_FREESPACE1	1
#define DMREG_GAMETYPE_FREESPACE2	2

#define DMREG_DISTRIBUTIONTYPE_FULL	0
#define DMREG_DISTRIBUTIONTYPE_DEMO	1
#define DMREG_DISTRIBUTIONTYPE_OEM	2

void DMReg_Init();
BOOL DMReg_SetGameDir(int GameType,int DistributionType,CString path);
CString DMReg_GetGameDir(int GameType,int DistributionType);
CString DMReg_GetGameDir(CString id);
CString DMReg_GetEXEDir();
CString DMReg_GetTEMPDir();
CString DMReg_GetVersion();
int DMReg_GetBuild();
BOOL DMReg_WriteHKCU(CString key,CString value);
BOOL DMReg_WriteHKCU(CString key,int value);
int DMReg_ReadHKCUint(CString key,int def);
CString DMReg_ReadHKCU(CString key,CString def);
BOOL DMReg_WriteHKLM(CString key,int value);
int DMReg_ReadHKLMint(CString key,int def);
BOOL _FileExists(CString file);

#endif

