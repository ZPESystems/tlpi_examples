#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>

#include <syslog.h>
#include <string.h>
#include <errno.h>

#include "error_aux.h"

static const char* CONFIG_FILE = "/tmp/ds.config";

static volatile sig_atomic_t hupReceived = 0;

static void sighupHandler(int sig)
{
	hupReceived = 1;
}

void readConfig()
{
	FILE *f = fopen(CONFIG_FILE, "r");
	if (!f) {
		syslog(LOG_ERR, "Coudl not open the config file: %s", strerror(errno));
		_exit(EXIT_FAILURE);
	}

	char conf[255];
	fscanf(f, "%s", conf);
	syslog(LOG_INFO, "CONFIG: %s", conf);
}

int main(int agc, char *argv[])
{
	const int SLEEP_TIME = 15;
	int count = 0; // number of completed sleep calls
	int unsleept; // time remaining in sleep interval

	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sighupHandler;
	if (sigaction(SIGHUP, &sa, NULL) == -1)
		exit_failure("sigaction");

	struct stat sb;

	if (stat(CONFIG_FILE, &sb) == -1)
		exit_failure(CONFIG_FILE);

	if (daemon(0, 0) == -1)
		exit_failure("daemon");

	openlog(argv[0], LOG_PID | LOG_CONS | LOG_NOWAIT, LOG_LOCAL0);

	time_t t = time(NULL);
	syslog(LOG_INFO, "Log open in: %s", ctime(&t));
	readConfig();

	unsleept = SLEEP_TIME;
	for (;;) {
		unsleept = sleep(unsleept); // returns > 0 when receive a signal

		if (hupReceived == 1) {
			t = time(NULL);
			syslog(LOG_INFO, "Log reopen in: %s", ctime(&t));
			readConfig();
			hupReceived = 0;
		}

		if (unsleept == 0) {
			count++;
			syslog(LOG_INFO, "Main: %d times", count);
			unsleept = SLEEP_TIME;
		}
	}
}
