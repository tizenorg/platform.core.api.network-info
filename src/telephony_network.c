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


#include <telephony_network.h>
#include <vconf-keys.h>
#include <vconf.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <dlog.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "CAPI_TELEPHONY_NETWORK_INFO"

// whether vconf_notify_key_changed is registered or not  
static bool flight_mode_is_registered = false;
static bool svctype_is_registered = false;
static bool svc_cs_is_registered = false;
static bool cell_id_is_registered = false;
static bool rssi_is_registered = false;
static bool roaming_is_registered = false;

typedef struct _telephony_cb_data
{
	int previous_value;
	const void* cb;
	void* user_data;
} telephony_cb_data;

// Callback function data
static telephony_cb_data service_state_cb = {NETWORK_INFO_SERVICE_STATE_OUT_OF_SERVICE, NULL, NULL};
static telephony_cb_data cell_id_cb = {0, NULL, NULL};
static telephony_cb_data rssi_cb = {NETWORK_INFO_RSSI_0, NULL, NULL};
static telephony_cb_data roaming_cb = {false, NULL, NULL};

// Callback function adapter
static void __telephony_service_changed_cb_adapter(keynode_t *node, void* user_data);
static void __cell_id_changed_cb_adapter(keynode_t *node, void* user_data);
static void __rssi_changed_cb_adapter(keynode_t *node, void* user_data);
static void __roaming_changed_cb_adapter(keynode_t *node, void* user_data);
static char* __convert_error_code_to_string(network_info_error_e error_code);
static int __check_service_state(char* function_name);

// Internal Macros
#define NETWORK_INFO_CHECK_INPUT_PARAMETER(arg) \
	if( arg == NULL ) \
	{ \
		LOGE("[%s] INVALID_PARAMETER(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_INVALID_PARAMETER); \
		return NETWORK_INFO_ERROR_INVALID_PARAMETER; \
	}
	

int network_info_get_lac(int* lac)
{
	int ret = NETWORK_INFO_ERROR_NONE;

	NETWORK_INFO_CHECK_INPUT_PARAMETER(lac);

	ret = __check_service_state((char*)__FUNCTION__);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		return ret;
	}

	if( vconf_get_int(VCONFKEY_TELEPHONY_LAC, lac) != 0 )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	return NETWORK_INFO_ERROR_NONE;
}


int network_info_get_cell_id(int* cell_id)
{
	int ret = NETWORK_INFO_ERROR_NONE;
	
	NETWORK_INFO_CHECK_INPUT_PARAMETER(cell_id);
	
	ret = __check_service_state((char*)__FUNCTION__);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		return ret;
	}

	if( vconf_get_int(VCONFKEY_TELEPHONY_CELLID, cell_id) != 0 ) 
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	return NETWORK_INFO_ERROR_NONE;
}


int network_info_get_rssi(network_info_rssi_e* rssi)
{
	int ret = NETWORK_INFO_ERROR_NONE;
	
	NETWORK_INFO_CHECK_INPUT_PARAMETER(rssi);
	
	ret = __check_service_state((char*)__FUNCTION__);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		return ret;
	}

	if( vconf_get_int(VCONFKEY_TELEPHONY_RSSI, (int *)rssi) != 0 )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	return NETWORK_INFO_ERROR_NONE;
}


int network_info_is_roaming(bool* is_roaming)
{
	int roaming_state = 0;
	int ret = NETWORK_INFO_ERROR_NONE;
	
	NETWORK_INFO_CHECK_INPUT_PARAMETER(is_roaming);
	
	ret = __check_service_state((char*)__FUNCTION__);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		return ret;
	}

	if( vconf_get_int(VCONFKEY_TELEPHONY_SVC_ROAM, &roaming_state) != 0 )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	if( roaming_state == VCONFKEY_TELEPHONY_SVC_ROAM_ON )
	{
		*is_roaming = true;
	}
	else
	{
		*is_roaming = false;
	}			

	return NETWORK_INFO_ERROR_NONE;
}


