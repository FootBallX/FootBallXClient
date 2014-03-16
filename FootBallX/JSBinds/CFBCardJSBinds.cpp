#include "CFBCardJSBinds.hpp"
#include "cocos2d_specifics.hpp"
#include "CFBCardManager.h"

enum CARD_PROP
{
    ID,
    TYPE,
    STR,
    SPD,
    DRB,
    PAS,
    SHT,
    DEF,
    TAK,
    HIT,
    ATK,
    GRD,
    AIR,
};


static JSBool
card_getProperty(JSContext *cx, JS::Handle<JSObject*> obj, JS::Handle<jsid> id, JS::MutableHandle<JS::Value> vp)
{
    jsval property;
    JS_IdToValue(cx, id, &property);
    {
        auto proxy = jsb_get_js_proxy(obj);
        CFBCard* ptr = (CFBCard*)proxy->ptr;
        switch (JSVAL_TO_INT(property))
        {
            case CARD_PROP::ID: vp.set(std_string_to_jsval(cx, ptr->m_cardID)); break;
            case CARD_PROP::TYPE: vp.set(int32_to_jsval(cx, ptr->m_type)); break;
            case CARD_PROP::STR: vp.set(DOUBLE_TO_JSVAL(ptr->m_strength)); break;
            case CARD_PROP::SPD: vp.set(DOUBLE_TO_JSVAL(ptr->m_speed)); break;
            case CARD_PROP::DRB: vp.set(DOUBLE_TO_JSVAL(ptr->m_dribbleSkill)); break;
            case CARD_PROP::PAS: vp.set(DOUBLE_TO_JSVAL(ptr->m_passSkill)); break;
            case CARD_PROP::SHT: vp.set(DOUBLE_TO_JSVAL(ptr->m_shootSkill)); break;
            case CARD_PROP::DEF: vp.set(DOUBLE_TO_JSVAL(ptr->m_defenceSkill)); break;
            case CARD_PROP::TAK: vp.set(DOUBLE_TO_JSVAL(ptr->m_takeBallSkill)); break;
            case CARD_PROP::HIT: vp.set(DOUBLE_TO_JSVAL(ptr->m_takeBallSkill)); break;
            case CARD_PROP::ATK: vp.set(DOUBLE_TO_JSVAL(ptr->m_attackSkill)); break;
            case CARD_PROP::GRD: vp.set(DOUBLE_TO_JSVAL(ptr->m_groundSkill)); break;
            case CARD_PROP::AIR: vp.set(DOUBLE_TO_JSVAL(ptr->m_airSkill)); break;
        }
    }
    return JS_TRUE;
}



template<class T>
static JSBool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::RootedValue initializing(cx);
    JSBool isNewValid = JS_TRUE;
	if (isNewValid)
	{
		TypeTest<T> t;
		js_type_class_t *typeClass = nullptr;
		std::string typeName = t.s_name();
		auto typeMapIter = _js_global_type_map.find(typeName);
		CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
		typeClass = typeMapIter->second;
		CCASSERT(typeClass, "The value is null.");

		JSObject *_tmp = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
		T* cobj = new T();
		js_proxy_t *pp = jsb_new_proxy(cobj, _tmp);
		JS_AddObjectRoot(cx, &pp->obj);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(_tmp));
		return JS_TRUE;
	}

    return JS_FALSE;
}

static JSBool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
	return JS_FALSE;
}

JSClass  *jsb_CFBCard_class;
JSObject *jsb_CFBCard_prototype;

