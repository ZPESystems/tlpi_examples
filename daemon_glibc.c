#include <unistd.h>

int main()
{
	daemon(0, 0); // do chdir to root dir and redirects all open fds to /dev/null
	sleep(30); // just to verify the output in ps
	return 0;
}
