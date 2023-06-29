#ifndef SENSORS_H
#define SENSORS_H

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>

#define DEBUG				true
#define PRINT_SENSOR_DATA 	true

#define SENSOR_BMP581_ID		1
#define SENSOR_BMI323_ACC_ID	2
#define SENSOR_BMI323_GYR_ID	3
#define SENSOR_SHTC_ID			4
#define SENSOR_TOF_ID			5
#define SENSOR_VEML_ID			6
#define SENSOR_MLX_ID			7


/**
 * @brief Helper function for converting struct sensor_value to float.
 *
 * @param val A pointer to a sensor_value struct.
 * @return The converted value.
 */
static inline float sensor_value_to_float(const struct sensor_value *val) 
{
	return (float)val->val1 + (float)val->val2 / 1000000;
}

typedef struct {
	int max_events;
	int current_event;
	float pressure;
	float temperature;
	float timestamp;
	float array[3*10];
	uint8_t config[20];
}BMP;

typedef struct {
	int event_size;
	int event_number;
	int nOutputs;
	int16_t acc_array[3*30];//x,y,z,t
	int16_t gyr_array[3*30];//x,y,z,t
	uint8_t config[20];
}BMI;

typedef struct {
	int event_size;
	int event_number;
	int nOutputs;
	int16_t data_array[3*30];//x,y,z,t
	uint8_t config[20];
	uint8_t mode;
}MLX;

typedef struct {
	int event_size;
	int event_number;
	uint16_t data_array[2*2];//distance, sigma distance
	uint8_t config[20];
}TOF;

typedef struct {
	uint16_t data_array[2];//distance, sigma distance
	uint8_t config[20];
	uint8_t gain;
	uint8_t integration_time;
}VEML;

static BMP bmp_data;
extern BMI bmi_data;
extern MLX mlx_data;
extern TOF tof_data;
extern VEML veml_data;

#endif // SENSORS_H