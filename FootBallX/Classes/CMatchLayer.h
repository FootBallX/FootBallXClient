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
#include "CMatchMenuLayer.h"

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
    friend class CMatchMenuLayer;

    virtual bool init();
    
    virtual void update(float dt);
    
protected:
    void updateTeam(CFBTeam* team, vector<Sprite*>& sprites);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
    void onPassBall(Ref* pSender);
    
    void setMenuItem(FBDefs::MENU_ITEMS mi);
    
    virtual void onPass(Ref* pSender);          // 传球
    virtual void onDribble(Ref* pSender);       // 盘带
    virtual void onShoot(Ref* pSender);         // 射门
    virtual void onOneTwo(Ref* pSender);        // 二过一
    virtual void onTackle(Ref* pSender);        // 铲球
    virtual void onBlock(Ref* pSender);         // 封堵
    virtual void onIntercept(Ref* pSender);     // 拦截
    virtual void onHit(Ref* pSender);           // 击球
    virtual void onCatch(Ref* pSender);         // 接球
    
#pragma mark - IFBMatchUI
    virtual void onMenu(FBDefs::MENU_TYPE, const vector<int>&, const vector<int>&, int) override;
    virtual void onPlayAnimation(const string&, float) override;
    virtual void onInstrunctionEnd(void) override;
    virtual void onPauseGame(bool) override;
    virtual void onGameEnd(void) override;
    virtual void waitInstruction(void) override;
    virtual void showAttackMenu(bool) override;
    
    void onMenuCallback(FBDefs::MENU_TYPE type, bool isAir, const vector<int>& involePlayers);
    void onPauseGameCallback(bool p);
    
    void playAnimation(const string& name, float delay);
    void onAnimationEnd();

    void togglePitchLieDown(bool lieDown);
    
    int getSelectedPlayerId(const Point& pt, FBDefs::SIDE side);
#ifdef SHOW_GRID
    void refreshGrids();
#endif
    
    void onInstructionEnd();

    void onEndMatch(Node* node, void* resp);
    
    vector<Sprite*> m_players[(int)FBDefs::SIDE::NONE];
    
    bool m_isTouchDown = false;
    bool m_isPitchViewLieDown = true;
    
    Point m_screenCenter;
    
    CFBAnimationLayer* m_animationRoot = nullptr;
    
    enum class OP
    {
        PASS_BALL,
        SHOOT_BALL,
        ONE_TWO,
        NONE,
    };
    
    OP m_operator = OP::NONE;
    
    CMatchMenuLayer* m_menuLayer = nullptr;
    
    
    //========AUTO CREATE CODE START========

public:
    CREATE_FUNC(CMatchLayer);
    
    CMatchLayer();
    virtual ~CMatchLayer();

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual Control::Handler onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode) override;
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) override;
    
protected:
    Sprite* m_pitch = nullptr;
    Sprite* m_b0 = nullptr;
    Sprite* m_b1 = nullptr;
    Sprite* m_b2 = nullptr;
    Sprite* m_b3 = nullptr;
    Sprite* m_b4 = nullptr;
    Sprite* m_b5 = nullptr;
    Sprite* m_b6 = nullptr;
    Sprite* m_b7 = nullptr;
    Sprite* m_b8 = nullptr;
    Sprite* m_b9 = nullptr;
    Sprite* m_b10 = nullptr;
    Sprite* m_r0 = nullptr;
    Sprite* m_r1 = nullptr;
    Sprite* m_r2 = nullptr;
    Sprite* m_r3 = nullptr;
    Sprite* m_r4 = nullptr;
    Sprite* m_r5 = nullptr;
    Sprite* m_r6 = nullptr;
    Sprite* m_r7 = nullptr;
    Sprite* m_r8 = nullptr;
    Sprite* m_r9 = nullptr;
    Sprite* m_r10 = nullptr;
    Sprite* m_ball = nullptr;
    Sprite* m_arrow = nullptr;
    

};

class CMatchLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CMatchLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CMatchLayer);
};



//========AUTO CREATE CODE END========

#endif /* defined(__FootBallX__CMatchLayer__) */
