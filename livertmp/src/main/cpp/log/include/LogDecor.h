//
// Created by HWilliam on 2021/7/5.
//

#include "log_abs.h"

#ifndef LIVERTMP_LOG_DECOR_H
#define LIVERTMP_LOG_DECOR_H

class LogDecor : public Log {
protected:
    Log *target;
public:
    LogDecor(Log *target) : target(target) {}

    void v(const char *msg) override;

    void d(const char *msg) override;

    void i(const char *msg) override;

    void w(const char *msg) override;

    void e(const char *msg) override;

    void vTag(const char *tag, const char *msg) override;

    void dTag(const char *tag, const char *msg) override;

    void iTag(const char *tag, const char *msg) override;

    void wTag(const char *tag, const char *msg) override;

    void eTag(const char *tag, const char *msg) override;

    ~LogDecor();
};

#endif //LIVERTMP_LOG_DECOR_H
