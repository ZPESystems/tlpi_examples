CC=gcc
CLAGS=-Wall -Werror -g

SRCS=cp tee big_file append_seek_0 modify_env show_argv time sys_limits proc_info \
	get_processes file_monitor \
	pipe pipe_filter popen \
	fifo_server fifo_client \
	sv_mq_view_all

SRCS_SPECIAL=big_file_better \
	user_info

.PHONY: others

all: $(SRCS) others

$(SRCS):
	$(CC) $@.c $(CLAGS) -o $@

others:
	$(CC) -D_FILE_OFFSET_BITS=64 big_file_better.c $(CLAGS) -o big_file_better
	$(CC) user_info.c $(CLAGS) -o user_info -lcrypt
	
clean:
	rm $(SRCS) $(SRCS_SPECIAL)
