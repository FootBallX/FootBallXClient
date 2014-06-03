//
//  CJsonT.h
//  FootBallX
//
//  Created by 马 俊 on 14-4-15.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CJsonT__
#define __FootBallX__CJsonT__

#include "jansson.h"
#include <string>

class CJsonT
{
public:
    CJsonT();
    CJsonT(const CJsonT& obj);
    CJsonT(json_t* jo);
    CJsonT(const char* str);
    CJsonT(float value);
    CJsonT(double value);
    CJsonT(int value);
    CJsonT(unsigned int value);
    CJsonT(json_int_t value);
    CJsonT(bool value);
    virtual ~CJsonT();
    
    operator json_t*() const;
    CJsonT& operator= (const CJsonT& obj);
    
    float toFloat();
    double toDouble();
    int toInt();
    unsigned int toUInt();
    json_int_t toJsonInt();
    bool toBool();
    const char* toString();
    
    float getFloat(const char* key);
    double getDouble(const char* key);
    int getInt(const char* key);
    unsigned int getUInt(const char* key);
    json_int_t getJsonInt(const char* key);
    bool getBool(const char* key);
    const char* getString(const char* key);
    json_t* getChild(const char* key);
    
    void setChild(const char* key, const CJsonT& cjo);
    void setChild(const char* key, json_t* jo);
    void setChild(const char* key, float value);
    void setChild(const char* key, double value);
    void setChild(const char* key, int value);
    void setChild(const char* key, unsigned int value);
    void setChild(const char* key, json_int_t value);
    void setChild(const char* key, const char* value);
    void setChild(const char* key, bool value);
    
    std::string dump();
    
    void release();
protected:
    json_t* m_value = nullptr;
};


#pragma mark - CJsonTArray

class CJsonTArray
{
public:
    CJsonTArray();
    CJsonTArray(json_t* jo);
    virtual ~CJsonTArray();
    
    void append(const CJsonT& cjo);
    void insert(size_t index, const CJsonT& cjo);
    void set(size_t index, const CJsonT& cjo);
    CJsonT get(size_t index);
    void remove(size_t index);
    void clear();
    size_t size();
    
    CJsonT operator [](int index);
    operator json_t*();
    
    void release();
    
protected:
    json_t* m_value = nullptr;
};

#endif /* defined(__FootBallX__CJsonT__) */
