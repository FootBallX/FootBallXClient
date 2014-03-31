//
//  CFBCardManager.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-3-6.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBCardManager.h"

IMPLEMENT_SINGLETON(CFBCardManager);

CFBCardManager::CFBCardManager()
{
    
}



CFBCardManager::~CFBCardManager()
{
    
}



bool CFBCardManager::init()
{
    do
    {
        m_cards["10001"] = new CFBCard{"10001",1,0,0,0,0,0,0,100,100,100};
        m_cards["10002"] = new CFBCard{"10002",1,0,0,0,0,0,0,100,100,100};
        m_cards["20001"] = new CFBCard{"20001",2,1000,100,100,100,100,100,0,100,100};
        m_cards["20002"] = new CFBCard{"20002",2,1000,100,100,100,100,100,0,100,100};
        m_cards["20003"] = new CFBCard{"20003",2,1000,100,100,100,100,100,0,100,100};
        m_cards["20004"] = new CFBCard{"20004",2,1000,100,100,100,100,100,0,100,100};
        m_cards["30001"] = new CFBCard{"30001",3,1000,100,100,100,100,100,0,100,100};
        m_cards["30002"] = new CFBCard{"30002",3,1000,100,100,100,100,100,0,100,100};
        m_cards["30003"] = new CFBCard{"30003",3,1000,100,100,100,100,100,0,100,100};
        m_cards["30004"] = new CFBCard{"30004",3,1000,100,100,100,100,100,0,100,100};
        m_cards["30005"] = new CFBCard{"30005",3,1000,100,100,100,100,100,0,100,100};
        m_cards["30006"] = new CFBCard{"30006",3,1000,100,100,100,100,100,0,100,100};
        m_cards["30007"] = new CFBCard{"30007",3,1000,100,100,100,100,100,0,100,100};
        m_cards["30008"] = new CFBCard{"30008",3,1000,100,100,100,100,100,0,100,100};
        m_cards["30009"] = new CFBCard{"30009",3,1000,100,100,100,100,100,0,100,100};
        m_cards["40001"] = new CFBCard{"40001",4,1000,100,100,100,100,100,0,100,100};
        m_cards["40002"] = new CFBCard{"40002",4,1000,100,100,100,100,100,0,100,100};
        m_cards["40003"] = new CFBCard{"40003",4,1000,100,100,100,100,100,0,100,100};
        m_cards["40004"] = new CFBCard{"40004",4,1000,100,100,100,100,100,0,100,100};
        m_cards["40005"] = new CFBCard{"40005",4,1000,100,100,100,100,100,0,100,100};
        m_cards["40006"] = new CFBCard{"40006",4,1000,100,100,100,100,100,0,100,100};
        m_cards["40007"] = new CFBCard{"40007",4,1000,100,100,100,100,100,0,100,100};

        return true;
    } while (false);
    
    return false;
}



const CFBCard& CFBCardManager::getCardById(const string& sid)
{
    auto it = m_cards.find(sid);
    CC_ASSERT(it != m_cards.end());
    
    return *((*it).second);
}