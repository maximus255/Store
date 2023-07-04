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

#import <QuartzCore/QuartzCore.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
//void DebugAlert(char *str);
BOOL checkCC2014(char *path);
//[Max 2021.06.18
int OSXVersionBig=0;//for OSX version bigger 10.9
void Gestalt_my();
//setup readwrite permissions
void SetPermissions(NSString* destinationPath);

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
    //[Max 2021.06.07
    Gestalt_my();
    
    [self.view.window setBackgroundColor:[NSColor colorWithDeviceRed:239.0f/255 green:239.0f/255 blue:239.0f/255 alpha:1.0f]];
    
    _Log("viewController viewDidLoad");
    // Do view setup here.
    activeColor = [NSColor colorWithDeviceRed:0.25f green:0.65f blue:0.25f alpha:1.0f];
    passiveColor = [NSColor colorWithDeviceRed:0.35f green:0.35f blue:0.35f alpha:1.0f];
    disableColor = [NSColor colorWithDeviceRed:0.6f green:0.6f blue:0.6f alpha:1.0f];
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
    
    //[Max 2021.06.18
    NSDictionary *attributes =  [NSDictionary dictionaryWithObject:[NSNumber numberWithShort:0777] forKey:NSFilePosixPermissions];
    char PathRoot[1024];
    strcpy(PathRoot,Path1);
    PathRoot[strlen(Path1) - len2] = 0;
    long len3 = strlen(PathRoot);
    if ((PathRoot[len3-1]=='/')||(PathRoot[len3-1]=='\\'))
        PathRoot[len3-1]=0;
    NSString * PRoot = [NSString stringWithCString:PathRoot encoding:NSUTF8StringEncoding];
    {
        NSFileManager *fileManager = [NSFileManager defaultManager];
        fileManager.delegate = self;
        
        //set 777 attributes
        [fileManager setAttributes:attributes ofItemAtPath:PRoot error:nil];
        
    }
    
    destinationPath = [NSString stringWithCString:Path1 encoding:NSUTF8StringEncoding];
    
    //[Max 2021.06.18
    //NSDictionary *attributes =  [NSDictionary dictionaryWithObject:[NSNumber numberWithShort:0777] forKey:NSFilePosixPermissions];
    
    NSError *error;
    _Log("Check MisterRetro folder exist")
    if (![self CreateIfNotExist:destinationPath error:&error attributes:attributes])
    {
        [self EndCopying:NO error:error];
        return;
    }
    
    //check CC214 and later or not
    BOOL isCC2014;
    _Log("before GenerateCommonPluginPath")
    NSString *commonPath = GenerateCommonPluginPath();
    if ([destinationPath rangeOfString:commonPath].location != NSNotFound)
        //if ([sourcePath containsString:commonPath])//for 10.8 and later
    {
        isCC2014 = YES;
    }
    else
        isCC2014 = checkCC2014(_Path);
    
    //make source paths
    char pluginSource[1024];
#ifdef _AUTOMATION_
    char automationSource[1024];
#endif//_AUTOMATION_
    strcpy(pluginSource,FullPath);
    strcat(pluginSource,"/Contents/Resources/");
    
    //[Max 2021.06.18
    bool _BigSur = false;
    if (OSXVersionBig>=101300)//10.13
        _BigSur = true;
    
    if (_BigSur)
        strcat(pluginSource,"M1/");
    else
        strcat(pluginSource,"CC/");
    
//    if (isCC2014)
//        strcat(pluginSource,"CC/");
//    else
//        strcat(pluginSource,"CS5/");
    
    strcat(pluginSource, _StoreFolder);
    
#ifdef _AUTOMATION_
    strcpy(automationSource,FullPath);
    //[Max 2021.06.18
    if (_BigSur)
        strcat(automationSource,"/Contents/Resources/AutomationM1/");
    else
        strcat(automationSource,"/Contents/Resources/AutomationCS5/");
    strcat(automationSource, _StoreFolder);
#endif//_AUTOMATION_
    
    _Log("pluginSource is")
    _Log(pluginSource)
    
#ifdef _AUTOMATION_
    _Log("automationSource is")
    _Log(automationSource)
#endif//_AUTOMATION_
    _Log("destination _Path is")
    _Log(_Path)
    _Log("SourcePath is")
    _Log(SourcePath)
    
    //calculate plugins sizes
    uint64_t pluginSize = [self folderSizeRecurent:[NSString stringWithCString:pluginSource encoding:NSUTF8StringEncoding]];
#ifdef _AUTOMATION_
    uint64_t automationSize = [self folderSizeRecurent:[NSString stringWithCString:automationSource encoding:NSUTF8StringEncoding]];
#endif//_AUTOMATION_
    //uint64_t tmp = [self folderSizeRecurent:sourcePath];
    //uint64_t folderSize = [self folderSize:sourcePath];
    uint64_t folderSize = [self folderSizeRecurent:sourcePath];
    NSLog(@"folder size = %llu",folderSize);
    _LogInt("folder size is %d",folderSize)
    
    
    //NSFileManager *fileManager = [NSFileManager defaultManager];
    //uint64_t fileSize = [[fileManager attributesOfItemAtPath:sourcePath error:nil] fileSize];
#ifdef _AUTOMATION_
    [progressIndicator setMaxValue:(double)(folderSize*1.05 + pluginSize + automationSize)];
#else
    [progressIndicator setMaxValue:(double)(folderSize*1.05 + pluginSize )];
