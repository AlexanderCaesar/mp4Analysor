//����  �����  20171214  xiaoc@pku.edu.cn
//ISO / IEC 14496 - 12
#include <stdlib.h>
#include "mp4.h"
#include "mp4file.h"
#include "analysis.h"

#pragma warning(disable:4996)
#include <time.h>
#include <string.h>
int main(int argc, char **argv)
{
    bool ret = false;

    Anlysis *anlysis = new Anlysis();

    mp4_param_default(&m_param);//����Ĭ�ϲ���
    ret = parse(&m_param,argc,argv);  //��������
    if(!ret) //�������ô���
    {
        system("pause");
        return 0;
    }

	initMp4Info();
	openMp4Files();//����ض�д�ļ�

    anlysis->create(&m_param); //�����ڴ�
    anlysis->anlysis();//����mp4�ļ�
    anlysis->destory();//�ͷ��ڴ�

    delete anlysis;
    closeMp4Files();//�ر���ض�д�ļ�
    system("pause");
    return 0;
}