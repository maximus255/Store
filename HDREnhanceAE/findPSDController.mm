//
//  findPSDController.m
//  TestXNib
//
//  Created by Admin on 22/05/2018.
//  Copyright © 2018 alphaplugins. All rights reserved.
//

#import "findPSDController.h"
#include <string.h>
#include "log.h"

extern char _Path[];

@interface FindPSDController ()
{
    NSArray *hostsArray;
    //BOOL hasCC;
    int previous_choose;
}

@end

@implementation FindPSDController

-(void) awakeFromNib
{
    _Log("awakeFromNib FindPSDController")
    [self viewDidLoad1];//for before OSX10.10
}
- (void)viewDidLoad1
{
    _Log("viewDidLoad findPDSController>>>")
    //[super viewDidLoad];//10.10
    // Do view setup here.
    NSLog(@"FindPDS Load");
    //fild a list of /application
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationDirectory, NSLocalDomainMask, YES);
    NSString *documentsDirectory;
    if (paths.count<1)
    {
        NSLog(@"cannot automaticcally localize Applications!");
        documentsDirectory = @"/Applications";
    }
    else
    {
        documentsDirectory = [paths objectAtIndex:0];//paths[0];
    }
    
    NSString *appSupportDirectory;
    paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSLocalDomainMask, YES);
    if (paths.count<=0)
    {
        NSLog(@"cannot locate library");
        appSupportDirectory = @"/Library/Application Support";
    }
    else
    {
        appSupportDirectory = [paths objectAtIndex:0];
    }
    appSupportDirectory = [appSupportDirectory stringByAppendingPathComponent:@"Adobe/Common/Plug-ins"];
    
    NSMutableArray *Hosts = [[NSMutableArray alloc] init];
        
    [Hosts addObject:@"Common plugins folder (Recommended)"];
    NSArray *urls = [fileManager contentsOfDirectoryAtPath:appSupportDirectory error:nil];
    for (NSString *str in urls)
    {
        if (([str rangeOfString:@"7.0"].location == NSNotFound) ) //already added
        {
            NSString *str1 = [appSupportDirectory stringByAppendingPathComponent: str];
            str1 = [str1 stringByAppendingPathComponent:@"MediaCore"];
            [Hosts addObject:str1];
        }
    }//for str
    /*if (Hosts.count<=0){
        NSLog(@"MediaCore was not found!");
        [Hosts addObject:GenerateCommonPluginPath()];
    }*/
    
    urls = [fileManager contentsOfDirectoryAtPath:documentsDirectory error:nil];
    for (NSString *str in urls)
    {
        if (([str rangeOfString:@"After Effects"].location != NSNotFound)
            //|| ([str rangeOfString:@"Premiere Pro"].location != NSNotFound)
            )
        {
            NSString *str1 = [documentsDirectory stringByAppendingPathComponent: str];
            str1 = [str stringByAppendingPathComponent:@"/Plug-Ins/Effects"];
            [Hosts addObject:str1];
        }
    }//for str
    
//    if (hasCC)
//    {
//        [Hosts insertObject:@"Common Plug-ins Folder (Recommended)" atIndex:0];
//    }
    
    hostsArray = [Hosts copy];
    [_hostsTable reloadData];
    
}
-(void)lastChoose:(int)row
{
    _LogInt("lastChoose row=%d",row)
    previous_choose = row;
}
-(void)viewWillAppear1
{
    _Log("viewWillAppear findPSDController")
    if (previous_choose==-1)//no last choose yet
    {
        //if (hasCC)
        {
            NSIndexSet *indexSet = [NSIndexSet indexSetWithIndex:0];
            [_hostsTable selectRowIndexes:indexSet byExtendingSelection:NO];
            [self SelectRoad:0];
        }// if (hasCC)
    }
    else
    {
        if (previous_choose==-2)//CustomChoose
        {
            NSString *path =[NSString stringWithCString:_Path encoding:NSUTF8StringEncoding];
            self.destinationPath.stringValue = path;
            
            //strcpy(_Path,[path cStringUsingEncoding:NSUTF8StringEncoding]);
            
            NSLog(@"_Path is %s",_Path);
            
            [self.delegate SetupNextButton:YES clicked_row:-2];
            [_hostsTable deselectAll:self];
        }
        else
        {
            NSIndexSet *indexSet = [NSIndexSet indexSetWithIndex:previous_choose];
            [_hostsTable selectRowIndexes:indexSet byExtendingSelection:NO];
            [self SelectRoad:previous_choose];
        }
    }
}

