/** 
 * @file
 * LTC2497.c
 * 
 * @brief LTC2497 library
 *
 * This file contains implementations of functions declared in header file.
 * 
 * @author Radchenko Evgeny
 * captain.97r@gmail.com
 */

#include "LTC2497.h"


ret_code_t ltc2497_select_single_channel(uint8_t address, uint8_t channel)
{
	
	if (channel > 15)
		return NRF_ERROR_INVALID_ADDR;
	
	uint8_t payload[2] = {
		SELECT_BYTE_PREAMBLE_BITS | SELECT_BYTE_ENABLE_BIT | SELECT_BYTE_SINGLE_INPUT | channel, 
		0x00
	};
	return nrf_drv_twi_tx(&m_twi_instance, address, payload, sizeof(payload), true); 
}

ret_code_t ltc2497_select_diff_channel(uint8_t address, uint8_t channel, uint8_t polarity)
{
	if (channel > 7)
		return NRF_ERROR_INVALID_ADDR;
	
	uint8_t payload[2] = { SELECT_BYTE_PREAMBLE_BITS | SELECT_BYTE_ENABLE_BIT | SELECT_BYTE_DIFF_INPUT | polarity | channel, 
						   0x00 };
	return nrf_drv_twi_tx(&m_twi_instance, address, payload, sizeof(payload), true);
}


ret_code_t ltc2497_setup(uint8_t address, LTC2497_setup_t* setup)
{
	uint8_t payload[2] = {
		SELECT_BYTE_PREAMBLE_BITS | SELECT_BYTE_ENABLE_BIT, 
		SETUP_BYTE_ENABLE_BIT | setup->freq | setup->speed | setup->temp
	};
	return nrf_drv_twi_tx(&m_twi_instance, address, payload, sizeof(payload), true); 
}

ret_code_t ltc_read_data(uint8_t address, uint8_t* data)
{
	return nrf_drv_twi_rx(&m_twi_instance, address, data, 4);
}

