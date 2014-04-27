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
};

static class CMatchLayerRegister
{
public:
    CMatchLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "CMatchLayer", CMatchLayerLoader::loader());
    }
} __reg;


#define TAG_ACTION_FLIP3D           1001
#define TAG_COUNT_DOWN              1002

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
        
        cocosbuilder::CCBReader* pReader = new cocosbuilder::CCBReader(cocosbuilder::NodeLoaderLibrary::getInstance());
        m_menuLayer = dynamic_cast<CMatchMenuLayer*>(pReader->readNodeGraphFromFile("fb_menu_0.ccbi"));
        CC_BREAK_IF(nullptr == m_menuLayer);

        m_menuLayer->setMatchLayer(this);
        this->addChild(m_menuLayer, (int)Z_ORDER::MENU);
        this->scheduleUpdate();

        return true;
    } while (false);
    
    return false;
}



bool CMatchLayer::onTouchBegan(Touch* touch, Event* event)
{    
    auto team = FBMATCH->getControlSideTeam();
    if (team)
    {
        auto loc = touch->getLocation();
        switch (m_operator)
        {
            case OP::PASS_BALL:
            {
                int idx = getSelectedPlayerId(loc, true);
                if (idx != -1)
                {
                    auto team = FBMATCH->getTeam(FBDefs::SIDE::LEFT);
                    auto from = team->getHilightPlayer();
                    auto to = team->getFormation()->getPlayer(idx);
                    FBMATCH->tryPassBall(from, to);
                    m_operator = OP::NONE;
                }
                break;
            }
            case OP::NONE:
                if (!FBMATCH->isPausing())
                {
                    FBMATCH->setBallControllerMove((loc - m_screenCenter).normalize());
                    FBMATCH->syncTeam();
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
    FBMATCH->syncTeam();
    m_isTouchDown = false;
}



void CMatchLayer::onTouchCancelled(Touch* touch, Event* event)
{
    FBMATCH->setBallControllerMove(Point());
    FBMATCH->syncTeam();
    m_isTouchDown = false;
}



SEL_MenuHandler CMatchLayer::onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName)
{
    return nullptr;
}



//函数定义类型为：void pressTitle(Ref*pSender);
Control::Handler CMatchLayer::onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName)
{
    return nullptr;
}



bool CMatchLayer::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node* pNode)
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
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pitch", Sprite*, m_pitchSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ball", Sprite*, m_ball);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "arrow", Sprite*, m_arrow);

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
            m_pitchSprite->getParent()->addChild(ng, (int)Z_ORDER::PITCH);
            m_pitchSprite->removeFromParent();
            ng->addChild(m_pitchSprite);
            m_isPitchViewLieDown = true;
        }
        
        Size pitchSz = m_pitchSprite->getContentSize();
        
        const char* FONT_NAME = "Helvetica";
        const int FONT_SIZE = 32 * cocosbuilder::CCBReader::getResolutionScale();
        
        auto countDownSpr = Label::createWithSystemFont("", FONT_NAME, FONT_SIZE);
        addChild(countDownSpr, (int)Z_ORDER::COUNT_DOWN, TAG_COUNT_DOWN);
        countDownSpr->setVisible(false);
        auto sz = Director::getInstance()->getWinSize();
        countDownSpr->setPosition(sz.width * 0.5f, sz.height * 0.5f);
        
        BREAK_IF_FAILED(FBMATCH->init(pitchSz.width, pitchSz.height, this, new CFBMatchProxyNet()));

        vector<string> redPlayercards =
        {
            "10001",
            "40001",
            "40002",
            "40003",
            "40004",
            "30001",
            "30002",
            "30003",
            "30004",
            "20001",
            "20002",
        };
        vector<string> blackPlayercards =
        {
            "10002",
            "40005",
            "40006",
            "40007",
            "30005",
            "30006",
            "30007",
            "30008",
            "30009",
            "20003",
            "20004",
        };
        
        CFBTeam* red = new CFBTeam;
        BREAK_IF_FAILED(red->init(redPlayercards));
        CFBTeam* black = new CFBTeam;
        BREAK_IF_FAILED(black->init(blackPlayercards));
        FBMATCH->setTeam(FBDefs::SIDE::LEFT, red);
        FBMATCH->setTeam(FBDefs::SIDE::RIGHT, black);
        
        BREAK_IF_FAILED(black->changeFormation(FBDefs::FORMATION::F_3_2_3_2))
        
        BREAK_IF_FAILED(FBMATCH->startMatch());
