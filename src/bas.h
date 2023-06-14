#ifndef _BAS_H /* Include guard */
#define _BAS_H

#include <zephyr/drivers/led.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <hal/nrf_saadc.h>
#include <math.h>

#define ADC_RESOLUTION 12
#define ADC_GAIN ADC_GAIN_1_6
#define ADC_REFERENCE ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 10)
#define BUFFER_SIZE 1
#define BAD_ANALOG_READ -123

#define ADC_1ST_CHANNEL_ID 7
#define ADC_1ST_CHANNEL_INPUT NRF_SAADC_INPUT_AIN7

// #define ADC_CHANNEL          DT_IO_CHANNELS_INPUT_BY_IDX(DT_PATH(zephyr_user), 0)
// #define ADC_NODE		     DT_PHANDLE(DT_PATH(zephyr_user), io_channels)
// #define ADC_NUM_CHANNELS	 DT_PROP_LEN(DT_PATH(zephyr_user), io_channels)


#define ADC_DEVICE_NAME DT_LABEL(DT_INST(0, nordic_nrf_saadc))


static struct k_timer timer_bas;
static struct k_work work_bas;

const static struct device *adc_dev;

extern void init_BAS();
void update_supercap_level();
float getVoltage();

uint8_t battery_level(float);
void time_to_update_battery_service();

#endif