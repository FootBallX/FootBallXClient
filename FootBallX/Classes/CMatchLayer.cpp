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
#include "CFBAnimationLayer.h"
#include "CPlayerInfo.h"
#include "CFBMatchProxyNet.h"
#include "CFBMatchProxySim.h"
#include "CCBReadHelper.h"

#ifdef SHOW_GRID
#define TAG_GRID_DRAW_NODE          20001
#endif

enum class Z_ORDER : int
{
    PITCH,
    ANIMATION,
#ifdef SHOW_GRID
    GRID_DEBUG,
#endif
    COUNT_DOWN,
    MENU,
#ifdef DEBUG
    STAT,
#endif
};



#define TAG_ACTION_FLIP3D           1001
#define TAG_COUNT_DOWN              1002
#ifdef DEBUG
#define TAG_STAT                    1003
#endif

static class CMatchLayerRegister
{
public:
    CMatchLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "CMatchLayer", CMatchLayerLoader::loader());
    }
} __reg;



CMatchLayer::CMatchLayer()
{
}

CMatchLayer::~CMatchLayer()
{
}

void CMatchLayer::onEnter()
{
    CBaseLayer::onEnter();
}

void CMatchLayer::onExit()
{
    CBaseLayer::onExit();
}

SEL_MenuHandler CMatchLayer::onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_MENU_ITEM_BEGIN
    // AUTO_GEN_MENU_ITEM_END
    return nullptr;
}



Control::Handler CMatchLayer::onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_CONTROL_BEGIN
    // AUTO_GEN_CONTROL_END
    return nullptr;
}



bool CMatchLayer::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode)
{
    // AUTO_GEN_VAR_BEGIN
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "pitch", Sprite*, this->m_pitch);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b0", Sprite*, this->m_b0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b1", Sprite*, this->m_b1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b2", Sprite*, this->m_b2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b3", Sprite*, this->m_b3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b4", Sprite*, this->m_b4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b5", Sprite*, this->m_b5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b6", Sprite*, this->m_b6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b7", Sprite*, this->m_b7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b8", Sprite*, this->m_b8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b9", Sprite*, this->m_b9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "b10", Sprite*, this->m_b10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r0", Sprite*, this->m_r0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r1", Sprite*, this->m_r1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r2", Sprite*, this->m_r2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r3", Sprite*, this->m_r3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r4", Sprite*, this->m_r4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r5", Sprite*, this->m_r5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r6", Sprite*, this->m_r6);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r7", Sprite*, this->m_r7);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r8", Sprite*, this->m_r8);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r9", Sprite*, this->m_r9);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "r10", Sprite*, this->m_r10);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "ball", Sprite*, this->m_ball);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "arrow", Sprite*, this->m_arrow);
    // AUTO_GEN_VAR_END
	return false;
}



