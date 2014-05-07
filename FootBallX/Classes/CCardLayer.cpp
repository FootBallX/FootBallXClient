

#include "CCardLayer.h"

static class CCardLayerRegister
{
public:
    CCardLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "CCardLayer", CCardLayerLoader::loader());
    }
} __reg;

CCardLayer::CCardLayer()
{
}

CCardLayer::~CCardLayer()
{
}

void CCardLayer::onEnter()
{
    CBaseLayer::onEnter();
}

void CCardLayer::onExit()
{
    CBaseLayer::onExit();
}

SEL_MenuHandler CCardLayer::onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_MENU_ITEM_BEGIN
    // AUTO_GEN_MENU_ITEM_END
    return nullptr;
}



Control::Handler CCardLayer::onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_CONTROL_BEGIN
    // AUTO_GEN_CONTROL_END
    return nullptr;
}



bool CCardLayer::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode)
{
    // AUTO_GEN_VAR_BEGIN
    // AUTO_GEN_VAR_END
	return false;
}



void CCardLayer::onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader)
{
    
}



void CCardLayer::setCardName(int cardId)
{
    char s[32];
    sprintf(s, "card:%d", cardId);
    auto p = Label::createWithSystemFont(s, "Helvetica", 24);
    addChild(p, 0, 123);
}

