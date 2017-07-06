#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/miscdevice.h>
#include <linux/sysfs.h>

#define COURSE_STRING "KERNEL_COURSE"

int access;
int errors;
struct kobject *kobj;
DEFINE_MUTEX(access_mtx);

static ssize_t course_read(struct file *filp, char __user *buf, size_t count
				, loff_t *ppos)
{
	int ret;
	ret = simple_read_from_buffer(buf, count, ppos, COURSE_STRING
					, strlen(COURSE_STRING));
	mutex_lock(&access_mtx);
	if (ret < 0)
		errors++;
	/* avoid duplicates */
	else if (ret > 0)
		access++;
	mutex_unlock(&access_mtx);
	pr_info("read return %d\n", ret);
	return ret;
}

static ssize_t course_write(struct file *filp, const char __user *buf
				, size_t count, loff_t *pos)
{
	char val[14];
	int ret;
	memset(val, 0, sizeof(val));

	ret = simple_write_to_buffer(val, sizeof(val) - 1, pos, buf, count);
	if (ret < 0)
		goto out;

	pr_info("val: %s\n", val);
	if (strncmp(val, COURSE_STRING, 13)) {
		ret = -EINVAL;
		goto out;
	}
out:

	mutex_lock(&access_mtx);
	if (ret < 0)
		errors++;
	else
		access++;
	mutex_unlock(&access_mtx);
	return ret;
}

static ssize_t access_show(struct kobject *kobj, struct kobj_attribute *attr
			, char *buf)
{
	return sprintf(buf, "%d\n", access);
}

static ssize_t errors_show(struct kobject *kobj, struct kobj_attribute *attr
			, char *buf)
{
	return sprintf(buf, "%d\n", errors);
}

static ssize_t reset_store(struct kobject *kobj, struct kobj_attribute *attr
			, const char *buf, size_t count)
{
	int ret, val;
	ret = kstrtoint(buf, 0, &val);
	if (ret)
		return ret;

	if (val != 1)
		return -EINVAL;

	mutex_lock(&access_mtx);
	access = 0;
	errors = 0;
	mutex_unlock(&access_mtx);

	return count;
}

struct kobj_attribute kaccess = __ATTR_RO(access);
struct kobj_attribute kerrors = __ATTR_RO(errors);
struct kobj_attribute kreset = __ATTR_WO(reset);
struct attribute *attrs[] = {
	&kaccess.attr,
	&kerrors.attr,
	&kreset.attr,
	NULL
};

struct attribute_group attr_group = {
	.attrs = attrs
};

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
	int ret;

	pr_info("Starting driver...\n");
	access = 0;

	kobj = kobject_create_and_add("course_module", NULL);
	if (!kobj) {
		pr_err("Could not create kboj\n");
		return -ENOMEM;
	}

	ret = sysfs_create_group(kobj, &attr_group);
	if (ret)
		goto unregister;

	return misc_register(&course_misc);

unregister:
	misc_deregister(&course_misc);

	return ret;
}

static void __exit finish_driver(void)
{
	pr_info("Removing driver...\n");

	kobject_del(kobj);
	misc_deregister(&course_misc);
}

module_init(start_driver);
module_exit(finish_driver);
MODULE_AUTHOR("Marcos Paulo de Souza <marcos.souza.org@gmail.com>");
MODULE_LICENSE("GPL");
