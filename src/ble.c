#include "ble.h"

BMI bmi_data;
VEML veml_data;
TOF tof_data;
MLX mlx_data;

static const struct bt_le_adv_param adv_param_normal = {
	.options = BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_NAME,
	.interval_min = BT_GAP_ADV_SLOW_INT_MIN,
	.interval_max = BT_GAP_ADV_SLOW_INT_MAX,
};

static const struct bt_le_conn_param conn_paramter = {
	.interval_min = 6,
	.interval_max = 200,
	.latency = 0,
	.timeout = 10
};

static ssize_t read_u16(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	uint8_t *value = attr->user_data;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(value));
}

static ssize_t config_submits(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset,uint8_t flags)
{
	uint8_t *value = attr->user_data;
	if (offset + len > sizeof(config_data)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}
	memcpy(value + offset, buf, len);
	/*
	if(attr->uuid == &bmp_cnfg.uuid){
		submit_config_bmp();
	}
	*/
	if(attr->uuid == &bmi_cnfg.uuid){
		submit_config_bmi();
	}
	if(attr->uuid == &shtc_cnfg.uuid){
		submit_config_shtc();
	}
	if(attr->uuid == &tof_cnfg.uuid){
		update_config_tof();
	}
	if(attr->uuid == &veml_cnfg.uuid){
		update_config_veml();
	}
	if(attr->uuid == &mlx_cnfg.uuid){
		update_config_mlx();
	}	
	return len;
};

