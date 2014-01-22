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
class CFBFormation;
class CFBPlayer;

class CFBTeam
{
public:
    CC_SYNTHESIZE_READONLY(CFBFormation*, m_formation, Formation);
    
    enum TEAM
    {
        BLACK,
        RED,
        NUM,
    };
    
    virtual bool init();
    virtual bool onStartMatch();
    virtual void kickOff();
protected:
private:
    vector<CFBPlayer*> m_teamMembers;
};


#endif /* defined(__FootBallX__CFBTeam__) */