-(void)SelectRoad:(int)row
{
    _Log("SelectRoad")
    if ((row<0) || (row>=hostsArray.count) )
    {
        NSLog(@"Wrong row %d",row);
        return;
    }
    NSString *path;
    
    /*if ((hasCC)&&(row==0))//common recommended
    {
        path = GenerateCommonPluginPath();
        //NSLog(path);
    }
    else
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationDirectory, NSLocalDomainMask, YES);
        if (paths.count<=0)
        {
            NSLog(@"cannot locate applications");
            path = @"/Applications";
        }
        else
        {
            //path = paths[0];
            path = [paths objectAtIndex:0];
        }
        path = [path stringByAppendingString:@"/"];
        path = [path stringByAppendingString:[hostsArray objectAtIndex:row]];
        path = [path stringByAppendingString:@"/Plug-Ins/Effects"];
    }*/
    if (row==0)//Common
    {
        path = GenerateCommonPluginPath();
    }else{
        path = [hostsArray objectAtIndex:row];
    }
    
    self.destinationPath.stringValue = path;
    
    strcpy(_Path,[path cStringUsingEncoding:NSUTF8StringEncoding]);
    
    NSLog(@"_Path is %s",_Path);
    
    [self.delegate SetupNextButton:YES clicked_row:row];
}
#pragma mark --NSTableView delegates
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
     _Log("tableView: numberOfRowsInTableView")
    return [hostsArray count];
}
//for Cell Base
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    _Log("tableView: objectValueForTableColumn")
    return [hostsArray objectAtIndex: row];
}

- (void)tableViewSelectionDidChange:(NSNotification *)notification
{
    _Log("tableView: tableViewSelectionDidChange")
    NSInteger row = [notification.object selectedRow];
    if (row>=hostsArray.count)
    {
        NSLog(@"clicked row [%ld] > array count [%lu]",(long)row, (unsigned long)hostsArray.count);
        return;
    }
    NSLog(@"%@ was choosed",[hostsArray objectAtIndex:row]);
    [self SelectRoad:(int)row];
}
- (IBAction)customPressed:(id)sender
{
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    
    // Don't allow the user to choose files, only directories
    // for our destination.
    [panel setCanChooseFiles:NO];
    [panel setCanChooseDirectories:YES];
    
    NSString *aplicationDirectory = self.destinationPath.stringValue;
    
    if ([aplicationDirectory length]<=0)
    {
        //NSFileManager *fileManager = [NSFileManager defaultManager];
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationDirectory, NSLocalDomainMask, YES);
        
        if (paths.count<1)
        {
            NSLog(@"cannot automaticcally localize Applications!");
            aplicationDirectory = @"/Applications";
        }
        else
        {
            aplicationDirectory = [paths objectAtIndex:0];//paths[0];
        }
    }//length<=0
    [panel setDirectoryURL:[NSURL fileURLWithPath:aplicationDirectory]];
    
    if( [panel runModalForDirectory:aplicationDirectory file:nil] == NSOKButton )
    {
        NSString *path = [panel directory];
        
        self.destinationPath.stringValue = path;
        strcpy(_Path,[path cStringUsingEncoding:NSUTF8StringEncoding]);
        
        NSLog(@"_Path is %s",_Path);
        [self.delegate SetupNextButton:YES clicked_row:-2];
        [_hostsTable deselectAll:self];
    }
}
@end

NSString* GenerateCommonPluginPath()
{
    NSString *path;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSLocalDomainMask, YES);
    if (paths.count<=0)
    {
        NSLog(@"cannot locate library");
        path = @"/Library/Application Support";
    }
    else
    {
        //path = paths[0];
        path = [paths objectAtIndex:0];
    }
    path = [path stringByAppendingPathComponent:@"Adobe/Common/Plug-ins/7.0/MediaCore"];
    return path;
}
