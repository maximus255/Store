//
//  viewController.h
//  TestXNib
//
//  Created by Admin on 22/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "Engine/UI.h"
#include "Engine/Engine.h"
#import "findPSDController.h"
#import "copyingController.h"
@interface viewController : NSViewController <findPSDDelegate, installingDelegate, NSFileManagerDelegate>
- (void)viewDidLoad1;
@end
extern viewController* _globalViewController;
