#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

extern "C" {
#include "our_driver.h"
}

#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
// #define LED_NODE DT_ALIAS(app_led)

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

const struct device *led = DEVICE_DT_GET(DT_NODELABEL(my_sensor));

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    if(!device_is_ready(led)) {
        LOG_ERR("LED device is not ready");
        return 0;
    }

    struct sensor_value value;

    while (1)
    {
        sensor_sample_fetch(led);
        LOG_INF("LED set using the fetch API");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
        sensor_channel_get(led, SENSOR_CHAN_ALL, &value);
        LOG_INF("LED reset using the channel get API");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        LOG_INF("Toggle count: %d", our_driver_get_toggle_count(led));

        /* Reset the toggle count every 10 toggles using the custom extension API */
        if (our_driver_get_toggle_count(led) >= 10) {
            our_driver_reset_toggle_count(led);
        }
    }
    return 0;
}
