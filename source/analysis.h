#ifndef XIAOC_ANALYSIS_H
#define  XIAOC_MP4_ANALYSIS_H
//作者  蔡砚刚  20171214  xiaoc@pku.edu.cn
#include "mp4.h"
#include "mp4file.h"

class Anlysis
{
    mp4_param* m_param;         //配置参数信息

    int        m_boxConter;     //统计box的个数    

public:
    Anlysis();
    void create(mp4_param *param);/* 申请内存 */
    void destory();/* 释放内存 */

    /* 分析mp4文件 */
    void anlysis();
};
#endif