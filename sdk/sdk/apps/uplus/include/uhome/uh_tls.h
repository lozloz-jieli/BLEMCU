/*Copyright (c) 2019 Qingdao Haier Technology Co., Ltd. All rights reserved. */

/**
 * @file uh_tls.h
 * @brief TLS鐩稿叧鎺ュ彛瀹氫箟
 *
 * @date 2017-04-14
 * @author fanming
 *
 */

#ifndef __UH_TLS_H__
#define __UH_TLS_H__

#include "uh_common.h"

#ifdef UH_TLS_USE_MBEDTLS
#include "mbedtls/config.h"
#include "mbedtls/ssl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UH_TLS_USE_MBEDTLS

/** @def SSL_IS_CLIENT/SSL_IS_SERVER
 *
 * @brief SSL绔偣绫诲瀷锛屽鎴风鎴栬�呮湇鍔″櫒銆�
 *
 */
/*end point*/
#define SSL_IS_CLIENT MBEDTLS_SSL_IS_CLIENT
#define SSL_IS_SERVER MBEDTLS_SSL_IS_SERVER

/** @def SSL_TRANSPORT_STREAM/SSL_TRANSPORT_DATAGRAM
 *
 * @brief SSL浼犺緭绫诲瀷锛孴CP鎴栬�匲DP銆�
 *
 */
/*transport*/
#define SSL_TRANSPORT_STREAM MBEDTLS_SSL_TRANSPORT_STREAM /*TLS*/
#define SSL_TRANSPORT_DATAGRAM MBEDTLS_SSL_TRANSPORT_DATAGRAM /*DTLS*/

/** @def SSL_AUTH_MODE_VERIFY/SSL_AUTH_MODE_NONE
 *
 * @brief SSL楠岃瘉妯″紡锛岄獙璇佽瘉涔︽垨鑰呬笉楠岃瘉銆�
 *
 */
#define SSL_AUTH_MODE_VERIFY MBEDTLS_SSL_VERIFY_REQUIRED
#define SSL_AUTH_MODE_NONE MBEDTLS_SSL_VERIFY_NONE

/** @def SSL_VER_XXX
 *
 * @brief SSL鐗堟湰鍙枫��
 * @details XXX鍙互鏄疭SL_3_0/SSL_3_1/SSL_3_2/SSL_3_3/TLS_1_0/TLS_1_1/TLS_1_2銆�
 *
 */
/*SSL version*/
#define SSL_VER_SSL_3_0 MBEDTLS_SSL_MINOR_VERSION_0
#define SSL_VER_SSL_3_1 MBEDTLS_SSL_MINOR_VERSION_1
#define SSL_VER_SSL_3_2 MBEDTLS_SSL_MINOR_VERSION_2
#define SSL_VER_SSL_3_3 MBEDTLS_SSL_MINOR_VERSION_3

#define SSL_VER_TLS_1_0 SSL_VER_SSL_3_1
#define SSL_VER_TLS_1_1 SSL_VER_SSL_3_2
#define SSL_VER_TLS_1_2 SSL_VER_SSL_3_3

#endif

/** @def TLS_HANDSHAKE_XXX
 *
 * @brief SSL/TLS鎻℃墜杩斿洖鍊笺��
 * @details XXX鍙互鏄疐AIL锛堟彙鎵嬪け璐ワ級锛孋ONTINUE锛堢户缁級锛孫K锛堟彙鎵嬪畬鎴愶級銆�
 *
 */
#define TLS_HANDSHAKE_FAIL (-1)
#define TLS_HANDSHAKE_CONTINUE (1)
#define TLS_HANDSHAKE_OK (0)

/*
 * Supported ciphersuites (Official IANA names)
 */
#define TLS_RSA_WITH_NULL_MD5                    0x01   /**< Weak! */
#define TLS_RSA_WITH_NULL_SHA                    0x02   /**< Weak! */

#define TLS_RSA_WITH_RC4_128_MD5                 0x04
#define TLS_RSA_WITH_RC4_128_SHA                 0x05
#define TLS_RSA_WITH_DES_CBC_SHA                 0x09   /**< Weak! Not in TLS 1.2 */

#define TLS_RSA_WITH_3DES_EDE_CBC_SHA            0x0A

#define TLS_DHE_RSA_WITH_DES_CBC_SHA             0x15   /**< Weak! Not in TLS 1.2 */
#define TLS_DHE_RSA_WITH_3DES_EDE_CBC_SHA        0x16

