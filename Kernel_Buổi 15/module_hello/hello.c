#include <linux/module.h>

#define DRIVER_AUTHOR "DevLinux linuxfromscratch@gmail.com"
#define DRIVER_DESC "Hello world kernel module"
#define DRIVER_VERS "1.0"

/* Constructor */
static int __init chdev_init(void)
{
	pr_info("DevLinux: hello\n");
	return 0;
}

/* Destructor */
static void __exit chdev_exit(void)
{
	pr_info("DevLinux: goodbye\n");
}

module_init(chdev_init);
module_exit(chdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);