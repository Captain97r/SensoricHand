/** 
 * @file
 * i2c.h
 * 
 * @brief I2C library for nRF52
 *
 * This file is a header of library for I2C protocol.
 * In fact, it's just a wrapper of default nRF52 SDK
 * I2C funcions.
 * 
 * @author Radchenko Evgeny
 * captain.97r@gmail.com
 */
#pragma once

#include "nrf_twi_mngr.h"
#include "nrf_drv_twi.h"
#include "nrfx_twi.h"

#define TWI_INSTANCE		0
#define TWI_ADDRESSES		127

#if USE_LEGACY_DRIVERS
static const nrf_drv_twi_t m_twi_instance = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE);
#else
static const nrfx_twi_t m_twi_instance = NRFX_TWI_INSTANCE(TWI_INSTANCE);
#endif //USE_LEGACY_DRIVERS

/**
  * @brief  Configures I2C instance.
  *
  * @retval None
  */
void twi_init(void);

/**
  * @brief  Scans I2C for connected modules.
  *
  * @retval Number of connected devices
  */
uint8_t twi_scan(void);