#define TLS_PSK_WITH_NULL_SHA                    0x2C   /**< Weak! */
#define TLS_DHE_PSK_WITH_NULL_SHA                0x2D   /**< Weak! */
#define TLS_RSA_PSK_WITH_NULL_SHA                0x2E   /**< Weak! */
#define TLS_RSA_WITH_AES_128_CBC_SHA             0x2F

#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA         0x33
#define TLS_RSA_WITH_AES_256_CBC_SHA             0x35
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA         0x39

#define TLS_RSA_WITH_NULL_SHA256                 0x3B   /**< Weak! */
#define TLS_RSA_WITH_AES_128_CBC_SHA256          0x3C   /**< TLS 1.2 */
#define TLS_RSA_WITH_AES_256_CBC_SHA256          0x3D   /**< TLS 1.2 */

#define TLS_RSA_WITH_CAMELLIA_128_CBC_SHA        0x41
#define TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA    0x45

#define TLS_DHE_RSA_WITH_AES_128_CBC_SHA256      0x67   /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_AES_256_CBC_SHA256      0x6B   /**< TLS 1.2 */

#define TLS_RSA_WITH_CAMELLIA_256_CBC_SHA        0x84
#define TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA    0x88

#define TLS_PSK_WITH_RC4_128_SHA                 0x8A
#define TLS_PSK_WITH_3DES_EDE_CBC_SHA            0x8B
#define TLS_PSK_WITH_AES_128_CBC_SHA             0x8C
#define TLS_PSK_WITH_AES_256_CBC_SHA             0x8D

#define TLS_DHE_PSK_WITH_RC4_128_SHA             0x8E
#define TLS_DHE_PSK_WITH_3DES_EDE_CBC_SHA        0x8F
#define TLS_DHE_PSK_WITH_AES_128_CBC_SHA         0x90
#define TLS_DHE_PSK_WITH_AES_256_CBC_SHA         0x91

#define TLS_RSA_PSK_WITH_RC4_128_SHA             0x92
#define TLS_RSA_PSK_WITH_3DES_EDE_CBC_SHA        0x93
#define TLS_RSA_PSK_WITH_AES_128_CBC_SHA         0x94
#define TLS_RSA_PSK_WITH_AES_256_CBC_SHA         0x95

#define TLS_RSA_WITH_AES_128_GCM_SHA256          0x9C   /**< TLS 1.2 */
#define TLS_RSA_WITH_AES_256_GCM_SHA384          0x9D   /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_AES_128_GCM_SHA256      0x9E   /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_AES_256_GCM_SHA384      0x9F   /**< TLS 1.2 */

#define TLS_PSK_WITH_AES_128_GCM_SHA256          0xA8   /**< TLS 1.2 */
#define TLS_PSK_WITH_AES_256_GCM_SHA384          0xA9   /**< TLS 1.2 */
#define TLS_DHE_PSK_WITH_AES_128_GCM_SHA256      0xAA   /**< TLS 1.2 */
#define TLS_DHE_PSK_WITH_AES_256_GCM_SHA384      0xAB   /**< TLS 1.2 */
#define TLS_RSA_PSK_WITH_AES_128_GCM_SHA256      0xAC   /**< TLS 1.2 */
#define TLS_RSA_PSK_WITH_AES_256_GCM_SHA384      0xAD   /**< TLS 1.2 */

#define TLS_PSK_WITH_AES_128_CBC_SHA256          0xAE
#define TLS_PSK_WITH_AES_256_CBC_SHA384          0xAF
#define TLS_PSK_WITH_NULL_SHA256                 0xB0   /**< Weak! */
#define TLS_PSK_WITH_NULL_SHA384                 0xB1   /**< Weak! */

#define TLS_DHE_PSK_WITH_AES_128_CBC_SHA256      0xB2
#define TLS_DHE_PSK_WITH_AES_256_CBC_SHA384      0xB3
#define TLS_DHE_PSK_WITH_NULL_SHA256             0xB4   /**< Weak! */
#define TLS_DHE_PSK_WITH_NULL_SHA384             0xB5   /**< Weak! */

#define TLS_RSA_PSK_WITH_AES_128_CBC_SHA256      0xB6
#define TLS_RSA_PSK_WITH_AES_256_CBC_SHA384      0xB7
#define TLS_RSA_PSK_WITH_NULL_SHA256             0xB8   /**< Weak! */
#define TLS_RSA_PSK_WITH_NULL_SHA384             0xB9   /**< Weak! */

