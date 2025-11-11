#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "Redji_char_dev"
#define CLASS_NAME "Redji_class"
#define BUFFER_SIZE 1024

static int major_num;
static char kernel_buffer[BUFFER_SIZE];
static struct class* my_class=NULL;
static struct device* my_device =NULL;

static int device_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO"%s: Device opened\n", DEVICE_NAME);
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "%s: DEvice is closed\n", DEVICE_NAME);
    return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer, size_t len, loff_t *offset)
{
    int bytes_read = simple_read_from_buffer(user_buffer, len, offset, kernel_buffer, BUFFER_SIZE);
    printk(KERN_INFO "%s: Read %d bytes\n", DEVICE_NAME, bytes_read);
    return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t len, loff_t *offset)
{
    int bytes_written = simple_write_to_buffer(kernel_buffer, BUFFER_SIZE, offset, user_buffer, len);
    printk(KERN_INFO "%s: Wrote %d bytes\n", DEVICE_NAME, bytes_written);
    return bytes_written;
}

static struct file_operations fops ={
    .owner =THIS_MODULE,
    .open = device_open,
    .read = device_read,
    .write = device_write,
    .release = device_release
};  

static int __init char_device_init(void){
    major_num = register_chrdev(0,DEVICE_NAME,&fops);
    if(major_num<0){
        printk(KERN_INFO"The device:%s is unable to register",DEVICE_NAME);
        return major_num;
    }
    printk(KERN_INFO"The device:%s is registerd and major number is: %d",DEVICE_NAME, major_num);
    
    my_class= class_create(CLASS_NAME);
    if(IS_ERR(my_class))
    {
        unregister_chrdev(major_num,DEVICE_NAME);
        printk(KERN_ALERT "%s:Failed to register class\n", DEVICE_NAME);
        return PTR_ERR(my_class);
    }
    my_device=device_create(my_class,NULL,MKDEV(major_num,0),NULL, DEVICE_NAME);
    if(IS_ERR(my_device)){
        class_destroy(my_class);
        unregister_chrdev(major_num,DEVICE_NAME);
        printk(KERN_ALERT"%s: failed to create device\n",DEVICE_NAME);
        return PTR_ERR(my_device);
    }
    printk(KERN_ALERT"Successfuly created Device name:%s and major_num: %d\n",DEVICE_NAME, major_num);
    return 0;
}

static void __exit char_device_exit(void)
{
    device_destroy(my_class,MKDEV(major_num, 0));
    class_unregister(my_class);
    class_destroy(my_class);
    unregister_chrdev(major_num,DEVICE_NAME);
    printk(KERN_INFO"%s: Unregister the device successfully\n",DEVICE_NAME);

}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YerraJi");
MODULE_DESCRIPTION("Character Device Driver with Auto /dev Node Creation");

module_init(char_device_init);
module_exit(char_device_exit);
