#ifndef  XIAOC_MP4_SDTPBOX_H
#define  XIAOC_MP4_SDTPBOX_H
//作者  蔡砚刚  20180105  xiaoc@pku.edu.cn
#include "box.h"

class SDTPBOX : public FullBox
{
public:
	unsigned int   reserved; //保留字段占2位
	unsigned int   sample_depends_on;//占2位 当前帧参考其它帧类型 0：不确定 1 参考非I帧 2 参考I帧 3保留字段
	unsigned int   sample_is_depended_on;//占2位 当前帧参被考其它帧类型 0：不确定 1 不被其它帧参考 2 被其它帧参考 3保留字段
	unsigned int   sample_has_redundancy; //占2位 是否有冗余编码数据 0：不确定 1 拥有 2 不拥有 3不确定

public:
	SDTPBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif