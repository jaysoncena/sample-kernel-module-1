#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#define MAJOR_NUMBER        61
#define ONEBYTE_DEVICENAME  "onebyte"
#define ONEBYTE_MAXSIZE     1

/* forward declaration */
int onebyte_open(struct inode *inode, struct file *filep);
int onebyte_release(struct inode *inode, struct file *filep);
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);
static void onebyte_exit(void);

/* definition of file_operation structure */
struct file_operations onebyte_fops = {
     read:      onebyte_read,
     write:     onebyte_write,
     open:      onebyte_open,
     release:   onebyte_release
};

char *onebyte_data = NULL;

int onebyte_open(struct inode *inode, struct file *filep)
{
    printk(KERN_ALERT "singlebyte: %s()\n", __FUNCTION__);
    return 0; // always successful
}

int onebyte_release(struct inode *inode, struct file *filep)
{
    printk(KERN_ALERT "singlebyte: %s()\n", __FUNCTION__);
    return 0; // always successful
}

ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_ALERT "singlebyte: %s()\n", __FUNCTION__);
    /*please complete the function on your own*/

    return 0;
}

ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
    size_t lennotcopied = 0;
    
    printk(KERN_ALERT "singlebyte: %s()\n", __FUNCTION__);

    lennotcopied = copy_from_user(onebyte_data, buf, ONEBYTE_MAXSIZE);
    printk(KERN_ALERT "singlebyte: %s(): Stored %zd bytes out of %zd bytes\n",
        __FUNCTION__, (count-lennotcopied), count);

    printk(KERN_ALERT "singlebyte: %s(): onebyte_data=%.*s\n",
        __FUNCTION__, (int)sizeof(onebyte_data), onebyte_data);

    if (count > ONEBYTE_MAXSIZE) return -ENOSPC;
    return count;
}

static int onebyte_init(void)
{
    int result;

    printk(KERN_ALERT "singlebyte: %s()\n", __FUNCTION__);

    // register the device
    result = register_chrdev(MAJOR_NUMBER, ONEBYTE_DEVICENAME, &onebyte_fops);

    if (result < 0) {
        return result;
    }

    // allocate one byte of memory for storage
    // kmalloc is just like malloc, the second parameter is
    // the type of memory to be allocated.
    // To release the memory allocated by kmalloc, use kfree.
    onebyte_data = kmalloc(sizeof(char), GFP_KERNEL);
    if (!onebyte_data) {
        printk(KERN_ALERT "singlebyte: %s(): kmalloc() failed\n", __FUNCTION__);
        onebyte_exit();
        // cannot allocate memory
        // return no memory error, negative signify a failure
        return -ENOMEM;
    }

    // initialize the value to be X
    *onebyte_data = 'X';
    printk(KERN_ALERT "This is a onebyte device module\n");
    return 0;
}

static void onebyte_exit(void)
{
    printk(KERN_ALERT "singlebyte: %s()\n", __FUNCTION__);

    // if the pointer is pointing to something
    if (onebyte_data) {
        // free the memory and assign the pointer to NULL
        kfree(onebyte_data);
        onebyte_data = NULL;
    }

    // unregister the device
    unregister_chrdev(MAJOR_NUMBER, ONEBYTE_DEVICENAME);
    printk(KERN_ALERT "Onebyte device module is unloaded\n");
}

MODULE_LICENSE("GPL");

module_init(onebyte_init);
module_exit(onebyte_exit);