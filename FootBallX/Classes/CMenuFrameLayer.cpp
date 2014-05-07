

#include "CMenuFrameLayer.h"
#include "CCardLayer.h"

static class CMenuFrameLayerRegister
{
public:
    CMenuFrameLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "CMenuFrameLayer", CMenuFrameLayerLoader::loader());
    }
} __reg;

CMenuFrameLayer::CMenuFrameLayer()
{
}

CMenuFrameLayer::~CMenuFrameLayer()
{
}

void CMenuFrameLayer::onEnter()
{
    CBaseLayer::onEnter();
}

void CMenuFrameLayer::onExit()
{
    CBaseLayer::onExit();
}

SEL_MenuHandler CMenuFrameLayer::onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_MENU_ITEM_BEGIN
    // AUTO_GEN_MENU_ITEM_END
    return nullptr;
}



Control::Handler CMenuFrameLayer::onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_CONTROL_BEGIN
    // AUTO_GEN_CONTROL_END
    return nullptr;
}



bool CMenuFrameLayer::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode)
{
    // AUTO_GEN_VAR_BEGIN
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "playerImg3", Sprite*, this->m_playerImg3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "playerImg1", Sprite*, this->m_playerImg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "playerImg0", Sprite*, this->m_playerImg0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "playerImg2", Sprite*, this->m_playerImg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "playerImg4", Sprite*, this->m_playerImg4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "defCard4", CCardLayer*, this->m_defCard4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "defCard3", CCardLayer*, this->m_defCard3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "defCard2", CCardLayer*, this->m_defCard2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "defCard1", CCardLayer*, this->m_defCard1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "defCard0", CCardLayer*, this->m_defCard0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "atkCard", CCardLayer*, this->m_atkCard);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "playerNick0", Label*, this->m_playerNick0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "playerNick1", Label*, this->m_playerNick1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "playerNick2", Label*, this->m_playerNick2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "playerNick3", Label*, this->m_playerNick3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "playerNick4", Label*, this->m_playerNick4);
    // AUTO_GEN_VAR_END
	return false;
}



void CMenuFrameLayer::onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader)
{
    
}


void CMenuFrameLayer::setPlayers(const vector<int>& ap, const vector<int>& dp)
{
    m_atkCard->setCardName(ap[0]);
    auto size = dp.size();
    
    m_defCard0->setCardName(dp[0]);
    if (size > 1)
    {
        m_defCard1->setCardName(dp[1]);
    }
    if (size > 2)
    {
        m_defCard2->setCardName(dp[2]);
    }
    if (size > 3)
    {
        m_defCard3->setCardName(dp[3]);
    }
    if (size > 4)
    {
        m_defCard4->setCardName(dp[4]);
    }
}







