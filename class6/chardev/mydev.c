#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>        // Required for the copy to user function
#define  DEVICE_NAME "mydev"      ///< The device will appear at /dev/mydev using this value
#define  CLASS_NAME  "char_dev"        ///< The device class -- this is a character device driver

MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("David Lin");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("Linux char device example");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users

static int    majorNumber;                  ///< Stores the device number -- determined automatically
static char   message[256] = {0};           ///< Memory for the string that is passed from userspace
static short  size_of_message;              ///< Used to remember the size of the string stored
static int    numberOpens = 0;              ///< Counts the number of times the device is opened
static struct class*  char_dev_class  = NULL; ///< The device-driver class struct pointer
static struct device* my_char_dev = NULL; ///< The device-driver device struct pointer

// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int __init mydev_init(void)
{
   printk(KERN_INFO "char_device: Initializing the char_device LKM\n");
   // Try to dynamically allocate a majonumberr number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);  //dynamically allocate a major number  cat /proc/devices
   if (majorNumber<0)
   {
      printk(KERN_ALERT "char_device failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "char_device: registered correctly with major number %d\n", majorNumber);

   // Register the device class
   char_dev_class = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(char_dev_class))
   {                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(char_dev_class);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "char_device: device class registered correctly\n");

   // Register the device driver
   my_char_dev = device_create(char_dev_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(my_char_dev)){               // Clean up if there is an error
      class_destroy(char_dev_class);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(my_char_dev);
   }
   printk(KERN_INFO "char_device: device class created correctly\n"); // Made it! device was initialized
   return 0;
}

static void __exit mydev_exit(void)
{
   device_destroy(char_dev_class, MKDEV(majorNumber, 0));     // remove the device
   class_unregister(char_dev_class);                          // unregister the device class
   class_destroy(char_dev_class);                             // remove the device class
   unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
   printk(KERN_INFO "char_device: Goodbye from the LKM!\n");
}

static int Device_Open=0;  //being exclusive

static int dev_open(struct inode *inodep, struct file *filep)
{
	if(Device_Open)
		return -EBUSY;
	Device_Open=1;
	numberOpens++;
	try_module_get(THIS_MODULE);
	printk(KERN_INFO "char_device: Device has been opened %d time(s)\n", numberOpens);
	return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int error_count = 0;
	if(access_ok(buffer,size_of_message)==0)  // check whether user space pointer is valid, return non-0 if valid
	{
		printk(KERN_ALERT "writting to user space buffer fail, bad pointer\n");
		return -EFAULT;  //bad address error
	}


   // copy_to_user has the format ( * to, *from, size) and returns 0 on success
   error_count = copy_to_user(buffer, message, size_of_message);

   if (error_count==0)
   {            // if true then have success
      printk(KERN_INFO "char_device: Sent %d characters to the user\n", size_of_message);
      return (size_of_message=0);  // clear the position to the start and return 0
   }
   else
   {
      printk(KERN_INFO "char_device: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   }
   return error_count;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	if(access_ok(buffer,len)==0)
	{
		printk(KERN_ALERT "reading from user space buffer fail, bad user pointer\n");
		return -EFAULT;
	}

	copy_from_user(message,buffer,len);

   // sprintf(message, "%s(%zu letters)", buffer, len);   // appending received string with its length
   size_of_message = strlen(message);                 // store the length of the stored message
   printk(KERN_INFO "char_device: Received %zu characters from the user\n", len);
   return len;
}


static int dev_release(struct inode *inodep, struct file *filep)
{
	Device_Open=0;
	module_put(THIS_MODULE);
	printk(KERN_INFO "char_device: Device successfully closed\n");
	return 0;
}

module_init(mydev_init);
module_exit(mydev_exit);