#endif//_AUTOMATION_
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
    
    //[Max 2021.06.18
    [fileManager setAttributes:attributes ofItemAtPath:PRoot error:nil];
    
    //[Max 2018.06.12
    __block unsigned long long int curFileSize = 0;
    if(![self folderCopyRecurent:[NSString stringWithCString:pluginSource encoding:NSUTF8StringEncoding] destination:destinationPath copied:&curFileSize error:&error showProgress:NO])
   // if (![fileManager copyItemAtPath:[NSString stringWithCString:pluginSource encoding:NSUTF8StringEncoding] toPath:destinationPath error:&error])
    {
        [self EndCopying:NO error:error];
        return;
    }
    
    //[Max 2018.06.12
    //set 777 attributes
    //[fileManager setAttributes:attributes ofItemAtPath:destinationPath error:nil];
    
   
    
    [progressIndicator setDoubleValue:(double)pluginSize];
    [progressIndicator setNeedsDisplay:YES];
    
#ifdef _AUTOMATION_
    ///Copying Automation
    _Log("before copying automation")
    //[Max 2018.06.12
    //if (![fileManager copyItemAtPath:[NSString stringWithCString:automationSource encoding:NSUTF8StringEncoding] toPath:destinationPath error:&error])
    if(![self folderCopyRecurent:[NSString stringWithCString:automationSource encoding:NSUTF8StringEncoding] destination:destinationPath copied:&curFileSize error:&error showProgress:NO])
    {
        [self EndCopying:NO error:error];
        return;
    }
    [progressIndicator setDoubleValue:(double)(pluginSize+automationSize)];
    [progressIndicator setNeedsDisplay:YES];
    _Log("after copying automation")
#endif//_AUTOMATION_
    
    //timer = [NSTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(TimerProc) userInfo:nil repeats:YES];
    
    dispatch_queue_t previewQueue = dispatch_queue_create("copying queue", NULL);
    
    dispatch_async(previewQueue,
                   ^{
                       
                       NSError *error;
                       //NSDictionary *sourceAttributes = [fileManager fileAttributesAtPath:sourcePath traverseLink:YES];
                       //NSNumber *sourceFileSize;
                       
                       
                       
                       BOOL success;
                       //if ([fileManager copyItemAtPath:sourcePath toPath:destinationPath error:&error])
#ifdef _AUTOMATION_
                       curFileSize = (pluginSize+automationSize);//0;
#else
                       curFileSize = (pluginSize);//0;
#endif//_AUTOMATION_
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

//[Max 2021.06.18
void StrGetInt(char *sw,  int *z);
char * seek_digit_int(char *s){
    while (*s!=0){
        if (((*s)>='0')&&((*s)<='9'))
        {
            return s;
        }
        s++;
    }
    return s;
}
void copy_int_digit(char* &s, char* ss)
{
    while (*s!=0)
    {
        if ((*s<'0')||(*s>'9'))
        {
            *ss = 0;
            return;
        }
        *ss = *s;
        s++; ss++;
    }
}
void ParseBigVersion(char* str1){
    _Log("parse str version:")
    _Log(str1)
    int major=0,minor=0,patch=0;
    char *s = str1;
    char ss[64];
    
    //find first digit
    s = seek_digit_int(s);
    if (*s==0){
        _Log("ERROR! cannot find first digit!")
        return;
    }
    
    
    copy_int_digit(s,ss);
    StrGetInt(ss,&major);
    
    
    if (*s==0){
        _Log("Error! cannot find first .");
        return;
    }
    
    s = seek_digit_int(s);
    if (*s==0){
        _Log("ERROR! cannot find second digit!")
        return;
    }
    
    copy_int_digit(s,ss);
    StrGetInt(ss,&minor);
    
    if (*s==0){
        _Log("Error! cannot find second .");
        return;
    }
    
    s = seek_digit_int(s);
    if (*s==0){
        _Log("ERROR! cannot find threed digit!")
        return;
    }
    
    copy_int_digit(s,ss);
    StrGetInt(ss,&patch);
    
    OSXVersionBig = major*10000 + minor*100 + patch;
    
   // if ((OSXVersionBig>=101600) && (OSXVersionBig<110200) )
    //   _FlipList = true;
            
}

void Gestalt_my() {
    _Log("Gestalt_my>>>")
    
    //Test
    //ParseBigVersion("Version 10.15.5 (sdsds sdsds)");
    
//    Gestalt(gestaltSystemVersion, response);
//    if (*response<=0x1080){
//        _LogInt("old OSX version=0x%x",*response)
//        OSXVersionBig = 0;
//        return;
//    }
    
    
    
    //typedef struct{
//        NSInteger majorVersion;
//        NSInteger minorVersion;
//        NSInteger patchVersion;
//    } NSOperatingSystemVersionMy;
    
    _Log("before operatingSystemVersion")
    //NSOperatingSystemVersionMy version = (NSOperatingSystemVersionMy)[[NSProcessInfo processInfo] operatingSystemVersion];
    NSString *string = [[NSProcessInfo processInfo] operatingSystemVersionString];
    //NSString string = [[NSProcessInfo processInfo] operatingSystem];
    
    const char *str1 = (const char*)[string UTF8String];
#ifdef _LOG_
    {
        //char str[256];
        
        //sprintf(str,"version is %d.%d.%d",version.majorVersion, version.minorVersion, version.patchVersion);
        _Log(str1)
    }
#endif
    
    //*response = 4240; // hex 1090
    _Log("before ParseBigVersion. str1 is")
    _Log(str1)
    ParseBigVersion((char*)str1);
    
}
//[Max 2021.06.18
//setup readwrite permissions
void SetPermissions(NSString *destinationPath){
    NSFileManager *manager = [NSFileManager defaultManager];
    NSDictionary *attributes;

    [attributes setValue:[NSString stringWithFormat:@"%d", 0777]
                 forKey:NSFilePosixPermissions]; // chmod permissions 777
    [manager setAttributes:attributes ofItemAtPath:destinationPath error:nil];
}

