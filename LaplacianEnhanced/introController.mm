//
//  introController.m
//  TestXNib
//
//  Created by Admin on 22/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import "introController.h"
#include "Engine/UI.h"
#include "log.h"
@interface IntroController ()
{
    __unsafe_unretained IBOutlet NSTextField *intro_Text;
    __unsafe_unretained IBOutlet NSTextField *version_Text;
    __unsafe_unretained IBOutlet NSTextField *copyright_Text;
    
}
@end

@implementation IntroController
-(void) awakeFromNib
{
    _Log("awakeFromNib IntroController")
    [self viewDidLoad1];//for before OSX10.10
}
- (void)loadView
{
    _Log("loadView IntroController")
    //  [super loadView];
}

-(id) initWithNibName:(NSNibName)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    _Log("initWithNibName IntroController")
    return self;
}
- (id)initWithFrame:(NSRect)frameRect
{
    _Log("initWithFrame IntroController")
    //[super initWithFrame:frameRect];
    return self;
}
-(id) initWithCoder:(NSCoder *)coder
{
    _Log("initWithCoder IntroController")
    self = [super initWithCoder:coder];
    
    return self;
}
-(id) Init
{
    _Log("Init IntroController")
    return self;
}
- (void)viewDidLoad1 {
   // [super viewDidLoad];
    // Do view setup here.
    NSLog(@"Intro controller load");
    _Log("IntroController viewDidLoad")
    
     [self viewWillAppear1];
}

-(void)viewWillAppear1
{
     _Log("IntroController viewWillAppear")
    intro_Text.stringValue = [NSString stringWithCString:_Intro encoding:NSASCIIStringEncoding];
    version_Text.stringValue = [NSString stringWithCString:_Version encoding:NSASCIIStringEncoding];
    copyright_Text.stringValue = [NSString stringWithCString:_Copyright encoding:NSUTF8StringEncoding];
}

@end
