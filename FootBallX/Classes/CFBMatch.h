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


class CFBMatch : public CSingleton<CFBMatch>
{
public:

    CC_SYNTHESIZE_READONLY(CFBPitch*, m_pitch, Pitch);
    CC_SYNTHESIZE(CFBTeam*, m_redTeam, RedTeam);
    CC_SYNTHESIZE(CFBTeam*, m_blackTeam, BlackTeam);
    
    CFBMatch();
    ~CFBMatch();
    
    bool init();
    
    bool startMatch(bool redTeamKickoff = true);
    
    CFBTeam* getPlayingTeam();
    CFBPlayer* getPlayingPlayer();
protected:
    int m_redTeamScore = 0;
    int m_blackTeamScore = 0;
};

#define FBMATCH     (CFBMatch::getInstance())
#endif /* defined(__FootBallX__CFBMatch__) */
