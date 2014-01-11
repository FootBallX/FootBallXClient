//
//  CTransitLayer.h
//  GingyBattle
//
//  Created by Ming Zhao on 3/1/13.
//  Copyright (c) 2013 Tencent. All rights reserved.
//

#ifndef __GingyBattle__GBTransitLayer__
#define __GingyBattle__GBTransitLayer__

#include "CBaseLayer.h"
#include "CSingleton.h"

class CTransitLayer : public CBaseLayer , public CSingleton<CTransitLayer>
{
    CC_SYNTHESIZE(LAYERS, m_outLayer, OutLayer);
    
public:
    CTransitLayer();
    virtual ~CTransitLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void unLoad();
    
protected:

    virtual bool addLayers();
    
private:
    Scale9Sprite* pBackground_;

    void runLoadingAnimation(bool needCallback);
    void loadingAnimationDidFinished();
};

#define TRANSIT_LAYER   (CTransitLayer::getInstance())

#endif /* defined(__GingyBattle__GBTransitLayer__) */
