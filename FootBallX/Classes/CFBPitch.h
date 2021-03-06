//
//  CFBPitch.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-10.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBPitch__
#define __FootBallX__CFBPitch__

#include "Common.h"
#include "CFBTeam.h"
#include "CFBPitchGrid.h"
#include "FBDefs.h"


class CFBPitch
{
public:
    cocos2d::Point transformBySide(cocos2d::Point pos, FBDefs::SIDE side = FBDefs::SIDE::LEFT);
    float transformBySide(float x, FBDefs::SIDE side = FBDefs::SIDE::LEFT);
    
    cocos2d::Point transToScreen(cocos2d::Point pos);
    
    bool init(int w, int h);
    int getPitchWidth() const { return m_width; }
    int getPitchHeight() const { return m_height; }
    
    int getGridWidth() const { return m_gridWidth; }
    int getGridHeight() const { return m_gridHeight; }
    
    const vector<CFBPitchGrid>& getGrids() const { return m_grids; }
    const CFBPitchGrid& getGrid(int index) const { return m_grids[index]; }
    void setGridScore(int index, int s) { m_grids[index].m_score = s; }
    void increaseGridScore(int index, int s) {m_grids[index].m_score += s; }
    
    void setGridDefenceScore(int index, int s) { m_grids[index].m_defenceScore = s; }
    void increaseGridDefenceScore(int index, int s) { m_grids[index].m_defenceScore += s; }
    
    int getGridScore(int index) { return m_grids[index].m_score; }
    const vector<int>& getGridsInPenaltyAreaBySide(FBDefs::SIDE side) const { return m_GridsInPenaltyArea[(int)side]; }
    const vector<int>& getGridsOutsidePenaltyAreaBySide(FBDefs::SIDE side) const { return m_GridsOutsidePenaltyArea[(int)side]; }
    
    bool getGridsAroundPosition(const cocos2d::Point& pos, vector<int>& out_grids);
    
    cocos2d::Point getBestSupportPosition(FBDefs::SIDE side);
    cocos2d::Point getBestAssistantDeffendingPosition(const cocos2d::Point& targetPos, FBDefs::SIDE side);
#ifdef SHOW_GRID
    void setGridDrawNode(int index, DrawNode* node)
    {
        m_grids[index].m_drawNode = node;
    }
    
    DrawNode* getGridDrawNode(int index)
    {
        return m_grids[index].m_drawNode;
    }
#endif
    
    void calcBestShootPosition(FBDefs::SIDE side);
    bool isOffside(cocos2d::Point pos, FBDefs::SIDE side);
    bool isInPenaltyArea(cocos2d::Point pos, FBDefs::SIDE side);
    
    FBDefs::SIDE getOtherSide(FBDefs::SIDE side) const;
    
    cocos2d::Point getGoalPos(FBDefs::SIDE side) const;
protected:
    int m_width = 0;
    int m_height = 0;
    cocos2d::Point m_Goals[int(FBDefs::SIDE::NONE)];
    cocos2d::Rect m_penaltyArea[int(FBDefs::SIDE::NONE)];
    
    const int m_gridWidth = 30;
    const int m_gridHeight = 15;
    
    vector<CFBPitchGrid> m_grids;
    vector<int> m_GridsOfSide[(int)FBDefs::SIDE::NONE];
    vector<int> m_GridsInPenaltyArea[(int)FBDefs::SIDE::NONE];
    vector<int> m_GridsOutsidePenaltyArea[(int)FBDefs::SIDE::NONE];
    
    float m_pitchScale = 0.f;
};

#endif /* defined(__FootBallX__CFBPitch__) */
