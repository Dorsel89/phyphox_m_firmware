#include "mlx90392.h"

extern uint8_t init_mlx(){
    //go into power down mode
    sleep_mlx(true);
    //init struct
    mlx_data.event_number = 0;
    mlx_data.event_size = 1;
    mlx_data.mode = MLX_POWER_DOWN;
    

    mlx_data.mode = CONTINUOUS_50;
    set_mode();
}

extern uint8_t readout_sensor(){
    //read status
    uint8_t ret;
    uint8_t status_reg;
    ret = read_bytes(MLX_STATUS_REG,1,&status_reg);
    if(DEBUG){
        printk("mlx: status reg: %i \n\r",status_reg);
    }
    //if new data is available
    if((status_reg >> 0) & 0x01){
        //New data ready!
        uint8_t new_data[2*3];
        ret = read_bytes(MLX_DATA_REG,6,&new_data[0]);
        if(DEBUG){
            printk("mlx: data: %i %i \n\r",new_data[0],new_data[1]);
        }
    }else{
        return true;
    }
            
}

extern uint8_t sleep_mlx(bool SLEEP){
    if(SLEEP){
        write_byte(MLX_MODE_REG,MLX_POWER_DOWN);
    }else{
        write_byte(MLX_MODE_REG,mlx_data.mode);
    }    
}

uint8_t set_mode(){
    return write_byte(MLX_MODE_REG,mlx_data.mode);;
}

static uint8_t read_bytes( uint8_t reg_address, uint8_t bytes, uint8_t* dest){
    uint8_t ret;
    ret = i2c_burst_read(mlx_dev,mlx_i2c_address,reg_address,dest,bytes);
}
static uint8_t write_byte(uint8_t reg_address, uint8_t data){
    uint8_t ret;
    uint8_t dataBuffer[2];
    dataBuffer[0]=reg_address;
    dataBuffer[1]=data;
    return i2c_write(mlx_dev, &dataBuffer, 2, mlx_i2c_address);
}
