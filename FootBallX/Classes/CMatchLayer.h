//
//  CMatchLayer.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-11.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CMatchLayer__
#define __FootBallX__CMatchLayer__
#include "CBaseLayer.h"
#include "CSpriteEx.h"

class CMatchLayer
: public CBaseLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
{
public:
    CREATE_FUNC(CMatchLayer);
    
    CMatchLayer();
    virtual ~CMatchLayer();
    
    virtual bool init();
    
    virtual void update(float dt);
    
    //CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Object * pTarget, const char* pSelectorName) ;
    virtual Control::Handler onResolveCCBCCControlSelector(Object * pTarget, const char* pSelectorName) ;
    
    //CCBMemberVariableAssigner
    virtual bool onAssignCCBMemberVariable(Object * pTarget, const char* pMemberVariableName, Node * pNode);
    
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
protected:
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
    void onPassBall(Object* pSender);
//    virtual void onFormation(Object *pSender);
//    virtual void onBattle(Object* pSender);
//    virtual void onHome(Object* pSender);
//    virtual void onTestSocket(Object* pSender);
    
//    void onMsg(Node* node, void* resp);
    
    void togglePitchLieDown();
    int getSelectedPlayerId(const Point& pt, bool isBlack);
#ifdef SHOW_GRID
    void refreshGrids();
#endif
    
    Sprite* m_blackPlayers[11] = {nullptr};
    Sprite* m_redPlayers[11] = {nullptr};
    CSpriteEx* m_pitchSprite = nullptr;
    
    Sprite* m_ball = nullptr;
    Sprite* m_arrow = nullptr;
    
    MenuItem* m_menuPassBall = nullptr;
    
    bool m_isTouchDown = false;
    bool m_isPitchViewLieDown = false;
    
    Point m_ballMovingVec;
    Point m_screenCenter;
    
    
    enum class OP
    {
        PASS_BALL,
        NONE,
    };
    
    OP m_operator = OP::NONE;
private:
};


class CMatchLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CMatchLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CMatchLayer);
};


#endif /* defined(__FootBallX__CMatchLayer__) */
