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
data | |    
-------------------|-----
uuid          | cddf1005-30f7-4671-8b43-5e40ba53514a
byte 0-3          | temperature (float32LittleEndian)
byte 4-7          | humidity (float32LittleEndian)
byte 8-11         | timestamp (float32LittleEndian)

config | |    
-------------------|-----
uuid        | cddf1006-30f7-4671-8b43-5e40ba53514a
byte 0          | enable (bool)
byte 1          | measurement interval in 10ms (uint8) 

Example: The configuration value 0x0104 will enable the SHTC3 sensor with a measurement interval of 40ms.

### Distance [VL53L4CD](https://www.st.com/resource/en/datasheet/vl53l4cd.pdf)<a name="VL53L4CD"></a> ###
data | |    
-------------------|-----
uuid          | cddf1013-30f7-4671-8b43-5e40ba53514a
byte 0-1          | distance (uInt16) 
byte 2-3          | standart deviation of distance value (uInt16)
byte 4-7          | timestamp (float32LittleEndian)

Data is repeating every 8 byte.

config | |    
-------------------|-----
uuid        | cddf1014-30f7-4671-8b43-5e40ba53514a
byte 0          | enable (bool)
byte 1          | measurement interval in 10ms (uint8) 

### Pressure, Temperature [BMP581](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp581-ds004.pdf)<a name="BMP581"></a> ###
data | |    
-------------------|-----
uuid          | cddf1013-30f7-4671-8b43-5e40ba53514a
byte 0-3          | pressure (float32LittleEndian) 
byte 4-7          | temperature (float32LittleEndian)
byte 8-11         | timestamp (float32LittleEndian)

Data is repeating every 12 byte.

config | |    
-------------------|-----
uuid        | cddf1014-30f7-4671-8b43-5e40ba53514a
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

### Light [VEML6030](https://www.vishay.com/docs/84366/veml6030.pdf)<a name="VEML6030"></a> ###
data | |    
-------------------|-----
uuid          | cddf1015-30f7-4671-8b43-5e40ba53514a
byte 0-3          | temperature (float32LittleEndian)
byte 4-7          | humidity (float32LittleEndian)
byte 8-11         | timestamp (float32LittleEndian)

config | |    
-------------------|-----
uuid        | cddf1016-30f7-4671-8b43-5e40ba53514a
byte 0          | enable (bool)
byte 1          | integration time (uint8) 
byte 2          | gain (uint8) 

integration time | |
-----------------|------
25ms | 0x12
50ms | 0x08
100ms | 0x00
200ms | 0x01
400ms | 0x02
800ms | 0x03

gain | |
-----------------|------
1x | 0x00
2x | 0x01
1/8x | 0x02
1/4x | 0x03

### MLX90393 [MLX90393](https://media.melexis.com/-/media/files/documents/datasheets/mlx90393-datasheet-melexis.pdf)<a name="MLX90393"></a> ###
data | |    
-------------------|-----
uuid          | cddf1009-30f7-4671-8b43-5e40ba53514a
byte 0-3          | magnetometer x (float32LittleEndian)
byte 4-7          | magnetometer y (float32LittleEndian)
byte 8-11         | magnetometer z (float32LittleEndian)
byte 12-15         | timestamp (float32LittleEndian)

config | |    
-------------------|-----
uuid        | cddf100a-30f7-4671-8b43-5e40ba53514a
byte          | settings
0    | enable magnetometer
1    | uint8_t gain-mode: <br> 0: 5x <br> 1: 4x <br> 2: 3x <br> 3: 2.5x <br> 4: 2x <br> 5: 1.667x <br> 6: 1.333x <br> 7: 1x
2    | uint8_t digital filter: 0-7
3    | uint8_t oversampling: 0-3
4    | uint8_t x resolution: 0-3
5    | uint8_t y resolution: 0-3
6    | uint8_t z resolution: 0-3

integration time | |
-----------------|------
25ms | 0x12
50ms | 0x08
100ms | 0x00
200ms | 0x01
400ms | 0x02
800ms | 0x03

## Flash new board with specific serial number

```console
nrfjprog --eraseall
nrfjprog --memwr 0x10001080 --val 123
nrfjprog --program merged.hex --verify
```
