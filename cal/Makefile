OBJS= stack.o calculator.o main.o 
calculator:$(OBJS)
	cc $(OBJS) -o calculator -lreadline -lhistory -lm

main.o:main.c stack.h calculator.h common.h
	cc -c main.c -o main.o

calculator.o:calculator.c stack.c calculator.h stack.h common.h
	cc -c calculator.c -o calculator.o	
	
stack.o:stack.c stack.h common.h
	cc -c stack.c -o stack.o
	
.PHONY:clean
clean:
	rm $(OBJS) calculator


