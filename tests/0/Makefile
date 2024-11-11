SRC = test

CC  = clang
CXX = clang++

CFLAGS = -g -flto=full -fvisibility=hidden -fsanitize=cfi-icall
LDFLAGS = -flto=full -fuse-ld=lld	
all: $(SRC).bc $(SRC).ll $(SRC).out 

$(SRC).bc: $(SRC).c
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC).ll: $(SRC).bc
	llvm-dis $< -o $@

$(SRC).out: $(SRC).bc 
	$(CC) $(SRC).bc $(LDFLAGS) -o $(SRC).out

clean:
	rm -f $(SRC).bc $(SRC).ll $(SRC).out 

.PHONY: clean

