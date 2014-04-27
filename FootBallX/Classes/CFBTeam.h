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
    
    virtual bool init(const vector<string>& cardPlayers);
    virtual void update(float dt);
    virtual void think();
    virtual bool onStartMatch(bool networkControl);
    virtual void kickOff();
    virtual CFBPlayer* getHilightPlayer();
    virtual bool changeFormation(FBDefs::FORMATION formationId);
    virtual bool isAttacking() const { return m_state == FBDefs::TEAM_STATE::ATTACKING; }
    virtual bool isDefending() const { return m_state == FBDefs::TEAM_STATE::DEFENDING; }
    virtual void setAttacking(bool attacking) { m_state = attacking ? FBDefs::TEAM_STATE::ATTACKING : FBDefs::TEAM_STATE::DEFENDING; }
    
    virtual const vector<CFBPlayer*>& getTeamMembers() const { return m_teamMembers; }
    
    virtual float getLastPosOfPlayer() const { return m_lastPosOfPlayer; }
    
    virtual int getActivePlayer() const { return m_activePlayerId; }
    virtual int getAssistantPlayer() const { return m_assistantPlayerId; }
    virtual void setActivePlayer(int p) { m_activePlayerId = p; }
    virtual void setAssistantPlayer(int p) { m_assistantPlayerId = p; }
    
    virtual void updateFieldStatusOnAttack();
    virtual void updateFieldStatusOnDefend();
    
    virtual bool canShootDirectly(CFBPlayer* player);
    virtual int getNumberOfDefenderBetweenPlayerAndBall(CFBPlayer* player);
    virtual int getNumberOfDefenderAroundPlayer(CFBPlayer* player);
    
    virtual void setHilightPlayerId(int pid) { m_hilightPlayerId = pid; }
    virtual int getHilightPlayerId() { return m_hilightPlayerId; }
    
    virtual void switchHilightPlayer();
protected:
    FBDefs::TEAM_STATE m_state = FBDefs::TEAM_STATE::NONE;
    vector<CFBPlayer*> m_teamMembers;
    
    int m_score = 0;        // 比分
    float m_lastPosOfPlayer = 0.f;
    int m_activePlayerId = -1;   // 进攻时是控球队员，防守时是上前逼抢的球员
    int m_assistantPlayerId = -1;  // 进攻时是助攻接应球员（前锋除外），防守时是协防球员
    int m_hilightPlayerId = -1;     // 进攻方就是控球队员，防守方是当前可以控制移动的球员。
};


#endif /* defined(__FootBallX__CFBTeam__) */
