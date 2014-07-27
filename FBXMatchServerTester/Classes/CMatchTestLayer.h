//
//  CMatchTestLayer.h
//  FBXMatchServerTester
//
//  Created by Ray M on 14-7-25.
//
//

#ifndef __FBXMatchServerTester__CMatchTestLayer__
#define __FBXMatchServerTester__CMatchTestLayer__

#include "Common.h"

class CMatchTestLayer : public Layer
{
public:
    CREATE_FUNC(CMatchTestLayer);
    
    CMatchTestLayer();
    virtual ~CMatchTestLayer();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    virtual void addLog(const string& text, Color3B color = Color3B::GREEN);
protected:
    void onBack(Ref*, Control::EventType);
    void onStartTest(Ref*, Control::EventType);
    void onCaseClick(Ref*, Control::EventType);
    
    void createCaseList();
    void createBorder();
    void createLogList();
private:
    ScrollView* m_caseList;
    LayerColor* m_caseListContainer;
    DrawNode* m_borderNode;
    ScrollView* m_logList;
    LayerColor* m_logListContainer;
};

#endif /* defined(__FBXMatchServerTester__CMatchTestLayer__) */
