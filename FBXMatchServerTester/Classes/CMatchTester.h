//
//  CMatchTester.h
//  FBXMatchServerTester
//
//  Created by 马 俊 on 14-5-30.
//
//

#ifndef __FBXMatchServerTester__CMatchTester__
#define __FBXMatchServerTester__CMatchTester__

#include "Common.h"
#include "CTestClient.h"
#include "CSingleton.h"
#include "CTestClient.h"

class CMatchTester
: public CSingleton<CMatchTester>
, public Node
{
public:
    CMatchTester() = default;
    ~CMatchTester() = default;
    
    virtual bool startAll();
    virtual bool startCase(int index);
    
protected:
    virtual void updateAll(float delta);
    virtual void update(float delta);
    
    CTestClient m_client[2];
    
    int m_caseIndex = 0;
};


#define MT  (CMatchTester::getInstance())

#endif /* defined(__FBXMatchServerTester__CMatchTester__) */
