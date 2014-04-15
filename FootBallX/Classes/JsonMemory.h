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

void json_memory_init();
void* json_malloc(size_t s);
void json_free(void* p);

void json_memory_dump();

class CJsonMemPool
{
public:
    CJsonMemPool();
    ~CJsonMemPool();
    
    void* pool_alloc(size_t s);
    void pool_free(void* p);
protected:
    enum class BlockSizeType
    {
        B32,
        B64,
        B128,
        B256,
        B512,
        NONE,
    };
    
    void* m_pool[(int)BlockSizeType::NONE];
    
    int m_typeSize[(int)BlockSizeType::NONE] =    { 32,  64, 128, 256, 512};
    int m_size[(int)BlockSizeType::NONE] =        {128,  64,  64,  32,  32};
};

#endif /* defined(__FootBallX__JsonMemory__) */
