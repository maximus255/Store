/*
 *  Engine.h
 *  
 *
 *  Created by Maxim Chernousov on 10/21/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <Carbon/Carbon.h>

#ifndef _ENGINE_
#define _ENGINE_

//[mwd2
#define _CS3_CS5_
//#define _AUTOMATION_
//]mwd2

#define CONFIG_VERSION "Version"
#define CONFIG_NEEDSPACE "NeededSize"
#define CONFIG_COPYRIGHT "Copyright"
#define CONFIG_INTRO "Intro"
#define CONFIG_COPYINTRO "CopyIntro"
#define CONFIG_STOREFOLDER	"Subfolder"
#define CONFIG_CONG	"Congratulation"

#define TXT_LOCKED "The disk is locked."
#define TXT_NET "The disk is network volume."
#define TXT_NOSPACE "No enoght space."
#define TXT_ANOTHER "Please select another disk."

//#define FIND_COMPLETE "Finding is completed"
#define FIND_COMPLETE "Location search has been completed"
#define FIND_BREAK "Finding has been stopped"
#define FIND_START "Finding Photoshop plug-ins folder"
#define FIND_FINDED "Photoshop Plug-in's folder has been successfully located"

#define COPY_ERROR "Cannot copy. Some error has occured"
#define COPY_STOPPED "The Coppying process has been stopped"

#define kFindStateUndef		-1
#define kFindStateRun		0
#define kFindStateDone		1
#define kFindStateStopped	-2
extern char FullPath[1024],ScriptsPath[1024];
extern ThreadID    _FindThreadID;
extern char _FindState;
extern char _BreakFinding, _BreakCopying, _NoCS2,_NoCS3,_NoCS4,_NoCS5,_NoCS6,_NoCS7,_NoCC2014,_NoCC2015,_NoCC2016,_PlugInFound;
extern IBNibRef         sNibRef;
extern FSRef _aFoundFolder;
extern FSRef *_FoundFolder,*_SourceFolder,_aFoundfolder, _aSourceFolder, *_SourceFolderCS5, _aSourceFolderCS5;
extern char _Path[];
extern char _UsePPC;
extern SInt32 OSXVersion;
#define PATHSIZE	1024*2
bool AddPluginsFilters();
bool AddPlugins();
OSErr FSGetMountedVolumes(FSRef ***volumeRefsHandle,	ItemCount *numVolumes);
IconRef GetVolIcon(int i);
OSStatus CheckNetwork(ItemCount volumeIndex, char *isNet,  FSVolumeRefNum aVolume=0);
OSErr FSGetVInfo(
	//FSVolumeRefNum volume,	
	ItemCount volumeIndex,
	HFSUniStr255 *volumeName,	UInt64 *freeBytes, UInt64 *totalBytes ,FSVolumeRefNum aVolume = 0)	;
//OSErr FSCheckLock(const FSRef *ref);
OSErr FSCheckLock(ItemCount volumeIndex, FSVolumeRefNum aVolume = 0);
void FindFolder(ItemCount volumeIndex);
void FindFolderThreadStart();
void FindFolderSincronously();
void GetPathDialog();
void ParseConfig();
int ReadStr(FILE *hFile,char str[]);
void SeekOnStartStr(char *  &s);
void SeekOnEndStr(char *  &s);
void StrGetFloat(char *sw,  double *z);
void StrGetInt(char *sw,  int *z);
void RemoveSpacesInEnd(char *str);
void StrGetStr(char *sw, char *res);
//void StrGetBool(char *sw,  BYTE *z);
void GetArgVal(char *sw, char* &arg, char* &val);
void CopyFile();
OSStatus IterateFolder( FSRef * inFolder, char NoSubfolders );
//[Max 01.01.10
char* URLEncode2(char* sIn);
char* URLEncode1(const char* sIn);
void DeleteFile(char *aFileName);
void DeleteFolder(char *aFileName);
typedef struct _HDDInfo
{
	char Locked;
	UInt64 Space;
	UInt64 Free;
	HFSUniStr255 Name;
}HDDInfo;

//[Max 2016.06.04
void AddSlashIfNeed(char *str);

void DebugAlert(char *str);
#endif //_ENGINE_
