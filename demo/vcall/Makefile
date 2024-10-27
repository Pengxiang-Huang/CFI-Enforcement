SRC = rtti
CC = clang++
# requires LLVMgold.so, enables link time optimization 
CFALG = -flto=full -fvisibility=hidden -fsanitize=cfi-icall -fuse-ld=lld

all: $(SRC).bc $(SRC).ll $(SRC).out

$(SRC).bc: $(SRC).cpp
	$(CC) $(CFALG) -emit-llvm -c -o $@ $<

$(SRC).ll: $(SRC).bc
	llvm-dis $< -o $@ 

$(SRC).out: $(SRC).cpp 
	$(CC) $< $(CFALG) -o $@ 

clean:
	rm -f $(SRC).out $(SRC).bc $(SRC).ll*

