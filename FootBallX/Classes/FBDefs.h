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
    enum class FORMATION
    {
        F_4_4_2,
        F_3_2_3_2,
        NONE,
    };
    
    static Point InitFormation[(int)FORMATION::NONE][11] = {
        // F_4_4_2
        {
            Point(25, 325),
            Point(140, 120), Point(140, 222.5), Point(140, 427.5), Point(140, 530),
            Point(300, 120), Point(300, 222.5), Point(300, 427.5), Point(300, 530),
            Point(440, 260), Point(500, 330),
        },
        // F_3_2_3_2
        {
            Point(25, 325),
            Point(140, 190), Point(140, 325), Point(140, 460),
            Point(240, 230), Point(240, 420),
            Point(340, 190), Point(340, 325), Point(340, 460),
            Point(440, 260), Point(500, 330),
        },
    };
    
    static Point HomeFormation[(int)FORMATION::NONE][11] = {
        // F_4_4_2
        {
            Point(0, 325),
            Point(0, 120), Point(0, 222.5), Point(0, 427.5), Point(0, 530),
            Point(0, 120), Point(0, 222.5), Point(0, 427.5), Point(0, 530),
            Point(0, 260), Point(0, 330),
        },
        // F_3_2_3_2
        {
            Point(0, 325),
            Point(0, 190), Point(0, 325), Point(0, 460),
            Point(-50, 230), Point(-50, 420),
            Point(50, 190), Point(50, 325), Point(50, 460),
            Point(0, 260), Point(0, 330),
        },
    };
    
    static const float OFFSET_Y = 100;          // 无球球员横向移动的便宜范围
    static const float PASS_BALL_REDUCTION = 200;   // 传球的衰减距离，超过这个距离的传球可能性会减小
    static const float DEFENDER_PLAYER_RADIUS = 100; // 检查这个半径范围内的对方球员为防守球员
    static const float DRIBBLE_CHECK_DIST = 200;    // 传球的检查长度，在这个长度的前方范围没有对方球员则可以传球
    static const float ASSIST_DEFEND_DIST = 75;  // 协防球员与控球球员间保持的一定距离
    
    static const float PITCH_WIDTH = 1000;
    static const float PITCH_HEIGHT = 650;
    
    static const float GOAL_KEEPER_LINE = 50;
    // 进攻中，球在己方半场
    static const float ATK_DEF_BACK_LINE_MIN = 50;
    static const float ATK_DEF_BACK_LINE_MAX = 200;
    static const float ATK_DEF_HALF_BACK_LINE_MIN = 200;
    static const float ATK_DEF_HALF_BACK_LINE_MAX = 400;
    static const float ATK_DEF_FORWORD_LINE_MIN = 480;
    static const float ATK_DEF_FORWORD_LINE_MAX = 800;
    // 进攻中，球在对方半场
    static const float ATK_ATK_BACK_LINE_MIN = 500;
    static const float ATK_ATK_BACK_LINE_MAX = 650;
    static const float ATK_ATK_HALF_BACK_LINE_MIN = 650;
    static const float ATK_ATK_HALF_BACK_LINE_MAX = 800;
    static const float ATK_ATK_FORWORD_LINE_MIN = 850;
    static const float ATK_ATK_FORWORD_LINE_MAX = 950;
    
    // 防守中，球在己方半场
    static const float DEF_DEF_BACK_LINE_MIN = 50;
    static const float DEF_DEF_BACK_LINE_MAX = 200;
    static const float DEF_DEF_HALF_BACK_LINE_MIN = 200;
    static const float DEF_DEF_HALF_BACK_LINE_MAX = 400;
    static const float DEF_DEF_FORWORD_LINE_MIN = 400;
    static const float DEF_DEF_FORWORD_LINE_MAX = 500;
    // 防守中，球在对方半场
    static const float DEF_ATK_BACK_LINE_MIN = 100;
    static const float DEF_ATK_BACK_LINE_MAX = 350;
    static const float DEF_ATK_HALF_BACK_LINE_MIN = 350;
    static const float DEF_ATK_HALF_BACK_LINE_MAX = 550;
    static const float DEF_ATK_FORWORD_LINE_MIN = 650;
    static const float DEF_ATK_FORWORD_LINE_MAX = 750;
    
    // 球员碰撞距离
    static const float PLAYER_DISTANCE = 40;
    
    // 抢球队员接近球多久后触发
    static const float PLAYER_ENCOUNTER_TRIGGER_TIME = 1.f;
    
    enum class SIDE
    {
        LEFT,
        RIGHT,
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

    
    enum class PLAYER_INS
    {
        TAKCLE,
        BLOCK,
        INTERCEPT,
        HIT,
        TAKE,
        NONE,
    };
    
    
    enum class JS_RET_VAL
    {
        FAIL = 0,
        SUCCESS = 1,
        RANDOM_BALL = 2,
        NONE,
    };
    
    
    enum MENU_TYPE
    {
        DEFAULT_ATK_G,      // 控球方 地面带球中断: 传球 射门 二过一
        ENCOUNTER_ATK_G,    // 控球方 地面遭遇: 盘带 传球 射门 二过一
        ONE_ZERO_ATK_G,     // 控球方 单刀: 射门 盘带
        ENCOUNTER_ATK_OPPSITE_A,  // 控球方 对方禁区内半空遭遇: 盘带 传球 射门
        ENCOUNTER_ATK_SELF_A,  // 控球方 己方禁区内半空遭遇: 传球 解围
        ENCOUTNER_DEF_G,    // 防守方 地面遭遇: 铲球 拦截 封堵
        ENCOUNTER_DEF_SELF_A,    // 防守方 己方禁区内半空遭遇: 解围 拦截 封堵
        ENCOUNTER_DEF_OPPSITE_A, // 防守方 对方禁区半空遭遇: 拦截 封堵
        GOAL_KEEPER_DEF_G,  // 防守方 守门员防守: 接球 击球
        ONE_ZERO_DEF_G,     // 防守方 单刀门将: 封堵盘带 封堵射门
        GOAL_KEEPER_DEF_A,  // 防守方 守门员空中遭遇: 出击 待机
        NONE,
    };
    
    enum class MATCH_FLOW_TYPE      // 相对应CFBInstruction的类型
    {
        PASSBALL,
        SHOOT_GROUND,
        SHOOT_AIR,
        NONE,
    };
    
    bool isPointOnTheWay(const cocos2d::Point& p1, const cocos2d::Point& p2, const cocos2d::Point& p);
    bool isPlayersOnTheWayToGoal(const vector<CFBPlayer*>& players, const CFBPitchGrid* grid);
    bool computeGridShootAngleAndMat(const cocos2d::Point& goalPos, CFBPitchGrid* grid);
}


#endif
