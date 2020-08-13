; naskfunc

[FORMAT "WCOFF"]	;制作目标文件的模式
[INSTRSET "i486p"]				;使用到486为止的指令
[BITS 32]	;制作32位模式用的机械语言

;制作目标文件的信息

[FILE "naskfunc.nas"]	;源文件名信息

		GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt
		GLOBAL	_io_in8,  _io_in16,  _io_in32
		GLOBAL	_io_out8, _io_out16, _io_out32
		GLOBAL	_io_load_eflags, _io_store_eflags

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
	
_io_cli: ;void io_cli(void);
	CLI
	RET
	
_io_sti: ;void io_sti(void);
	STI
	RET
	
_io_stihlt: ;void io_stihlt(void);
	STI
	CLI
	RET

_io_in8:	; int io_in8(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AL,DX
		RET

_io_in16:	; int io_in16(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AX,DX
		RET

_io_in32:	; int io_in32(int port);
		MOV		EDX,[ESP+4]		; port
		IN		EAX,DX
		RET

_io_out8:	; void io_out8(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX,AL
		RET

_io_out16:	; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,AX
		RET

_io_out32:	; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,EAX
		RET

_io_load_eflags:	; int io_load_eflags(void);
		PUSHFD		; PUSH EFLAGS
		POP		EAX
		RET

_io_store_eflags:	; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; POP EFLAGS
		RET

_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET