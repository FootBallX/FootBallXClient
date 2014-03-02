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

class CFBPlayer;
class CFBTeam;

#define GOALKEEPER_ORBIT_RATE                0.1f        // 10% pitch width
#define BACK_ORBIT_RATE                      0.15f       // 15%
#define HALF_BACK_ORBIT_RATE                 0.15f
#define FORWARD_ORBIT_RATE                   0.1f


#pragma mark ----- FormationBase

class CFBFormation
{
public:
    CFBFormation() = default;
    virtual ~CFBFormation();
    virtual const char* description() = 0;

    virtual bool init(CFBTeam* team);
    virtual void addPlayer(CFBPlayer* player, int pos) = 0;
    virtual CFBPlayer* getPlayer(int pos);
    
    virtual void update(float dt);
    
    virtual int getPlayerNumber() { return m_playerNumber;}
    
    virtual bool onStartMatch();
    
    virtual CFBPlayer* getKickOffPlayer() = 0;
    
    virtual CFBTeam* getTeam() const {return m_team;}
    
    virtual CFBPlayerAI* getPlayerAIById(int idx) { return m_playerAIs[idx]; }
    
    virtual FBDefs::FORMATION getFormationId() const { return m_formationId; }
protected:
    FBDefs::FORMATION m_formationId = FBDefs::FORMATION::NONE;
    CFBTeam* m_team;        // weak reference to the team object;
    int m_playerNumber = 0;
    CFBPlayerAI** m_playerAIs = nullptr;
    int m_updateIndex = -1;
    int m_updateCD = 0;
    vector<int> m_playerIndices;
};

#pragma mark ----- 442

class CFBFormation442
: public CFBFormation
{
public:
    CFBFormation442();
    virtual ~CFBFormation442() = default;
    virtual bool init(CFBTeam* team) override;
    virtual const char* description() override;
    virtual void addPlayer(CFBPlayer* player, int pos) override;
    virtual CFBPlayer* getKickOffPlayer() override {return getPlayer(9);}

protected:
};

#pragma mark ----- 352

class CFBFormation352
: public CFBFormation
{
public:
    CFBFormation352();
    virtual ~CFBFormation352() = default;
    virtual bool init(CFBTeam* team) override;
    virtual const char* description() override;
    virtual void addPlayer(CFBPlayer* player, int pos) override;
    virtual CFBPlayer* getKickOffPlayer() override {return getPlayer(9);}

protected:
};

#endif /* defined(__FootBallX__CFBFormation__) */
