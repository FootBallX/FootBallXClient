//
//  CMatchMenuLayer.h
//  FootBallX
//
//  Created by 马 俊 on 14-4-19.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CMatchMenuLayer__
#define __FootBallX__CMatchMenuLayer__

#include "CBaseLayer.h"
#include "FBDefs.h"

class CFBAnimationLayer;
class CFBTeam;
class CMatchLayer;
class CCardLayer;
class CMenuFrameLayer;

class CMatchMenuLayer
: public CBaseLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
{
public:
    CC_SYNTHESIZE(CMatchLayer*, m_matchLayer, MatchLayer);
    
    virtual bool init();
    
    virtual void update(float dt);
    
    // side : 0 -> ap, 1 -> dp
    virtual void setPlayers(const vector<int>& ap, const vector<int>& dp, int side);
    
protected:
    void nextHiliteCard();
    
    int m_curPlayerIndex = 0;
    
//========AUTO CREATE CODE START========

public:
    CREATE_FUNC(CMatchMenuLayer);
    
    CMatchMenuLayer();
    virtual ~CMatchMenuLayer();

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual Control::Handler onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode) override;
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) override;
    
protected:
    CMenuFrameLayer* m_menuFrameLayer = nullptr;
    ControlButton* m_varButtonDribble = nullptr;
    ControlButton* m_varButtonPass = nullptr;
    ControlButton* m_varButtonShoot = nullptr;
    ControlButton* m_varButtonOneTwo = nullptr;
    ControlButton* m_varButtonClear = nullptr;
    ControlButton* m_varButtonTackle = nullptr;
    ControlButton* m_varButtonIntercept = nullptr;
    ControlButton* m_varButtonBlock = nullptr;
    ControlButton* m_varButtonCatch = nullptr;
    ControlButton* m_varButtonHit = nullptr;
    ControlButton* m_varButtonBlockDribble = nullptr;
    ControlButton* m_varButtonBlockShoot = nullptr;
    ControlButton* m_varButtonAttack = nullptr;
    ControlButton* m_varButtonWait = nullptr;
    
    void onDribble(Ref* sender, Control::EventType event);
    void onPass(Ref* sender, Control::EventType event);
    void onShoot(Ref* sender, Control::EventType event);
    void onOneTwo(Ref* sender, Control::EventType event);
    void onClear(Ref* sender, Control::EventType event);
    void onTackle(Ref* sender, Control::EventType event);
    void onIntercept(Ref* sender, Control::EventType event);
    void onBlock(Ref* sender, Control::EventType event);
    void onCatch(Ref* sender, Control::EventType event);
    void onHit(Ref* sender, Control::EventType event);
    void onBlockDribble(Ref* sender, Control::EventType event);
    void onBlockShoot(Ref* sender, Control::EventType event);
    void onAttack(Ref* sender, Control::EventType event);
    void onWait(Ref* sender, Control::EventType event);
};

class CMatchMenuLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CMatchMenuLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CMatchMenuLayer);
};



//========AUTO CREATE CODE END========

#endif /* defined(__FootBallX__CMatchMenuLayer__) */
