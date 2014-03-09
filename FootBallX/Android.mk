LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := footballx_static

LOCAL_MODULE_FILENAME := libfootballxcommon

CPP_FILE_LIST := $(wildcard $(LOCAL_PATH)/Classes/*.cpp) \
				$(wildcard $(LOCAL_PATH)/JSBinds/*.cpp)

LOCAL_SRC_FILES := $(CPP_FILE_LIST:$(LOCAL_PATH)/%=%)										

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../cocos2d-x/cocos \
					$(LOCAL_PATH)/../cocos2d-x/cocos/extensions \
					$(LOCAL_PATH)/Classes \
					$(LOCAL_PATH)/JSBinds

#LOCAL_CFLAGS := -I/$(LOCAL_PATH)/proj.android/jni/footballx/Classes
#LOCAL_CFLAGS += -DUSE_FILE32API -DNDEBUG
#LOCAL_EXPORT_CFLAGS := -DUSE_FILE32API -DNDEBUG

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
LOCAL_WHOLE_STATIC_LIBRARIES += pomelo_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_jsb_static

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Classes \
							$(LOCAL_PATH)/JSBinds
            
include $(BUILD_STATIC_LIBRARY)

$(call import-module,2d)
$(call import-module,extensions)
$(call import-module,audio/android)
$(call import-module,editor-support/cocosbuilder)
$(call import-module,libpomelo)
$(call import-module,scripting/javascript/bindings)