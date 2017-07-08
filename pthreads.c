#include <stdio.h>
#include <pthread.h>

void do_one_thing(int*);
void do_another_thing(int*);
void do_wrap_up(int, int);

int r1 = 0, r2=0;

int main(int argc, char** argv)
{
	pthread_t thread1, thread2;

	pthread_create(&thread1,
		NULL, //thread arguments
		(void*) do_one_thing, // thread routine
		(void*) &r1); // routine's arguments

	pthread_create(&thread2,
		NULL, //thread arguments
		(void*) do_another_thing, // thread routine
		(void*) &r2); // routine's arguments

	// let's both thread finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	do_wrap_up(r1, r2);
}

void do_one_thing(int* pnum_times)
{
	int i, j, x;
	for (i = 0; i < 50; i++){
		printf("doing one thing\n");
		for (j = 0; j < 100000; j++)
			x = x + 1;
		(*pnum_times)++;
	}
}

void do_another_thing(int* pnum_times)
{
	int i, j, x;
	for (i = 0; i < 50; i++){
		printf("doing another thing\n");
		for (j = 0; j < 100000; j++)
			x = x + 1;
		(*pnum_times)++;
	}
}

void do_wrap_up(int one_times, int another_times)
{
	int total;
	total = one_times + another_times;
	printf("wrap up: one thing %d, another %d, total %d\n", one_times, another_times, total);
}
