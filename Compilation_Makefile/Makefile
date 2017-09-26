main : main.o file_methods.o double_ended_linkedlist.o
	cc -o main main.o file_methods.o double_ended_linkedlist.o

main.o : main.c ./includes/double_ended_linkedlist.h ./includes/file_methods.h ./includes/_error.h
	cc -c main.c
file_methods.o : file_methods.c ./includes/file_methods.h ./includes/double_ended_linkedlist.h ./includes/_error.h
	cc -c file_methods.c
double_ended_linkedlist.o : double_ended_linkedlist.c ./includes/double_ended_linkedlist.h ./includes/_error.h
	cc -c double_ended_linkedlist.c
clean :
	rm main edit main.o file_methods.o double_ended_linkedlist.o

