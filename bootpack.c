#include <stdio.h>
#include "bootpack.h"


extern struct KEYBUF keybuf;

void HariMain(void){
	struct BOOTINFO *binfo=(struct BOOTINFO*)0x0ff0;
	char s[40], mcursor[256];
	int mx,my;

	init_gdtidt();
	init_pic();
	io_sti();

	// putfonts8_asc(binfo->vram,binfo->scrnx,8,8,COL8_FFFFFF,"ABC_123");
	// putfonts8_asc(binfo->vram,binfo->scrnx,31,31,COL8_000000,"Radish OS.");
	// putfonts8_asc(binfo->vram,binfo->scrnx,30,30,COL8_FFFFFF,"Radish OS.");

	io_out8(PIC0_IMR, 0xf9); /* 开放PIC1和键盘中断(11111001) */
	io_out8(PIC1_IMR, 0xef); /* 开放鼠标中断(11101111) */
	
	init_palette();
	init_screen(binfo->vram,binfo->scrnx,binfo->scrny);
	mx=100;
	my=100;
	init_mouse_cursor8(mcursor,13);
	putblock8_8(binfo->vram,binfo->scrnx,16,16,mx,my,mcursor,16);


	
	// while(1)io_hlt();

	while(1){
		io_cli();
		if(keybuf.flag==0){
			io_stihlt();
		}else{
			int i=keybuf.data;
			keybuf.flag=0;
			io_sti();
			sprintf(s,"%02X",i);
			boxfill8(binfo->vram,binfo->scrnx,COL8_008484,0,16,16,31);
			putfonts8_asc(binfo->vram,binfo->scrnx,0,16,COL8_FFFFFF,s);
		}
	}
	
}
