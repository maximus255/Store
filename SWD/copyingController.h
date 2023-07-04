//
//  copyingController.h
//  TestXNib
//
//  Created by Admin on 22/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@protocol installingDelegate <NSObject>
-(void)StartCopying;
-(void)EndCopying:(BOOL)Success error:(NSError*)error;
@end//protocol

@interface CopyingController : NSViewController
@property (nonatomic, strong) IBOutlet NSTextField *destinationPath;
@property (nonatomic, strong) IBOutlet NSTextField *copyingText;
@property (nonatomic, strong) IBOutlet NSProgressIndicator *progressBar;

@property (nonatomic ) id<installingDelegate> delegate;

-(void)viewWillAppear1;
-(void)viewDidAppear1;
@end
