LIBINCLUDE = -ltrace -lm
DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

CC = gcc
CFLAGS = -g -Wall -I${DIR_INC}


SRC = $(wildcard ${DIR_SRC}/*.c)
DIR = $(notdir ${SRC})
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

TARGET = Finn_trace_analysis

BIN_TARGET = ${DIR_BIN}/${TARGET}




${BIN_TARGET}:${OBJ}
	$(CC) $(OBJ) -o $@ $(LIBINCLUDE)


${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBINCLUDE)

.PHONY:clean

test:
	./bin/Finn_trace_analysis /root/Desktop/libtrace_tutorial/traces/2minutes.erf.gz Test 30 5

run:

	./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200701011400.dump.gz  clifford_{0}-30/Analysis_sec_30s_200701011400 30 1



clean:
	rm -f all $(DIR_OBJ)/*.o
	rm -f all $(DIR_BIN)/*
	rm -f Test.csv
