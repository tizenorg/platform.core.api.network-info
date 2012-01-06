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



#include <stdio.h>
#include <string.h>
#include <telephony_network.h>
#include <dlog.h>
#include <glib.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "TIZEN_N_NETWORK_INFO_TEST"

static GMainLoop *event_loop;

char* convert_network_service_state_to_string(network_info_service_state_e network_service_state)
{
	switch(network_service_state)
	{
		case NETWORK_INFO_SERVICE_STATE_IN_SERVICE:
			return "In service";
		case NETWORK_INFO_SERVICE_STATE_OUT_OF_SERVICE:
			return "Out of service";
		case NETWORK_INFO_SERVICE_STATE_EMERGENCY_ONLY:
			return "Emergency only";
		case NETWORK_INFO_SERVICE_STATE_RADIO_OFF:
			return "Radio off";
		default:
			return "unexpected";
	}	
}

void state_changed_cb(network_info_service_state_e status, void* user_data)
{
	LOGI("[%s] Start telephony_service_changed_cb", __FUNCTION__);

	LOGI("[%s] Service status: %s", __FUNCTION__, convert_network_service_state_to_string(status));
	LOGI("[%s] user data: %s", __FUNCTION__, user_data);

	LOGI("[%s] End telephony_service_changed_cb", __FUNCTION__);	
	g_main_loop_quit(event_loop);
}

int main()
{
	if( network_info_set_service_state_changed_cb(state_changed_cb, "telephony_service_changed_test") == NETWORK_INFO_ERROR_NONE )
	{
		LOGI("[%s] Succeeded to add callback function", __FUNCTION__);
	}
	else
	{
		LOGE("[%s] Failed to add callback function", __FUNCTION__);
		return -1;
	}

	LOGI("[%s] If service status is changed, then callback function will be called", __FUNCTION__);
	event_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(event_loop);

	if( network_info_unset_service_state_changed_cb() == NETWORK_INFO_ERROR_NONE )
	{
		LOGI("[%s] Succeeded to remove callback function", __FUNCTION__);
	}
	else
	{
		LOGE("[%s] Failed to remove callback function", __FUNCTION__);
		return -1;
	}

	return 0;
}
