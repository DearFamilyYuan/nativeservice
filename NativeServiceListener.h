/* Copyright (C) 2016 Tcl Corporation Limited */
#ifndef _NATIVESERVICESOCKETLISTENER_H
#define _NATIVESERVICESOCKETLISTENER_H

#include "SocketListener.h"

class SocketClient;

class NativeServiceListener : public SocketListener {
public:
    static const int CMD_ARGS_MAX = 26;
    static const int CMD_BUF_SIZE = 1024;

    /* 1 out of errorRate will be dropped */
    int errorRate;


protected:
    bool onDataAvailable(SocketClient *c);

public:
    NativeServiceListener(const char *socketName);
    NativeServiceListener(const char *socketName, bool withSeq);
    NativeServiceListener(int sock);
    virtual ~NativeServiceListener() {}

private:

    void init(const char *socketName, bool withSeq);
};
#endif
