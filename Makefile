TOOLSPATH=./z_tools


default :
	make clean
	make run

bin : helloos.nas 
	$(TOOLSPATH)/nask.exe helloos.nas helloos.bin

helloos.img : Makefile
	make bin
	$(TOOLSPATH)/edimg imgin:./z_tools/fdimg0at.tek \
		wbinimg src:helloos.bin len:512 from:0 to:0   imgout:helloos.img

run : Makefile
	make helloos.img
	copy helloos.img .\z_tools\qemu\fdimage0.bin
	$(TOOLSPATH)\make.exe -C .\z_tools/qemu
	
clean:
	del *.img
	del *.bin