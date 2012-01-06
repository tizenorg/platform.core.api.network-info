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

void rssi_changed_cb_impl(network_info_rssi_e rssi, void* user_data)
{
	LOGI("[%s] Start rssi_changed_cb_impl", __FUNCTION__);

	LOGI("[%s] RSSI: %d", __FUNCTION__, rssi);
	LOGI("[%s] user data: %s", __FUNCTION__, user_data);

	LOGI("[%s] End rssi_changed_cb_impl", __FUNCTION__);	
	g_main_loop_quit(event_loop);
}

int main()
{
	if( network_info_set_rssi_changed_cb(rssi_changed_cb_impl, "rssi_changed_test") == NETWORK_INFO_ERROR_NONE )
	{
		LOGI("[%s] Succeeded to add callback function", __FUNCTION__);
	}
	else
	{
		LOGE("[%s] Failed to add callback function", __FUNCTION__);
		return -1;
	}

	LOGI("[%s] If RSSI is changed, then callback function will be called", __FUNCTION__);
	event_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(event_loop);

	if( network_info_unset_rssi_changed_cb() == NETWORK_INFO_ERROR_NONE )
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
