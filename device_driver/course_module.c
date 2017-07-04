#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/miscdevice.h>

#define COURSE_STRING "KERNEL_COURSE"

static ssize_t course_read(struct file *filp, char __user *buf, size_t count
				, loff_t *ppos)
{
	return simple_read_from_buffer(buf, count, ppos, COURSE_STRING
					, strlen(COURSE_STRING));
}

static ssize_t course_write(struct file *filp, const char __user *buf
				, size_t count, loff_t *pos)
{
	char val[14];
	int ret;
	memset(val, 0, sizeof(val));

	ret = simple_write_to_buffer(val, sizeof(val) - 1, pos, buf, count);
	if (ret < 0)
		return ret;

	pr_info("val: %s\n", val);
	if (strncmp(val, COURSE_STRING, 13))
		return -EINVAL;

	return ret;
}

struct file_operations course_fops = {
	.read = course_read,
	.write = course_write
};

struct miscdevice course_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "course_module",
	.fops = &course_fops,
	.mode = 0777
};

static int __init start_driver(void)
{
	pr_info("Starting driver...\n");
	return misc_register(&course_misc);
}

static void __exit finish_driver(void)
{
	pr_info("Removing driver...\n");
	misc_deregister(&course_misc);
}

module_init(start_driver);
module_exit(finish_driver);
MODULE_AUTHOR("Marcos Paulo de Souza <marcos.souza.org@gmail.com>");
MODULE_LICENSE("GPL");
