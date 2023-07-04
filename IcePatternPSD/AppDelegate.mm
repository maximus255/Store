//
//  AppDelegate.m
//  TestXNib
//
//  Created by Admin on 22/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import "AppDelegate.h"
#import "viewController.h"
#include "log.h"
@interface AppDelegate ()

int WaitUntilLaunched();

@property IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
}
- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
    _Log("applicationWillFinishLaunching")
    
    /*
    int res = WaitUntilLaunched();
    if (res)
    {
        _Log("return because YesNo Photoshop return 0")
        [NSApp terminate: nil];
        //return 0;
    }*/
    
    /*if (_window!=nil)
    {
        if (_window.contentView!=nil)
        {
            _Log("_window.contentView!=nil")
            NSString *c = _window.contentView.className;
            _Log((char*)[c UTF8String])
        }
        else
        {
            _Log("_window.contentView is nil!!!")
        }
        
        if (_window.contentViewController!=nil)
        {
            _Log("contentViewController!=nil")
            NSString *c = _window.contentViewController.className;
            _Log((char*)[c UTF8String]);
        }
        else
        {
            //_Log("contentViewController=nil!")
            
            //_window.contentViewController = _globalViewController;//10.10 :(
            //[_window bind:NSTitleBinding toObject:_globalViewController withKeyPath:@"title" options:nil];
            
            //[_window setContentViewController:_globalViewController];
            //[_window setContentView:_globalViewController.view];
        }
        //_globalViewController.view.frame = _window.frame;
        //[_window.contentView addSubview:_globalViewController.view];
        //[_window setRo
        
        
    }
    else
    {
        _Log("window = nil!!!")
    }
    */
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
    return YES;
}

@end
