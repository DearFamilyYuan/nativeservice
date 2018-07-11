# Copyright 2013 The Android Open Source Project
# 当前路径
LOCAL_PATH := $(call my-dir)
#清除历史变量
include $(CLEAR_VARS)

### nativeservice ###

#待编译的源码文件
LOCAL_SRC_FILES := \
    NativeServiceListener.cpp \
    native_main.cpp \

common_c_includes := \
    bionic \
    system/core/include/sysutils \
#引用一些函数库
common_shared_libraries := \
    libsysutils \
    libcutils \
    liblog \
    libutils \
    libbinder \
    libbase

LOCAL_C_INCLUDES := \
    $(common_c_includes)

#守护进程的名字
LOCAL_MODULE := nativeservice
LOCAL_CFLAGS := -Wall -Wno-unused-parameter -Werror
LOCAL_SHARED_LIBRARIES := \
    $(common_shared_libraries)
LOCAL_MODULE_TAGS := optional

#编译守护进程，也就是可执行文件
#编译后，在/system/bin/ 下，变多了 nativeservice 可执行文件。
include $(BUILD_EXECUTABLE)
