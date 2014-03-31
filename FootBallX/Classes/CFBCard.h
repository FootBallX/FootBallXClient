//
//  CFBCard.h
//  FootBallX
//
//  Created by 马 俊 on 14-3-6.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBCard__
#define __FootBallX__CFBCard__

#include <string>
#include "CMemPool.h"

#define CARD_POOL_SIZE      2000

class CFBCard
{
public:
    MEM_POOL_DECLARE(CFBCard, CARD_POOL_SIZE);
    
    CFBCard() : CFBCard("", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    {
    }
    
    CFBCard(const std::string& cid, int tp, float str, float spd, float drb, float pas, float sht, float def, float atk, float grd, float air)
    : m_cardID(cid), m_type(tp), m_strength(str), m_speed(spd), m_dribbleSkill(drb),
    m_passSkill(pas), m_shootSkill(sht), m_defenceSkill(def),
    m_attackSkill(atk), m_groundSkill(grd), m_airSkill(air)
    {
        
    }
    
    CFBCard(const CFBCard& obj) : CFBCard(obj.m_cardID, obj.m_type, obj.m_strength, obj.m_speed, obj.m_dribbleSkill,
                                          obj.m_passSkill, obj.m_shootSkill, obj.m_defenceSkill, obj.m_attackSkill,
                                          obj.m_groundSkill, obj.m_airSkill)
    {
    }
    
    std::string m_cardID;
    
    int m_type;
    ////////////////////////////////
    float m_strength = 0.f;
    float m_speed = 0.f;
    float m_dribbleSkill = 0.f;
    float m_passSkill = 0.f;
    float m_shootSkill = 0.f;
    float m_defenceSkill = 0.f;
    float m_attackSkill = 0.f;      // goalkeeper only
    float m_groundSkill = 0.f;
    float m_airSkill = 0.f;
};

#endif /* defined(__FootBallX__FBCard__) */
