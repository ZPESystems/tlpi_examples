all:
	gcc cp.c -Wall -Werror -o cp
	gcc tee.c -Wall -Werror -o tee
	gcc big_file.c -Wall -Werror -o big_file
	gcc -D_FILE_OFFSET_BITS=64 big_file_better.c -Wall -Werror -o big_file_better
clean:
	rm cp tee big_file big_file_better
