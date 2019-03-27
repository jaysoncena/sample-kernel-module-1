obj-m += singlebyte.o

all:
	make -C /code/kernel/linux-4.20.4 M=$(PWD) modules

clean:
	make -C /code/kernel/linux-4.20.4 M=$(PWD) clean