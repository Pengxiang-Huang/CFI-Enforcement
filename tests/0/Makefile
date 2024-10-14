SRC = test

CC  = clang
CXX = clang++

CFLAGS = -O2 -emit-llvm

all: $(SRC).bc $(SRC).ll $(SRC).out 

$(SRC).bc: $(SRC).c
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC).ll: $(SRC).bc
	llvm-dis $< -o $@

$(SRC).out: $(SRC).bc 
	$(CC) $(SRC).bc -o $(SRC).out

clean:
	rm -f $(SRC).bc $(SRC).ll $(SRC).out 

.PHONY: clean

