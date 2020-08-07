TOOLSPATH=./z_tools


default :
	make clean
	make put
	make run

helloos.bin : helloos.nas 
	$(TOOLSPATH)/nask.exe helloos.nas helloos.bin

helloos.img : helloos.bin
	$(TOOLSPATH)/edimg imgin:./z_tools/fdimg0at.tek \
		wbinimg src:helloos.bin len:512 from:0 to:0   imgout:helloos.img
		
fin.sys : fin.nas
	$(TOOLSPATH)/nask.exe fin.nas fin.sys

run : helloos.img
	copy helloos.img .\z_tools\qemu\fdimage0.bin
	$(TOOLSPATH)\make.exe -C .\z_tools/qemu
	
put : helloos.img helloos.bin fin.sys
	$(TOOLSPATH)/edimg imgin:./z_tools/fdimg0at.tek \
		wbinimg src:helloos.bin len:512 from:0 to:0 \
		copy from:fin.sys to:@: \
		imgout:helloos.img
	
clean:
	del *.img
	del *.bin
	del *.sys