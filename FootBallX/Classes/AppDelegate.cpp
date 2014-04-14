//
//  TheForceAppDelegate.cpp
//  TheForce
//
//  Created by Ming Zhao on 2/1/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "Common.h"
//#include "CGlobalConfigration.h"
//#include "CObjectFactory.h"
//#include "CPathManager.h"
//#include "TFCollisionMgr.h"
//#include "CRole.h"
//#include "CBatchNodeManager.h"
//#include "CDataCenterManager.h"
//#include "CBackgroundManager.h"
#include "CGameSceneManager.h"
#include "CFBCardManager.h"
//#include "CTimeProfile.h"
//#include "CShopManager.h"
#include <time.h>
#include "CFBFunctionsJS.h"
#include "CSyncedTime.h"


int ct = 0;

int gcount = 0;

pthread_mutex_t  mt;

void *jm(size_t s)
{
    pthread_mutex_lock(&mt);
    
    void* p = malloc(s);

    ct++;
    gcount++;
    log("%d+++ ct: %d", gcount, ct);
    pthread_mutex_unlock(&mt);
    return p;
}

void fr(void* p)
{
    pthread_mutex_lock(&mt);
    ct--;
    gcount++;
    log("%d--- ct: %d", gcount, ct);
    free(p);
    if (gcount == 45)
    {
        int a = 0;
        a++;
    }
    pthread_mutex_unlock(&mt);
}

AppDelegate::AppDelegate()
{
    time_t t;
    srand((unsigned) time(&t));
//    pthread_mutex_init(&mt, NULL);
//    json_set_alloc_funcs(jm, fr);
}

AppDelegate::~AppDelegate()
{

}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto pDirector = Director::getInstance();
    
    auto glview = pDirector->getOpenGLView();
    if(!glview) {
//        glview = GLView::create("FootBallX");
        float scale = 8;
        glview = GLView::createWithRect("FootBallX", Rect(0, 0, 96 * scale, 64 * scale));
        pDirector->setOpenGLView(glview);
    }
    
    pDirector->setOpenGLView(glview);
    
    pDirector->setProjection(Director::Projection::_3D);
    
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    pDirector->setDepthTest(true);
    
    this->setupConfigurationFiles();
    
    this->setupMultipleResolutionSupport();

    do
    {
//        BREAK_IF_FAILED(DATA_CENTER->initialize());
//        
//        BREAK_IF_FAILED(SHOP->initialize());
//        
//        BREAK_IF_FAILED(OBJECT_FACTORY->Initialize());
//        
//        BREAK_IF_FAILED(COLLISION_MANAGER->init());
//
////        BREAK_IF_FAILED(PATH_MANAGER->initialize());
//
//        BREAK_IF_FAILED(SCENE_MANAGER->initialize());
//
//        TP_INIT();
//

        BREAK_IF_FAILED(CARD_MGR->init());
        
        SCENE_MANAGER->go(ST_LOGIN);

        FB_FUNC_JS->init();
        
        return true;
    } while (false);
        

    return false;
}



// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}



// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}



void AppDelegate::setupConfigurationFiles()
{
//    const char* udFileName = "GameSettings.plist";
//
//    if (!CCUserDefault::sharedUserDefault()->getBoolForKey("_IS_EXISTED", false))
//    {
//        Dictionary* dict = Dictionary::createWithContentsOfFile(udFileName);
//        for (int i = 0; i < GAME_SETTINGS_MAX; i++)
//        {
//            const String* value = dict->valueForKey(GAME_SETTINGS_TEXT(i));
//            SaveStringToUserDefault(i, value->getCString());
//        }
//        
//        CCUserDefault::sharedUserDefault()->setBoolForKey("_IS_EXISTED", true);
//        
//        FlushUserDefault;
//    }
}


void AppDelegate::setupMultipleResolutionSupport()
{
    Size sizeIphone = Size(480, 320);
    Size sizeIphoneHD = Size(960, 640);
    Size sizeIphone5 = Size(1136, 640);
    Size sizeIpad = Size(1024, 768);
    Size sizeIpadHD = Size(2048, 1536);
    
    Size sizeSmall = Size(480, 320);
    Size sizeLarge = Size(1024, 768);
    Size sizeXLarge = Size(2048, 1536);
    
    Size designSize = sizeIphoneHD;
    Size resourceSize = sizeIphoneHD;
    
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();
    Size screenSize = glView->getFrameSize();
    
    if (screenSize.width < screenSize.height)
    {
        swap(screenSize.width, screenSize.height);
    }
    
    std::vector<std::string> searchPaths;
    std::vector<std::string> resDirOrders;
    
    resDirOrders.push_back("JS");
    
    Application::Platform platform = Application::getInstance()->getTargetPlatform();
    if (platform == Application::Platform::OS_IPHONE || platform == Application::Platform::OS_IPAD)
    {
        resDirOrders.push_back("Resources.ios");
        FileUtils::getInstance()->setSearchPaths(searchPaths);
        
        if (screenSize.height > 768)
        {
            resourceSize = sizeIpadHD;
            designSize = sizeIpadHD;
            resDirOrders.push_back("Resources.ios/resources-ipadhd");
            
            cocosbuilder::CCBReader::setResolutionScale(4.0f);
        } else if (screenSize.height > 640) {
            resourceSize = sizeIpad;
            designSize = resourceSize;
            resDirOrders.push_back("Resources.ios/resources-ipad");
            resDirOrders.push_back("Resources.ios/resources-iphonehd");
            
            cocosbuilder::CCBReader::setResolutionScale(2.0f);
        } else if (screenSize.height > 480) {
            if (screenSize.width > 960) {
                designSize = sizeIphone5;
            }
            else {
                designSize = sizeIphoneHD;
            }
            resourceSize = sizeIphoneHD;
            
            cocosbuilder::CCBReader::setResolutionScale(2.0f);
            
            resDirOrders.push_back("Resources.ios/resources-iphonehd");
        } else {
            cocosbuilder::CCBReader::setResolutionScale(1.0f);
            
            designSize = sizeIphone;
            resourceSize = sizeIphone; 
            
            resDirOrders.push_back("Resources.ios/resources-iphone"); 
        } 
    }
    else if (platform == Application::Platform::OS_ANDROID)
    {
        if (screenSize.height > 768)
        {
            resourceSize = sizeXLarge;
            designSize = resourceSize;
            resDirOrders.push_back("resources-xlarge");
            cocosbuilder::CCBReader::setResolutionScale(4.0f);
        }
        else if (screenSize.height > 320)
        {
            resourceSize = sizeLarge;
            designSize = resourceSize;
            resDirOrders.push_back("resources-large");
            cocosbuilder::CCBReader::setResolutionScale(2.0f);
        }
        else
        {
            resourceSize = sizeSmall;
            designSize = resourceSize;
            resDirOrders.push_back("resources-small");
            cocosbuilder::CCBReader::setResolutionScale(1.0f);
        }
    }
    else if (platform == Application::Platform::OS_MAC)
    {
        resDirOrders.push_back("Resources.ios");
        resDirOrders.push_back("Resources.ios/resources-iphonehd");
        cocosbuilder::CCBReader::setResolutionScale(2.0f);
    }

    FileUtils::getInstance()->setSearchResolutionsOrder(resDirOrders);
    
    Director* pDirector = Director::getInstance();
    pDirector->setContentScaleFactor(resourceSize.width / designSize.width);
    glView->setDesignResolutionSize(
                                    designSize.width, designSize.height,
                                    ResolutionPolicy::EXACT_FIT);
    
}