int network_info_get_mcc(char** mcc)
{
	char plmn_str[32] = "";
	int plmn_int = 0;
	int mcc_length = 3;
	int ret = NETWORK_INFO_ERROR_NONE;	

	NETWORK_INFO_CHECK_INPUT_PARAMETER(mcc);
	
	ret = __check_service_state((char*)__FUNCTION__);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		return ret;
	}

	if( vconf_get_int(VCONFKEY_TELEPHONY_PLMN, &plmn_int) != 0 )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	snprintf(plmn_str, 32, "%d", plmn_int);
	*mcc = (char*)malloc(sizeof(char) * (mcc_length+1));
	if( *mcc == NULL )
	{
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_OUT_OF_MEMORY);
		return NETWORK_INFO_ERROR_OUT_OF_MEMORY;
	}
	memset(*mcc, 0x00, mcc_length+1);
	strncpy(*mcc, plmn_str, mcc_length);

	return NETWORK_INFO_ERROR_NONE;
}


int network_info_get_mnc(char** mnc)
{
	char plmn_str[32] = "";
	int plmn_int = 0;
	int mnc_length = 3; //mcc_length can 2 or 3 depending on a network, so have to proper way to get the exact length of mcc
	int ret = NETWORK_INFO_ERROR_NONE;
	
	NETWORK_INFO_CHECK_INPUT_PARAMETER(mnc);
	
	ret = __check_service_state((char*)__FUNCTION__);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		return ret;
	}

	if( vconf_get_int(VCONFKEY_TELEPHONY_PLMN, &plmn_int) != 0 )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	snprintf(plmn_str, 32, "%d", plmn_int);
	*mnc = (char*)malloc(sizeof(char) * (mnc_length+1));
	if( *mnc == NULL )
	{
		LOGE("[%s] OUT_OF_MEMORY(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_OUT_OF_MEMORY);
		return NETWORK_INFO_ERROR_OUT_OF_MEMORY;
	}
	memset(*mnc, 0x00, mnc_length+1);                
	strncpy(*mnc, plmn_str+3, mnc_length);

	return NETWORK_INFO_ERROR_NONE;
}


int network_info_get_provider_name(char** provider_name)
{
	char* provider_name_p = NULL;
	int ret = NETWORK_INFO_ERROR_NONE;
	
	NETWORK_INFO_CHECK_INPUT_PARAMETER(provider_name);
	
	ret = __check_service_state((char*)__FUNCTION__);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		return ret;
	}

	provider_name_p = vconf_get_str(VCONFKEY_TELEPHONY_NWNAME);
	if( provider_name_p == NULL )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	*provider_name = provider_name_p;

	return NETWORK_INFO_ERROR_NONE;
}


int network_info_get_type(network_info_type_e* network_type)
{
	int service_type = 0;
	int ret = NETWORK_INFO_ERROR_NONE;
	
	NETWORK_INFO_CHECK_INPUT_PARAMETER(network_type);
	
	ret = __check_service_state((char*)__FUNCTION__);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		return ret;
	}

	if( vconf_get_int(VCONFKEY_TELEPHONY_SVCTYPE, &service_type) != 0 )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x)", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	switch(service_type)
	{
		case VCONFKEY_TELEPHONY_SVCTYPE_2G:
			*network_type = NETWORK_INFO_TYPE_GSM;
			break;
		case VCONFKEY_TELEPHONY_SVCTYPE_2_5G:
			*network_type = NETWORK_INFO_TYPE_GPRS;
			break;
		case VCONFKEY_TELEPHONY_SVCTYPE_2_5G_EDGE:
			*network_type = NETWORK_INFO_TYPE_EDGE;
			break;
		case VCONFKEY_TELEPHONY_SVCTYPE_3G:
			*network_type = NETWORK_INFO_TYPE_UMTS;;
			break;
		case VCONFKEY_TELEPHONY_SVCTYPE_HSDPA:
			*network_type = NETWORK_INFO_TYPE_HSDPA;
			break;
		default:
			*network_type = NETWORK_INFO_TYPE_UNKNOWN;
			break;			
	}

	return NETWORK_INFO_ERROR_NONE;
}


