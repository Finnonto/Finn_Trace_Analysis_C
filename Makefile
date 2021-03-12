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
	$(CC) $(OBJ) -o $@  $(LIBINCLUDE)


${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBINCLUDE)

.PHONY:clean

sim:
	./bin/Finn_trace_analysis simulation -l 500000 -r 30000  -k 20 -z 1.4 -t  5  -a 4  Clifford Clifford_HT Clifford_HTo  PingLi  -it 10 -Tbs 16384

run:

	./bin/Finn_trace_analysis  /mnt/d/Download/MAWI2007/200701011400.dump.gz 


clean:
	rm -f all $(DIR_OBJ)/*.o
	rm -f all $(DIR_BIN)/*
	rm -f Test.csv
