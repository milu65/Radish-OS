TOOLSPATH=./z_tools
INCPATH  = ./z_tools/haribote/

CC1		=./z_tools/cc1.exe -I$(INCPATH) -Os -Wall -quiet


default :
	make run

ipl10.bin : ipl10.nas
	$(TOOLSPATH)/nask.exe ipl10.nas ipl10.bin
		
asmhead.bin : asmhead.nas
	$(TOOLSPATH)/nask.exe asmhead.nas asmhead.bin
		
bootpack.gas : bootpack.c Makefile
	$(CC1) -o bootpack.gas bootpack.c 
	
bootpack.nas : bootpack.gas Makefile
	$(TOOLSPATH)/gas2nask.exe -a bootpack.gas bootpack.nas
	
bootpack.obj : bootpack.nas Makefile
	$(TOOLSPATH)/nask.exe bootpack.nas bootpack.obj
	
naskfunc.obj : naskfunc.nas Makefile
	$(TOOLSPATH)/nask.exe naskfunc.nas naskfunc.obj

bootpack.bim : bootpack.obj naskfunc.obj Makefile
	$(TOOLSPATH)/obj2bim.exe @$(TOOLSPATH)/haribote/haribote.rul out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj naskfunc.obj
# 3MB+64KB=3135KB

bootpack.hrb : bootpack.bim Makefile
	$(TOOLSPATH)/bim2hrb.exe bootpack.bim bootpack.hrb 0
	
asmhead.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb asmhead.sys

haribote.img : ipl10.bin asmhead.sys Makefile
	$(TOOLSPATH)/edimg imgin:./z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:asmhead.sys to:@: \
		imgout:haribote.img


run : haribote.img
	copy haribote.img .\z_tools\qemu\fdimage0.bin
	$(TOOLSPATH)\make.exe -C .\z_tools/qemu
	
clean:
	del *.img
	del *.bin
	del *.sys
	del *.obj
	del *.hrb
	del *.gas
	del *.map
	del *.bim
	del bootpack.nas