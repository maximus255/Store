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
BOOL checkCC2014(char *path);

@interface FindPSDController ()
{
    NSArray *photoshopsArray;
    BOOL hasCC;
    int previous_choose;
    //[Max 2023.04.04
    BOOL hasAffinity;
    NSString *affinityPath;
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
    NSArray *urls = [fileManager contentsOfDirectoryAtPath:documentsDirectory error:nil];
    //NSLog(@"%@",urls);
    //make a subllist of "Photoshop CC"
    NSMutableArray *Photoshops = [[NSMutableArray alloc] init];
    hasCC = NO;
    for (NSString *str in urls)
    {
        //[Max 2020.01.12
        if (([str rangeOfString:@"Photoshop CC"].location != NSNotFound)
            || ([str rangeOfString:@"Photoshop 20"].location != NSNotFound)
            )
            //if ([str containsString:@"Photoshop CC"])//for 10.8 and later
        {
            const char *tmpPath = [str UTF8String];
            if (!hasCC)
                if (checkCC2014((char*)tmpPath))
                    hasCC = YES;
            [Photoshops addObject:str];
        }
        if ([str rangeOfString:@"Photoshop CS5"].location != NSNotFound)
        //if ([str containsString:@"Photoshop CS5"])//OSX10.8
        {
            [Photoshops addObject:str];
        }
        if ([str rangeOfString:@"Photoshop CS6"].location != NSNotFound)
        //if ([str containsString:@"Photoshop CS6"])
        {
            [Photoshops addObject:str];
        }
    }//for str
    if (hasCC)
    {
        [Photoshops insertObject:@"Common Plug-ins Folder (Recommended)" atIndex:0];
    }
    
    //[Max 2023.04.04
    //find affinity
    hasAffinity = NO;
    paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    if (paths.count>0){
        documentsDirectory = [paths objectAtIndex:0];//paths[0];
        documentsDirectory = [documentsDirectory stringByAppendingPathComponent:@"Application Support/Affinity Photo 2/Plugins"];
        
        BOOL isDir = YES;
        BOOL isExist = [[NSFileManager defaultManager] fileExistsAtPath:documentsDirectory isDirectory:&isDir];
        if (isExist){
            [Photoshops addObject:@"Affinity Photo 2 Plugins folder"];
            hasAffinity = YES;
            affinityPath = documentsDirectory;
        }
    }
    //NSLog(@"%@",Photoshops);
    photoshopsArray = [Photoshops copy];
    [_photoshopsTable reloadData];
    
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
        if (hasCC)
        {
            NSIndexSet *indexSet = [NSIndexSet indexSetWithIndex:0];
            [_photoshopsTable selectRowIndexes:indexSet byExtendingSelection:NO];
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
            [_photoshopsTable deselectAll:self];
        }
        else
        {
            NSIndexSet *indexSet = [NSIndexSet indexSetWithIndex:previous_choose];
            [_photoshopsTable selectRowIndexes:indexSet byExtendingSelection:NO];
            [self SelectRoad:previous_choose];
        }
    }
    /*if (hasCC)
     {
     [self SelectRoad:0];
     }// if (hasCC)*/
}

-(void)SelectRoad:(int)row
{
    _Log("SelectRoad")
    if ((row<0) || (row>=photoshopsArray.count) )
    {
        NSLog(@"Wrong row %d",row);
        return;
    }
    NSString *path;
    //[Max 2023.04.04
    if ((hasAffinity)&&(row == [photoshopsArray count]-1)){
        path = affinityPath;
    }else
    if ((hasCC)&&(row==0))//common recommended
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
        //path = [path stringByAppendingString:photoshopsArray[row]];
        path = [path stringByAppendingString:[photoshopsArray objectAtIndex:row]];
        path = [path stringByAppendingString:@"/Plug-Ins"];
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
    //if ([tableView.identifier isEqualToString:@"photoshops"])//10.7
    {
        return [photoshopsArray count];
    }
    return 0;
}
//for Cell Base
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    _Log("tableView: objectValueForTableColumn")
   // if ([tableView.identifier isEqualToString:@"photoshops"])//10.7
    {
       // return @"Test";
        return [photoshopsArray objectAtIndex: row];
    }
    
    return nil;
}

//if (@available(macOS 10.7, *))
/* //for View Based
- (NSView*)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
       NSTableCellView *cellView = [tableView makeViewWithIdentifier:tableColumn.identifier owner:self];
    
    if ([tableView.identifier isEqualToString:@"photoshops"])
    {
        if ([tableColumn.identifier isEqualToString:@"firstColumn"])
        {
            if (row<photoshopsArray.count)
                cellView.textField.stringValue = [photoshopsArray objectAtIndex: row];
        }
    }
    return cellView;
}*/
- (void)tableViewSelectionDidChange:(NSNotification *)notification
{
    _Log("tableView: tableViewSelectionDidChange")
   // if ([[notification.object identifier] isEqualToString:@"photoshops"])//10.7
    {
        NSInteger row = [notification.object selectedRow];
        if (row>=photoshopsArray.count)
        {
            NSLog(@"clicked row [%ld] > array count [%lu]",(long)row, (unsigned long)photoshopsArray.count);
            return;
        }
        NSLog(@"%@ was choosed",[photoshopsArray objectAtIndex:row]);
        [self SelectRoad:(int)row];
    }
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
        
        //[MWDVolume
        //remove "Machine Wash Deluxe"
        {
            int len = (int)strlen(_Path);
            if (len>0)
            {
                if ((_Path[len-1]!='/')&&(_Path[len-1]!='\\'))
                    strcat(_Path,"/");
                
                char subStr[] = "Machine Wash Deluxe/";
                int len1 = (int)strlen(subStr);
                if (len>len1)
                if (strcmp(_Path,_Path+len-len1)!=0)
                    _Path[len-len1] = 0;
            }//len >0
        }//]
        
        NSLog(@"_Path is %s",_Path);
        [self.delegate SetupNextButton:YES clicked_row:-2];
        [_photoshopsTable deselectAll:self];
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
    path = [path stringByAppendingString:@"/Adobe/Plug-Ins/CC"];
    return path;
}
