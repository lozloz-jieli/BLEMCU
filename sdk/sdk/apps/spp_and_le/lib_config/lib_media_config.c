/*********************************************************************************************
    *   Filename        : lib_driver_config.c

    *   Description     : Optimized Code & RAM (编译优化配置)

    *   Author          : Bingquan

    *   Email           : caibingquan@zh-jieli.com

    *   Last modifiled  : 2019-03-18 14:58

    *   Copyright:(c)JIELI  2011-2019  @ , All Rights Reserved.
*********************************************************************************************/
#include "app_config.h"
#include "system/includes.h"
#include "media/includes.h"


#if (TCFG_AUDIO_DECODER_OCCUPY_TRACE)
const u8 audio_decoder_occupy_trace_enable = 1;
const u8 audio_decoder_occupy_trace_dump = 0;
#else
const u8 audio_decoder_occupy_trace_enable = 0;
const u8 audio_decoder_occupy_trace_dump = 0;
#endif/*TCFG_AUDIO_DECODER_OCCUPY_TRACE*/

#if TCFG_EQ_ENABLE
const int config_audio_eq_en = 1;

#if TCFG_EQ_ONLINE_ENABLE
const int config_audio_eq_online_en = 1;
#else
const int config_audio_eq_online_en = 0;
#endif

#if TCFG_USE_EQ_FILE
const int config_audio_eq_file_en = 1;
#else
const int config_audio_eq_file_en = 0;
#endif

const int config_audio_eq_file_sw_en = 0;
#if TCFG_AUDIO_OUT_EQ_ENABLE
const int config_high_bass_en = 1;
#else
const int config_high_bass_en = 0;
#endif
const int config_filter_coeff_fade_en = 0;

#if (RCSP_ADV_EN)&&(JL_EARPHONE_APP_EN)&&(TCFG_DRC_ENABLE == 0)
const int config_filter_coeff_limit_zero = 1;
#else
const int config_filter_coeff_limit_zero = 0;
#endif

#ifdef CONFIG_SOUNDBOX_FLASH_256K
const int HW_EQ_UPDATE_COEFF_ONLY_EN = 0;
const int HW_EQ_LR_ALONE = 0 ;
#else
const int HW_EQ_UPDATE_COEFF_ONLY_EN = 1;// 有空闲的段可以使用，就不需要切换系数 */
const int HW_EQ_LR_ALONE = 1 ;// 左右声道分开处理
#endif

#else
const int config_audio_eq_file_sw_en = 0;
const int config_audio_eq_file_en = 0;
const int config_audio_eq_online_en = 0;
const int config_audio_eq_en = 0;
const int config_high_bass_en = 0;
const int config_filter_coeff_fade_en = 0;
const int config_filter_coeff_limit_zero = 0;
const int HW_EQ_UPDATE_COEFF_ONLY_EN = 0;
const int HW_EQ_LR_ALONE = 0 ;

#endif

const int SUPPORT_32BIT_SYNC_EQ = 0;// 支持同步方式32biteq
const int AUDIO_EQ_CLEAR_MEM_BY_MUTE_TIME_MS = 0;//300 //连续多长时间静音就清除EQ MEM
const int AUDIO_EQ_CLEAR_MEM_BY_MUTE_LIMIT = 0; //静音判断阀值
const int hw_eq_support_multi_channels = 0;

#if TCFG_DRC_ENABLE
const int config_audio_drc_en = 1;
#else
const int config_audio_drc_en = 0;
#endif

#if TCFG_MIC_EFFECT_ENABLE
const int config_audio_dac_mix_enable = 1;
#else
const int config_audio_dac_mix_enable = 0;
#endif

#ifdef CONFIG_SOUNDBOX_FLASH_256K
// mixer模块使能。不使能将关闭大部分功能，mix为直通
const int config_mixer_en = 0;
// mixer变采样使能
const int config_mixer_src_en = 0;

// audio解码资源叠加功能使能。不使能，如果配置了叠加方式，将改成抢占方式
const int config_audio_dec_wait_protect_en = 0;

// audio数据流分支功能使能。
const int config_audio_stream_frame_copy_en = 0;

// audio dec app调用mixer相关函数控制。关闭后需上层设置数据流的输出节点
const int audio_dec_app_mix_en = 0;

#else

// mixer模块使能。不使能将关闭大部分功能，mix为直通
const int config_mixer_en = 1;
// mixer变采样使能
const int config_mixer_src_en = 1;

