#include <stdio.h>
#include "bootpack.h"

void HariMain(void){
	struct BOOTINFO *binfo=(struct BOOTINFO*)0x0ff0;
	
	char mcursor[256];
	int mx,my;
	mx=100;
	my=100;
	init_gdtidt();
	init_pic();


	init_palette();
	init_screen(binfo->vram,binfo->scrnx,binfo->scrny);
	io_sti();

	// putfonts8_asc(binfo->vram,binfo->scrnx,8,8,COL8_FFFFFF,"ABC_123");
	// putfonts8_asc(binfo->vram,binfo->scrnx,31,31,COL8_000000,"Radish OS.");
	// putfonts8_asc(binfo->vram,binfo->scrnx,30,30,COL8_FFFFFF,"Radish OS.");

	char* s;
	sprintf(s,"scrnx = %d",binfo->scrnx);
	putfonts8_asc(binfo->vram,binfo->scrnx,30,60,COL8_FFFFFF,s);

	init_mouse_cursor8(mcursor,13);
	putblock8_8(binfo->vram,binfo->scrnx,16,16,mx,my,mcursor,16);


	io_out8(PIC0_IMR, 0xf9); /* 开放PIC1和键盘中断(11111001) */
	io_out8(PIC1_IMR, 0xef); /* 开放鼠标中断(11101111) */
	
	while(1)io_hlt();
}
