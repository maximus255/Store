//
//  main.m
//  LightLeakInstaller
//
//  Created by Admin on 15/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import <Cocoa/Cocoa.h>
/*extern "C"
 {
 #import <Foundation/Foundation.h>
 #import <AppKit/AppKit.h>
 }*/

#include "log.h"
#import <Security/Authorization.h>
#import <Security/AuthorizationDB.h>
#include <sys/stat.h>
UInt64 _NeededSpace = 7000000;
char _Version[64];
char _Copyright[256];
char _Intro[1024];
char _CopyIntro[1024];
char _StoreFolder[64];
extern char SourcePath[];
char _Congratulation[512];
SInt32 OSXVersion = 0x1000;
char _UsePPC = -1;
CFBundleRef widgetBundle;
void DebugAlert(char *str);
int YesNoAlert(char *msg);
int RepeatCancelAlert(char *msg);
extern char FullPath[1024],ScriptsPath[1024];
//extern FSRef _aSourceFolder;
//extern FSRef *_SourceFolder;//[Max 2012.12.11
void ParseConfig();
bool GetPaths()
{
    widgetBundle = CFBundleGetMainBundle();
    //widgetBundle = CFBundleGetBundleWithIdentifier(CFSTR("com.AlphaPlugins.Engraver2Installe"));
    
    if (widgetBundle==NULL)
    {
        DebugAlert("Error get Bundle with Identifier");
        //UnregisterToolboxObjectClass(PreviewControl);
        return false;
    }
    
    CFURLRef BundleURL = CFBundleCopyBundleURL(widgetBundle);
    Boolean succ = CFURLGetFileSystemRepresentation(BundleURL,TRUE,(UInt8*)FullPath,1024);
    //        _Log("After CFURLGetFileSystemRepresentation");
    if (!succ)
    {
        DebugAlert("Error convert bundle to FS Path!");
        return false;
    }
    //DebugAlert(PluginPath);
    CFRelease(BundleURL);
    strcpy(ScriptsPath,FullPath);
    strcat(ScriptsPath,"/Contents/Resources/Scripts/");
    ParseConfig();
    
    //char SourcePath[1024];
    strcpy(SourcePath,FullPath);
    strcat(SourcePath,"/Contents/Resources/");
    strcat(SourcePath,_StoreFolder);
    //if (SourcePath[strlen(SourcePath)-1]!='/')
    //    strcat(SourcePath,"/");
    
   
    return true;
}

int CheckIsLaunched()
{
    /*widgetBundle = CFBundleGetBundleWithIdentifier(CFSTR("adobefx.com.mashinewashdelux"));
     if (widgetBundle==NULL)
     return 0;*/
    
    ProcessSerialNumber psn = { kNoProcess, kNoProcess };
    //CFStringRef procname;
    while (GetNextProcess(&psn) == noErr)
    {
        CFDictionaryRef cfDict = ProcessInformationCopyDictionary(&psn,  kProcessDictionaryIncludeAllInformationMask);
        if (cfDict)
        {
            
            CFStringRef procname = (CFStringRef)CFDictionaryGetValue(cfDict, (kCFBundleNameKey) );
            
            int res = CFStringCompare(procname,CFSTR("Photoshop"),kCFCompareCaseInsensitive);
            
            //CFRelease(procname);
            CFRelease(cfDict);
            
            if (res==0)
                return 1;
        }
    }
    
    
    return 0;
}

