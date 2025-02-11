//
//  finishController.m
//  TestXNib
//
//  Created by Admin on 22/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import "finishController.h"
#include "log.h"
#include "Engine/UI.h"
@interface FinishController ()
{
    
    __unsafe_unretained IBOutlet NSTextField *congratulationText;
    __unsafe_unretained IBOutlet NSTextField *pathText;
}
@end
extern char _Path[];
@implementation FinishController
-(void) awakeFromNib
{
    _Log("awakeFromNib FinishController")
    [self viewDidLoad1];//for before OSX10.10
}
- (void)viewDidLoad1 {
    //[super viewDidLoad];//10.10
    // Do view setup here.
    NSLog(@"Finish Load");
    _Log("viewDidLoad FinishController")
   // [self viewWillAppear1];
}
- (void)viewWillAppear1
{
     _Log("viewWillAppear FinishController")
    congratulationText.stringValue = [NSString stringWithCString:_Congratulation encoding:NSASCIIStringEncoding];
    pathText.stringValue = [NSString stringWithCString:_Path encoding:NSASCIIStringEncoding];
}
@end
