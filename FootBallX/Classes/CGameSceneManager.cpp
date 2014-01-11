//
//  CGameSceneManager.cpp
//  GameX
//
//  Created by 马 俊 on 13-8-29.
//
//

#include "CGameSceneManager.h"
#include "CLoginLayer.h"
#include "CMatchLayer.h"


IMPLEMENT_SINGLETON(CGameSceneManager);


CGameSceneManager::CGameSceneManager()
: m_scenes(nullptr)
{
    
}



CGameSceneManager::~CGameSceneManager()
{
    CC_SAFE_RELEASE(m_scenes);
}



bool CGameSceneManager::initialize()
{
    do
    {
        m_scenes = Array::createWithCapacity(ST_NUM);
        CC_SAFE_RETAIN(m_scenes);

        return true;
    } while (false);
    
    
    return false;
}


bool CGameSceneManager::go(SCENE_TYPE st)
{
    Scene* scene = nullptr;
    switch (st)
    {
        case ST_LOGIN:
        {
            CCBReader* pReader = new CCBReader(NodeLoaderLibrary::getInstance());
            scene = pReader->createSceneWithNodeGraphFromFile("login_layer.ccbi");
            delete pReader;
            break;
        }
        case ST_MATCH:
        {
            CCBReader* pReader = new CCBReader(NodeLoaderLibrary::getInstance());
            scene = pReader->createSceneWithNodeGraphFromFile("pitch_layer.ccbi");
            delete pReader;
            break;
        }
        default:
            break;
    }
    if (scene)
    {
        Director* pDirector = Director::getInstance();
        if (pDirector->getRunningScene())
        {
            TransitionFade* trans = TransitionFade::create(.2f, scene, Color3B(255, 255, 255));
            pDirector->replaceScene(trans);
        }
        else
        {
            pDirector->runWithScene(scene);
        }
    }
    
    return true;
}
