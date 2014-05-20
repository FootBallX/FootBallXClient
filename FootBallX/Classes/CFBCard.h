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
#include "FBDefs.h"
#include "CMemPool.h"

#define CARD_POOL_SIZE      2000

class CFBCard
{
public:
    MEM_POOL_DECLARE(CFBCard, CARD_POOL_SIZE);
    
    CFBCard() : CFBCard(-1, nullptr, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    {
    }
    
    CFBCard(int cid, const char* icon, int qlt, float str, float spd, float drb, float pas, float sht, float def, float atk, float grd, float air)
    : m_cardID(cid), m_quality(qlt), m_strength(str), m_speed(spd), m_dribbleSkill(drb),
    m_passSkill(pas), m_shootSkill(sht), m_defenceSkill(def),
    m_attackSkill(atk), m_groundSkill(grd), m_airSkill(air)
    {
        if (nullptr != icon)
        {
            strncpy(m_icon, icon, FBDefs::MAX_CARD_ICON_LEN -1);
        }
    }
    
    CFBCard(const CFBCard& obj) : CFBCard(obj.m_cardID, obj.m_icon, obj.m_quality, obj.m_strength, obj.m_speed, obj.m_dribbleSkill,
                                          obj.m_passSkill, obj.m_shootSkill, obj.m_defenceSkill, obj.m_attackSkill,
                                          obj.m_groundSkill, obj.m_airSkill)
    {
    }
    
    int m_cardID = -1;
    char m_icon[FBDefs::MAX_CARD_ICON_LEN] = {0};
    
    int m_quality;
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
