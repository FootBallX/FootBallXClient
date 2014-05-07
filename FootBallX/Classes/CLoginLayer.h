//
//  CLoginLayer.h
//  GameX
//
//  Created by 马 俊 on 13-11-20.
//
//

#ifndef __GameX__CLoginLayer__
#define __GameX__CLoginLayer__

#include "CBaseLayer.h"

class CLoginLayer
: public CBaseLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
{
public:
    virtual bool init();

protected:
    EditBox* m_userName = nullptr;
    EditBox* m_password = nullptr;

    void connectToConnector(const char* ip, int port);
    void getPlayerInfo(void);
    
//========AUTO CREATE CODE START========

public:
    CREATE_FUNC(CLoginLayer);
    
    CLoginLayer();
    virtual ~CLoginLayer();

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual Control::Handler onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode) override;
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) override;
    
protected:
    Layer* m_editUserNameLayer = nullptr;
    Layer* m_editPasswordLayer = nullptr;
    
    void onLogin(Ref* sender, Control::EventType event);
    void onCancel(Ref* sender, Control::EventType event);
    void onTest(Ref* sender, Control::EventType event);

};

class CLoginLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CLoginLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CLoginLayer);
};



//========AUTO CREATE CODE END========
#endif /* defined(__GameX__CLoginLayer__) */