int network_info_get_service_state(network_info_service_state_e* network_service_state)
{
	int service_type = 0;
	int cs_status = 0;
	int is_flight_mode = 0;

	NETWORK_INFO_CHECK_INPUT_PARAMETER(network_service_state);
	
	// get service type	
	if( vconf_get_int(VCONFKEY_TELEPHONY_SVCTYPE, &service_type) != 0 )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to get service type", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	if( service_type == VCONFKEY_TELEPHONY_SVCTYPE_EMERGENCY )
	{
		*network_service_state = NETWORK_INFO_SERVICE_STATE_EMERGENCY_ONLY;
		return NETWORK_INFO_ERROR_NONE;
	}	

	// get circuit service	
	if( vconf_get_int(VCONFKEY_TELEPHONY_SVC_CS, &cs_status) != 0 )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to get the status of cs", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	if( cs_status == VCONFKEY_TELEPHONY_SVC_CS_ON )
	{
		*network_service_state = NETWORK_INFO_SERVICE_STATE_IN_SERVICE;
		return NETWORK_INFO_ERROR_NONE;		
	}	

	// get flight mode
	if( vconf_get_bool(VCONFKEY_TELEPHONY_FLIGHT_MODE, &is_flight_mode) != 0 )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to get flight mode", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	if( is_flight_mode == 1 )
	{
		*network_service_state = NETWORK_INFO_SERVICE_STATE_RADIO_OFF;
		return NETWORK_INFO_ERROR_NONE;	
	}

	*network_service_state = NETWORK_INFO_SERVICE_STATE_OUT_OF_SERVICE;
	return NETWORK_INFO_ERROR_NONE;
}

int network_info_set_service_state_changed_cb(network_info_service_state_changed_cb callback, void* user_data)
{
	int ret = NETWORK_INFO_ERROR_OPERATION_FAILED;
	network_info_service_state_e service_state = NETWORK_INFO_SERVICE_STATE_OUT_OF_SERVICE;

	NETWORK_INFO_CHECK_INPUT_PARAMETER(callback);

	ret = network_info_get_service_state(&service_state);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		LOGE("[%s] %s(0x%08x) : fail to get current service state", __FUNCTION__, __convert_error_code_to_string(ret), ret);
		return ret;
	}

	if( flight_mode_is_registered == false) 
	{
		if( vconf_notify_key_changed(VCONFKEY_TELEPHONY_FLIGHT_MODE, (vconf_callback_fn)__telephony_service_changed_cb_adapter, NULL) != 0 )
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to register callback of flight mode", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}
		flight_mode_is_registered = true;		
	}

	if( svctype_is_registered == false )
	{
		if(vconf_notify_key_changed(VCONFKEY_TELEPHONY_SVCTYPE, (vconf_callback_fn)__telephony_service_changed_cb_adapter, NULL) != 0)
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to register callback of service type", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}
		svctype_is_registered = true;
	}

	if( svc_cs_is_registered == false )
	{
		if(vconf_notify_key_changed(VCONFKEY_TELEPHONY_SVC_CS, (vconf_callback_fn)__telephony_service_changed_cb_adapter, NULL) != 0)
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to register callback of circuit service", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}
		svc_cs_is_registered = true;
	}

	service_state_cb.previous_value = service_state;
	service_state_cb.cb = callback;
	service_state_cb.user_data = user_data;

	return NETWORK_INFO_ERROR_NONE;
}

int network_info_unset_service_state_changed_cb()
{
	if( flight_mode_is_registered == true )
	{
		if( vconf_ignore_key_changed(VCONFKEY_TELEPHONY_FLIGHT_MODE, (vconf_callback_fn)__telephony_service_changed_cb_adapter) != 0 )
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to unregister callback of flight mode", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}
		flight_mode_is_registered = false;
	}

	if( svctype_is_registered == true )
	{
		if(vconf_ignore_key_changed(VCONFKEY_TELEPHONY_SVCTYPE, (vconf_callback_fn)__telephony_service_changed_cb_adapter) != 0)
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to unregister callback of service type", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}
		svctype_is_registered = false;
	}

	if( svc_cs_is_registered == true )
	{
		if(vconf_ignore_key_changed(VCONFKEY_TELEPHONY_SVC_CS, (vconf_callback_fn)__telephony_service_changed_cb_adapter) != 0)
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to unregister callback of circuit service", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}
		svc_cs_is_registered = false;
	}	

	service_state_cb.previous_value = NETWORK_INFO_SERVICE_STATE_OUT_OF_SERVICE;
	service_state_cb.cb = NULL;
	service_state_cb.user_data = NULL;

	return NETWORK_INFO_ERROR_NONE;	
}


