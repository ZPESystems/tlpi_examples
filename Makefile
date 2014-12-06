all:
	gcc cp.c -Wall -Werror -o cp
	gcc tee.c -Wall -Werror -o tee
	gcc show_argv.c -Wall -Werror -o show_argv
	gcc user_info.c -Wall -Werror -o user_info
clean:
	rm cp tee show_argv
