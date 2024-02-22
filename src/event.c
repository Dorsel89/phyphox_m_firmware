#include "event.h"

extern void phyphox_event_received(){
    printf("PHYPHOX EVENTHANDLER\r\n");
    if(event_data.config[0]==0xff){
        global_timestamp = k_uptime_ticks()/32768.0;  
    }
};