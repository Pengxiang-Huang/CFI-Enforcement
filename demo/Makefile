SRC = dispatch
CC = clang
CFALG = -O2 -emit-llvm

all: $(SRC).bc $(SRC).ll $(SRC).out

$(SRC).bc: $(SRC).c
	$(CC) $(CFALG) -c -o $@ $<

$(SRC).ll: $(SRC).bc
	llvm-dis $< -o $@ 

$(SRC).out: $(SRC).c 
	$(CC) $(SRC).c -o $(SRC).out 

clean:
	rm -f $(SRC).out $(SRC).bc $(SRC).ll

