#ifndef LIVERTMP_HW_UTIL
#define LIVERTMP_HW_UTIL

//计算数组长度
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

namespace HWUtils {

    /**
     * h264码流的nalu类型
     */
    enum nal_unit_type_e {
        NAL_UNKNOWN = 0,
        NAL_SLICE = 1,
        NAL_SLICE_DPA = 2,
        NAL_SLICE_DPB = 3,
        NAL_SLICE_DPC = 4,
        NAL_SLICE_IDR = 5,  /* 0x65 IDR-instantaneous decoding refresh (IDR)picture ; */
        NAL_SEI = 6,
        NAL_SPS = 7,        /* 0x67 */
        NAL_PPS = 8,        /* 0x68 */
        NAL_AUD = 9,
        NAL_FILLER = 12,
    };

    /**
     * 从带路径的文件名中获取文件名
     * @param input 输入
     * @return      输出（需要外部管理内存）
     */
    char *getFileNameFromPath(const char *input);

    /**
     * 解析码流中的16进制的nalu类型的值
     * @param hexNaluValue 16进制nalu类型
     * @return nal_unit_type_e
     */
    int parseHexNaluType(int hexNaluValue);
}

#endif //LIVERTMP_HW_UTIL