int WaitUntilLaunched()
{
    int res = 0,choose = 0;
    do
    {
        res = CheckIsLaunched();
        if (res==1)
        {
            choose = RepeatCancelAlert("Please quit Photoshop before continuing");
            if (choose==0)
                choose = 2;
        }
    }while((res!=0)&&(choose!=2));
    
    if ((res!=0)&&(choose==2))
        return 1;
    
    return 0;
    
}
int main(int argc, const char * argv[])
{
#ifdef _LOG_
    {
        _Log("Print argv")
        char str[256];
        for (int i=0;i<argc;i++)
        {
            sprintf(str,"argv[%d]=%s",i,argv[i]);
            _Log(str)
        }//for
    }
#endif
    
   // #define NOAUTHORIZE
#ifndef NOAUTHORIZE
    //check --self-repair
    Boolean NeedAuthorize = 1;
    
    for (int i=0;i<argc;i++)
    {
        if (!strcmp(argv[i],"--self-repair"))
        {
            _Log("--self-repair found!")
            NeedAuthorize = 0;
            break;
        }
    }
    
    //if (0)//test
    if (NeedAuthorize)
    {
        _Log("before AuthorizationCreate")
        AuthorizationRef authorization;
        OSStatus status = AuthorizationCreate (NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &authorization);
        if (status != 0) {
            _Log("Error creating authorization reference")
            return -1;
        }
        //#define kAuthorizationRightExecute "system.privilege.admin"
        //AuthorizationItem right = { "system.privilege.adminRights", 0, 0 , 0 };
        //char rightName[] = "system.install.root.user";
        AuthorizationItem right = { kAuthorizationRightExecute, 0, 0 , 0 };
        AuthorizationItem items[] = { right };
        AuthorizationRights rights = { sizeof(items) / sizeof(items[0]), items };
        AuthorizationFlags flags = kAuthorizationFlagInteractionAllowed | kAuthorizationFlagExtendRights | kAuthorizationFlagPreAuthorize;
        
        _Log("before AuthorizationCopyRights")
        status = AuthorizationCopyRights (authorization, &rights, kAuthorizationEmptyEnvironment, flags, NULL);
        if (status != 0) {
            _Log( "Error authorizing current process with right to call task_for_pid")
            return -1;
        }
        
        /////
        _Log("rights accepted!!!!")
        
        //will launch it again
        char *arguments[] = { "--self-repair", NULL };
        FILE *pipe = NULL;
        
        status = AuthorizationExecuteWithPrivileges(authorization, argv[0],
                                                    kAuthorizationFlagDefaults, arguments, &pipe);
        if (status != errAuthorizationSuccess)
        {
            _Log("AuthorizationExecuteWithPrivileges error")
        }
        else
        {
            status = AuthorizationFree(authorization, kAuthorizationFlagDestroyRights);
            _Log("return from main")
            return 0;
        }
        
        
    }//argc<2
    
    _Log("NeedAuthorize=0")
#endif//NOAUTHORIZE
    
    umask(0);
    
    
    
    //[Max 2015.08.25
    ProcessSerialNumber PSN;
    GetCurrentProcess(&PSN);
    SetFrontProcess(&PSN);
    
    ///[Max 2013.03.05 check Photoshop
    int res = WaitUntilLaunched();
    if (res)
    {
        _Log("return because YesNo Photoshop return 0")
        return 0;
    }
    //]
    
    Gestalt(gestaltSystemVersion,&OSXVersion);
    
#ifdef _LOG_
    {
        char str[256];
        sprintf(str,"OSX Version=0x%x",OSXVersion);
        //DebugAlert(str);
        _Log(str)
    }
#endif//LOg
    
    if (!GetPaths())
    {
        _Log("GetPaths false")
        return false;
    }
    
    _Log("before load XIB")
    /*if (![NSBundle loadNibNamed:@"MainMenu" owner:nil])
    {
        _Log("cannot loafNibName!")
    }*/
    _Log("before NSApplicationMain")
    return NSApplicationMain(argc, argv);
}
void DebugAlert(char *str)
{
    NSAlert *alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText:@"Installer Alert"];
    [alert setInformativeText:[NSString stringWithUTF8String:str]];
    [alert setAlertStyle:NSWarningAlertStyle];
    
    [alert runModal];
    //[alert release];
}
int YesNoAlert(char *msg)
{
    _Log("yesnoalert x86_64")
    NSAlert *alert = [[NSAlert alloc] init];
#ifdef _LOG_
    {
        if (!alert)
        {
            _Log("ERROR, cannot init NSAlert")
        }
    }
#endif
    [alert addButtonWithTitle:@"Yes"];
    [alert addButtonWithTitle:@"No"];
    [alert setMessageText:@"Plugin Alert"];
    
    [alert setInformativeText:[NSString stringWithFormat:@"%s", msg]];
    
    [alert setAlertStyle:NSInformationalAlertStyle];
    
    _Log("before alert runModal")
    NSInteger resBool = [alert runModal];
    _Log("after alert run Modal")
    //Boolean res = (resBool==1000);
    /*Boolean res;
     if (resBool==1000)
     res = YES;
     else
     res = NO;
     
     */
    
    int res = (resBool==1000);
    
    //[alert release];
    return res;
}

bool is_cocoa_initialized = NO;
int RepeatCancelAlert(char *msg)
{
    _Log("yesnoalert x86_64")
    NSAlert *alert = [[NSAlert alloc] init];
#ifdef _LOG_
    {
        if (!alert)
        {
            _Log("ERROR, cannot init NSAlert")
        }
    }
#endif
    _Log("before NSApplicationLoad()")
    if (!is_cocoa_initialized)
    {
        is_cocoa_initialized = YES;
        NSApplicationLoad();
    }
    [alert addButtonWithTitle:@"Continue"];
    [alert addButtonWithTitle:@"Cancel"];
    [alert setMessageText:@"Plugin Alert"];
    
    [alert setInformativeText:[NSString stringWithFormat:@"%s", msg]];
    
    [alert setAlertStyle:NSInformationalAlertStyle];
    
    _Log("before alert runModal")
    NSInteger resBool = [alert runModal];
    _Log("after alert run Modal")
    //Boolean res = (resBool==1000);
    /*Boolean res;
     if (resBool==1000)
     res = YES;
     else
     res = NO;
     
     */
    
    int res = (resBool==1000);
    
    //[alert release];
    return res;
}