#ifdef SHOW_GRID
        auto draw = DrawNode::create();
        m_pitchSprite->addChild(draw, (int)Z_ORDER::GRID_DEBUG, TAG_GRID_DRAW_NODE);
        
        refreshGrids();
#endif // SHOW_GRID
        
    } while (false);
}



void CMatchLayer::update(float dt)
{
    CBaseLayer::update(dt);
    
    FBMATCH->update(dt);
    
    switch (FBMATCH->getMatchStep())
    {
        case FBDefs::MATCH_STEP::WAIT_START:
            break;
        case FBDefs::MATCH_STEP::COUNT_DOWN:
        {
            char s[25];
            sprintf(s, "%d", FBMATCH->getCountDownTime());
            auto spr = (Label*)getChildByTag(TAG_COUNT_DOWN);
            spr->setVisible(true);
            spr->setString(s);
            break;
        }
        case FBDefs::MATCH_STEP::MATCHING:
        {
            auto spr = getChildByTag(TAG_COUNT_DOWN);
            spr->setVisible(false);
            
            updateTeam(FBMATCH->getTeam(FBDefs::SIDE::LEFT), m_blackPlayers);
            updateTeam(FBMATCH->getTeam(FBDefs::SIDE::RIGHT), m_redPlayers);
            
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



void CMatchLayer::updateTeam(CFBTeam* team, Sprite** sprites)
{
    auto pitch = FBMATCH->getPitch();
    
    auto fmt = team->getFormation();
    
    auto hilightPlayer = team->getHilightPlayer();
    
    for (int i = 0; i < fmt->getPlayerNumber(); ++i)
    {
        auto player = fmt->getPlayer(i);
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
            NodeGrid* ng = (NodeGrid*)m_pitchSprite->getParent();
            ng->stopActionByTag(TAG_ACTION_FLIP3D);
            auto fx = CFlip3DYEx::create(.5f, 90, 180);
            fx->setTag(TAG_ACTION_FLIP3D);
            ng->runAction(fx);
        }
        else
        {
            m_isPitchViewLieDown = true;
            NodeGrid* ng = (NodeGrid*)m_pitchSprite->getParent();
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



void CMatchLayer::onPass(Ref* pSender)
{
    m_operator = OP::PASS_BALL;
}



void CMatchLayer::onDribble(Ref* pSender)
{
}



void CMatchLayer::onShoot(Ref* pSender)
{
    m_operator = OP::SHOOT_BALL;
    auto team = FBMATCH->getTeam(FBDefs::SIDE::LEFT);
    auto from = team->getHilightPlayer();
    FBMATCH->tryShootBall(from, false);
    
}



void CMatchLayer::onOneTwo(Ref* pSender)
{
}



void CMatchLayer::onTackle(Ref* pSender)
{
}



void CMatchLayer::onIntercept(Ref* pSender)
{
}



void CMatchLayer::onBlock(Ref* pSender)
{
}



void CMatchLayer::onHit(Ref* pSender)
{
    
}


void CMatchLayer::onAnimationEnd()
{
    removeChild(m_animationRoot);
    m_animationRoot = nullptr;
    
    FBMATCH->onAnimationEnd();
}


#pragma mark - IFBMatchUI

void CMatchLayer::onMenu(FBDefs::MENU_TYPE, bool, const vector<int>&)
{
    FBMATCH->pauseGame(true);
    togglePitchLieDown(false);
}



void CMatchLayer::onPlayAnimation(const string& name, float delay)
{
    cocosbuilder::CCBReader* pReader = new cocosbuilder::CCBReader(cocosbuilder::NodeLoaderLibrary::getInstance());
    auto node = pReader->readNodeGraphFromFile(name.c_str());
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



void CMatchLayer::onPauseGame(bool)
{
}



void CMatchLayer::onGameEnd(void)
{
    SCENE_MANAGER->go(ST_LOBBY);
}

