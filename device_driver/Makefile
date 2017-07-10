obj-m += course_module.o
obj-m += kernel_probes.o
SRC=/lib/modules/$(shell uname -r)/build
SOURCES=handle_driver blk_ioctl

all: $(SOURCES)
	make -C $(SRC) M=$(PWD) modules

$(SOURCES):
	gcc $@.c -Wall -Werror -Wextra -o $@

clean:
	make -C $(SRC) M=$(PWD) clean
	rm -rf $(SOURCES)
