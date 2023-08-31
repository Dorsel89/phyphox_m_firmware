#include "bas.h"

static int16_t m_sample_buffer[BUFFER_SIZE];
static bool* usb_status = NULL;
float getVoltage(){

	int ret;
	const struct adc_sequence sequence = {
		.channels = BIT(ADC_1ST_CHANNEL_ID),
		.buffer = m_sample_buffer,
		.buffer_size = sizeof(m_sample_buffer),
		.resolution = ADC_RESOLUTION,
		.oversampling = 4,
	};

	if (!adc_dev) {
		return -1;
	}

	ret = adc_read(adc_dev, &sequence);
	if (ret) {
        printk("adc_read() failed with code %d\n", ret);
	}
	float val;
	for (int i = 0; i < BUFFER_SIZE; i++) {
                //printk("ADC raw value: %d\n", m_sample_buffer[i]);
				//3.6 is max voltage with internal_ref (0.6V) and 1/6 Gain.
                val = (float)m_sample_buffer[i]*3.6/pow(2,ADC_RESOLUTION);
	}
	battery_level(val);
	return val;
}

static const struct adc_channel_cfg m_1st_channel_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = ADC_1ST_CHANNEL_ID,
#if defined(CONFIG_ADC_CONFIGURABLE_INPUTS)
	.input_positive = ADC_1ST_CHANNEL_INPUT,
#endif
};


extern void init_BAS(bool* usb_status_pointer){
	
	int err = 0;
	usb_status = usb_status_pointer;
	adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc));
	if(adc_dev == NULL){
		printf("issue..\n");
	}
	
	err = adc_channel_setup(adc_dev, &m_1st_channel_cfg);
    if (err) {
	    printk("Error in adc setup: %d\n", err);
	}
	
	//init periodic updates
    
	k_work_init(&work_bas, update_supercap_level);
	k_timer_init(&timer_bas, time_to_update_battery_service,NULL);
	k_timer_start(&timer_bas,K_SECONDS(1),K_SECONDS(10));
    
};

void update_supercap_level(){
	set_supercap_level(battery_level(getVoltage()));
};

void time_to_update_battery_service(){
	k_work_submit(&work_bas);
}

uint8_t battery_level(float adc_voltage){
	int bat;
	float cap_voltage = adc_voltage;
	//boost-buck converter works between 0.6V to 5V
	//energy: 0.5*5F*voltage^2
	float max_energy = 0.5*5*(pow(2.9,2)-pow(0.65,2));
	float current_energy = 0.5*5*(pow(cap_voltage,2)-pow(0.65,2));
	bat = (uint8_t)current_energy*100/max_energy;
	printf("adc: %f cap: %f maxE: %f currentE: %f bat: %i\n",adc_voltage,cap_voltage,max_energy,current_energy, bat);
	if(*usb_status && bat>=90){
		gpio_pin_configure_dt(&ledB, GPIO_OUTPUT_INACTIVE);
		gpio_pin_configure_dt(&ledG, GPIO_OUTPUT_ACTIVE);

	}
	return bat;
};


