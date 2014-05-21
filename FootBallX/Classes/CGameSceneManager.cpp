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
#include "CCBReadHelper.h"


IMPLEMENT_SINGLETON(CGameSceneManager);


CGameSceneManager::CGameSceneManager()
{
    
}



CGameSceneManager::~CGameSceneManager()
{
}



bool CGameSceneManager::initialize()
{
    do
    {
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
            scene = CCBReadHelper::readScene("login_layer.ccbi");
            break;
        }
        case ST_LOBBY:
        {
            scene = CCBReadHelper::readScene("lobby_layer.ccbi");
            break;
        }
        case ST_MATCH:
        {
            scene = CCBReadHelper::readScene("pitch_layer.ccbi");
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
