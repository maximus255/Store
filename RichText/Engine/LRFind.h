/*
 *  LRFind.h
 *  RetroGrapherInstaller
 *
 *  Created by Maxim on 11/14/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <Carbon/Carbon.h>
#include "log.h"
extern FSRef *_FoundFolderSub;
extern char _FoundPathSub[];
extern char _EndExactly;
char FindSubfolderByName(ItemCount volumeIndex,char *StartSubfolder,char *aFoundSubfolder,char FindInSubfolders);
char FindSubfolderByNameRef(FSRef* itemRef,char *StartSubfolder,char *aFoundSubfolder,char FindInSubfolders);
void CopyFileSimple(char *aSource, char *aDest);