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
    CC_SYNTHESIZE(CFBBall*, m_ball, Ball);
    
    CFBMatch();
    ~CFBMatch();
    
    bool init(float pitchWidth, float pitchHeight);
    void update(float dt);
    
    bool startMatch(FBDefs::SIDE side = FBDefs::SIDE::LEFT);
    CFBTeam* getTeam(FBDefs::SIDE side);
    void setTeam(FBDefs::SIDE side, CFBTeam* team);
    
    CFBTeam* getPlayingTeam();
    CFBPlayer* getPlayingPlayer();
    
    bool isBallOnTheSide(FBDefs::SIDE side);
    float getBallPosRateBySide(FBDefs::SIDE side);
    
    void setOnAtkMenuCallback(function<void(const vector<int>&)> cb);
    void setOnDefMenuCallback(function<void(const vector<int>&)> cb);
    void setOnPlayAnimationCallback(function<void(const string& name, float delay)> cb);
    void pauseGame(bool p);
    
    bool checkEncounter(float dt);
    
    void tryPassBall(CFBPlayer* from, CFBPlayer* to);
    void updatePassBall(float dt);  // 负责播放传球动画
    
    void playAnimation(const string& name, float delay);
protected:
    CFBTeam* m_teams[(int)FBDefs::SIDE::NONE];
    
    function<void(const vector<int>&)> m_onAtkMenu;
    function<void(const vector<int>&)> m_onDefMenu;
    function<void(const string& name, float delay)> m_onPlayAnimation;
    
    float m_playerDistanceSq = FLT_MAX;
    
    float m_encounterTime = FLT_MAX;
    
    vector<int> m_defendPlayerIds;
    
    FBDefs::MATCH_EVENT_STATE m_eventState = FBDefs::MATCH_EVENT_STATE::NONE;
    
    CFBInstruction* m_currentInstruction = nullptr;
    
    void onInstructionEnd();
    
    bool m_isPause = false;
};

#define FBMATCH     (CFBMatch::getInstance())
#endif /* defined(__FootBallX__CFBMatch__) */
