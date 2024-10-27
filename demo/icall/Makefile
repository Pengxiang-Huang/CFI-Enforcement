SRC = dispatch
CC = clang
CFALG = -flto=full -fvisibility=hidden -fsanitize=cfi-icall

all: $(SRC).bc $(SRC).ll $(SRC).out

without_cfi: CFALG =
without_cfi: $(SRC).bc $(SRC).ll $(SRC).out

$(SRC).bc: $(SRC).c
	$(CC) $(CFALG) -emit-llvm -c -o $@ $<

$(SRC).ll: $(SRC).bc
	llvm-dis $< -o $@ 

$(SRC).out: $(SRC).bc 
	$(CC) $< -flto=full -fuse-ld=lld -o $@ 

clean:
	rm -f $(SRC).out $(SRC).bc $(SRC).ll*