// audio解码资源叠加功能使能。不使能，如果配置了叠加方式，将改成抢占方式
const int config_audio_dec_wait_protect_en = 1;

// audio数据流分支功能使能。
const int config_audio_stream_frame_copy_en = 1;

// audio dec app调用mixer相关函数控制。关闭后需上层设置数据流的输出节点
const int audio_dec_app_mix_en = 1;

#endif

//wts解码支持采样率可选择，可以同时打开也可以单独打开
const  int  silk_fsN_enable = 1;   //支持8-12k采样率
const  int  silk_fsW_enable = 1;  //支持16-24k采样率

// audio数据流分支cbuf大小控制
const int config_audio_stream_frame_copy_cbuf_min = 128;
const int config_audio_stream_frame_copy_cbuf_max = 1024;

#if RECORDER_MIX_EN
// 超时等待其他解码unactive步骤完成
const int config_audio_dec_unactive_to = 10;
// audio数据流ioctrl使能
const int config_audio_stream_frame_ioctrl_en = 1;
#else
// 超时等待其他解码unactive步骤完成
const int config_audio_dec_unactive_to = 0;
// audio数据流ioctrl使能
const int config_audio_stream_frame_ioctrl_en = 0;
#endif

#if TCFG_TONE2TWS_ENABLE

// audio dec app tws同步使能
const int audio_dec_app_sync_en = 1;

// wma tws 解码控制
const int WMA_TWSDEC_EN = 1;

#else

// audio dec app tws同步使能
const int audio_dec_app_sync_en = 0;

// wma tws 解码控制
const int WMA_TWSDEC_EN = 0;

#endif


#if TCFG_DEC2TWS_ENABLE || SOUNDCARD_ENABLE

// 解码一次输出点数，1代表32对点，n就是n*32对点
// 超过1时，解码需要使用malloc，如config_mp3_dec_use_malloc=1
const int MP3_OUTPUT_LEN = 4;
const int WMA_OUTPUT_LEN = 4;

#else

// 解码一次输出点数，1代表32对点，n就是n*32对点
// 超过1时，解码需要使用malloc，如config_mp3_dec_use_malloc=1
const int MP3_OUTPUT_LEN = 1;
const int WMA_OUTPUT_LEN = 1;

#endif

// wav最大支持比特率，单位kbps
const int WAV_MAX_BITRATEV = (48 * 2 * 24);


// 解码一次输出点数,建议范围32到900,例如128代表128对点
// 超过128时，解码需要使用malloc，如config_wav_dec_use_malloc=1
const int  WAV_DECODER_PCM_POINTS = 128;

// output超过128时，如果不使用malloc，需要增大对应buf
// 可以看打印中解码器需要的大小，一般输出每增加1长度增加4个字节
int wav_mem_ext[(1316  + 3) / 4] SEC(.wav_mem); //超过128要增加这个数组的大小



// mixer在单独任务中输出
const int config_mixer_task = 0;

// tws音频解码自动设置输出声道。
// 单声道：AUDIO_CH_L/AUDIO_CH_R。双声道：AUDIO_CH_DUAL_L/AUDIO_CH_DUAL_R
// 关闭后，按照output_ch_num和output_ch_type/ch_type设置输出声道
const int audio_tws_auto_channel = 1;

// 解码使用单独任务做输出
const int config_audio_dec_out_task_en = 0;


/*省电容mic配置*/
#if TCFG_SUPPORT_MIC_CAPLESS
const u8 const_mic_capless_en = 1;
#else
const u8 const_mic_capless_en = 0;
#endif/*TCFG_SUPPORT_MIC_CAPLESS*/

#if TCFG_EQ_DIVIDE_ENABLE
const int config_divide_en = 1;
#else
const int config_divide_en = 0;
#endif


#if AUDIO_EQUALLOUDNESS_CONFIG
const int const_equall_loundness_en = 1;
#else
const int const_equall_loundness_en = 0;
#endif

#if AUDIO_VBASS_CONFIG
const int const_vbass_en = 1;
#else
const int const_vbass_en = 0;
#endif

#if AUDIO_SURROUND_CONFIG
const int const_surround_en = 1;
#else
const int const_surround_en = 0;
#endif




const int const_sel_adpcm_type = 0;//1：使用imaen_adpcm,  0:msen_adpcm

