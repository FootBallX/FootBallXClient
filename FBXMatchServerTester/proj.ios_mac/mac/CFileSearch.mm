//
//  CFileSearch.cpp
//  FBXMatchServerTester
//
//  Created by Ray M on 14-7-25.
//
//

#include "CFileSearch.h"
#include <Foundation/Foundation.h>
#include <Cocoa/Cocoa.h>


bool CFileSearch::search(const char* path, vector<string>& files)
{
    NSFileManager* fileManager = [NSFileManager defaultManager];
    NSString* pathName = [NSString stringWithFormat:@"%s", path];
    NSArray* file_arr = [fileManager contentsOfDirectoryAtPath:pathName error:nil];
    if (file_arr == nil)
    {
        return false;
    }
    
    for (NSString* f in file_arr)
    {
        NSString* ext = [f substringFromIndex:[f length] - 5];
        if ([ext compare:@".ccbi" options:NSCaseInsensitiveSearch] == NSOrderedSame)
        {
            files.push_back([f cStringUsingEncoding:NSASCIIStringEncoding]);
        }
    }
    return true;
}