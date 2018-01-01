//作者  蔡砚刚  20171214  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "mp4file.h"

#pragma warning(disable: 4996) // POSIX setmode and fileno deprecated

FILE *g_mp4_file  = NULL;    //MP4 原始文件
FILE *g_mp4_log = NULL;      //打印每个box的详细信息
FILE *g_mp4_video = NULL;    //输出视频流信息
FILE *g_mp4_audio = NULL;    //输出音频流信息
FILE *g_mp4_out = NULL;      //输出修改后的mp4流

char *mp4_file_name = NULL;  //mp4原始文件            -i 配置 如：-i brazil-bq.mp4
char *mp4_log_name = NULL;   //打印每个box的详细信息  -t 配置 如：-t brazil-bq.log
char *mp4_video_name = NULL; //输出视频流信息         -v 配置 如：-v brazil-bq.264 
char *mp4_audio_name = NULL; //输出音频流信息         -a 配置 如：-a brazil-bq.aac 
char *mp4_out_name = NULL; //输出修改后的mp4流      -o 配置 如：-o brazil-new.mp4

int  g_errors        = 0;    //统计错误个数

Mp4Info mp4Info;

void initMp4Info()
{
	mp4Info.timescale= 1;
	mp4Info.current_track_ID = 0;
	mp4Info.audio_track_ID = 0;   //audio的track_ID
	mp4Info.video_track_ID = 0;   //video的track_ID

	for (int i = 0; i < MAX_TRACKINFOR_NUM; i++)
	{
		mp4Info.trackInfor[i].track_ID = 0;
	}
}

//打开mp4相关文件
void openMp4Files()    
{
	if (mp4_file_name)
    {
        g_mp4_file = fopen(mp4_file_name,"rb");
        if(!g_mp4_file)
        {
            printf("打开文件失败: %s\n",mp4_file_name);
            system("pause");
            exit(0);
        }
    }
    if(mp4_log_name)
    {
        g_mp4_log = fopen(mp4_log_name,"w");
        if(!g_mp4_log)
        {
            printf("打开文件失败: %s\n",mp4_log_name);
            system("pause");
            exit(0);
        }
    }

    if(mp4_video_name)
    {
        g_mp4_video = fopen(mp4_video_name,"wb");
        if(!g_mp4_video)
        {
            printf("打开文件失败: %s\n",mp4_video_name);
            system("pause");
            exit(0);
        }
    }

    if(mp4_audio_name)
    {
        g_mp4_audio = fopen(mp4_audio_name,"wb");
        if(!g_mp4_audio)
        {
            printf("打开文件失败: %s\n",mp4_audio_name);
            system("pause");
            exit(0);
        }
    }

    if(mp4_out_name)
    {
        g_mp4_out = fopen(mp4_out_name,"wb");
        if(!g_mp4_out)
        {
            printf("打开文件失败: %s\n",mp4_out_name);
            system("pause");
            exit(0);
        }
    }
}

//关闭mp4相关文件
void closeMp4Files()
{
    if(g_mp4_file)
        fclose(g_mp4_file);
    if(g_mp4_log)
        fclose(g_mp4_log);
    if(g_mp4_video)
        fclose(g_mp4_video);
    if(g_mp4_audio)
        fclose(g_mp4_audio);
    if(g_mp4_out)
        fclose(g_mp4_out);
}