#ifdef CONFIG_MIDI_DEC_ADDR
const int MIDI_TONE_MODE = 0;//0是地址访问(仅支持在内置flash,读数快，消耗mips低)，1 是文件访问(内置、外挂flash,sd,u盘均可,读数慢，消耗mips较大)
#else
const int MIDI_TONE_MODE = 1;
#endif


#if TCFG_MEDIA_LIB_USE_MALLOC
const int config_mp3_dec_use_malloc     = 1;
const int config_mp3pick_dec_use_malloc = 1;
const int config_wma_dec_use_malloc     = 1;
const int config_wmapick_dec_use_malloc = 1;
const int config_m4a_dec_use_malloc     = 1;
const int config_m4apick_dec_use_malloc = 1;
const int config_wav_dec_use_malloc     = 1;
const int config_alac_dec_use_malloc    = 1;
const int config_dts_dec_use_malloc     = 1;
const int config_amr_dec_use_malloc     = 1;
const int config_flac_dec_use_malloc    = 1;
const int config_ape_dec_use_malloc     = 1;
const int config_aac_dec_use_malloc     = 1;
const int config_aptx_dec_use_malloc    = 1;
const int config_midi_dec_use_malloc    = 1;
const int config_lc3_dec_use_malloc     = 1;
#else
const int config_mp3_dec_use_malloc     = 0;
const int config_mp3pick_dec_use_malloc = 0;
const int config_wma_dec_use_malloc     = 0;
const int config_wmapick_dec_use_malloc = 0;
const int config_m4a_dec_use_malloc     = 0;
const int config_m4apick_dec_use_malloc = 0;
#if TCFG_DEC_WAV_ENABLE
const int config_wav_dec_use_malloc     = 1;
#else
const int config_wav_dec_use_malloc     = 0;
#endif
const int config_alac_dec_use_malloc    = 0;
const int config_dts_dec_use_malloc     = 0;
const int config_amr_dec_use_malloc     = 0;
const int config_flac_dec_use_malloc    = 0;
const int config_ape_dec_use_malloc     = 0;
const int config_aac_dec_use_malloc     = 0;
const int config_aptx_dec_use_malloc    = 0;
const int config_midi_dec_use_malloc    = 0;
const int config_lc3_dec_use_malloc     = 0;
#endif
const int vc_pitchshift_fastmode_flag        = 1;
const  int  vc_pitchshift_downmode_flag = 0;  //变声下采样处理使能
const int howling_pitchshift_fastmode_flag   = 1;
const  int RS_FAST_MODE_QUALITY = 2;	//软件变采样 滤波阶数配置，范围2到8， 8代表16阶的变采样模式 ,速度跟它的大小呈正相关
const int config_howling_enable_pemafrow_mode = 0;
const int config_howling_enable_trap_mode     = 0;//陷波啸叫抑制模式使能
const int config_howling_enable_pitchps_mode  = 1; //移频啸叫抑制模式使能
const  int DOWN_S_FLAG 				= 0; //混响降采样处理使能

#if (AUDIO_OUTPUT_WAY == AUDIO_OUTPUT_WAY_DONGLE)
const int config_mp3_enc_use_layer_3	= 1;
#else
const int config_mp3_enc_use_layer_3	= 0;
#endif

#define FAST_FREQ_restrict				0x01 //限制超过16k的频率不解【一般超出人耳听力范围，但是仪器会测出来】
#define FAST_FILTER_restrict			0x02 //限制滤波器长度【子带滤波器旁瓣加大，边缘不够陡】
#define FAST_CHANNEL_restrict			0x04 //混合左右声道，再解码【如果是左右声道独立性较强的歌曲，会牺牲空间感，特别耳机听会听出来的话】

const int config_mp3_dec_speed_mode 	= 0; //FAST_FREQ_restrict | FAST_FILTER_restrict | FAST_CHANNEL_restrict; //3个开关都置上，是最快的解码模式

// 快进快退到文件end返回结束消息
const int config_decoder_ff_fr_end_return_event_end = 0;



