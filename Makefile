all:
	gcc cp.c -Wall -Werror -o cp
	gcc tee.c -Wall -Werror -o tee
	gcc show_argv.c -Wall -Werror -o show_argv
clean:
	rm cp tee show_argv
