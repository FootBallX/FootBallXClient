//
//  CRandomManager.h
//  FootBallX
//
//  Created by 马 俊 on 14-2-23.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CRandomManager__
#define __FootBallX__CRandomManager__

#include "Common.h"
#include "CSingleton.h"

class CRandomManager : public CSingleton<CRandomManager>
{
public:
    CRandomManager() = default;
    ~CRandomManager() = default;
    
    int getRand();
};

#define RANDOM_MGR  (CRandomManager::getInstance())

#endif /* defined(__FootBallX__CRandomManager__) */
