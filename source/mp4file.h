#ifndef  XIAOC_MP4_FILE_H
#define  XIAOC_MP4_FILE_H
//作者  蔡砚刚  20171214  xiaoc@pku.edu.cn

#include <stdio.h>
#include "mp4.h"


extern FILE *g_mp4_file;  //MP4 原始文件
extern FILE *g_mp4_log;   //打印每个box的详细信息
extern FILE *g_mp4_video; //输出视频流信息
extern FILE *g_mp4_audio; //输出音频流信息
extern FILE *g_mp4_out;   //输出修改后的mp4流

extern char *mp4_file_name;  //mp4 原始文件           -i 配置 如：-i brazil-bq.mp4
extern char *mp4_log_name;   //打印每个tag的详细信息  -t 配置 如：-t brazil-bq.log
extern char *mp4_video_name; //输出视频流信息         -v 配置 如：-v brazil-bq.264 
extern char *mp4_audio_name; //输出音频流信息         -a 配置 如：-a brazil-bq.aac 
extern char *mp4_out_name;   //输出修改后的mp4流      -o 配置 如：-o brazil-new.mp4 

extern int  g_errors;        //统计错误个数

void openMp4Files();    //打开mp4相关文件
void closeMp4Files();   //关闭mp4相关文件

void initMp4Info();     //初始化MP4常用的一些解析info信息


union Mp4xType
{
	unsigned char c_type[4]; // Box类型 32位
	unsigned int  i_type;
};

#define  MAX_TRACKINFOR_NUM 128
struct TrackInfor
{
	Mp4xType          handler_type;      //字段占32位 指示当前轨道的类型
	unsigned      int track_ID;          //audio的track_ID
};

enum HANDLETYPE
{
	HANDLETYPE_VIDE = 1701079414,
	HANDLETYPE_SOUN = 1853190003,
};

struct Mp4Info
{
	unsigned      int timescale;         //一秒钟分隔的份数 如：1000 表示 1秒钟分为1000份

	unsigned      int current_track_ID; //当前分析的track_ID

	unsigned      int audio_track_ID;   //audio的track_ID
	unsigned      int video_track_ID;   //video的track_ID
	TrackInfor        trackInfor[MAX_TRACKINFOR_NUM];

};

extern Mp4Info mp4Info;

#endif