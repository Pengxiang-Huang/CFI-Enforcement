SRC = naivetest

CC  = clang
CXX = clang++


CFLAGS = -g -flto=full -fvisibility=hidden -fsanitize=cfi-icall 
LDFLAGS = -flto=full -fuse-ld=lld	
all: $(SRC).bc $(SRC).ll $(SRC).out $(SRC)_patched.out

$(SRC).bc: $(SRC).c
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC).ll: $(SRC).bc
	llvm-dis $< -o $@

$(SRC).out: $(SRC).bc 
	$(CC) $(SRC).bc $(LDFLAGS) -o $(SRC).out

$(SRC)_patched.out: $(SRC).ll
	$(TOOL) $(SRC).ll -o $(SRC)_patched.ll
	$(CC) $(SRC)_patched.ll $(LDFLAGS) -o $(SRC)_patched.out


clean:
	rm -f $(SRC).bc $(SRC).ll $(SRC).out $(SRC)_patched.*

.PHONY: clean

