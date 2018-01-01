//作者  蔡砚刚  20171214  xiaoc@pku.edu.cn

#include <stdio.h>
#include <stdlib.h>
#include "mp4file.h"
#include "mp4.h"

mp4_param m_param; //配置参数信息

/* 打印help信息 */
static void showHelp()
{
    #define H0 printf
    H0("\n-h 打印参数配置信息并退出\n");
    H0("\n-i 配置 如：-i brazil-bq.mp4\n");
    H0("\n-t 配置 如：-t brazil-bq.log\n");
    H0("\n-v 配置 如：-v brazil-bq.264\n");
    H0("\n-a 配置 如：-a brazil-bq.aac\n");
    H0("\n-o 配置 如：-o brazil-new.mp4\n");
}
/* 设置mp4的默认参数 */
void mp4_param_default(mp4_param *param)
{
    param->b_video = 0;
    param->b_audio = 0;    
    param->b_n_mp4 = 0;
}

/* 解析配置参数 */
bool parse(mp4_param *param,int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("没有任何配置参数\n");
        return false;
    }
    
    for(int i = 1; i < argc; i += 2)
    {
        if(argv[i][0] != '-')
        {
            printf("参数配置有误:%s\n",argv[i]);
            return false;
        }
        switch(argv[i][1])
        {
        case 'h':showHelp();system("pause\n");exit(0);          break;
        case 'i':mp4_file_name = argv[i+1];                     break;
		case 't':mp4_log_name = argv[i + 1];                    break;
        case 'v':mp4_video_name = argv[i+1];param->b_video = 1; break;
        case 'a':mp4_audio_name = argv[i+1];param->b_audio = 1; break;
        case 'o':mp4_out_name = argv[i+1];  param->b_n_mp4  = 1; break;
        default:printf("参数配置有误:%s\n",argv[i]); return false;
        }
    }
    return true;
}
