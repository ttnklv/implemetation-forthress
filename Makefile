all: build

build: clean fort.o
	ld -o fort fort.o

fort.o: fort.asm 
	nasm -f elf64 -g -F dwarf fort.asm -o fort.o

clean:
	rm fort fort.o

