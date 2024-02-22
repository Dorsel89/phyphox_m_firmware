#include "veml6030.h"

static const struct gpio_dt_spec vemlInt = GPIO_DT_SPEC_GET_OR(VEML_INT, gpios,{0});
static struct gpio_callback vemlInt_cb_data;

static const uint16_t it_msec[6]={25,50,100,200,400,800};
static const uint8_t it_hex[6]={0x0C,0x08,0x00,0x01,0x02,0x03};

//gain 2 1 1/4 1/8
static const uint8_t gain_hex[4]={0x01,0x00,0x11,0x10};

int8_t init_veml(){

    k_work_init(&work_veml, send_data_veml);
    k_work_init(&config_work_veml, update_config_veml);
    k_timer_init(&timer_veml, vemlDataReady, NULL);

    configure_veml_int(); //configure data ready interrupt

    //set default values
    veml_data.integration_time = 0;
    veml_data.gain = 0x00;
    if(set_configuration(true) && DEBUG){printf("error at veml configuration \n");};
}

int8_t set_configuration(bool sleep){
    uint8_t config_register[2];
    uint16_t reg_buffer = sleep | (0x01<< 1) | (0x00 << 4) | ( it_hex[veml_data.integration_time]  << 6) | (gain_hex[veml_data.gain] << 11);
    
    config_register[0] = reg_buffer & 0xff;
    config_register[1] = reg_buffer >> 8;
    if(sleep){
        k_timer_stop(&timer_veml);
        skipped_first = false;
    }else{
        k_timer_start(&timer_veml, K_MSEC(it_msec[veml_data.integration_time]), K_MSEC(it_msec[veml_data.integration_time]));
    }
    
    return write_bytes(0x00,&config_register[0],2,VEML_ADDRESS);
}

uint16_t compensation(uint16_t val){
    return (.00000000000060135 * (pow(val, 4))) -(.0000000093924 * (pow(val, 3))) + (.000081488 * (pow(val,2))) + (1.0023 * val);
}

int8_t veml_get_data(){
    uint8_t light_array[2];
    float factor = (0.1152*veml_data.gain)/(veml_data.integration_time+1);

    if(read_bytes(0x04,&light_array[0],2,VEML_ADDRESS) && DEBUG){printf("error with reading veml\n");}
    uint16_t light= (light_array[0] | (light_array[1] << 8))*factor;
    memcpy(&veml_data.data_array[0],&light,2);

    // readout white
    if(read_bytes(0x05,&light_array[0],2,VEML_ADDRESS) && DEBUG){printf("error with reading veml\n");}
    uint16_t light_white= (light_array[0] | (light_array[1] << 8))*factor;
    memcpy(&veml_data.data_array[2],&light_white,2);
    float timeval[1];
    timeval[0] = (k_uptime_ticks()/32768.0)-global_timestamp;
    memcpy(&veml_data.data_array[4],&timeval,4);
}

uint8_t sleep_veml(bool b){
    if(b){
        set_configuration(b);
    }else{
        //TODO
    }
}

extern void send_data_veml(void){
    veml_get_data();
    if(!skipped_first){
        skipped_first = true;
        return;
    }
    send_data(SENSOR_VEML_ID, &veml_data.data_array, 8);
}

void update_config_veml(){
    if(DEBUG){
        printf("VEML setting config to\n");
        printf("enable: %d \n",veml_data.config[0]);
        printf("integration time: %d \n",veml_data.config[1]);
        printf("gain: %d \n",veml_data.config[2]);
    }
    veml_data.gain = veml_data.config[2];
    veml_data.integration_time = veml_data.config[1];
    if(set_configuration(false) && DEBUG){printf("error at veml configuration \n");};
}

void vemlDataReady(const struct device *dev, struct gpio_callback *cb,uint32_t pins){
	k_work_submit(&work_veml);
}

int8_t configure_veml_int(){
    if (!device_is_ready(vemlInt.port)) {
		printk("Error: veml interrupt %s is not ready\n",
		       vemlInt.port->name);
		return 1;
	}
    vemlResult = gpio_pin_configure_dt(&vemlInt, GPIO_INPUT);
    if (vemlResult != 0) {
		printk("Error %d: failed to configure %s pin %d\n\r",
		       vemlResult, vemlInt.port->name, vemlInt.pin);
		return vemlResult;
	}
    vemlResult = gpio_pin_interrupt_configure_dt(&vemlInt,GPIO_INT_EDGE_FALLING);
	if (vemlResult != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n\r",
			vemlResult, vemlInt.port->name, vemlInt.pin);
		return vemlResult;
	}
    gpio_init_callback(&vemlInt_cb_data, vemlDataReady, BIT(vemlInt.pin));
    gpio_add_callback(vemlInt.port, &vemlInt_cb_data);
    return vemlResult;

}