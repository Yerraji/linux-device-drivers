#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "Yerraji_char_device"
#define Buffer_size 1024

static int major_number;
static char kernel_buffer[Buffer_size];

static int device_open(struct inode *inode, struct file *file){
    printk(KERN_INFO "Yerraji's char driver: Device opened \n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file){

    printk(KERN_INFO "yerraji's_char_device: device closed\n");
    return 0;
}
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t len, loff_t *offset){
    int bytes_read=0;
    bytes_read= simple_read_from_buffer(user_buffer,len,offset,kernel_buffer, Buffer_size);
    printk(KERN_INFO "yerraji's char device: read%d bytes\n", bytes_read);
    return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t len, loff_t *offset){
    int bytes_write=0;
    bytes_write= simple_write_to_buffer(kernel_buffer, Buffer_size, offset, user_buffer, len);
    printk(KERN_INFO "yerraji char device: write %d bytes\n", bytes_write);
    return bytes_write;
}

static struct file_operations fops = 
{
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write
};

static int __init char_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if(major_number <0)
    {
        printk(KERN_ALERT "my_char_device : failed to register device\n");
        return major_number;
    }
    printk(KERN_INFO "my_char_device: registered with major number%d\n", major_number);
    return 0;
}
static void __exit char_exit(void){

    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "my_char_device: unregistered device \n");
}

MODULE_LICENSE("gpl");
MODULE_AUTHOR("Yerraji");
MODULE_DESCRIPTION("Basic Character Device Driver");


module_init (char_init);
module_exit (char_exit);