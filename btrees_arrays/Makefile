all: run 
run: run.o implementation.o 
	gcc -w run.o implementation.o -o run

run.o: run.c headers.h
	gcc -w -g -c run.c -D t=$(t) 

implementation.o: implementation.c headers.h
	gcc -w -g -c implementation.c -D t=$(t) 

clean:
	rm -rf *.o