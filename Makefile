all:
	gcc cp.c -Wall -Werror -o cp
	gcc tee.c -Wall -Werror -o tee
	gcc big_file.c -Wall -Werror -o big_file
	gcc -D_FILE_OFFSET_BITS=64 big_file_better.c -Wall -Werror -o big_file_better
	gcc append_seek_0.c -Wall -Werror -o append_seek_0
	gcc modify_env.c -Wall -Werror -o modify_env
	gcc show_argv.c -Wall -Werror -o show_argv
	gcc user_info.c -Wall -Werror -o user_info -lcrypt -g
	gcc time.c -Wall -Werror -o time
	gcc sys_limits.c -Wall -Werror -o sys_limits
	gcc proc_info.c -Wall -Werror -o proc_info
	gcc get_processes.c -Wall -Werror -o get_processes
	gcc file_monitor.c -Wall -Werror -o file_monitor
	gcc pipe.c -Wall -Werror -o pipe
	gcc pipe_filter.c -Wall -Werror -o pipe_filter
clean:
	rm cp tee big_file big_file_better append_seek_0 modify_env show_argv user_info time sys_limits proc_info get_processes file_monitor pipe pipe_filter
