#include <linux/module.h>          /* Defines functions such as module_init/module_exit */
#include <linux/gpio.h>            /* Defines functions such as gpio_request/gpio_free */
#include <linux/platform_device.h> /* For platform devices */
#include <linux/gpio/consumer.h>   /* For GPIO Descriptor */
#include <linux/of.h>              /* For DT */

#define LOW    0
#define HIGH   1

struct gpio_desc *gpio_27;

static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "gpio-descriptor-based", },
    { /* sentinel */ }
};

static int mgpio_driver_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;

    // Set GPIO27 to HIGH
    gpio_27 = gpiod_get(dev, "led27", GPIOD_OUT_LOW);
    gpiod_set_value(gpio_27, HIGH);

    pr_info("%s - %d", __func__, __LINE__);

    return 0;
}

static int mgpio_driver_remove(struct platform_device *pdev)
{
    // Set GPIO27 to LOW before exiting
    gpiod_set_value(gpio_27, LOW);
    gpiod_put(gpio_27);

    pr_info("%s - %d", __func__, __LINE__);

    return 0;
}

static struct platform_driver mgpio = {
    .probe = mgpio_driver_probe,
    .remove = mgpio_driver_remove,
    .driver = {
        .name = "gpio-descriptor-based",
        .of_match_table = of_match_ptr(gpiod_dt_ids),
        .owner = THIS_MODULE,
    },
};

module_platform_driver(mgpio);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DevLinux");
MODULE_DESCRIPTION("Descriptor-Based GPIO Driver for Raspberry Pi Zero W");