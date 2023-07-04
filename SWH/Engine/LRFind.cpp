/*
 *  LRFind.cpp
 *  RetroGrapherInstaller
 *
 *  Created by Maxim on 11/14/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "LRFind.h"
#include "CopyProc.h"

OSStatus IterateFolderSub( FSRef * inFolder, CFStringRef FoundSubfolder,char FindInSubfolders);
OSStatus DoSomethingWithThisObjectSub( const FSCatalogInfo * inCatInfo, FSRef * refs, CFStringRef FoundSubfolder,char FindInSubfolders);
char _BreakFindingSub = 0;
FSRef *_FoundFolderSub;
char _FoundPathSub[1024];
char _EndExactly = 0;
char FindSubfolderByName(ItemCount volumeIndex,char *StartSubfolder,char *aFoundSubfolder,char FindInSubfolders)
{
	_Log("FindSubfolderByName Start")
	OSErr    err;
	FSRef itemRef;
	
	_Log("FindFolder: before FSGetVolumeInfo")
	err = FSGetVolumeInfo(0, volumeIndex, NULL, kFSVolInfoNone, NULL, NULL, &itemRef);
	if (err != noErr)
	{
		_Log("ERRR FindFolder GetVolumeInfo error")
		return 0;// err;
	}
	char curPath[1024];
	FSRefMakePath(&itemRef,(UInt8*)curPath,1024);//Copy Volume path
	strcat(curPath,StartSubfolder);
	FSPathMakeRef((UInt8*)curPath,&itemRef,NULL);
	
	_BreakFindingSub = 0;
	
	//CFStringRef FoundSubfolder = CFStringCreateWithCharacters(NULL,FoundSubfolder,strlen(FoundSubfolder));	
	//rawCFString		= CFStringCreateWithCString( NULL, LicPath, kCFStringEncodingUTF8 );
	CFStringRef FoundSubfolder = CFStringCreateWithCString(NULL, aFoundSubfolder, kCFStringEncodingASCII);
	_Log("before IterateFolderSub")
	err = IterateFolderSub(&itemRef,FoundSubfolder,FindInSubfolders);
	
	_Log("before CFRelease FoundSubfolder")
	CFRelease(FoundSubfolder);
	_Log("FindSubfolderByName end<<<")
	return _BreakFindingSub;
		
}

char FindSubfolderByNameRef(FSRef* itemRef,char *StartSubfolder,char *aFoundSubfolder,char FindInSubfolders)
{
	_Log("FindSubfolderByNameRef Start")
	OSErr    err;
	
	char curPath[1024];
	FSRefMakePath(itemRef,(UInt8*)curPath,1024);//Copy Volume path
	strcat(curPath,StartSubfolder);
	FSPathMakeRef((UInt8*)curPath,itemRef,NULL);
	
	_BreakFindingSub = 0;
	
	//CFStringRef FoundSubfolder = CFStringCreateWithCharacters(NULL,FoundSubfolder,strlen(FoundSubfolder));	
	//rawCFString		= CFStringCreateWithCString( NULL, LicPath, kCFStringEncodingUTF8 );
	CFStringRef FoundSubfolder = CFStringCreateWithCString(NULL, aFoundSubfolder, kCFStringEncodingASCII);
	_Log("before IterateFolderSub")
	err = IterateFolderSub(itemRef,FoundSubfolder,FindInSubfolders);
	
	_Log("before CFRelease FoundSubfolder")
	CFRelease(FoundSubfolder);
	_Log("FindSubfolderByName end<<<")
	return _BreakFindingSub;
	
}

OSStatus IterateFolderSub( FSRef * inFolder, CFStringRef FoundSubfolder, char FindInSubfolders )
{
	OSStatus outStatus;
	//_Log("IterateFolder: Start")
	if (_BreakFindingSub)
		return noErr;
	//SysBeep(1);
	// Get permissions and node flags and Finder info
	//
	// For maximum performance, specify in the catalog
	// bitmap only the information you need to know
	FSCatalogInfoBitmap kCatalogInfoBitmap = (kFSCatInfoNodeFlags |	kFSCatInfoFinderInfo);
	// On each iteration of the do-while loop, retrieve this
	// number of catalog infos
	//
	// We use the number of FSCatalogInfos that will fit in
	// exactly four VM pages (#113). This is a good balance
	// between the iteration I/O overhead and the risk of
	// incurring additional I/O from additional memory
	// allocation
	int MaxDev = (sizeof(FSCatalogInfo)>sizeof(FSRef))?sizeof(FSCatalogInfo) : sizeof(FSRef);
	const size_t kRequestCountPerIteration = ((4096 * 4) / MaxDev);
	FSIterator iterator;
	FSCatalogInfo * catalogInfoArray;
	FSRef *refsArray;
	
	//YieldToAnyThread();
	// Create an iterator
	//kFSIterateSubtree
	//_Log("IterateFolder: before OpeIterator")
	outStatus = FSOpenIterator( inFolder, kFSIterateFlat, &iterator );
	if( outStatus == noErr )
	{
		// Allocate storage for the returned information
		catalogInfoArray = (FSCatalogInfo *) malloc(sizeof(FSCatalogInfo) *	kRequestCountPerIteration);
		refsArray = (FSRef*)malloc(sizeof(FSRef)*kRequestCountPerIteration);
		if(( catalogInfoArray == NULL )||(refsArray==NULL))
		{
			_Log("IterateFolder: ERROR memFullErr")
			outStatus = memFullErr;
		}
		else
		{
			// Request information about files in the given directory,
			// until we get a status code back from the File Manager
			do
			{
				ItemCount actualCount;
				//_Log("IterateFolder: before GetCatalogInfoBulk")
				outStatus = FSGetCatalogInfoBulk(iterator, kRequestCountPerIteration, &actualCount, NULL, kCatalogInfoBitmap,
												 catalogInfoArray, refsArray, NULL, NULL );
				// Process all items received
				if( outStatus == noErr || outStatus == errFSNoMoreItems )
				{
					
					UInt32 index;
					for( index = 0; index < actualCount; index += 1 )
					{
						if (_BreakFindingSub)
							break;
						// Do something interesting with the object found
						//_Log("IterateFolder: before DoSomething")
						outStatus = DoSomethingWithThisObjectSub( &catalogInfoArray[ index ], &refsArray[index], FoundSubfolder ,FindInSubfolders);
						//_Log("IterateFolder: after DoSomething")
						/*if (outStatus!= noErr)
						 {
						 break;
						 }*/
					}
				}
			}
			while(( outStatus == noErr )&&(_BreakFindingSub==0));
			// errFSNoMoreItems tells us we have successfully processed all
			// items in the directory -- not really an error
			if( outStatus == errFSNoMoreItems )
			{
				outStatus = noErr;
			}
			// Free the array memory
			free( (void *) catalogInfoArray );
			free( (void *) refsArray);
		}
	}
	else
	{
		_Log("IterateFolder: OpenIterator ERROR")
	}
	FSCloseIterator(iterator);
	
	//YieldToThread(_FindThreadID);
	//SetThreadState(_FindThreadID,kRunningThreadState,kNoThreadID);
	//SetThreadState(_FindThreadID,kRunningThreadState,_FindThreadID);
	//YieldToThread(_FindThreadID);
	//YieldToAnyThread();
	
	//_Log("IterateFolder FINISH")
	return outStatus;
}

