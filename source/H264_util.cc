//作者  蔡砚刚  20180101  xiaoc@pku.edu.cn
#include "H264_util.h"

char* NaluTypeString[] =
{
	"NAL_UNKNOWN",
	"NAL_SLICE",
	"NAL_SLICE_DPA",
	"NAL_SLICE_DPB",
	"NAL_SLICE_DPC",
	"NAL_SLICE_IDR",    /* ref_idc != 0 */
	"NAL_SEI",   /* ref_idc == 0 */
	"NAL_SPS",
	"NAL_PPS",
	"NAL_AUD",
	"NONE",
	"NONE",
	"NONE",
	"NAL_FILLER", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE"
	/* ref_idc == 0 for 6,9,10,11,12 */
};

char *SliceTypeString[]
{
	"P_SLICE",
		"B_SLICE",
		"I_SLICE",
		"SPSLICE",
		"SISLICE",
		"P_SLICE",
		"B_SLICE",
		"I_SLICE",
		"SPSLICE",
		"SISLICE",
		"ERROR_SLICE"
};


#pragma warning(disable:4996)
int u(uint8_t* data, unsigned bits, int start_bit) {
  int r = 0;
  int offset = start_bit;

  for (unsigned i = 0; i < bits; i++) {
    r <<= 1;

    if (data[offset / 8] & (0x80 >> offset % 8)) {
      r++;
    }

    offset++;
  }

  return r;
}

int ue(uint8_t* data, int& start_bit) {
  int zero_bits = 0;
  int start = start_bit;

  while (u(data, 1, start) == 0) {
    zero_bits++;
    start++;
  }

  start++;

  int r = (int)(pow(2, zero_bits) - 1 + u(data, zero_bits, start));
  //    dumpBits(data,  zero_bits * 2 + 1, start_bit);
  start_bit = start + zero_bits;
  return r;
}

int se(uint8_t* data, int& start_bit) {
  auto ue_val = ue(data, start_bit);
  return (int)(pow(-1, ue_val + 1) * ceil(ue_val / 2));
}


SliceType H264Util::ParseSliceHeader(uint8_t* slice_nal)
{
	if (!slice_nal) {
		return ERROR_SLICE;
	}

	NaluType naltype = ParseNaluType(*slice_nal);
	if (naltype != NAL_SLICE &&
		naltype != NAL_SLICE_IDR )
	{
		return ERROR_SLICE;
	}


	uint8_t* ptr = slice_nal + 1;

	uint32_t uiCode;
	int start_bit = 0;

	uiCode = ue(ptr, start_bit);

	SliceType slicetype = (SliceType)ue(ptr, start_bit);

	uiCode = ue(ptr, start_bit);

	frame_num = u(ptr, log2_max_frame_num_minus4 + 4, start_bit);
	
	return slicetype;
}

