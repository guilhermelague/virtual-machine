all: Linker.exe Assembler.exe VirtualMachine.exe 

Linker.exe: linker.c
	gcc linker.c -o Linker.exe

Assembler.exe: assembler.c
	gcc assembler.c -o Assembler.exe

VirtualMachine.exe: virtualMachine.c
	gcc virtualMachine.c -o VirtualMachine.exe

clean:
	rm *.exe