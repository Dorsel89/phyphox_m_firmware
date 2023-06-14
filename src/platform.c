/*
 Copyright (c) 2021, STMicroelectronics - All Rights Reserved

 This file : part of VL53L4CD Ultra Lite Driver and : dual licensed, either
 'STMicroelectronics Proprietary license'
 or 'BSD 3-clause "New" or "Revised" License' , at your option.

*******************************************************************************

 'STMicroelectronics Proprietary license'

*******************************************************************************

 License terms: STMicroelectronics Proprietary in accordance with licensing
 terms at www.st.com/sla0081

 STMicroelectronics confidential
 Reproduction and Communication of this document : strictly prohibited unless
 specifically authorized in writing by STMicroelectronics.


*******************************************************************************

 Alternatively, VL53L4CD Ultra Lite Driver may be distributed under the terms of
 'BSD 3-clause "New" or "Revised" License', in which case the following
 provisions apply instead of the ones mentioned above :

*******************************************************************************

 License terms: BSD 3-clause "New" or "Revised" License.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************
*/

#include "platform.h"


uint8_t VL53L4CD_RdDWord(Dev_t dev, uint16_t RegisterAdress, uint32_t *value)
{
	uint8_t status = 255;
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	uint8_t data_buffer[4];
	uint8_t addr_buffer[2];
	addr_buffer[1] = RegisterAdress & 0xFF;
	addr_buffer[0] = RegisterAdress >> 8;
	
	status = i2c_write(tof_dev,&addr_buffer[0],2,tof_i2c_address);
	if(status){
		return status;
	}
	status = i2c_read(tof_dev,&data_buffer[0],4,tof_i2c_address);

	uint32_t int32_buffer = data_buffer[3] | (data_buffer[2] << 8) | (data_buffer[1] << 16) | (data_buffer[0] << 24);
	memcpy(value,&int32_buffer,4);
	return status;
}

uint8_t VL53L4CD_RdWord(Dev_t dev, uint16_t RegisterAdress, uint16_t *value)
{
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	uint8_t data_buffer[2];
	uint8_t addr_buffer[2];
	addr_buffer[1] = RegisterAdress & 0xFF;
	addr_buffer[0] = RegisterAdress >> 8;

	//status = i2c_write_read(tof_dev,tof_i2c_address,&addr_buffer[0],2,&data_buffer[0],2);
	status = i2c_write(tof_dev,&addr_buffer[0],2,tof_i2c_address);
	if(status){
		return status;
	}
	status = i2c_read(tof_dev,&data_buffer[0],2,tof_i2c_address);
	
	uint16_t int16_buffer = data_buffer[1] | (data_buffer[0] << 8);
	memcpy(value,&int16_buffer,2);
	return status;
}

uint8_t VL53L4CD_RdByte(Dev_t dev, uint16_t RegisterAdress, uint8_t *value)
{
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
	uint8_t data_buffer[1];
	uint8_t addr_buffer[2];
	addr_buffer[1] = RegisterAdress & 0xFF;
	addr_buffer[0] = RegisterAdress >> 8;
	
	status = i2c_write(tof_dev,&addr_buffer[0],2,tof_i2c_address);
	if(status){
		return status;
	}
	status = i2c_read(tof_dev,&data_buffer[0],1,tof_i2c_address);

	memcpy(value,&data_buffer[0],1);
	return status;
}

uint8_t VL53L4CD_WrByte(Dev_t dev, uint16_t RegisterAdress, uint8_t value)
{
	uint8_t status = 255;

	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
    uint8_t dataBuffer[3];

    dataBuffer[0]=RegisterAdress >> 8;
	dataBuffer[1]=RegisterAdress & 0xFF;
    dataBuffer[2]=value;
	status = i2c_write(tof_dev, &dataBuffer, 3, tof_i2c_address);
	return status;
}

uint8_t VL53L4CD_WrWord(Dev_t dev, uint16_t RegisterAdress, uint16_t value)
{
	uint8_t status = 255;
	
	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
    uint8_t dataBuffer[4];
	uint8_t val_buffer[2];
	memcpy(&val_buffer[0],&value,2);

    dataBuffer[0]=RegisterAdress >> 8;
	dataBuffer[1]=RegisterAdress & 0xFF;
	dataBuffer[2]=val_buffer[1];
	dataBuffer[3]=val_buffer[0];
	
	status = i2c_write(tof_dev, &dataBuffer, 4, tof_i2c_address);
	return status;
}

uint8_t VL53L4CD_WrDWord(Dev_t dev, uint16_t RegisterAdress, uint32_t value)
{
	uint8_t status = 255;

	/* To be filled by customer. Return 0 if OK */
	/* Warning : For big endian platforms, fields 'RegisterAdress' and 'value' need to be swapped. */
    uint8_t dataBuffer[6];
	uint8_t val_buffer[4];
	memcpy(&val_buffer[0],&value,4);

    dataBuffer[0]=RegisterAdress >> 8;
	dataBuffer[1]=RegisterAdress & 0xFF;
	dataBuffer[2]=val_buffer[3];
	dataBuffer[3]=val_buffer[2];
	dataBuffer[4]=val_buffer[1];
	dataBuffer[5]=val_buffer[0];

	status = i2c_write(tof_dev, &dataBuffer, 6, tof_i2c_address);
	return status;
}

uint8_t WaitMs(Dev_t dev, uint32_t TimeMs)
{
	uint8_t status = 255;
	/* To be filled by customer */
	K_MSEC(TimeMs);
	return status;
}
