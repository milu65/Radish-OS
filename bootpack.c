
void io_hlt(void);
void write_mem8(int addr,int data);

void HariMain(void){
	int i;
	char*p;
	for(i=0xa0000;i<0xaffff;i++){
		// p=(char *)i;
		// *p=i&15;
		*((char *)i)=(i&15);
		//write_mem8(i,i^15);
	}
		
	
	while(1){io_hlt();}
}
