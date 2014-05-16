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
#include "CFBMatchProxy.h"
#include "IFBMatchUI.h"

class CFBMatch : public CSingleton<CFBMatch>
{
public:
    CC_SYNTHESIZE_READONLY(CFBPitch*, m_pitch, Pitch);
    
    CFBMatch();
    ~CFBMatch();
    
    bool init(float pitchWidth, float pitchHeight, IFBMatchUI* matchUI, CFBMatchProxy* proxy);
    void update(float dt);
    
    bool startMatch();
    void setControlSide(FBDefs::SIDE side);
    bool checkControlSide(FBDefs::SIDE side);
    CFBTeam* getControlSideTeam();
    FBDefs::SIDE getControlSide();
    
    CFBTeam* getTeam(FBDefs::SIDE side);
    CFBTeam* getOtherTeam(CFBTeam* team);
    void setTeam(FBDefs::SIDE side, CFBTeam* team);
    
    CFBTeam* getAttackingTeam();
    CFBTeam* getDefendingTeam();
    
    bool isBallOnTheSide(FBDefs::SIDE side);
    void setBallPosition(const cocos2d::Point& pos);
    float getBallPosRateBySide(FBDefs::SIDE side);
    const cocos2d::Point& getBallPosition();
    
    void pauseGame(bool p);
    bool isPausing() { return m_isPause; }
    
    void tryPassBall(CFBPlayer* from, CFBPlayer* to);
    void tryShootBall(CFBPlayer* player, bool isAir);
    
    void playAnimation(const string& name, float delay);
    void onAnimationEnd();
    
    FBDefs::MATCH_STEP getMatchStep();
    void setBallControllerMove(const cocos2d::Point& vec);
    
    int getCountDownTime();
    unsigned int getTime();

    void setMenuItem(FBDefs::MENU_ITEMS mi, int targetPlayer = -1);
protected:
    IFBMatchUI* m_matchUI = nullptr;
    CFBBall* m_ball = nullptr;
    
    CFBTeam* m_teams[(int)FBDefs::SIDE::NONE];
    
    float m_playerDistanceSq = FLT_MAX;
    
    float m_encounterTime = FLT_MAX;
    FBDefs::MENU_TYPE m_menuType = FBDefs::MENU_TYPE::NONE;
    bool m_isAir = false;
    
    FBDefs::MATCH_FLOW_TYPE m_recentEndedFlow = FBDefs::MATCH_FLOW_TYPE::NONE;
    
    set<int> m_defendPlayerIds;
    
    vector<int> m_involvePlayerIds;
    
    CFBInstruction* m_currentInstruction = nullptr;

    bool m_isPause = false;
    
    FBDefs::SIDE m_controlSide = FBDefs::SIDE::NONE;
    
    Point m_vecFromUser;        // 玩家当前操作的缓存
    
#pragma mark - Instructinos
    vector<FBDefs::MENU_ITEMS> m_playerInstructions;    // 玩家指令
    vector<int> m_attackPlayerNumbers;
    vector<int> m_defendPlayerNumbers;
    int m_targetPlayerId = -1;       // 仅传球时候有效，传球对象
    
#pragma mark - net or sim
    void syncTeam();
    void teamPositionAck(const vector<float>& p, int ballPlayerId, unsigned int timestamp);
    void startMatchAck(const vector<vector<float>>& allPos, FBDefs::SIDE mySide, FBDefs::SIDE kickOffSide, unsigned int st);
    void endMatchAck();
    void triggerMenuAck(FBDefs::MENU_TYPE menuType, vector<int>& attackPlayerNumbers, vector<int>& defendPlayerNumbers);
    void instructionAck(unsigned int countDown);
    void instructionResultAck(const CFBInstructionResult&);
    
    enum class SIDE
    {
        SELF,       // 自己
        OPP,        // 对方
        NONE,
    };
    const float m_SYNC_TIME = 1.0f;
    
    CFBMatchProxy* m_proxy = nullptr;
    
    CFBTeam* m_teamsInMatch[(int)SIDE::NONE];       // 这里重新组织一下，按照己方和对方保存team
    float m_syncTime[(int)SIDE::NONE];
    unsigned int m_startTime = 0;

    void onInstructionEnd();
    
    void updateEncounter(float dt);
    void checkEncounterInDribble();
    void checkEncounterInPenaltyArea();
    void updateDefendPlayerAroundBall();
    
    FBDefs::MATCH_STEP m_matchStep = FBDefs::MATCH_STEP::NONE;
};

#define FBMATCH     (CFBMatch::getInstance())
#endif /* defined(__FootBallX__CFBMatch__) */
