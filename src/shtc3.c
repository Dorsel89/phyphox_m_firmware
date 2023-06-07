#include "shtc3.h"
SHTC shtc_data;

extern bool init_shtc() 
{   
    if(!device_is_ready(shtc_dev)){
        printk("Device not ready or not found");
        return false;
    }
    shtc_data.timer_interval = 3;
    k_work_init(&work_shtc, send_data_shtc);
	k_work_init(&config_work_shtc, set_config_shtc);
    k_timer_init(&timer_shtc, shtc_data_ready, NULL);

    return true;
}

extern void sleep_shtc(bool sleep) 
{
    if (sleep) {
        k_timer_stop(&timer_shtc);
    }
    else{
        k_timer_start(&timer_shtc, K_MSEC(shtc_data.timer_interval*10), K_MSEC(shtc_data.timer_interval*10));
    }
}

void shtc_data_ready()
{
	k_work_submit(&work_shtc);
}

void send_data_shtc()
{
    sensor_sample_fetch(shtc_dev);
    sensor_channel_get(shtc_dev, SENSOR_CHAN_AMBIENT_TEMP, &shtc_temp);
    sensor_channel_get(shtc_dev, SENSOR_CHAN_HUMIDITY, &shtc_humid);
    
    shtc_data.temperature = sensor_value_to_float(&shtc_temp);
    shtc_data.humidity = sensor_value_to_float(&shtc_humid);

    float timestamp = k_uptime_get() /1000.0;
    shtc_data.timestamp = timestamp;
    shtc_data.array[0] = shtc_data.temperature;
    shtc_data.array[1] = shtc_data.humidity;
    shtc_data.array[2] = shtc_data.timestamp;

    //printk("%f || %f \n", shtc_data.temperature, shtc_data.humidity);

    send_data(SENSOR_SHTC_ID, &shtc_data.array, 4*3);
}

void set_config_shtc() 
{
    sleep_shtc(true);
    shtc_data.timer_interval = shtc_data.config[1];
    printk("SHTC3 config received \n");
    printk("SHTC3 interval: %i\n",shtc_data.timer_interval);
    //Ensure minimum of 30ms
    if (shtc_data.timer_interval < 3) {shtc_data.timer_interval = 3;}
    sleep_shtc(!shtc_data.config[0]);
}

extern void submit_config_shtc()
{	
    k_work_submit(&config_work_shtc);
}