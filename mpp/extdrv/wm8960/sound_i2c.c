///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IO.c>
//   @author Max.Kao@ite.com.tw
//   @date   2014/06/26
//   @fileversion: ITE_MHLRX_SAMPLE_V1.10
//******************************************/
// #include "config.h"
#include "sound_i2c.h"


#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/i2c.h>
//#include "nop.h"
//#include <stdio.h>

#define SOUND_DEV  0

static struct i2c_client* sound_client;


////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////

void delay1ms(USHORT ms)
{
	mdelay(ms);
}

void sound_read(BYTE offset,BYTE byteno,BYTE *p_data)
{
	BYTE i;
 	struct i2c_client* client = sound_client;
  unsigned char buf[2];
  int ret;
  
  //printk("i2c read\n");
 
	for(i = 0; i < byteno; ++i)
	{
		
    buf[0] = offset;
    ret = i2c_master_recv(client, buf, 1);
    if (ret >= 0)
    {
        p_data[i] = buf[0];
    }
		++offset;

	}

}

void sound_write(BYTE offset,BYTE byteno,BYTE *p_data)
{
	BYTE i;
 	struct i2c_client* client = sound_client;
  unsigned char buf[2];
  int ret=0;
  
  //printk("i2c write \n");
 
	
	for(i = 0; i < byteno; ++i)
	{
		buf[0] = offset + i;
    buf[1] = p_data[i];

    ret = i2c_master_send(client, buf, 2);
    if(ret !=2)
    {	
    	printk("i2c write failed\n");
    } 

	}
}

SYS_STATUS i2c_write_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device )
{
	sound_write(offset,byteno,p_data);

	return ER_SUCCESS;
}

SYS_STATUS i2c_read_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device )
{
	sound_read(offset,byteno,p_data);

  return ER_SUCCESS;
}

static struct i2c_board_info wm8960_info =
{
    I2C_BOARD_INFO("wm8960_i2c", 0x34), //7bit
};

void sound_dev_init(void)
{
    struct i2c_adapter* i2c_adap_0;

    // use i2c0
    i2c_adap_0 = i2c_get_adapter(0);
    
    sound_client = i2c_new_device(i2c_adap_0, &wm8960_info);
}

void sound_dev_exit(void)
{
    //i2c_unregister_device(sound_client);
}
