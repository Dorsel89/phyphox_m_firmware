#include "i2c_zephyr.h"

uint8_t write_bytes(uint8_t reg_addr, uint8_t *data, int length, uint8_t device_address){
        return i2c_burst_write(tw_dev,device_address,reg_addr,data,length);
}
uint8_t read_bytes(uint8_t reg_addr, uint8_t *data, int length, uint8_t device_address){
    return i2c_burst_read(tw_dev,device_address,reg_addr,data,length);
}



    
