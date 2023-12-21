#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "lab5_device"
#define BUF_LEN 80

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Volodymyr Poltavskiy");
MODULE_DESCRIPTION("My Lab 5 variant 4");

static int major;
static char msg[BUF_LEN];
static char *msg_Ptr;

static ssize_t device_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset) {
    if (length > BUF_LEN - 1)
    return -EINVAL;

    if (copy_from_user(msg, buffer, length) != 0)
    return -EFAULT;

    msg[length] = '\0';
    msg_Ptr = msg;

    printk(KERN_INFO "Message length: %d\n", length);
    printk(KERN_INFO "Message: %s\n", msg);

    return length;
}

static struct file_operations fops = {
        .write = device_write,
};

static int __init mymodule_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Device registration error");
        return major;
    }

    printk(KERN_INFO "Execute command: 'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);

    return 0;
}

static void __exit mymodule_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Kernel end\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);
