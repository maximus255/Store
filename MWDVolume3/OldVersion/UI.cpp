/*
 *  UI.cpp
 *  Engraver2Installer
 *
 *  Created by Maxim Chernousov on 10/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "UI.h"
#include "Engine.h"
#include "PictControl.h"
#include "GeneratedPict.h"
#include "CopyProc.h"
#include "yesnodialog.h"
#include "CustomInfoAboutSets.h"
#include "log.h"
#include "LRFind.h"
extern TPictCtr *IntroPic;//,*AgreementBorder;
void DebugAlertString( CFStringRef Str);
BYTE _State = STATE_INTRO;
char _Agree=-1;
WindowRef MainDlg;

ControlRef NextCtr,PrevCtr,CloseCtr;
ControlID controlID; 

ControlRef LicenseYesCtr,LicenseNoCtr,LicenseTxtCtr,LicenseTxtVCtr,LicenseBorderCtr;

static CGColorSpaceRef myGetGenericRGBSpace(void)
{
    // Only create the color space once.
    static CGColorSpaceRef colorSpace = NULL;
    if ( colorSpace == NULL ) {
	colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    }
    return colorSpace;
}

static CGColorRef myGetGrayColor(void)
{
    // Only create the CGColor object once.
    static CGColorRef gray = NULL;
    if(gray == NULL){
	// R,G,B,A
	//float opaqueGray[4] = { 0.85f, 0.85f, 0.85f, 1 };
	//float opaqueGray[4] = { 244.0f/255, 238.0f/255, 215.0f/255, 1 };
	float opaqueGray[4] = { 227.0f/255, 221.0f/255, 200.0f/255, 1 };
	//float opaqueGray[4] = { (float)232/255, (float)232/255, (float)232/255, 1 };
	//float opaqueGray[4] = { 0.9f, 0.9f, 0.9f, 1 };
	gray = CGColorCreate(myGetGenericRGBSpace(), opaqueGray);
    }
    return gray;
}

int _HDDSelected = -1;
void CheckHDDPress(int ID)
{
	if (GenPict==NULL)
		return;
		
	if ((ID>IDC_HDDITEMS)&&(ID<IDC_HDDSELECTED))
	{
		int i = ID-IDC_HDDITEMS-1;
		
		if( (i>=GenPict->iNControls)||(i<0))
			return;
		/*//test
		{		
			char str[256];
			sprintf(str,"i = %d GenPict->iNControls=%d",i, GenPict->iNControls);
			DebugAlert(str);
		}*/
		//if (GenPict->iHDDInfo[i].Locked==0)//is not locked						
		{
			//ClearSelection
			for (int j=0;j<GenPict->iNControls;j++)
			{
				if ((GenPict->iPictCtr[j].iState==kPickCtrStateChecked)&&(j!=i))
				{
					GenPict->iPictCtr[j].iState=kPictCtrStateNormal;
					Draw1Control(GenPict->iPictCtr[j].selfCtr);
				}
			}//for
			
			
			GenPict->iPictCtr[i].iState=kPickCtrStateChecked;
			Draw1Control(GenPict->iPictCtr[i].selfCtr);
			
			
		}
		if (GenPict->iHDDInfo[i].Locked==0)//is not locked
		{
			_HDDSelected = i;
			SelectedPict->CopySelected(GenPict->iPictCtr[i],GenPict->iHDDInfo[i]);
			Draw1Control(SelectedPict->selfCtr);
			EnableControl(NextCtr);
		}
		else
		{
			_HDDSelected = -1;
			SelectedPict->CopySelected(GenPict->iPictCtr[i],GenPict->iHDDInfo[i]);
			Draw1Control(SelectedPict->selfCtr);
			
			DisableControl(NextCtr);
		}
	}//ID>HDDITEMS
}

/*static	TXNObject	GetTXNObjectBySigAndID( WindowRef window, OSType signature, SInt32 id )
{
	HIViewRef		hiTextView;
	TXNObject		txnObject		= NULL;
	
	if ( window != NULL )
		if ( HIViewFindBySigAndID( HIViewGetRoot(window), signature, id, &hiTextView ) == noErr )
			if ( hiTextView != NULL )
				txnObject	= HITextViewGetTXNObject( hiTextView );
	
	return( txnObject );
}
*/

void SetFinishControls(ControlRef PanelCtr)
{
	Rect rect1,rect2;
	GetControlBounds(PanelCtr,&rect1);
	int CurW,CurH;
	CurW =  rect1.right - rect1.left;
	CurH = rect1.bottom - rect1.top;
	rect1.left = 0; rect1.top = 0;
	rect1.right = CurW; rect1.bottom = CurH;
	
	#define CONGR_LEFT  40//50
	#define CONGR_TOP   2
	#define CONGR_HEIGHT  50
	rect2.left = rect1.left + CONGR_LEFT;
	rect2.right = rect1.right-20-10;
	rect2.top = rect1.top + CONGR_TOP+15;
	rect2.bottom = rect2.top + CONGR_HEIGHT;
	SetControlBounds(FinishCongCtr,&rect2);
	
	
	GetControlBounds(FinishPathLCtr,&rect2);
	CurW = rect2.right- rect2.left;
	CurH = rect2.bottom - rect2.top;
	#define FINISH_PATH_LEFT 15
	#define FINISH_PATH_TOP	CONGR_TOP + 255+28;
	rect2.left = rect1.left+FINISH_PATH_LEFT;
	rect2.right = rect2.left+CurW;
	rect2.top = rect1.top + FINISH_PATH_TOP;
	rect2.bottom = rect2.top + CurH;
	//SetControlBounds(FinishPathLCtr,&rect2);
	
	rect2.left += 5; 
	rect2.right = rect1.right - 20;
	rect2.top  += 10;//20;
	rect2.bottom = rect1.bottom -5;
	SetControlBounds(FinishPathCtr,&rect2);
	
	
	SetControlData(FinishCongCtr,0,kControlStaticTextTextTag,strlen(_Congratulation),_Congratulation);
	//[mwd2
	SetControlData(FinishPathCtr,0,kControlStaticTextTextTag,strlen(_Path),_Path);
	/*if (_FoundFolder)
	{
		FSRefMakePath(_FoundFolder,(UInt8*)_Path,PATHSIZE);
		SetControlData(FinishPathCtr,0,kControlStaticTextTextTag,strlen(_Path),_Path);
	}*/
	
}//SetFinishControls


