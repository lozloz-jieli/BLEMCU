/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

/**
 * @file uh_crypt.h
 * @brief 涓庡畨鍏ㄧ浉鍏崇殑鎺ュ彛瀹氫箟锛屽寘鎷琓LS銆佸姞瑙ｅ瘑绛夌瓑銆�
 *
 * @date 2017-04-14
 * @author fanming
 *
 */

#ifndef __UH_CRYPT_H__
#define __UH_CRYPT_H__

//#include "uh_device.h"
#define UH_CRYPT_USE_MBEDTLS//TelinkDel
#ifdef UH_CRYPT_USE_MBEDTLS
#include "mbedtls/config.h"
#include "mbedtls/md.h"
#include "mbedtls/aes.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UH_CRYPT_USE_MBEDTLS

/** @def UH_MD_XXX
 *
 * @brief 鏀寔鐨勬秷鎭憳瑕佺畻娉曘��
 *
 * @details - XXX鍖呮嫭MD5銆丼HA1銆丼HA224銆丼HA256銆丼HA384銆丼HA512鍜孯IPEMD160銆�
 */
#define UH_MD_MD5 MBEDTLS_MD_MD5
#define UH_MD_SHA1 MBEDTLS_MD_SHA1
#define UH_MD_SHA224 MBEDTLS_MD_SHA224
#define UH_MD_SHA256 MBEDTLS_MD_SHA256
#define UH_MD_SHA384 MBEDTLS_MD_SHA384
#define UH_MD_SHA512 MBEDTLS_MD_SHA512
#define UH_MD_RIPEMD160 MBEDTLS_MD_RIPEMD160

typedef mbedtls_md_context_t uh_md_context_t;
typedef mbedtls_md_context_t cae_ble_md5_context;

/** @def UH_MODE_XXX
 *
 * @brief 鎿嶄綔鏂瑰紡锛屽姞瀵嗘垨鑰呰В瀵嗐��
 *
 * @details - XXX鍖呮嫭ENCRYPT鍜孌ECRYPT銆�
 */
#define UH_MODE_ENCRYPT MBEDTLS_AES_ENCRYPT
#define UH_MODE_DECRYPT MBEDTLS_AES_DECRYPT

/** @def UH_KET_BIT_XXX
 *
 * @brief 瀵嗛挜闀垮害锛屾寜姣旂壒闀垮害琛ㄧず銆�
 *
 * @details - XXX鍖呮嫭128鍜�256銆�
 */
#define UH_KET_BIT_128 128
#define UH_KET_BIT_256 256


/**
 * @brief 瀹夊叏妯″潡鍒濆鍖栥��
 * @note 涓嶉渶瑕佹樉绀鸿皟鐢紝鍦╱h_system_init鑷姩璋冪敤銆�
 *
 * @return 鎴愬姛杩斿洖UH_OK锛屽け璐ヨ繑鍥濽H_FAIL銆�
 */
extern int uh_crypt_init(void);

/**
 * @brief 鍒濆鍖栨秷鎭憳瑕佺畻娉曚笂涓嬫枃锛屽父鐢ㄤ簬鐢熸垚涓嶈繛缁暟鎹紙澶氭鏁版嵁锛夌殑娑堟伅鎽樿銆�
 *
 * @param md_ctx 鎸囧悜涓婁笅鏂囩粨鏋勭殑鎸囬拡銆�
 * @param md_type 浣跨敤鐨勬憳瑕佺畻娉曪紝鐢卞畯UH_MD_XXX瀹氫箟銆�
 *
 * @return 鎴愬姛杩斿洖UH_OK锛屽け璐ヨ繑鍥濽H_FAIL銆�
 */
extern int uh_crypt_md_init(uh_md_context_t *md_ctx, int md_type);

/**
 * @brief 鎵ц娑堟伅鎽樿鎿嶄綔锛屽彲澶氭鎵ц銆�
 *
 * @param md_ctx 鎸囧悜涓婁笅鏂囩粨鏋勭殑鎸囬拡锛屽繀椤讳娇鐢╱h_crypt_md_init杩涜鍒濆鍖栥��
 * @param buf 闇�瑕佽繘琛屾秷鎭憳瑕佺殑鏁版嵁鎸囬拡銆�
 * @param len 闇�瑕佽繘琛屾秷鎭憳瑕佺殑鏁版嵁闀垮害銆�
 *
 * @return N/A銆�
 */
extern void uh_crypt_md_update(uh_md_context_t *md_ctx, uint8_t *buf, unsigned int len);

/**
 * @brief 瀹屾垚娑堟伅鎽樿銆�
 *
 * @param md_ctx 鎸囧悜涓婁笅鏂囩粨鏋勭殑鎸囬拡锛屽繀椤讳娇鐢╱h_crypt_md_init杩涜鍒濆鍖栥��
 * @param output 杈撳嚭鐨勬秷鎭憳瑕佹暟鎹紝涓嶅悓娑堟伅鎽樿绠楁硶鐢熸垚涓嶅悓闀垮害鐨勬憳瑕侊紝璋冪敤鑰呭繀椤讳繚璇佹湁瓒冲绌洪棿瀛樺偍鎽樿銆�
 *
 * @return N/A銆�
 */
