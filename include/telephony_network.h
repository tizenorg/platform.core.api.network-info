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


#ifndef __TIZEN_TELEPHONY_NETWORK_INFO_H__
#define __TIZEN_TELEPHONY_NETWORK_INFO_H__

#include <stdbool.h>
#include <tizen.h>
#include <telephony_network_type.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file network_info.h
 * @brief This file contains the network information APIs.
 */

/**
 * @addtogroup CAPI_TELEPHONY_NETWORK_INFO_MODULE
 * @{
 */

/**
 * @brief Gets the LAC ( Location Area Code ) of current network.
 *
 * @param[out] lac The Location Area Code
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @pre  The Network service state must be #NETWORK_INFO_SERVICE_STATE_IN_SERVICE.
 * @see	network_info_get_service_state()
 *
 */
int network_info_get_lac(int *lac);

/**
 * @brief Gets the cell ID.
 * 
 * @param[out] cell_id The cell identification number
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @pre  The Network service state must be #NETWORK_INFO_SERVICE_STATE_IN_SERVICE.
 * @see	network_info_get_service_state()
 *
 */
int network_info_get_cell_id(int *cell_id);

/**
 * @brief Gets the RSSI (Received Signal Strength Indicator).
 *
 * @param[out] rssi The Received Signal Strength Indicator. \n 
 * The higher the received number is, the stronger the signal strength is.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @pre  The Network service state must be #NETWORK_INFO_SERVICE_STATE_IN_SERVICE.
 * @see	network_info_get_service_state()
 *
 */
int network_info_get_rssi(network_info_rssi_e *rssi);

/**
 * @brief Gets the roaming state.
 *
 * @param[out] is_roaming The roaming state: (@c true = roaming, @c false = not roaming)
 * @return 0 on success, otherwise a negative error value. 
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @pre  The Network service state must be #NETWORK_INFO_SERVICE_STATE_IN_SERVICE.
 * @see	network_info_get_service_state()
 *
 */
int network_info_is_roaming(bool *is_roaming);

/**
 * @brief Gets the MCC (Mobile Country Code) of current network. 
 *
 * @remarks @a mcc must be released with free() by you.
 *
 * @param[out] mcc The Mobile Country Code (three digits). \n
 * Mobile Country Code (MCC) identifies the country where the cell is being used.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service
 * @retval #NETWORK_INFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @pre  The Network service state must be #NETWORK_INFO_SERVICE_STATE_IN_SERVICE.
 * @see	network_info_get_service_state()
 *
 */
int network_info_get_mcc(char **mcc);

/**
 * @brief Gets the MNC (Mobile Network Code) of current network.
 *
 * @remarks @a mnc must be released with free() by you.
 *
 * @param[out] mnc The Mobile Network Code (three digits). \n
 * The Mobile Network Code (MNC) identifies the mobile phone operator and network provider.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service
 * @retval #NETWORK_INFO_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @pre The Network service state must be #NETWORK_INFO_SERVICE_STATE_IN_SERVICE.
 * @see	network_info_get_service_state()
 *
 */
int network_info_get_mnc(char **mnc);

/**
 * @brief Gets the name of the network provider.
 *
 * @remarks @a provider_name must be released with free() by you.
 *
 * @param[out] provider_name The name of the network provider
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @pre The Network service state must be #NETWORK_INFO_SERVICE_STATE_IN_SERVICE.
 * @see	network_info_get_service_state()
 *
 */
int network_info_get_provider_name(char **provider_name);

/**
 * @brief Gets the network type.
 *
 * @param[out] network_type The network type of telephony
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @pre The Network service state must be #NETWORK_INFO_SERVICE_STATE_IN_SERVICE.
 * @see	network_info_get_service_state()
 *
 */
int network_info_get_type(network_info_type_e *network_type);

/**
 * @brief Gets the network state of the telephony service. 
 *
 * @remarks To know the state of data network, call connection_get_network_status() in Connection Service.
 * @param[out] network_service_state The network state of call service
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int network_info_get_service_state(network_info_service_state_e *network_service_state);

/**
 * @brief  Called when the cell ID changes.
 * @remarks If the cell ID changes, then LAC, MCC, MNC, provider name and network type can be changed.
 * @param [in] cell_id The cell identification number to be changed
 * @param [in] user_data The user data passed from the callback registration function
 * @pre This callback function is invoked if you register this function using network_info_set_cell_id_changed_cb().
 * @see network_info_set_cell_id_changed_cb()
 * @see network_info_unset_cell_id_changed_cb()
 */
typedef void(* network_info_cell_id_changed_cb)(int cell_id, void *user_data);

