//
// Created by HWilliam on 2021/7/1.
//

#include "SimpleLog.h"
#include "android/log.h"
#include <cstdarg>

void SimpleLog::v(const char *msg) {
    if (!msg) {
        return;
    }
    const char *tag;
    if (m_tag) {
        tag = m_tag;
    } else {
        tag = DEFAULT_TAG;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, tag, "%s", msg);
}

void SimpleLog::d(const char *msg) {
    if (!msg) {
        return;
    }
    const char *tag;
    if (m_tag) {
        tag = m_tag;
    } else {
        tag = DEFAULT_TAG;
    }
    __android_log_print(ANDROID_LOG_DEBUG, tag, "%s", msg);
}

void SimpleLog::i(const char *msg) {
    if (!msg) {
        return;
    }
    const char *tag;
    if (m_tag) {
        tag = m_tag;
    } else {
        tag = DEFAULT_TAG;
    }
    __android_log_print(ANDROID_LOG_INFO, tag, "%s", msg);
}

void SimpleLog::w(const char *msg) {
    if (!msg) {
        return;
    }
    const char *tag;
    if (m_tag) {
        tag = m_tag;
    } else {
        tag = DEFAULT_TAG;
    }
    __android_log_print(ANDROID_LOG_WARN, tag, "%s", msg);
}

void SimpleLog::e(const char *msg) {
    if (!msg) {
        return;
    }
    const char *tag;
    if (m_tag) {
        tag = m_tag;
    } else {
        tag = DEFAULT_TAG;
    }
    __android_log_print(ANDROID_LOG_ERROR, tag, "%s", msg);
}

void SimpleLog::vTag(const char *tag, const char *msg) {
    if (!msg) {
        return;
    }
    if (!tag) {
        if (m_tag) {
            tag = m_tag;
        } else {
            tag = DEFAULT_TAG;
        }
    }
    __android_log_print(ANDROID_LOG_VERBOSE, tag, "%s", msg);
}

void SimpleLog::dTag(const char *tag, const char *msg) {
    if (!msg) {
        return;
    }
    if (!tag) {
        if (m_tag) {
            tag = m_tag;
        } else {
            tag = DEFAULT_TAG;
        }
    }
    __android_log_print(ANDROID_LOG_DEBUG, tag, "%s", msg);
}

void SimpleLog::iTag(const char *tag, const char *msg) {
    if (!msg) {
        return;
    }
    if (!tag) {
        if (m_tag) {
            tag = m_tag;
        } else {
            tag = DEFAULT_TAG;
        }
    }
    __android_log_print(ANDROID_LOG_INFO, tag, "%s", msg);
}

void SimpleLog::wTag(const char *tag, const char *msg) {
    if (!msg) {
        return;
    }
    if (!tag) {
        if (m_tag) {
            tag = m_tag;
        } else {
            tag = DEFAULT_TAG;
        }
    }
    __android_log_print(ANDROID_LOG_WARN, tag, "%s", msg);
}

void SimpleLog::eTag(const char *tag, const char *msg) {
    if (!msg) {
        return;
    }
    if (!tag) {
        if (m_tag) {
            tag = m_tag;
        } else {
            tag = DEFAULT_TAG;
        }
    }
    __android_log_print(ANDROID_LOG_ERROR, tag, "%s", msg);
}

SimpleLog::~SimpleLog() = default;
