//
//  JsonMemory.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-4-15.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "JsonMemory.h"
#include "Common.h"

static int ct = 0;

static int gcount = 0;

static pthread_mutex_t  mt;

static vector<size_t> v;
map<void*, size_t> st;

static CJsonMemPool g_memPool;

void json_memory_init()
{
    pthread_mutex_init(&mt, NULL);
    json_set_alloc_funcs(json_malloc, json_free);
    pc_json_set_alloc_funcs(json_malloc, json_free);
}


int b32 = 0;
int b64 = 0;
int b128 = 0;
int b256 = 0;
int b512 = 0;
int b1024 = 0;
int b = 0;


int mb32 = 0;
int mb64 = 0;
int mb128 = 0;
int mb256 = 0;
int mb512 = 0;
int mb1024 = 0;
int mb = 0;

void json_memory_dump()
{
    for (auto x : v)
    {
        if (x <= 32) b32++;
        else if (x <= 64) b64++;
        else if (x <= 128) b128++;
        else if (x <= 256) b256++;
        else if (x <= 512) b512++;
        else if (x <= 1024) b1024++;
        else b++;
    }
    
    log("b32: %d, b64: %d, b128: %d, b256: %d, b512: %d, b1024: %d, other: %d",
        mb32, mb64, mb128, mb256, mb512, mb1024, mb);
    
}


void* json_malloc(size_t s)
{
    pthread_mutex_lock(&mt);
    
    void* p = g_memPool.pool_alloc(s);
    st[p] = s;
    
    if (s <= 32) b32++;
    else if (s <= 64) b64++;
    else if (s <= 128) b128++;
    else if (s <= 256) b256++;
    else if (s <= 512) b512++;
    else if (s <= 1024) b1024++;
    else b++;
    
    mb32 = max(mb32, b32);
    mb64 = max(mb64, b64);
    mb128 = max(mb128, b128);
    mb256 = max(mb256, b256);
    mb512 = max(mb512, b512);
    mb1024 = max(mb1024, b1024);
    mb = max(mb, b);
    
    v.push_back(s);
    ct++;
    gcount++;
    log("%d+++ ct: %d", gcount, ct);
    pthread_mutex_unlock(&mt);
    return p;
}

void json_free(void* p)
{
    pthread_mutex_lock(&mt);
    
    auto s = st[p];
    st.erase(p);
    
    if (s <= 32) b32--;
    else if (s <= 64) b64--;
    else if (s <= 128) b128--;
    else if (s <= 256) b256--;
    else if (s <= 512) b512--;
    else if (s <= 1024) b1024--;
    else b--;
    
    ct--;
    gcount++;
    log("%d--- ct: %d", gcount, ct);
    g_memPool.pool_free(p);
    if (gcount == 45)
    {
        int a = 0;
        a++;
    }
    pthread_mutex_unlock(&mt);
}



CJsonMemPool::CJsonMemPool()
{
    int num = (int)BlockSizeType::NONE;
    // buff结构，第一个int存放一个索引,表示分配到哪里的下标
    // 每个buff块，前面有一个WORD标记是否被分配，0表示未分配，>0表示分配了，数字是分配的长度
    
    for (int i = 0; i < num; ++i)
    {
        int sz = (m_typeSize[i] + sizeof(unsigned short)) * m_size[i] + sizeof(int);
        m_pool[i] = malloc(sz);
        memset(m_pool[i], 0, sz);
    }
}




CJsonMemPool::~CJsonMemPool()
{
    for (auto x : m_pool)
    {
        free(x);
    }
}



void* CJsonMemPool::pool_alloc(size_t s)
{
    void* retP = nullptr;
    int debct = 0;
    
    for (int i = 0; i < (int)BlockSizeType::NONE; ++i)
    {
        if (m_typeSize[i] >= s)
        {
            int* p = (int*)m_pool[i];
            int& startIdx = *p;
            ++p;
            
            unsigned char* startP = (unsigned char*)p;
            
            int idx = startIdx;
            unsigned char* allocP = startP + (m_typeSize[i] + sizeof(unsigned short)) * idx;
            
            do
            {
                debct++;
                if (*allocP == 0)
                {
                    retP = allocP + sizeof(unsigned short);
                    *((unsigned short*)allocP) = s;
                    break;
                }
                idx++;
                if (idx >= m_size[i])
                {
                    idx = 0;
                }
                
                allocP = startP + (m_typeSize[i] + sizeof(unsigned short)) * idx;

            } while (idx != startIdx);
            
            if (nullptr != retP)
            {
                startIdx = (idx + 1) % m_size[i];
                break;
            }
        }
    }
    log("count: %d", debct);
	return retP;
}



void CJsonMemPool::pool_free(void* p)
{
    unsigned char* allocP = (unsigned char*)p - sizeof(unsigned short);
    *((unsigned short*)allocP) = 0;
}




