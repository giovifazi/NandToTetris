VMtranslator: main.o funzioni.o
	gcc -o VMtranslator main.c funzioni.c

main.o: main.c
	gcc -c main.c

funzioni.o: funzioni.c funzioni.h
	gcc -c funzioni.c funzioni.h

clean:
	rm *o VMtranslator
