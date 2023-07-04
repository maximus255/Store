//
//  findPSDController.h
//  TestXNib
//
//  Created by Admin on 22/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol findPSDDelegate <NSObject>
-(void)SetupNextButton:(BOOL)en clicked_row:(int)row;
@end//protocol

@interface FindPSDController : NSViewController < NSTableViewDelegate, NSTableViewDataSource >
{
    
}
-(void)lastChoose:(int)row;
@property (nonatomic, strong) IBOutlet NSTableView *photoshopsTable;
@property (nonatomic, strong) IBOutlet NSTextField *destinationPath;

@property (nonatomic ) id<findPSDDelegate> delegate;
-(void)viewWillAppear1;
@end
NSString* GenerateCommonPluginPath();
