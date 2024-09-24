//
//  copyingController.m
//  TestXNib
//
//  Created by Admin on 22/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import "copyingController.h"
#include "log.h"

extern int _NeedToCopyTextures;
extern char _Path[];
extern char _CopyIntro[];
extern char SourcePath[];
extern char _StoreFolder[];
extern char _Version[];
NSProgressIndicator *progressIndicator;

@interface CopyingController ()

@end

@implementation CopyingController
-(void) awakeFromNib
{
    _Log("awakeFromNib CopyingController")
    [self viewDidLoad1];
}
- (void)viewDidLoad1 {
    // Do view setup here.
    NSLog(@"Copying Load");
    _Log("viewDidLoad CopyingController")
    progressIndicator = _progressBar;
}
-(void)viewWillAppear1
{
    _Log("viewWillAppear CopyingController")
    _destinationPath.stringValue = [NSString stringWithCString:_Path encoding:NSUTF8StringEncoding];
    _copyingText.stringValue = [NSString stringWithCString:_CopyIntro encoding:NSASCIIStringEncoding];
    
    NSLog(@"Start Copying...");
    NSLog(@"Source %s",SourcePath);
    NSLog(@"Destination %s",_Path);
    
}

- (void)viewDidAppear1
{
    _Log("viewDidAppear CopyingController")
    [self.delegate StartCopying];

}
@end
