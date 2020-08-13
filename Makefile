TOOLSPATH	=./z_tools
NASK		=$(TOOLSPATH)/nask.exe 
GAS2NASK	=$(TOOLSPATH)/gas2nask.exe -a 
INCPATH  	= ./z_tools/haribote/
CC1			=./z_tools/cc1.exe -I$(INCPATH) -Os -Wall -quiet


default :
	make run

ipl10.bin : ipl10.nas
	$(NASK) ipl10.nas ipl10.bin
		
asmhead.bin : asmhead.nas
	$(NASK) asmhead.nas asmhead.bin
		
bootpack.gas : bootpack.c Makefile
	$(CC1) -o bootpack.gas bootpack.c 
	
bootpack.nas : bootpack.gas Makefile
	$(GAS2NASK) bootpack.gas bootpack.nas
	
bootpack.obj : bootpack.nas Makefile
	$(NASK) bootpack.nas bootpack.obj
	
naskfunc.obj : naskfunc.nas Makefile
	$(NASK) naskfunc.nas naskfunc.obj

bootpack.bim : bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj Makefile
	$(TOOLSPATH)/obj2bim.exe @$(TOOLSPATH)/haribote/haribote.rul out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj
# 3MB+64KB=3135KB

bootpack.hrb : bootpack.bim Makefile
	$(TOOLSPATH)/bim2hrb.exe bootpack.bim bootpack.hrb 0
	
asmhead.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb asmhead.sys

radish.img : ipl10.bin asmhead.sys Makefile
	$(TOOLSPATH)/edimg imgin:./z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:asmhead.sys to:@: \
		imgout:radish.img
		
hankaku.bin : hankaku.txt Makefile
	$(TOOLSPATH)/makefont.exe hankaku.txt hankaku.bin

hankaku.obj :hankaku.bin Makefile
	$(TOOLSPATH)/bin2obj.exe hankaku.bin hankaku.obj _hankaku

# graphic.obj : graphic.nas Makefile
# 	$(NASK) graphic.nas graphic.obj

# dsctbl.obj : dsctbl.nas Makefile
# 	$(NASK) dsctbl.nas dsctbl.obj

%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj



run : radish.img
	copy radish.img .\z_tools\qemu\fdimage0.bin
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