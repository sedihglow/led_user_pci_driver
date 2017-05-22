CC = gcc
CPPFLAGS = -c -Wall -pthread -O0 -g -m64 -std=c99
LDFLAGS = -pthread -m64 -lpci -lz

pciLED.out:				main.o err_handle.o userLED.o
						$(CC) main.o err_handle.o userLED.o -o pciLED.out $(LDFLAGS) 

main.o:					main.c ./userLED.h ./utility_sys.h
						$(CC) ./main.c $(CPPFLAGS)

err_handle.o:			./err_handle/err_handle.c ./utility_sys.h
						$(CC) ./err_handle/err_handle.c $(CPPFLAGS) 

userLED.o:				userLED.c ./utility_sys.h
						$(CC) ./userLED.c $(CPPFLAGS) 

.PHONY: check
check:					# check for memory leak
						$(info -- Checking For Memory Leaks --)
						make
						valgrind --leak-check=full ./pciLED.out

.PHONY: debug
debug:					# GNU debugger
						$(info -- Debugging --)
						make
						gdb ./pciLED.out

.PHONY: clean
clean:					# clean the directory
						$(info -- Cleaning The Directory --)
						rm -rf *.o pciLED.out

.PHONY: run 
run:					# run the program as follows
						$(info -- Running Program --)
						make
						./pciLED.out
