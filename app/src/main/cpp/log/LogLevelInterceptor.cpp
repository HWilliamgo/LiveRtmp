//
// Created by HWilliam on 2021/7/6.
//

#include "LogLevelInterceptor.h"


void LogLevelInterceptor::v(const char *msg) {
    if (logLevel == LogLevel::LOG_LEVEL_VERBOSE) {
        LogDecor::v(msg);
    }
}

void LogLevelInterceptor::d(const char *msg) {
    if (logLevel != LogLevel::LOG_LEVEL_NO && logLevel <= LogLevel::LOG_LEVEL_DEBUG) {
        LogDecor::d(msg);
    }
}

void LogLevelInterceptor::i(const char *msg) {
    if (logLevel != LogLevel::LOG_LEVEL_NO && logLevel <= LogLevel::LOG_LEVEL_INFO) {
        LogDecor::i(msg);
    }
}

void LogLevelInterceptor::w(const char *msg) {
    if (logLevel != LogLevel::LOG_LEVEL_NO && logLevel <= LogLevel::LOG_LEVEL_WARN) {
        LogDecor::w(msg);
    }
}

void LogLevelInterceptor::e(const char *msg) {
    if (logLevel != LogLevel::LOG_LEVEL_NO && logLevel <= LogLevel::LOG_LEVEL_ERROR) {
        LogDecor::e(msg);
    }
}

void LogLevelInterceptor::vTag(const char *tag, const char *msg) {
    if (logLevel != LogLevel::LOG_LEVEL_NO && logLevel <= LogLevel::LOG_LEVEL_VERBOSE) {
        LogDecor::vTag(tag, msg);
    }
}

void LogLevelInterceptor::dTag(const char *tag, const char *msg) {
    if (logLevel == LogLevel::LOG_LEVEL_VERBOSE) {
        LogDecor::dTag(tag, msg);
    }
}

void LogLevelInterceptor::iTag(const char *tag, const char *msg) {
    if (logLevel != LogLevel::LOG_LEVEL_NO && logLevel <= LogLevel::LOG_LEVEL_INFO) {
        LogDecor::iTag(tag, msg);
    }
}

void LogLevelInterceptor::wTag(const char *tag, const char *msg) {
    if (logLevel != LogLevel::LOG_LEVEL_NO && logLevel <= LogLevel::LOG_LEVEL_WARN) {
        LogDecor::wTag(tag, msg);
    }
}

void LogLevelInterceptor::eTag(const char *tag, const char *msg) {
    if (logLevel != LogLevel::LOG_LEVEL_NO && logLevel <= LogLevel::LOG_LEVEL_ERROR) {
        LogDecor::eTag(tag, msg);
    }
}

LogLevelInterceptor::LogLevelInterceptor(Log *target) : LogDecor(target) {
}

LogLevelInterceptor::~LogLevelInterceptor() = default;
