#ifndef __CFBCardJSBinds_h__
#define __CFBCardJSBinds_h__

#include "jsapi.h"
#include "jsfriendapi.h"


extern JSClass  *jsb_CFBCard_class;
extern JSObject *jsb_CFBCard_prototype;

JSBool js_CFBCardJSBinds_CFBCard_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_CFBCardJSBinds_CFBCard_finalize(JSContext *cx, JSObject *obj);
void js_register_CFBCardJSBinds_CFBCard(JSContext *cx, JSObject *global);
void register_all_CFBCardJSBinds(JSContext* cx, JSObject* obj);
JSBool js_CFBCardJSBinds_CFBCard_CFBCard(JSContext *cx, uint32_t argc, jsval *vp);
#endif