extern void uh_crypt_md_finish(uh_md_context_t *md_ctx, uint8_t *output);

/**
 * @brief 鐢熸垚娑堟伅鎽樿锛岄�氬父鐢ㄤ簬鐢熸垚杩炵画鏁版嵁锛堝崟娈垫暟鎹級鐨勬秷鎭憳瑕併��
 *
 * @param md_type 浣跨敤鐨勬憳瑕佺畻娉曪紝鐢卞畯UH_MD_XXX瀹氫箟銆�
 * @param buf 闇�瑕佽繘琛屾秷鎭憳瑕佺殑鏁版嵁鎸囬拡銆�
 * @param len 闇�瑕佽繘琛屾秷鎭憳瑕佺殑鏁版嵁闀垮害銆�
 * @param output 杈撳嚭鐨勬秷鎭憳瑕佹暟鎹紝涓嶅悓娑堟伅鎽樿绠楁硶鐢熸垚涓嶅悓闀垮害鐨勬憳瑕侊紝璋冪敤鑰呭繀椤讳繚璇佹湁瓒冲绌洪棿瀛樺偍鎽樿銆�
 *
 * @return 鎴愬姛杩斿洖UH_OK锛屽け璐ヨ繑鍥濽H_FAIL銆�
 */
extern int uh_crypt_md(int md_type, uint8_t *buf, unsigned int len, uint8_t *output);

/**
 * @brief AES CBC鍔犺В瀵嗐��
 *
 * @param mode 鎿嶄綔鏂规硶锛屽姞瀵嗘垨鑰呰В瀵嗭紝鐢卞畯UH_MODE_XXX瀹氫箟銆�
 * @param key 鍔犺В瀵嗕娇鐢ㄧ殑瀵嗛挜銆�
 * @param key_bit_len 鍔犺В瀵嗕娇鐢ㄧ殑瀵嗛挜闀垮害锛岀敱瀹廢H_KET_BIT_XXX瀹氫箟銆�
 * @param in 闇�瑕佽繘琛屽姞瑙ｅ瘑鐨勬暟鎹寚閽堛��
 * @param in_len 闇�瑕佽繘琛屽姞瑙ｅ瘑鐨勬暟鎹暱搴︺�傚繀椤绘槸16瀛楄妭鐨勬暣鍊嶆暟銆�
 * @param iv 鍔犺В瀵嗘椂鐨勫垵濮嬪悜閲忋��
 * @param iv_len 鍔犺В瀵嗘椂鐨勫垵濮嬪悜閲忛暱搴︺�傚綋鍓嶅彧鏀寔16銆�
 * @param out 杈撳嚭鐨勫姞瀵嗘垨鑰呰В瀵嗘暟鎹紝闀垮害涓庢簮鏁版嵁鐩稿悓銆�
 *
 * @return N/A銆�
 */
extern void uh_crypt_aes_crypt_cbc_with_iv(int mode, uint8_t * key, uint32_t key_bit_len,
	uint8_t * in, unsigned int in_len, uint8_t *iv, unsigned int iv_len, uint8_t *out);

/**
 * @brief AES CBC鍔犺В瀵嗐��
 *
 * @param mode 鎿嶄綔鏂规硶锛屽姞瀵嗘垨鑰呰В瀵嗭紝鐢卞畯UH_MODE_XXX瀹氫箟銆�
 * @param key 鍔犺В瀵嗕娇鐢ㄧ殑瀵嗛挜銆�
 * @param key_bit_len 鍔犺В瀵嗕娇鐢ㄧ殑瀵嗛挜闀垮害锛岀敱瀹廢H_KET_BIT_XXX瀹氫箟銆�
 * @param in 闇�瑕佽繘琛屽姞瑙ｅ瘑鐨勬暟鎹寚閽堛��
 * @param in_len 闇�瑕佽繘琛屽姞瑙ｅ瘑鐨勬暟鎹暱搴︺�傚繀椤绘槸16瀛楄妭鐨勬暣鍊嶆暟銆�
 * @param out 杈撳嚭鐨勫姞瀵嗘垨鑰呰В瀵嗘暟鎹紝闀垮害涓庢簮鏁版嵁鐩稿悓銆�
 *
 * @return N/A銆�
 */
extern void uh_crypt_aes_crypt_cbc(int mode, uint8_t * key, uint32_t key_bit_len, uint8_t * in, uint32_t in_len, uint8_t *out);
#else
#define UH_MD_MD5 3 //TelinkAdd
#define UH_MODE_ENCRYPT 1 //TelinkAdd
#define UH_MODE_DECRYPT 0 //TelinkAdd
#endif

#ifdef __cplusplus
}
#endif

#endif /*__UH_CRYPT_H__*/