// 解码任务测试
const int audio_decoder_test_en = 0;
// 当audio_decoder_test_en使能时需要实现以下接口
#if 0
void audio_decoder_test_out_before(struct audio_decoder *dec, void *buff, int len) {} ;
void audio_decoder_test_out_after(struct audio_decoder *dec, int wlen) {} ;
void audio_decoder_test_read_before(struct audio_decoder *dec, int len, u32 offset) {} ;
void audio_decoder_test_read_after(struct audio_decoder *dec, u8 *data, int rlen) {} ;
void audio_decoder_test_get_frame_before(struct audio_decoder *dec) {} ;
void audio_decoder_test_get_frame_after(struct audio_decoder *dec, u8 *frame, int rlen) {} ;
void audio_decoder_test_fetch_before(struct audio_decoder *dec) {} ;
void audio_decoder_test_fetch_after(struct audio_decoder *dec, u8 *frame, int rlen) {} ;
void audio_decoder_test_run_before(struct audio_decoder *dec) {} ;
void audio_decoder_test_run_after(struct audio_decoder *dec, int err) {} ;
#else
// 接口实现示例
#include "audio/demo/audio_decoder_test.c"
#endif


// 编码任务测试
const int audio_encoder_test_en = 0;
// 当audio_encoder_test_en使能时需要实现以下接口
#if 0
void audio_encoder_test_out_before(struct audio_encoder *enc, void *buff, int len) {} ;
void audio_encoder_test_out_after(struct audio_encoder *enc, int wlen) {} ;
void audio_encoder_test_get_frame_before(struct audio_encoder *enc, u16 frame_len) {} ;
void audio_encoder_test_get_frame_after(struct audio_encoder *enc, s16 *frame, int rlen) {} ;
void audio_encoder_test_run_before(struct audio_encoder *enc) {} ;
void audio_encoder_test_run_after(struct audio_encoder *enc, int err) {} ;
#else
// 接口实现示例
#include "audio/demo/audio_encoder_test.c"
#endif



/**
 * @brief Log (Verbose/Info/Debug/Warn/Error)
 */
/*-----------------------------------------------------------*/
const char log_tag_const_v_EQ_CFG AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(FALSE);
const char log_tag_const_i_EQ_CFG AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(FALSE);
const char log_tag_const_d_EQ_CFG AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(FALSE);
const char log_tag_const_w_EQ_CFG AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_EQ_CFG AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);

const char log_tag_const_v_EQ_CFG_TOOL AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(FALSE);
const char log_tag_const_i_EQ_CFG_TOOL AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(FALSE);
const char log_tag_const_d_EQ_CFG_TOOL AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(FALSE);
const char log_tag_const_w_EQ_CFG_TOOL AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_EQ_CFG_TOOL AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);

const char log_tag_const_v_EQ_APPLY AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_EQ_APPLY AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_EQ_APPLY AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_w_EQ_APPLY AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_EQ_APPLY AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);

const char log_tag_const_v_DRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_DRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_DRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_w_DRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_DRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);

const char log_tag_const_v_APP_DRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_APP_DRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_APP_DRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_w_APP_DRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_APP_DRC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);


const char log_tag_const_v_EQ AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_EQ AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_EQ AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_w_EQ AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_EQ AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);


const char log_tag_const_v_VBASS AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_VBASS AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_VBASS AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_w_VBASS AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_VBASS AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);

const char log_tag_const_v_AUD_ADC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_AUD_ADC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_AUD_ADC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_w_AUD_ADC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_e_AUD_ADC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);

const char log_tag_const_v_AUD_DAC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_AUD_DAC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_AUD_DAC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_w_AUD_DAC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_e_AUD_DAC AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);

const char log_tag_const_v_AUD_AUX AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_AUD_AUX AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_AUD_AUX AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_w_AUD_AUX AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_e_AUD_AUX AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);



const char log_tag_const_v_MIXER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_c_MIXER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_MIXER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_MIXER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_MIXER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);

const char log_tag_const_v_AUDIO_STREAM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_c_AUDIO_STREAM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_AUDIO_STREAM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_AUDIO_STREAM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_AUDIO_STREAM AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);

const char log_tag_const_v_AUDIO_DECODER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_c_AUDIO_DECODER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_AUDIO_DECODER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_AUDIO_DECODER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_AUDIO_DECODER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);

const char log_tag_const_v_AUDIO_ENCODER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_c_AUDIO_ENCODER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_i_AUDIO_ENCODER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(0);
const char log_tag_const_d_AUDIO_ENCODER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);
const char log_tag_const_e_AUDIO_ENCODER AT(.LOG_TAG_CONST) = CONFIG_DEBUG_LIB(TRUE);