void CMatchLayer::onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader)
{
    m_players[(int)FBDefs::SIDE::LEFT] = {m_b0, m_b1, m_b2, m_b3, m_b4, m_b5, m_b6, m_b7, m_b8, m_b9, m_b10};
    m_players[(int)FBDefs::SIDE::RIGHT] = {m_r0, m_r1, m_r2, m_r3, m_r4, m_r5, m_r6, m_r7, m_r8, m_r9, m_r10};
    
    for (auto v :m_players)
    {
        int i = 0;
        for (auto x : v)
        {
            CC_ASSERT(x != nullptr);
            x->setVisible(false);
            x->setTag(i);
            ++i;
        }
    }

    m_ball->setVisible(false);
    
    do
    {
        {
            auto ng = NodeGrid::create();
            FlipX3D* fx3 = CFlip3DYEx::create(1, 90, 90);
            fx3->setTag(TAG_ACTION_FLIP3D);
            ng->runAction(fx3);
            m_pitch->getParent()->addChild(ng, (int)Z_ORDER::PITCH);
            m_pitch->removeFromParent();
            ng->addChild(m_pitch);
            m_isPitchViewLieDown = true;
        }
        
        Size pitchSz = m_pitch->getContentSize();
        
        const char* FONT_NAME = "Helvetica";
        const int FONT_SIZE = 32 * cocosbuilder::CCBReader::getResolutionScale();
        
        auto countDownSpr = Label::createWithSystemFont("", FONT_NAME, FONT_SIZE);
        addChild(countDownSpr, (int)Z_ORDER::COUNT_DOWN, TAG_COUNT_DOWN);
        countDownSpr->setVisible(false);
        auto sz = Director::getInstance()->getWinSize();
        countDownSpr->setPosition(sz.width * 0.5f, sz.height * 0.5f);
        
#ifdef DEBUG
        auto stat = Label::createWithSystemFont("", FONT_NAME, 18 * cocosbuilder::CCBReader::getResolutionScale());
        stat->setAnchorPoint(Point(0, 1));
        stat->setPosition(0, sz.height);
        addChild(stat, (int)Z_ORDER::STAT, TAG_STAT);
#endif
        
        BREAK_IF_FAILED(FBMATCH->init(pitchSz.width, pitchSz.height, this, new CFBMatchProxyNet()));
        BREAK_IF_FAILED(FBMATCH->startMatch());
#ifdef SHOW_GRID
        auto draw = DrawNode::create();
        m_pitch->addChild(draw, (int)Z_ORDER::GRID_DEBUG, TAG_GRID_DRAW_NODE);
        
        refreshGrids();
#endif // SHOW_GRID
        
    } while (false);
    
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
    if (FBMATCH->getMatchStep() != FBDefs::MATCH_STEP::MATCHING)
    {
        return true;
    }
    
    auto team = FBMATCH->getControlSideTeam();
    if (team)
    {
        auto loc = touch->getLocation();
        switch (m_operator)
        {
            case OP::PASS_BALL:
            {
                int idx = getSelectedPlayerId(loc, FBMATCH->getControlSide());
                if (idx != -1)
                {
                    FBMATCH->setMenuItem(FBDefs::MENU_ITEMS::Pass, idx);
                    m_operator = OP::NONE;
                }
                break;
            }
            case OP::NONE:
                if (!FBMATCH->isPausing())
                {
                    FBMATCH->setBallControllerMove((loc - m_screenCenter).normalize());
                }
                break;
            default:
                break;
        }
    }
    
    return true;
}



void CMatchLayer::onTouchMoved(Touch* touch, Event* event)
{
    if (FBMATCH->isPausing()) return;
    
    auto team = FBMATCH->getControlSideTeam();
    if (team)
    {
        auto loc = touch->getLocation();
        switch (m_operator)
        {
            case OP::PASS_BALL:
                break;
            case OP::NONE:
                FBMATCH->setBallControllerMove((loc - m_screenCenter).normalize());
                break;
            default:
                break;
        }
    }
}



void CMatchLayer::onTouchEnded(Touch* touch, Event* event)
{
    FBMATCH->setBallControllerMove(Point());
    m_isTouchDown = false;
}



void CMatchLayer::onTouchCancelled(Touch* touch, Event* event)
{
    FBMATCH->setBallControllerMove(Point());
    m_isTouchDown = false;
}




void CMatchLayer::update(float dt)
{
    CBaseLayer::update(dt);
    
#ifdef DEBUG
    {
        auto stat = (Label*)getChildByTag(TAG_STAT);
        char ss[100];
        sprintf(ss, "time: %u", FBMATCH->getTime());
        stat->setString(ss);
    }
#endif
    
    FBMATCH->update(dt);
    
    switch (FBMATCH->getMatchStep())
    {
        case FBDefs::MATCH_STEP::WAIT_START:
            break;
        case FBDefs::MATCH_STEP::COUNT_DOWN:
        {
            char s[25];
            sprintf(s, "%u", FBMATCH->getCountDownTime());
            auto spr = (Label*)getChildByTag(TAG_COUNT_DOWN);
            spr->setVisible(true);
            spr->setString(s);
            break;
        }
        case FBDefs::MATCH_STEP::MATCHING:
        {
            auto spr = getChildByTag(TAG_COUNT_DOWN);
            spr->setVisible(false);
            
            updateTeam(FBMATCH->getTeam(FBDefs::SIDE::LEFT), m_players[(int)FBDefs::SIDE::LEFT]);
            updateTeam(FBMATCH->getTeam(FBDefs::SIDE::RIGHT), m_players[(int)FBDefs::SIDE::RIGHT]);
            
            auto pitch = FBMATCH->getPitch();
            auto team = FBMATCH->getControlSideTeam();
            
            auto hilitePlayer = team->getHilightPlayer();
            auto& vec = hilitePlayer->getMovingVector();
            if (vec.x != 0 || vec.y != 0)
            {
                Point base = {1, 0};
                float angle = CC_RADIANS_TO_DEGREES(vec.getAngle(base));
                m_arrow->setRotation(angle);
                
                auto pos = hilitePlayer->getPosition();
                
                m_arrow->setPosition(pitch->transToScreen(pos));
                m_arrow->setVisible(true);
            }
            else
            {
//                m_arrow->setVisible(false);
            }
            
            m_ball->setVisible(true);
            m_ball->setPosition(pitch->transToScreen(FBMATCH->getBallPosition()));
            
            break;
        }
        default:
            break;
    }
}



