//
//  viewController.m
//  TestXNib
//
//  Created by Admin on 22/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import "viewController.h"

#import "introController.h"
#import "AgreementController.h"
#import "finishController.h"

//[MWD2
#include "CustomInfoAboutSets.h"

#import <QuartzCore/QuartzCore.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
//void DebugAlert(char *str);
BOOL checkCC2014(char *path);

void NSDebugAlert(NSString* Mes, NSString *Title, NSAlertStyle style);
extern char _Path[];
extern char SourcePath[];
extern char FullPath[];
extern NSProgressIndicator *progressIndicator;
viewController* _globalViewController;
BYTE _State = STATE_INTRO;

@interface viewController ()
{
    NSViewController *current_controller;
    
    __unsafe_unretained IBOutlet NSTextField *intro_Menu;
    __unsafe_unretained IBOutlet NSTextField *license_Menu;
    __unsafe_unretained IBOutlet NSTextField *destination_Menu;
    __unsafe_unretained IBOutlet NSTextField *findPSD_Menu;
    __unsafe_unretained IBOutlet NSTextField *installing_Menu;
    __unsafe_unretained IBOutlet NSTextField *finish_Menu;
    NSColor *activeColor, *passiveColor, *disableColor;
    //introController *introContr;
    
    NSString *destinationPath;
    int chosen_row;
}
@property (unsafe_unretained) IBOutlet IntroController *introController;
@property (unsafe_unretained) IBOutlet NSView *container_view;
@property (unsafe_unretained) IBOutlet AgreementController *agreementController;
@property (unsafe_unretained) IBOutlet FindPSDController *findPSDController;
@property (unsafe_unretained) IBOutlet CopyingController *installingtController;
@property (unsafe_unretained) IBOutlet FinishController *finishController;
- (IBAction)nextPressed:(id)sender;
- (IBAction)prevPressed:(id)sender;
@property (unsafe_unretained) IBOutlet NSButton *prevButton;
@property (unsafe_unretained) IBOutlet NSButton *nextButton;

@end

@implementation viewController
-(void) awakeFromNib
{
    _Log("awakeFromNib viewController")
     [self viewDidLoad1];//for before OSX10.10
    /*if (self.nibName==nil)
    {
        _Log("nibName=nil!")
    }
    if (self.nibBundle==nil)
    {
        _Log("nibBundle=nil")
    }

    if (self.view==nil)
    {
        _Log("self view = nil")
    }
    */
}
- (void)loadView
{
    _Log("loadView viewController")
   //  [super loadView];
}

