//
//  CFBPitch.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-10.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBPitch.h"
#include "CFBMatch.h"
#include "kazmath/kazmath.h"
#include "CRandomManager.h"

Point CFBPitch::transformPersentage(const Point& pt, FBDefs::SIDE side)
{
    Point point(pt);
    if (side == FBDefs::SIDE::RIGHT)
    {
        point.x = 1 - point.x;
        point.y = point.y;
    }
    return Point(point.x * m_width, point.y * m_height);
}



float CFBPitch::transformPersentageX(float x, FBDefs::SIDE side)
{
    if (side == FBDefs::SIDE::RIGHT)
    {
        x = 1 - x;
    }
    return x * m_width;
}



float CFBPitch::transformToPersentageX(float x, FBDefs::SIDE side)
{
    float rate = x / (float)m_width;
    
    if (side == FBDefs::SIDE::RIGHT)
    {
        rate = 1 - rate;
    }
    
    return rate;
}


bool CFBPitch::init(int w, int h)
{
    do
    {
        m_width = w;
        m_height = h;
        
        {
            Rect& rc = m_penaltyArea[(int)FBDefs::SIDE::LEFT];
            rc.origin.x = w * .02f;
            rc.origin.y = h * .27f;
            rc.size.width = w * .17f;
            rc.size.height = h * .46f;
        }
        {
            Rect& rc = m_penaltyArea[(int)FBDefs::SIDE::RIGHT];
            rc.origin.x = w * .78f;
            rc.origin.y = h * .27f;
            rc.size.width = w * .17f;
            rc.size.height = h * .46f;
        }
        
        {
            Point& pt = m_Goals[(int)FBDefs::SIDE::LEFT];
            pt.x = w * .02f;
            pt.y = .1f;     // range
        }
        
        {
            Point& pt = m_Goals[(int)FBDefs::SIDE::RIGHT];
            pt.x = w * .98f;
            pt.y = .1f;
        }
        
        auto gw = m_gridWidth - 2;
        auto gh = m_gridHeight - 2;
        
        float midW = gw / 2.f - 0.5f;
        
        auto gpw = w / m_gridWidth;
        auto gph = h / m_gridHeight;
        
        m_grids.resize(gw * gh);
        for (int y = 0; y < gh; ++y)
        {
            for (int x = 0; x < gw; ++x)
            {
                int index = x + gw * y;
                auto& grid = m_grids[index];
                grid.m_position.setPoint((1.5f + x) * gpw, (1.5f + y) * gph);
                grid.m_index = index;
                
                if (x < midW)
                {
                    int side = (int)FBDefs::SIDE::LEFT;
                    Point goalGate = m_Goals[(int)side];
                    goalGate.y = m_height * 0.5f;
                    
                    m_GridsOfSide[side].push_back(index);
                    if (m_penaltyArea[side].containsPoint(grid.m_position))
                    {
                        FBDefs::computeGridShootAngleAndMat(goalGate, &grid);
                        m_GridsInPenaltyArea[side].push_back(index);
                    }
                    else
                    {
                        m_GridsOutsidePenaltyArea[side].push_back(index);
                    }
                }
                else if (x > midW)
                {
                    int side = (int)FBDefs::SIDE::RIGHT;
                    Point goalGate = m_Goals[(int)side];
                    goalGate.y = m_height * 0.5f;
                    
                    m_GridsOfSide[side].push_back(index);
                    if (m_penaltyArea[side].containsPoint(grid.m_position))
                    {
                        FBDefs::computeGridShootAngleAndMat(goalGate, &grid);
                        m_GridsInPenaltyArea[side].push_back(index);
                    }
                    else
                    {
                        m_GridsOutsidePenaltyArea[side].push_back(index);
                    }
                }
            }
        }
        return true;
    } while (false);
    
    return false;
}



void CFBPitch::calcBestShootPosition(FBDefs::SIDE side)
{
    Point goalGate = m_Goals[(int)side];
    goalGate.y = m_height * 0.5f;
    
    auto vs = getGridsInPenaltyAreaBySide(side);
    auto team = FBMATCH->getTeam(side);
    auto& players = team->getTeamMembers();
    
    for (auto& x : vs)
    {
        auto& grid = getGrid(x);
        setGridScore(x, 0);
        
        if (!isOffside(grid.m_position, side))
        {
//            log("angle: %d", grid.m_shootAngleScore);
            increaseGridScore(x, grid.m_shootAngleScore);
            if (!FBDefs::isPlayersOnTheWayToGoal(players, &grid))
            {
                increaseGridScore(x, 10);
            }
        }
        else
        {
            increaseGridScore(x, -1000);
        }
    }
}



FBDefs::SIDE CFBPitch::getOtherSide(FBDefs::SIDE side) const
{

    switch (side)
    {
        case FBDefs::SIDE::LEFT:
            return FBDefs::SIDE::RIGHT;
        case FBDefs::SIDE::RIGHT:
            return FBDefs::SIDE::LEFT;
        default:
            return FBDefs::SIDE::NONE;
    }
    
    return FBDefs::SIDE::NONE;
}



bool CFBPitch::isOffside(cocos2d::Point pos, FBDefs::SIDE side)
{
    float offsidePos = FBMATCH->getTeam(side)->getLastPosOfPlayer();
    switch (side)
    {
        case FBDefs::SIDE::LEFT:
            if (pos.x < offsidePos)
            {
                return true;
            }
            break;
        case FBDefs::SIDE::RIGHT:
            if (pos.x > offsidePos)
            {
                return true;
            }
            break;
            
        default:
            break;
    }
    
    return false;
}



Point CFBPitch::getBestSupportPosition(FBDefs::SIDE side)
{
    this->calcBestShootPosition(side);
    
    Point pt;
    
    FBDefs::SIDE otherSide = this->getOtherSide(side);
    auto grids = this->getGridsInPenaltyAreaBySide(otherSide);
    
    std::sort(grids.begin(), grids.end(), [=](int a, int b){
        return this->getGrid(a).m_score > this->getGrid(b).m_score;
    });
    
    int r = RANDOM_MGR->getRand() % std::min((int)grids.size(), 5);

    auto& g = this->getGrid(grids[r]);
    return g.m_position;
}



Point CFBPitch::getGoalPos(FBDefs::SIDE side) const
{
    Point pt = m_Goals[(int)side];
    pt.y = 0.5 * m_height;
    return pt;
}



Point CFBPitch::getBestAssistantDeffendingPosition(const Point& targetPos, FBDefs::SIDE side)
{
    Point gp = getGoalPos(side);
    gp = gp - targetPos;
    gp = gp.normalize();
    
    float dist = 0.075;

    return targetPos + (gp * transformPersentageX(dist));
}



bool CFBPitch::getGridsAroundPosition(const Point& pos, vector<int>& out_grids)
{
    out_grids.clear();
    
    auto gw = m_gridWidth - 2;
    auto gh = m_gridHeight - 2;
    
    auto gpw = m_width / m_gridWidth;
    auto gph = m_height / m_gridHeight;
    
    int origX = (pos.x / gpw) - 1;
    int origY = (pos.y / gph) - 1;
    
    auto func = [&](int x, int y)
    {
        if (x >= 0 && x < gw && y >= 0 && y < gh)
        {
            out_grids.push_back(x + gw * y);
        }
    };
    
    for (int i = -2; i <= 2; ++i)
    {
        for (int j = -2; j <= 2; ++j)
        {
            func(origX + i, origY + j);
        }
    }

    return out_grids.size() > 0;
}

