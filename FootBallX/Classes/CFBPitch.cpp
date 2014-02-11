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

Point CFBPitch::transformPersentage(const Point& pt, FBDefs::SIDE side)
{
    Point point(pt);
    if (side == FBDefs::SIDE::RIGHT)
    {
        point.x = 1 - point.x;
        point.y = 1 - point.y;
    }
    return Point(point.x * m_width, point.y * m_height);
}



float CFBPitch::transformPersentage(float p)
{
    return p * m_width;
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
                grid.m_coordinate.setPoint((1.5f + x) * gpw, (1.5f + y) * gph);
                grid.m_index = index;
                
                if (x < midW)
                {
                    int side = (int)FBDefs::SIDE::LEFT;
                    m_GridsOfSide[side].push_back(index);
                    if (m_penaltyArea[side].containsPoint(grid.m_coordinate))
                    {
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
                    m_GridsOfSide[side].push_back(index);
                    if (m_penaltyArea[side].containsPoint(grid.m_coordinate))
                    {
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



void CFBPitch::calc(FBDefs::SIDE side)
{
    FBDefs::SIDE otherSide;
    Point goalGate = m_Goals[(int)side];
    goalGate.y = m_height * 0.5f;
    
    switch (side)
    {
        case FBDefs::SIDE::LEFT:
            otherSide = FBDefs::SIDE::RIGHT;
            break;
        case FBDefs::SIDE::RIGHT:
            otherSide = FBDefs::SIDE::LEFT;
            break;
        default:
            return;
            break;
    }
    
    auto vs = getGridsInPenaltyAreaBySide(side);
    auto team = FBMATCH->getTeam(side);
    auto& players = team->getTeamMembers();
    
    for (auto& x : vs)
    {
        auto& grid = getGrid(x);
        setGridScore(x, 0);
        if (!FBDefs::isPlayersOnTheWay(players, grid.m_coordinate, goalGate))
        {
            increaseGridScore(x, 10);
        }
    }
}




