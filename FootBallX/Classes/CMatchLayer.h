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
#include "FBDefs.h"
#include "IFBMatchUI.h"

class CFBAnimationLayer;
class CFBTeam;

class CMatchLayer
: public CBaseLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
, public IFBMatchUI
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
    void updateTeam(CFBTeam* team, Sprite** sprites);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
    void onPassBall(Ref* pSender);
    
    void onPass(Ref* pSender);
    void onDribble(Ref* pSender);
    void onShoot(Ref* pSender);
    void onOneTwo(Ref* pSender);
    void onTackle(Ref* pSender);
    void onIntercept(Ref* pSender);
    void onPlug(Ref* pSender);

#pragma mark - IFBMatchUI
    virtual void onMenu(FBDefs::MENU_TYPE, bool, const vector<int>&);
    virtual void onPlayAnimation(const string&, float);
    virtual void onInstrunctionEnd(void);
    virtual void onPauseGame(bool);
    virtual void onGameEnd(void);
    
    void onMenuCallback(FBDefs::MENU_TYPE type, bool isAir, const vector<int>& involePlayers);
    void onPauseGameCallback(bool p);
    
    void playAnimation(const string& name, float delay);
    void onAnimationEnd();

    void togglePitchLieDown(bool lieDown);
    
    int getSelectedPlayerId(const Point& pt, bool isBlack);
#ifdef SHOW_GRID
    void refreshGrids();
#endif
    
    void onInstructionEnd();
    
//    void onSync(Node* node, void* resp);
    void onEndMatch(Node* node, void* resp);
//    void syncBall();
//    void syncTeam();
//    void syncHilightPlayer(int playerId);
    
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
    
    Point m_screenCenter;
    
    CFBAnimationLayer* m_animationRoot = nullptr;
    
    enum class OP
    {
        PASS_BALL,
        SHOOT_BALL,
        NONE,
    };
    
    OP m_operator = OP::NONE;
};


class CMatchLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CMatchLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CMatchLayer);
};


#endif /* defined(__FootBallX__CMatchLayer__) */
