//
//  CFBTeam.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBTeam__
#define __FootBallX__CFBTeam__

#include "Common.h"
#include "FBDefs.h"

class CFBFormation;
class CFBPlayer;

class CFBTeam
{
public:
    CC_SYNTHESIZE_READONLY(CFBFormation*, m_formation, Formation);
    CC_SYNTHESIZE(FBDefs::SIDE, m_side, Side);
    
    CFBTeam();
    virtual ~CFBTeam() = default;
    
    virtual bool init();
    virtual void update(float dt);
    virtual bool onStartMatch();
    virtual void kickOff();
    virtual CFBPlayer* getPlayingPlayer();
    virtual bool changeFormation(FBDefs::FORMATION formationId);
    virtual bool isAttacking() const { return m_state == FBDefs::TEAM_STATE::ATTACKING; }
    virtual bool isDefending() const { return m_state == FBDefs::TEAM_STATE::DEFENDING; }
protected:
    FBDefs::TEAM_STATE m_state = FBDefs::TEAM_STATE::NONE;
private:
    vector<CFBPlayer*> m_teamMembers;
    
};


#endif /* defined(__FootBallX__CFBTeam__) */
