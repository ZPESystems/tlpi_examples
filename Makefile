CC=gcc
CLAGS=-Wall -Werror -g

SRCS=cp tee append_seek_0 modify_env show_argv time sys_limits proc_info \
	get_processes \
	pipe pipe_filter popen \
	fifo_server fifo_client \
	mmap_cat mmap_replace mmap_anon mprotect \
	posix_shm_writer posix_shm_reader \
	daemon_imp \
	fork fork_file_sharing vfork \

ifneq ($(shell uname -s),Darwin)
SRCS+=big_file file_monitor \
	sv_mq_view_all svshm_writer svshm_reader \
	daemon_glibc daemon_sighup \
	program_termination
endif

SRCS_SPECIAL=big_file_better \
	user_info

.PHONY: others

all: $(SRCS) others

$(SRCS):
	$(CC) $@.c $(CLAGS) -o $@

others:
ifneq ($(shell uname -s),Darwin)
	$(CC) -D_FILE_OFFSET_BITS=64 big_file_better.c $(CLAGS) -o big_file_better
	$(CC) user_info.c $(CLAGS) -o user_info -lcrypt
else
	$(info MacOS: Some files will not be compiled because they are not compatbile)
endif
	
clean:
	rm -rf $(SRCS) $(SRCS_SPECIAL)

re: clean all
