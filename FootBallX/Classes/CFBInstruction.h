//
//  CFBInstruction.h
//  FootBallX
//
//  Created by 马 俊 on 14-3-14.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBInstruction__
#define __FootBallX__CFBInstruction__

#include "Common.h"
#include "CSingleton.h"
#include "FBDefs.h"

class CFBPlayer;
class CFBPassBallIns;
class CFBShootBallGroundIns;
class CFBShootBallAirIns;

class CFBInstruction
{
public:
    typedef void CALLBACK_TYPE(void);
    
    CFBInstruction() = default;
    virtual ~CFBInstruction() = default;

    virtual void update(float dt) = 0;
    virtual void start(function<CALLBACK_TYPE> callback);
    virtual void addPlayer(CFBPlayer* player);

    virtual void onAnimationEnd() = 0;
    
    virtual FBDefs::MATCH_FLOW_TYPE getInstructionType() = 0;
protected:
    virtual void onInstructionEnd();
    
    vector<CFBPlayer*> m_players;
    function<CALLBACK_TYPE> m_callback;
};

#pragma mark - factory

class CFBInstructionFactory : public CSingleton<CFBInstructionFactory>
{
public:
    CFBInstructionFactory() = default;
    ~CFBInstructionFactory();
    
    CFBPassBallIns* getPassBallIns();
    CFBShootBallAirIns* getShootBallAirIns();
    CFBShootBallGroundIns* getShootBallGroundIns();
protected:
    CFBPassBallIns* m_passBallIns = nullptr;
    CFBShootBallAirIns* m_shootBallAirIns = nullptr;
    CFBShootBallGroundIns* m_shootBallGroundIns = nullptr;
};


#define INS_FAC     (CFBInstructionFactory::getInstance())


#endif /* defined(__FootBallX__CFBInstruction__) */
