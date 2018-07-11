///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <Main.c>
//   @author Max.Kao@ite.com.tw
//   @date   2014/06/26
//   @fileversion: ITE_MHLRX_SAMPLE_V1.10
//******************************************/
//#include "config.h"
//#include "IO.h"
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

int it6801_open(struct inode * inode, struct file * file)
{
    return 0;
}

int it6801_close(struct inode * inode, struct file * file)
{
    return 0;
}

int it6801_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    void __user *argp = (void __user *)arg;
		unsigned char mDataIn;
		
	if(cmd == 0x10)
	{
		//i2c_read_byte(0x90, 0x9A, 1, &mDataIn, 0);
		//printk("PCLK0x9A 0x%x\n",mDataIn); 
		//i2c_read_byte(0x90, 0x9D, 1, &mDataIn, 0);
		//printk("0x9D 0x%x\n",mDataIn); 
		i2c_read_byte(0x90, 0x99, 1, &mDataIn, 0);
		//printk("mDataIn 0x%x\n",mDataIn);  
		#define INTERLACED_MODE (1)
		#define PROGRESSIVE_MODE (0)
		bool is_interlaced_mode = mDataIn & 0x02;
		if(is_interlaced_mode) {
			return INTERLACED_MODE;
		} else {
			return PROGRESSIVE_MODE;
		}
	}
	
	if(cmd == 0x12)
	{
		i2c_read_byte(0x90, 0x9F, 1, &mDataIn, 0);  //9F= 7(1920),5(1280),2(720)
		printk("9F 0x%x\n",mDataIn); 
#if 1
		switch(mDataIn)
		{
			case 7://1920
				return 1;
				break;
			case 5://1280
				return 2;
			case 2://720
				i2c_read_byte(0x90, 0xA5, 1, &mDataIn, 0);
				//printk("0xA5 0x%x\n",mDataIn); 
				if(40 == mDataIn) {
					return 3;
				} else {
					return 4;	
				}
				break;
			default:
				printk("Vin: unknown resolution");
				return -1;
				break;
		}
#else				  
		if(5==mDataIn) { //1280x720
					return 2;	
		}	
		if(2==mDataIn) {	//720x
			i2c_read_byte(0x90, 0xA5, 1, &mDataIn, 0);
			//printk("0xA5 0x%x\n",mDataIn); 
			if(40==mDataIn) {
					return 3;
			} else {
					return 4;	
			}
		}	
		//if(7==mDataIn)	
			return 1;
#endif
	}
	
	if(cmd == 0x15)
	{
		if(askVmode())
		{
					//printk("Input Video mode change interrupt\n");
					return 1;
		}				
		
	}
	
	if(cmd == 0x18)
	{
		i2c_read_byte(0x90, 0x0A, 1, &mDataIn, 0);
		//printk("reg 0x0A: 0x%x\n",mDataIn); 
		if((mDataIn&0x1))  //5V detect
			return 1;
	}
	
	if(cmd == 0x20)
	{
		//i2c_read_byte(0x90, 0xAE, 1, &mDataIn, 0);
		//printk("reg 0xAE: 0x%x\n",mDataIn); 
		//i2c_read_byte(0x90, 0xAF, 1, &mDataIn, 0);
		//printk("reg 0xAF: 0x%x\n",mDataIn); 
		i2c_read_byte(0x90, 0x0A, 1, &mDataIn, 0);
		//printk("reg 0x0A: 0x%x\n",mDataIn); 
		if((mDataIn&0x80))  //Video Stable
			return 1;
	}
	
	if(cmd == 0x25)
	{
		//i2c_read_byte(0x90, 0xAE, 1, &mDataIn, 0);
		//printk("reg 0xAE: 0x%x\n",mDataIn); 
		//i2c_read_byte(0x90, 0xAF, 1, &mDataIn, 0);
		//printk("reg 0xAF: 0x%x\n",mDataIn); 
		i2c_read_byte(0x90, 0xAA, 1, &mDataIn, 0);
		//printk("reg 0x0A: 0x%x\n",mDataIn); 
		if((mDataIn!=0))  //aInput Stable
			return 1;
	}
	
	if(cmd == 0x30)
	{
		//i2c_read_byte(0x90, 0x9A, 1, &mDataIn, 0);
		//printk("PCLK0x9A 0x%x\n",mDataIn); 
		//i2c_read_byte(0x90, 0x2D, 1, &mDataIn, 0);
		//printk("reg 0x2D: 0x%x\n",mDataIn); 
		i2c_read_byte(0x90, 0x99, 1, &mDataIn, 0);
		//printk("0x99 0x%x\n",mDataIn); 
		
		//i2c_read_byte(0x90, 0x0C, 1, &mDataIn, 0);
		//printk("reg 0x0C: 0x%x\n",mDataIn); 
		
		
		if(mDataIn&0x08) //0x99 Indicate if video signal is stable .
		//if(mDataIn&0x40)	//0xC P0_RX_CLK stable
			return 1;
	}

    return 0;
}

static struct file_operations it6801_fops = 
{
    .owner      = THIS_MODULE,
    .open       = it6801_open,
    .release    = it6801_close,
    .unlocked_ioctl      = it6801_ioctl,
};

static struct miscdevice it6801_dev = 
{   
    .minor		= MISC_DYNAMIC_MINOR,
    .name		= "it6801",
    .fops  		= &it6801_fops,
};



static int __init it6801_init(void)
{
	//unsigned char ret;

    if (misc_register(&it6801_dev))
    {
        printk("ERROR: could not register it6801 devices\n");
		return -1;
    }
    
    hi_dev_init();
    it6802HPDCtrl(1,0);	// HDMI port , set HPD = 0

		delay1ms(1000);	//for power sequence
		
		//printk("IT6802_fsm_init\n");
		IT6802_fsm_init();


		init_timer(&checker);
		checker.function = (void *)checker_handler;

		checker.expires =  200 + jiffies;
		add_timer(&checker);

    return 0;
}

static void __exit it6801_exit(void)
{
	
    misc_deregister(&it6801_dev);
}

module_init(it6801_init);
module_exit(it6801_exit);

MODULE_LICENSE("GPL");

