/** 
 * @file
 * i2c.c
 * 
 * @brief I2C library for nRF52
 *
 * This file contains implementations of functions, declared in i2c.h
 * 
 * @author Radchenko Evgeny
 * captain.97r@gmail.com
 */

#include "i2c.h"
#include "nrf_delay.h"

void twi_init(void)
{
#if USE_LEGACY_DRIVERS
	uint32_t err_code;
	    
	const nrf_drv_twi_config_t twi_mpu_config = {
		.scl = 26,
		.sda = 25,
		.frequency = NRF_DRV_TWI_FREQ_400K,
		.interrupt_priority = APP_IRQ_PRIORITY_HIGHEST,
		.clear_bus_init = false
	};
	    
	err_code = nrf_drv_twi_init(&m_twi_instance, &twi_mpu_config, NULL, NULL);
	nrf_drv_twi_enable(&m_twi_instance);
#else
	//	nrfx_err_t err_code;
//	
//	const nrfx_twi_config_t twi_config = { 
//		.scl = 26,
//		.sda = 25,
//		.frequency = TWI_FREQUENCY_FREQUENCY_K400,
//		.interrupt_priority = NRFX_TWI_DEFAULT_CONFIG_IRQ_PRIORITY,
//		.hold_bus_uninit = NRFX_TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT
//	};
//	
//	err_code = nrfx_twi_init(&m_twi_instance, &twi_config, NULL, NULL);
//	nrfx_twi_enable(&m_twi_instance);
//	
//	uint8_t data[1] = { 0b10101010 };
//	err_code = nrfx_twi_tx(&m_twi_instance, 0b0010100, data, 1, false);
#endif //USE_LEGACY_DRIVERS
}

uint8_t twi_scan(void)
{
	ret_code_t err_code;
	uint8_t sample_data;
	bool detected_device = false;
	uint8_t device_num = 0;
	uint8_t address_array[5] = { 0 };
	
	for (uint8_t address = 0x01; address <= TWI_ADDRESSES; address++)
	{
		err_code = nrf_drv_twi_rx(&m_twi_instance, address, &sample_data, sizeof(sample_data));
		if (err_code == NRF_SUCCESS)
		{
			detected_device = true;
			address_array[device_num++] = address;
		}
	}

 	if (!detected_device)
	{
		sample_data = 0;
	}
	
	return device_num;
}