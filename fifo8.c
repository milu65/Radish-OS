#include "bootpack.h"

void fifo8_init(struct FIFO8 *fifo,unsigned char* data){
    fifo->data=data;
    fifo->RT=0;
	fifo->past=0;
}

void fifo8_put(struct FIFO8 *fifo,unsigned char data){
    fifo->RT=fifo->RT%KEYBUF_SIZE;
    fifo->data[fifo->RT]=data;
    fifo->RT++;
}

int fifo8_get(struct FIFO8* fifo){
    fifo->RT=fifo->RT%KEYBUF_SIZE;
	fifo->past=fifo->past%KEYBUF_SIZE;
	if(fifo->RT==fifo->past){
            return -1;
	}
	int i=fifo->data[fifo->past];
	fifo->past++;
    return i;
}

int fifo8_hasmore(struct FIFO8* fifo){
    if(fifo->RT==fifo->past)return 0;
    return 1;
}