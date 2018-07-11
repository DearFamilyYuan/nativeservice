//
// Created familyyuan user on 18-4-20.
//

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <cutils/sockets.h>
#include <cutils/log.h>
#include "NativeServiceListener.h"

#include <fcntl.h>
#include <android-base/logging.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <cutils/uevent.h>

#include <sys/ioctl.h>

using namespace android;

#define MAX_EPOLL_EVENTS 40

#define BUFFER_SIZE PIPE_BUF

int main(int argc, char *argv[]) {
    SLOGD("native_service start");
    //
    fcntl(android_get_control_socket("nativeservice"), F_SETFD, FD_CLOEXEC);
    NativeServiceListener *cl;
    cl = new NativeServiceListener("nativeservice", true);
    if (cl->startListener()) {
        SLOGE("native_service Unable to start NativeServiceListener (%s)", strerror(errno));
        exit(1);
    }

    while(1){
        sleep(1000);
    }

    SLOGD("native_service die");
    return 0;
}
