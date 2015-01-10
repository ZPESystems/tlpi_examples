#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>

static const char* LOG_FILE = "/tmp/ds.log";
static const char* CONFIG_FILE = "/tmp/ds.config";

static volatile sig_atomic_t hupReceived = 0;

static void sighupHandler(int sig)
{
	hupReceived = 1;
}

void readConfig(FILE *lg)
{
	FILE *f = fopen(CONFIG_FILE, "r");
	if (!f) {
		perror("fopen");
		fprintf(lg, "CONFIG ERROR\n");
		_exit(EXIT_FAILURE);
	}

	char conf[255];
	fscanf(f, "%s", conf);
	fprintf(lg, "CONFIG: %s\n", conf);
	fflush(lg);
}

int main()
{
	const int SLEEP_TIME = 15;
	int count = 0; // number of completed sleep calls
	int unsleept; // time remaining in sleep interval

	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sighupHandler;
	if (sigaction(SIGHUP, &sa, NULL) == -1) {
		perror("sigaction");
		_exit(EXIT_FAILURE);
	}

	struct stat sb;

	if (stat(CONFIG_FILE, &sb) == -1) {
		perror(CONFIG_FILE);
		_exit(EXIT_FAILURE);
	}

	if (daemon(0, 0) == -1) {
		perror("daemon");
		_exit(EXIT_FAILURE);
	}

	FILE *logf = fopen(LOG_FILE, "w");
	if (!logf) {
		perror("fopen");
		_exit(EXIT_FAILURE);
	}

	time_t t = time(NULL);
	fprintf(logf, "Log open in %s", ctime(&t));
	fflush(logf);
	readConfig(logf);

	unsleept = SLEEP_TIME;
	for (;;) {
		unsleept = sleep(unsleept); // returns > 0 when receive a signal

		if (hupReceived == 1) {
			fclose(logf);
			logf = fopen(LOG_FILE, "w");
			if (!logf) {
				perror("fopen");
				_exit(EXIT_FAILURE);
			}
			time_t t = time(NULL);
			fprintf(logf, "Log open in %s", ctime(&t));
			readConfig(logf);
			hupReceived = 0;
		}

		if (unsleept == 0) {
			count++;
			fprintf(logf, "Main: %d times\n", count);
			fflush(logf);
			unsleept = SLEEP_TIME;
		}
	}
}
