#include <stdio.h>
#include "bootpack.h"

void HariMain(void){
	struct BOOTINFO *binfo=(struct BOOTINFO*)0x0ff0;
	
	char mcursor[256];
	int mx,my;
	mx=100;
	my=100;

	init_palette();
	init_screen(binfo->vram,binfo->scrnx,binfo->scrny);

	putfonts8_asc(binfo->vram,binfo->scrnx,8,8,COL8_FFFFFF,"ABC_123");
	putfonts8_asc(binfo->vram,binfo->scrnx,31,31,COL8_000000,"Radish OS.");
	putfonts8_asc(binfo->vram,binfo->scrnx,30,30,COL8_FFFFFF,"Radish OS.");

	char* s;
	sprintf(s,"scrnx = %d",binfo->scrnx);
	putfonts8_asc(binfo->vram,binfo->scrnx,30,60,COL8_FFFFFF,s);

	init_mouse_cursor8(mcursor,13);
	putblock8_8(binfo->vram,binfo->scrnx,16,16,mx,my,mcursor,16);

	
	
	while(1)io_hlt();
}
