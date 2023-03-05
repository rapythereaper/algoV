cli: algo_v.o cli.o
	gcc -o out/cli  out/algo_v.o out/cli.o -lSDL2

cli.o:	cli.h cli.c
	gcc	-c cli.c -o out/cli.o 

algo_v.o: algo_v.h algo_v.c
	gcc -c  algo_v.c -o out/algo_v.o

run: 
	./out/cli
	
clean:
	rm out/*.o out/cli