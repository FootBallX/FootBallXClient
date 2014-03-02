//
//  CFBMatch.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-21.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBMatch__
#define __FootBallX__CFBMatch__

#include "Common.h"
#include "CSingleton.h"
#include "CFBPitch.h"
#include "CFBTeam.h"
#include "CFBFormation.h"
#include "CFBPlayer.h"
#include "CFBBall.H"

class CFBMatch : public CSingleton<CFBMatch>
{
public:

    CC_SYNTHESIZE_READONLY(CFBPitch*, m_pitch, Pitch);
    CC_SYNTHESIZE(CFBBall*, m_ball, Ball);
    
    CFBMatch();
    ~CFBMatch();
    
    bool init();
    void update(float dt);
    
    bool startMatch(FBDefs::SIDE side = FBDefs::SIDE::LEFT);
    CFBTeam* getTeam(FBDefs::SIDE side);
    void setTeam(FBDefs::SIDE side, CFBTeam* team);
    
    CFBTeam* getPlayingTeam();
    CFBPlayer* getPlayingPlayer();
    
    bool isBallOnTheSide(FBDefs::SIDE side);
    float getBallPosRateBySide(FBDefs::SIDE side);
protected:
    CFBTeam* m_teams[(int)FBDefs::SIDE::NONE];
};

#define FBMATCH     (CFBMatch::getInstance())
#endif /* defined(__FootBallX__CFBMatch__) */
