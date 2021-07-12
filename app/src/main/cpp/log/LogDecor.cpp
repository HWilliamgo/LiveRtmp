//
// Created by HWilliam on 2021/7/5.
//
#include "LogDecor.h"
#include "log_abs.h"

void LogDecor::v(const char *msg) {
    target->v(msg);
}

void LogDecor::d(const char *msg) {
    target->d(msg);
}

void LogDecor::i(const char *msg) {
    target->i(msg);
}

void LogDecor::w(const char *msg) {
    target->w(msg);
}

void LogDecor::e(const char *msg) {
    target->e(msg);
}

void LogDecor::vTag(const char *tag, const char *msg) {
    target->vTag(tag, msg);
}

void LogDecor::dTag(const char *tag, const char *msg) {
    target->dTag(tag, msg);
}

void LogDecor::iTag(const char *tag, const char *msg) {
    target->iTag(tag, msg);
}

void LogDecor::wTag(const char *tag, const char *msg) {
    target->wTag(tag, msg);
}

void LogDecor::eTag(const char *tag, const char *msg) {
    target->eTag(tag, msg);
}

LogDecor::~LogDecor() {
    if (target) {
        delete target;
        target = nullptr;
    }
};