-(id) initWithNibName:(NSNibName)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    _Log("initWithNibName viewController")
    return self;
}
- (id)initWithFrame:(NSRect)frameRect
{
    _Log("initWithFrame viewController")
    //[super initWithFrame:frameRect];
    return self;
}
-(id) initWithCoder:(NSCoder *)coder
{
    _Log("initWithCoder viewController")
    self = [super initWithCoder:coder];
    
    _globalViewController = self;
    
    return self;
}
-(id) Init
{
    _Log("Init viewController")
    return self;
}
/*-(void) loadView
{
    _Log("loadView")
    
}*/
- (void)viewDidLoad1 {
   // [super viewDidLoad];
    
    _Log("viewController viewDidLoad");
    [self.view.window setBackgroundColor:[NSColor colorWithDeviceRed:239.0f/255 green:239.0f/255 blue:239.0f/255 alpha:1.0f]];
    // Do view setup here.
    activeColor = [NSColor colorWithDeviceRed:0.0f green:0.0f blue:0.0f alpha:1.0f];
    passiveColor = [NSColor colorWithDeviceRed:0.8f green:0.8f blue:0.0f alpha:1.0f];
    disableColor = [NSColor colorWithDeviceRed:0.36f green:0.36f blue:0.36f alpha:1.0f];
    chosen_row = -1;//what row was choosen in findPSD controller. -1 no, -2 custom
    //introContr = [[IntroController alloc] initWithNibName:@"introController" bundle:nil];
   // [window addSubview:[controller view]];
    
    [self viewWillAppear1];
}
-(void)viewWillAppear1
{
     _Log("viewController viewWillAppear");
     [self ShowCurrentPage];
    // [self BindToViewController:_introController];
    _findPSDController.delegate = self;
    _installingtController.delegate = self;
    
}
-(void)ShowCurrentPage
{
    _Log("ShowCurrentPage>>>")
    switch (_State)
    {
        case STATE_INTRO:
        {
            _Log("State Intro")
            _LogInt("_introController is 0x%x",(long)_introController)
            [self BindToViewController:_introController];
            [_prevButton setEnabled:NO];
            [_nextButton setEnabled:YES];
            
            intro_Menu.textColor = activeColor;
            license_Menu.textColor = passiveColor;
            destination_Menu.textColor = passiveColor;
            findPSD_Menu.textColor = passiveColor;
            installing_Menu.textColor = passiveColor;
            finish_Menu.textColor = passiveColor;
        }break;
        case STATE_LICENSE:
        {
            [self BindToViewController:_agreementController];
            [_prevButton setEnabled:YES];
            [_nextButton setEnabled:YES];
            
            intro_Menu.textColor = disableColor;
            license_Menu.textColor = activeColor;
            destination_Menu.textColor = passiveColor;
            findPSD_Menu.textColor = passiveColor;
            installing_Menu.textColor = passiveColor;
            finish_Menu.textColor = passiveColor;
        }break;
        case STATE_FIND:
        {
            [_prevButton setEnabled:YES];
            [_nextButton setEnabled:NO];
            [_findPSDController lastChoose:chosen_row];
            [_findPSDController  viewWillAppear1];//for before OSX10.10
            [self BindToViewController:_findPSDController];
            
            
            intro_Menu.textColor = disableColor;
            license_Menu.textColor = disableColor;
            destination_Menu.textColor = disableColor;
            findPSD_Menu.textColor = activeColor;
            installing_Menu.textColor = passiveColor;
            finish_Menu.textColor = passiveColor;
        }break;
        case STATE_COPY:
        {
            [_prevButton setEnabled:NO];
            [_nextButton setEnabled:NO];
            _Log("STATE_COPY")
            [_installingtController viewWillAppear1];
            [self BindToViewController:_installingtController];
            _Log("before installingtController viewDidAppear")
            [_installingtController viewDidAppear1];
            
            
            intro_Menu.textColor = disableColor;
            license_Menu.textColor = disableColor;
            destination_Menu.textColor = disableColor;
            findPSD_Menu.textColor = disableColor;
            installing_Menu.textColor = activeColor;
            finish_Menu.textColor = passiveColor;
        }break;
        case STATE_FINISH:
        {
            _Log("STATE_FINISH")
            [_finishController viewWillAppear1];
            [self BindToViewController:_finishController];
            [_prevButton setEnabled:NO];
            [_prevButton setHidden:YES];
            [_nextButton setEnabled:YES];
            [_nextButton setTitle:@"Close"];
            
            intro_Menu.textColor = disableColor;
            license_Menu.textColor = disableColor;
            destination_Menu.textColor = disableColor;
            findPSD_Menu.textColor = disableColor;
            installing_Menu.textColor = disableColor;
            finish_Menu.textColor = activeColor;
        }break;
        default:
        {
            NSLog(@"Undefined state!");
            intro_Menu.textColor = disableColor;
            license_Menu.textColor = disableColor;
            destination_Menu.textColor = disableColor;
            findPSD_Menu.textColor = disableColor;
            installing_Menu.textColor = disableColor;
            finish_Menu.textColor = disableColor;
            [_prevButton setEnabled:YES];
        } break;
    }//switch
}

