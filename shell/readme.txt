��������Լ����ļ�ϵͳ����

make OSDRV_CROSS=arm-hisiv300-linux CHIP=hi3520dv300 all

1. SDK�����osdrv/pub/Ŀ¼����rootfs_uclibc.tgz�ļ�
2. ��ѹrootfs_uclibc.tgz�󽫱��ļ����µ����ݸ��Ƶ���Ӧλ��
   ko�ļ���  		--> rootfs_uclibcĿ¼�ĸ�Ŀ¼
   shell�ű� 		--> rootfs_uclibc/var/shell/
   mdev.conf 		--> rootfs_uclibc/etc/
   rcS       		--> rootfs_uclibc/etc/init.d/
   mp4file��phidi 	--> rootfs_uclibc/var/bin/
3. ����rootfs_uclibc/mnt/usb1Ŀ¼
4. �����豸�ڵ�
   ��rootfs_uclibc/dev/Ŀ¼
	mknod console c 5 1
	mknod null c 1 3
	chmod 666 *
5. ʹ��mkfs.jffs2���ɾ���
   mkfs.jffs2 -d ./rootfs_uclibc -l -U -e 0x10000 -o rootfs_hi3520dv300_64k.jffs2