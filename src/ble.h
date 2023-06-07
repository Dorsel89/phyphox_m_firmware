#ifndef _BLE_H   /* Include guard */
#define _BLE_H

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/sys/byteorder.h>

#include "sensors.h"

#include "bmiZephyr.h"
#include "shtc3.h"

static uint8_t phyphox_data[20] = {0};
static uint8_t config_data[20] = {0};

void init_ble();
void send_data(uint8_t ID, float* DATA,uint8_t LEN);

static bool notify_enabled;
static void ccc_cfg_changed(const struct bt_gatt_attr *attr,
				 uint16_t value)
{
	notify_enabled = (value == BT_GATT_CCC_NOTIFY) ? 1 : 0;
}

static struct bt_uuid_128 data_service_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xcddf1001, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a));
//BMP
static struct bt_uuid_128 bmp_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xcddf1007, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a));
static struct bt_uuid_128 bmp_cnfg = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xcddf1008, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a)); 
//BMI
static struct bt_uuid_128 bmi_gyr_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xcddf1000, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a));
static struct bt_uuid_128 bmi_acc_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xcddf1001, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a));
static struct bt_uuid_128 bmi_cnfg = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xcddf1002, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a)); 

//SHTC3
static struct bt_uuid_128 shtc_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xcddf1005, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a));
static struct bt_uuid_128 shtc_cnfg = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xcddf1006, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a));

static struct bt_uuid_128 hardware_uuid = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xcddf1021, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a)); 
static struct bt_uuid_128 hardware_cnfg = BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xcddf1022, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a));

#endif