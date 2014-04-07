//
//  CLobbyLayer.h
//  GameX
//
//  Created by 马 俊 on 13-11-20.
//
//

#ifndef __GameX__CLobbyLayer__
#define __GameX__CLobbyLayer__

#include "CBaseLayer.h"

class CLobbyLayer
: public CBaseLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
{
public:
    CREATE_FUNC(CLobbyLayer);
    
    CLobbyLayer();
    virtual ~CLobbyLayer();
    
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
    
    ControlButton* m_cancelButton = nullptr;
    ControlButton* m_singUpButton = nullptr;
    LabelTTF* m_logText = nullptr;
    
    virtual void onSignUp(Ref* sender, Control::EventType event);
    virtual void onCancel(Ref* sender, Control::EventType event);
    
    virtual void connectMatchServer(json_int_t token);
};


class CLobbyLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CLobbyLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CLobbyLayer);
};


#endif /* defined(__GameX__CLobbyLayer__) */
