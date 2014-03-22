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

bool JSlog(JSContext* cx, uint32_t argc, jsval *vp)
{
    if (argc > 0) {
        JSString *string = NULL;
        JS_ConvertArguments(cx, argc, JS_ARGV(cx, vp), "S", &string);
        if (string) {
            JSStringWrapper wrapper(string);
            cocos2d::log("%s", wrapper.get());
        }
    }
    return true;
}


bool JSRand(JSContext* cx, uint32_t argc, jsval *vp)
{
	jsval out = INT_TO_JSVAL(RANDOM_MGR->getRand());
	JS_SET_RVAL(cx, vp, out);
    return true;
}


bool JSPlayAnimation(JSContext* cx, uint32_t argc, jsval *vp)
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
        
        return true;
    }
    
    return false;
}



bool CFBFunctionsJS::init()
{
    auto sc = ScriptingCore::getInstance();
    cocos2d::ScriptEngineManager::getInstance()->setScriptEngine(sc);
    do
    {
        sc->addRegisterCallback(register_all_CFBCardJSBinds);
        sc->start();
        
        bool ret;
        ret = sc->runScript("CompactFunctions.js");
        BREAK_IF(ret == false);
        
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



jsval CFBFunctionsJS::callJSFunc(const char* name, const char* format, ...)
{
    int count = (int)strlen(format);
    jsval* argv = new jsval[count];
    va_list insvalist;
    va_start(insvalist, format);
    for (int i = 0; i < count; ++i)
    {
        switch (format[i])
        {
            case 'j': // jsval
                argv[i] = va_arg(insvalist, jsval);
                break;
            case 'b':   // bool
                argv[i] = BOOLEAN_TO_JSVAL(va_arg(insvalist, int));
                break;
            case 'd':   // int
                argv[i] = INT_TO_JSVAL(va_arg(insvalist, int));
                break;
            case 'f':   //float
                argv[i] = DOUBLE_TO_JSVAL(va_arg(insvalist, double));
                break;
            case 's':   // c style string
                argv[i] = c_string_to_jsval(_cx, va_arg(insvalist, const char*));
                break;
            default:
                CC_ASSERT(false);
        }
    }
    va_end(insvalist);
    
    
    jsval res;
    JSAutoCompartment ac(_cx, _go);
    JS_CallFunctionName(_cx, _go, name, count, argv, &res);
    
    auto sc = ScriptingCore::getInstance();
    sc->forceGC(_cx, 0, nullptr);
    return res ;
}



float CFBFunctionsJS::getSpeed(const CFBCard& co)
{
    auto res = callJSFunc("GetSpeed", "j", createJsvalFromCard(co));
    if (JSVAL_IS_DOUBLE(res))
    {
        return (float)JSVAL_TO_DOUBLE(res);
    }
    
    return 0.f;
}



void CFBFunctionsJS::startPassBall(const CFBCard& co1, bool isAir)
{
    callJSFunc("StartPassBall", "jb", createJsvalFromCard(co1), isAir);
}



bool CFBFunctionsJS::tackleBall(const CFBCard& co1, const CFBCard& co2)
{
    auto res = callJSFunc("TackleBall", "jj", createJsvalFromCard(co1), createJsvalFromCard(co2));
    if (JSVAL_TO_BOOLEAN(res))
    {
        return true == JSVAL_TO_BOOLEAN(res);
    }
    
    return false;
}



bool CFBFunctionsJS::interceptBall(const CFBCard& co1, const CFBCard& co2)
{
    auto res = callJSFunc("InterceptBall", "jj", createJsvalFromCard(co1), createJsvalFromCard(co2));
    if (JSVAL_TO_BOOLEAN(res))
    {
        return true == JSVAL_TO_BOOLEAN(res);
    }
    
    return false;
}



bool CFBFunctionsJS::blockBall(const CFBCard& co1, const CFBCard& co2)
{
    auto res = callJSFunc("BlockBall", "jj", createJsvalFromCard(co1), createJsvalFromCard(co2));
    if (JSVAL_TO_BOOLEAN(res))
    {
        return true == JSVAL_TO_BOOLEAN(res);
    }
    
    return false;
}



void CFBFunctionsJS::receiveBall(const CFBCard& co1)
{
    callJSFunc("ReceiveBall", "j", createJsvalFromCard(co1));
}



