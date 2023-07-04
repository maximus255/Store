/*
 *  CustomInfoAboutSets.h
 *  MachineWashDeluxInstaller
 *
 *  Created by Maxim Chernousov on 10/26/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <Carbon/Carbon.h>

void AddSetsInstallInfo( char *FullPath, char* aSetUID, char* aSetName,int aInstalled);
int CreateDBIfNeed(char *FullPath,char *LastVers);