#ifndef _LEDS_H
#define _LEDS_H
#include <zephyr/drivers/gpio.h>


#define SLEEP_TIME_MS   1000
#define LED_B_NODE DT_ALIAS(led_blue)
#define LED_G_NODE DT_ALIAS(led_green)
#define LED_R_NODE DT_ALIAS(led_red)

static const struct gpio_dt_spec ledB = GPIO_DT_SPEC_GET(LED_B_NODE, gpios);
static const struct gpio_dt_spec ledG = GPIO_DT_SPEC_GET(LED_G_NODE, gpios);
static const struct gpio_dt_spec ledR = GPIO_DT_SPEC_GET(LED_R_NODE, gpios);

static void my_work_handler(struct k_work *work);

#endif