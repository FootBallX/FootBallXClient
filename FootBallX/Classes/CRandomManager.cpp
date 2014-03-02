//
//  CRandomManager.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-2-23.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CRandomManager.h"
#include <random>

IMPLEMENT_SINGLETON(CRandomManager);

int CRandomManager::getRand()
{
    return rand();
}