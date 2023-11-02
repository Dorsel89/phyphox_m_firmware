#include "bmpZephyr.h"

static uint8_t dev_addr;
static struct bmp5_osr_odr_press_config osr_odr_press_cfg = { 0 };

BMP5_INTF_RET_TYPE bmp5_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    uint8_t device_addr = *(uint8_t*)intf_ptr;

    (void)intf_ptr;
    return i2c_burst_read(intf_ptr,BMP581_I2C_ADDR,reg_addr,reg_data,length);
}

BMP5_INTF_RET_TYPE bmp5_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    uint8_t device_addr = *(uint8_t*)intf_ptr;

    (void)intf_ptr;

    return i2c_burst_write(intf_ptr,BMP581_I2C_ADDR,reg_addr,reg_data,length);
}

void bmp5_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;
    return k_busy_wait(period);
}

void bmp5_error_codes_print_result(const char api_name[], int8_t rslt)
{
    if (rslt != BMP5_OK)
    {
        printk("%s\t", api_name);
        if (rslt == BMP5_E_NULL_PTR)
        {
            printk("Error [%d] : Null pointer\r\n\r", rslt);
        }
        else if (rslt == BMP5_E_COM_FAIL)
        {
            printk("Error [%d] : Communication failure\r\n\r", rslt);
        }
        else if (rslt == BMP5_E_DEV_NOT_FOUND)
        {
            printk("Error [%d] : Device not found\r\n\r", rslt);
        }
        else if (rslt == BMP5_E_INVALID_CHIP_ID)
        {
            printk("Error [%d] : Invalid chip id\r\n\r", rslt);
        }
        else if (rslt == BMP5_E_POWER_UP)
        {
            printk("Error [%d] : Power up error\r\n\r", rslt);
        }
        else if (rslt == BMP5_E_POR_SOFTRESET)
        {
            printk("Error [%d] : Power-on reset/softreset failure\r\n\r", rslt);
        }
        else if (rslt == BMP5_E_INVALID_POWERMODE)
        {
            printk("Error [%d] : Invalid powermode\r\n\r", rslt);
        }
        else
        {
            /* For more error codes refer "*_defs.h" */
            printk("Error [%d] : Unknown error code\r\n\r", rslt);
        }
    }
}

static const struct gpio_dt_spec bmpInt = GPIO_DT_SPEC_GET_OR(BMP_INT, gpios,{0});
static struct gpio_callback bmpInt_cb_data;

static void bmpDataReady(const struct device *dev, struct gpio_callback *cb,uint32_t pins)
{
	k_work_submit(&work_bmp);
}

