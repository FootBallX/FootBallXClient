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

class CFBAnimationLayer;

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
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName) ;
    virtual Control::Handler onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName) ;
    
    //CCBMemberVariableAssigner
    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode);
    
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
    
protected:
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
    void onPassBall(Ref* pSender);
//    virtual void onFormation(Ref*pSender);
//    virtual void onBattle(Ref* pSender);
//    virtual void onHome(Ref* pSender);
//    virtual void onTestSocket(Ref* pSender);
    
//    void onMsg(Node* node, void* resp);
    
    void onPass(Ref* pSender);
    void onDribble(Ref* pSender);
    void onShoot(Ref* pSender);
    void onOneTwo(Ref* pSender);
    void onTackle(Ref* pSender);
    void onIntercept(Ref* pSender);
    void onPlug(Ref* pSender);
    
    void onAtkMenuCallback(const vector<int>& defPlayers);
    void onDefMenuCallback(const vector<int>& defPlayers);
    void playAnimation(const string& name, float delay);
    void onAnimationEnd();

    void togglePitchLieDown(bool lieDown);
    
    int getSelectedPlayerId(const Point& pt, bool isBlack);
#ifdef SHOW_GRID
    void refreshGrids();
#endif
    
    void onInstructionEnd();
    
    Sprite* m_blackPlayers[11] = {nullptr};
    Sprite* m_redPlayers[11] = {nullptr};
    Sprite* m_pitchSprite = nullptr;
    
    Sprite* m_ball = nullptr;
    Sprite* m_arrow = nullptr;
    
    MenuItem* m_menuPassBall = nullptr;
    
    Node* m_atkMenu = nullptr;
    Node* m_defMenu = nullptr;
    
    bool m_isTouchDown = false;
    bool m_isPitchViewLieDown = true;
    
    Point m_ballMovingVec;
    Point m_screenCenter;
    
    CFBAnimationLayer* m_animationRoot = nullptr;
    
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
