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


run:
	./bin/Finn_trace_analysis ~/Desktop/1113/202004080000.pcap.gz 202004080000_cl 30
	./bin/Finn_trace_analysis ~/Desktop/1113/202004080015.pcap.gz 202004080015_cl 30
	./bin/Finn_trace_analysis ~/Desktop/1113/202004080030.pcap.gz 202004080030_cl 30

clean:
	rm -f all $(DIR_OBJ)/*.o
	rm -f all $(DIR_BIN)/*
	rm -f Test.csv
