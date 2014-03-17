//
//  CFBFunctionsJS.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-3-8.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBFunctionsJS.h"
#include "CFBCard.h"
#include "../JSBinds/CFBCardJSBinds.hpp"
#include "CFBCardManager.h"
#include "CRandomManager.h"
#include "CFBMatch.h"

IMPLEMENT_SINGLETON(CFBFunctionsJS);

JSBool JSlog(JSContext* cx, uint32_t argc, jsval *vp)
{
    if (argc > 0) {
        JSString *string = NULL;
        JS_ConvertArguments(cx, argc, JS_ARGV(cx, vp), "S", &string);
        if (string) {
            JSStringWrapper wrapper(string);
            cocos2d::log("%s", wrapper.get());
        }
    }
    return JS_TRUE;
}


JSBool JSRand(JSContext* cx, uint32_t argc, jsval *vp)
{
	jsval out = INT_TO_JSVAL(RANDOM_MGR->getRand());
	JS_SET_RVAL(cx, vp, out);
    return JS_TRUE;
}


JSBool JSPlayAnimation(JSContext* cx, uint32_t argc, jsval *vp)
{
    if (argc == 2)
    {
        JSString *name = NULL;
        double delay;
        JS_ConvertArguments(cx, argc, JS_ARGV(cx, vp), "Sd", &name, &delay);
        if (name) {
            JSStringWrapper wrapper(name);
            FBMATCH->playAnimation(wrapper.get(), (float)delay);
        }
        
        return JS_TRUE;
    }
    
    return JS_FALSE;
}



bool CFBFunctionsJS::init()
{
    auto sc = ScriptingCore::getInstance();
    cocos2d::ScriptEngineManager::getInstance()->setScriptEngine(sc);
    do
    {
        sc->addRegisterCallback(register_all_CFBCardJSBinds);
        sc->start();
        
        JSBool ret;
        ret = sc->runScript("CompactFunctions.js");
        BREAK_IF(ret == JS_FALSE);
        
        this->_cx = sc->getGlobalContext();
        this->_go = sc->getGlobalObject();

        JSAutoCompartment ac(_cx, _go);
        
        JS_DefineFunction(_cx, _go, "Log", JSlog, 0, JSPROP_READONLY | JSPROP_PERMANENT);
        JS_DefineFunction(_cx, _go, "Rand", JSRand, 0, JSPROP_READONLY | JSPROP_PERMANENT);
        JS_DefineFunction(_cx, _go, "PlayAnimation", JSPlayAnimation, 0, JSPROP_READONLY | JSPROP_PERMANENT);
        
        return true;
    } while (false);
    
    return false;
}

// this function copies co to a new object and pass it to js. And this new object will be deleted automatic by GC.
jsval CFBFunctionsJS::createJsvalFromCard(const CFBCard& co)
{
    JSAutoCompartment ac(_cx, _go);
    
    TypeTest<CFBCard> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    JSObject *obj = JS_NewObject(_cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
    jsb_new_proxy(new CFBCard(co), obj);
    
    return OBJECT_TO_JSVAL(obj);
}


jsval CFBFunctionsJS::callJSFunc2Obj(const char* name, const CFBCard& c1, const CFBCard& c2)
{
    jsval res;
    jsval argv[2];
    argv[0] = createJsvalFromCard(c1);
    argv[1] = createJsvalFromCard(c2);

    JSAutoCompartment ac(_cx, _go);
    JS_CallFunctionName(_cx, _go, name, 2, argv, &res);

    auto sc = ScriptingCore::getInstance();
    sc->forceGC(_cx, 0, nullptr);
    return res ;
}



jsval CFBFunctionsJS::callJSFunc1Obj(const char *name, const CFBCard& c1)
{
    jsval res;
    jsval argv[1];
    argv[0] = createJsvalFromCard(c1);
    
    JSAutoCompartment ac(_cx, _go);
    JS_CallFunctionName(_cx, _go, name, 1, argv, &res);
    
    auto sc = ScriptingCore::getInstance();
    sc->forceGC(_cx, 0, nullptr);
    return res ;
}



jsval CFBFunctionsJS::callJSFunc(const char *name, int count, ...)
{
    ScriptingCore* sc = ScriptingCore::getInstance();
    int argc = count;
    jsval res;
    jsval *argv = new jsval[count];
    va_list insvalist;
    va_start(insvalist,count);
    for (int i = 0; i < count; ++i)
    {
        argv[i] = std_string_to_jsval(sc->getGlobalContext(),std::string(va_arg(insvalist,const char *)));
    }
    va_end(insvalist) ;
    JS_CallFunctionName(sc->getGlobalContext(), sc->getGlobalObject(), name, argc, argv, &res);
    delete [] argv;
    return res ;
}



float CFBFunctionsJS::getSpeed(const CFBCard& co)
{
    auto res = callJSFunc1Obj("GetSpeed", co);
    if (JSVAL_IS_DOUBLE(res))
    {
        return (float)JSVAL_TO_DOUBLE(res);
    }
    
    return 0.f;
}



void CFBFunctionsJS::startPassBall(const CFBCard& co1)
{
    callJSFunc1Obj("StartPassBall", co1);
}



bool CFBFunctionsJS::tackleBall(const CFBCard& co1, const CFBCard& co2)
{
    auto res = callJSFunc2Obj("TackleBall", co1, co2);
    if (JSVAL_TO_BOOLEAN(res))
    {
        return JS_TRUE == JSVAL_TO_BOOLEAN(res);
    }
    
    return false;
}



bool CFBFunctionsJS::interceptBall(const CFBCard& co1, const CFBCard& co2)
{
    auto res = callJSFunc2Obj("InterceptBall", co1, co2);
    if (JSVAL_TO_BOOLEAN(res))
    {
        return JS_TRUE == JSVAL_TO_BOOLEAN(res);
    }
    
    return false;
}



bool CFBFunctionsJS::blockBall(const CFBCard& co1, const CFBCard& co2)
{
    auto res = callJSFunc2Obj("BlockBall", co1, co2);
    if (JSVAL_TO_BOOLEAN(res))
    {
        return JS_TRUE == JSVAL_TO_BOOLEAN(res);
    }
    
    return false;
}



void CFBFunctionsJS::recieveBall(const CFBCard& co1)
{
    callJSFunc1Obj("RecieveBall", co1);
}



