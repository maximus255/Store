/*
 *  UI.h
 *  
 *
 *  Created by Maxim Chernousov on 10/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <Carbon/Carbon.h>
//#include "PictControl.h"


#ifndef _INSTALLERUI
#define _INSTALLERUI

#define BACKGROUND_RED 255
#define BACKGROUND_GREEN 255
#define BACKGROUND_BLUE 255

#define BACKGROUND_EULA_RED 238.0f/255
#define BACKGROUND_EULA_GREEN	238.0f/255
#define BACKGROUND_EULA_BLUE 238.0f/255

#define BACKGROUND_DISK_RED 246.0f/255
#define BACKGROUND_DISK_GREEN	246.0f/255
#define BACKGROUND_DISK_BLUE 246.0f/255
//window width 616
//windw height 422
//split left 166
#define WINDOW_WIDTH 616
#define WINDOW_HEIGHT 422	
#define SPLITLEFT	166

#define FRAMELEFT	181 //200 //197
#define FRAMETOP	17 //15
#define FRAMEWIDTH	598+1 // 450
#define FRAMEHEIGHT	347+1 //400

#define BYTE	unsigned char
#define sign	'inst'

#define IDC_BANNERLEFT	9
#define IDC_NEXT	10
#define IDC_PREV	11
#define IDC_CLOSE	8
#define IDC_NAME	7

#define IDD_INTRO	12
#define IDD_CHOOSEHDD	13
#define IDD_LICENSE		14
#define IDD_FIND		15
#define IDD_COPY		16
#define IDD_FINISH		17


#define IDC_INTRO_PIC	1201
#define IDC_INTROVERS	1202
#define IDC_INTROVERSL	1203
#define IDC_INTROCOPY	1204
#define IDC_INTROINTRO	1205

#define IDC_LICENSETXT		  1403
#define IDC_LICENSETXTV		  1404
#define IDC_LICENSEYES		  1401
#define IDC_LICENSENO		  1402
#define IDC_LICENSEBORDER	  1405

#define IDC_HDDSCROLL		1301
#define IDC_HDDVIEW			1302
#define IDC_HDDITEMS		1304
#define IDC_HDDSELECTED		1399

#define IDC_FINDTEXT		1501
#define IDC_FINDPROGRESS	1502
#define IDC_FINDINDIC		1503
#define IDC_FINDSTOP		1504
#define IDC_FINDPATH		1505
#define IDC_FINDCHANGE		1506
#define IDC_FINDDIV			1507
#define IDC_FINDHDDL		1509
#define IDC_FINDHDD			1510
#define IDC_FINDPATHL		1511
//[MWD2
#define IDC_ADDTOLIGHTROOM		1512
#define IDC_ADDTOAPERTURE		1513
#define IDC_ADDTOIPHOTO		1514
//]MWD2

#define IDC_COPYINTRO		1601
#define IDC_COPYCURRENT		1602
#define IDC_COPYPROGRESS	1603
#define IDC_COPYSTOP		1604

#define IDC_FINISHPIC		1701
#define IDC_FINISHCONG		1702
#define IDC_FINISHPATHL		1703
#define IDC_FINISHPATH		1704

#define IDC_LINTRO		101
#define IDC_LLICENSE	102
#define IDC_LHDD		103
#define IDC_LPLACE		104
#define IDC_LINSTALLING	105
#define IDC_LFINISH		106

enum{
	STATE_INTRO = 1,
	STATE_LICENSE,
	STATE_CHOOSEHDD,
	STATE_FIND,
	STATE_COPY,
	STATE_FINISH,
	STATE_ALL
};

extern BYTE _State;
extern char _Agree;
extern int _HDDSelected;
extern WindowRef MainDlg;

extern ControlRef FinishCongCtr,FinishPathLCtr,FinishPathCtr;
extern ControlRef IntroVersLCtr,IntroVersCtr,IntroCopyrightCtr,IntroIntroCtr;
extern ControlRef CopyIntroCtr,CopyCurrentCtr,CopyProgressCtr,CopyStopCtr;

extern ControlRef NextCtr,PrevCtr,CloseCtr;
extern ControlID controlID; 
extern ControlRef LIntroCtr,LLicenseCtr,LHDDCtr,LPlaceCtr,LInstallingCtr,LFinishCtr;

extern ControlRef LicenseYesCtr,LicenseNoCtr,LicenseTxtCtr,LicenseTxtVCtr,LicenseBorderCtr;
extern ControlRef HDDScrollCtr,HDDViewCtr,HDDSelectedCtr;

extern ControlRef FindTextCtr,FindProgressCtr,FindIndicCtr,FindStopCtr,FindPathCtr,FindChangeCtr,FindDivCtr,FindHDDLCtr,FindHDDCtr,FindPathLCtr
//[mwd2
,AddToLightroomCtr,AddToApertureCtr,AddToiPhotoCtr;
//]mwd2

extern ControlFontStyleRec ActiveStyle,GrayStyle,PassStyle;
extern UInt64	_NeededSpace;
extern char _Version[];
extern char _Copyright[],_Intro[],_CopyIntro[],_StoreFolder[],_Congratulation[];

void DebugAlert(char *str);
void ShowCurPannel();
void PressNext();
void PressPrev();
void CheckHDDPress(int ID);

void CreateDirectories(char *aDstPath);

#endif//INSTALLERUI

