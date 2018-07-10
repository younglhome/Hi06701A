#ifndef _NVP6124_VIDEO_HI_
#define _NVP6124_VIDEO_HI_


//#include "common.h"
#include "nvp6124.h"

void nvp6124_ntsc_common_init(void);
void nvp6124_pal_common_init(void);
void nvp6124_outport_1mux_chseq(void);
void nvp6124_outport_2mux(unsigned char vformat, unsigned char port1_mode, unsigned char port2_mode);
void nvp6124_outport_4mux(unsigned char vformat, unsigned char port1_mode);
void nvp6114a_outport_1mux_chseq(void);
void nvp6114a_outport_2mux(unsigned char vformat, unsigned char port1_mode, unsigned char port2_mode);
void nvp6114a_outport_4mux(unsigned char vformat, unsigned char port1_mode);
void nvp6124_set_portmode(unsigned char chip, unsigned char portsel, unsigned char portmode, unsigned char chid);
void nvp6114a_set_portmode(unsigned char chip, unsigned char portsel, unsigned char portmode, unsigned char chid);
int nvp6124_set_chnmode(unsigned char ch, unsigned char chnmode);
void nvp6124_set_clockmode(unsigned char chip, unsigned char is297MHz);
void nvp6124_each_mode_setting(nvp6124_video_mode *pvmode);
void video_fmt_det(nvp6124_input_videofmt *pvideofmt);
unsigned int nvp6124_getvideoloss(void);
void nvp6124_syncchange(void);
void nvp6124_set_equalizer(void);

void nvp6124_video_set_contrast(unsigned int ch, unsigned int value, unsigned int v_format);
void nvp6124_video_set_brightness(unsigned int ch, unsigned int value, unsigned int v_format);
void nvp6124_video_set_saturation(unsigned int ch, unsigned int value, unsigned int v_format);
void nvp6124_video_set_hue(unsigned int ch, unsigned int value, unsigned int v_format);
void nvp6124_video_set_sharpness(unsigned int ch, unsigned int value);

enum
{
	NVP6124_VI_SD = 0,
	NVP6124_VI_720P_2530,
	NVP6124_VI_720P_5060,
	NVP6124_VI_1080P_2530,
	NVP6124_VI_1920H,
	NVP6124_VI_720H,
	NVP6124_VI_1280H,
	NVP6124_VI_1440H,
	NVP6124_VI_BUTT
};

enum
{
	NVP6124_OUTMODE_1MUX_SD = 0,
	NVP6124_OUTMODE_1MUX_HD,
	NVP6124_OUTMODE_1MUX_HD5060,
	NVP6124_OUTMODE_1MUX_FHD,
	NVP6124_OUTMODE_2MUX_SD,
	NVP6124_OUTMODE_2MUX_HD_X,
	NVP6124_OUTMODE_2MUX_HD,
	NVP6124_OUTMODE_2MUX_FHD_X,
	NVP6124_OUTMODE_4MUX_SD,
	NVP6124_OUTMODE_4MUX_HD_X,
	NVP6124_OUTMODE_4MUX_HD,
	NVP6124_OUTMODE_2MUX_FHD,
	NVP6124_OUTMODE_BUTT
};

#endif