void CMatchLayer::updateTeam(CFBTeam* team, vector<Sprite*>& sprites)
{
    auto pitch = FBMATCH->getPitch();
    
    auto hilightPlayer = team->getHilightPlayer();
    
    for (int i = 0; i < team->getPlayerNumber(); ++i)
    {
        auto player = team->getPlayer(i);
        auto spr = sprites[i];
        spr->setVisible(true);
        spr->setPosition(pitch->transToScreen(player->getPosition()));
        if (hilightPlayer == player)
        {
            spr->setOpacity(50);
        }
        else
        {
            spr->setOpacity(255);
        }
    }
}



void CMatchLayer::togglePitchLieDown(bool lieDown)
{
    if (m_isPitchViewLieDown != lieDown)
    {
        if (m_isPitchViewLieDown)
        {
            m_isPitchViewLieDown = false;
            NodeGrid* ng = (NodeGrid*)m_pitch->getParent();
            ng->stopActionByTag(TAG_ACTION_FLIP3D);
            auto fx = CFlip3DYEx::create(.5f, 90, 180);
            fx->setTag(TAG_ACTION_FLIP3D);
            ng->runAction(fx);
        }
        else
        {
            m_isPitchViewLieDown = true;
            NodeGrid* ng = (NodeGrid*)m_pitch->getParent();
            ng->stopActionByTag(TAG_ACTION_FLIP3D);
            auto fx = CFlip3DYEx::create(.5f, 0, 90);
            fx->setTag(TAG_ACTION_FLIP3D);
            ng->runAction(fx);
        }
    }
}


void CMatchLayer::onPassBall(Ref* pSender)
{
    auto team = FBMATCH->getControlSideTeam();
    if (team->isAttacking())
    {
        FBMATCH->pauseGame(true);
        togglePitchLieDown(false);
        
#ifdef SHOW_GRID
        FBMATCH->getPitch()->calcBestShootPosition(FBDefs::SIDE::RIGHT);
        FBMATCH->getPitch()->calcBestShootPosition(FBDefs::SIDE::LEFT);
        refreshGrids();
#endif
    }
    else
    {
        team->switchHilightPlayer();
    }
}