OSStatus DoSomethingWithThisObjectSub( const FSCatalogInfo * inCatInfo, FSRef * refs, CFStringRef FoundSubfolder,char FindInSubfolders)
{
	//_Log("DoSomethingWithThisObject Start")
	OSStatus err = noErr;
	if (_BreakFindingSub)
		return err;
	
	if( (inCatInfo->nodeFlags & kFSNodeIsDirectoryMask) == kFSNodeIsDirectoryMask )
	{
		//printf( "Found a folder\n" );
		HFSUniStr255 DirName;
		err = FSGetCatalogInfo(refs, kFSCatInfoNone, NULL, &DirName, nil, nil);
		
		if (err!=noErr)
			return err;
		
		if (DirName.unicode[0]==46)//.
			return err;
		
		
		CFStringRef CurName;
		CurName = CFStringCreateWithCharacters(NULL,DirName.unicode,DirName.length);
		
		CFRange foundapp = CFStringFind(CurName,FoundSubfolder,kCFCompareCaseInsensitive);		
		if ((foundapp.length!=0)&&(foundapp.location!=kCFNotFound))
		{
			int NoRetun = 1;
			if (_EndExactly)
			{
				_Log("_EndExactly")
				int CurNameLength = CFStringGetLength(CurName);
#ifdef _LOG_
				{
					char str[256];
					sprintf(str,"foundapp.location=%d foundapp.length=%d CurNameLength=%d",foundapp.location,foundapp.length,CurNameLength);
					_Log(str)
				}
#endif
				if (foundapp.location+foundapp.length!=CurNameLength)
				{
					_Log("foundapp.location+foundapp.length!=CurNameLength")
					NoRetun = 0;
				}
			}
			if (NoRetun)
			{
				_BreakFindingSub = 1;
				_FoundFolderSub = refs;
				if (FSRefMakePath(_FoundFolderSub,(UInt8*)_FoundPathSub,1024)!=noErr)
				{
					_Log("ERROR, FSRefMakePath ERROR")
				}
				
				
				if (CurName)
					CFRelease(CurName);
				return err;
			}
		}
		

		
			if (CurName)
				CFRelease(CurName);
		//]
		

		if (FindInSubfolders)
			err = IterateFolderSub(refs,FoundSubfolder,FindInSubfolders);
	}
	/*else
	 {
	 FInfo * theFinderInfo;
	 OSType type;
	 theFinderInfo = (FInfo *)&inCatInfo->finderInfo[0];
	 type = theFinderInfo->fdType;
	 printf( "Found a file (type %c%c%c%c)\n",
	 (char) ((type & 0xFF000000) >> 24),
	 (char) ((type & 0x00FF0000) >> 16),
	 (char) ((type & 0x0000FF00) >> 8),
	 (char) (type & 0x000000FF)
	 );
	 }*/
	return err;
}

void CopyFileSimple(char *aSource, char *aDest)
{

}

