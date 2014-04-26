#ifndef __CFBCardJSBinds_h__
#define __CFBCardJSBinds_h__

#ifdef DEBUG
#undef DEBUG
#include "jsapi.h"
#include "jsfriendapi.h"
#define DEBUG   1
#else
#include "jsapi.h"
#include "jsfriendapi.h"
#endif

extern JSClass  *jsb_CFBCard_class;
extern JSObject *jsb_CFBCard_prototype;

bool js_CFBCardJSBinds_CFBCard_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_CFBCardJSBinds_CFBCard_finalize(JSContext *cx, JSObject *obj);
void js_register_CFBCardJSBinds_CFBCard(JSContext *cx, JSObject *global);
void register_all_CFBCardJSBinds(JSContext* cx, JSObject* obj);
bool js_CFBCardJSBinds_CFBCard_CFBCard(JSContext *cx, uint32_t argc, jsval *vp);
#endif

