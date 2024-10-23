SRC = dispatch
CC = clang
# requires LLVMgold.so, enables link time optimization 
CFALG = -flto -fvisibility=hidden -fsanitize=cfi-icall

all: $(SRC).bc $(SRC).ll $(SRC).out

$(SRC).bc: $(SRC).c
	$(CC) $(CFALG) -emit-llvm -c -o $@ $<

$(SRC).ll: $(SRC).bc
	llvm-dis $< -o $@ 

$(SRC).out: $(SRC).c 
	$(CC) $< $(CFALG) -o $@ 

clean:
	rm -f $(SRC).out $(SRC).bc $(SRC).ll*

