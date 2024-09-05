//
//  AgreementController.m
//  TestXNib
//
//  Created by Admin on 22/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import "AgreementController.h"
#include "log.h"
extern char ScriptsPath[];
@interface AgreementController ()
{
    IBOutlet NSTextView *textView;
}
@end

@implementation AgreementController

-(void) awakeFromNib
{
    _Log("awakeFromNib AgreementController")
    [self viewDidLoad1];//for before OSX10.10
}
- (void)loadView
{
    _Log("loadView AgreementController")
    //  [super loadView];
}

-(id) initWithNibName:(NSNibName)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    _Log("initWithNibName AgreementController")
    return self;
}
- (id)initWithFrame:(NSRect)frameRect
{
    _Log("initWithFrame AgreementController")
    //[super initWithFrame:frameRect];
    return self;
}
-(id) initWithCoder:(NSCoder *)coder
{
    _Log("initWithCoder AgreementController")
    self = [super initWithCoder:coder];
    
    return self;
}
-(id) Init
{
    _Log("Init AgreementController")
    return self;
}

- (void)viewDidLoad1 {
    //[super viewDidLoad];
    // Do view setup here.
    NSLog(@"Load Agreement controller");
    
    [textView setEditable:NO];
    
    
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"license" ofType:@"txt" inDirectory:@"Scripts"];
    
    /*NSError *err = nil;
     NSString *contents = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:&err];
     [textView setString:contents];
     */
    [textView readRTFDFromFile:path];
    
    [textView setTextColor:[NSColor whiteColor]];

}

@end
