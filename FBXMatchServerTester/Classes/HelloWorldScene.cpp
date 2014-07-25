#include "HelloWorldScene.h"
#include "CCCBTestLayer.h"
#include "CMatchTestLayer.h"


USING_NS_CC;
USING_NS_CC_EXT;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    closeItem->setAnchorPoint(Point(1, 0));
    closeItem->setPosition(Point(visibleSize.width, 0));
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);
    
    ControlButton* ccbBtn = ControlButton::create("CCB Test", "", 50);
    ccbBtn->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.6);
    ccbBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::onCCBTestButtonClicked), Control::EventType::TOUCH_UP_INSIDE);
    addChild(ccbBtn);
    
    ControlButton* matchBtn = ControlButton::create("Match Server Test", "", 50);
    matchBtn->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.4);
    matchBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::onMatchTestButtonClicked), Control::EventType::TOUCH_UP_INSIDE);
    addChild(matchBtn);
    
    return true;
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}



void HelloWorld::onCCBTestButtonClicked(Ref*, Control::EventType)
{
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CCCBTestLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);

    Director::getInstance()->pushScene(scene);
}



void HelloWorld::onMatchTestButtonClicked(Ref*, Control::EventType)
{
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CMatchTestLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    Director::getInstance()->pushScene(scene);
}