-(void)RemoveViewController
{
    [current_controller.view removeFromSuperview];
    //if (@available(macOS 10.10, *))
    //{
    //    [current_controller removeFromParentViewController];
    //} else {
        // Fallback on earlier versions
    //}
}
-(void)BindToViewController:(NSViewController*)target_controller
{
    _Log("BindToViewController>>>")
    if (current_controller!=nil)
    {
        _Log("before RemoveViewController")
        [self RemoveViewController];
    }
    if (target_controller==nil)
    {
        _Log("target_controller = nil!!!!")
        return;
    }
 
#ifdef _LOG_
    {
        char str[256];
        sprintf(str,"bounds (%f %f) [ %f %f]",_container_view.bounds.origin.x,_container_view.bounds.origin.y,_container_view.bounds.size.width,_container_view.bounds.size.height);
        _Log(str)
    }
#endif//LOG
    //[self addChildViewController:target_controller];
    target_controller.view.frame = _container_view.bounds;
    //NSLog(@"frame %fx%f bounds %fx%f",_container_view.frame.origin.x,_container_view.frame.origin.y, _container_view.bounds.origin.x,_container_view.bounds.origin.y);
    [_container_view addSubview:target_controller.view];
    current_controller = target_controller;
    _Log("BindToViewController<<<<")
    
}
- (void)sheetDidEndLicense: (NSWindow *)sheet
                     returnCode: (NSInteger)returnCode
                    contextInfo: (void *)contextInfo
{
    if (returnCode == NSAlertDefaultReturn)
    {
        NSLog(@"Yes was pressed!");
        _State = STATE_FIND;
    }
    [self ShowCurrentPage];
}

-(void)ShowLicenseSheet
{
    [_prevButton setEnabled:NO];
    [_nextButton setEnabled:NO];
    
    if (@available(macOS 10.9, *))
    {
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setMessageText:@"To continue installation you must agree to the End User License Agreement."];
        [alert setInformativeText:@"Please click \"Yes\" to contunue or \"No\" to cancel."];
        [alert addButtonWithTitle:@"Yes"];
        [alert addButtonWithTitle:@"No"];
        [alert setAlertStyle:NSWarningAlertStyle];
        
        __block viewController *blocksafeSelf = self;
        
        [alert beginSheetModalForWindow:self.view.window completionHandler: ^(NSModalResponse returnCode)
         {
             if (returnCode == NSAlertSecondButtonReturn)
             {
                 NSLog(@"No was pressed!");
                 //return;
             }
             else
             {
                 NSLog(@"Yes was pressed!");
                 _State = STATE_FIND;
             }
             [blocksafeSelf ShowCurrentPage];
         }];
    }//if (@available(macOS 10.9, *))
    else
    {
        NSBeginAlertSheet(
                          @"To continue installation you must agree to the End User License Agreement.",
                          // sheet message
                          @"Yes",              // default button label
                          nil,                    // no third button
                          @"No",              // other button label
                          self.view.window,                 // window sheet is attached to
                          self,                   // we’ll be our own delegate
                          @selector(sheetDidEndLicense:returnCode:contextInfo:),
                          // did-end selector
                          NULL,                   // no need for did-dismiss selector
                          nil,                 // context info
                          @"Please click \"Yes\" to contunue or \"No\" to cancel.");
        // additional text
    }
}

- (IBAction)nextPressed:(id)sender
{
    _Log("nextPressed")
    switch (_State)
    {
        case STATE_INTRO:
        {
            _State = STATE_LICENSE;
        }break;
        case STATE_LICENSE:
        {
            [self ShowLicenseSheet];
            return;
        }break;
        case STATE_FIND:
        {
            //[MWDVolume
            if( IsUpdatingDBHere(_Path))
            _State = STATE_COPY;
        }break;
        case STATE_FINISH:
        {
            [NSApp terminate: nil];
        }break;
            
        default:
            NSLog(@"Undefined state!");
            break;
    }//switch
    
    [self ShowCurrentPage];
}

