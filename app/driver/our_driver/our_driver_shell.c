#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

static int subcmd_fetch_get_handler(const struct shell * sh, int argc, char** argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);
    if(!dev)
    {
        shell_error(sh, "Could not find device %s", argv[1]);
        return -EFAULT;
    }

    int ret = sensor_sample_fetch(dev);

    if (ret!=0) 
    {
        shell_error(sh, "Could not fetch channel, got %d", ret);
        return -EFAULT;
    
    }

    shell_info(sh, " Hello from Fetch command");

    return 0;
}

static int subcmd_read_get_handler(const struct shell *sh, int argc, char** argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);
    if(!dev)
    {
        shell_error(sh, "Could not find device %s", argv[1]);
        return -EFAULT;
    }

    struct sensor_value value;

    int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &value);

    if (ret!=0) 
    {
        shell_error(sh, "Could not get the channel, got %d", ret);
        return -EFAULT;
    
    }

    shell_info(sh,"The value received is %d", value.val1);

    shell_info(sh, " Hello from get command");

    return 0;

}

static int subcmd_info_get_handler(const struct shell *sh, int argc, char** argv)
{
    const char *dev_name = argv[1];
    const struct device *dev = shell_device_get_binding(dev_name);
    if (!dev) {
        shell_error(sh, "Could not find device %s", dev_name);
        return -ENOENT;
    }
    //shell_print(shell, "This command would print information about %s driver", dev_name);
    shell_info(sh, "Device name: %s, Ready state: %s", dev->name, device_is_ready(dev) ? "ready" : "not ready");

    return 0;

}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcmd,
    SHELL_CMD_ARG(fetch, NULL, "Calls sensor_sample_fetch to Turn ON LED", subcmd_fetch_get_handler, 2, 0),
    SHELL_CMD_ARG(read, NULL, "Calls sensor sample get to Turn OFF LED", subcmd_read_get_handler, 2, 0),
    SHELL_CMD_ARG(info, NULL, "Info to the driver", subcmd_info_get_handler, 2, 0),
    SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(sensor, &sensor_subcmd, "Get status for our IO sesnor", NULL);
