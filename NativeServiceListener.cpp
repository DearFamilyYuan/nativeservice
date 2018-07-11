/* Copyright (C) 2016 Tcl Corporation Limited */
#define LOG_TAG "NativeServiceListener"

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>

#include <cutils/log.h>
#include <sysutils/SocketClient.h>
#include <cutils/properties.h>

#include "NativeServiceListener.h"

static int file_fd;

#define DUMP_ARGS 1

#define UNUSED __attribute__((unused))

NativeServiceListener::NativeServiceListener(const char *socketName, bool withSeq) :
                            SocketListener(socketName, true, withSeq) {
    init(socketName, withSeq);
}

NativeServiceListener::NativeServiceListener(const char *socketName) :
                            SocketListener(socketName, true, false) {
    init(socketName, false);
}

NativeServiceListener::NativeServiceListener(int sock) :
                            SocketListener(sock, true) {
    init(NULL, false);
}

void NativeServiceListener::init(const char *socketName UNUSED, bool withSeq) {
}

bool NativeServiceListener::onDataAvailable(SocketClient *c) {
    char buffer[CMD_BUF_SIZE] = {0};
    int len;

    len = TEMP_FAILURE_RETRY(read(c->getSocket(), buffer, sizeof(buffer)));
    if (len < 0) {
        SLOGE("native_service read() failed (%s)", strerror(errno));
        return false;
    } else if (!len) {
        SLOGD("native_service socket data %s", buffer);
        return false;
    }
    SLOGD("native_service runnig");

    char buffer_data[20];
    int res = -1;
    file_fd = TEMP_FAILURE_RETRY(open("/proc/cpufreq/MT_CPU_DVFS_LL/cpufreq_freq",O_RDONLY));
    if (file_fd < 0) {
        SLOGD("native_service open failed");
    } else {
        SLOGD("native_service open success");
    }
    if (file_fd != -1){
        //读取管道数据，如果没有数据，阻塞等待数据被写入，激活
        res = read(file_fd, buffer_data, sizeof(buffer_data));
        SLOGD("native_service result=%s", buffer_data);
    } else {
        SLOGD("native_service open failed");
    }

    if(res > 0){
        char *buf;
        int ret = 0;
        strtok(buffer_data, "\n");
        ret = asprintf(&buf, "%s%s%s%s", "{\"code\":200,", "\"cpu\":\"", buffer_data, "\"}");
        SLOGD("native_service for java result=%s", buf);
        if (ret != -1) {
            c->sendMsg(buf);
        } else {
            c->sendMsg(buffer_data);
        }
        free(buf);
    } else {
        SLOGD("native_service open failed %d", 500);
        c->sendCode(500);
    }
    close(file_fd);
    free(buffer);
    free(buffer_data);
    return true;
}

