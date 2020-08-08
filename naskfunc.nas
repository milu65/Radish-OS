; naskfunc

[FORMAT "WCOFF"]	;制作目标文件的模式
[INSTRSET "i486p"]				;使用到486为止的指令
[BITS 32]	;制作32位模式用的机械语言

;制作目标文件的信息

[FILE "naskfunc.nas"]	;源文件名信息
	GLOBAL _io_hlt,_write_mem8

;以下是实际函数

[SECTION .text]	;目标文件中写了这些之后再写程序
_io_hlt:		;void io_hlt(void);
	HLT
	RET
	
_write_mem8:	;void write_mem8(int addr,int data);
	MOV ECX,[ESP+4]	;对应上面的地址(+4)参数和数据(+8)参数
	MOV AL,[ESP+8]
	MOV [ECX],AL
	RET
	
	