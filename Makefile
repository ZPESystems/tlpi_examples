CC=gcc
CLAGS=-Wall -Werror

all:
	$(CC) cp.c $(CLAGS) -o cp
	$(CC) tee.c $(CLAGS) -o tee
	$(CC) big_file.c $(CLAGS) -o big_file
	$(CC) -D_FILE_OFFSET_BITS=64 big_file_better.c $(CLAGS) -o big_file_better
	$(CC) append_seek_0.c $(CLAGS) -o append_seek_0
	$(CC) modify_env.c $(CLAGS) -o modify_env
	$(CC) show_argv.c $(CLAGS) -o show_argv
	$(CC) user_info.c $(CLAGS) -o user_info -lcrypt -g
	$(CC) time.c $(CLAGS) -o time
	$(CC) sys_limits.c $(CLAGS) -o sys_limits
	$(CC) proc_info.c $(CLAGS) -o proc_info
	$(CC) get_processes.c $(CLAGS) -o get_processes
	$(CC) file_monitor.c $(CLAGS) -o file_monitor
	$(CC) pipe.c $(CLAGS) -o pipe
	$(CC) pipe_filter.c $(CLAGS) -o pipe_filter
	$(CC) popen.c $(CLAGS) -o popen
	$(CC) fifo_server.c $(CLAGS) -o fifo_server
	$(CC) fifo_client.c $(CLAGS) -o fifo_client
	$(CC) sv_mq_view_all.c $(CLAGS) -o sv_mq_view_all
clean:
	rm cp tee big_file big_file_better append_seek_0 modify_env show_argv user_info time sys_limits proc_info get_processes file_monitor pipe pipe_filter popen fifo_server fifo_client sv_mq_view_all
