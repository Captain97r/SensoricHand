/** 
 * @file
 * LTC2497.h
 * 
 * @brief LTC2497 library
 *
 * This file is a header of library for ADC LTC2497. It contains 
 * register and some set-up bits definitions, and declares several 
 * functions for ADC use.
 * 
 * @author Radchenko Evgeny
 * captain.97r@gmail.com
 */

#pragma once
#include "i2c.h"

#define ADC_ADDRESS_ONE					0x14
#define ADC_ADDRESS_TWO					0x56

#define SELECT_BYTE_PREAMBLE_BITS		0x02 << 6
#define SELECT_BYTE_ENABLE_BIT			0x01 << 5
#define SELECT_BYTE_DIFF_INPUT			0x00 << 4
#define SELECT_BYTE_SINGLE_INPUT		0x01 << 4
#define SELECT_BYTE_POSITIVE_POL		0x00 << 3
#define SELECT_BYTE_NEGATIVE_POL		0x01 << 3

#define SETUP_BYTE_ENABLE_BIT			0x01 << 7
#define SETUP_BYTE_TEMP_OUTPUT_ON		0x01 << 6
#define SETUP_BYTE_TEMP_OUTPUT_OFF		0x00 << 6
#define SETUP_BYTE_50_60_HZ_FREQ		0x00 << 4
#define SETUP_BYTE_50_HZ_FREQ			0x01 << 4
#define SETUP_BYTE_60_HZ_FREQ			0x02 << 4
#define SETUP_BYTE_1X_SPEED				0x00 << 3
#define SETUP_BYTE_2X_SPEED				0x01 << 3


typedef enum
{
	LTC2497_DIFF_POLARITY_POSITIVE		= SELECT_BYTE_POSITIVE_POL,
	LTC2497_DIFF_POLARITY_NEGATIVE		= SELECT_BYTE_NEGATIVE_POL
} LTC2497_DIFF_POLARITY;

typedef enum
{
	LTC2497_REJECTION_FREQ_50_60_HZ		= SETUP_BYTE_50_60_HZ_FREQ,
	LTC2497_REJECTION_FREQ_50_HZ		= SETUP_BYTE_50_HZ_FREQ,
	LTC2497_REJECTION_FREQ_60_HZ		= SETUP_BYTE_60_HZ_FREQ
} LTC2497_REJECTION_FREQ;

typedef enum
{
	LTC2497_CONVERSION_SPEED_1X			= SETUP_BYTE_1X_SPEED,
	LTC2497_CONVERSION_SPEED_2X			= SETUP_BYTE_2X_SPEED
} LTC2497_CONVERSION_SPEED;

typedef enum
{
	LTC2497_TEMP_OUTPUT_OFF				= SETUP_BYTE_TEMP_OUTPUT_OFF,
	LTC2497_TEMP_OUTPUT_ON				= SETUP_BYTE_TEMP_OUTPUT_ON
} LTC2497_TEMP_OUTPUT;

/**@brief LTC2497 setup structure. This contains all options needed for
 *        initialization of the module*/
typedef struct 
{
	uint8_t			freq;
	uint8_t			speed;
	uint8_t			temp;
} LTC2497_setup_t;


/**
  * @brief  Selects single channel to futher interaction with.
  *
  *
  * @param  address - I2C address of LTC2497 module
  * @param  channel - number of channel to select
  * 
  * @retval TX operation result code
  */
ret_code_t ltc2497_select_single_channel(uint8_t address, uint8_t channel);

/**
  * @brief  Selects differential channel to futher interaction with.
  *
  *
  * @param[in]	address		I2C address of LTC2497 module
  * @param[in]  channel		number of channel to select
  * 
  * @retval TX operation result code
  */
ret_code_t ltc2497_select_diff_channel(uint8_t address, uint8_t channel, uint8_t polarity);

/**
  * @brief  Initializes LTC2497 chip.
  *
  *
  * @param[in]  address		I2C address of LTC2497 module
  * @param[in]  setup		structure with chip init parameters
  * 
  * @retval		TX operation result code
  */
ret_code_t ltc2497_setup(uint8_t address, LTC2497_setup_t* setup);

/**
  * @brief  reads data form LTC2497 using pre-selected channel
  *
  *
  * @param[in]  address		I2C address of LTC2497 module
  * @param[out] channel		data which have been read
  * 
  * @retval		TX operation result code
  */
ret_code_t ltc_read_data(uint8_t address, uint8_t* data);
