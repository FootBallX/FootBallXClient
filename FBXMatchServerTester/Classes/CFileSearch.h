//
//  CFileSearch.h
//  FBXMatchServerTester
//
//  Created by Ray M on 14-7-25.
//
//

#ifndef __FBXMatchServerTester__CFileSearch__
#define __FBXMatchServerTester__CFileSearch__

#include <string>
#include <vector>

using namespace std;


class CFileSearch
{
public:
    static bool search(const char* path, vector<string>& files);
};

#endif /* defined(__FBXMatchServerTester__CFileSearch__) */
