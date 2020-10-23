/** 
 * @file
 * ble_measurement_service.c
 * 
 * @brief Measurement Service implementation
 *
 * This file contains implementations of all functions, declared in
 * ble_measurement_service.h, and some more, needed for proper initialization
 * and operation BLE Measurement Service.
 * 
 */


#include "sdk_common.h"
#include "ble_srv_common.h"
#include <string.h>
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_log.h"

#include "ble_measurement_service.h"
#include "ble_service_handler.h"


static void set_char_to_zero(ble_meas_t * p_meas, ble_gatts_attr_t * attr_char_value, uint8_t value_char_num)
{
	uint32_t            err_code;
	ble_gatts_value_t	gatts_value;
	uint8_t init_val  = 0x00;
	
	memset(&gatts_value, 0, sizeof(gatts_value));

	
	for (int i = 0; i < (attr_char_value->max_len) / (sizeof(uint8_t)); i++)
	{
		gatts_value.len     = sizeof(uint8_t);
		gatts_value.offset  = i * sizeof(uint8_t);
		gatts_value.p_value = &init_val;
	
	
		err_code = sd_ble_gatts_value_set(p_meas->conn_handle,
			p_meas->value_handles[value_char_num].value_handle,
			&gatts_value);
		APP_ERROR_CHECK(err_code);
	}
}

/**@brief Function for adding the Custom Value characteristic.
 *
 * @param[in]   p_cus        Custom Service structure.
 * @param[in]   p_cus_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t value_char_add(ble_meas_t * p_meas, const ble_meas_init_t * p_meas_init, const ble_char_init_t * p_char_init)
{
	static uint8_t value_char_num = 0;
	
	uint32_t            err_code;
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t    attr_char_value;
	ble_uuid_t          ble_uuid;
	ble_gatts_attr_md_t attr_md;
	
	memset(&cccd_md, 0, sizeof(cccd_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	cccd_md.vloc       = BLE_GATTS_VLOC_STACK;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.read   = p_char_init->char_prop_read;
	char_md.char_props.write  = p_char_init->char_prop_write;
	char_md.char_props.notify = p_char_init->char_prop_notify; 
	char_md.p_char_user_desc  = NULL;
	char_md.p_char_pf         = NULL;
	char_md.p_user_desc_md    = NULL;
	char_md.p_cccd_md         = NULL; 
	char_md.p_sccd_md         = (p_char_init->char_prop_notify == 1) ? &cccd_md : NULL;
		
	memset(&attr_md, 0, sizeof(attr_md));

	attr_md.read_perm  = p_meas_init->value_char_attr_md.read_perm;
	attr_md.write_perm = p_meas_init->value_char_attr_md.write_perm;
	attr_md.vloc       = BLE_GATTS_VLOC_STACK;
	attr_md.rd_auth    = 0;
	attr_md.wr_auth    = 0;
	attr_md.vlen       = 0;

	ble_uuid.type = p_meas->uuid_type;
	ble_uuid.uuid = p_char_init->ble_uuid;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid    = &ble_uuid;
	attr_char_value.p_attr_md = &attr_md;
	attr_char_value.init_len  = sizeof(uint8_t);
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = sizeof(uint8_t) * p_char_init->attr_char_max_len;

	err_code = sd_ble_gatts_characteristic_add(p_meas->service_handle,
		&char_md,
		&attr_char_value,
		&p_meas->value_handles[value_char_num]);
	if (err_code != NRF_SUCCESS)
	{
		return err_code;
	}
	
	set_char_to_zero(p_meas, &attr_char_value, value_char_num);
	
	NRF_LOG_INFO("Success!");
	value_char_num++;

	return NRF_SUCCESS;
}

static uint32_t ble_chars_create(ble_meas_t * p_meas, const ble_meas_init_t * p_meas_init)
{

	//Creating char for voltage value
		
	uint32_t   err_code;
	ble_char_init_t ble_char_init;
	
	ble_char_init.char_prop_read = 1;
	ble_char_init.char_prop_write = 1;
	ble_char_init.char_prop_notify = 1;
	
	ble_char_init.ble_uuid = MEASUREMENT_SERVICE_UUID;
	ble_char_init.attr_char_max_len = 4;
	
	for (int CHAR_UUID = MEASUREMENT_CH01_CHAR_UUID; CHAR_UUID <= MEASUREMENT_CH16_CHAR_UUID; CHAR_UUID++)
	{
		ble_char_init.ble_uuid = CHAR_UUID;
		
		err_code = value_char_add(p_meas, p_meas_init, &ble_char_init);
		if (err_code != NRF_SUCCESS)
			return err_code;
	}
	
	return NRF_SUCCESS;
}


/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_meas_t * p_meas, ble_evt_t const * p_ble_evt)
{
	p_meas->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
	
	ble_meas_evt_t evt;
	evt.evt_type = BLE_MEAS_EVT_CONNECTED;
	p_meas->evt_handler(p_meas, &evt);
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_meas_t * p_meas, ble_evt_t const * p_ble_evt)
{
	UNUSED_PARAMETER(p_ble_evt);
	p_meas->conn_handle = BLE_CONN_HANDLE_INVALID;
	
	ble_meas_evt_t evt;
	evt.evt_type = BLE_MEAS_EVT_DISCONNECTED;
	p_meas->evt_handler(p_meas, &evt);
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_meas_t * p_meas, ble_evt_t const * p_ble_evt)
{
	const ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

	switch (p_evt_write->uuid.uuid)
	{
	}
	
	// Check if the handler of current event is exists
	bool handler_found = false;
	for (uint8_t handler = 0; handler < 16; handler++)
	{
		if (p_evt_write->handle == p_meas->value_handles[handler].cccd_handle)
		{
			handler_found = true;
			break;
		}
	}
	
	if (handler_found)
	{
		// CCCD written, call application event handler
		if(p_meas->evt_handler != NULL)
		{
			ble_meas_evt_t evt;

			if (ble_srv_is_notification_enabled(p_evt_write->data))
			{
				evt.evt_type = BLE_MEAS_EVT_NOTIFICATION_ENABLED;
			}
			else
			{
				evt.evt_type = BLE_MEAS_EVT_NOTIFICATION_DISABLED;
			}
			evt.p_evt_write = p_evt_write;
			// Call the application event handler.
			p_meas->evt_handler(p_meas, &evt);
		}
	}
}


uint32_t ble_meas_init(ble_meas_t * p_meas, const ble_meas_init_t * p_meas_init)
{
	if (p_meas == NULL || p_meas_init == NULL)
	{
		return NRF_ERROR_NULL;
	}

	uint32_t   err_code;
	ble_uuid_t ble_uuid;
	
	// Initialize service structure
	p_meas->evt_handler				= p_meas_init->evt_handler;
	p_meas->conn_handle				= BLE_CONN_HANDLE_INVALID;
	
	// Add Custom Service UUID
	ble_uuid128_t base_uuid = { MEASUREMENT_SERVICE_UUID_BASE };
	err_code =  sd_ble_uuid_vs_add(&base_uuid, &p_meas->uuid_type);
	VERIFY_SUCCESS(err_code);

	ble_uuid.type = p_meas->uuid_type;
	ble_uuid.uuid = MEASUREMENT_SERVICE_UUID;
	
	// Add the Measurement Service
	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_meas->service_handle);
	VERIFY_SUCCESS(err_code);
	
	return ble_chars_create(p_meas, p_meas_init);
}


void ble_meas_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
	ble_meas_t * p_cus = (ble_meas_t *) p_context;
    
	if (p_cus == NULL || p_ble_evt == NULL)
	{
		return;
	}
	
	switch (p_ble_evt->header.evt_id)
	{
	case BLE_GAP_EVT_CONNECTED:
		on_connect(p_cus, p_ble_evt);
		break;

	case BLE_GAP_EVT_DISCONNECTED:
		on_disconnect(p_cus, p_ble_evt);
		break;
		
	case BLE_GATTS_EVT_WRITE:
		on_write(p_cus, p_ble_evt);
		break;

	default:
		// No implementation needed.
		break;
	}
}


uint32_t ble_meas_value_update(ble_meas_t * p_cus, uint8_t* value, uint8_t value_char_num) {
	if (p_cus == NULL)
	{
		return NRF_ERROR_NULL;
	}
	
	uint32_t err_code = NRF_SUCCESS;
	ble_gatts_value_t gatts_value;

	// Initialize value struct.
	memset(&gatts_value, 0, sizeof(gatts_value));

	gatts_value.len     = 4 * sizeof(uint8_t);
	gatts_value.offset  = 0;
	gatts_value.p_value = value;

	// Update database.
	err_code = sd_ble_gatts_value_set(p_cus->conn_handle,
		p_cus->value_handles[value_char_num].value_handle,
		&gatts_value);
	if (err_code != NRF_SUCCESS)
	{
		return err_code;
	}
	
	if ((p_cus->conn_handle != BLE_CONN_HANDLE_INVALID)) 
	{
		ble_gatts_hvx_params_t hvx_params;

		memset(&hvx_params, 0, sizeof(hvx_params));

		hvx_params.handle = p_cus->value_handles[value_char_num].value_handle;
		hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
		hvx_params.offset = gatts_value.offset;
		hvx_params.p_len  = &gatts_value.len;
		hvx_params.p_data = gatts_value.p_value;

		err_code = sd_ble_gatts_hvx(p_cus->conn_handle, &hvx_params);
	}
	else
	{
		err_code = NRF_ERROR_INVALID_STATE;
	}

	return err_code;
	
}