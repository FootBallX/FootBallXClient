//
//  CMatchTestLayer.cpp
//  FBXMatchServerTester
//
//  Created by Ray M on 14-7-25.
//
//

#include "CMatchTestLayer.h"
#include "CMatchTester.h"
#include "CTestCaseManager.h"


CMatchTestLayer::CMatchTestLayer() :
m_borderNode(nullptr)
, m_caseList(nullptr)
, m_caseListContainer(nullptr)
, m_logList(nullptr)
, m_logListContainer(nullptr)
{
    
}



CMatchTestLayer::~CMatchTestLayer()
{
    
}

bool CMatchTestLayer::init()
{
    do
    {
        BREAK_IF_FAILED(Layer::init());
        auto winSz = Director::getInstance()->getVisibleSize();
        ControlButton* ccbBtn = ControlButton::create("Back", "", 20);
        ccbBtn->setAnchorPoint(Point(1, 0));
        ccbBtn->setPosition(Point(winSz.width, 0));
        ccbBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(CMatchTestLayer::onBack), Control::EventType::TOUCH_UP_INSIDE);
        addChild(ccbBtn);
        
        auto startBtn = ControlButton::create("Start", "", 30);
        startBtn->setAnchorPoint(Point(0.5, 1));
        startBtn->setPosition(Point(winSz.width * 0.5f, winSz.height));
        startBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(CMatchTestLayer::onStartTest), Control::EventType::TOUCH_UP_INSIDE);
        addChild(startBtn);
        
        createCaseList();
        
        createBorder();
        createLogList();
        
        return true;
    } while (false);
    
    return false;
}


void CMatchTestLayer::onEnter()
{
    Layer::onEnter();
}


void CMatchTestLayer::onExit()
{
    Layer::onExit();
}


void CMatchTestLayer::onBack(Ref*, Control::EventType)
{
    addLog("adslkfjdafkljasdklfjasdlfkjdsafkljasdklfjdsafljdsflkdjaflkasjflksajflksfjkldsafjaslkf");
    return;
    Director::getInstance()->popScene();
}


void CMatchTestLayer::onStartTest(Ref*, Control::EventType)
{
    MT->startAll();
}


void CMatchTestLayer::createCaseList()
{
    auto winSz = Director::getInstance()->getVisibleSize();
    Size svSz(winSz.width * 0.4f, winSz.height * 0.9);
    m_caseList = ScrollView::create(svSz);
    m_caseList->setPosition(Point(0, 0));
    m_caseList->setDirection(ScrollView::Direction::BOTH);
    
    m_caseListContainer = LayerColor::create(Color4B(0, 0, 100, 128));
    m_caseList->setContainer(m_caseListContainer);
    
    addChild(m_caseList);
    
    m_caseListContainer->removeAllChildrenWithCleanup(true);
    
    const float GAP = 20;
    float y = 0;
    
    TC->init();

    auto count = TC->getCaseCount();
    Size containerSize(svSz.width, GAP * TC->getCaseCount());
    
    if (containerSize.height < svSz.height)
    {
        containerSize.height = svSz.height;
    }

    for (int i = 0; i < count; ++i)
    {
        auto cs = TC->getCase(i);
        auto btn = ControlButton::create(cs->name.c_str(), "", 18);
        btn->setTag(i);
        btn->setAnchorPoint(Point(0, 1));
        btn->setPosition(0, containerSize.height - y);
        btn->addTargetWithActionForControlEvents(this, cccontrol_selector(CMatchTestLayer::onCaseClick), Control::EventType::TOUCH_UP_INSIDE);
        
        btn->setTitleColorForState(Color3B(0xFF, 0xFF, 0xFF), Control::State::NORMAL);
        btn->setTitleColorForState(Color3B(0xFF, 0, 0), Control::State::HIGH_LIGHTED);
        btn->setTitleColorForState(Color3B(0xFF, 0, 0), Control::State::SELECTED);
        btn->setTitleColorForState(Color3B(0xFF, 0, 0), Control::State::DISABLED);
        
        float w = btn->getContentSize().width;
        if (w > containerSize.width)
        {
            containerSize.width = w;
        }
        m_caseListContainer->addChild(btn);
        
        y += GAP;
    }
    
    m_caseListContainer->setContentSize(containerSize);
    m_caseList->setContentOffset(Point(0, m_caseList->minContainerOffset().y));
}


void CMatchTestLayer::onCaseClick(Ref* obj, Control::EventType)
{
    ControlButton* btn = dynamic_cast<ControlButton*>(obj);
    if (btn && m_caseList->isNodeVisible(btn))
    {
        int idx = btn->getTag();
        auto count = TC->getCaseCount();
        if (idx >= 0 && idx < count)
        {
            for (int i = 0; i < count; ++i)
            {
                ControlButton* _btn = dynamic_cast<ControlButton*>(m_caseListContainer->getChildByTag(i));
                if (_btn)
                {
                    _btn->setEnabled(true);
                }
            }
            
            btn->setEnabled(false);
        }
        
        MT->startCase(idx);
    }
}

void CMatchTestLayer::createBorder()
{
    auto winSz = Director::getInstance()->getVisibleSize();
    m_borderNode = DrawNode::create();
    m_borderNode->drawSegment(Point(winSz.width * 0.45, winSz.height * 0.9f), Point(winSz.width * 0.95f, winSz.height * 0.9), .5f, Color4F(0, 1.f, 0, 1.f));
    m_borderNode->drawSegment(Point(winSz.width * 0.45, winSz.height * 0.05f), Point(winSz.width * 0.95f, winSz.height * 0.05f), .5f, Color4F(0, 1.f, 0, 1.f));
    m_borderNode->drawSegment(Point(winSz.width * 0.45, winSz.height * 0.05f), Point(winSz.width * 0.45, winSz.height * 0.9f), 0.5f, Color4F(0, 1.f, 0, 1.f));
    m_borderNode->drawSegment(Point(winSz.width * 0.95, winSz.height * 0.05f), Point(winSz.width * 0.95, winSz.height * 0.9f), 0.5f, Color4F(0, 1.f, 0, 1.f));
    addChild(m_borderNode);
}



void  CMatchTestLayer::createLogList()
{
    auto winSz = Director::getInstance()->getVisibleSize();
    Size svSz(winSz.width * (0.95f - 0.45f) - 10, winSz.height * (0.9f - 0.05f) - 10);
    m_logList = ScrollView::create(svSz);
    
    m_logList->setPosition(Point(winSz.width * 0.45f + 5, winSz.height * 0.05f + 5));
    m_logList->setDirection(ScrollView::Direction::VERTICAL);
    
    m_logListContainer = LayerColor::create(Color4B(0, 0, 0, 128));
    m_logListContainer->setContentSize(svSz);
    m_logList->setContainer(m_logListContainer);
    
    addChild(m_logList);
}



void CMatchTestLayer::addLog(const string& text, Color3B color)
{
    auto sz = m_logListContainer->getContentSize();
    auto lb = Label::createWithSystemFont(text, "Helvetica", 16, Size(sz.width, 0));
    m_logListContainer->addChild(lb);
    
    float height = sz.height;
    const auto& logs = m_logListContainer->getChildren();
    for (auto obj : logs)
    {
        obj->setPosition(0, height);
        height -= obj->getContentSize().height + 2;
    }
}

