CompilerName := compiler
CFLAGS=-std=c11 -g -static 

main: main.c
	gcc $(CFLAGS) -o $(CompilerName) main.c

test: $(CompilerName)
	./test.sh

clean:
	rm -r $(CompilerName)
