//
// Created by HWilliam on 2021/8/7.
//
#include <cstdlib>
#include <cstring>
#include "hwutil.h"

namespace HWUtils {
    char *getFileNameFromPath(const char *input) {
        if (!input) {
            return nullptr;
        }
        int len = strlen(input);
        char sig = '/';
        int lastSigIndex = -1;
        for (int i = len - 1; i >= 0; i--) {
            if (input[i] == sig) {
                lastSigIndex = i;
                break;
            }
        }
        if (lastSigIndex == -1) {
            return nullptr;
        }
        int retLen = len - lastSigIndex;
        char *ret = (char *) calloc(retLen + 1, sizeof(char));
        strncpy(ret, input + lastSigIndex + 1, retLen);
        return ret;
    }

    int parseHexNaluType(int hexNaluValue) {
        int flag = 11100000;
        return hexNaluValue &= ~flag;
    }
}
