//
//  CJsonT.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-4-15.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CJsonT.h"
#include "JsonMemory.h"

CJsonT::CJsonT()
{
    m_value = json_object();
}



CJsonT::CJsonT(const CJsonT& obj)
{
    m_value = obj.m_value;
    json_incref(m_value);
}



CJsonT::CJsonT(json_t* jo) : m_value(jo)
{
    json_incref(m_value);
}



CJsonT::CJsonT(const char* str)
{
    m_value = json_string(str);
}



CJsonT::CJsonT(float value) : CJsonT((double)value)
{
    
}



CJsonT::CJsonT(double value)
{
    m_value = json_real(value);
}



CJsonT::CJsonT(int value) : CJsonT((json_int_t)value)
{
}



CJsonT::CJsonT(unsigned int value) : CJsonT((json_int_t)value)
{
}



CJsonT::CJsonT(json_int_t value)
{
    m_value = json_integer(value);
}



CJsonT::CJsonT(bool value)
{
    m_value = json_boolean(value);
}



CJsonT::~CJsonT()
{
    if (m_value)
    {
        json_decref(m_value);
    }
}



CJsonT::operator json_t*() const
{
    return m_value;
}



CJsonT& CJsonT::operator= (const CJsonT& obj)
{
    m_value = obj.m_value;
    json_incref(m_value);
    
    return *this;
}



void CJsonT::setChild(const char* key, const CJsonT& cjo)
{
    json_object_set(m_value, key, cjo);
}



void CJsonT::setChild(const char* key, json_t* jo)
{
    json_object_set(m_value, key, jo);
}



float CJsonT::toFloat()
{
    return (float)toDouble();
}



double CJsonT::toDouble()
{
    return (double)json_number_value(m_value);
}



int CJsonT::toInt()
{
    if (json_is_integer(m_value))
    {
        return (int)json_integer_value(m_value);
    }
    
    return 0;
}



unsigned int CJsonT::toUInt()
{
    if (json_is_integer(m_value))
    {
        return (unsigned int)json_integer_value(m_value);
    }
    
    return 0;
}


json_int_t CJsonT::toJsonInt()
{
    if (json_is_integer(m_value))
    {
        return json_integer_value(m_value);
    }
    
    return 0;
}



bool CJsonT::toBool()
{
    return json_typeof(m_value) == JSON_TRUE;
}



const char* CJsonT::toString()
{
    if (json_is_string(m_value))
    {
        return json_string_value(m_value);
    }
    
    return nullptr;
}



float CJsonT::getFloat(const char* key)
{
    return (float)getDouble(key);
}



double CJsonT::getDouble(const char* key)
{
    return CJsonT(json_object_get(m_value, key)).toDouble();
}



int CJsonT::getInt(const char* key)
{
    return CJsonT(json_object_get(m_value, key)).toInt();
}



unsigned int CJsonT::getUInt(const char* key)
{
    return CJsonT(json_object_get(m_value, key)).toUInt();
}



json_int_t CJsonT::getJsonInt(const char* key)
{
    return CJsonT(json_object_get(m_value, key)).toJsonInt();
}



bool CJsonT::getBool(const char* key)
{
    return CJsonT(json_object_get(m_value, key)).toBool();
}



const char* CJsonT::getString(const char* key)
{
    return CJsonT(json_object_get(m_value, key)).toString();
}



json_t* CJsonT::getChild(const char* key)
{
    return json_object_get(m_value, key);
}



void CJsonT::setChild(const char* key, float value)
{
    setChild(key, CJsonT(value));
}



void CJsonT::setChild(const char* key, double value)
{
    setChild(key, CJsonT(value));
}



void CJsonT::setChild(const char* key, int value)
{
    setChild(key, CJsonT(value));
}



void CJsonT::setChild(const char* key, unsigned int value)
{
    setChild(key, CJsonT(value));
}



void CJsonT::setChild(const char* key, json_int_t value)
{
    setChild(key, CJsonT(value));
}



void CJsonT::setChild(const char* key, const char* value)
{
    setChild(key, CJsonT(value));
}



void CJsonT::setChild(const char* key, bool value)
{
    setChild(key, CJsonT(value));
}



std::string CJsonT::dump()
{
    auto s = json_dumps(m_value, JSON_COMPACT);
    std::string str = s;
    json_free(s);
    
    return str;
}



void CJsonT::release()
{
    m_value = nullptr;
}


#pragma mark - CJsonTArray

CJsonTArray::CJsonTArray()
{
    m_value = json_array();
}



CJsonTArray::CJsonTArray(json_t* jo)
{
    if (json_is_array(jo))
    {
        m_value = jo;
        json_incref(m_value);
    }
}



CJsonTArray::~CJsonTArray()
{
    if (m_value)
    {
        json_decref(m_value);
    }
}



void CJsonTArray::append(const CJsonT& cjo)
{
    json_array_append(m_value, cjo);
}



void CJsonTArray::insert(size_t index, const CJsonT& cjo)
{
    json_array_insert(m_value, index, cjo);
}



void CJsonTArray::set(size_t index, const CJsonT& cjo)
{
    json_array_set(m_value, index, cjo);
}



CJsonT CJsonTArray::get(size_t index)
{
    return CJsonT(json_array_get(m_value, index));
}



void CJsonTArray::remove(size_t index)
{
    json_array_remove(m_value, index);
}



void CJsonTArray::clear()
{
    json_array_clear(m_value);
}



size_t CJsonTArray::size()
{
    return json_array_size(m_value);
}



CJsonT CJsonTArray::operator[] (int index)
{
    return get(index);
}



CJsonTArray::operator json_t*()
{
    return m_value;
}



void CJsonTArray::release()
{
    m_value = nullptr;
}



