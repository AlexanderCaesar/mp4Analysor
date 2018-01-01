//作者  蔡砚刚  20171214  xiaoc@pku.edu.cn
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

    mp4_param_default(&m_param);//设置默认参数
    ret = parse(&m_param,argc,argv);  //解析参数
    if(!ret) //参数配置错误
    {
        system("pause");
        return 0;
    }

	initMp4Info();
	openMp4Files();//打开相关读写文件

    anlysis->create(&m_param); //申请内存
    anlysis->anlysis();//分析mp4文件
    anlysis->destory();//释放内存

    delete anlysis;
    closeMp4Files();//关闭相关读写文件
    system("pause");
    return 0;
}