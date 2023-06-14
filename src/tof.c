#include "tof.h"


static const struct gpio_dt_spec tofInt = GPIO_DT_SPEC_GET_OR(TOF_INT, gpios,{0});
static struct gpio_callback tofInt_cb_data;

int8_t init_tof(){
    dev = 0x29;
    tof_data.event_size = 2;
    tof_data.event_number = 0;
    status = VL53L4CD_GetSensorId(dev, &sensor_id);
	if(status || (sensor_id != 0xEBAA))
	{
		printf("VL53L4CD not detected at requested address\n");
		return status;
	}

	/* (Mandatory) Init VL53L4CD sensor */
	status = VL53L4CD_SensorInit(dev);
	if(status)
	{
		printf("VL53L4CD ULD Loading failed\n");
		return status;
	}

	printf("VL53L4CD ULD ready !\n");

    k_work_init(&work_tof, send_data_tof);
    k_work_init(&config_work_tof, update_config_tof);
    status = configure_tof_int(); //configure data ready interrupt
    printf("status interrupt: %i \n",status);

    //REWORK THIS
    status = VL53L4CD_SetRangeTiming(dev, 10, 0);
	if(status)
	{
		printf("VL53L4CD_SetRangeTiming failed with status %u\n", status);
		return status;
	}
    //status = VL53L4CD_SetDetectionThresholds(dev, 100, 300, 3);

    status = VL53L4CD_StartRanging(dev);
}

uint8_t sleep_tof(bool b){
    if(b){
        status = VL53L4CD_StopRanging(dev);
    }else{
        //TODO
    }
}

extern void send_data_tof(void){
    //TODO
    printf("send new data! \n");
    uint32_t time = k_uptime_get_32;
    VL53L4CD_ClearInterrupt(dev);
    VL53L4CD_GetResult(dev, &results);
    printf("Status = %6u, Distance = %6u, Signal = %6u\n",
				 results.range_status,
				 results.distance_mm,
				 results.sigma_mm);
    if(results.range_status == 0){
        tof_data.data_array[2*tof_data.event_number] = results.distance_mm;
        tof_data.data_array[2*tof_data.event_number+1] = results.sigma_mm;
        tof_data.event_number+=1;
        if(tof_data.event_number == tof_data.event_size){
            send_data(SENSOR_TOF_ID, &tof_data.data_array, tof_data.event_size*2*2);
        }
    }
}
void update_config_tof(){
    //TODO
    status = VL53L4CD_StartRanging(dev);
    if(DEBUG){printf("tof config updated! \n");};
}

static void tofDataReady(const struct device *dev, struct gpio_callback *cb,uint32_t pins){
	k_work_submit(&work_tof);
    printf("hardware int fired!\n");
}

int8_t configure_tof_int(){
    if (!device_is_ready(tofInt.port)) {
		printk("Error: tof interrupt %s is not ready\n",
		       tofInt.port->name);
		return 1;
	}
    tofResult = gpio_pin_configure_dt(&tofInt, GPIO_INPUT);
    if (tofResult != 0) {
		printk("Error %d: failed to configure %s pin %d\n\r",
		       tofResult, tofInt.port->name, tofInt.pin);
		return tofResult;
	}
    tofResult = gpio_pin_interrupt_configure_dt(&tofInt,GPIO_INT_EDGE_RISING);
	if (tofResult != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n\r",
			tofResult, tofInt.port->name, tofInt.pin);
		return tofResult;
	}
    gpio_init_callback(&tofInt_cb_data, tofDataReady, BIT(tofInt.pin));
    gpio_add_callback(tofInt.port, &tofInt_cb_data);
    return tofResult;

}