static int8_t set_config(struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev)
{
    int8_t rslt;
    struct bmp5_iir_config set_iir_cfg;
    struct bmp5_int_source_select int_source_select;

    rslt = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, dev);
    bmp5_error_codes_print_result("bmp5_set_power_mode1", rslt);
    /**/
    if (rslt == BMP5_OK)
    {
        /* Get default odr */
        rslt = bmp5_get_osr_odr_press_config(osr_odr_press_cfg, dev);
        bmp5_error_codes_print_result("bmp5_get_osr_odr_press_config", rslt);

        if (rslt == BMP5_OK)
        {
            /* Set ODR as 50Hz */
            osr_odr_press_cfg->odr = BMP5_ODR_240_HZ;//irrelevant due to continous mode

            /* Enable pressure */
            osr_odr_press_cfg->press_en = BMP5_ENABLE;

            /* Set Over-sampling rate with respect to odr */
            uint8_t oversampling_t;
            if(bmp_data.odr <= BMP5_OVERSAMPLING_16X){
                oversampling_t = BMP5_OVERSAMPLING_1X;
            }else if (bmp_data.odr == BMP5_OVERSAMPLING_32X){
                oversampling_t = BMP5_OVERSAMPLING_2X;
            }else if (bmp_data.odr == BMP5_OVERSAMPLING_64X){
                oversampling_t = BMP5_OVERSAMPLING_4X;
            }else if (bmp_data.odr == BMP5_OVERSAMPLING_128X){
                oversampling_t = BMP5_OVERSAMPLING_8X;
            }
            
            osr_odr_press_cfg->osr_t = oversampling_t;
            osr_odr_press_cfg->osr_p = bmp_data.oversampling_p;

            rslt = bmp5_set_osr_odr_press_config(osr_odr_press_cfg, dev);
            bmp5_error_codes_print_result("bmp5_set_osr_odr_press_config", rslt);
        }

        if (rslt == BMP5_OK)
        {
            set_iir_cfg.set_iir_t = BMP5_IIR_FILTER_COEFF_1;
            set_iir_cfg.set_iir_p = BMP5_IIR_FILTER_COEFF_1;
            set_iir_cfg.shdw_set_iir_t = BMP5_ENABLE;
            set_iir_cfg.shdw_set_iir_p = BMP5_ENABLE;

            rslt = bmp5_set_iir_config(&set_iir_cfg, dev);
            bmp5_error_codes_print_result("bmp5_set_iir_config", rslt);
        }

        if (rslt == BMP5_OK)
        {
            rslt = bmp5_configure_interrupt(BMP5_PULSED, BMP5_ACTIVE_LOW , BMP5_INTR_OPEN_DRAIN, BMP5_INTR_ENABLE, dev);
            bmp5_error_codes_print_result("bmp5_configure_interrupt", rslt);

            if (rslt == BMP5_OK)
            {
                /* Note : Select INT_SOURCE after configuring interrupt */
                int_source_select.drdy_en = BMP5_ENABLE;
                rslt = bmp5_int_source_select(&int_source_select, dev);
                bmp5_error_codes_print_result("bmp5_int_source_select", rslt);
            }
        }

        /* Set powermode as normal */
       // rslt = bmp5_set_power_mode(BMP5_POWERMODE_NORMAL, dev);
        //bmp5_error_codes_print_result("bmp5_set_power_mode", rslt);

        rslt = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, dev);
    }

    return rslt;
}

static int8_t get_sensor_data(const struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev)
{
    int8_t rslt = 0;
    uint8_t int_status;
    struct bmp5_sensor_data sensor_data;


    rslt = bmp5_get_interrupt_status(&int_status, dev);
    bmp5_error_codes_print_result("bmp5_get_interrupt_status", rslt);

    if (int_status & BMP5_INT_ASSERTED_DRDY)
    {
        rslt = bmp5_get_sensor_data(&sensor_data, osr_odr_press_cfg, dev);
        bmp5_error_codes_print_result("bmp5_get_sensor_data", rslt);

        if (rslt == BMP5_OK)
        {
#ifdef BMP5_USE_FIXED_POINT
            printf("%lu, %ld\n\r", (long unsigned int)sensor_data.pressure,
                    (long int)sensor_data.temperature);
#else
            //printf("%f, %f\n\r", sensor_data.pressure, sensor_data.temperature);
#endif
        bmp_data.pressure = sensor_data.pressure/100.0;
        bmp_data.temperature = sensor_data.temperature;
        }
    }
    

    return rslt;
}

extern void send_data_bmp(void){
    uint8_t result = get_sensor_data(&osr_odr_press_cfg, &bmp581_dev);
    bmp5_error_codes_print_result("get_sensor_data", result);

    bmp_data.array[0+bmp_data.current_event*3]=bmp_data.pressure;
    bmp_data.array[1+bmp_data.current_event*3]=bmp_data.temperature;
    bmp_data.array[2+bmp_data.current_event*3]=(k_uptime_get()/1000.0)-global_timestamp;
    
    bmp_data.current_event++;
    if(bmp_data.current_event == bmp_data.max_events){
        send_data(SENSOR_BMP581_ID, &bmp_data.array, bmp_data.max_events*3*4);   
        printk("send data nr: %i \n\r",bmp_data.current_event);        
        bmp_data.current_event=0;
    }
}



