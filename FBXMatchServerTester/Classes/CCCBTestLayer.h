//
//  CCCBTestLayer.h
//  FBXMatchServerTester
//
//  Created by Ray M on 14-7-25.
//
//

#ifndef __FBXMatchServerTester__CCCBTestLayer__
#define __FBXMatchServerTester__CCCBTestLayer__

#include "Common.h"

class CCCBTestLayer : public Layer
{
public:
    CREATE_FUNC(CCCBTestLayer);
    
    CCCBTestLayer();
    virtual ~CCCBTestLayer();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
protected:
    void createCCBList();
    void createPathEdit();
    
    void onBack(Ref*, Control::EventType);
    void onLoadCCBList(Ref*, Control::EventType);
    void onCCBClick(Ref*, Control::EventType);
    
    void playCCB(const string& name);
private:
    ScrollView* m_ccbListView;
    EditBox* m_editPath;
    LayerColor* m_container;
    vector<string> m_ccbList;
    Node* m_ccbi;
};

#endif /* defined(__FBXMatchServerTester__CCCBTestLayer__) */