JSBool js_CFBCardJSBinds_CFBCard_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	JSBool ok = JS_TRUE;
	if (argc == 13) {
		std::string arg0;
		int arg1;
		double arg2;
		double arg3;
		double arg4;
		double arg5;
		double arg6;
		double arg7;
		double arg8;
		double arg9;
		double arg10;
		double arg11;
		double arg12;
		ok &= jsval_to_std_string(cx, argv[0], &arg0);
		ok &= jsval_to_int32(cx, argv[1], (int32_t *)&arg1);
		ok &= JS_ValueToNumber(cx, argv[2], &arg2);
		ok &= JS_ValueToNumber(cx, argv[3], &arg3);
		ok &= JS_ValueToNumber(cx, argv[4], &arg4);
		ok &= JS_ValueToNumber(cx, argv[5], &arg5);
		ok &= JS_ValueToNumber(cx, argv[6], &arg6);
		ok &= JS_ValueToNumber(cx, argv[7], &arg7);
		ok &= JS_ValueToNumber(cx, argv[8], &arg8);
		ok &= JS_ValueToNumber(cx, argv[9], &arg9);
		ok &= JS_ValueToNumber(cx, argv[10], &arg10);
		ok &= JS_ValueToNumber(cx, argv[11], &arg11);
		ok &= JS_ValueToNumber(cx, argv[12], &arg12);
		JSB_PRECONDITION2(ok, cx, JS_FALSE, "js_CFBCardJSBinds_CFBCard_constructor : Error processing arguments");
		CFBCard* cobj = new CFBCard(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
		TypeTest<CFBCard> t;
		js_type_class_t *typeClass = nullptr;
		std::string typeName = t.s_name();
		auto typeMapIter = _js_global_type_map.find(typeName);
		CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
		typeClass = typeMapIter->second;
		CCASSERT(typeClass, "The value is null.");
		JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
		// link the native object with the javascript object
		js_proxy_t* p = jsb_new_proxy(cobj, obj);
		return JS_TRUE;
	}

	JS_ReportError(cx, "js_CFBCardJSBinds_CFBCard_constructor : wrong number of arguments: %d, was expecting %d", argc, 13);
	return JS_FALSE;
}



void js_CFBCard_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (CFBCard)", obj);
    js_proxy_t* nproxy;
    js_proxy_t* jsproxy;
    jsproxy = jsb_get_js_proxy(obj);
    if (jsproxy) {
        nproxy = jsb_get_native_proxy(jsproxy->ptr);

        CFBCard *nobj = static_cast<CFBCard *>(nproxy->ptr);
        if (nobj)
            delete nobj;
        
        jsb_remove_proxy(nproxy, jsproxy);
    }
}

void js_register_CFBCardJSBinds_CFBCard(JSContext *cx, JSObject *global) {
	jsb_CFBCard_class = (JSClass *)calloc(1, sizeof(JSClass));
	jsb_CFBCard_class->name = "CFBCard";
	jsb_CFBCard_class->addProperty = JS_PropertyStub;
	jsb_CFBCard_class->delProperty = JS_DeletePropertyStub;
	jsb_CFBCard_class->getProperty = card_getProperty;
	jsb_CFBCard_class->setProperty = JS_StrictPropertyStub;
	jsb_CFBCard_class->enumerate = JS_EnumerateStub;
	jsb_CFBCard_class->resolve = JS_ResolveStub;
	jsb_CFBCard_class->convert = JS_ConvertStub;
	jsb_CFBCard_class->finalize = js_CFBCard_finalize;
	jsb_CFBCard_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

	static JSPropertySpec properties[] = {
        {"id", (int8_t)CARD_PROP::ID, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"type", (int8_t)CARD_PROP::TYPE, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"strength", (int8_t)CARD_PROP::STR, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"speed", (int8_t)CARD_PROP::SPD, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"dribble", (int8_t)CARD_PROP::DRB, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"pass", (int8_t)CARD_PROP::PAS, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"shoot", (int8_t)CARD_PROP::SHT, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"defence", (int8_t)CARD_PROP::DEF, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"take", (int8_t)CARD_PROP::TAK, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"hit", (int8_t)CARD_PROP::HIT, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"attack", (int8_t)CARD_PROP::ATK, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"ground", (int8_t)CARD_PROP::GRD, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
        {"air", (int8_t)CARD_PROP::AIR, JSPROP_ENUMERATE, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER},
		{0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
	};

	JSFunctionSpec *funcs = NULL;

	JSFunctionSpec *st_funcs = NULL;

	jsb_CFBCard_prototype = JS_InitClass(
		cx, global,
		NULL, // parent proto
		jsb_CFBCard_class,
		js_CFBCardJSBinds_CFBCard_constructor, 0, // constructor
		properties,
		funcs,
		NULL, // no static properties
		st_funcs);
	// make the class enumerable in the registered namespace
	JSBool found;
	JS_SetPropertyAttributes(cx, global, "CFBCard", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

	// add the proto and JSClass to the type->js info hash table
	TypeTest<CFBCard> t;
	js_type_class_t *p;
	std::string typeName = t.s_name();
	if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
	{
		p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
		p->jsclass = jsb_CFBCard_class;
		p->proto = jsb_CFBCard_prototype;
		p->parentProto = NULL;
		_js_global_type_map.insert(std::make_pair(typeName, p));
	}
}

void register_all_CFBCardJSBinds(JSContext* cx, JSObject* obj) {

	js_register_CFBCardJSBinds_CFBCard(cx, obj);
}

