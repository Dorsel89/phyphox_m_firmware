# phyphox:m
The sensorbox phyphox:m is the next version of the so called satelite-box. The following sensors are available:
- Temperature, Humidity ([SHTC3](https://sensirion.com/media/documents/643F9C8E/63A5A436/Datasheet_SHTC3.pdf))
- Pressure ([BMP581](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp581-ds004.pdf))
- Acceleration, Gyroscope ([BMI323](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmi323-ds000.pdf))
- Magnetometer ([MLX90393](https://www.melexis.com/en/documents/documentation/datasheets/datasheet-mlx90393))
- Light ([VEML6030](https://www.vishay.com/docs/84366/veml6030.pdf))
- Distance ([VL53L4CD](https://www.st.com/resource/en/datasheet/vl53l4cd.pdf))
  
## phyphox-Experiments
TODO
## Additional experiment material
TODO
## Configuration
Each sensor can be configured via a specific charateristic. The respective uuids and options of each sensor are listed below.

### Temperature/Humidity [SHTC3](https://www.mouser.com/datasheet/2/682/Sensirion_04202018_HT_DS_SHTC3_Preliminiary_D2-1323493.pdf)<a name="SHTC3"></a> ###
data charateristic | |    
-------------------|-----
uuid          | cddf1005-30f7-4671-8b43-5e40ba53514a
byte 0-3  |       | temperature (float32LittleEndian)
byte 4-7          | humidity (float32LittleEndian)
byte 8-11         | timestamp (float32LittleEndian)

config charateristic | |    
-------------------|-----
config        | cddf1006-30f7-4671-8b43-5e40ba53514a
byte 0          | enable (bool)
byte 1          | measurement interval in 10ms (uint8) 

Example: The configuration value 0x0104 will enable the SHTC3 sensor with a measurement interval of 40ms.

### Distance [VL53L4CD](https://www.st.com/resource/en/datasheet/vl53l4cd.pdf)<a name="VL53L4CD"></a> ###
data charateristic | |    
-------------------|-----
uuid          | cddf1013-30f7-4671-8b43-5e40ba53514a
byte 0-3  |       | distance (uInt16) 
byte 4-7          | standart deviation of distance value (uInt16)
byte 4-7          | timestamp (float32LittleEndian)

Data is repeating every 8 byte.

config charateristic | |    
-------------------|-----
config        | cddf1014-30f7-4671-8b43-5e40ba53514a
byte 0          | enable (bool)
byte 1          | measurement interval in 10ms (uint8) 

### Pressure, Temperature [BMP581](https://www.st.com/resource/en/datasheet/vl53l4cd.pdf)<a name="VL53L4CD"></a> ###
data charateristic | |    
-------------------|-----
uuid          | cddf1013-30f7-4671-8b43-5e40ba53514a
byte 0-3  |       | pressure (float32LittleEndian) 
byte 4-7          | temperature (float32LittleEndian)
byte 8-11         | timestamp (float32LittleEndian)

Data is repeating every 12 byte.

config charateristic | |    
-------------------|-----
config        | cddf1014-30f7-4671-8b43-5e40ba53514a
byte 0          | enable (bool)
byte 1          | pressure oversampling rate (uint8) 
byte 2          | iir filter (uint8)

oversampling rate | |
-----------------|------
1x | 0x00
2x | 0x01
4x | 0x02
8x | 0x03
16x | 0x04
32x | 0x05
64x | 0x06
128x | 0x07

IIR filter | |
-----------------|------
bypass | 0x00
coefficient 1 | 0x01
coefficient 3 | 0x02
coefficient 7 | 0x03
coefficient 15 | 0x04
coefficient 31 | 0x05
coefficient 63 | 0x06
coefficient 127 | 0x07


## Flash new board with specific serial number

```console
nrfjprog --eraseall
nrfjprog --memwr 0x10001080 --val 123
nrfjprog --program merged.hex --verify
```
