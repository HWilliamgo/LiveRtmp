#ifndef LIVERTMP_HW_UTIL
#define LIVERTMP_HW_UTIL

//计算数组长度
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

namespace HWUtils {
    /**
     * 从带路径的文件名中获取文件名
     * @param input 输入
     * @return      输出（需要外部管理内存）
     */
    char *getFileNameFromPath(const char *input);
}

#endif //LIVERTMP_HW_UTIL