int CMatchLayer::getSelectedPlayerId(const Point& pt, FBDefs::SIDE side)
{
    auto mat = m_pitch->getWorldToNodeTransform();
    Point pos = PointApplyTransform(pt, mat);
    
    auto players = m_players[(int)side];
    for (auto spr : players)
    {
        auto bx = spr->getBoundingBox();
        if (bx.containsPoint(pos))
        {
            return spr->getTag();
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
    
    DrawNode* draw = (DrawNode*)m_pitch->getChildByTag(TAG_GRID_DRAW_NODE);
    draw->clear();
    for (const auto& g: grids)
    {
        Color4F color = Color4F::GRAY;
        int size = 1;
        if (g.m_score > 0)
        {
            size = 10.f * g.m_score / 200.f;
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
        
        draw->drawDot(g.m_position, size, color);
        pitch->setGridDrawNode(g.m_index, draw);
    }
    
}
#endif



void CMatchLayer::setMenuItem(FBDefs::MENU_ITEMS mi)
{
}



void CMatchLayer::onPass(Ref* pSender)
{
    m_operator = OP::PASS_BALL;
    m_pitch->setVisible(true);
    m_menuLayer->setVisible(false);
    this->togglePitchLieDown(false);
}



void CMatchLayer::onDribble(Ref* pSender)
{
    FBMATCH->setMenuItem(FBDefs::MENU_ITEMS::Dribble);
}



void CMatchLayer::onShoot(Ref* pSender)
{
    m_operator = OP::SHOOT_BALL;
    auto team = FBMATCH->getTeam(FBDefs::SIDE::LEFT);
    auto from = team->getHilightPlayer();
    FBMATCH->tryShootBall(from, false);
    
    FBMATCH->setMenuItem(FBDefs::MENU_ITEMS::Shoot);
}



void CMatchLayer::onOneTwo(Ref* pSender)
{
    FBMATCH->setMenuItem(FBDefs::MENU_ITEMS::OneTwo);
}



void CMatchLayer::onTackle(Ref* pSender)
{
    FBMATCH->setMenuItem(FBDefs::MENU_ITEMS::Tackle);
}



void CMatchLayer::onIntercept(Ref* pSender)
{
    FBMATCH->setMenuItem(FBDefs::MENU_ITEMS::Intercept);
}



void CMatchLayer::onBlock(Ref* pSender)
{
    FBMATCH->setMenuItem(FBDefs::MENU_ITEMS::Block);
}



void CMatchLayer::onHit(Ref* pSender)
{
    FBMATCH->setMenuItem(FBDefs::MENU_ITEMS::Hit);
}


void CMatchLayer::onAnimationEnd()
{
    removeChild(m_animationRoot);
    m_animationRoot = nullptr;
    
    FBMATCH->onAnimationEnd();
}


#pragma mark - IFBMatchUI

void CMatchLayer::onMenu(FBDefs::MENU_TYPE type, const vector<int>& attackPlayerNumbers, const vector<int>& defendPlayerNumbers, int side)
{
    FBMATCH->pauseGame(true);
    m_pitch->setVisible(false);
    
    if (nullptr != m_menuLayer)
    {
        m_menuLayer->removeFromParentAndCleanup(true);
    }
    
    char name[256];
    sprintf(name, "fb_menu_%d.ccbi", (int)type);
    m_menuLayer = dynamic_cast<CMatchMenuLayer*>(CCBReadHelper::read(name));
    m_menuLayer->setMatchLayer(this);
    m_menuLayer->setPlayers(attackPlayerNumbers, defendPlayerNumbers, side);
    this->addChild(m_menuLayer, (int)Z_ORDER::MENU);
}



void CMatchLayer::onPlayAnimation(const string& name, float delay)
{
    auto node = CCBReadHelper::read(name.c_str());
    node->retain();
    
    runAction(Sequence::create(DelayTime::create(delay), CallFuncN::create([&, node](Node*){
        if (m_animationRoot == nullptr)
        {
            m_animationRoot = (CFBAnimationLayer*)node;
            this->addChild(m_animationRoot, (int)Z_ORDER::ANIMATION);
            m_animationRoot->setAnimationEndCallback(bind(&CMatchLayer::onAnimationEnd, this));
        }
        else
        {
            m_animationRoot->addChild(node);
        }
        
        node->release();
    }), NULL));

}



void CMatchLayer::onInstrunctionEnd(void)
{
    m_operator = OP::NONE;
    togglePitchLieDown(true);
}



void CMatchLayer::onPauseGame(bool pause)
{
    
}



void CMatchLayer::onGameEnd(void)
{
    SCENE_MANAGER->go(ST_LOBBY);
}



void CMatchLayer::showAttackMenu(bool show)
{
    if (nullptr != m_menuLayer)
    {
        m_menuLayer->removeFromParentAndCleanup(true);
        m_menuLayer = nullptr;
    }
    
    if (show)
    {
        m_menuLayer = dynamic_cast<CMatchMenuLayer*>(CCBReadHelper::read("fb_menu_0.ccbi"));
        CC_ASSERT(nullptr != m_menuLayer);
        
        m_menuLayer->setMatchLayer(this);
        this->addChild(m_menuLayer, (int)Z_ORDER::MENU);
    }
}



void CMatchLayer::waitInstruction(void)
{
    log("waitInstruction --------------------------------");
    // TODO: close menu and show wait prompt.
    if (m_menuLayer)
    {
        m_menuLayer->removeFromParentAndCleanup(true);
        m_menuLayer = nullptr;
    }
    
    m_pitch->setVisible(true);
    togglePitchLieDown(true);
}



