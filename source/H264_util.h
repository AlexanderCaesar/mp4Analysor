#pragma once

#include <stdint.h>
#include <cstdio>
#include <utility>
#include <vector>

typedef enum
{
	NAL_UNKNOWN = 0,
	NAL_SLICE = 1,
	NAL_SLICE_DPA = 2,
	NAL_SLICE_DPB = 3,
	NAL_SLICE_DPC = 4,
	NAL_SLICE_IDR = 5,    /* ref_idc != 0 */
	NAL_SEI = 6,    /* ref_idc == 0 */
	NAL_SPS = 7,
	NAL_PPS = 8,
	NAL_AUD = 9,
	NAL_FILLER = 12,
	/* ref_idc == 0 for 6,9,10,11,12 */
}NaluType;

/// supported slice type
enum SliceType
{
	P0SLICE,
	B1SLICE,
	I2SLICE,
	SP3SLICE,
	SI4SLICE,
	P_SLICE,
	B_SLICE,
	I_SLICE,
	SPSLICE,
	SISLICE,
	ERROR_SLICE
};

extern char* NaluTypeString[];
extern char *SliceTypeString[];

class VideoEncodedFrame
{
public:
    int data_len;
    uint8_t* data;
};

class H264Util {
public:

	int log2_max_frame_num_minus4;
	int frame_num;
	int nal_ref_idc;
	SliceType ParseSliceHeader(uint8_t* slice_nal);
	bool ParseSps(uint8_t* sps_nal, uint32_t* w, uint32_t* h);
	NaluType ParseNaluType(uint8_t data);
	void writeNAL(VideoEncodedFrame *video);
};