- (IBAction)prevPressed:(id)sender
{
    _Log("prevPressed")
    switch (_State)
    {
        case STATE_INTRO:
        {
            NSLog(@"Wrong case");
        }break;
        case STATE_LICENSE:
        {
            _State = STATE_INTRO;
        }break;
        case STATE_FIND:
        {
            _State = STATE_LICENSE;
        }break;
        case STATE_COPY:
        {
            _State = STATE_FIND;
        }break;
        default:
            NSLog(@"Undefined state!");
            break;
    }//switch
    
    [self ShowCurrentPage];
}
#pragma mark -findPSDDelegate
-(void)SetupNextButton:(BOOL)en clicked_row:(int)row
{
    [_nextButton setEnabled:en];
    chosen_row = row;
}
- (unsigned long long int)folderSizeRecurent:(NSString *)folderPath
{
    unsigned long long int fileSize = 0;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSArray *filesArray = [fileManager contentsOfDirectoryAtPath:folderPath error:nil];
    NSEnumerator *filesEnumerator = [filesArray objectEnumerator];
    NSString *fileName;
    while (fileName = [filesEnumerator nextObject])
    {
        NSString * NewName = [folderPath stringByAppendingPathComponent:fileName];
        NSDictionary *attr = [fileManager attributesOfItemAtPath:NewName error:nil];
        if (attr.fileType == NSFileTypeDirectory)
        {
            fileSize += [self folderSizeRecurent:NewName];
        }
        else
        {
            fileSize += attr.fileSize;
        }
        
    }//while
    
    return fileSize;
}
//[Max 2018.06.12
-(BOOL)CreateIfNotExist:(NSString *)Path error:(NSError **)error attributes:(NSDictionary*)attributes
{
    _Log("CreateIfNotExist>>>")
#ifdef _LOG_
    _Log((char*)[Path UTF8String])
#endif//LOG
    NSFileManager *fileManager = [NSFileManager defaultManager];
    //if folder is not exist then create
    if (![fileManager fileExistsAtPath:Path])
    {
        _Log("Not exist will try to create")
        if (! [fileManager createDirectoryAtPath: Path withIntermediateDirectories:NO attributes: attributes error:error] )
        {
#ifdef _LOG_
            {
                char str[256];
                sprintf(str,"Cannot create folder. There are the follow error[%d]:\n%s \n%s",(int)[*error code] ,[[*error localizedDescription] UTF8String], [[*error localizedFailureReason] UTF8String]);
                _Log(str)
            }
#endif//LOG
            NSLog(@"Cannot create dir %@",Path);
            return NO;
        }
    }
    _Log("CreateIfNotExist<<<<")
    return YES;
}
-(BOOL)folderCopyRecurent:(NSString *)folderPath destination:(NSString*)destinationPath copied:(unsigned long long int*)fileSize error:(NSError **)error showProgress:(BOOL)showProgress
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSArray *filesArray = [fileManager contentsOfDirectoryAtPath:folderPath error:nil];
    NSEnumerator *filesEnumerator = [filesArray objectEnumerator];
    NSString *fileName;
    //set 777 attributes
    //[Max 2018.06.12
    //NSDictionary *attributes = [[NSDictionary alloc] init];
    //[attributes setValue:[NSNumber numberWithShort:0777] forKey:NSFilePosixPermissions];
    NSDictionary *attributes =  [NSDictionary dictionaryWithObject:[NSNumber numberWithShort:0777] forKey:NSFilePosixPermissions];
#ifdef _LOG_
    {
        char str[256];
        sprintf(str,"Attribute1 is 0x%x",attributes);
        _Log(str)
    }
