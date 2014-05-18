//
//  CFBFormation.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBFormation__
#define __FootBallX__CFBFormation__

#include "Common.h"
#include "CFBPlayerAI.h"
#include "FBDefs.h"
#include "CFBPlayerInitInfo.h"

class CFBPlayer;
class CFBTeam;

#define GOALKEEPER_ORBIT_RATE                100
#define BACK_ORBIT_RATE                      150
#define HALF_BACK_ORBIT_RATE                 150
#define FORWARD_ORBIT_RATE                   100


#pragma mark ----- FormationBase

class CFBFormation
{
public:
    CFBFormation() = default;
    virtual ~CFBFormation();
    virtual const char* description(){CC_ASSERT(false); return nullptr;};

    virtual bool init(CFBTeam* team);
    virtual CFBPlayer* getPlayer(int pos);
    
    virtual void update(float dt);
    
    virtual void think();
    
    virtual int getPlayerNumber() { return m_playerNumber;}
    
    virtual bool onStartMatch(const vector<CFBPlayerInitInfo>& initPlayerInfo, const vector<CFBPlayer*>& players, bool networkControl);
    
    virtual CFBPlayer* getKickOffPlayer();
    
    virtual CFBTeam* getTeam() const {return m_team;}
    
    virtual CFBPlayerAI* getPlayerAIById(int idx) { return m_playerAIs[idx]; }
    
    virtual FBDefs::FORMATION getFormationId() const { return m_formationId; }
    
    virtual CFBPlayerAI* getPassBallTarget();
    
//    virtual Point getDribbleTargetPos();
protected:
    FBDefs::FORMATION m_formationId = FBDefs::FORMATION::NONE;
    CFBTeam* m_team;        // weak reference to the team object;
    int m_playerNumber = 0;
    vector<CFBPlayerAI*> m_playerAIs;
    int m_updateIndex = -1;
    int m_updateCD = 0;
    vector<int> m_playerIndices;
};


#endif /* defined(__FootBallX__CFBFormation__) */
