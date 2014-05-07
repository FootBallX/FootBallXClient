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

class CMatchMenuLayer
: public CBaseLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
{
public:
    CC_SYNTHESIZE(CMatchLayer*, m_matchLayer, MatchLayer);
    
    CREATE_FUNC(CMatchMenuLayer);
    
    CMatchMenuLayer();
    virtual ~CMatchMenuLayer();
    
    virtual bool init();
    
    virtual void update(float dt);
    
    //CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual Control::Handler onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName) override;
    
    //CCBMemberVariableAssigner
    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode) override;
    
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) override;
    
    virtual void setPlayers(const vector<int>& ap, const vector<int>& dp);
    
protected:
    virtual void onPass(Ref* sender, Control::EventType event);          // 传球
    virtual void onDribble(Ref* sender, Control::EventType event);       // 盘带
    virtual void onShoot(Ref* sender, Control::EventType event);         // 射门
    virtual void onOneTwo(Ref* sender, Control::EventType event);        // 二过一
    virtual void onTackle(Ref* sender, Control::EventType event);        // 铲球
    virtual void onBlock(Ref* sender, Control::EventType event);         // 封堵
    virtual void onIntercept(Ref* sender, Control::EventType event);     // 拦截
    virtual void onHit(Ref* sender, Control::EventType event);           // 击球
    
    ControlButton* m_btnPass = nullptr;
    ControlButton* m_btnDribble = nullptr;
    ControlButton* m_btnShoot = nullptr;
    ControlButton* m_btnOneTwo = nullptr;
    
    Sprite* m_PlayerImg3 = nullptr;
    Sprite* m_PlayerImg1 = nullptr;
    Sprite* m_PlayerImg0 = nullptr;
    Sprite* m_PlayerImg2 = nullptr;
    Sprite* m_PlayerImg4 = nullptr;
    CCardLayer* m_DefCard4 = nullptr;
    CCardLayer* m_DefCard3 = nullptr;
    CCardLayer* m_DefCard2 = nullptr;
    CCardLayer* m_DefCard1 = nullptr;
    CCardLayer* m_DefCard0 = nullptr;
    CCardLayer* m_AtkCard = nullptr;
    Label* m_PlayerNick0 = nullptr;
    Label* m_PlayerNick1 = nullptr;
    Label* m_PlayerNick2 = nullptr;
    Label* m_PlayerNick3 = nullptr;
    Label* m_PlayerNick4 = nullptr;
    
    vector<int> m_attackPlayerNumbers;
    vector<int> m_defendPlayerNumbers;
};


class CMatchMenuLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CMatchMenuLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CMatchMenuLayer);
};


#endif /* defined(__FootBallX__CMatchMenuLayer__) */
