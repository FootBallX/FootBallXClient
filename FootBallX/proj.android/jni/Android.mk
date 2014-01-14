LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := footballx_shared

LOCAL_MODULE_FILENAME := libfootballx

#CPP_FILE_LIST := $(wildcard $(LOCAL_PATH)../../Classes/*.cpp)
                   
LOCAL_SRC_FILES := footballx/main.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := footballx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += pomelo_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,FootBallX)
$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,libpomelo)
