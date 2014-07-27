//
//  CCCBTestLayer.cpp
//  FBXMatchServerTester
//
//  Created by Ray M on 14-7-25.
//
//

#include "CCCBTestLayer.h"
#include "CFileSearch.h"
#include "CCBReadHelper.h"


#define PATH_KEY        "PathKey"


CCCBTestLayer::CCCBTestLayer() :
m_ccbi(nullptr)
{
    
}



CCCBTestLayer::~CCCBTestLayer()
{
    
}

bool CCCBTestLayer::init()
{
    do
    {
        BREAK_IF_FAILED(Layer::init());
        createCCBList();
        createPathEdit();
        
        auto winSz = Director::getInstance()->getVisibleSize();
        ControlButton* ccbBtn = ControlButton::create("Back", "", 20);
        ccbBtn->setAnchorPoint(Point(1, 0));
        ccbBtn->setPosition(Point(winSz.width, 0));
        ccbBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(CCCBTestLayer::onBack), Control::EventType::TOUCH_UP_INSIDE);
        addChild(ccbBtn);

        return true;
    } while (false);
    
    return false;
}


void CCCBTestLayer::onEnter()
{
    Layer::onEnter();
    
}


void CCCBTestLayer::onExit()
{
    Layer::onExit();
}


void CCCBTestLayer::onBack(Ref*, Control::EventType)
{
    Director::getInstance()->popScene();
}



void CCCBTestLayer::createCCBList()
{
    auto winSz = Director::getInstance()->getVisibleSize();
    m_ccbListView = ScrollView::create(Size(winSz.width * 0.25f, winSz.height * 0.9f));
    m_ccbListView->setPosition(Point(0, 0));
    m_ccbListView->setDirection(ScrollView::Direction::BOTH);
    
    m_container = LayerColor::create(Color4B(0, 0, 100, 128));
    m_ccbListView->setContainer(m_container);
    
    addChild(m_ccbListView);
}


void CCCBTestLayer::createPathEdit()
{
    auto winSz = Director::getInstance()->getVisibleSize();
    
    m_editPath = EditBox::create(Size(winSz.width * 0.8, 30), Scale9Sprite::create("EditBg.png"));
    m_editPath->setAnchorPoint(Point(0, 1));
    m_editPath->setPosition(Point(winSz.width * 0.1, winSz.height));
    
    addChild(m_editPath);
    
    auto btn = ControlButton::create("Load", "", 30);
    btn->setAnchorPoint(Point(1, 1));
    btn->setPosition(winSz.width * 0.1, winSz.height);
    btn->addTargetWithActionForControlEvents(this, cccontrol_selector(CCCBTestLayer::onLoadCCBList), Control::EventType::TOUCH_UP_INSIDE);
    addChild(btn);
    
    
    string path = UserDefault::getInstance()->getStringForKey(PATH_KEY);
    if (path.length() > 0)
    {
        m_editPath->setText(path.c_str());
        onLoadCCBList(btn, Control::EventType::TOUCH_UP_INSIDE);
    }
}


void CCCBTestLayer::onLoadCCBList(Ref*, Control::EventType)
{
    string path = m_editPath->getText();
    if (path[path.length() - 1] != '/')
    {
        path += "/";
    }
    
    const float GAP = 20;
    
    m_ccbList.clear();
    
    if (CFileSearch::search(path.c_str(), m_ccbList))
    {
        if (m_ccbList.size() > 0)
        {
            m_container->removeAllChildrenWithCleanup(true);
            
            UserDefault::getInstance()->setStringForKey(PATH_KEY, path);
            
            std::vector<std::string> resDirOrders;
            resDirOrders.push_back(path);
            resDirOrders.push_back(string(path +"resources-iphone"));
            cocosbuilder::CCBReader::setResolutionScale(1.0f);
            FileUtils::getInstance()->setSearchResolutionsOrder(resDirOrders);
            
            float y = 0;

            Size containerSize(0, GAP * m_ccbList.size());

            for (int i = 0; i < m_ccbList.size(); ++i)
            {
                auto& s = m_ccbList[i];
                auto btn = ControlButton::create(s.c_str(), "", 18);
                btn->setTag(i);
                btn->setAnchorPoint(Point(0, 1));
                btn->setPosition(0, containerSize.height - y);
                btn->addTargetWithActionForControlEvents(this, cccontrol_selector(CCCBTestLayer::onCCBClick), Control::EventType::TOUCH_UP_INSIDE);
                
                btn->setTitleColorForState(Color3B(0xFF, 0xFF, 0xFF), Control::State::NORMAL);
                btn->setTitleColorForState(Color3B(0xFF, 0, 0), Control::State::HIGH_LIGHTED);
                btn->setTitleColorForState(Color3B(0xFF, 0, 0), Control::State::SELECTED);
                btn->setTitleColorForState(Color3B(0xFF, 0, 0), Control::State::DISABLED);
                
                float w = btn->getContentSize().width;
                if (w > containerSize.width)
                {
                    containerSize.width = w;
                }
                m_container->addChild(btn);
                
                y += GAP;
            }
            
            m_container->setContentSize(containerSize);
            m_ccbListView->setContentOffset(Point(0, m_ccbListView->minContainerOffset().y));
        }
    }
}


void CCCBTestLayer::onCCBClick(Ref* obj, Control::EventType)
{
    ControlButton* btn = dynamic_cast<ControlButton*>(obj);
    if (btn && m_ccbListView->isNodeVisible(btn))
    {
        int idx = btn->getTag();
        if (idx >= 0 && idx < m_ccbList.size())
        {
            for (int i = 0; i < m_ccbList.size(); ++i)
            {
                ControlButton* _btn = dynamic_cast<ControlButton*>(m_container->getChildByTag(i));
                if (_btn)
                {
                    _btn->setEnabled(true);
                }
            }
            
            btn->setEnabled(false);
            
            playCCB(m_ccbList[idx]);
        }
    }
}



void CCCBTestLayer::playCCB(const string& name)
{
    string path = UserDefault::getInstance()->getStringForKey(PATH_KEY);
    path += name;
    
    if (m_ccbi)
    {
        removeChild(m_ccbi);
    }
    m_ccbi = CCBReadHelper::read(path.c_str());
    addChild(m_ccbi);
}


