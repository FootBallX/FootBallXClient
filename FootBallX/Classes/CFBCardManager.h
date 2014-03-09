//
//  CFBCardManager.h
//  FootBallX
//
//  Created by 马 俊 on 14-3-6.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBCardManager__
#define __FootBallX__CFBCardManager__

#include "Common.h"
#include "CSingleton.h"
#include "CFBCard.h"

class CFBCardManager : public CSingleton<CFBCardManager>
{
public:
    CFBCardManager();
    ~CFBCardManager();
    
    bool init();
    
    const CFBCard& getCardById(const string& sid);
protected:
    map<string, CFBCard*> m_cards;
};

#define CARD_MGR        (CFBCardManager::getInstance())

#endif /* defined(__FootBallX__CFBCardManager__) */
