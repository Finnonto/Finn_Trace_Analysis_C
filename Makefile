LIBINCLUDE = -ltrace -lm -lmpdec
DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

CC = gcc -mcmodel=medium
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
	./bin/Finn_trace_analysis simulation -l 500000 -r 30000  -k 20 -z 1.4 -t 3  -a 4  Clifford  Clifford_cdf Clifford_cdf_parallel_mhash_est  Clifford_cdf_parallel -it 20 -tbint 0 -tbidx 0 -Tbs 65536 -resolution 14 -interth 16 16 -interspan 4 -Chash 0 -CCliffordSeed 1 -hseed 3 -mhash 4 

sim_AD:
	./bin/Finn_trace_analysis simulation -l 10000000 -r 30000  -k 20 -z -1 -t  1 -PA 0.999999 -a 2  Clifford   PingLi  -it 10 -Tbs 16384 -resolution 2

sim_AS:
	./bin/Finn_trace_analysis simulation -l 1000000 -r 30000  -k 20 -z 0 -t  3 -PA 0.999999 -a 2  Clifford   PingLi  -it 10 -Tbs 16384 -resolution 16

sim_ALLDIFF:
	./bin/Finn_trace_analysis simulation -l 10000000 -k 20 -z -1 -t 1000 -a 1  Clifford 

trace:

	./bin/Finn_trace_analysis trace /mnt/c/Users/Lab108/Desktop/libtrace_tutorial/traces/2minutes.erf -a 4  Clifford  Clifford_cdf  Clifford_cdf_parallel Clifford_cdf_parallel_interpolation -resolution MAX


clean:
	rm -f all $(DIR_OBJ)/*.o
	rm -f all $(DIR_BIN)/*

