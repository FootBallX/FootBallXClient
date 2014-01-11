//
//  CGameSceneManager.h
//  GameX
//
//  Created by 马 俊 on 13-8-29.
//
//

#ifndef __GameX__CGameSceneManager__
#define __GameX__CGameSceneManager__

#include "Common.h"
#include "CSingleton.h"
#include "CCBReader/CCBReader.h"

enum SCENE_TYPE
{
    ST_LOGIN,
    ST_MATCH,
    ST_NUM,
};


class CGameSceneManager : public CSingleton<CGameSceneManager>
{
public:
    CGameSceneManager();
    virtual ~CGameSceneManager();
    
    virtual bool initialize();
    
    virtual bool go(SCENE_TYPE st);

protected:
private:
    Array* m_scenes;
};

#define SCENE_MANAGER           (CGameSceneManager::getInstance())

#endif /* defined(__GameX__CGameSceneManager__) */
