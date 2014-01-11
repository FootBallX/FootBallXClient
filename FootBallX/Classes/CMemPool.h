//
//  CMemPool.h
//  GameX
//
//  Created by 马 俊 on 13-8-15.
//
//

#ifndef __GameX__CMemPool__
#define __GameX__CMemPool__

#include <list>
#include <map>
#include <algorithm>
#include <assert.h>


using namespace std;

template <class T, int S>
class CMemPool
{
public:
    CMemPool() : m_usedCount(0)
    {
        m_pBuffer = ::new T[S];
        for (int i = 0; i < S; ++i)
        {
            m_pool.push_back(m_pBuffer + i);
        }
    }
    virtual ~CMemPool()
    {
        ::delete []m_pBuffer;
        m_pool.clear();
    }
    
    T* alloc()
    {
        if (m_usedCount < S)
        {
            m_usedCount++;
            T* p = m_pool.front();
            m_pool.pop_front();
            return p;
        }
        else
        {
            assert(false);
        }
        
        return nullptr;
    }

    
    void deAlloc(T* p)
    {
        if (m_usedCount <= 0)
        {
            m_usedCount = 0;
            return;
        }

        m_usedCount--;
        m_pool.push_front(p);

    }

private:
    typedef list<T*> LC;
    typedef typename list<T*>::iterator LC_IT;
    
    typedef map<T*, LC_IT> MTIT;
    typedef typename map<T*, LC_IT>::iterator MTIT_IT;
    
    int m_usedCount;
    T* m_pBuffer;
    LC m_pool;
    MTIT m_usedPool;
private:
};

#define MEM_POOL_DECLARE(__CLASS__, __SIZE__)\
static CMemPool<__CLASS__, __SIZE__>* m_pMemPool;\
inline void* operator new(size_t nSize )\
{\
    if (!m_pMemPool)\
    {\
        m_pMemPool = new CMemPool<__CLASS__, __SIZE__>;\
    }\
    \
    return m_pMemPool->alloc();\
}\
inline void operator delete(void* p)\
{\
    if (m_pMemPool)\
    {\
        m_pMemPool->deAlloc((__CLASS__*)p);\
    }\
};

#define MEM_POOL_DEFINE(__CLASS__, __SIZE__)\
CMemPool<__CLASS__, __SIZE__>* __CLASS__::m_pMemPool = nullptr;

#endif /* defined(__GameX__CMemPool__) */
