#include <stdio.h>

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
void io_out8(int port, int data);
void io_out16(int port, int data);
void io_out32(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void set_palette(int start,int end,unsigned char*rgb);
void init_palette(void);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(char *vram,short scrnx,short scrny);
void putblock8_8(char *vram,int vxsize,int pxsize,int pysize,int px0,int py0,char*buf,int bxsize);
void init_mouse_cursor8(char*mouse, char bc);
void putfonts8_asc(char *vram, int xsize,int x,int y,char c,unsigned char*s);
void putfont8(char *vram,int xsize,int x,int y,char c,char*font);


struct BOOTINFO{
	char cyls,leds,vmode,reserve;
	short scrnx,scrny;
	char *vram;
};

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

	init_mouse_cursor8(mcursor,COL8_008484);
	putblock8_8(binfo->vram,binfo->scrnx,16,16,mx,my,mcursor,16);

	
	
	while(1)io_hlt();
}

void init_mouse_cursor8(char*mouse, char bc){
	static char cursor[16][16]={
		"*...............",
		"**..............",
		"*o*.............",
		"*oo*............",
		"*ooo*...........",
		"*oooo*..........",
		"*ooooo*.........",
		"*oooooo*........",
		"*ooooooo*.......",
		"*oooooooo*......",
		"*ooo*******.....",
		"*oo*............",
		"*o*.............",
		"**..............",
		"*...............",
		"................"
	};
	int i;
	for(i=0;i<16;i++){
		int j;
		for(j=0;j<16;j++){
			int index=i*16+j;
			if(cursor[i][j]=='*'){
				mouse[index]=COL8_FFFFFF;
			}else if(cursor[i][j]=='o'){
				mouse[index]=COL8_000000;
			}else mouse[index]=bc;
		}
	}
}

void putblock8_8(char *vram,int vxsize,int pxsize,int pysize,int px0,int py0,char*buf,int bxsize){
	int x,y;
	for(y=0;y<pysize;y++){
		for(x=0;x<pxsize;x++){
			vram[(py0+y)*vxsize+(px0+x)]=buf[y*bxsize+x];
		}
	}
}

void putfonts8_asc(char *vram, int xsize,int x,int y,char c,unsigned char*s){
	extern char hankaku[4096];
	while((*s)!=0){
		putfont8(vram,xsize,x,y,c,hankaku+(*s++)*16);
		x+=8;
	}
	return;
}

void putfont8(char *vram,int xsize,int x,int y,char c,char*font){
	int i;
	char *p, d;
	for(i=0;i<16;i++){
		p=vram+(y+i)*xsize+x;
		d=font[i];
		int j;
		for(j=8;j>=0;j--){
			if((d&(1<<j)))p[8-j]=c;
		}
	}
	return;
}

void init_screen(char *vram,short xsize,short ysize){
	boxfill8(vram, xsize, COL8_008484,  0,         0,          xsize -  1, ysize - 29);
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 28, xsize -  1, ysize - 28);
	boxfill8(vram, xsize, COL8_FFFFFF,  0,         ysize - 27, xsize -  1, ysize - 27);
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 26, xsize -  1, ysize -  1);

	boxfill8(vram, xsize, COL8_FFFFFF,  3,         ysize - 24, 59,         ysize - 24);
	boxfill8(vram, xsize, COL8_FFFFFF,  2,         ysize - 24,  2,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484,  3,         ysize -  4, 59,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484, 59,         ysize - 23, 59,         ysize -  5);
	boxfill8(vram, xsize, COL8_000000,  2,         ysize -  3, 59,         ysize -  3);
	boxfill8(vram, xsize, COL8_000000, 60,         ysize - 24, 60,         ysize -  3);

	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize -  3, ysize - 24, xsize -  3, ysize -  3);
}


void set_palette(int start,int end,unsigned char*rgb){
	int i;
	int flag=io_load_eflags();//记录中断许可的标识的值
	io_cli();	//将中断许可标识设置为0,禁止中断
	io_out8(0x03c8,start);
	for(i=start;i<=end;i++){
		io_out8(0x03c9,rgb[0]/4);
		io_out8(0x03c9,rgb[1]/4);
		io_out8(0x03c9,rgb[2]/4);
		rgb += 3;
	}
	io_store_eflags(flag);
	return;
}

void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:黑 */
		0xff, 0x00, 0x00,	/*  1:亮红 */
		0x00, 0xff, 0x00,	/*  2:亮绿 */
		0xff, 0xff, 0x00,	/*  3:亮黄 */
		0x00, 0x00, 0xff,	/*  4:亮蓝 */
		0xff, 0x00, 0xff,	/*  5:亮紫 */
		0x00, 0xff, 0xff,	/*  6:浅亮蓝 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xc6, 0xc6, 0xc6,	/*  8:亮灰 */
		0x84, 0x00, 0x00,	/*  9:暗红 */
		0x00, 0x84, 0x00,	/* 10:暗绿 */
		0x84, 0x84, 0x00,	/* 11:暗黄 */
		0x00, 0x00, 0x84,	/* 12:暗青 */
		0x84, 0x00, 0x84,	/* 13:暗紫 */
		0x00, 0x84, 0x84,	/* 14:浅暗蓝 */
		0x84, 0x84, 0x84	/* 15:暗灰 */
	};
	set_palette(0, 15, table_rgb);
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