#endif//LOG
    
    //NSError *error;
    while (fileName = [filesEnumerator nextObject])
    {
        NSString * NewName = [folderPath stringByAppendingPathComponent:fileName];
        NSString *NewDestinaion = [destinationPath stringByAppendingPathComponent:fileName];
        NSDictionary *attr = [fileManager attributesOfItemAtPath:NewName error:nil];
        if (attr.fileType == NSFileTypeDirectory)
        {
            //if folder is not exist then create
            if (![fileManager fileExistsAtPath:NewDestinaion])
            {
                if (! [fileManager createDirectoryAtPath: NewDestinaion withIntermediateDirectories:NO attributes: nil error:error] )
                {
                    NSLog(@"Cannot create dir %@",NewDestinaion);
                    return NO;
                }
                //set 777 attributes
                [fileManager setAttributes:attributes ofItemAtPath:NewDestinaion error:nil];
            }
            if (![self folderCopyRecurent:NewName destination:NewDestinaion copied:fileSize error:error showProgress:showProgress])
            {
                NSLog(@"error in reurtion");
                return NO;
            }
        }
        else
        {
            *fileSize += attr.fileSize;
            
            //[Max 2018.06.12
            if (showProgress)
            {
                dispatch_async(dispatch_get_main_queue(), ^{
                    
                    ///animated
                    {
                        [progressIndicator.layer removeAllAnimations];
                        CATransition *transition = [CATransition animation];
                        transition.duration = 0.5f;
                        transition.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
                        transition.type = kCATransitionFade;//kCATransitionMoveIn;//kCATransitionPush;//kCATransitionFade;
                        //transition.subtype = kCATransitionFromRight;
                        
                        [progressIndicator.layer addAnimation:transition forKey:nil];
                    }//animation
                    [progressIndicator setDoubleValue:(double)*fileSize];
                });//dispatch_get_main_queue
            }//showProgress
            
            //[Max 2019.06.27
            //force replace! else dont replace :(
            if ([fileManager fileExistsAtPath:NewDestinaion])
            {
                _Log("remove existed item")
                _Log((char*)[NewDestinaion UTF8String])
                [fileManager removeItemAtPath:NewDestinaion error:nil];
            }
            
            if (![fileManager copyItemAtPath:NewName toPath:NewDestinaion error:error])
            {
                NSLog(@"Error copy %@, error:%@",NewName,*error);
                return NO;
            }
            
            //[Max 2018.06.12
            //set 777 attributes
#ifdef _LOG_
            {
                char str[256];
                sprintf(str,"Attribute is 0x%x",attributes);
                _Log(str)
                _Log("NewDestinaion is:")
                _Log((char*)[NewDestinaion UTF8String])
            }
#endif//LOG
            [fileManager setAttributes:attributes ofItemAtPath:NewDestinaion error:nil];//NewDestination is folder
        }
    }//while
    return YES;
}
- (unsigned long long int)folderSize:(NSString *)folderPath
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSArray *filesArray = [fileManager subpathsOfDirectoryAtPath:folderPath error:nil];
    NSEnumerator *filesEnumerator = [filesArray objectEnumerator];
    NSString *fileName;
    unsigned long long int fileSize = 0;
    
    while (fileName = [filesEnumerator nextObject])
    {
        uint64_t size = [[fileManager attributesOfItemAtPath:[folderPath stringByAppendingPathComponent:fileName] error:nil] fileSize];
        fileSize+=size;
        //NSDictionary *fileDictionary = [[NSFileManager defaultManager] fileAttributesAtPath:[folderPath stringByAppendingPathComponent:fileName] traverseLink:YES];
        //fileSize += [fileDictionary fileSize];
    }
    
    return fileSize;
}
#pragma mark -installingDelegate
//[MWD2
int _NeedToCopyTextures = 1;
-(void)ProcessUpdatingsDB
{
    _Log(ProcessUpdatingsDB)
    //[MWD2
    //add MisterRetro end
    char Path1[1024];
    strcpy(Path1,_Path);
    long len = strlen(Path1);
    long len2 = strlen(_StoreFolder);
    if (len>=len2)
        if (strcasecmp(&Path1[len - len2], _StoreFolder)!=0)
            //if (strrstr(_Path,_StoreFolder)==NULL)
        {
            
            if (len>0)
            {
                if (Path1[len-1]!='/')
                    strcat(Path1,"/");
            }
            strcat(Path1,_StoreFolder);
        }
    
    strcat(Path1,"/Machine Wash Deluxe/Updatings.db");
    //strcat(FullPath1,"/Machine Wash Deluxe/Updatings.db");
    _Log(FullPath1)
    
    //if (_Version!=NULL)
    {
        _Log("_Version is presented")
        if(!_Version[0])
            strcpy(_Version,"2.0");
        _NeedToCopyTextures = CreateDBIfNeed(Path1,_Version);
    }
    /*else
    {
        _Log("no _Version")
        _NeedToCopyTextures = CreateDBIfNeed(Path1,"2.0");
    }*/
#ifdef _LOG_
    {
        char str[64];
        sprintf(str,"_NeedToCopyTextures=%d",_NeedToCopyTextures);
        _Log(str)
    }
#endif
    
    //if (_NeedToCopyTextures)
    {
        _Log("_NeedToCopyTextures = 1")
        //AddSetsInstallInfo( Path1, "39276", "Default Textures",1);
        //AddSetsInstallInfo( Path1, "29486", "Texture Set One",1);
        //AddSetsInstallInfo( Path1, "59321", "Texture Set Two",1);
        AddSetsInstallInfo( Path1, "10734", "Texture Set Three",1);
        //AddSetsInstallInfo( Path1, "82349", "Texture Set Four",1);
        //AddSetsInstallInfo( Path1, "51203", "Texture Set Five",2);
    }
    _Log("after AddSetsInstallInfo")
}
-(void)StartCopying
{
    _Log(">>>>>>>>>>>>>>Start Copying....")
    [_nextButton setEnabled:NO];
    [_prevButton setEnabled:NO];
    
    //[progressIndicator setUsesThreadedAnimation:YES];
    //[progressIndicator startAnimation:nil];
    
    NSString *sourcePath = [NSString stringWithCString:SourcePath encoding:NSUTF8StringEncoding];
    
    //add MisterRetro end
    char Path1[1024];
    strcpy(Path1,_Path);
    long len = strlen(Path1);
    long len2 = strlen(_StoreFolder);
    if (len>=len2)
        if (strcasecmp(&Path1[len - len2], _StoreFolder)!=0)
            //if (strrstr(_Path,_StoreFolder)==NULL)
        {
            
            if (len>0)
            {
                if (Path1[len-1]!='/')
                    strcat(Path1,"/");
            }
            strcat(Path1,_StoreFolder);
        }
    
    
    destinationPath = [NSString stringWithCString:Path1 encoding:NSUTF8StringEncoding];
    
    //[Max 2018.06.12
    NSDictionary *attributes =  [NSDictionary dictionaryWithObject:[NSNumber numberWithShort:0777] forKey:NSFilePosixPermissions];
    
    NSError *error;
    _Log("Check MisterRetro folder exist")
    if (![self CreateIfNotExist:destinationPath error:&error attributes:attributes])
    {
        [self EndCopying:NO error:error];
        return;
    }
    
    //[MWD2
    _Log("before ProcessUpdatingsDB")
    //create /Machine Wash Deluxe subfolder
    {
        char tmpPath[1024];
        strcpy(tmpPath,Path1);
        strcat(tmpPath,"/Machine Wash Deluxe");
        
        //it should exist but for any case
        if (![self CreateIfNotExist:[NSString stringWithCString:tmpPath encoding:NSUTF8StringEncoding] error:&error attributes:attributes])
        {
            [self EndCopying:NO error:error];
            return;
        }
    }
    [self ProcessUpdatingsDB];
    _Log("after ProcessUpdatingsDB")
    
    
    //make source paths
   /* char pluginSource[1024];
    
    strcpy(pluginSource,FullPath);
    strcat(pluginSource,"/Contents/Resources/");
    strcat(pluginSource, _StoreFolder);
    */
    _Log("destination _Path is")
    _Log(_Path)
    _Log("SourcePath is")
    _Log(SourcePath)
    
    //uint64_t tmp = [self folderSizeRecurent:sourcePath];
    //uint64_t folderSize = [self folderSize:sourcePath];
    uint64_t folderSize = [self folderSizeRecurent:sourcePath];
    NSLog(@"folder size = %llu",folderSize);
    _LogInt("folder size is %d",folderSize)
    
    
    [progressIndicator setMaxValue:(double)(folderSize*1.05)];
    [progressIndicator setDoubleValue:0.0];
    
    //set 777 attributes
    //[Max 2018.06.12
    //NSDictionary *attributes;
    //[attributes setValue:[NSNumber numberWithShort:0777] forKey:NSFilePosixPermissions];
    
    ///Copying Plug-in
    _Log("before copying plug-in")
    NSFileManager *fileManager = [NSFileManager defaultManager];
    fileManager.delegate = self;
    
    //set 777 attributes
    [fileManager setAttributes:attributes ofItemAtPath:destinationPath error:nil];
    
    //[Max 2018.06.12
    __block unsigned long long int curFileSize = 0;
    /*if(![self folderCopyRecurent:[NSString stringWithCString:pluginSource encoding:NSUTF8StringEncoding] destination:destinationPath copied:&curFileSize error:&error showProgress:NO])
    {
        [self EndCopying:NO error:error];
        return;
    }*/
    
   // [progressIndicator setDoubleValue:(double)pluginSize];
   // [progressIndicator setNeedsDisplay:YES];
    
    
    //timer = [NSTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(TimerProc) userInfo:nil repeats:YES];
    
    
    dispatch_queue_t previewQueue = dispatch_queue_create("copying queue", NULL);
    
    dispatch_async(previewQueue,
                   ^{
                       
                       NSError *error;
                       //NSDictionary *sourceAttributes = [fileManager fileAttributesAtPath:sourcePath traverseLink:YES];
                       //NSNumber *sourceFileSize;
                       
                       
                       
                       BOOL success;
                       //if ([fileManager copyItemAtPath:sourcePath toPath:destinationPath error:&error])
                       curFileSize = 0;//(pluginSize);//0;
                       _Log("before folderCopyRecurent")
                       if( [self folderCopyRecurent:sourcePath destination:destinationPath copied:&curFileSize error:&error showProgress:YES])
                       {
                           success = YES;
                           NSLog(@"Copy Success");
                       }
                       else
                       {
                           success = NO;
                       }
                       
                       
                       dispatch_async(dispatch_get_main_queue(), ^{
                           //[progressIndicator stopAnimation:nil];
                           [self EndCopying:success error:error];
                       });//dispatch_get_main_queue
                       
                   });//dispatch_async
    
}
-(void)EndCopying:(BOOL)Success error:(NSError*)error
{
    //[timer invalidate];
    //timer = nil;
    [progressIndicator setDoubleValue:0.0];
    
    if (Success)
    {
        _State = STATE_FINISH;
        [self ShowCurrentPage];
    }
    else
    {
        NSLog(@"Error copying: %@",error);
        [_prevButton setEnabled:YES];
        
        NSLog(@"Copy error: %@", error);
        NSString *nserror = [NSString stringWithFormat:@"Cannot install files. There are the follow error:\n%@",error];
        //char str[256];
        //sprintf(str,"Cannot install files. There are the follow error[%d]:\n%s \n%s",(int)[error code] ,[[error localizedDescription] UTF8String], [[error localizedFailureReason] UTF8String]);
        //DebugAlert(str);
        //DebugAlert((char*)[nserror UTF8String]);
        NSDebugAlert(nserror, @"Installer Error", NSAlertStyleCritical);//NSWarningAlertStyle);
    }
    
    
}
#pragma mark -NSFileManagerDelegate
- (BOOL)fileManager:(NSFileManager *)fileManager shouldCopyItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath
{
    _Log("shouldCopyItemAtPath >>>>")
#ifdef _LOG_
    char str[256];
    sprintf(str,"srcPath \n%s",(char*)[srcPath UTF8String]);
    _Log(str)
    sprintf(str,"dstPath \n%s",(char*)[dstPath UTF8String]);
    _Log(str)
#endif//LOG
    return YES;
}
- (BOOL)fileManager:(NSFileManager *)fileManager shouldProceedAfterError:(NSError *)error copyingItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath
{
    _Log("fileManager shouldProceedAfterError>>>")
    return YES;
}

@end

void NSDebugAlert(NSString* Mes, NSString *Title, NSAlertStyle style)
{
    NSAlert *alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText:Title];
    [alert setInformativeText:Mes];//[NSString stringWithUTF8String:str]];
    [alert setAlertStyle:style];//NSWarningAlertStyle];
    
    [alert runModal];
    //[alert release];
}
BOOL checkCC2014(char *Path)
{
    
    if (strlen(Path)<=0)
        return NO;
    
    char *CC = strstr(Path,"CC");
    //[Max 2020.01.12
    if (!CC)
    {
        CC = strstr(Path,"Photoshop 20");
        if (CC)
            return YES;
        
        return NO;
    }
    //]
    CC+=2;
    SeekOnStartStr(CC);
    int N = 0;
    StrGetInt(CC,  &N);
    if (N>=2014)
        return YES;
    
    return NO;
}

