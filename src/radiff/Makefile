include ../../config.mk

OBJ=main.o ../rdb.o ../ut.o ../w32utils.o ../rahash/crc16.o rdbdiff.o bytediff.o
BIN=radiff${BINSFX}
CC?=gcc
OBJ_RDB=rdbdiff_main.o rdbdiff.o ../rdb.o ../ut.o ../w32utils.o ../rahash/crc16.o
CFLAGS+=-I..


all: ${BIN} _bdiff rdbdiff$(BINSFX)
	
${BIN}: ${OBJ}
	${CC} ${CFLAGS} ${LDFLAGS} ${OBJ} ${LIBS} -o ${BIN}

_bdiff:
	-cd bdiff && ${MAKE} all

rdbdiff${BINSFX}: ${OBJ_RDB}
	${CC} ${LDFLAGS} -g ${OBJ_RDB} ${LIBS} -o rdbdiff${BINSFX}

clean:
	cd bdiff && ${MAKE} clean
	-rm -f ${OBJ_RDB} ${OBJ} ${BIN} rdbdiff

.PHONY: bdiff all clean
