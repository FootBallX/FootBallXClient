//
//  FBDefs.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-25.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef FootBallX_FBDefs_h
#define FootBallX_FBDefs_h

#include "Common.h"

class CFBPlayer;
class CFBPitchGrid;

namespace FBDefs
{
    static const float GOAL_KEEPER_LINE = 0.05f;
    // 进攻中，球在己方半场
    static const float ATK_DEF_BACK_LINE_MIN = 0.05f;
    static const float ATK_DEF_BACK_LINE_MAX = 0.2f;
    static const float ATK_DEF_HALF_BACK_LINE_MIN = 0.2f;
    static const float ATK_DEF_HALF_BACK_LINE_MAX = 0.4f;
    static const float ATK_DEF_FORWORD_LINE_MIN = 0.48f;
    static const float ATK_DEF_FORWORD_LINE_MAX = 0.8f;
    // 进攻中，球在对方半场
    static const float ATK_ATK_BACK_LINE_MIN = 0.5f;
    static const float ATK_ATK_BACK_LINE_MAX = 0.65f;
    static const float ATK_ATK_HALF_BACK_LINE_MIN = 0.65f;
    static const float ATK_ATK_HALF_BACK_LINE_MAX = 0.80f;
    static const float ATK_ATK_FORWORD_LINE_MIN = 0.85f;
    static const float ATK_ATK_FORWORD_LINE_MAX = 0.95f;
    
    // 防守中，球在己方半场
    static const float DEF_DEF_BACK_LINE_MIN = 0.05f;
    static const float DEF_DEF_BACK_LINE_MAX = 0.2f;
    static const float DEF_DEF_HALF_BACK_LINE_MIN = 0.2f;
    static const float DEF_DEF_HALF_BACK_LINE_MAX = 0.4f;
    static const float DEF_DEF_FORWORD_LINE_MIN = 0.4f;
    static const float DEF_DEF_FORWORD_LINE_MAX = 0.5f;
    // 防守中，球在对方半场
    static const float DEF_ATK_BACK_LINE_MIN = 0.1f;
    static const float DEF_ATK_BACK_LINE_MAX = 0.35f;
    static const float DEF_ATK_HALF_BACK_LINE_MIN = 0.35f;
    static const float DEF_ATK_HALF_BACK_LINE_MAX = 0.55f;
    static const float DEF_ATK_FORWORD_LINE_MIN = 0.65f;
    static const float DEF_ATK_FORWORD_LINE_MAX = 0.75f;
    
    // 球员碰撞距离
    static const float PLAYER_DISTANCE = 0.04f;
    
    // 抢球队员接近球多久后触发
    static const float PLAYER_ENCOUNTER_TRIGGER_TIME = 2.f;
    
    enum class SIDE
    {
        LEFT,
        RIGHT,
        NONE,
    };
    
    enum class FORMATION
    {
        F_4_4_2,
        F_3_5_2,
        NONE,
    };
    
    
    enum class TEAM_STATE
    {
        KICKOFF,
        ATTACKING,
        DEFENDING,
        NONE,
    };
    
    enum class AI_STATE
    {
        WAIT,
        BACKHOME,
        CONTROL,
        SUPPORT,
        CHASE,
        NONE,
    };
    
    enum class AI_STATE_SUPPORT
    {
        FIND_POS,
        MOVE_TO_POS,
        NONE,
    };
    
    enum class AI_STATE_CONTROL
    {
        DRIBBLE,
        NONE,
    };
    
    enum class MATCH_EVENT_STATE
    {
        PASS_BALL,
        PLAY_CLIP,
        NONE,
    };
    
    enum class MATCH_CLIP
    {
        PASS_BALL_SUCCESS,
        PASS_BALL_FAIL,
    };
    
    bool isPointOnTheWay(const cocos2d::Point& p1, const cocos2d::Point& p2, const cocos2d::Point& p);
    bool isPlayersOnTheWayToGoal(const vector<CFBPlayer*>& players, const CFBPitchGrid* grid);
    bool computeGridShootAngleAndMat(const cocos2d::Point& goalPos, CFBPitchGrid* grid);
}


#endif
