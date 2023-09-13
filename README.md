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
## Flash new board with specific serial number

```console
nrfjprog --eraseall
nrfjprog --memwr 0x10001080 --val 123
nrfjprog --program merged.hex --verify
```