#define TLS_RSA_WITH_CAMELLIA_128_CBC_SHA256     0xBA   /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_CAMELLIA_128_CBC_SHA256 0xBE   /**< TLS 1.2 */

#define TLS_RSA_WITH_CAMELLIA_256_CBC_SHA256     0xC0   /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_CAMELLIA_256_CBC_SHA256 0xC4   /**< TLS 1.2 */

#define TLS_ECDH_ECDSA_WITH_NULL_SHA             0xC001 /**< Weak! */
#define TLS_ECDH_ECDSA_WITH_RC4_128_SHA          0xC002 /**< Not in SSL3! */
#define TLS_ECDH_ECDSA_WITH_3DES_EDE_CBC_SHA     0xC003 /**< Not in SSL3! */
#define TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA      0xC004 /**< Not in SSL3! */
#define TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA      0xC005 /**< Not in SSL3! */

#define TLS_ECDHE_ECDSA_WITH_NULL_SHA            0xC006 /**< Weak! */
#define TLS_ECDHE_ECDSA_WITH_RC4_128_SHA         0xC007 /**< Not in SSL3! */
#define TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA    0xC008 /**< Not in SSL3! */
#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA     0xC009 /**< Not in SSL3! */
#define TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA     0xC00A /**< Not in SSL3! */

#define TLS_ECDH_RSA_WITH_NULL_SHA               0xC00B /**< Weak! */
#define TLS_ECDH_RSA_WITH_RC4_128_SHA            0xC00C /**< Not in SSL3! */
#define TLS_ECDH_RSA_WITH_3DES_EDE_CBC_SHA       0xC00D /**< Not in SSL3! */
#define TLS_ECDH_RSA_WITH_AES_128_CBC_SHA        0xC00E /**< Not in SSL3! */
#define TLS_ECDH_RSA_WITH_AES_256_CBC_SHA        0xC00F /**< Not in SSL3! */

#define TLS_ECDHE_RSA_WITH_NULL_SHA              0xC010 /**< Weak! */
#define TLS_ECDHE_RSA_WITH_RC4_128_SHA           0xC011 /**< Not in SSL3! */
#define TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA      0xC012 /**< Not in SSL3! */
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA       0xC013 /**< Not in SSL3! */
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA       0xC014 /**< Not in SSL3! */

#define TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256  0xC023 /**< TLS 1.2 */
#define TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384  0xC024 /**< TLS 1.2 */
#define TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256   0xC025 /**< TLS 1.2 */
#define TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384   0xC026 /**< TLS 1.2 */
#define TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256    0xC027 /**< TLS 1.2 */
#define TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384    0xC028 /**< TLS 1.2 */
#define TLS_ECDH_RSA_WITH_AES_128_CBC_SHA256     0xC029 /**< TLS 1.2 */
#define TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384     0xC02A /**< TLS 1.2 */

#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256  0xC02B /**< TLS 1.2 */
#define TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384  0xC02C /**< TLS 1.2 */
#define TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256   0xC02D /**< TLS 1.2 */
#define TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384   0xC02E /**< TLS 1.2 */
#define TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256    0xC02F /**< TLS 1.2 */
#define TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384    0xC030 /**< TLS 1.2 */
#define TLS_ECDH_RSA_WITH_AES_128_GCM_SHA256     0xC031 /**< TLS 1.2 */
#define TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384     0xC032 /**< TLS 1.2 */

#define TLS_ECDHE_PSK_WITH_RC4_128_SHA           0xC033 /**< Not in SSL3! */
#define TLS_ECDHE_PSK_WITH_3DES_EDE_CBC_SHA      0xC034 /**< Not in SSL3! */
#define TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA       0xC035 /**< Not in SSL3! */
#define TLS_ECDHE_PSK_WITH_AES_256_CBC_SHA       0xC036 /**< Not in SSL3! */
#define TLS_ECDHE_PSK_WITH_AES_128_CBC_SHA256    0xC037 /**< Not in SSL3! */
#define TLS_ECDHE_PSK_WITH_AES_256_CBC_SHA384    0xC038 /**< Not in SSL3! */
#define TLS_ECDHE_PSK_WITH_NULL_SHA              0xC039 /**< Weak! No SSL3! */
#define TLS_ECDHE_PSK_WITH_NULL_SHA256           0xC03A /**< Weak! No SSL3! */
#define TLS_ECDHE_PSK_WITH_NULL_SHA384           0xC03B /**< Weak! No SSL3! */

