
#ifndef FootballX_CMenuFrameLayer_
#define FootballX_CMenuFrameLayer_

#include "Common.h"
#include "CBaseLayer.h"

class CCardLayer;

class CMenuFrameLayer
: public CBaseLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
{
public:
    virtual void setPlayers(const vector<int>& ap, const vector<int>& dp);

//========AUTO CREATE CODE START========

public:
    CREATE_FUNC(CMenuFrameLayer);
    
    CMenuFrameLayer();
    virtual ~CMenuFrameLayer();

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual Control::Handler onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode) override;
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) override;
    
protected:
    Sprite* m_playerImg3 = nullptr;
    Sprite* m_playerImg1 = nullptr;
    Sprite* m_playerImg0 = nullptr;
    Sprite* m_playerImg2 = nullptr;
    Sprite* m_playerImg4 = nullptr;
    CCardLayer* m_defCard4 = nullptr;
    CCardLayer* m_defCard3 = nullptr;
    CCardLayer* m_defCard2 = nullptr;
    CCardLayer* m_defCard1 = nullptr;
    CCardLayer* m_defCard0 = nullptr;
    CCardLayer* m_atkCard = nullptr;
    Label* m_playerNick0 = nullptr;
    Label* m_playerNick1 = nullptr;
    Label* m_playerNick2 = nullptr;
    Label* m_playerNick3 = nullptr;
    Label* m_playerNick4 = nullptr;
    

};

class CMenuFrameLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CMenuFrameLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CMenuFrameLayer);
};



//========AUTO CREATE CODE END========

#endif  // FootballX_CMenuFrameLayer_
