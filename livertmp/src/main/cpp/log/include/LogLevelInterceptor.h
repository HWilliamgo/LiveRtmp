//
// Created by HWilliam on 2021/7/6.
//

#include "LogDecor.h"

#ifndef LIVERTMP_LOGLEVELINTERCEPTOR_H
#define LIVERTMP_LOGLEVELINTERCEPTOR_H

class LogLevelInterceptor : public LogDecor {
public:
    LogLevelInterceptor(Log *target);

private:
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

    virtual ~LogLevelInterceptor() override;

};

#endif //LIVERTMP_LOGLEVELINTERCEPTOR_H
