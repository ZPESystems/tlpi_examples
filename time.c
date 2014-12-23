/**
 * Changing the TZ env var changes the output of ctime, asctime and strftime
 * Example of how to set an TZ env var and test:
 * 	TZ=":Pacific/Auckland" ./time
 * Or use the setlocale calls as did in this source file
 */

#include <time.h> // time
#include <sys/time.h> // gettimeofday
#include <locale.h> // setlocale

#include <stdio.h>
#include <stdlib.h>

void show_time_functs()
{
	struct timeval tv;
	time_t t;
	struct tm *tm;
	char out[200];

	t = time(NULL);
	gettimeofday(&tv, NULL);

	printf("time_t: %ld\n", t);

	printf("ctime %s", ctime(&t));
	printf("gettimeofday->ctime: %s", ctime(&tv.tv_sec));

	tm = gmtime(&t);
	printf("gmtime -> year %d month %d day %d\n", tm->tm_year + 1900, tm->tm_mon, tm->tm_mday);

	tm = localtime(&t);
	printf("localhost -> year %d month %d day %d\n", tm->tm_year + 1900, tm->tm_mon, tm->tm_mday);

	strftime(out, sizeof(out), "%Y %B %d %A", tm);
	printf("strftime: Year Month Day WeekName: %s\n", out);

	t = mktime(tm);
	printf("mktime -> time_t: %ld\n", t);

	/* already provides a \n character */
	printf("asctime: %s", asctime(tm));
}

int main(int argc, char *argv[])
{
	char *new_locale = NULL;
	char *df_locale = setlocale(LC_ALL, "");

	printf("Default locale: %s\n", df_locale);
	show_time_functs();

	new_locale = setlocale(LC_ALL, "en_US");
	printf("\nNew locale: %s\n", new_locale);

	show_time_functs();

	return EXIT_SUCCESS;
}
