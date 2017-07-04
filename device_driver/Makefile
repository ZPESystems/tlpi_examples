obj-m += course_module.o
SRC=/lib/modules/$(shell uname -r)/build

all:
	make -C $(SRC) M=$(PWD) modules
	gcc handle_driver.c -Wall -Werror -Wextra -o handle_driver
clean:
	make -C $(SRC) M=$(PWD) clean
	rm -rf handle_driver
