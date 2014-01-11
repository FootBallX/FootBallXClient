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
#include "CCBReader/CCBReader.h"


class CMatchLayer
: public CBaseLayer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public NodeLoaderListener
{
public:
    CREATE_FUNC(CMatchLayer);
    
    CMatchLayer();
    virtual ~CMatchLayer();
    
    virtual bool init();
    
    //CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Object * pTarget, const char* pSelectorName) ;
    virtual Control::Handler onResolveCCBCCControlSelector(Object * pTarget, const char* pSelectorName) ;
    
    //CCBMemberVariableAssigner
    virtual bool onAssignCCBMemberVariable(Object * pTarget, const char* pMemberVariableName, Node * pNode);
    
    virtual void onNodeLoaded(Node * pNode, NodeLoader * pNodeLoader);
protected:
//    virtual void onFormation(Object *pSender);
//    virtual void onBattle(Object* pSender);
//    virtual void onHome(Object* pSender);
//    virtual void onTestSocket(Object* pSender);
    
//    void onMsg(Node* node, void* resp);
private:
};


class CMatchLayerLoader : public cocos2d::extension::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CMatchLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CMatchLayer);
};


#endif /* defined(__FootBallX__CMatchLayer__) */
