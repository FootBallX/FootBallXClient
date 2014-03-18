#include "CFBCardJSBinds.hpp"
#include "cocos2d_specifics.hpp"
#include "CFBCard.h"

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


static bool
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
    return true;
}


template<class T>
static bool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::RootedValue initializing(cx);
    bool isNewValid = true;
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
		return true;
	}

    return false;
}

static bool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
	return false;
}

//static bool js_is_native_obj(JSContext *cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
//{
//	vp.set(BOOLEAN_TO_JSVAL(true));
//	return true;	
//}
JSClass  *jsb_CFBCard_class;
JSObject *jsb_CFBCard_prototype;

bool js_CFBCardJSBinds_CFBCard_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
	jsval *argv = JS_ARGV(cx, vp);
	bool ok = true;

	JSObject *obj = NULL;
	CFBCard* cobj = NULL;
	do {
		if (argc == 13) {
			std::string arg0;
			ok &= jsval_to_std_string(cx, argv[0], &arg0);
			if (!ok) { ok = true; break; }
			int arg1;
			ok &= jsval_to_int32(cx, argv[1], (int32_t *)&arg1);
			if (!ok) { ok = true; break; }
			double arg2;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[2]), &arg2);
			if (!ok) { ok = true; break; }
			double arg3;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[3]), &arg3);
			if (!ok) { ok = true; break; }
			double arg4;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[4]), &arg4);
			if (!ok) { ok = true; break; }
			double arg5;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[5]), &arg5);
			if (!ok) { ok = true; break; }
			double arg6;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[6]), &arg6);
			if (!ok) { ok = true; break; }
			double arg7;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[7]), &arg7);
			if (!ok) { ok = true; break; }
			double arg8;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[8]), &arg8);
			if (!ok) { ok = true; break; }
			double arg9;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[9]), &arg9);
			if (!ok) { ok = true; break; }
			double arg10;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[10]), &arg10);
			if (!ok) { ok = true; break; }
			double arg11;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[11]), &arg11);
			if (!ok) { ok = true; break; }
			double arg12;
			ok &= JS::ToNumber( cx, JS::RootedValue(cx, argv[12]), &arg12);
			if (!ok) { ok = true; break; }
			cobj = new CFBCard(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
			TypeTest<CFBCard> t;
			js_type_class_t *typeClass = nullptr;
			std::string typeName = t.s_name();
			auto typeMapIter = _js_global_type_map.find(typeName);
			CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
			typeClass = typeMapIter->second;
			CCASSERT(typeClass, "The value is null.");
			obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
			js_proxy_t* p = jsb_new_proxy(cobj, obj);
		}
	} while(0);

	do {
		if (argc == 0) {
			cobj = new CFBCard();
			TypeTest<CFBCard> t;
			js_type_class_t *typeClass = nullptr;
			std::string typeName = t.s_name();
			auto typeMapIter = _js_global_type_map.find(typeName);
			CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
			typeClass = typeMapIter->second;
			CCASSERT(typeClass, "The value is null.");
			obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
			js_proxy_t* p = jsb_new_proxy(cobj, obj);
		}
	} while(0);

	if (cobj) {
		JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
		return true;
	}
	JS_ReportError(cx, "js_CFBCardJSBinds_CFBCard_constructor : wrong number of arguments");
	return false;
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
//		{"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
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

	static JSFunctionSpec funcs[] = {
        JS_FS_END
	};

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
//	bool found;
//FIXME: Removed in Firefox v27	
//	JS_SetPropertyAttributes(cx, global, "CFBCard", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

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

