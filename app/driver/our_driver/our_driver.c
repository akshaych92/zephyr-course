#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);
#define DT_DRV_COMPAT our_driver

struct our_sensor_driver_config
{
    struct gpio_dt_spec gpio;
};

static int our_driver_init(const struct device *dev)
{
    const struct our_sensor_driver_config *config = dev->config;
    if (!device_is_ready(config->gpio.port)) {
        LOG_ERR("GPIO device not ready");
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&config->gpio, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure GPIO pin");
        return ret;
    }


    LOG_INF("%s Initializing our driver", dev->name);
    return 0;
}

static int our_driver_led_off_fetch(const struct device *dev, enum sensor_channel chan)
{
    const struct our_sensor_driver_config *config = dev->config;
    gpio_pin_set_dt(&config->gpio, 1);

    LOG_INF("Turning ON LED from our driver");
    return 0;
}

static int our_driver_led_on_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    const struct our_sensor_driver_config *config = dev->config;
    gpio_pin_set_dt(&config->gpio, 0);

    val->val1 = 0;
    val->val2 = 0;

    LOG_INF("Turning OFF LED from our driver");
    return 0;
}

static DEVICE_API(sensor, api_akshay_practice) = {
    .sample_fetch = our_driver_led_off_fetch,
    .channel_get = our_driver_led_on_channel_get,
};

#define DEVICE_INST(inst) static const struct our_sensor_driver_config our_driver_config_##inst = { \
    .gpio = GPIO_DT_SPEC_INST_GET(inst, gpios) \
}; \
DEVICE_DT_INST_DEFINE(inst, our_driver_init, NULL, NULL, &our_driver_config_##inst, POST_KERNEL, 80, &api_akshay_practice);
DT_INST_FOREACH_STATUS_OKAY(DEVICE_INST);

