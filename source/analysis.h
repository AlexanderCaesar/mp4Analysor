#ifndef XIAOC_ANALYSIS_H
#define  XIAOC_MP4_ANALYSIS_H
//����  �����  20171214  xiaoc@pku.edu.cn
#include "mp4.h"
#include "mp4file.h"

class Anlysis
{
    mp4_param* m_param;         //���ò�����Ϣ

    int        m_boxConter;     //ͳ��box�ĸ���    

public:
    Anlysis();
    void create(mp4_param *param);/* �����ڴ� */
    void destory();/* �ͷ��ڴ� */

    /* ����mp4�ļ� */
    void anlysis();
};
#endif