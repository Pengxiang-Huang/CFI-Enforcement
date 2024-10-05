SRC = test

CC  = clang
CXX = clang++

CFLAGS = -O2 -emit-llvm

# New output file for the optimized bitcode
OPT_BC = $(SRC)_optimized.bc


all: $(SRC).bc $(SRC).ll $(SRC).out $(OPT_BC) $(SRC)_optimized.ll

$(SRC).bc: $(SRC).c
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC).ll: $(SRC).bc
	llvm-dis $< -o $@

$(SRC).out: $(SRC).bc $(OPT_BC)
	$(CC) $(OPT_BC) -o $(SRC).out

$(OPT_BC): $(SRC).bc
	opt -load ../../build/CFIEnforcement.so -enable-new-pm=0 --CFIEnforcement $< -o $@

$(SRC)_optimized.ll: $(OPT_BC)
	llvm-dis $< -o $@

clean:
	rm -f $(SRC).bc $(SRC).ll $(SRC).out $(OPT_BC) $(SRC)_optimized.ll

.PHONY: clean

