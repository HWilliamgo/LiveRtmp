//
// Created by HWilliam on 2021/7/1.
//

#ifndef LIVERTMP_LOG_ABS_H
#define LIVERTMP_LOG_ABS_H

#include "string.h"
#include "stdlib.h"

/**
 * 日志等级
 */
enum class LogLevel : uint8_t {
    /** no log */
    LOG_LEVEL_NO = 0,
    /** Verbose logging. Should typically be disabled for a release apk. */
    LOG_LEVEL_VERBOSE = 1,
    /** LEVEL_Debug logging. Should typically be disabled for a release apk. */
    LOG_LEVEL_DEBUG = 2,
    /** LEVEL_Informational logging. Should typically be disabled for a release apk. */
    LOG_LEVEL_INFO = 3,
    /** LEVEL_Warning logging. For use with recoverable failures. */
    LOG_LEVEL_WARN = 4,
    /** LEVEL_Error logging. For use with unrecoverable failures. */
    LOG_LEVEL_ERROR = 5,
    /** LEVEL_Fatal logging. For use when aborting. */
    LOG_LEVEL_FATAL = 6,
};

/**
 * Log抽象类
 */
class Log {
protected:
    char *m_tag = nullptr;
    LogLevel logLevel = LogLevel::LOG_LEVEL_VERBOSE;
public:
    void setTag(const char *tag) {
        if (!tag) {
            return;
        }
        if (m_tag) {
            free(m_tag);
            m_tag = nullptr;
        }
        size_t len = strlen(tag);
        m_tag = static_cast<char *>(calloc(1, len + 1));
        memcpy(m_tag, tag, len);
    }

    virtual void v(const char *msg) = 0;

    virtual void d(const char *msg) = 0;

    virtual void i(const char *msg) = 0;

    virtual void w(const char *msg) = 0;

    virtual void e(const char *msg) = 0;

    virtual void vTag(const char *tag, const char *msg) = 0;

    virtual void dTag(const char *tag, const char *msg) = 0;

    virtual void iTag(const char *tag, const char *msg) = 0;

    virtual void wTag(const char *tag, const char *msg) = 0;

    virtual void eTag(const char *tag, const char *msg) = 0;

    virtual ~Log() {
        if (!m_tag) {
            free(m_tag);
            m_tag = nullptr;
        }
    }
};

/**
 * 日志类型
 */
enum class LogType : uint8_t {
    SimpleLog
};

// <editor-fold defaultstate="collapsed" desc="Log全局操作管理">

namespace MyLog {
    /**
    * 初始化日志
    * @param logType 日志类型
    * @param tag  TAG
    */
    void init_log(LogType logType, const char *tag);

    /**
     * 销毁日志
     */
    void destroy_log();

    extern Log *log;

    void v(const char *msg, ...);

    void d(const char *msg, ...);

    void i(const char *msg, ...);

    void w(const char *msg, ...);

    void e(const char *msg, ...);

    void vTag(const char *tag, const char *msg, ...);
    void dTag(const char *tag, const char *msg, ...);
    void iTag(const char *tag, const char *msg, ...);
    void wTag(const char *tag, const char *msg, ...);
    void eTag(const char *tag, const char *msg, ...);
}
// </editor-fold>


#endif //LIVERTMP_LOG_ABS_H
