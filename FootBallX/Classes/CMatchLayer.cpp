//
//  CMatchLayer.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-11.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CMatchLayer.h"
#include "CGameSceneManager.h"
#include "CCPomelo.h"
#include "CFBMatch.h"
#include "CFlip3DEx.h"

#ifdef SHOW_GRID
#define TAG_GRID_DRAW_NODE          20001
#endif

static class CMatchLayerRegister
{
public:
    CMatchLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "CMatchLayer", CMatchLayerLoader::loader());
    }
} __reg;


#define TAG_ACTION_FLIP3D           1001


CMatchLayer::CMatchLayer()
{
    
}



CMatchLayer::~CMatchLayer()
{
    
}



bool CMatchLayer::init()
{
    do
    {
        BREAK_IF_FAILED(CBaseLayer::init());
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(CMatchLayer::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(CMatchLayer::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(CMatchLayer::onTouchEnded, this);
        listener->onTouchCancelled = CC_CALLBACK_2(CMatchLayer::onTouchCancelled, this);
        
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        m_screenCenter = this->getContentSize() / 2;
        this->scheduleUpdate();
        return true;
    } while (false);
    
    return false;
}



bool CMatchLayer::onTouchBegan(Touch* touch, Event* event)
{
    auto loc = touch->getLocation();
    
    switch (m_operator)
    {
        case OP::PASS_BALL:
        {
            int idx = getSelectedPlayerId(loc, true);
            if (idx != -1)
            {
                FBMATCH->getTeam(FBDefs::SIDE::LEFT)->passBall(idx);
            }
            break;
        }
        case OP::NONE:
            m_ballMovingVec = (loc - m_screenCenter).normalize();
            break;
    }
    
    return true;
}



void CMatchLayer::onTouchMoved(Touch* touch, Event* event)
{
    auto loc = touch->getLocation();
    switch (m_operator)
    {
        case OP::PASS_BALL:
            break;
        case OP::NONE:
            m_ballMovingVec = (loc - m_screenCenter).normalize();
            break;
    }
}



void CMatchLayer::onTouchEnded(Touch* touch, Event* event)
{
    m_ballMovingVec.setPoint(0, 0);
    m_isTouchDown = false;
}



void CMatchLayer::onTouchCancelled(Touch* touch, Event* event)
{
    m_ballMovingVec.setPoint(0, 0);
    m_isTouchDown = false;
}



SEL_MenuHandler CMatchLayer::onResolveCCBCCMenuItemSelector(Object * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPassBall", CMatchLayer::onPassBall);

    return nullptr;
}



//函数定义类型为：void pressTitle(Object *pSender);
Control::Handler CMatchLayer::onResolveCCBCCControlSelector(Object * pTarget, const char* pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE( this, "pressTitle", CMatchLayer::controlButtonTest);
    return nullptr;
}



bool CMatchLayer::onAssignCCBMemberVariable(Object* pTarget, const char* pMemberVariableName, Node* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b0", Sprite*, m_blackPlayers[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b1", Sprite*, m_blackPlayers[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b2", Sprite*, m_blackPlayers[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b3", Sprite*, m_blackPlayers[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b4", Sprite*, m_blackPlayers[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b5", Sprite*, m_blackPlayers[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b6", Sprite*, m_blackPlayers[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b7", Sprite*, m_blackPlayers[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b8", Sprite*, m_blackPlayers[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b9", Sprite*, m_blackPlayers[9]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b10", Sprite*, m_blackPlayers[10]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r0", Sprite*, m_redPlayers[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r1", Sprite*, m_redPlayers[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r2", Sprite*, m_redPlayers[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r3", Sprite*, m_redPlayers[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r4", Sprite*, m_redPlayers[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r5", Sprite*, m_redPlayers[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r6", Sprite*, m_redPlayers[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r7", Sprite*, m_redPlayers[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r8", Sprite*, m_redPlayers[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r9", Sprite*, m_redPlayers[9]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r10", Sprite*, m_redPlayers[10]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pitch", CSpriteEx*, m_pitchSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ball", Sprite*, m_ball);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "arrow", Sprite*, m_arrow);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menuPassBall", MenuItem*, m_menuPassBall);
    
    return false;
}


void CMatchLayer::onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader* pNodeLoader)
{
    for (auto x : m_blackPlayers)
    {
        x->setVisible(false);
    }
    for (auto x : m_redPlayers)
    {
        x->setVisible(false);
    }
    
    m_ball->setVisible(false);
    
    do
    {
        {
            auto ng = NodeGrid::create();
            FlipX3D* fx3 = CFlip3DYEx::create(1, 90, 90);
            fx3->setTag(TAG_ACTION_FLIP3D);
            ng->runAction(fx3);
            m_pitchSprite->getParent()->addChild(ng, -1);
            m_pitchSprite->removeFromParent();
            ng->addChild(m_pitchSprite);
            m_isPitchViewLieDown = true;
        }
        
        BREAK_IF_FAILED(FBMATCH->init());
        
        Size pitchSz = m_pitchSprite->getContentSize();
        FBMATCH->getPitch()->init(pitchSz.width, pitchSz.height);
        
        CFBTeam* red = new CFBTeam;
        BREAK_IF_FAILED(red->init());
        CFBTeam* black = new CFBTeam;
        BREAK_IF_FAILED(black->init());
        FBMATCH->setTeam(FBDefs::SIDE::LEFT, red);
        FBMATCH->setTeam(FBDefs::SIDE::RIGHT, black);
        
        BREAK_IF_FAILED(black->changeFormation(FBDefs::FORMATION::F_3_5_2))
        
        BREAK_IF_FAILED(FBMATCH->startMatch());
        
#ifdef SHOW_GRID
        auto draw = DrawNode::create();
        m_pitchSprite->addChild(draw, 10, TAG_GRID_DRAW_NODE);
        
        refreshGrids();
#endif // SHOW_GRID

    } while (false);
}



void CMatchLayer::update(float dt)
{
    CBaseLayer::update(dt);
    
    if (m_isPitchViewLieDown)
    {
        FBMATCH->update(dt);
        
        auto black = FBMATCH->getTeam(FBDefs::SIDE::LEFT);
        auto blackFmt = black->getFormation();
        int i;
        for (i = 0; i < blackFmt->getPlayerNumber(); ++i)
        {
            m_blackPlayers[i]->setVisible(true);
            m_blackPlayers[i]->setPosition(blackFmt->getPlayer(i)->m_curPosition);
        }
        
        auto red = FBMATCH->getTeam(FBDefs::SIDE::RIGHT);
        auto redFmt = red->getFormation();
        for (i = 0; i < redFmt->getPlayerNumber(); ++i)
        {
            m_redPlayers[i]->setVisible(true);
            m_redPlayers[i]->setPosition(redFmt->getPlayer(i)->m_curPosition);
        }
        
        auto ball = FBMATCH->getBall();

        if (m_ballMovingVec.equals(Point(0, 0)))
        {
            m_arrow->setVisible(false);
        }
        else
        {
            m_arrow->setVisible(true);
            Point base = {1, 0};
            float angle = CC_RADIANS_TO_DEGREES(m_ballMovingVec.getAngle(base));
            m_arrow->setRotation(angle);
            
            Point pos = ball->getBallPos();
            pos += m_ballMovingVec * ball->m_ownerPlayer->getSpeed() * dt;
            ball->setBallPos(pos);
        }
        
        m_ball->setVisible(true);
        m_ball->setPosition(ball->getBallPos());
        m_arrow->setPosition(ball->getBallPos());
    }
}



void CMatchLayer::togglePitchLieDown()
{
    if (m_isPitchViewLieDown)
    {
        m_isPitchViewLieDown = false;
        NodeGrid* ng = (NodeGrid*)m_pitchSprite->getParent();
        ng->stopActionByTag(TAG_ACTION_FLIP3D);
        auto fx = CFlip3DYEx::create(.5f, 90, 180);
        fx->setTag(TAG_ACTION_FLIP3D);
        ng->runAction(fx);
        m_operator = OP::PASS_BALL;
    }
    else
    {
        m_isPitchViewLieDown = true;
        NodeGrid* ng = (NodeGrid*)m_pitchSprite->getParent();
        ng->stopActionByTag(TAG_ACTION_FLIP3D);
        auto fx = CFlip3DYEx::create(.5f, 0, 90);
        fx->setTag(TAG_ACTION_FLIP3D);
        ng->runAction(fx);
        m_operator = OP::NONE;
    }
}



void CMatchLayer::onPassBall(Object* pSender)
{
    togglePitchLieDown();
    
    FBMATCH->getPitch()->calc(FBDefs::SIDE::RIGHT);
    FBMATCH->getPitch()->calc(FBDefs::SIDE::LEFT);
#ifdef SHOW_GRID
    refreshGrids();
#endif
}



int CMatchLayer::getSelectedPlayerId(const Point& pt, bool isBlack)
{
    auto mat = m_pitchSprite->getWorldToNodeTransform();
    Point pos = PointApplyTransform(pt, mat);
    
    if (isBlack)
    {
        for (auto spr : m_blackPlayers)
        {
            auto bx = spr->getBoundingBox();
            if (bx.containsPoint(pos))
            {
                return spr->getTag();
            }
        }
    }
    else
    {
        for (auto spr : m_redPlayers)
        {
            auto bx = spr->getBoundingBox();
            if (bx.containsPoint(pos))
            {
                return spr->getTag();
            }
        }
    }
    
    return -1;
}



#ifdef SHOW_GRID
void CMatchLayer::refreshGrids()
{
    auto pitch = FBMATCH->getPitch();
    auto& grids = pitch->getGrids();
    auto& InLeftPenalty = pitch->getGridsInPenaltyAreaBySide(FBDefs::SIDE::LEFT);
    auto& OutLeftPenalty = pitch->getGridsOutsidePenaltyAreaBySide(FBDefs::SIDE::LEFT);
    auto& InRightPenalty = pitch->getGridsInPenaltyAreaBySide(FBDefs::SIDE::RIGHT);
    auto& OutRightPenalty = pitch->getGridsOutsidePenaltyAreaBySide(FBDefs::SIDE::RIGHT);
 
    DrawNode* draw = (DrawNode*)m_pitchSprite->getChildByTag(TAG_GRID_DRAW_NODE);
    draw->clear();
    for (const auto& g: grids)
    {
        Color4F color = Color4F::GRAY;
        int size = 2;
        if (g.m_score > 0)
        {
            size = 5;
        }
        if (std::find(InLeftPenalty.begin(), InLeftPenalty.end(), g.m_index) == InLeftPenalty.end())
        {
            if (std::find(OutLeftPenalty.begin(), OutLeftPenalty.end(), g.m_index) == OutLeftPenalty.end())
            {
                if (std::find(InRightPenalty.begin(), InRightPenalty.end(), g.m_index) == InRightPenalty.end())
                {
                    if (std::find(OutRightPenalty.begin(), OutRightPenalty.end(), g.m_index) == OutRightPenalty.end())
                    {
                        
                    }
                    else
                    {
                        color.r = 0;
                        color.g = .5f;
                        color.b = 0;
                        color.a = 1;
                    }
                }
                else
                {
                    color.r = 0;
                    color.g = 1;
                    color.b = 0;
                    color.a = 1;
                }
            }
            else
            {
                color.r = 0;
                color.g = 0;
                color.b = .5f;
                color.a = 1;
            }
        }
        else
        {
            color.r = 0;
            color.g = 0;
            color.b = 1;
            color.a = 1;
        }
        
        draw->drawDot(g.m_coordinate, size, color);
        pitch->setGridDrawNode(g.m_index, draw);
    }

}
#endif