#define TLS_ECDHE_ECDSA_WITH_CAMELLIA_128_CBC_SHA256 0xC072 /**< Not in SSL3! */
#define TLS_ECDHE_ECDSA_WITH_CAMELLIA_256_CBC_SHA384 0xC073 /**< Not in SSL3! */
#define TLS_ECDH_ECDSA_WITH_CAMELLIA_128_CBC_SHA256  0xC074 /**< Not in SSL3! */
#define TLS_ECDH_ECDSA_WITH_CAMELLIA_256_CBC_SHA384  0xC075 /**< Not in SSL3! */
#define TLS_ECDHE_RSA_WITH_CAMELLIA_128_CBC_SHA256   0xC076 /**< Not in SSL3! */
#define TLS_ECDHE_RSA_WITH_CAMELLIA_256_CBC_SHA384   0xC077 /**< Not in SSL3! */
#define TLS_ECDH_RSA_WITH_CAMELLIA_128_CBC_SHA256    0xC078 /**< Not in SSL3! */
#define TLS_ECDH_RSA_WITH_CAMELLIA_256_CBC_SHA384    0xC079 /**< Not in SSL3! */

#define TLS_RSA_WITH_CAMELLIA_128_GCM_SHA256         0xC07A /**< TLS 1.2 */
#define TLS_RSA_WITH_CAMELLIA_256_GCM_SHA384         0xC07B /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_CAMELLIA_128_GCM_SHA256     0xC07C /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_CAMELLIA_256_GCM_SHA384     0xC07D /**< TLS 1.2 */
#define TLS_ECDHE_ECDSA_WITH_CAMELLIA_128_GCM_SHA256 0xC086 /**< TLS 1.2 */
#define TLS_ECDHE_ECDSA_WITH_CAMELLIA_256_GCM_SHA384 0xC087 /**< TLS 1.2 */
#define TLS_ECDH_ECDSA_WITH_CAMELLIA_128_GCM_SHA256  0xC088 /**< TLS 1.2 */
#define TLS_ECDH_ECDSA_WITH_CAMELLIA_256_GCM_SHA384  0xC089 /**< TLS 1.2 */
#define TLS_ECDHE_RSA_WITH_CAMELLIA_128_GCM_SHA256   0xC08A /**< TLS 1.2 */
#define TLS_ECDHE_RSA_WITH_CAMELLIA_256_GCM_SHA384   0xC08B /**< TLS 1.2 */
#define TLS_ECDH_RSA_WITH_CAMELLIA_128_GCM_SHA256    0xC08C /**< TLS 1.2 */
#define TLS_ECDH_RSA_WITH_CAMELLIA_256_GCM_SHA384    0xC08D /**< TLS 1.2 */

#define TLS_PSK_WITH_CAMELLIA_128_GCM_SHA256       0xC08E /**< TLS 1.2 */
#define TLS_PSK_WITH_CAMELLIA_256_GCM_SHA384       0xC08F /**< TLS 1.2 */
#define TLS_DHE_PSK_WITH_CAMELLIA_128_GCM_SHA256   0xC090 /**< TLS 1.2 */
#define TLS_DHE_PSK_WITH_CAMELLIA_256_GCM_SHA384   0xC091 /**< TLS 1.2 */
#define TLS_RSA_PSK_WITH_CAMELLIA_128_GCM_SHA256   0xC092 /**< TLS 1.2 */
#define TLS_RSA_PSK_WITH_CAMELLIA_256_GCM_SHA384   0xC093 /**< TLS 1.2 */

#define TLS_PSK_WITH_CAMELLIA_128_CBC_SHA256       0xC094
#define TLS_PSK_WITH_CAMELLIA_256_CBC_SHA384       0xC095
#define TLS_DHE_PSK_WITH_CAMELLIA_128_CBC_SHA256   0xC096
#define TLS_DHE_PSK_WITH_CAMELLIA_256_CBC_SHA384   0xC097
#define TLS_RSA_PSK_WITH_CAMELLIA_128_CBC_SHA256   0xC098
#define TLS_RSA_PSK_WITH_CAMELLIA_256_CBC_SHA384   0xC099
#define TLS_ECDHE_PSK_WITH_CAMELLIA_128_CBC_SHA256 0xC09A /**< Not in SSL3! */
#define TLS_ECDHE_PSK_WITH_CAMELLIA_256_CBC_SHA384 0xC09B /**< Not in SSL3! */

