#ifndef  XIAOC_MP4_STSCBOX_H
#define  XIAOC_MP4_STSCBOX_H
//作者  蔡砚刚  20180105  xiaoc@pku.edu.cn
#include "box.h"
/*样例
first_chunk  samples_per_chunk  sample_description_index
 1             15                  1
 105           30                  1
 106           22                  1
 上面表示总共106个chunk
 分为三大类 ：
 分别有每个chunk有15个sample 当前chunk组从标号为1开始一直到标号为104
 分别有每个chunk有30个sample 当前chunk组只有一个chunk 标号105
 分别有每个chunk有22个sample 当前chunk组只有一个chunk 标号106
*/
class STSCBOX : public FullBox
{
public:
	unsigned     int entry_count; //表示媒体片段数目


	unsigned     int first_chunk;//当前相同chunk组的第一个chunk标号 从1开始计数 32位
	unsigned     int samples_per_chunk;//每个chunk的sample个数 32位
	unsigned     int sample_description_index;//只是stsd box（不止一个）的索引位置 32位

public:
	STSCBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif