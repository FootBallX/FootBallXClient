//
//  CTestCaseManager.cpp
//  FBXMatchServerTester
//
//  Created by 马 俊 on 14-6-3.
//
//

#include "CTestCaseManager.h"

IMPLEMENT_SINGLETON(CTestCaseManager);


CTestCaseManager::~CTestCaseManager()
{
    for (auto p : m_cases)
    {
        delete p;
    }
    
    m_cases.clear();
}



bool CTestCaseManager::init()
{
    do
    {
        const char* filename = "testCases.txt";
        string fullFilename = FileUtils::getInstance()->fullPathForFilename(filename);
        FILE *fp = fopen(fullFilename.c_str(), "r");
        BREAK_IF(nullptr == fp);
        fseek(fp, 0, SEEK_END);
        auto len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        string str;
        str.resize(len + 1, '\0');
        fread(&(str[0]), len, 1, fp);

        BREAK_IF_FAILED(parse(str));
        return true;
    } while (true);
    
    return false;
}


bool CTestCaseManager::parse(const string& buf)
{
    do
    {
        const string begin = "------>";
        const string end = "<------";
        
        auto pos = buf.find(begin, 0);

        while (pos != string::npos)
        {
            pos += begin.size();
            auto ePos = buf.find(end, pos + 1);
            string caseString = buf.substr(pos, ePos - pos);
            auto ret = parseCase(caseString);
            CC_ASSERT(ret);
            
            m_cases.push_back(ret);
            
            pos = buf.find(begin, ePos + 1);
            
            BREAK_IF(pos > 0 && buf[pos-1] == 's');
        }
        return true;
    } while (false);
    
    return false;
}



CTestCase* CTestCaseManager::parseCase(const string& buf)
{
    do
    {
        auto_ptr<CTestCase> tc(new CTestCase);
        
        auto pos = buf.find("\n", 0);
        int bPos = 0;
        while (pos != string::npos)
        {
            string line = buf.substr(bPos, pos - bPos);
            if (bPos == 0)
            {
                tc->name = line;
            }
            else
            {
                switch (line[0])
                {
                    case 'a':
                        if (!parseAtkOrDef(line.substr(2, line.size()-2), tc->m_atk))
                        {
                            return nullptr;
                        }
                        break;
                    case 'b':
                        if (!parseBallPos(line.substr(2, line.size()-2), tc.get()))
                        {
                            return nullptr;
                        }
                        break;
                    case 'd':
                        if (!parseAtkOrDef(line.substr(2, line.size()-2), tc->m_def))
                        {
                            return nullptr;
                        }
                        break;
                    case 'r':
                        if (!parseRepeatCounts(line.substr(2, line.size()-2), tc.get()))
                        {
                            return nullptr;
                        }
                        break;
                    case 'u':
                        if (!parseActiveIns(line.substr(2, line.size()-2), tc.get()))
                        {
                            return nullptr;
                        }
                        break;
                }
            }
            bPos = pos + 1;
            pos = buf.find("\n", bPos);
        }
        
        return tc.release();
    } while (false);
    
    return nullptr;
}



bool CTestCaseManager::parseAtkOrDef(const string& line, vector<CTestPlayerInfo*>& v)
{
    int bPos = 0;
    auto pos = line.find(";", bPos);
    if (pos == string::npos)
    {
        pos = line.find("\n", bPos);
        if (pos == string::npos)
        {
            pos = line.size();
        }
    }
    while (pos != string::npos && bPos < line.size())
    {
        string elem = line.substr(bPos, pos - bPos);
        bPos = pos + 1;
        
        auto_ptr<CTestPlayerInfo> tpi(new CTestPlayerInfo);
        // elem is like: 9->(160,150)pass
        {
            auto elemPos = elem.find("->(");
            tpi->playerNumber = atoi(elem.substr(0, elemPos).c_str());
            if (tpi->playerNumber < 0 || tpi->playerNumber >= 11)
            {
                return false;
            }
            
            auto elemStartPos = elemPos + 3;
            elemPos = elem.find(",", elemStartPos);
            if (elemPos == string::npos)
            {
                return false;
            }
            tpi->position.x = atof(elem.substr(elemStartPos, elemPos - elemStartPos).c_str());
            if (tpi->position.x < 0 || tpi->position.x > FBDefs::PITCH_WIDTH)
            {
                return false;
            }
            
            elemStartPos = elemPos + 1;
            elemPos = elem.find(")", elemStartPos);
            if (elemPos == string::npos)
            {
                return false;
            }
            tpi->position.y = atof(elem.substr(elemStartPos, elemPos - elemStartPos).c_str());
            if (tpi->position.y < 0 || tpi->position.y > FBDefs::PITCH_HEIGHT)
            {
                return false;
            }
            
            elemStartPos = elemPos + 1;
            string ins = elem.substr(elemStartPos, elem.size() - elemStartPos);
            
            if (ins == "none")
            {
                tpi->instruction = FBDefs::MENU_ITEMS::None;
            }
            else if (ins == "pass")
            {
                tpi->instruction = FBDefs::MENU_ITEMS::Pass;
            }
            else if (ins == "dribble")
            {
                tpi->instruction = FBDefs::MENU_ITEMS::Dribble;
            }
            else if (ins == "tackle")
            {
                tpi->instruction = FBDefs::MENU_ITEMS::Tackle;
            }
            else if (ins == "block")
            {
                tpi->instruction = FBDefs::MENU_ITEMS::Block;
            }
            else if (ins == "intercept")
            {
                tpi->instruction = FBDefs::MENU_ITEMS::Intercept;
            }
            else if (ins == "onetwo")
            {
                tpi->instruction = FBDefs::MENU_ITEMS::OneTwo;
            }
            else if (ins == "shoot")
            {
                tpi->instruction = FBDefs::MENU_ITEMS::Shoot;
            }
            else if (ins == "catch")
            {
                tpi->instruction = FBDefs::MENU_ITEMS::Catch;
            }
            else if (ins == "hit")
            {
                tpi->instruction = FBDefs::MENU_ITEMS::Hit;
            }
            else
            {
                return false;
            }
        }
        
        v.push_back(tpi.release());
        
        pos = line.find(";", bPos);
        if (pos == string::npos)
        {
            pos = line.find("\n", bPos);
            if (pos == string::npos)
            {
                pos = line.size();
            }
        }
    }
    
	return true;
}



bool CTestCaseManager::parseBallPos(const string& line, CTestCase* tc)
{
    tc->ballPos = atoi(line.c_str());
    if (tc->ballPos < 0 || tc->ballPos >= 11) return false;
    
	return true;
}
                            
                            
                            
bool CTestCaseManager::parseRepeatCounts(const string& line, CTestCase* tc)
{
    tc->repeatCount = atoi(line.c_str());
    if (tc->repeatCount <= 0) return false;
    
    return true;
}



bool CTestCaseManager::parseActiveIns(const string& line, CTestCase* tc)
{
    tc->active = (atoi(line.c_str()) == 1);
    
    return true;
}



CTestCase* CTestCaseManager::getCase(int idx)
{
    if (idx >= 0 && idx < m_cases.size())
    {
        return m_cases[idx];
    }
    
    return nullptr;
}