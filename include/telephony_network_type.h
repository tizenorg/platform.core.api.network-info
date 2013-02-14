/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 

#ifndef __TIZEN_TELEPHONY_NETWORK_INFO_TYPE_H__
#define __TIZEN_TELEPHONY_NETWORK_INFO_TYPE_H__

#ifdef __cplusplus
 extern "C" {
#endif


/**
 * @file telephony_network_type.h
 * @brief This file contains the enumeration related with telephony network.
 */

/**
 * @addtogroup CAPI_TELEPHONY_NETWORK_INFO_MODULE
 * @{
 */

/**
 * @brief
 * Enumerations for telephony error code.
 */
typedef enum
{
	NETWORK_INFO_ERROR_NONE = TIZEN_ERROR_NONE,	/**< Successful */
	NETWORK_INFO_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	NETWORK_INFO_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */	
	NETWORK_INFO_ERROR_OPERATION_FAILED = TIZEN_ERROR_TELEPHONY_CLASS | 0x2000, /**< Operation failed */	
	NETWORK_INFO_ERROR_OUT_OF_SERVICE = TIZEN_ERROR_TELEPHONY_CLASS | 0x2001, /**< Out of service */		
} network_info_error_e;


/**
 * @brief Enumerations for RSSI (Receive Signal Strength Indicator).
 * @details #NETWORK_INFO_RSSI_6 is the highest.
 */
typedef enum
{
   NETWORK_INFO_RSSI_0 = 0x00, /**< Strength 0 */
   NETWORK_INFO_RSSI_1,        /**< Strength 1 */
   NETWORK_INFO_RSSI_2,        /**< Strength 2 */
   NETWORK_INFO_RSSI_3,        /**< Strength 3 */
   NETWORK_INFO_RSSI_4,        /**< Strength 4 */
   NETWORK_INFO_RSSI_5,        /**< Strength 5 */
   NETWORK_INFO_RSSI_6		   /**< Strength 6 */
} network_info_rssi_e;


/**
 * @brief Enumerations for network type.
 */
typedef enum
{
	NETWORK_INFO_TYPE_UNKNOWN = 0x00,	/**< Unknown */
	NETWORK_INFO_TYPE_GSM,	/**< 2G GSM network type */
	NETWORK_INFO_TYPE_GPRS,	/**< 2.5G GPRS network type */
	NETWORK_INFO_TYPE_EDGE,	/**< 2.5G EDGE network type */
	NETWORK_INFO_TYPE_UMTS,	/**< 3G UMTS network type */
	NETWORK_INFO_TYPE_HSDPA,	/**< HSDPA network type */
} network_info_type_e;


/**
 * @brief Enumeration for network state.
 */
typedef enum
{
    NETWORK_INFO_SERVICE_STATE_IN_SERVICE = 0x00,	/**< In service */
    NETWORK_INFO_SERVICE_STATE_OUT_OF_SERVICE,	/**< Out of service */
    NETWORK_INFO_SERVICE_STATE_EMERGENCY_ONLY,	/**< Only emergency call is allowed */
    NETWORK_INFO_SERVICE_STATE_RADIO_OFF,	/**< Power of radio is turned off */
} network_info_service_state_e;


#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif	// __TIZEN_TELEPHONY_NETWORK_INFO_TYPE_H__

