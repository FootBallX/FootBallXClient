//
//  CFBFunctionsJS.h
//  FootBallX
//
//  Created by 马 俊 on 14-3-8.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBFunctionsJS__
#define __FootBallX__CFBFunctionsJS__

#include "Common.h"
#include "CSingleton.h"
#include "CFBCard.h"

class CFBFunctionsJS : public CSingleton<CFBFunctionsJS>
{
public:
    CFBFunctionsJS() = default;
    ~CFBFunctionsJS() = default;
    
    bool init();

    float getSpeed(const CFBCard& co);

    void startPassBall(const CFBCard& co1);
    bool tackleBall(const CFBCard& co1, const CFBCard& co2);
    bool interceptBall(const CFBCard& co1, const CFBCard& co2);
    bool blockBall(const CFBCard& co1, const CFBCard& co2);
    void recieveBall(const CFBCard& co1);
protected:
    jsval callJSFunc(const char *name, int count, ...);
    jsval callJSFunc2Obj(const char *name, const CFBCard& c1, const CFBCard& c2);
    jsval callJSFunc1Obj(const char *name, const CFBCard& c1);
    jsval createJsvalFromCard(const CFBCard& co);
private:
    JSContext* _cx = nullptr;
    JSObject* _go = nullptr;
};

#define FB_FUNC_JS      (CFBFunctionsJS::getInstance())

#endif /* defined(__FootBallX__CFBFunctionsJS__) */