BT_GATT_SERVICE_DEFINE(phyphox_gatt, 
	BT_GATT_PRIMARY_SERVICE(&data_service_uuid),
	//BMP384 
	BT_GATT_CHARACTERISTIC(&bmp_uuid,					
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
			       read_u16, NULL, &bmp_data.array[0]),
	BT_GATT_CCC(ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
//SHTC
	BT_GATT_CHARACTERISTIC(&shtc_uuid,					
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
			       read_u16, NULL, &shtc_data.array[0]),
	BT_GATT_CCC(ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(&shtc_cnfg,					
			       BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_WRITE,
			       NULL, config_submits, &shtc_data.config[0]),
	BT_GATT_CCC(ccc_cfg_changed,	//notification handler
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
//TOF
	BT_GATT_CHARACTERISTIC(&tof_uuid,					
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
			       read_u16, NULL, &tof_data.data_array[0]),
	BT_GATT_CCC(ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(&tof_cnfg,					
			       BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_WRITE,
			       NULL, config_submits, &tof_data.config[0]),
	BT_GATT_CCC(ccc_cfg_changed,	//notification handler
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),								
//VEML
	BT_GATT_CHARACTERISTIC(&veml_uuid,					
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
			       read_u16, NULL, &veml_data.data_array[0]),
	BT_GATT_CCC(ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(&veml_cnfg,					
			       BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_WRITE,
			       NULL, config_submits, &veml_data.config[0]),
	BT_GATT_CCC(ccc_cfg_changed,	//notification handler
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
//MLX
	BT_GATT_CHARACTERISTIC(&mlx_uuid,					
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
			       read_u16, NULL, &mlx_data.data_array[0]),
	BT_GATT_CCC(ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(&mlx_cnfg,					
			       BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_WRITE,
			       NULL, config_submits, &mlx_data.config[0]),
	BT_GATT_CCC(ccc_cfg_changed,	//notification handler
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),						
//BMI323 - GYROSCOPE
	BT_GATT_CHARACTERISTIC(&bmi_gyr_uuid,					
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
			       read_u16, NULL, &bmi_data.gyr_array[0]),
	BT_GATT_CCC(ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
BT_GATT_CHARACTERISTIC(&bmi_acc_uuid,					
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
			       read_u16, NULL, &bmi_data.acc_array[0]),
	BT_GATT_CCC(ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
			
    BT_GATT_CHARACTERISTIC(&bmi_cnfg,					
			       BT_GATT_CHRC_WRITE | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_WRITE,
			       NULL, config_submits, &bmi_data.config[0]),
				   
	BT_GATT_CCC(ccc_cfg_changed,	//notification handler
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)		
);
// PRIMARY ADVERTISING DATA
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,
		BT_UUID_16_ENCODE(BT_UUID_BAS_VAL)), //battery service
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, 
		BT_UUID_128_ENCODE(0xcddf1001, 0x30f7, 0x4671, 0x8b43, 0x5e40ba53514a))
};
// SCAN RESPONSE DATA
static const struct bt_data sd[] = {
BT_DATA_BYTES(BT_DATA_UUID128_ALL,
		      0x84, 0xaa, 0x60, 0x74, 0x52, 0x8a, 0x8b, 0x86,
		      0xd3, 0x4c, 0xb7, 0x1d, 0x1d, 0xdc, 0x53, 0x8d),
};

static void bt_ready(void)
{
	int err;

	printk("Bluetooth initialized\n\r");

	err = bt_le_adv_start(&adv_param_normal, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		printk("Advertising failed to start (err %d)\n\r", err);
		return;
	}

	printk("Advertising successfully started\n\r");
}

static void connected(struct bt_conn *conn, uint8_t err)
{
	printk("Device with index %i trying to connect...\n\r",bt_conn_index(conn));
	
	bt_conn_le_param_update(conn,&conn_paramter);
	if (err) {
		printk("Connection failed (err 0x%02x)\n\r", err);
	} else {
		printk("Connected\n\r");
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason 0x%02x)\n\r", reason);
	sleep_bmi(true);
	sleep_shtc(true);
	sleep_tof(true);
	sleep_veml(true);
	sleep_mlx(true);
	
}
static void le_param_updated(struct bt_conn *conn, uint16_t interval,
			     uint16_t latency, uint16_t timeout){
	printk("Connection parameters updated.\n\r"
	       " interval: %d, latency: %d, timeout: %d\n\r",
	       interval, latency, timeout);
}
static void le_phy_updated(struct bt_conn *conn,struct bt_conn_le_phy_info *param){
	if(DEBUG){
		printk("LE PHY updated\n\r");
	}
}
static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param){
	if(DEBUG){
		printk("Connection parameters update request received.\n\r");
		printk("Minimum interval: %d, Maximum interval: %d\n\r",
	       param->interval_min, param->interval_max);
		printk("Latency: %d, Timeout: %d\n\r", param->latency, param->timeout);
	}	
	return true;
}
static void le_data_len_updated(struct bt_conn *conn, struct bt_conn_le_data_len_info *info)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
	if(DEBUG){
		printk("Data length updated: %s max tx %u (%u us) max rx %u (%u us)\n\r",
           addr, info->tx_max_len, info->tx_max_time, info->rx_max_len,
           info->rx_max_time);
		   }
}
static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
	.le_param_req = le_param_req,
	.le_param_updated = le_param_updated,
	.le_phy_updated = le_phy_updated,
	.le_data_len_updated = le_data_len_updated,

};

void init_ble(){
	bt_enable(NULL);
	bt_ready();
	bt_conn_cb_register(&conn_callbacks);
};

extern void set_supercap_level(uint8_t val){
	bt_bas_set_battery_level(val);
}

extern void send_data(uint8_t ID, float* DATA,uint8_t LEN){
	if(ID == SENSOR_BMP581_ID){
		bt_gatt_notify_uuid(NULL, &bmp_uuid.uuid,&phyphox_gatt.attrs[0],DATA,LEN);
		return;
	}
	if(ID == SENSOR_BMI323_GYR_ID){
		bt_gatt_notify_uuid(NULL, &bmi_gyr_uuid.uuid,&phyphox_gatt.attrs[0],DATA,LEN);
		return;
	}
	if(ID == SENSOR_BMI323_ACC_ID){
		bt_gatt_notify_uuid(NULL, &bmi_acc_uuid.uuid,&phyphox_gatt.attrs[0],DATA,LEN);
		return;
	}
	if (ID == SENSOR_SHTC_ID)
	{
		bt_gatt_notify_uuid(NULL, &shtc_uuid.uuid,&phyphox_gatt.attrs[0],DATA,LEN);
		return;
	}
	if (ID == SENSOR_TOF_ID)
	{
		bt_gatt_notify_uuid(NULL, &tof_uuid.uuid,&phyphox_gatt.attrs[0],DATA,LEN);
		return;
	}
	if (ID == SENSOR_VEML_ID)
	{
		bt_gatt_notify_uuid(NULL, &veml_uuid.uuid,&phyphox_gatt.attrs[0],DATA,LEN);
		return;
	}
	if (ID == SENSOR_MLX_ID)
	{
		bt_gatt_notify_uuid(NULL, &mlx_uuid.uuid,&phyphox_gatt.attrs[0],DATA,LEN);
		return;
	}			
};