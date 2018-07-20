///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <wm8960.c>
//   @author Max.Kao@ite.com.tw
//   @date   2014/06/26
//   @fileversion: ITE_MHLRX_SAMPLE_V1.10
//******************************************/
//#include "config.h"
//#include "IO.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include "sound_i2c.h"

#define DEVICE_NAME "wm8960_codec"
#if 0
#include "Mhlrx.h"



static struct timer_list checker;

static int checker_handler(unsigned long data)
{
  //unsigned char mDataIn;
  //printk("IT6802_fsm\n");  
	IT6802_fsm();
	//i2c_read_byte(0x90, 0x75, 1, &mDataIn, 0);
	//printk("0x75 0x%x\n",mDataIn); 

	
	checker.expires = HZ / 50 + jiffies;
	add_timer(&checker);

	return 0;
}
#endif

int wm8960_open(struct inode * inode, struct file * file)
{
	printk(KERN_INFO DEVICE_NAME "opened!\n");
    return 0;
}

int wm8960_close(struct inode * inode, struct file * file)
{
	printk(KERN_INFO DEVICE_NAME "closed!\n");
    return 0;
}

int wm8960_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk(KERN_INFO DEVICE_NAME "ioctl method: cmd[0x%x] arg[0x%x]!\n",cmd,arg);

    // void __user *argp = (void __user *)arg;
	
	if(cmd == 0x18)
	{
		unsigned char mDataIn;
		i2c_read_byte(0x34, 0x0A, 1, &mDataIn, 0);
		printk("reg 0x0A: 0x%x\n",mDataIn); 

		return 1;
	} else {
		return 0;
	}
	
    return 0;
}

static struct file_operations wm8960_fops = 
{
    .owner               = THIS_MODULE,
    .open                = wm8960_open,
    .release             = wm8960_close,
    .unlocked_ioctl      = wm8960_ioctl,
};

static struct miscdevice wm8960_dev = 
{   
    .minor		= MISC_DYNAMIC_MINOR,
    .name		= DEVICE_NAME,
    .fops  		= &wm8960_fops,
};


static int __init wm8960_init(void)
{
	//unsigned char ret;
	printk("hello,I'm wm8960\n");

    if (misc_register(&wm8960_dev))
    {
        printk("ERROR: could not register wm8960 devices\n");
		return -1;
    }
    
	sound_dev_init();
    // it6802HPDCtrl(1,0);	// HDMI port , set HPD = 0
	delay1ms(1000);	//for power sequence

	unsigned char mDataIn;
	i2c_read_byte(0x34, 0x00, 1, &mDataIn, 0);
	printk("reg 0x0A: 0x%x\n",mDataIn); 
	i2c_write_byte(0x34,0x00,1,&mDataIn, 0);
		
	#if 0
 		//printk("IT6802_fsm_init\n");
		IT6802_fsm_init();


		init_timer(&checker);
		checker.function = (void *)checker_handler;

		checker.expires =  200 + jiffies;
		add_timer(&checker);
	#endif

    return 0;
}

static void __exit wm8960_exit(void)
{
	printk("goodbye,I'm wm8960\n");
    misc_deregister(&wm8960_dev);
}

module_init(wm8960_init);
module_exit(wm8960_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("young <younglhome@hotmail.com>");
MODULE_DESCRIPTION("wm8960 codec Driver");
MODULE_VERSION("V0.01");