#define TLS_RSA_WITH_AES_128_CCM                0xC09C  /**< TLS 1.2 */
#define TLS_RSA_WITH_AES_256_CCM                0xC09D  /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_AES_128_CCM            0xC09E  /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_AES_256_CCM            0xC09F  /**< TLS 1.2 */
#define TLS_RSA_WITH_AES_128_CCM_8              0xC0A0  /**< TLS 1.2 */
#define TLS_RSA_WITH_AES_256_CCM_8              0xC0A1  /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_AES_128_CCM_8          0xC0A2  /**< TLS 1.2 */
#define TLS_DHE_RSA_WITH_AES_256_CCM_8          0xC0A3  /**< TLS 1.2 */
#define TLS_PSK_WITH_AES_128_CCM                0xC0A4  /**< TLS 1.2 */
#define TLS_PSK_WITH_AES_256_CCM                0xC0A5  /**< TLS 1.2 */
#define TLS_DHE_PSK_WITH_AES_128_CCM            0xC0A6  /**< TLS 1.2 */
#define TLS_DHE_PSK_WITH_AES_256_CCM            0xC0A7  /**< TLS 1.2 */
#define TLS_PSK_WITH_AES_128_CCM_8              0xC0A8  /**< TLS 1.2 */
#define TLS_PSK_WITH_AES_256_CCM_8              0xC0A9  /**< TLS 1.2 */
#define TLS_DHE_PSK_WITH_AES_128_CCM_8          0xC0AA  /**< TLS 1.2 */
#define TLS_DHE_PSK_WITH_AES_256_CCM_8          0xC0AB  /**< TLS 1.2 */
/* The last two are named with PSK_DHE in the RFC, which looks like a typo */

#define TLS_ECDHE_ECDSA_WITH_AES_128_CCM        0xC0AC  /**< TLS 1.2 */
#define TLS_ECDHE_ECDSA_WITH_AES_256_CCM        0xC0AD  /**< TLS 1.2 */
#define TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8      0xC0AE  /**< TLS 1.2 */
#define TLS_ECDHE_ECDSA_WITH_AES_256_CCM_8      0xC0AF  /**< TLS 1.2 */

#define TLS_ECJPAKE_WITH_AES_128_CCM_8          0xC0FF  /**< experimental */

/**
 * @brief 鍒涘缓SSL閰嶇疆鍙傛暟銆�
 *
 * @param end_point SSL绔偣绫诲瀷锛孲SL_IS_CLIENT/SSL_IS_SERVER
 * @param transport SSL浼犺緭绫诲瀷锛孲SL_TRANSPORT_STREAM/SSL_TRANSPORT_DATAGRAM銆�
 *
 * @return 鎴愬姛杩斿洖閰嶇疆鍙傛暟鎸囬拡锛屽け璐ヨ繑鍥濽H_FAIL銆�
 */
extern void * uh_tls_conf_init(int end_point, int transport);

/**
 * @brief 璁剧疆SSL鏀寔鐨勬渶灏忕増鏈��
 *
 * @param conf 鐢眜h_tls_conf_init鍒涘缓鐨勯厤缃弬鏁般��
 * @param ssl_ver SSL鐗堟湰锛孲SL_VER_XXX銆�
 *
 * @return N/A銆�
 */
extern void uh_tls_conf_set_min_version(void *conf, int ssl_ver);

/**
 * @brief 璁剧疆SSL鏀寔鐨勫姞瀵嗗浠躲��
 * @details 鍙�夎缃紝涓嶈缃敮鎸佹墍鏈夈��
 *
 * @param conf 鐢眜h_tls_conf_init鍒涘缓鐨勯厤缃弬鏁般��
 * @param ciphersuite_list 鏀寔鐨勫姞瀵嗗浠跺垪琛ㄣ��
 *
 * @return N/A銆�
 */
extern void uh_tls_conf_set_ciphersuite_list(void *conf, int *ciphersuite_list);

/**
 * @brief 璁剧疆SSL楠岃瘉妯″紡銆�
 *
 * @param conf 鐢眜h_tls_conf_init鍒涘缓鐨勯厤缃弬鏁般��
 * @param stack_size SSL楠岃瘉妯″紡锛孲SL_AUTH_MODE_VERIFY/SSL_AUTH_MODE_NONE銆�
 *
 * @return N/A銆�
 */
extern void uh_tls_conf_set_auth_mode(void *conf, int auth_mode);

