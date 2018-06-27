//作者  蔡砚刚  20171214  xiaoc@pku.edu.cn
#include <stdlib.h>
#include <string.h>
#include "analysis.h"
#include "box.h"
void author(mp4_param *param)
{
    fprintf(g_mp4_log,"==================作者 信息=========================\n");
	fprintf(g_mp4_log, "网       名:    xiaoC/cabbage/Alexander/Caesar\n");
	fprintf(g_mp4_log, "邮       箱:    xiaoc@pku.edu.cn\n");
	fprintf(g_mp4_log, "博       客:    http://blog.csdn.net/cabbage2008\n");
}
Anlysis::Anlysis()
{
    m_param          = NULL;
};
/* 申请内存 */
void Anlysis::create(mp4_param *param)
{
    m_param = param;
}
/* 释放内存 */
void Anlysis::destory()
{
}

/* 分析mp4文件 */
void Anlysis::anlysis()
{
    author(m_param);
    m_boxConter = 0;   //初始化为0
    while(1)
    {
		BOX box;

		fprintf(g_mp4_log, "\n\n==================mp4 box %5d=========================\n", m_boxConter);
		if (box.readHeader()<0)
			break; 

		if (box.anlysis()<0)//分析box
			break;

        if(m_boxConter%100==0)
        {
            printf("%5d   Boxs 分析完毕\n",m_boxConter);
        }
        m_boxConter++;   //计数Tag个数    
    }
    printf("errors:    %d\n",g_errors);
    printf("warns :    %d\n", g_warning);

    //mp4概要信息
    fprintf(g_mp4_log,"\n\n==================mp4 概要信息=========================\n");
    fprintf(g_mp4_log,"mp4 box 个数:    %d\n",m_boxConter);
    fprintf(g_mp4_log,"mp4错误个数:    %d\n",g_errors);
    fprintf(g_mp4_log,"mp4警告个数:    %d\n", g_warning);
}