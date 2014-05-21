//
//  CCBReadHelper.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-5-21.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CCBReadHelper.h"


Node* CCBReadHelper::read(const char* ccbi)
{
    auto reader = new cocosbuilder::CCBReader(cocosbuilder::NodeLoaderLibrary::getInstance());
    reader->autorelease();
    
    return reader->readNodeGraphFromFile(ccbi);
}


Scene* CCBReadHelper::readScene(const char* ccbi)
{
    auto reader = new cocosbuilder::CCBReader(cocosbuilder::NodeLoaderLibrary::getInstance());
    reader->autorelease();
    
    return reader->createSceneWithNodeGraphFromFile(ccbi);
}