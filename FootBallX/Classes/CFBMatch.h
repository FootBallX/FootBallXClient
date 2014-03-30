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
#include "FBDefs.h"
#include "CFBInstruction.h"

class CFBMatch : public CSingleton<CFBMatch>
{
public:

    CC_SYNTHESIZE_READONLY(CFBPitch*, m_pitch, Pitch);
    
    CFBMatch();
    ~CFBMatch();
    
    bool init(float pitchWidth, float pitchHeight);
    void update(float dt);
    
    bool startMatch(FBDefs::SIDE side = FBDefs::SIDE::LEFT);
    void setControlSide(FBDefs::SIDE side);
    bool checkControlSide(FBDefs::SIDE side);
    CFBTeam* getControlSideTeam();
    FBDefs::SIDE getControlSide();
    
    CFBTeam* getTeam(FBDefs::SIDE side);
    void setTeam(FBDefs::SIDE side, CFBTeam* team);
    
    CFBTeam* getAttackingTeam();
    CFBTeam* getDefendingTeam();
    
    bool isBallOnTheSide(FBDefs::SIDE side);
    float getBallPosRateBySide(FBDefs::SIDE side);
    void setBallPosition(const Point& pos);
    const Point& getBallPosition();
    
    void setOnAtkMenuCallback(function<void(const set<int>&)> cb);
    void setOnDefMenuCallback(function<void(const set<int>&)> cb);
    void setOnPlayAnimationCallback(function<void(const string&, float)> cb);
    void setOnInstructionEnd(function<void(void)> cb);
    void setOnPauseGame(function<void(bool)> cb);
    
    void pauseGame(bool p);
    bool isPausing() { return m_isPause; }
    
    bool checkEncounter(float dt);
    
    void tryPassBall(CFBPlayer* from, CFBPlayer* to);
    void updatePassBall(float dt);  // 负责播放传球动画
    
    void playAnimation(const string& name, float delay);
protected:
    CFBBall* m_ball = nullptr;
    
    CFBTeam* m_teams[(int)FBDefs::SIDE::NONE];
    
    function<void(const set<int>&)> m_onAtkMenu;
    function<void(const set<int>&)> m_onDefMenu;
    function<void(const string&, float)> m_onPlayAnimation;
    function<void(void)> m_onInstructionEnd;
    function<void(bool)> m_onPauseGame;
    
    float m_playerDistanceSq = FLT_MAX;
    
    float m_encounterTime = FLT_MAX;
    
    set<int> m_defendPlayerIds;
    
    FBDefs::MATCH_EVENT_STATE m_eventState = FBDefs::MATCH_EVENT_STATE::NONE;
    
    CFBInstruction* m_currentInstruction = nullptr;
    
    void onInstructionEnd();
    
    bool m_isPause = false;
    
    FBDefs::SIDE m_controlSide = FBDefs::SIDE::NONE;
};

#define FBMATCH     (CFBMatch::getInstance())
#endif /* defined(__FootBallX__CFBMatch__) */
