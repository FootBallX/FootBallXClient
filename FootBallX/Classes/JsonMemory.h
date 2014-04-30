//
//  JsonMemory.h
//  FootBallX
//
//  Created by 马 俊 on 14-4-15.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__JsonMemory__
#define __FootBallX__JsonMemory__

#include <stddef.h>
#include <vector>
#ifdef DEBUG
#include <map>
#include <numeric>
#endif

void json_memory_init();
void* json_malloc(size_t s);
void json_free(void* p);

void json_memory_dump();

class CJsonMemPool
{
public:
    CJsonMemPool(const std::vector<int>& poolSize);
    ~CJsonMemPool();
    
    void* pool_alloc(size_t s);
    void pool_free(void* p);
    
#ifdef DEBUG
    void debugInfo();
    bool checkSize(void* p, int s);
    
    std::vector<int> m_curAlloc;
    std::vector<int> m_peakAlloc;
    std::map<void*, int> m_allocMap;
    
    std::map<void*, int> m_rowPool;
#endif
    
protected:
    void* m_pool;
    int m_totalSize = 0;

    std::vector<int> m_poolSize;
    
    typedef int BLOCK_HEADER_TYPE;
    typedef int UNIT_HEADER_TYPE;
    
    const int START_BUFFER_SIZE = 32;
    
};

#endif /* defined(__FootBallX__JsonMemory__) */
