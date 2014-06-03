//
//  CTestCaseManager.h
//  FBXMatchServerTester
//
//  Created by 马 俊 on 14-6-3.
//
//

#ifndef __FBXMatchServerTester__CTestCaseManager__
#define __FBXMatchServerTester__CTestCaseManager__

#include "Common.h"
#include "CSingleton.h"
#include "FBDefs.h"

class CTestPlayerInfo
{
public:
    int playerNumber = -1;
    Point position;
    FBDefs::MENU_ITEMS instruction = FBDefs::MENU_ITEMS::None;
};

class CTestCase
{
public:
    CTestCase() = default;
    ~CTestCase()
    {
        for (auto p : m_atk)
        {
            delete p;
        }
        for (auto p : m_def)
        {
            delete p;
        }
        
        m_atk.clear();
        m_def.clear();
    }
    
    vector<CTestPlayerInfo*> m_atk;
    vector<CTestPlayerInfo*> m_def;
    int ballPos = -1;
    string name;
};

class CTestCaseManager : public CSingleton<CTestCaseManager>
{
public:
    CTestCaseManager() = default;
    ~CTestCaseManager();
    
    bool init();
    
    CTestCase* getCase(int idx);
protected:
    bool parse(const string& buf);
    CTestCase* parseCase(const string& buf);
    bool parseAtkOrDef(const string& line, vector<CTestPlayerInfo*>& v);
    bool parseBallPos(const string& line, CTestCase* tc);
    
    vector<CTestCase*> m_cases;
};


#define TC      (CTestCaseManager::getInstance())

#endif /* defined(__FBXMatchServerTester__CTestCaseManager__) */
