LIBINCLUDE = -ltrace -lm -lmpdec
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
	$(CC)  -g -o $@  $(OBJ) $(LIBINCLUDE)


${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBINCLUDE)



.PHONY:clean

sim:
	./bin/Finn_trace_analysis simulation -l 500000 -r 30000  -k 20 -z 1.4 -t  3 -PA 0.999999 -a 2  Clifford   PingLi  -it 1 -Tbs 16384 -resolution 12

sim_AD:
	./bin/Finn_trace_analysis simulation -l 10000000 -r 30000  -k 20 -z 1 -t  1 -PA 0.999999 -a 2  Clifford   PingLi  -it 10 -Tbs 16384 -resolution 2

sim_AS:
	./bin/Finn_trace_analysis simulation -l 1000000 -r 30000  -k 20 -z 0 -t  3 -PA 0.999999 -a 2  Clifford   PingLi  -it 10 -Tbs 16384 -resolution 16


trace:

	./bin/Finn_trace_analysis trace ~/Desktop/libtrace_tutorial/traces/2minutes.erf -a 1 Clifford_cdf -resolution MAX


clean:
	rm -f all $(DIR_OBJ)/*.o
	rm -f all $(DIR_BIN)/*

