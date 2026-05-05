#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>

/* Dynamic (runtime-mutable) data stored per device instance */
struct our_sensor_driver_data {
    int toggle_count;
};

/**
 * @brief Custom extension API: get the number of times the LED has been toggled.
 *
 * @param dev  Pointer to the device.
 * @return Current toggle count.
 */
int our_driver_get_toggle_count(const struct device *dev);

/**
 * @brief Custom extension API: reset the toggle count back to zero.
 *
 * @param dev  Pointer to the device.
 * @return 0 on success.
 */
int our_driver_reset_toggle_count(const struct device *dev);

#endif /* OUR_DRIVER_H */
