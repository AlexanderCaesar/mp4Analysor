#ifndef XIAOC_MP4_H
#define XIAOC_MP4_H
//作者  蔡砚刚  20171214  xiaoc@pku.edu.cn

typedef struct mp4_param
{
    /* 配置信息
    -i 配置 如：-i brazil-bq.mp4
    -t 配置 如：-t brazil-bq.log
    -v 配置 如：-v brazil-bq.264
    -a 配置 如：-a brazil-bq.aac
	-o 配置 如：-o brazil-new.mp4
    **/
    int  b_video; //是否输出视频流信息         -v 配置 如：-v brazil-bq.264 
    int  b_audio; //是否输出音频流信息         -a 配置 如：-a brazil-bq.aac 
    int  b_n_mp4;  //是否输出新的mp4文件       -o 配置 如：-o brazil-new.mp4

}mp4_param;

extern mp4_param m_param; //配置参数信息

/* 设置mp4的默认参数 */
void mp4_param_default(mp4_param *param);

/* 解析配置参数 */
bool parse(mp4_param *param,int argc, char **argv);

#endif
