//
//  IFBMatchUI.h
//  FootBallX
//
//  Created by 马 俊 on 14-4-12.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__IFBMatchUI__
#define __FootBallX__IFBMatchUI__

#include "Common.h"
#include "FBDefs.h"

class IFBMatchUI
{
public:
    virtual void onMenu(FBDefs::MENU_TYPE, bool, const vector<int>&) = 0;
    virtual void onPlayAnimation(const string&, float) = 0;
    virtual void onInstrunctionEnd(void) = 0;
    virtual void onPauseGame(bool) = 0;
    virtual void onGameEnd(void) = 0;
};

#endif /* defined(__FootBallX__IFBMatchUI__) */
