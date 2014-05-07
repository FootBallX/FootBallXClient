
#ifndef FootballX_CCardLayer_
#define FootballX_CCardLayer_

#include "Common.h"
#include "CBaseLayer.h"


class CCardLayer
: public CBaseLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
{
public:

    void setCardName(int cardId);
//========AUTO CREATE CODE START========

public:
    CREATE_FUNC(CCardLayer);
    
    CCardLayer();
    virtual ~CCardLayer();

    virtual void onEnter();
    virtual void onExit();

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual Control::Handler onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode) override;
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) override;
    
protected:
    

};

class CCardLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCardLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCardLayer);
};



//========AUTO CREATE CODE END========

#endif  // FootballX_CCardLayer_
