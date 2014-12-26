all:magiclisp

BaseCalculation.o:BaseCalculation.c BaseCalculation.h MLDebug.h
	gcc -c BaseCalculation.c -o BaseCalculation.o

MLLib.o:MLLib.c MLLib.h LinuxList.h BaseCalculation.h MLDebug.h
	gcc -c MLLib.c -o MLLib.o

MLEngine.o:MLEngine.c MLEngine.h MLDebug.h
	gcc -c	MLEngine.c -o MLEngine.o

magiclisp.o:main.c BaseCalculation.h MLLib.h LinuxList.h MLEngine.h MLDebug.h
	gcc -c main.c -o magiclisp.o -lcurses

MLDebug.o:MLDebug.c MLDebug.h
	gcc -c MLDebug.c -o MLDebug.o

magiclisp:BaseCalculation.o MLLib.o MLEngine.o magiclisp.o MLDebug.o
	gcc BaseCalculation.o MLLib.o MLEngine.o magiclisp.o -lcurses -o magiclisp

clean:
	rm BaseCalculation.o MLLib.o MLEngine.o magiclisp.o magiclisp
