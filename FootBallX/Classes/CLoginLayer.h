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
    CREATE_FUNC(CLoginLayer);
    
    CLoginLayer();
    virtual ~CLoginLayer();
    
    virtual bool init();
    
    //CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName) ;
    virtual Control::Handler onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName) ;
    
    //CCBMemberVariableAssigner
    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode);
    
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader);
protected:
    Layer* m_editUserNameLayer = nullptr;
    Layer* m_editPasswordLayer = nullptr;
    
    EditBox* m_userName = nullptr;
    EditBox* m_password = nullptr;

    
    virtual void onLogin(Ref* sender, Control::EventType event);
    virtual void onCancel(Ref* sender, Control::EventType event);

    void connectToConnector(const char* ip, int port);
    void getPlayerInfo(void);
};


class CLoginLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CLoginLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CLoginLayer);
};


#endif /* defined(__GameX__CLoginLayer__) */
