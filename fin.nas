	;ORG 0xc200
	
	;MOV AL,0x13
	;MOV AH,0x00
	;INT 0x10
	
	MOV SI,msg
out:		;output String, need set SI as address of String
	MOV AL,[SI]
	ADD SI,1
	CMP AL,0
	JE f
	MOV AH,0x0e
	MOV BX,15
	INT 0x10
	JMP out
msg:
	DB 0x0a,0x0a
	DB "done"
	DB 0
f:
	HLT
	JMP f