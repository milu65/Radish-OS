#include <stdio.h>
#include "bootpack.h"

extern struct FIFO8 keybuf;
extern struct FIFO8 mousebuf;

#define PORT_KEYDAT		0x0060
#define PORT_KEYSTA		0x0064
#define PORT_KEYCMD		0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 	 0x60
#define KBC_MODE			 0x47

#define KEYCMD_SENDTO_MOUSE	0xd4
#define MOUSECMD_ENABLE		0xf4

void enable_mouse(){
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD,KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT,MOUSECMD_ENABLE);
}

void wait_KBC_sendready(){
	while(1){
		if((io_in8(PORT_KEYSTA)&KEYSTA_SEND_NOTREADY)==0){
			break;
		}
	}
}

void init_keyboard(){
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD,KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT,KBC_MODE);
}

void moveMouse(struct BOOTINFO *binfo,char* mcursor,int* pos,int x,int y);

void HariMain(){
	struct BOOTINFO *binfo=(struct BOOTINFO*)0x0ff0;
	char s[40], mcursor[256];
	unsigned char mouseData[256],keyboardData[256];
	int pos[2]={100,100};

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

	init_mouse_cursor8(mcursor,13);
	putblock8_8(binfo->vram,binfo->scrnx,16,16,pos[0],pos[1],mcursor,16);


	init_keyboard();
	enable_mouse();
	
	
	// while(1)io_hlt();

	fifo8_init(&keybuf,mouseData);
	fifo8_init(&mousebuf,keyboardData);
	int mouse_phase=0;
	int mouse_data[3]={0,0,0};
	int counter=0;
	int counter2=0;
	while(1){
		io_cli();
		if(fifo8_hasmore(&keybuf)+fifo8_hasmore(&mousebuf)==0){
			io_stihlt();
			continue;
		}


		int key=fifo8_get(&keybuf);
		int mouse=fifo8_get(&mousebuf);
		io_sti();

		if(key!=-1){
			sprintf(s,"%02X",key);
			boxfill8(binfo->vram,binfo->scrnx,COL8_008484,0,16,16,31);
			putfonts8_asc(binfo->vram,binfo->scrnx,0,16,COL8_FFFFFF,s);
		}
		if(mouse!=-1){
		
			counter2++;
			if(counter2>=256){
				counter++;
				counter2=0;
			}
				// sprintf(s,"%02X",mouse);
				sprintf(s,"%d",counter);
				boxfill8(binfo->vram,binfo->scrnx,COL8_008484,0,76,300,96);
				putfonts8_asc(binfo->vram,binfo->scrnx,0,76,COL8_FFFFFF,s);
			if(mouse_phase==0){
				if(mouse==0xfa){
					mouse_phase=1;
					continue;
				}
			}else if(mouse_phase==1){
				if((mouse&0xc8)==0x08){
					mouse_data[0]=mouse;
					mouse_phase++;
				}
			}else if(mouse_phase==2){
				mouse_data[1]=mouse;
				mouse_phase++;
			}else if(mouse_phase==3){
				mouse_data[2]=mouse;
				mouse_phase=1;
				char btn=mouse_data[0]&0x7;
				char mouse0=mouse_data[0];
				char x=mouse_data[1];
				char y=mouse_data[2];
				if((mouse0&0x10)!=0)x|=0xffffff00;
				if((mouse0&0x20)!=0)y|=0xffffff00;
				y=-y;
				moveMouse(binfo,mcursor,&pos,x,y);
				// sprintf(s,"%02X_%02X_%02X_%d_%d_%d",mouse_data[0],mouse_data[1],mouse_data[2],btn,x,y);
				// boxfill8(binfo->vram,binfo->scrnx,COL8_008484,0,46,300,61);
				// putfonts8_asc(binfo->vram,binfo->scrnx,0,46,COL8_FFFFFF,s);
			}

		}
		
	}
	
}

void moveMouse(struct BOOTINFO *binfo,char* mcursor,int* pos,int x,int y){
	boxfill8(binfo->vram,binfo->scrnx, COL8_008484,pos[0],pos[1],pos[0]+16,pos[1]+16);
	pos[0]+=x;
	pos[1]+=y;

	if(pos[0]<0)pos[0]=0;
	if(pos[1]<0)pos[1]=0;
	if(pos[0]>=binfo->scrnx)pos[0]=binfo->scrnx-1;
	if(pos[1]>=binfo->scrny)pos[1]=binfo->scrny-1;
	putblock8_8(binfo->vram,binfo->scrnx,16,16,pos[0],pos[1],mcursor,16);
}
