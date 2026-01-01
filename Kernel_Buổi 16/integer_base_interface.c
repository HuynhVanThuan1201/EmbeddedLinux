#include <linux/module.h>
#include <linux/gpio.h>

#define GPIO_NUMBER_27 27
#define LOW            0
#define HIGH           1

static int __init mgpio_driver_init(void)
{
    // Configure GPIO27 as output
    gpio_request(GPIO_NUMBER_27, "gpio_27");
    gpio_direction_output(GPIO_NUMBER_27, LOW);

    // Set GPIO27 to HIGH
    gpio_set_value(GPIO_NUMBER_27, HIGH);
    pr_info("GPIO27 set to HIGH, status: %d!\n", gpio_get_value(GPIO_NUMBER_27));

    return 0;
}

static void __exit mgpio_driver_exit(void)
{
    // Set GPIO27 to LOW before exiting
    gpio_set_value(GPIO_NUMBER_27, LOW);
    gpio_free(GPIO_NUMBER_27);
    pr_info("GPIO27 set to LOW\n");
}