/**
 * @brief 璁剧疆SSL鏍硅瘉涔︺��
 *
 * @param conf 鐢眜h_tls_conf_init鍒涘缓鐨勯厤缃弬鏁般��
 * @param ca_buf 璇佷功锛屾敮鎸丳EM鍜孌ER銆�
 * @param ca_buf_len 璇佷功闀垮害銆�
 *
 * @return 鎴愬姛杩斿洖UH_OK锛屽け璐ヨ繑鍥濽H_FAIL銆�
 */
extern int uh_tls_conf_set_ca(void *conf, const void *ca_buf, unsigned int ca_buf_len);

extern void uh_tls_conf_set_free_crt(void *conf, int free_crt_mem_when_handshake_ok);

/**
 * @brief 閲婃斁SSL閰嶇疆鍙傛暟銆�
 * @note 濡傛灉SSL閰嶇疆鍙傛暟宸茬粡鎴愬姛鍏宠仈鍒癝SL涓婁笅鏂囷紙uh_tls_create杩斿洖UH_OK锛夛紝
 * 涓嶉渶瑕佽皟鐢╱h_tls_conf_free銆傚惁鍒欏繀椤昏皟鐢╱h_tls_conf_free銆�
 *
 * @param conf 鐢眜h_tls_conf_init鍒涘缓鐨勯厤缃弬鏁般��
 *
 * @return N/A銆�
 */
extern void uh_tls_conf_free(void *conf);

/**
 * @brief 鍒涘缓SSL涓婁笅鏂囥��
 *
 * @param conf SSL閰嶇疆鍙傛暟銆�
 * @param fd socket鏂囦欢鎻忚堪绗︺��
 *
 * @return 鎴愬姛杩斿洖SSL涓婁笅鏂囷紝澶辫触杩斿洖NULL銆�
 */
extern void * uh_tls_create(void *conf, int fd);

/**
 * @brief SSL鎻℃墜銆�
 *
 * @param ssl_ctx 鐢眜h_tls_create鍒涘缓鐨凷SL涓婁笅鏂囥��
 *
 * @return 鎴愬姛杩斿洖TLS_HANDSHAKE_OK锛屽け璐ヨ繑鍥濼LS_HANDSHAKE_FAIL锛岀户缁繑鍥濼LS_HANDSHAKE_CONTINUE銆�
 */
extern int uh_tls_handshake(void *ssl_ctx);

/**
 * @brief 鍒犻櫎SSL涓婁笅鏂囥��
 *
 * @param ssl_ctx 鐢眜h_tls_create鍒涘缓鐨凷SL涓婁笅鏂囥��
 *
 * @return N/A銆�
 */
extern void uh_tls_free(void *ssl_ctx);

/**
 * @brief 浠嶴SL涓婁笅鏂囪鍙栨暟鎹��
 *
 * @param ssl_ctx 鐢眜h_tls_create鍒涘缓鐨凷SL涓婁笅鏂囥��
 * @param pbuf 瀛樻斁寰呰鍙栨暟鎹殑缂撳瓨鎸囬拡銆�
 * @param size 缂撳瓨澶у皬銆�
 *
 * @return 鎴愬姛杩斿洖璇诲彇鍒扮殑鏁版嵁闀垮害锛屽け璐ヨ繑鍥濽H_FAIL銆�
 */
extern int uh_tls_read(void *ssl_ctx, uint8_t *buf, unsigned int len);

/**
 * @brief 鍚慡SL涓婁笅鏂囧啓鍏ユ暟鎹��
 *
 * @param ssl_ctx 鐢眜h_tls_create鍒涘缓鐨凷SL涓婁笅鏂囥��
 * @param buf 瀛樻斁寰呭啓鍏ユ暟鎹殑缂撳瓨鎸囬拡銆�
 * @param len 缂撳瓨鏁版嵁澶у皬
 *
 * @return 鎴愬姛杩斿洖瀹為檯鍐欏叆鐨勬暟鎹暱搴︼紝澶辫触杩斿洖UH_FAIL銆�
 */
extern int uh_tls_write(void *ssl_ctx, const uint8_t *buf, unsigned int len);

/**
 * @brief 鑾峰彇SSL涓婁笅鏂囦腑鏈鍙栫殑鏁版嵁闀垮害銆�
 *
 * @param ssl_ctx 鐢眜h_tls_create鍒涘缓鐨凷SL涓婁笅鏂囥��
 *
 * @return 鏈鍙栫殑鏁版嵁闀垮害銆�
 */
extern unsigned int uh_tls_get_avail_bytes(void *ssl_ctx);


#ifdef __cplusplus
}
#endif

#endif /*__UH_TLS_H__*/

