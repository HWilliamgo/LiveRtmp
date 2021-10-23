//
// Created by HWilliam on 2021/7/2.
//
#include "include/log_abs.h"
#include "SimpleLog.h"
#include "LogLevelInterceptor.h"

namespace MyLog {
    Log *log = nullptr;

    void init_log(LogType logType, const char *tag) {
        switch (logType) {
            case LogType::SimpleLog:
                Log *simpleLog = new SimpleLog();
                Log *levelInterceptor = new LogLevelInterceptor(simpleLog);
                log = levelInterceptor;
                break;
        }
        MyLog::log->setTag(tag);
    }

    void destroy_log() {
        if (MyLog::log) {
            delete MyLog::log;
            MyLog::log = nullptr;
        }
    }

    void v(const char *msg, ...) {
        if (!log) {
            return;
        }
        if (!msg) {
            return;
        }
        va_list args;
        va_start(args, msg);
        char *msg_to_print = nullptr;
        vasprintf(&msg_to_print, msg, args);
        va_end(args);

        log->v(msg_to_print);
        delete msg_to_print;
    }

    void d(const char *msg, ...) {
        if (!log) {
            return;
        }
        if (!msg) {
            return;
        }
        va_list args;
        va_start(args, msg);
        char *msg_to_print = nullptr;
        vasprintf(&msg_to_print, msg, args);
        va_end(args);

        log->d(msg_to_print);
        delete msg_to_print;
    }

    void i(const char *msg, ...) {
        if (!log) {
            return;
        }
        if (!msg) {
            return;
        }
        va_list args;
        va_start(args, msg);
        char *msg_to_print = nullptr;
        vasprintf(&msg_to_print, msg, args);
        va_end(args);

        log->i(msg_to_print);
        delete msg_to_print;
    }

    void w(const char *msg, ...) {
        if (!log) {
            return;
        }
        if (!msg) {
            return;
        }
        va_list args;
        va_start(args, msg);
        char *msg_to_print = nullptr;
        vasprintf(&msg_to_print, msg, args);
        va_end(args);

        log->w(msg_to_print);
        delete msg_to_print;
    }

    void e(const char *msg, ...) {
        if (!log) {
            return;
        }
        if (!msg) {
            return;
        }
        va_list args;
        va_start(args, msg);
        char *msg_to_print = nullptr;
        vasprintf(&msg_to_print, msg, args);
        va_end(args);

        log->e(msg_to_print);
        delete msg_to_print;
    }

    void vTag(const char *tag, const char *msg, ...) {
        if (!log) {
            return;
        }
        if (!msg) {
            return;
        }
        va_list args;
        va_start(args, msg);
        char *msg_to_print = nullptr;
        vasprintf(&msg_to_print, msg, args);
        va_end(args);

        log->vTag(tag, msg_to_print);
        delete msg_to_print;
    }

    void dTag(const char *tag, const char *msg, ...) {
        if (!log) {
            return;
        }
        if (!msg) {
            return;
        }
        va_list args;
        va_start(args, msg);
        char *msg_to_print = nullptr;
        vasprintf(&msg_to_print, msg, args);
        va_end(args);

        log->dTag(tag, msg_to_print);
        delete msg_to_print;
    }

    void iTag(const char *tag, const char *msg, ...) {
        if (!log) {
            return;
        }
        if (!msg) {
            return;
        }
        va_list args;
        va_start(args, msg);
        char *msg_to_print = nullptr;
        vasprintf(&msg_to_print, msg, args);
        va_end(args);

        log->iTag(tag, msg_to_print);
        delete msg_to_print;
    }

    void wTag(const char *tag, const char *msg, ...) {
        if (!log) {
            return;
        }
        if (!msg) {
            return;
        }
        va_list args;
        va_start(args, msg);
        char *msg_to_print = nullptr;
        vasprintf(&msg_to_print, msg, args);
        va_end(args);

        log->wTag(tag, msg_to_print);
        delete msg_to_print;
    }

    void eTag(const char *tag, const char *msg, ...) {
        if (!log) {
            return;
        }
        if (!msg) {
            return;
        }
        va_list args;
        va_start(args, msg);
        char *msg_to_print = nullptr;
        vasprintf(&msg_to_print, msg, args);
        va_end(args);

        log->eTag(tag, msg_to_print);
        delete msg_to_print;
    }
}
