obj-m += hello1.o

# .DEFAULT_GOAL := all

all:
	make -C /code/kernel/linux-4.20.4 M=$(PWD) modules

clean:
	make -C /code/kernel/linux-4.20.4 M=$(PWD) clean