#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h> 
#include <linux/device.h> 
#include <linux/cdev.h> 
/*no changes*/
#define MAJOR_NUMBER 300
#define ERROR_DIVISION_BY_ZERO -1
#define ERROR_INVALID_OPERATION -2

char module_name[5] = "calk";

static long first;
static long second;
static char operation;
static long result;
static int size;

static int calculate(void);
static int dev_open(struct inode* inode_, struct file* file_);
static ssize_t dev_read(struct file* file_, char* buffer, size_t length, loff_t* off);
static ssize_t dev_write(struct file* file_, const char* buffer, size_t length, loff_t* off);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .write = dev_write,
    .open = dev_open
 };

static int calculate(void) {
    switch(operation)     {
        case '+':
            result = first + second;
            break;
        case '-':
            result = first - second;
            break;		
        case '*':
            result = first * second;
            break;
        case '/':
            if (second == 0) { 
                return ERROR_DIVISION_BY_ZERO;
            }
            else {
                result = first / second;
            }
            break;
        default:
            return ERROR_INVALID_OPERATION;
	}
	return 0;
}

int init_module(void){
    int major_number;
    first = 0;
    second = 0;
    result = 0;
    operation = '+';

    major_number = register_chrdev(MAJOR_NUMBER, module_name, &fops);
    if (major_number < 0) {
        printk("Character device registration failed [error number = %d]\n", major_number);
        return major_number;
    }

    printk("Module \"%s\" loaded\n", module_name);
    printk("Create 'mknod /dev/calc_first c %d 0'\n", MAJOR_NUMBER); 
    printk("Create 'mknod /dev/calc_second c %d 1'\n", MAJOR_NUMBER);
    printk("Create 'mknod /dev/calc_operation c %d 2'\n", MAJOR_NUMBER); 
    printk("Create 'mknod /dev/calc_result c %d 3'\n", MAJOR_NUMBER); 
    return 0;
}

void cleanup_module(void){
    unregister_chrdev(MAJOR_NUMBER, module_name);
    printk("Module \"%s\" unloaded\n", module_name);
}

static int dev_open(struct inode* inode_, struct file* file_){
    return 0;
}


static ssize_t dev_read(struct file* file_, char* buffer, size_t length, loff_t* off){
    int error_code = 0;
    int major_num;
    int minor_num;
    char message[256];

    major_num = MAJOR(file_inode(file_)->i_rdev);
    minor_num = MINOR(file_inode(file_)->i_rdev);
    memset(message, 0, sizeof(message));

    switch(minor_num)    { 
        case 0: // first
            snprintf(message, sizeof(message), "first =  %ld\n", first);
            break;
        case 1: // second
            snprintf(message, sizeof(message), "second =  %ld\n", second);
            break;
        case 2: // operation
            snprintf(message, sizeof(message), "operation =  %c\n", operation);
            break;
        case 3: // result
            error_code = calculate();
            switch(error_code)            {
                case 0:
                    snprintf(message, sizeof(message), "result = %ld\n", result);
                    break;
                case ERROR_DIVISION_BY_ZERO:
                    snprintf(message, sizeof(message), "Error. Division by zero\n");
                    break;
                case ERROR_INVALID_OPERATION:
                    snprintf(message, sizeof(message), "Error. Invalid operation\n");
                    break;
                default:
                    break;
            }
           break;        
        default:
            break;
    }

    size = strlen(message);

    if (message[*off] == '\0') {
        return 0;
    }
    /* Besides copying the string to the user provided buffer,
       this function also checks that the user has permission to
       write to the buffer, that it is mapped */
    copy_to_user(buffer, &message[*off], 1);
    *off += 1;
    return 1;
}

static ssize_t dev_write(struct file* file_, const char* buffer, size_t length, loff_t* off){
    int major_num;
    int minor_num;
    char message[256];

    major_num = MAJOR(file_inode(file_)->i_rdev);
    minor_num = MINOR(file_inode(file_)->i_rdev);
    memset(message, 0, sizeof(message));

    copy_from_user(message, buffer, length);

    switch(minor_num)    { 
        case 0: // first
            kstrtol(message, 10, &first);
            printk("first = %ld\n", first);
            break;
        case 1: // second
            kstrtol(message, 10, &second);
            printk("second = %ld\n", second);
            break;
        case 2: // operation
            operation = message[0];
            printk("operation = %c\n", operation);
            break;
        default:
            break;
    }
    size = strlen(message);
    return size;
}


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("calculator");
