#include <time.h> // time
#include <sys/time.h> // gettimeofday

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct timeval tv;
	time_t t;
	struct tm *tm;

	t = time(NULL);
	gettimeofday(&tv, NULL);

	printf("time_t: %ld\n", t);

	printf("ctime %s", ctime(&t));
	printf("gettimeofday->ctime: %s", ctime(&tv.tv_sec));

	tm = gmtime(&t);
	printf("gmtime -> year %d month %d day %d\n", tm->tm_year + 1900, tm->tm_mon, tm->tm_mday);

	tm = localtime(&t);
	printf("localhost -> year %d month %d day %d\n", tm->tm_year + 1900, tm->tm_mon, tm->tm_mday);

	t = mktime(tm);
	printf("mktime -> time_t: %ld\n", t);

	/* already provides a \n character */
	printf("asctime: %s", asctime(tm));

	return EXIT_SUCCESS;
}
