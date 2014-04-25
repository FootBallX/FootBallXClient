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
#include "FBDefs.h"

class CFBFunctionsJS : public CSingleton<CFBFunctionsJS>
{
public:
    CFBFunctionsJS() = default;
    ~CFBFunctionsJS() = default;
    
    bool init();

    float getSpeed(const CFBCard& co);

    void startPassBall(const CFBCard& co1, bool isAir);
    void startShootBall(const CFBCard& co1, bool isAir);
    
    FBDefs::JS_RET_VAL tackleBall(const CFBCard& co1, const CFBCard& co2);
    FBDefs::JS_RET_VAL interceptBall(const CFBCard& co1, const CFBCard& co2);
    FBDefs::JS_RET_VAL blockBall(const CFBCard& co1, const CFBCard& co2);
    
    FBDefs::JS_RET_VAL hitBallGP(const CFBCard& co1, const CFBCard& co2);
    FBDefs::JS_RET_VAL catchBallGP(const CFBCard& co1, const CFBCard& co2);
    
    void receiveBall(const CFBCard& co1);
protected:
    jsval callJSFunc(const char* name, const char* format, ...);
    jsval createJsvalFromCard(const CFBCard& co);
private:
    JSContext* _cx = nullptr;
    JSObject* _go = nullptr;
};

#define FB_FUNC_JS      (CFBFunctionsJS::getInstance())

#endif /* defined(__FootBallX__CFBFunctionsJS__) */
