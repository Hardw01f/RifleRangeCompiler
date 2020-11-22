CompilerName := compiler
CFLAGS=-std=c11 -g -static 
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

main: main.c
	#gcc $(CFLAGS) -o $(CompilerName) main.c
	gcc $(CFLAGS) -c $(SRCS)
	gcc $(CFLAGS) $(OBJS) -o $(CompilerName)

$(OBJS): main.h

test: $(CompilerName)
	./test.sh

clean:
	rm -r $(CompilerName) *.s *.o a.out tmp