int network_info_set_cell_id_changed_cb(network_info_cell_id_changed_cb callback, void* user_data)
{
	int cell_id = 0;
	int ret = NETWORK_INFO_ERROR_NONE;

	NETWORK_INFO_CHECK_INPUT_PARAMETER(callback);

	ret = network_info_get_cell_id(&cell_id);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		LOGE("[%s] %s(0x%08x) : fail to get current CELL ID", __FUNCTION__, __convert_error_code_to_string(ret), ret);
		return ret;
	}

	if( cell_id_is_registered == false) 
	{		
		if( vconf_notify_key_changed(VCONFKEY_TELEPHONY_CELLID, (vconf_callback_fn)__cell_id_changed_cb_adapter, NULL) != 0 )
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to register callback function", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}

		cell_id_is_registered = true;		
	}

	cell_id_cb.previous_value = cell_id;
	cell_id_cb.cb = callback;
	cell_id_cb.user_data = user_data;

	return NETWORK_INFO_ERROR_NONE;
}

int network_info_unset_cell_id_changed_cb()
{
	if( cell_id_is_registered == true )
	{
		if( vconf_ignore_key_changed(VCONFKEY_TELEPHONY_CELLID, (vconf_callback_fn)__cell_id_changed_cb_adapter) != 0 )
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to unregister callback function", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}

		cell_id_is_registered = false;
		cell_id_cb.previous_value = 0;
		cell_id_cb.cb = NULL;
		cell_id_cb.user_data = NULL;		
	}

	return NETWORK_INFO_ERROR_NONE;	
}

int network_info_set_rssi_changed_cb(network_info_rssi_changed_cb callback, void* user_data)
{
	network_info_rssi_e rssi = NETWORK_INFO_RSSI_0;
	int ret = NETWORK_INFO_ERROR_NONE;

	NETWORK_INFO_CHECK_INPUT_PARAMETER(callback);

	ret = network_info_get_rssi(&rssi);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		LOGE("[%s] %s(0x%08x) : fail to get current RSSI", __FUNCTION__, __convert_error_code_to_string(ret), ret);
		return ret;
	}

	if( rssi_is_registered == false) 
	{		
		if( vconf_notify_key_changed(VCONFKEY_TELEPHONY_RSSI, (vconf_callback_fn)__rssi_changed_cb_adapter, NULL) != 0 )
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to register callback function", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}

		rssi_is_registered = true;		
	}

	rssi_cb.previous_value = rssi;
	rssi_cb.cb = callback;
	rssi_cb.user_data = user_data;

	return NETWORK_INFO_ERROR_NONE;
}

int network_info_unset_rssi_changed_cb()
{
	if( rssi_is_registered == true )
	{
		if( vconf_ignore_key_changed(VCONFKEY_TELEPHONY_RSSI, (vconf_callback_fn)__rssi_changed_cb_adapter) != 0 )
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to unregister callback function", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}

		rssi_is_registered = false;
		rssi_cb.previous_value = NETWORK_INFO_RSSI_0;
		rssi_cb.cb = NULL;
		rssi_cb.user_data = NULL;		
	}

	return NETWORK_INFO_ERROR_NONE;	
}

int network_info_set_roaming_state_changed_cb(network_info_roaming_state_changed_cb callback, void* user_data)
{
	bool is_roaming = false;
	int ret = NETWORK_INFO_ERROR_NONE;

	NETWORK_INFO_CHECK_INPUT_PARAMETER(callback);

	ret = network_info_is_roaming(&is_roaming);
	if( ret != NETWORK_INFO_ERROR_NONE )
	{
		LOGE("[%s] %s(0x%08x) : fail to get current roaming state", __FUNCTION__, __convert_error_code_to_string(ret), ret);
		return ret;
	}

	if( roaming_is_registered == false) 
	{		
		if( vconf_notify_key_changed(VCONFKEY_TELEPHONY_SVC_ROAM, (vconf_callback_fn)__roaming_changed_cb_adapter, NULL) != 0 )
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to register callback function", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}

		roaming_is_registered = true;		
	}

	roaming_cb.previous_value = is_roaming;
	roaming_cb.cb = callback;
	roaming_cb.user_data = user_data;

	return NETWORK_INFO_ERROR_NONE;	
}

