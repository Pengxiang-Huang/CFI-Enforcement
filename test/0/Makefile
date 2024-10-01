SRC = test

CC  = clang 

CFLAGS = -O2 -emit-llvm

all: $(SRC).bc $(SRC).ll $(SRC).out


$(SRC).bc: $(SRC).c 
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC).ll: $(SRC).bc
	llvm-dis $< -o $@

$(SRC).out: $(SRC).c 
	$(CC) $(SRC).c -o $(SRC).out 

clean: 
	rm -f $(SRC).bc $(SRC).ll $(SRC).out 

.PHONY: clean 
