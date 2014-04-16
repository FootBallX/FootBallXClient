//
//  JsonMemory.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-4-15.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "JsonMemory.h"
#include "Common.h"


static pthread_mutex_t  mt;
static CJsonMemPool g_memPool({128,  64,  64,  32,  32});

void json_memory_init()
{
    pthread_mutex_init(&mt, NULL);
    json_set_alloc_funcs(json_malloc, json_free);
    pc_json_set_alloc_funcs(json_malloc, json_free);
}


void json_memory_dump()
{
#ifdef DEBUG
    pthread_mutex_lock(&mt);
    g_memPool.debugInfo();
    pthread_mutex_unlock(&mt);
#endif
}


void* json_malloc(size_t s)
{
    pthread_mutex_lock(&mt);
    void* p = g_memPool.pool_alloc(s);
//    void* p = malloc(s);
    pthread_mutex_unlock(&mt);
    return p;
}

void json_free(void* p)
{
    pthread_mutex_lock(&mt);
    g_memPool.pool_free(p);
//    free(p);
    pthread_mutex_unlock(&mt);
}



CJsonMemPool::CJsonMemPool(const std::vector<int>& poolSize) : m_poolSize(poolSize)
{
    int num = (int)m_poolSize.size();
    
#ifdef DEBUG
    for (int i = 0; i < num ; ++i)
    {
        m_curAlloc.push_back(0);
        m_peakAlloc.push_back(0);
    }
#endif
    
    int poolHeaderSize = sizeof(void*) * num;
    int totalSize = poolHeaderSize;
    
    int i;
    int unitSize = START_BUFFER_SIZE;
    for (i = 0; i < num; ++i)
    {
        totalSize += (unitSize + sizeof(UNIT_HEADER_TYPE)) * m_poolSize[i] + sizeof(BLOCK_HEADER_TYPE);
        unitSize *= 2;
    }
    
    // buff结构，第一个int存放一个索引,表示分配到哪里的下标
    // 每个buff块，前面有一个WORD标记是否被分配，0表示未分配，>0表示分配了，数字是分配的长度
    m_pool = calloc(totalSize, sizeof(unsigned char));
    
    void** p = (void**)m_pool;
    int offset = 0;
    
    unitSize = START_BUFFER_SIZE;
    for (int i = 0; i < num; ++i)
    {
        *p = (unsigned char*)m_pool + poolHeaderSize + offset;
        offset += (unitSize + sizeof(UNIT_HEADER_TYPE)) * m_poolSize[i] + sizeof(BLOCK_HEADER_TYPE);
        ++p;
        
        unitSize *= 2;
    }
}




CJsonMemPool::~CJsonMemPool()
{
    free(m_pool);
}



void* CJsonMemPool::pool_alloc(size_t s)
{    
    void* retP = nullptr;
    
    int num = (int)m_poolSize.size();
    
    int unitSize = START_BUFFER_SIZE;
    for (int i = 0; i < num; ++i)
    {
        if (m_poolSize[i] > 0 && unitSize >= s)
        {
            BLOCK_HEADER_TYPE* p = (BLOCK_HEADER_TYPE*)(*(((void**)m_pool) + i));
            BLOCK_HEADER_TYPE& startIdx = *p;
            ++p;
            
            unsigned char* startP = (unsigned char*)p;
            
            int idx = startIdx;
            unsigned char* allocP = startP + (unitSize + sizeof(UNIT_HEADER_TYPE)) * idx;
            
            do
            {
                if (*((UNIT_HEADER_TYPE*)allocP) == 0)
                {
                    retP = allocP + sizeof(UNIT_HEADER_TYPE);
                    *((UNIT_HEADER_TYPE*)allocP) = s;
                    
#ifdef DEBUG
                    m_allocMap[retP] = i;
                    m_curAlloc[i]++;
                    m_peakAlloc[i] = max(m_curAlloc[i], m_peakAlloc[i]);
#endif
                    break;
                }
                idx++;
                if (idx >= m_poolSize[i])
                {
                    idx = 0;
                }
                
                allocP = startP + (unitSize + sizeof(UNIT_HEADER_TYPE)) * idx;

            } while (idx != startIdx);
            
            if (nullptr != retP)
            {
                startIdx = (idx + 1) % m_poolSize[i];
                break;
            }
        }
        
        unitSize *= 2;
    }

#ifdef DEBUG
    if (nullptr == retP)
    {
        debugInfo();
        
        log("pool is full!! ---> request size: %zu", s);
    }
#endif
	return retP;
}



void CJsonMemPool::pool_free(void* p)
{
    unsigned char* allocP = (unsigned char*)p - sizeof(UNIT_HEADER_TYPE);
    *((UNIT_HEADER_TYPE*)allocP) = 0;
    
#ifdef DEBUG
    auto it = m_allocMap.find(p);
    m_curAlloc[(*it).second]--;
    m_allocMap.erase(it);
#endif
}


#ifdef DEBUG
void CJsonMemPool::debugInfo()
{
    log("----------- debug info -----------");
    int num = (int)m_poolSize.size();
    
    int unitSize = START_BUFFER_SIZE;
    
    for (int i = 0; i < num; ++i)
    {
//        BLOCK_HEADER_TYPE* p = (BLOCK_HEADER_TYPE*)(*(((void**)m_pool) + i));
//        ++p;
//        
//        unsigned char* startP = (unsigned char*)p;
//        
//        int ct = 0;
//        for (int j = 0; j < m_poolSize[i]; ++j)
//        {
//            if (*((UNIT_HEADER_TYPE*)startP) > 0)
//            {
//                ct++;
//            }
//            
//            startP += unitSize + sizeof(UNIT_HEADER_TYPE);
//        }
        
        log("block size: %d, total size: %d, used: %d, peak used: %d", unitSize, m_poolSize[i], m_curAlloc[i], m_peakAlloc[i]);
        
        unitSize *= 2;
    }
    
    
    log("----------- debug info end -----------");
}


bool CJsonMemPool::checkSize(void* p, int s)
{
    unsigned char* allocP = (unsigned char*)p - sizeof(UNIT_HEADER_TYPE);
    return *((UNIT_HEADER_TYPE*)allocP) == s;
}
#endif