void SetIntroControls(ControlRef PanelCtr)
{
	Rect rect1,rect2;
	GetControlBounds(PanelCtr,&rect1);
	int CurW,CurH;
	CurW =  rect1.right - rect1.left;
	CurH = rect1.bottom - rect1.top;
	rect1.left = 0; rect1.top = 0;
	rect1.right = CurW; rect1.bottom = CurH;
	
	GetControlBounds(IntroVersLCtr,&rect2);
	CurW = rect2.right - rect2.left;
	CurH = rect2.bottom - rect2.top;
	#define VERS_LEFT  174//10
	#define VERS_TOP	22
	//#define VERS_TOP   
	rect2.left = VERS_LEFT;
	rect2.right = rect2.left + CurW;
	//rect2.top = rect1.top + VERS_TOP;
	rect2.bottom = rect1.bottom-VERS_TOP;
	rect2.top = rect2.bottom -63;//25;
	SetControlBounds(IntroVersLCtr,&rect2);
	
	rect2.left = rect2.right+0;
	//rect2.right = rect1.right-FRAMELEFT-0;
	rect2.right = rect2.right+50;
	SetControlBounds(IntroVersCtr,&rect2);
	
	rect2.left = 245;//115;
	rect2.right = rect2.left + 170;//rect1.right-FRAMELEFT+5;
	rect2.bottom = rect1.bottom-7;
	rect2.top = rect2.bottom -20;
	SetControlBounds(IntroCopyrightCtr,&rect2);
	
	rect2.left = 20;//75;//rect1.left - 20;
	rect2.right = 400;//rect1.right-FRAMELEFT-0;
	rect2.top = rect1.bottom - 303;
	rect2.bottom = rect2.top + 32;
	SetControlBounds(IntroIntroCtr,&rect2);
	
	SetControlData(IntroVersCtr,0,kControlStaticTextTextTag,strlen(_Version),_Version);
	SetControlData(IntroCopyrightCtr,0,kControlStaticTextTextTag,strlen(_Copyright),_Copyright);
	SetControlData(IntroIntroCtr,0,kControlStaticTextTextTag,strlen(_Intro),_Intro);
	
	//[Max 7.12.09
	Draw1Control(NextCtr);
	//]
}//SetIntroControls
char* URLEncode3(const char* sIn);
void SetLicenseControls(ControlRef PanelCtr)
{
#define LICBORD		10
#define LICBOTTOM	10 //40
	Rect rect1,rect2;
	
	GetControlBounds(PanelCtr,&rect1);
	int width1 = rect1.right-rect1.left;
	int height1=rect1.bottom-rect1.top;
	rect2.left = LICBORD;
	rect2.top = LICBORD+20;
	rect2.right=width1-LICBORD;
	rect2.bottom=height1-LICBOTTOM;
	/*
	rect2.left = rect1.left+LICBORD;
	rect2.right=rect1.right-LICBORD;
	rect2.top = rect1.top+LICBORD;
	rect2.bottom=rect1.bottom-LICBOTTOM;
	*/
	//SetControlBounds(LicenseTxtCtr,&rect2);
	SetControlBounds(LicenseBorderCtr,&rect2);
	/*AgreementBorder->Init(IDC_LICENSEBORDER);
	AgreementBorder->iType = kPictCtrTypeSolidRound;
	AgreementBorder->Bounds.left += FRAMELEFT;
	AgreementBorder->Bounds.right += FRAMELEFT;
	AgreementBorder->Bounds.top += FRAMETOP;
	AgreementBorder->Bounds.bottom += FRAMETOP;*/
	
	#define BORDEROFFSET	0
	rect2.left += BORDEROFFSET;
	rect2.top += BORDEROFFSET-20;
	rect2.right -= 20;
	rect2.bottom -= 40;
	SetControlBounds(LicenseTxtCtr,&rect2);
	
	HideControl(LicenseYesCtr);
	HideControl(LicenseNoCtr);
	/*
	////Agree
	GetControlBounds(LicenseYesCtr,&rect2);
	int width = rect2.right-rect2.left;
	int height=rect2.bottom-rect2.top;
	rect2.left = LICBOTTOM*2;
	rect2.right=rect2.left+width;
	rect2.top = height1-(LICBOTTOM+height)/2;
	rect2.bottom=rect2.top+height;
	SetControlBounds(LicenseYesCtr,&rect2);
	if (_Agree<=0)
	{
		SetControlValue(LicenseYesCtr,0);
	}
	else
	{
		SetControlValue(LicenseYesCtr,1);
	}
	
	////Disagree
	GetControlBounds(LicenseNoCtr,&rect2);
	width = rect2.right-rect2.left;
	height=rect2.bottom-rect2.top;
	rect2.left = width1-LICBOTTOM*2-width;
	rect2.right=rect2.left+width;
	rect2.top = height1-(LICBOTTOM+height)/2;
	rect2.bottom=rect2.top+height;
	SetControlBounds(LicenseNoCtr,&rect2);
	if (_Agree!=0)
		SetControlValue(LicenseNoCtr,0);
	else
		SetControlValue(LicenseNoCtr,1);
	*/
	
	
	//Read License
	//TXNObject txnObject = HITextViewGetTXNObject(hiTextView);
	CFURLRef tCFURLRef = NULL;
	CFStringRef					rawCFString=NULL;
	char LicPath[2048];
	strcpy(LicPath,ScriptsPath);
	strcat(LicPath,"license.txt");
	
	
	
	//rawCFString		= CFStringCreateWithCString( NULL, LicPath, CFStringGetSystemEncoding() );
	//[Max 01.01.10
	//DebugAlert(LicPath);
	//tCFURLRef= CFURLCreateWithBytes(NULL, (UInt8*)LicPath, strlen(LicPath), CFStringGetSystemEncoding(), NULL);
	//[Max 23.06.2010
	tCFURLRef= CFURLCreateWithBytes(NULL, (UInt8*)LicPath, strlen(LicPath), kCFStringEncodingUTF8, NULL);
	//char *request_safe = URLEncode1(LicPath);
	/*char *request_safe = URLEncode1(LicPath);
	//DebugAlert(request_safe);
	if (request_safe)
	{
		//DebugAlert(request_safe);
		//rawCFString		= CFStringCreateWithCString( NULL, request_safe, kCFStringEncodingUTF8 );
		rawCFString		= CFStringCreateWithCString( NULL, request_safe, CFStringGetSystemEncoding() );
		delete [] request_safe;
	}
	else
		//rawCFString		= CFStringCreateWithCString( NULL, LicPath, kCFStringEncodingUTF8 );
		rawCFString		= CFStringCreateWithCString( NULL, LicPath, CFStringGetSystemEncoding() );
	//]
	
	//DebugAlertString(rawCFString);
	
	if (rawCFString==NULL)
		{
			DebugAlert("Cannot create CFString for license file");
			return;
		}
	
	tCFURLRef= CFURLCreateWithString( kCFAllocatorDefault, rawCFString, NULL );	*/
	//DebugAlertString(CFURLGetString(tCFURLRef));
	if (rawCFString)
		CFRelease( rawCFString );
	//]
	if (tCFURLRef==NULL)
	{
		DebugAlert("Cannot convert license file to URL");
		return;
	}
	
	TXNObject txnObject = HITextViewGetTXNObject(LicenseTxtVCtr);
	
		
	TXNReadFromCFURL(txnObject, kTXNStartOffset, kTXNEndOffset, NULL, tCFURLRef, NULL);
	
	/*
	TXNObject txnObject   = GetTXNObjectBySigAndID( window, 'inst', IDC_LICENSETXTV );
	TXNSetDataFromCFURLRef( txnObject, tCFURLRef, kTXNStartOffset, kTXNEndOffset );*/
	
	
	//TXNSetScrollbarState(txnObject,kScrollBarsAlwaysActive);
	//SetControlFontStyle(LicenseTxtCtr,&PassStyle);
	
	//////////
	TXNRevert( txnObject );																	//  The change count is not set to 0 as it should,  a call to TXNRevert workarounds the problem
	TXNSetSelection( txnObject, kTXNEndOffset, kTXNEndOffset );
	TXNShowSelection( txnObject, true );													//  The HIScrollView scroll bars don't reflect the current size of the HITextView, a couple of calls to Set/Show selection workaround the problem
	TXNSetSelection( txnObject, kTXNStartOffset, kTXNStartOffset );
	TXNShowSelection( txnObject, true );	
	
	HITextViewSetBackgroundColor(LicenseTxtVCtr,myGetGrayColor());
	//////////
	
	if (tCFURLRef != NULL)
        CFRelease(tCFURLRef);

}//SetLicenseControls()
char _LRwasFound = 0,_AperturewasFound = 0,_iPhotowasFound = 0;
char _showLRcheckbox = 0,_showAperturecheckbox=0,_showiPhotocheckbox=0;
char _LRScriptPath[1024];
char _InstallLightroom = 1;
char _InstallAperture = 1;
char _InstalliPhoto = 1;
int _NeedToCopyTextures = 1;
//for chmod
#include <sys/stat.h>
void SetCopyControls(ControlRef PanelCtr)
{
	_Log("SetCopyControls >>>>")
	Rect rect1,rect2;
	GetControlBounds(PanelCtr,&rect1);
	
	rect1.left -= FRAMELEFT;
	rect1.right -= FRAMELEFT;
	rect1.top -=FRAMETOP;
	rect1.bottom -=FRAMETOP;
	
	rect2.left = rect1.left + 20;
	rect2.top = rect1.top + 50;
	rect2.bottom = rect2.top + 130;
	rect2.right = rect1.right - 20;
	SetControlBounds(CopyIntroCtr,&rect2);
	
	//Progress Stop
	#define COPY_PROGRESS_TOP	150
	GetControlBounds(CopyStopCtr,&rect2);
	int StpW = rect2.right - rect2.left;
	int StpH = rect2.bottom - rect2.top;
	
	rect2.left = rect1.left + 20;
	rect2.top = rect1.top + COPY_PROGRESS_TOP;
	rect2.bottom = rect2.top + 25;
	rect2.right = rect1.right - 20 - 10 - StpW;
	SetControlBounds(CopyProgressCtr,&rect2);
	
	rect2.left = rect1.right -StpW - 20;
	rect2.top = rect1.top + COPY_PROGRESS_TOP;
	rect2.bottom = rect2.top + StpH;
	rect2.right = rect1.right - 20;
	SetControlBounds(CopyStopCtr,&rect2);

	rect2.left = rect1.left + 20;
	rect2.top = rect1.top + COPY_PROGRESS_TOP - 20;
	rect2.bottom = rect2.top + 20;
	rect2.right = rect1.right - 20;
	SetControlBounds(CopyCurrentCtr,&rect2);
	
	
	SetControlData(CopyIntroCtr,0,kControlStaticTextTextTag,strlen(_CopyIntro),_CopyIntro);
	
	_Log("FoundFolder before deleteBasic")
#ifdef _LOG_
	///test
	{
		char str[256];
		FSRefMakePath(_FoundFolder,(UInt8*)str,256);
		_Log("The _FoundFolder is:")
		_Log(str)
	}
#endif
	
	//there it need to delete "Basic" presets folder!
	{
		_Log("before deleting BasicPresets")
		_Log("_Path is")
		_Log(_Path)
		char BasicPresetsPath[1024];
		strcpy(BasicPresetsPath,_Path);
		strcat(BasicPresetsPath,"/MisterRetro/Machine Wash Deluxe/Presets/Basic");
		_Log("BasicPresetsPath is")
		_Log(BasicPresetsPath)
		//DeleteFile(BasicPresetsPath);
		DeleteFolder(BasicPresetsPath);
	}
	///Start Copying
	_Log("Start Copying")
	if (_Path!=NULL)
	{
		_Log("_Path!=NULL")
		_Log(_Path)
		char FullPath1[1024];
		strcpy(FullPath1, _Path);
		
		//[Max 2014.05.29
		//Set folder readwrite permissions
		int chmodres = chmod(FullPath1, 0777);
#ifdef _LOG_
		{			
			_LogInt("chmodres=%d",chmodres)
			_Log(FullPath1)
			_Log("_StoreFolder is:")
			_Log(_StoreFolder)
		}
#endif
		//]
		
		strcat(FullPath1,"/");
		strcat(FullPath1,_StoreFolder);
		
		
		_Log("before AddSetsInstallInfo")
		//[Max 2013.01.09
		strcat(FullPath1,"/Machine Wash Deluxe");
		CreateDirectories(FullPath1);
		
		if (FSPathMakeRef((UInt8*)_Path,&_aFoundFolder,NULL)!=0)
		{
			_Log("ERROR FSPathMakeRef")
		}
	
		_Log("_FoundFolder after create directory")
#ifdef _LOG_
		///test
		{
			char str[256];
			FSRefMakePath(_FoundFolder,(UInt8*)str,256);
			_Log("The _FoundFolder is:")
			_Log(str)
		}
#endif		
		
		
		strcat(FullPath1,"/Updatings.db");
		//strcat(FullPath1,"/Machine Wash Deluxe/Updatings.db");
		_Log(FullPath1)
		
		if (_Version)
		{
			_Log("_Version is presented")
			if(!_Version[0]) 
				strcpy(_Version,"2.0");
			
			
			_NeedToCopyTextures = CreateDBIfNeed(FullPath1,_Version);
		}
		else
		{
			_Log("no _Version")
			_NeedToCopyTextures = CreateDBIfNeed(FullPath1,"2.0");
		}
#ifdef _LOG_
		{
			char str[64];
			sprintf(str,"_NeedToCopyTextures=%d",_NeedToCopyTextures);
			_Log(str)
		}
#endif
		
		if (_NeedToCopyTextures)
		{
			_Log("_NeedToCopyTextures = 1")
		//AddSetsInstallInfo( FullPath1, "12321", "Test Sets of Textures");
		AddSetsInstallInfo( FullPath1, "39276", "Default Textures",1);
		AddSetsInstallInfo( FullPath1, "29486", "Texture Set One",2);
		AddSetsInstallInfo( FullPath1, "59321", "Texture Set Two",2);
		AddSetsInstallInfo( FullPath1, "10734", "Texture Set Three",2);
		AddSetsInstallInfo( FullPath1, "82349", "Texture Set Four",2);
		AddSetsInstallInfo( FullPath1, "51203", "Texture Set Five",2);
		}
		_Log("after AddSetsInstallInfo")
	}
	else
	{
		_Log("_FoundFolder = NULL!!!")
	}
	
	CopyFile();
	//[MWD2
	///For LR
	if ((_showLRcheckbox)&&(_InstallLightroom)&&(_BreakCopying!=-1))
	{
		_Log("will generate script for LR")
		char SourceLRScript[1024];
		char DestLRScript[1024];
		strcpy(SourceLRScript,ScriptsPath);
		strcat(SourceLRScript,"External Editor Presets/");
		//strcat(SourceLRScript,"RetroGrapher.lrtemplate");
		
		strcpy(DestLRScript,_LRScriptPath);
		//strcat(DestLRScript,"/External Editor Presets/RetroGrapher.lrtemplate");
		//strcat(DestLRScript,"/External Editor Presets/");
		CreateDirectories(_LRScriptPath);
		strcat(DestLRScript,"/");
		
		_Log(SourceLRScript)
		_Log(DestLRScript)
		
		CopyFileSimple(SourceLRScript, DestLRScript);
		
		///Save second path of file
		char AppPath[1024];
		strcpy(AppPath,_Path);
		if (_BreakFinding<=1)
			strcat(AppPath,"/MachineWashDeluxeApp.app");
		else 
			strcat(AppPath,"/MisterRetro/MachineWashDeluxeApp.app");
		
		strcat(DestLRScript,"External Editor Presets/MachineWashDeluxe.lrtemplate");
		_Log(AppPath)
		//_Log(DestLRScript)
		//_Log(_Path)
		FILE *file;
		file = fopen(DestLRScript,"ab");
		if (file)
		{
			fwrite(AppPath,1,strlen(AppPath),file);
			char str[] = "\",\n\t\t},\n\t version = 0,\n }\n";
			fwrite(str,1,strlen(str),file);
			fclose(file);
		}
		
	}//LR
	
	///For Aperture
	if ((_showAperturecheckbox)&&(_InstallAperture)&&(_BreakCopying!=-1))
	{
		
		FSRef        userFolderRef;
		char UserPath[1024];
		if (FSFindFolder( kUserDomain, kCurrentUserFolderType, kCreateFolder, &userFolderRef )==noErr)
		{	
			if (FSRefMakePath(&userFolderRef,(UInt8*)UserPath,1024)==noErr)
			{
				char DestPath[1024];
				char SrcPath[1024];
				strcpy(SrcPath, FullPath);
				strcat(SrcPath,"/Contents/Resources/");
				//if ((_UsePPC==1)||(OSXVersion<0x1050)||(_BreakFinding<5))	
				if ((_UsePPC==1))//||(_BreakFinding<5))
				{
					_Log("PPC will be installed")
					strcat(SrcPath,"x32_PPC/");
				}
				else
				{
					_Log("x64 will be installed")
					strcat(SrcPath,"x64/");
				}
				strcat(SrcPath,"Aperture/");
				//char SrcApertureAppPath[1024];
				//strcpy(SrcApertureAppPath,SrcPath);
				strcat(SrcPath,"MachineWashDeluxe2Aperture.ApertureEdit");
				
				int len = strlen(UserPath);
				if (len>0)
					if ((UserPath[len-1]!='/')&&(UserPath[len-1]!='\\'))
						strcat(UserPath,"/");
				
				strcat(UserPath,"Library/Application Support/Aperture/Plug-ins/");
				/*char DestApertureAppPath[1024]; 
				 strcpy(DestApertureAppPath,UserPath);
				 strcat(DestApertureAppPath,"BordersAndTitles.ApertureEdit");*/
				
				_Log("Aperture source:")
				_Log(SrcPath)
				_Log("Aperture Dest")
				_Log(UserPath)
				//Create 
				CreateDirectories(UserPath);
				CopyFileSimple(SrcPath, UserPath);
				
				strcat(UserPath,"/MachineWashDeluxe2Aperture.ini");
				_Log("Aperture ini")
				_Log(UserPath)
				FILE *file;
				file = fopen(UserPath,"wb");
				if (file)
				{
					char PluginPath[1024];
					strcpy(PluginPath,_Path);
					int len = strlen(PluginPath);
					if (len>0)
						if ((PluginPath[len-1]!='/')&&(PluginPath[len-1]!='\\'))
							strcat(PluginPath,"/");
					if (_BreakFinding>1)
						strcat(PluginPath, "MisterRetro/");
					
					fwrite(PluginPath,1,strlen(PluginPath),file);
					fclose(file);
				}				
				
			}
			else 
			{
				_Log("ERROR, FSRefMakePath ERROR")
			}//get user folder path
		}//Find user folder ref
		else
		{
			_Log("Cennot to get User Folder Ref")
		}
		//
	}//Aperture
	
	///For iPhoto
	if ((_showiPhotocheckbox)&&(_InstalliPhoto)&&(_BreakCopying!=-1))
	{
		FSRef        userFolderRef;
		char UserPath[1024];
		if (FSFindFolder( kUserDomain, kCurrentUserFolderType, kCreateFolder, &userFolderRef )==noErr)
		{	
			if (FSRefMakePath(&userFolderRef,(UInt8*)UserPath,1024)==noErr)
			{
				char DestPath[1024];
				char SrcPath[1024];
				strcpy(SrcPath, FullPath);
				strcat(SrcPath,"/Contents/Resources/");
				//if ((_UsePPC==1)||(OSXVersion<0x1050)||(_BreakFinding<5))	
				if ((_UsePPC==1))//||(_BreakFinding<5))
				{
					_Log("PPC will be installed")
					strcat(SrcPath,"x32_PPC/");
				}
				else
				{
					_Log("x64 will be installed")
					strcat(SrcPath,"x64/");
				}
				strcat(SrcPath,"iPhoto/");
				
				
				//char SrcApertureAppPath[1024];
				//strcpy(SrcApertureAppPath,SrcPath);
				strcat(SrcPath,"MachineWashDeluxe4iPhoto.iPhotoExporter");
				
				int len = strlen(UserPath);
				if (len>0)
					if ((UserPath[len-1]!='/')&&(UserPath[len-1]!='\\'))
						strcat(UserPath,"/");
				
				strcat(UserPath,"Library/Application Support/iPhoto/Plugins");
				CreateDirectories(UserPath);
				/*char DestApertureAppPath[1024]; 
				 strcpy(DestApertureAppPath,UserPath);
				 strcat(DestApertureAppPath,"BordersAndTitles.ApertureEdit");*/
				
				_Log("iPhoto source:")
				_Log(SrcPath)
				_Log("iPhoto Dest")
				_Log(UserPath)
				CopyFileSimple(SrcPath, UserPath);
				
				strcat(UserPath,"/MachineWashDeluxeiPhoto.ini");
				_Log("iPhoto ini")
				_Log(UserPath)
				FILE *file;
				file = fopen(UserPath,"wb");
				if (file)
				{
					char PluginPath[1024];
					strcpy(PluginPath,_Path);
					int len = strlen(PluginPath);
					if (len>0)
						if ((PluginPath[len-1]!='/')&&(PluginPath[len-1]!='\\'))
							strcat(PluginPath,"/");
					if (_BreakFinding>1)
						strcat(PluginPath, "MisterRetro/");
					
					fwrite(PluginPath,1,strlen(PluginPath),file);
					fclose(file);
				}				
				
			}
			else 
			{
				_Log("ERROR, FSRefMakePath ERROR")
			}//get user folder path
		}//Find user folder ref
		else
		{
			_Log("Cennot to get User Folder Ref")
		}
	}//iPhoto
	
	//]mwd2
	

	
}//SetCopyControls
void SetFindControls(ControlRef PanelCtr)
{
	_Log("SetFindControls")
	Rect rect1,rect2;
	GetControlBounds(PanelCtr,&rect1);
	
	rect1.left -= FRAMELEFT;
	rect1.right -= FRAMELEFT;
	rect1.top -=FRAMETOP;
	rect1.bottom -=FRAMETOP;
	
	rect2.left = rect1.left + 20;
	rect2.top = rect1.top + 50;
	rect2.bottom = rect2.top + 100;
	rect2.right = rect1.right - 20;
	SetControlBounds(FindTextCtr,&rect2);
	
	
	//Progress Stop
	#define FIND_PROGRESS_TOP	150
	GetControlBounds(FindStopCtr,&rect2);
	int StpW = rect2.right - rect2.left;
	int StpH = rect2.bottom - rect2.top;
	
	rect2.left = rect1.left + 20;
	rect2.top = rect1.top + FIND_PROGRESS_TOP;
	rect2.bottom = rect2.top + 25;
	rect2.right = rect1.right - 20 - 10 - StpW;
	SetControlBounds(FindProgressCtr,&rect2);
	
	rect2.left = rect1.right -StpW - 20;
	rect2.top = rect1.top + FIND_PROGRESS_TOP;
	rect2.bottom = rect2.top + StpH;
	rect2.right = rect1.right - 20;
	SetControlBounds(FindStopCtr,&rect2);
	
	#define FIND_INDICT_TOP	FIND_PROGRESS_TOP-30
	rect2.left = rect1.left + 20;
	rect2.top = rect1.top + FIND_INDICT_TOP;
	rect2.bottom = rect2.top + 22;
	rect2.right = rect1.right - 20;
	SetControlBounds(FindIndicCtr,&rect2);
	
	//Div
	#define FIND_PATH_GROUP	200
	#define FIND_PATH_DIV	FIND_PATH_GROUP-20
	
	rect2.left = rect1.left + 20;
	rect2.right = rect1.right - 20;
	rect2.top = rect1.top + FIND_PATH_DIV;
	rect2.bottom = rect1.top+FIND_PATH_DIV+1;
	SetControlBounds(FindDivCtr,&rect2);
	
	//CurHDD
	#define FIND_HDD_TOP  FIND_PATH_GROUP
	#define HDDLW	150
	rect2.left = rect1.left + 20;
	rect2.top = rect1.top + FIND_HDD_TOP;
	rect2.right = rect2.left + HDDLW;
	rect2.bottom = rect2.top + 20;
	SetControlBounds(FindHDDLCtr,&rect2);
	
	rect2.left = rect1.left+25;
	rect2.right = rect1.right-20;
	rect2.top = rect2.top+20;
	rect2.bottom = rect2.bottom+20;
	SetControlBounds(FindHDDCtr,&rect2);
	
	//FindChange
	#define FIND_CHANGEL_TOP FIND_HDD_TOP+55	
	#define FIND_CHANGE_TOP	FIND_CHANGEL_TOP+18
	rect2.left = rect1.left+20;
	rect2.right=rect1.right-20;
	rect2.top = rect1.top+FIND_CHANGEL_TOP;
	rect2.bottom=rect2.top+20;
	SetControlBounds(FindPathLCtr,&rect2);
	
	GetControlBounds(FindChangeCtr,&rect2);
	int CngW = rect2.right-rect2.left;
	int CngH = rect2.bottom-rect2.top;
	rect2.left = rect1.right-CngW-20;
	rect2.top = rect1.top+FIND_CHANGE_TOP;
	rect2.right = rect2.left+CngW;
	rect2.bottom = rect2.top+CngH;
	SetControlBounds(FindChangeCtr,&rect2);
	
	rect2.right = rect2.left - 10;
	rect2.left = rect1.left + 25;
	rect2.top+=5;
	rect2.bottom = rect1.bottom-10;
	/*rect2.top = rect2.bottom+20;
	rect2.left = rect1.left+20;
	rect2.right = rect1.right-20;
	rect2.bottom = rect1.bottom-10;*/
	SetControlBounds(FindPathCtr,&rect2);
	SetControlData(FindPathCtr,0,kControlStaticTextTextTag,strlen(_Path), _Path);

	//[mwd2
	rect2.left = rect1.left + 15;
	rect2.right = rect2.left+130;
	rect2.bottom = rect1.bottom-5;
	rect2.top = rect2.bottom-20;
	SetControlBounds(AddToLightroomCtr,&rect2);
	
	rect2.left = rect2.right + 10;
	rect2.right = rect2.left+130;
	SetControlBounds(AddToApertureCtr,&rect2);
	
	rect2.left = rect2.right + 10;
	rect2.right = rect2.left+130;
	SetControlBounds(AddToiPhotoCtr,&rect2);
	//]mwd2
	
	if (_HDDSelected>=0)
	{
		//[Max MWD2
		if (!_LRwasFound)
		{
			
			if (FindSubfolderByName(_HDDSelected+1,"Applications/","Lightroom",0))
			{				
				_Log("Lightroom is found!!")
				_LRwasFound = 1;
				//Get User Folder
				FSRef        userFolderRef;
				if (FSFindFolder( kUserDomain, kCurrentUserFolderType, kCreateFolder, &userFolderRef )==noErr)
				{
					//char UserPath[1024];
					//FSRefMakePath(&userFolderRef,(UInt8*)UserPath,1024);//Copy user path
					//strcat(UserPath,"")
					if (FindSubfolderByNameRef(&userFolderRef,"/Library/Application Support/Adobe/","Lightroom",0))
					{
						strcpy(_LRScriptPath, _FoundPathSub);
						_showLRcheckbox = 1;
						_Log("User Library LR is found")
						//ShowControl(AddToLightroomCtr);
						EnableControl(AddToLightroomCtr);
						SetControlValue(AddToLightroomCtr, _InstallLightroom);
					}
					else 
					{
						_showLRcheckbox = 0;
						_Log("User Library LR is NOT found")
						//HideControl(AddToLightroomCtr);
						DisableControl(AddToLightroomCtr);
					}
					
				}
				else 
				{
					_Log("cannot return user folder")
					_showLRcheckbox = 0;
				}
			}
			else 
			{
				_Log("LightRoom is not found")
				_LRwasFound = -1;
				
			}
			
		}//find LR
		
		//[Max 2015.06.22
		if (OSXVersion<0x1090)
		{
			if (!_AperturewasFound)
			{
				
				if (FindSubfolderByName(_HDDSelected+1,"Applications/","Aperture",0))
				{				
					_Log("Aperture is found!!")
					_AperturewasFound = 1;
					_showAperturecheckbox = 1;
					//ShowControl(AddToApertureCtr);
					EnableControl(AddToApertureCtr);
					SetControlValue(AddToApertureCtr, _InstallAperture);
					
					
				}
				else 
				{
					_Log("Aperture is not found")
					_AperturewasFound = -1;
					
				}
				
			}//find Aperture
			
			if (!_iPhotowasFound)
			{
				
				if (FindSubfolderByName(_HDDSelected+1,"Applications/","iPhoto",0))
				{				
					_Log("iPhoto is found!!")
					_iPhotowasFound = 1;
					_showiPhotocheckbox = 1;
					//ShowControl(AddToiPhotoCtr);
					EnableControl(AddToiPhotoCtr);
					SetControlValue(AddToiPhotoCtr, _InstalliPhoto);
					
				}
				else 
				{
					_Log("iPhoto is not found")
					_iPhotowasFound = -1;
					
				}
				
			}//find iPhoto
		}//if OSXVersion
		else 
		{
			_InstalliPhoto = 0;
			_InstallAperture = 0;
			HideControl(AddToApertureCtr);
			HideControl(AddToiPhotoCtr);
		}//else OSXVersion
		//]mwd2
		/*_NoCS2 = _NoCS3 = 1;
		_NoCS4 = 1;
		_NoCS5 = _NoCS6 = 0;*/
		_NoCS2 = _NoCS3 = _NoCS4 = _NoCS5 = _NoCS6 = _NoCS7 = _NoCC2014 = _NoCC2015 = _NoCC2016 = 0;
		
		CFStringRef CurHDDName = CFStringCreateCopy(NULL,SelectedPict->HDDName);
		SetControlData(FindHDDCtr,0,kControlEditTextCFStringTag,sizeof(CurHDDName),&CurHDDName);
		
		////[MAX 02.05.12
		//just find CS5 and CS6 firstly
		_BreakFinding = 0;
		SetControlData(FindIndicCtr,0,kControlStaticTextTextTag,strlen(FIND_START), FIND_START);
		
		//[Max 2012.12.11
		if (strcmp(_Path,"/"))
		  _FoundFolder = &_aFoundFolder;
		
		////TEST
	    /*//case IDC_FINDSTOP:
		{
			if (!strcmp(_Path,"/"))
			{
				strcpy(_Path,"/Applications/MisterRetro");
				//FSRefMakePath(_FoundFolder, (UInt8*)_Path, PATHSIZE);
				//strcat(_Path,"/Plug-ins/Filters/");
				SetControlData(FindPathCtr,0,kControlStaticTextTextTag,strlen(_Path),_Path);
				EnableControl(NextCtr);
			}
			_BreakFinding = -1;
			return;
		}//break;*/
		
		_PlugInFound = 0;
		
		//[Max 2016.06.04
		BYTE CC_was_found = 0;
		if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CC 2017",0))
		{
			CC_was_found = 1;
			_PlugInFound = 1;
		}
		if (_PlugInFound<=0)
			if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CC 2015.5",0))
			{
				CC_was_found = 1;
				_PlugInFound = 1;
			}
		if (_PlugInFound<=0)
			if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CC 2015",0))
			{
				CC_was_found = 1;
				_PlugInFound = 1;
			}
		if (_PlugInFound<=0)
			if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CC 2014",0))
			{
				CC_was_found = 1;
				_PlugInFound = 1;
			}
		if (_PlugInFound<=0)
			if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CC",0))
			{
				CC_was_found = 1;
				_PlugInFound = 1;
			}
		
		if (CC_was_found)
		{
			if (FindSubfolderByName(_HDDSelected+1,"Library/","Application Support",0))
			{
				AddSlashIfNeed(_FoundPathSub);
				strcat(_FoundPathSub,"Adobe/Plug-Ins/CC");
				_FoundFolder = &_aFoundFolder;
				FSPathMakeRef((UInt8*)_FoundPathSub,_FoundFolder,NULL);
				strcpy(_Path,_FoundPathSub);
				_BreakFinding = 10;
				CreateDirectories(_Path);
			}
			else 
			{
				_PlugInFound = 0;
			}
		}
		//]2016.06.04
		
		////find CC 2016		
		if (_PlugInFound<=0)
		if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CC 2016",0))
		{
			_PlugInFound = 0;			
			IterateFolder(_FoundFolderSub,1);
			if (_PlugInFound)
			{
				_FoundFolder = &_aFoundFolder;
				FSPathMakeRef((UInt8*)_FoundPathSub,_FoundFolder,NULL);
				strcpy(_Path,_FoundPathSub);
				//_FoundFolder = _FoundFolderSub;
				
				char str[1024];
				strcpy(str,"Adobe Photoshop CC2016 was found \n");
				strcat(str, "Do you want to install the plug-in here?\n\n");
				//[Max 2013.02.22
				//strcat(str,_Path);
				//]
				
				
				
				//if (AddPluginsFilters())
				if (AddPlugins())
				{
					//[Max 2013.02.22
					strcat(str,_Path);
					//]
					
					if(YesNoAlert(str))
					{
						_Log("FindFolder: before MakePath")			
						FSRefMakePath(_FoundFolder, (UInt8*)_Path, PATHSIZE);
						_Log("FindFolder: after MakePath")
						
						_BreakFinding = 10;
						_PlugInFound = 1;
					}//yes
					else
					{
						_NoCC2016 = 1;
						_PlugInFound = 0;
					}//no
				}//AddPluginsFilters
				else
				{
					_Log("Plug-in/Filters folder is not found in CC2016")
					_NoCC2016 = 1;
					_PlugInFound = 0;
					_BreakFinding = 0;   
				} 
			}//if (_PlugInFound)
		}//CC 2016 was found
		
		////find CC 2015		
		if (_PlugInFound<=0)
			if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CC 2015",0))
			{
				_PlugInFound = 0;			
				IterateFolder(_FoundFolderSub,1);
				if (_PlugInFound)
				{
					_FoundFolder = &_aFoundFolder;
					FSPathMakeRef((UInt8*)_FoundPathSub,_FoundFolder,NULL);
					strcpy(_Path,_FoundPathSub);
					//_FoundFolder = _FoundFolderSub;
					
					char str[1024];
					strcpy(str,"Adobe Photoshop CC2015 was found \n");
					strcat(str, "Do you want to install the plug-in here?\n\n");
					//[Max 2013.02.22
					//strcat(str,_Path);
					//]
					
					
					
					//if (AddPluginsFilters())
					if (AddPlugins())
					{
						//[Max 2013.02.22
						strcat(str,_Path);
						//]
						
						if(YesNoAlert(str))
						{
							_Log("FindFolder: before MakePath")			
							FSRefMakePath(_FoundFolder, (UInt8*)_Path, PATHSIZE);
							_Log("FindFolder: after MakePath")
							
							_BreakFinding = 9;
							_PlugInFound = 1;
						}//yes
						else
						{
							_NoCC2015 = 1;
							_PlugInFound = 0;
						}//no
					}//AddPluginsFilters
					else
					{
						_Log("Plug-in/Filters folder is not found in CC2015")
						_NoCC2015 = 1;
						_PlugInFound = 0;
						_BreakFinding = 0;   
					} 
				}//if (_PlugInFound)
			}//CC 2015 was found
		
		////find CC 2014		
		if (_PlugInFound<=0)
		if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CC 2014",0))
			{
				_PlugInFound = 0;			
				IterateFolder(_FoundFolderSub,1);
				if (_PlugInFound)
				{
					_FoundFolder = &_aFoundFolder;
					FSPathMakeRef((UInt8*)_FoundPathSub,_FoundFolder,NULL);
					strcpy(_Path,_FoundPathSub);
					//_FoundFolder = _FoundFolderSub;
					
					char str[1024];
					strcpy(str,"Adobe Photoshop CC 2014 was found \n");
					strcat(str, "Do you want to install the plug-in here?\n\n");
					//[Max 2013.02.22
					//strcat(str,_Path);
					//]
					
					
					
					//if (AddPluginsFilters())
					if (AddPlugins())
					{
						//[Max 2013.02.22
						strcat(str,_Path);
						//]
						
						if(YesNoAlert(str))
						{
							_Log("FindFolder: before MakePath")			
							FSRefMakePath(_FoundFolder, (UInt8*)_Path, PATHSIZE);
							_Log("FindFolder: after MakePath")
							
							_BreakFinding = 8;
							_PlugInFound = 1;
						}//yes
						else
						{
							_NoCC2014 = 1;
							_PlugInFound = 0;
						}//no
					}//AddPluginsFilters
					else
					{
						_Log("Plug-in/Filters folder is not found in CC")
						_NoCC2014 = 1;
						_PlugInFound = 0;
						_BreakFinding = 0;   
					} 
				}//if (_PlugInFound)
			}//CC 2014 was found
		
		//[Max 2013.09.10
		////find CC
		_EndExactly = 1;//for check exactly Photoshop CC but not Photoshop CC 2014
		_Log("before FindSubfolderByName for CS7")
		if (_PlugInFound<=0)
		if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CC",0))
		{
			_PlugInFound = 0;			
			IterateFolder(_FoundFolderSub,1);
			if (_PlugInFound)
			{
				_FoundFolder = &_aFoundFolder;
				FSPathMakeRef((UInt8*)_FoundPathSub,_FoundFolder,NULL);
				strcpy(_Path,_FoundPathSub);
				//_FoundFolder = _FoundFolderSub;
				
				char str[1024];
				strcpy(str,"Adobe Photoshop CC was found \n");
				strcat(str, "Do you want to install the plug-in here?\n\n");
				//[Max 2013.02.22
				//strcat(str,_Path);
				//]
				
				
				
				//if (AddPluginsFilters())
				if (AddPlugins())
				{
					//[Max 2013.02.22
					strcat(str,_Path);
					//]
					
					if(YesNoAlert(str))
					{
						_Log("FindFolder: before MakePath")			
						FSRefMakePath(_FoundFolder, (UInt8*)_Path, PATHSIZE);
						_Log("FindFolder: after MakePath")
						
						_BreakFinding = 7;
						_PlugInFound = 1;
					}//yes
					else
					{
						_NoCS7 = 1;
						_PlugInFound = 0;
					}//no
				}//AddPluginsFilters
				else
				{
					_Log("Plug-in/Filters folder is not found in CC")
					_NoCS7 = 1;
					_PlugInFound = 0;
					_BreakFinding = 0;   
				} 
			}//if (_PlugInFound)
		}//CC was found
		_EndExactly = 0;
		//]for CC
		
		/////// CS6
		if (_PlugInFound<=0)
		if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CS6",0))
		{
			_PlugInFound = 0;			
			IterateFolder(_FoundFolderSub,1);
			if (_PlugInFound)
			{
				_FoundFolder = &_aFoundFolder;
				FSPathMakeRef((UInt8*)_FoundPathSub,_FoundFolder,NULL);
				strcpy(_Path,_FoundPathSub);
				//_FoundFolder = _FoundFolderSub;
				
				char str[1024];
				strcpy(str,"Adobe Photoshop CS6 was found \n");
				strcat(str, "Do you want to install the plug-in here?\n\n");
				//[Max 2013.02.22
				//strcat(str,_Path);
				//]
				
				
				
				//if (AddPluginsFilters())
				if (AddPlugins())
				{
					//[Max 2013.02.22
					strcat(str,_Path);
					//]
					
					if(YesNoAlert(str))
					{
						_Log("FindFolder: before MakePath")			
						FSRefMakePath(_FoundFolder, (UInt8*)_Path, PATHSIZE);
						_Log("FindFolder: after MakePath")
						
						_BreakFinding = 6;
						_PlugInFound = 1;
					}//yes
					else
					{
						_NoCS6 = 1;
						_PlugInFound = 0;
					}//no
				}//AddPluginsFilters
				else
				{
					_Log("Plug-in/Filters folder is not found in CS6")
					_NoCS6 = 1;
					_PlugInFound = 0;
					_BreakFinding = 0;   
				} 
			}//if (_PlugInFound)
		}//cs6 was found
		
		
		/////// CS5
		if (_PlugInFound<=0)
			if (FindSubfolderByName(_HDDSelected+1,"Applications/","Photoshop CS5",0))
			{
				//_FoundFolder = _FoundFolderSub;
				_PlugInFound = 0;			
				IterateFolder(_FoundFolderSub,1);
				if (_PlugInFound)
				{
					_FoundFolder = &_aFoundFolder;
					FSPathMakeRef((UInt8*)_FoundPathSub,_FoundFolder,NULL);
					strcpy(_Path,_FoundPathSub);
					
					
					char str[1024];
					strcpy(str,"Adobe Photoshop CS5 was found \n");
					strcat(str, "Do you want to install the plug-in here?\n\n");
					//[Max 2013.02.22
					//strcat(str,_Path);
					//]
					
					
					if (AddPluginsFilters())
					{
						//[Max 2013.02.22
						strcat(str,_Path);
						//]
						
						if(YesNoAlert(str))
						{
							_Log("FindFolder: before MakePath")			
							FSRefMakePath(_FoundFolder, (UInt8*)_Path, PATHSIZE);
							_Log("FindFolder: after MakePath")
							
							_BreakFinding = 5;
							_PlugInFound = 1;
						}//yes
						else
						{
							_NoCS5 = 1;
							_PlugInFound = 0;
						}//no
					}//AddPluginsFilters
					else
					{
						_Log("Plug-in/Filters folder is not found in CS5")
						_NoCS5 = 1;
						_PlugInFound = 0;
						_BreakFinding = 0;   
					} 
				}//if (_PlugInFound)
			}//cs5 was found
		//]
		
		if (!_PlugInFound)
		{
			_Log("SetFindControls: before FindFolderThreadStart")
			FindFolderThreadStart();
			_Log("SetFindControls: after FindFolderThreadStart")
		}//_PlugInFound
		else 
		{
			
			HideControl(FindStopCtr);
			HideControl(FindProgressCtr);
			EnableControl(FindChangeCtr);
			EnableControl(NextCtr);
			EnableControl(PrevCtr);
			Draw1Control(PrevCtr);
			Draw1Control(NextCtr);
			
			SetControlData(FindIndicCtr,0,kControlStaticTextTextTag,strlen(FIND_COMPLETE),FIND_COMPLETE);
			SetControlData(FindPathCtr,0,kControlStaticTextTextTag,strlen(_Path),_Path);	
			DrawControls(MainDlg);
			
		}		//FindFolder(_HDDSelected+1);
		//FindFolderSincronously();
	}
	
}
void SetHDDControls(ControlRef PanelCtr)
{
	//getfsstat();
	//FSRef **volumeRefsHandle = NULL;
	ItemCount numVolumes;
	if ( FSGetMountedVolumes(&(GenPict->volumeRefsHandle),	&numVolumes)==noErr)
	{
		/*//test
		{
			char str[256];
			sprintf(str,"Num Volumes=%d",numVolumes);
			DebugAlert(str);
		}*/
		
		//Set Scroll size
		Rect rect1,rect2;
		GetControlBounds(PanelCtr,&rect1);
		int Pwidth = rect1.right-rect1.left;
		int Pheight=rect1.bottom-rect1.top;
		#define HDDBORDER	10
		#define HDDBOTTOM	140
		rect2.left = HDDBORDER;
		rect2.top = HDDBORDER+22;
		rect2.right=Pwidth-HDDBORDER;
		rect2.bottom=Pheight - HDDBOTTOM;
		SetControlBounds(HDDScrollCtr,&rect2);
		
		if (GenPict->iControls==NULL)
			GenPict->Init(numVolumes);
		//DisposeHandle((Handle)volumeRefsHandle);
		
		
		//set selected
		rect2.left = rect1.left + 15;
		rect2.top = rect1.bottom - HDDBOTTOM + 15; //rect1.bottom - HDDBOTTOM + 30;
		rect2.right = rect1.right - 15;
		rect2.bottom = rect1.bottom -15;// GenPict->iPictCtr[0].WindowBottom;//rect1.bottom - 30;
		SetControlBounds(HDDSelectedCtr,&rect2);
		/*{
			char str[256];
			//sprintf(str,"left=%d top=%d right=%d bottom=%d",rect1.left, rect1.top,rect1.right, rect1.bottom);
			sprintf(str,"left=%d top=%d right=%d bottom=%d",rect2.left, rect2.top,rect2.right, rect2.bottom);
			DebugAlert(str);
		}*/
		SelectedPict->SetEnable(0);
		SelectedPict->iType = kPictCtrTypeHDD;
		SelectedPict->Init(IDC_HDDSELECTED);
		if (_HDDSelected<0)
		{//Will try to set default
			int i = 0;
			while (i<GenPict->iNControls)
			{
				if (GenPict->iHDDInfo[i].Locked==0)//is not locked
				{
					_HDDSelected = i;
					GenPict->iPictCtr[i].iState=kPickCtrStateChecked;
					break;
				}
				i++;
			}//while
		}//_HDDSelected<0
		
		if (_HDDSelected>=0)
		{
			
			SelectedPict->CopySelected(GenPict->iPictCtr[_HDDSelected],GenPict->iHDDInfo[_HDDSelected]);
			Draw1Control(SelectedPict->selfCtr);			
			EnableControl(NextCtr);
		}//int _HDDSelected>=0
	}
	else
	{
		DebugAlert("Cannot detect volumes");
	}
}//SetHDDControls
void ShowCurPannel()
{
//return;
//DebugAlert("ShowCurPannel");
	
	ControlRef userPaneControl;
	ControlRef selectedPaneControl = NULL;  
	int tabList[] = {STATE_ALL-1, IDD_INTRO, IDD_LICENSE, IDD_CHOOSEHDD, IDD_FIND,IDD_COPY,IDD_FINISH};
	
	//Hide All
		for ( int i=1 ; i<tabList[0]+1 ; i++ ) 
		{
			controlID.id = tabList[i]; 
			GetControlByID( MainDlg, &controlID, &userPaneControl );
			
			if ( i == _State ) 
				selectedPaneControl = userPaneControl; 
			else
			{ 
				/*if (userPaneControl==NULL)
				{
					DebugAlert("userPaneControl=NULL");
				}
				else
				{
					char str[256];
					sprintf(str,"id=%d",controlID.id);
					DebugAlert(str);
				}*/
				SetControlVisibility( userPaneControl, false, true ); 
				//Draw1Control(userPaneControl); 
			}
		}//for 
		
		if ( selectedPaneControl != NULL ) 
		{ 
			//DebugAlert("selecyrt Pane!=NULL");
			(void) ClearKeyboardFocus( MainDlg ); 
			{
					Rect Bounds;
					Bounds.left = FRAMELEFT;
					Bounds.top = FRAMETOP;
					Bounds.right = FRAMEWIDTH;
					Bounds.bottom =FRAMEHEIGHT;
					SetControlBounds(selectedPaneControl,&Bounds);
			}
			SetControlVisibility( selectedPaneControl, true, true ); 
			//Draw1Control(selectedPaneControl); 
		}
		/*if (_State!=STATE_INTRO)
		{
			//DebugAlert("HideControl!");
			SetControlVisibility( IntroPic->selfCtr, false, true ); 
			//HideControl(IntroPic->selfCtr);
		}
		else
		{
			SetControlVisibility( IntroPic->selfCtr, true, true ); 
			//ShowControl(IntroPic->selfCtr);
		}*/
		
	/*switch (_State)
	{
		case STATE_INTRO:
		{
		}break;//STATE_INTRO
	}//switch _State
	*/
	
	switch (_State)
	{
		case STATE_INTRO:
		{
			SetIntroControls(selectedPaneControl);
			//curStyle.font = kControlFontSmallBoldSystemFont ;			
			SetControlFontStyle(LIntroCtr,&ActiveStyle);
			
			SetControlFontStyle(LLicenseCtr,&GrayStyle);
			SetControlFontStyle(LHDDCtr,&GrayStyle);
			SetControlFontStyle(LPlaceCtr,&GrayStyle);
			SetControlFontStyle(LInstallingCtr,&GrayStyle);
			SetControlFontStyle(LFinishCtr,&GrayStyle);
		}break;
		case STATE_LICENSE:
		{
			SetLicenseControls(selectedPaneControl);
			SetControlFontStyle(LIntroCtr,&PassStyle);
			
			SetControlFontStyle(LLicenseCtr,&ActiveStyle);
			SetControlFontStyle(LHDDCtr,&GrayStyle);
			
			SetControlFontStyle(LPlaceCtr,&GrayStyle);
			SetControlFontStyle(LInstallingCtr,&GrayStyle);
			SetControlFontStyle(LFinishCtr,&GrayStyle);
		}break;
		case STATE_CHOOSEHDD:
		{
			SetHDDControls(selectedPaneControl);
			//curStyle.font = kControlFontSmallBoldSystemFont ;			
			SetControlFontStyle(LIntroCtr,&PassStyle);
			
			SetControlFontStyle(LLicenseCtr,&PassStyle);
			SetControlFontStyle(LHDDCtr,&ActiveStyle);
			
			SetControlFontStyle(LPlaceCtr,&GrayStyle);
			SetControlFontStyle(LInstallingCtr,&GrayStyle);
			SetControlFontStyle(LFinishCtr,&GrayStyle);
			
		}break;
		case STATE_FIND:
		{
			SetFindControls(selectedPaneControl);
			//curStyle.font = kControlFontSmallBoldSystemFont ;			
			SetControlFontStyle(LIntroCtr,&PassStyle);
			
			SetControlFontStyle(LLicenseCtr,&PassStyle);
			SetControlFontStyle(LHDDCtr,&PassStyle);			
			
			SetControlFontStyle(LPlaceCtr,&ActiveStyle);
			SetControlFontStyle(LInstallingCtr,&GrayStyle);
			SetControlFontStyle(LFinishCtr,&GrayStyle);
		}break;
		case STATE_COPY:
		{
			
			//curStyle.font = kControlFontSmallBoldSystemFont ;			
			SetControlFontStyle(LIntroCtr,&PassStyle);
			
			SetControlFontStyle(LLicenseCtr,&PassStyle);
			SetControlFontStyle(LHDDCtr,&PassStyle);			
			
			SetControlFontStyle(LPlaceCtr,&PassStyle);
			SetControlFontStyle(LInstallingCtr,&ActiveStyle);
			SetControlFontStyle(LFinishCtr,&GrayStyle);
			//SysBeep(1);
			
			SetCopyControls(selectedPaneControl);
			
		}break;
		case STATE_FINISH:
		{
			SetFinishControls(selectedPaneControl);
			//curStyle.font = kControlFontSmallBoldSystemFont ;			
			SetControlFontStyle(LIntroCtr,&PassStyle);
			
			SetControlFontStyle(LLicenseCtr,&PassStyle);
			SetControlFontStyle(LHDDCtr,&PassStyle);			
			
			SetControlFontStyle(LPlaceCtr,&PassStyle);
			SetControlFontStyle(LInstallingCtr,&PassStyle);
			SetControlFontStyle(LFinishCtr,&ActiveStyle);
			
			//Draw1Control(LInstallingCtr);
			//Draw1Control(LFinishCtr);
			
			//if (_BreakCopying!=0)
			{
				HideControl(PrevCtr);
				HideControl(NextCtr);
				ShowControl(CloseCtr);
			}
			
		}break;
	}//switch state
	SetThemeCursor(kThemeArrowCursor);
}
void PressPrev()
{
	switch (_State)
	{
		case STATE_LICENSE:
		case STATE_INTRO:
		{
		
			_State = STATE_INTRO;
			DisableControl(PrevCtr);
			EnableControl(NextCtr);
			ShowCurPannel();			
		}
		break;//state intro 
		case STATE_CHOOSEHDD:
		{
			_State = STATE_LICENSE;
			/*if (_Agree!=1)
				DisableControl(NextCtr);
			else*/
				EnableControl(NextCtr);
				
			EnableControl(PrevCtr);
			ShowCurPannel();			
		}break;
		case STATE_FIND:
		{
			_State = STATE_CHOOSEHDD;
			EnableControl(PrevCtr);
			if (_HDDSelected>=0)
			{
				EnableControl(NextCtr);
			}
			else
				DisableControl(NextCtr);
			ShowCurPannel();
		}break;
		case STATE_COPY:
		{
			_State = STATE_FIND;
			DisableControl(PrevCtr);
			DisableControl(NextCtr);
			ShowCurPannel();
		}break;
	}//switch _State
}//PressPrev
void PressNext()
{
	switch (_State)
	{
		case STATE_INTRO:
		{
			_State = STATE_LICENSE;
			EnableControl(PrevCtr);
			//if (_Agree==1)
				EnableControl(NextCtr);
			/*else
				DisableControl(NextCtr);*/
			ShowCurPannel();			
		}
		break;//state intro
		case STATE_LICENSE:
		{
		//DebugAlert
			if (!YesNoAlert("To continue installation, your must agree \n\nto the End User License Agreement.\n\n\nPlease click \"Yes\" to continue or \"No\" to cancel."))
				{
					//just return
					break;
				}
			_State = STATE_CHOOSEHDD;
			EnableControl(PrevCtr);
			if (_HDDSelected>=0)
				EnableControl(PrevCtr);
			else
				DisableControl(NextCtr);
			ShowCurPannel();			
		}break; 
		case STATE_CHOOSEHDD:
		{
			_State = STATE_FIND;
			DisableControl(PrevCtr);
			DisableControl(NextCtr);
			ShowCurPannel();
		}break;
		case STATE_FIND:
		{
			_State = STATE_COPY;
			DisableControl(PrevCtr);
			DisableControl(NextCtr);
			ShowCurPannel();
		}break;
	}//switch _State
}//PressNext();

void Char2Str255(char *str, Str255 Str )
{
 int len = strlen(str);
	Str[0] = len;
 memcpy((Str+1),str,len);
}

void DebugAlert(char *str)
{
	//DialogRef theItem; 
	DialogItemIndex itemIndex; 

    Str255 str1,str2;
	Char2Str255("AlphaPlugins Installer",str1);
	Char2Str255(str,str2);
	StandardAlert(kAlertNoteAlert, str1, str2, NULL, &itemIndex);

}

void DebugAlertString( CFStringRef Str)
{
	DialogRef theItem;
	DialogItemIndex itemIndex;
	CreateStandardAlert(kAlertNoteAlert, CFSTR("Installer"), Str, NULL, &theItem);
	RunStandardAlert(theItem, NULL, &itemIndex);
	
}

