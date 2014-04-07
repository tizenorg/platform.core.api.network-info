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
#include <telephony_network.h>
#include <dlog.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "TIZEN_N_NETWORK_INFO_TEST"


int main()
{
	int ret = 0;
	int cell_id = 0;
	int ret_value = network_info_get_cell_id(&cell_id);

	switch(ret_value)
	{
		case NETWORK_INFO_ERROR_NONE:
			LOGI("[%s] Cell ID is %d", __FUNCTION__, cell_id);
			ret = 0;
			break;
		case NETWORK_INFO_ERROR_INVALID_PARAMETER:
			LOGI("[%s] Invalid parameter", __FUNCTION__);
			ret = -1;
			break;
		case NETWORK_INFO_ERROR_OUT_OF_SERVICE:
			LOGI("[%s] Out of service\n", __FUNCTION__);
			ret = -1;
			break;
		case NETWORK_INFO_ERROR_OPERATION_FAILED:
			LOGI("[%s] Cannot find Cell ID value\n", __FUNCTION__);
			ret = -1;
			break;
		default:
			LOGI("[%s] Unexpected return value\n", __FUNCTION__);
			ret = -1;
			break;
	}

	return ret;
}
