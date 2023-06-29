#include "mlx90392.h"

static const uint8_t mlx_rate[9]={  MLX_POWER_DOWN,
                                    CONTINUOUS_10,
                                    CONTINUOUS_20,
                                    CONTINUOUS_50,
                                    CONTINUOUS_100,
                                    CONTINUOUS_200,
                                    CONTINUOUS_500,
                                    CONTINUOUS_700,
                                    CONTINUOUS_1400
                                    };
static const int mlx_period_time_us[9] = {0,100000,50000,20000,10000,5000,2000,1429,714};

#define MLX_POWER_DOWN 0
#define CONTINUOUS_10 2
#define CONTINUOUS_20 3
#define CONTINUOUS_50 4
#define CONTINUOUS_100 5
#define CONTINUOUS_200 10
#define CONTINUOUS_500 11
#define CONTINUOUS_700 12
#define CONTINUOUS_1400 13

extern uint8_t init_mlx(){
    //go into power down mode
    sleep_mlx(true);

    k_work_init(&work_mlx, readout_sensor);
    k_work_init(&config_work_mlx, update_config_mlx);
    k_timer_init(&timer_mlx, mlxDataReady, NULL);

    //init struct
    mlx_data.event_number = 0;
    mlx_data.event_size = 30;
    mlx_data.mode = MLX_POWER_DOWN;
    write_byte(MLX_CONFIG1_REG,0x00);
    write_byte(MLX_CONFIG2_REG,0xB0);
    set_mode();
}

extern uint8_t readout_sensor(){
    //read status
    uint8_t ret;
    uint8_t status_reg;
    ret = read_bytes(MLX_STATUS_REG,1,&status_reg);
    //if new data is available
    if((status_reg >> 0) & 0x01){
        //New data ready!
        int8_t new_data[2*3+1];
        ret = read_bytes(MLX_DATA_REG,7,&new_data[0]);
        //if no data overrun or sensoroverflow
        if(new_data[6]==2){
            printf("data where skipped\n");
        }
        if(new_data[6]!=1){
            int16_t m_x= new_data[0] | (new_data[1] << 8);
            int16_t m_y= new_data[2] | (new_data[3] << 8);
            int16_t m_z= new_data[4] | (new_data[5] << 8);

            mlx_data.data_array[mlx_data.event_number*3]=m_x;
            mlx_data.data_array[1+mlx_data.event_number*3]=m_y;
            mlx_data.data_array[2+mlx_data.event_number*3]=m_z;
            mlx_data.event_number+=1;
            //if(DEBUG){printf("x: %i y: %i z: %i event: %i time: %i\n",m_x,m_y,m_z,mlx_data.event_number, k_uptime_get_32());};
            if(mlx_data.event_number == mlx_data.event_size){
                printf("send_data\n");
                send_data(SENSOR_MLX_ID, &mlx_data.data_array, 2*3*mlx_data.event_size);
                mlx_data.event_number=0;
            }
        }else{
            printf("overrun/sensoroverflow: %i\n",k_uptime_get_32());
        }
    }else{
        printf("data not ready: %i\n",k_uptime_get_32());
        return true;
    }
            
}

void mlxDataReady(){
	k_work_submit(&work_mlx);    
}

void update_config_mlx(){
    mlx_data.mode = mlx_data.config[1];
    if(DEBUG){
        printf("MLX: setting config to\n");
        printf("enable: %d \n",mlx_data.config[0]);
        printf("mode: %d \n",mlx_data.mode);
    }

    if(mlx_data.config[1] && mlx_data.mode > 0){
        set_mode();
        printf("period: %d \n",mlx_period_time_us[mlx_data.mode]);
        k_timer_start(&timer_mlx, K_USEC(mlx_period_time_us[mlx_data.mode]), K_USEC(mlx_period_time_us[mlx_data.mode]));
    }else{
        sleep_mlx(true);
    }
    
}

extern uint8_t sleep_mlx(bool SLEEP){
    if(SLEEP){
        k_timer_stop(&timer_mlx);
        write_byte(MLX_MODE_REG,MLX_POWER_DOWN);
    }else{
        write_byte(MLX_MODE_REG,mlx_data.mode);
    }    
}

uint8_t set_mode(){
    return write_byte(MLX_MODE_REG,mlx_rate[mlx_data.mode]);
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
