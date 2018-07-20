ifeq ($(PARAM_FILE), )
        PARAM_FILE:=../../Makefile.param
        include $(PARAM_FILE)
endif

obj-m := wm8960drv.o
wm8960drv-objs := wm8960.o sound_i2c.o

default:
	@make -C $(LINUX_ROOT) M=$(PWD) modules

clean: 
	@make -C $(LINUX_ROOT) M=$(PWD) clean


