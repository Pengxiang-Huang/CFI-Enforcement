SRC = test

CC  = clang
CXX = clang++

CFLAGS = -O2 -emit-llvm

# New output file for the optimized bitcode
OPT_BC = $(SRC)_optimized.bc

all: $(SRC).bc $(SRC).ll $(SRC).out $(OPT_BC)

$(SRC).bc: $(SRC).c
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC).ll: $(SRC).bc
	llvm-dis $< -o $@

$(SRC).out: $(SRC).bc $(OPT_BC)
	$(CXX) $(SRC).c -o $(SRC).out

$(OPT_BC): $(SRC).bc
	opt -load ../../build/src/libCFIEnforcement.so --CFIEnforcement $< -o $@

clean:
	rm -f $(SRC).bc $(SRC).ll $(SRC).out $(OPT_BC)

.PHONY: clean