/**
 * @brief Registers a callback function to be invoked when the cell ID changes. 
 *
 * @param [in] callback The callback function to register
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service 
 * @post network_info_cell_id_changed_cb() will be invoked. 
 * @see network_info_cell_id_changed_cb()
 * @see	network_info_unset_cell_id_changed_cb() 
 *
 */
int network_info_set_cell_id_changed_cb(network_info_cell_id_changed_cb callback, void *user_data);

/**
 * @brief Unregisters the callback function.
 * 
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Operation failed
 * @see network_info_cell_id_changed_cb()
 * @see	network_info_set_cell_id_changed_cb()
 *
 */
int network_info_unset_cell_id_changed_cb();

/**
 * @brief Invoked when the RSSI (Received Signal Strength Indicator) changes.
 * @param [in] rssi The Received Signal Strength Indicator
 * @param [in] user_data The user data passed from the callback registration function
 * @pre This callback function is invoked if you register this function using network_info_set_rssi_changed_cb().
 * @see network_info_set_rssi_changed_cb()
 * @see network_info_unset_rssi_changed_cb()
 */
typedef void(* network_info_rssi_changed_cb)(network_info_rssi_e rssi, void *user_data);

/**
 * @brief Registers a callback function to be invoked when RSSI (Received Signal Strength Indicator) changes. 
 *
 * @param [in] callback The callback function to register
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service 
 * @post network_info_rssi_changed_cb() will be invoked. 
 * @see network_info_rssi_changed_cb()
 * @see	network_info_unset_rssi_changed_cb()
 *
 */
int network_info_set_rssi_changed_cb(network_info_rssi_changed_cb callback, void *user_data);

/**
 * @brief Unregisters the callback function.
 * 
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Operation failed
 * @see network_info_rssi_changed_cb()
 * @see	network_info_set_rssi_changed_cb()
 *
 */
int network_info_unset_rssi_changed_cb();

/**
 * @brief Invoked when the roaming state changes.
 * @param [in] is_roaming The roaming state
 * @param [in] user_data The user data passed from the callback registration function
 * @pre This callback function is invoked if you register this function using network_info_set_roaming_state_changed_cb().
 * @see network_info_set_roaming_state_changed_cb()
 * @see network_info_unset_roaming_state_changed_cb()
 */
typedef void(* network_info_roaming_state_changed_cb)(bool is_roaming, void *user_data);

/**
 * @brief Registers a callback function to be invoked when roaming state changes. 
 *
 * @param [in] callback The callback function to register
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service 
 * @post network_info_roaming_state_changed_cb() will be invoked. 
 * @see network_info_roaming_state_changed_cb()
 * @see	network_info_unset_roaming_state_changed_cb()
 *
 */
int network_info_set_roaming_state_changed_cb(network_info_roaming_state_changed_cb callback, void *user_data);

/**
 * @brief Unregisters the callback function.
 * 
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Operation failed
 * @see network_info_roaming_state_changed_cb()
 * @see	network_info_set_roaming_changed_cb()
 *
 */
int network_info_unset_roaming_state_changed_cb();

/**
 * @brief Invoked when the state of the telephony service changes. 
 * @param [in] network_service_state The state of telephony service
 * @param [in] user_data The user data passed from the callback registration function
 * @pre This callback function is invoked if you register this function using network_info_set_service_state_changed_cb().
 * @see #network_info_service_state_e
 * @see network_info_set_service_state_changed_cb()
 * @see  network_info_unset_service_state_changed_cb()
 */
typedef void(* network_info_service_state_changed_cb)(network_info_service_state_e network_service_state, void *user_data);

/**
 * @brief Registers a callback function to be invoked when telephony service state changes.
 *
 * @param [in] callback The callback function to register
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Internal error
 * @retval #NETWORK_INFO_ERROR_OUT_OF_SERVICE Out of service 
 * @post network_info_service_state_changed_cb() will be invoked. 
 * @see	network_info_unset_service_state_changed_cb()
 */
int network_info_set_service_state_changed_cb(network_info_service_state_changed_cb callback, void *user_data);

/**
 * @brief Unregisters the callback function. 
 * 
 * @return 0 on success, otherwise a negative error value.
 * @retval #NETWORK_INFO_ERROR_NONE Successful
 * @retval #NETWORK_INFO_ERROR_OPERATION_FAILED Operation failed
 * @see network_info_service_state_changed_cb()
 * @see	network_info_set_service_state_changed_cb()
 */
int network_info_unset_service_state_changed_cb();


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif	// __TIZEN_TELEPHONY_NETWORK_INFO_H__