extern int8_t init_bmp(){
    int8_t rslt = BMP5_OK;
    int16_t result;
    
    bmp_data.current_event=0;
    bmp_data.max_events=4;

    dev_addr = BMP5_I2C_ADDR_PRIM;
    bmp581_dev.read = bmp5_i2c_read;
    bmp581_dev.write = bmp5_i2c_write;
    bmp581_dev.intf = BMP5_I2C_INTF;
    bmp581_dev.intf_ptr = bmp_dev;
    bmp581_dev.chip_id = BMP5_CHIP_ID_PRIM;
    bmp581_dev.delay_us = bmp5_delay_us;
    bmp581_dev.intf_rslt = result;
    rslt = bmp5_soft_reset(&bmp581_dev);
    printk("bmp soft_reset %i\n\r",rslt);
    k_sleep(K_MSEC(150));
    //bmp5_error_codes_print_result("bmp5 init",rslt);
    rslt = bmp5_init(&bmp581_dev);
    //bmp5_error_codes_print_result("bmp5 init",rslt);
    //return rslt;
    rslt = set_config(&osr_odr_press_cfg, &bmp581_dev);
    bmp5_error_codes_print_result("set_config", rslt);
    //k_sleep(K_MSEC(150));
    //rslt = get_sensor_data(&osr_odr_press_cfg, &bmp581_dev);
    //bmp5_error_codes_print_result("get_sensor_data", rslt);

    k_work_init(&work_bmp, send_data_bmp);
	//k_work_init(&config_work_bmp, set_config_bmp);

    if (!device_is_ready(bmpInt.port)) {
		printk("Error: bmp interrupt %s is not ready\n\r",
		       bmpInt.port->name);
		return 1;
	}

	rslt = gpio_pin_configure_dt(&bmpInt, GPIO_INPUT);
	if (rslt != 0) {
		printk("Error %d: failed to configure %s pin %d\n\r",
		       rslt, bmpInt.port->name, bmpInt.pin);
		return rslt;
	}

	rslt = gpio_pin_interrupt_configure_dt(&bmpInt,GPIO_INT_EDGE_RISING);
	if (rslt != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n\r",
			rslt, bmpInt.port->name, bmpInt.pin);
		return rslt;
	}

	gpio_init_callback(&bmpInt_cb_data, bmpDataReady, BIT(bmpInt.pin));
	gpio_add_callback(bmpInt.port, &bmpInt_cb_data);
    return rslt;
}

extern uint8_t sleep_bmp(bool SLEEP){
    if(SLEEP){
            uint8_t rslt = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, &bmp581_dev);
    }else{
        bmp5_set_power_mode(BMP5_POWERMODE_CONTINOUS, &bmp581_dev);
    }    
}

void submit_config_bmp(){
    sleep_bmp(true);
    if(DEBUG){
        printf("BMP: setting config to\n");
        printf("enable: %d \n",bmp_data.config[0]);
    }
    bmp_data.oversampling_p = bmp_data.config[1];
    bmp_data.iir = bmp_data.config[2];
    osr_odr_press_cfg.osr_p=bmp_data.oversampling_p;
    bmi_data.event_number = 0;
    if(bmp_data.oversampling_p <= BMP5_OVERSAMPLING_2X){
        bmi_data.event_size = 10;
    }else if (bmp_data.oversampling_p <= BMP5_OVERSAMPLING_8X){
        bmi_data.event_size = 4;
    }else if (bmp_data.oversampling_p == BMP5_OVERSAMPLING_16X){
        bmi_data.event_size = 2;
    }else{
        bmi_data.event_size = 1;
    }
    
    set_config(&osr_odr_press_cfg, &bmp581_dev);

    if(bmp_data.config[0]){
        sleep_bmp(false);
    }else{
        sleep_bmp(true);
    }
    
}