int network_info_unset_roaming_state_changed_cb()
{
	if( roaming_is_registered == true )
	{
		if( vconf_ignore_key_changed(VCONFKEY_TELEPHONY_SVC_ROAM, (vconf_callback_fn)__roaming_changed_cb_adapter) != 0 )
		{
			LOGE("[%s] OPERATION_FAILED(0x%08x) : fail to unregister callback function", __FUNCTION__, NETWORK_INFO_ERROR_OPERATION_FAILED);
			return NETWORK_INFO_ERROR_OPERATION_FAILED;
		}

		roaming_is_registered = false;
		roaming_cb.previous_value = false;
		roaming_cb.cb = NULL;
		roaming_cb.user_data = NULL;		
	}

	return NETWORK_INFO_ERROR_NONE;	
}

static void __telephony_service_changed_cb_adapter(keynode_t *node, void* user_data) 
{
	network_info_service_state_e status = NETWORK_INFO_SERVICE_STATE_OUT_OF_SERVICE;

	if( service_state_cb.cb == NULL )
	{
		return;
	}

	if( network_info_get_service_state(&status) == NETWORK_INFO_ERROR_NONE )
	{
		if( status != service_state_cb.previous_value )
		{
			((network_info_service_state_changed_cb)(service_state_cb.cb))(status, service_state_cb.user_data);
			service_state_cb.previous_value = status;			
		}
	}
}

static void __cell_id_changed_cb_adapter(keynode_t *node, void* user_data) 
{
	int cell_id = 0;

	if( cell_id_cb.cb == NULL )
	{
		LOGI("[%s] There is no registered callback", __FUNCTION__);
		return;
	}

	if( network_info_get_cell_id(&cell_id) == NETWORK_INFO_ERROR_NONE )
	{
		if( cell_id != cell_id_cb.previous_value )
		{
			LOGI("[%s] network_info_cell_id_changed_cb will be called", __FUNCTION__);
			((network_info_cell_id_changed_cb)(cell_id_cb.cb))(cell_id, cell_id_cb.user_data);
			cell_id_cb.previous_value = cell_id;			
		}
	}
}

static void __rssi_changed_cb_adapter(keynode_t *node, void* user_data) 
{
	network_info_rssi_e rssi = 0;

	if( rssi_cb.cb == NULL )
	{
		return;
	}

	if( network_info_get_rssi(&rssi) == NETWORK_INFO_ERROR_NONE )
	{
		if( rssi != rssi_cb.previous_value )
		{
			((network_info_rssi_changed_cb)(rssi_cb.cb))(rssi, rssi_cb.user_data);
			rssi_cb.previous_value = rssi;			
		}
	}
}

static void __roaming_changed_cb_adapter(keynode_t *node, void* user_data) 
{
	bool is_roaming = 0;

	if( roaming_cb.cb == NULL )
	{
		return;
	}

	if( network_info_is_roaming(&is_roaming) == NETWORK_INFO_ERROR_NONE )
	{
		if( is_roaming != roaming_cb.previous_value )
		{
			((network_info_roaming_state_changed_cb)(roaming_cb.cb))(is_roaming, roaming_cb.user_data);
			roaming_cb.previous_value = is_roaming;			
		}
	}
}


static char* __convert_error_code_to_string(network_info_error_e error_code)
{
	switch(error_code)
	{
		case NETWORK_INFO_ERROR_OUT_OF_MEMORY:
			return "OUT_OF_MEMORY";
		case NETWORK_INFO_ERROR_INVALID_PARAMETER:
			return "INVALID_PARAMETER";
		case NETWORK_INFO_ERROR_OPERATION_FAILED:
			return "OPERATION_FAILED";
		case NETWORK_INFO_ERROR_OUT_OF_SERVICE:
			return "OUT_OF_SERVICE";
		default:
			return "UNKNOWN";
	}
}

static int __check_service_state(char* function_name)
{
	network_info_service_state_e service_state = NETWORK_INFO_SERVICE_STATE_OUT_OF_SERVICE;

	if( network_info_get_service_state(&service_state) != NETWORK_INFO_ERROR_NONE )
	{
		LOGE("[%s] OPERATION_FAILED(0x%08x)", function_name, NETWORK_INFO_ERROR_OPERATION_FAILED);
		return NETWORK_INFO_ERROR_OPERATION_FAILED;
	}

	if( service_state != NETWORK_INFO_SERVICE_STATE_IN_SERVICE )
	{
		LOGE("[%s] OUT_OF_SERVICE(0x%08x)", function_name, NETWORK_INFO_ERROR_OUT_OF_SERVICE);
		return NETWORK_INFO_ERROR_OUT_OF_SERVICE;
	}

	return NETWORK_INFO_ERROR_NONE;
}



