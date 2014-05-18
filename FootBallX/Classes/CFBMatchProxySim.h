//
//  CFBMatchProxySim.h
//  FootBallX
//
//  Created by 马 俊 on 14-4-9.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBMatchProxySim__
#define __FootBallX__CFBMatchProxySim__

#include "CFBMatchProxy.h"

class CFBMatchProxySim : public CFBMatchProxy
{
public:
    CFBMatchProxySim();
    virtual ~CFBMatchProxySim();
    
    virtual void setDelegator(IMatchProxyDelegator*) override;
    virtual void start() override;
    virtual void sendTeamPosition(const vector<float>&, int, int) override;
    virtual void sendMenuCmd(FBDefs::MENU_ITEMS, int) override;
    
    virtual void update(float dt) override;
    
    virtual unsigned int getTime() override;
    virtual float getDeltaTime(unsigned int) override;
protected:
};

#endif /* defined(__FootBallX__CFBMatchProxySim__) */