bool H264Util::ParseSps(uint8_t* sps_nal, uint32_t* w, uint32_t* h) {
	if (!sps_nal) {
		return false;
	}

	uint8_t* ptr = (uint8_t*)sps_nal;

	if (ParseNaluType(*ptr++) != NAL_SPS) {
		return false;
	}

	int start_bit = 0;

	int profile_idc = u(ptr, 8, start_bit);
	start_bit += 8;

	u(ptr, 8, 0); //constraint_set?_flag && reserved_zero_5bits
	start_bit += 8;

	u(ptr, 8, start_bit); // level_idc
	start_bit += 8;

	ue(ptr, start_bit); // seq_parameter_set_id
	int chroma_format_idc = 0;

	if (profile_idc == 100 || profile_idc == 110 || profile_idc == 122 || profile_idc == 244
		|| profile_idc == 44 || profile_idc == 83 || profile_idc == 86 || profile_idc == 118
		|| profile_idc == 128 || profile_idc == 138) {
		chroma_format_idc = ue(ptr, start_bit);
		if (chroma_format_idc == 3) {
			u(ptr, 1, start_bit++); // separate_colour_plane_flag
		}
		ue(ptr, start_bit); // bit_depth_luma_minus8
		ue(ptr, start_bit); // bit_depth_chroma_minus8
		u(ptr, 1, start_bit++); // qpprime_y_zero_transform_bypass_flag
		int seq_scaling_matrix_present_flag = u(ptr, 1, start_bit++);
		if (seq_scaling_matrix_present_flag) {
			for (auto i = 0; i < ((chroma_format_idc != 3) ? 8 : 12); ++i) {
				int seq_scaling_list_present_flag = u(ptr, 1, start_bit++);
				if (seq_scaling_list_present_flag) { // TODO
					if (i < 6) {
						// scaling_list( ScalingList4x4[ i ], 16, UseDefaultScalingMatrix4x4Flag[ i ])
					}
					else {
						// scaling_list( ScalingList8x8[ i − 6 ], 64, UseDefaultScalingMatrix8x8Flag[ i − 6 ] )
					}
				}
			}
		}
	}

	log2_max_frame_num_minus4 = ue(ptr, start_bit); // log2_max_frame_num_minus4

	int pic_order_cnt_type = ue(ptr, start_bit);

	if (pic_order_cnt_type == 0) {
		ue(ptr, start_bit); // log2_max_pic_order_cnt_lsb_minus4
	}
	else if (pic_order_cnt_type == 1) {
		u(ptr, 1, start_bit++); // delta_pic_order_always_zero_flag
		se(ptr, start_bit); // offset_for_non_ref_pic
		se(ptr, start_bit); // offset_for_top_to_bottom_field
		int num_ref_frames_in_pic_order_cnt_cycle = ue(ptr, start_bit);
		for (auto i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; ++i) {
			se(ptr, start_bit); // offset_for_ref_frame[ i ]
		}
	}

	int max_num_ref_frames = ue(ptr, start_bit); // max_num_ref_frames
	u(ptr, 1, start_bit++); // gaps_in_frame_num_value_allowed_flag

	int pic_width_in_mbs_minus1 = ue(ptr, start_bit);
	int pic_height_in_map_units_minus1 = ue(ptr, start_bit);

	int frame_mbs_only_flag = u(ptr, 1, start_bit++);
	if (!frame_mbs_only_flag) {
		u(ptr, 1, start_bit++); // mb_adaptive_frame_field_flag
	}

	u(ptr, 1, start_bit++); // direct_8x8_inference_flag

	int frame_cropping_flag = u(ptr, 1, start_bit++);
	int frame_crop_left_offset = 0;
	int frame_crop_right_offset = 0;
	int frame_crop_top_offset = 0;
	int frame_crop_bottom_offset = 0;
	if (frame_cropping_flag) {
		frame_crop_left_offset = ue(ptr, start_bit);
		frame_crop_right_offset = ue(ptr, start_bit);
		frame_crop_top_offset = ue(ptr, start_bit);
		frame_crop_bottom_offset = ue(ptr, start_bit);
	}

	int vui_parameters_present_flag = u(ptr, 1, start_bit++);
	if (vui_parameters_present_flag) {
		// vui_parameters( )
	}

	if (w) {
		int hsub = (chroma_format_idc == 1 || chroma_format_idc == 2) ? 1 : 0;
		int step_x = 1 << hsub;
		int crop_left = frame_crop_left_offset * step_x;
		int crop_right = frame_crop_right_offset * step_x;
		*w = (pic_width_in_mbs_minus1 + 1) * 16 - crop_left - crop_right;
	}
	if (h) {
		int vsub = (chroma_format_idc == 1) ? 1 : 0;
		int step_y = (2 - frame_mbs_only_flag) << vsub;
		int crop_top = frame_crop_top_offset * step_y;
		int crop_bottom = frame_crop_bottom_offset * step_y;
		*h = (pic_height_in_map_units_minus1 + 1) * 16 - crop_top - crop_bottom;
	}

	return true;
}

NaluType H264Util::ParseNaluType(uint8_t data) {
	nal_ref_idc = (data & 0x60)>>5;
    return static_cast<NaluType>(data & 0x1F);
}

void H264Util::writeNAL(VideoEncodedFrame *video)
{
	FILE *fp = fopen("new.264","ab");
	fwrite(video->data, 1, video->data_len, fp);
	fclose(fp);
}



