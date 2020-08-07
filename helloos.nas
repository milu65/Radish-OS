
	ORG 0x7c00
	
	JMP entry
	DB 0x90
	DB "HELLOIPL"	;启动扇区名称（8字节）
	DW 512			;每个扇区大小512字节，必须
	DB 1			;cluster大小（必须为一个扇区）
	DW 1			;FAT起始位置（一般为第一个扇区）
	DB 2			;FAT个数（必须为2）
	DW 224			;根目录大小（一般为224项）
	DW 2880			;该磁盘大小（必须为2880扇区1440*1024/512）
	DB 0xf0			;磁盘类型（必须为0xf0）
	DW 9			;FAT长度（必须为9扇区）
	DW 18			;一个磁道（track）有几个扇区（必须为18）
	DW 2			;磁头数量
	DD 0			;不适用分区，必须为0
	DD 2880			;重写一次磁盘大小
	DB 0,0,0x29		;意义不明（固定）
	DD 0xffffffff	;(可能是)卷标号码
	DB "HELLO-SO   ";磁盘名称（必须为11字节，不足填充其它字符）
	DB "FAT12   "	;磁盘格式名称（8字节）
	RESB 18			;空出18字节
	


entry:				
	MOV AX,0
	MOV SS,AX
	MOV SP,0x7c00
	MOV DS,AX
	
	MOV AX,0x0820
	MOV ES,AX
	
	MOV SI,0		;错误次数重置
	MOV AH,0x02		;读取磁盘
	MOV AL,1		;处理对象扇区数量
	MOV CH,0		;柱面号
	MOV CL,2		;扇区号
	MOV DH,0		;磁头号
	MOV DL,0x00		;驱动器号
	JMP redoBack
	
redo:
	MOV AX,ES
	ADD AX,0x20
	MOV ES,AX		;ES += 0x20
	MOV AH,0x02		;set as read mode
	MOV AL,1
	ADD CL,1		;add sector number
redoBack:		;check sector, cylinder, head number
	CMP CL,19
	JE clUP
	CMP DH,2
	JE dhUP
	CMP CH,10
	JE chUP
	INT 0x13	;read
	JC error
	JMP redo
	
error:
	ADD SI,1
	CMP SI,5
	JE errorRun
	JMP redoBack


errorRun:		;set error msg
	MOV SI,errorMsg
outputLoop:		;output String, need set SI as address of String
	MOV AL,[SI]
	ADD SI,1
	CMP AL,0
	JE fin
	MOV AH,0x0e
	MOV BX,15
	INT 0x10
	JMP outputLoop
	
fin:		;stop work
	HLT
	JMP fin

clUP:		;sector max exceed
	ADD DH,1
	MOV CL,0
	JMP redoBack
	
dhUP:		;head max exceed
	ADD CH,1
	MOV DH,0
	JMP redoBack

chUP:		;cylinder num exceed; finished load
	;MOV		[0x0ff0],CH		; 载入完成，跳转到对应地址
	JMP		0xc200
	;MOV SI,successMsg
	;JMP outputLoop
		
errorMsg:
	DB 0x0a,0x0a
	DB "load error"
	DB 0x0a
	DB 0

successMsg:
	DB 0x0a
	DB "load finished"
	DB 0x0a
	DB 0

	
	RESB	0x7dfe-$		; fill 0x00 from $ to 0x001fe
	DB		0x55, 0xaa