/** 
 * @file
 * ble_measurement_service.h
 * 
 * @brief Measurement Service implementation
 *
 * This file declares all structures and enumerations needed for 
 * initialization and functioning BLE Measurement Service, which contains
 * ADC read data.
 * 
 */


#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define MEASUREMENT_SERVICE_UUID_BASE			{0x25, 0x81, 0xE8, 0x8C, 0x72, 0x2E, 0x8F, 0x97,  \
                                                 0xEA, 0x11, 0x45, 0x29,  0x08, 0x91, 0xD3, 0x5B}

#define MEASUREMENT_SERVICE_UUID				0x1400
#define MEASUREMENT_CH01_CHAR_UUID              0x1401
#define MEASUREMENT_CH02_CHAR_UUID              0x1402
#define MEASUREMENT_CH03_CHAR_UUID              0x1403
#define MEASUREMENT_CH04_CHAR_UUID              0x1404
#define MEASUREMENT_CH05_CHAR_UUID              0x1405
#define MEASUREMENT_CH06_CHAR_UUID              0x1406
#define MEASUREMENT_CH07_CHAR_UUID              0x1407
#define MEASUREMENT_CH08_CHAR_UUID              0x1408
#define MEASUREMENT_CH09_CHAR_UUID              0x1409
#define MEASUREMENT_CH10_CHAR_UUID              0x140A
#define MEASUREMENT_CH11_CHAR_UUID              0x140B
#define MEASUREMENT_CH12_CHAR_UUID              0x140C
#define MEASUREMENT_CH13_CHAR_UUID              0x140D
#define MEASUREMENT_CH14_CHAR_UUID              0x140E
#define MEASUREMENT_CH15_CHAR_UUID              0x140F
#define MEASUREMENT_CH16_CHAR_UUID              0x1410


/**@brief   Macro for defining a Measurement Service instance.
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
#define BLE_MEAS_DEF(_name)                                                                         \
static ble_meas_t _name;                                                                            \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_HRS_BLE_OBSERVER_PRIO,                                                     \
                     ble_meas_on_ble_evt, &_name)

	

typedef enum
{
	BLE_MEAS_EVT_NOTIFICATION_ENABLED,
	BLE_MEAS_EVT_NOTIFICATION_DISABLED,
	BLE_MEAS_EVT_DISCONNECTED,
	BLE_MEAS_EVT_CONNECTED
} ble_meas_evt_type_t;


/**@brief Measurement Service event. */
typedef struct
{
	ble_meas_evt_type_t             evt_type;
	const ble_gatts_evt_write_t * p_evt_write;
} ble_meas_evt_t;


// Forward declaration of the ble_meas_t type.
typedef struct ble_meas_s ble_meas_t;

/**@brief Measurement Service event handler type. */
typedef void(*ble_meas_evt_handler_t)(ble_meas_t * p_cus, ble_meas_evt_t * p_evt);

	
/**@brief Measurement Service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
	ble_meas_evt_handler_t          evt_handler;            /**< Event handler to be called for handling events in the Custom Service. */
	uint8_t							initial_value;          /**< Initial value */
	ble_srv_cccd_security_mode_t	value_char_attr_md;     /**< Initial security level for Measurement characteristics attribute */
	
} ble_meas_init_t;


/**@brief Measurement Service structure. This contains various status information for the service. */
struct ble_meas_s
{
	ble_meas_evt_handler_t          evt_handler;            /**< Event handler to be called for handling events in the Custom Service. */
	uint16_t                        service_handle;         /**< Handle of Measurement Service (as provided by the BLE stack). */
	ble_gatts_char_handles_t		value_handles[16];	    /**< Handles related to the Measurement Value characteristic. */
	uint16_t						conn_handle;            /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
	uint8_t							uuid_type; 
};





typedef struct ble_char_init
{
	uint8_t							char_prop_read;
	uint8_t							char_prop_write;
	uint8_t							char_prop_notify;
	
	uint16_t						ble_uuid;
	uint16_t						attr_char_max_len;
	uint8_t							init_value;
}ble_char_init_t;



/**@brief Function for initializing the Measurement Service.
 *
 * @param[out]  p_meas      Measurement Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_meas_init Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_meas_init(ble_meas_t * p_meas, const ble_meas_init_t * p_meas_init);


/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Battery Service.
 *
 * @note 
 *
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 * @param[in]   p_context  Custom Service structure.
 */
void ble_meas_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);


/**@brief Function for updating the value.
 *
 * @details The application calls this function when the cutom value should be updated. If
 *          notification has been enabled, the custom value characteristic is sent to the client.
 *
 * @note 
 *       
 * @param[in]   p_cus          Measurement Service structure.
 * @param[in]   Custom value 
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_meas_value_update(ble_meas_t * p_cus, uint8_t* value, uint8_t value_char_num);