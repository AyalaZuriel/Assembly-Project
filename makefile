all:main
main: main.c preAssembler.c assembler.c lists.c fileFunctions.c firstPass.c firstPassAssist.c secondPass.c symbolTable.c preAssembler.h assembler.h lists.h  fileFunctions.h firstPass.h firstPassAssist.h lists.h symbolTable.h globalDeclarations.h
	gcc -g -ansi -pedantic -Wall main.c preAssembler.c assembler.c lists.c fileFunctions.c firstPass.c firstPassAssist.c secondPass.c symbolTable.c -o main
clean: rm *~

