all: run clean_obj

run:
	cl csourse.c

clean:
	del *.obj
	del *.exe
	del *.asm

clean_obj:
	del *.obj