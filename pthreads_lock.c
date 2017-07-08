#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>

#define FILENAME "output"
#define MSG_COUNT 100

void do_one_thing();
void do_another_thing();

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char** argv)
{
	pthread_t thread1, thread2;

	pthread_create(&thread1,
		NULL, //thread arguments
		(void*) do_one_thing, // thread routine
		NULL); // routine's arguments

	pthread_create(&thread2,
		NULL, //thread arguments
		(void*) do_another_thing, // thread routine
		NULL); // routine's arguments

	// let's both thread finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("It's done!");

}

void do_one_thing()
{
	int i;
	for (i = 0; i < MSG_COUNT; i++){
		sleep(1);
		pthread_mutex_lock(&mutex);
		int fd;
		if ((fd = open(FILENAME, O_WRONLY | O_CREAT | O_APPEND,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) >= 0){
			dprintf(fd, "do_one_thing: %d\n", i);
			close(fd);
		} else {
			printf("Cannot open file: %s", strerror(errno));
		}
		pthread_mutex_unlock(&mutex);
	}
}

void do_another_thing()
{
	int i;
	for (i = 0; i < MSG_COUNT; i++){
		sleep(1);
		pthread_mutex_lock(&mutex);
		int fd;
		if ((fd = open(FILENAME, O_WRONLY | O_CREAT | O_APPEND,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) >= 0){
			dprintf(fd, "do_another_thing: %d\n", i);
			close(fd);
		} else {
			printf("Cannot open file: %s", strerror(errno));
		}
		pthread_mutex_unlock(&mutex);
	}
}
