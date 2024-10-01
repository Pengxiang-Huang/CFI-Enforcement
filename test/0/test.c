#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LEN 50 

typedef void (*ftpr)(char *, char *);

struct A { 
	ftpr handler;
} ;

struct B { 
	struct A a;
};

struct C {
	struct A a;
};

void copy_with_check(char * dst, char *src) {
	if (strlen(src) < MAX_LEN) strcpy(dst, src);
}


void copy_no_check(char* dst, char *src){
	strcpy(dst, src);
}

struct B b = { .a = { .handler = &copy_with_check}};


struct C c = { .a = { .handler = &copy_no_check}};


int main() {
	char buf[] = "user input";
	char *dst = (char*) malloc(sizeof(char) * 80); 

	(*b.a.handler)(dst, buf);
	(*c.a.handler)(buf, dst);

	printf("the result is: %s\n", dst);
	return 0;
}

