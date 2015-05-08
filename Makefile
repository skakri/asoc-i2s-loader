obj-m := asoc-i2s-loader.o

HEADERS := /lib/modules/$(shell uname -r)/build

default:
	${MAKE} -C ${HEADERS} SUBDIRS=${PWD} modules

clean:
	${MAKE} -C ${HEADERS} SUBDIRS=${PWD} clean
