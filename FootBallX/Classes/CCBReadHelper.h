//
//  CCBReadHelper.h
//  FootBallX
//
//  Created by 马 俊 on 14-5-21.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CCBReadHelper__
#define __FootBallX__CCBReadHelper__

#include "Common.h"

class CCBReadHelper
{
public:
    static Node* read(const char* ccbi);
    static Scene* readScene(const char* ccbi);
};

#endif /* defined(__FootBallX__CCBReadHelper__) */
