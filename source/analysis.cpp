//����  �����  20171214  xiaoc@pku.edu.cn
#include <stdlib.h>
#include <string.h>
#include "analysis.h"
#include "box.h"
void author(mp4_param *param)
{
    fprintf(g_mp4_log,"==================���� ��Ϣ=========================\n");
	fprintf(g_mp4_log, "��       ��:    xiaoC/cabbage/Alexander/Caesar\n");
	fprintf(g_mp4_log, "��       ��:    xiaoc@pku.edu.cn\n");
	fprintf(g_mp4_log, "��       ��:    http://blog.csdn.net/cabbage2008\n");
}
Anlysis::Anlysis()
{
    m_param          = NULL;
};
/* �����ڴ� */
void Anlysis::create(mp4_param *param)
{
    m_param = param;
}
/* �ͷ��ڴ� */
void Anlysis::destory()
{
}

/* ����mp4�ļ� */
void Anlysis::anlysis()
{
    author(m_param);
    m_boxConter = 0;   //��ʼ��Ϊ0
    while(1)
    {
		BOX box;

		fprintf(g_mp4_log, "\n\n==================mp4 box %5d=========================\n", m_boxConter);
		if (box.readHeader()<0)
			break; 

		if (box.anlysis()<0)//����box
			break;

        if(m_boxConter%100==0)
        {
            printf("%5d   Boxs �������\n",m_boxConter);
        }
        m_boxConter++;   //����Tag����    
    }
    printf("errors:    %d\n",g_errors);
    printf("warns :    %d\n", g_warning);

    //mp4��Ҫ��Ϣ
    fprintf(g_mp4_log,"\n\n==================mp4 ��Ҫ��Ϣ=========================\n");
    fprintf(g_mp4_log,"mp4 box ����:    %d\n",m_boxConter);
    fprintf(g_mp4_log,"mp4�������:    %d\n",g_errors);
    fprintf(g_mp4_log,"mp4�������:    %d\n", g_warning);
}