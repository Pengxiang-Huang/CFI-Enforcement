// operations.h
#ifndef OPERATIONS_H
#define OPERATIONS_H
typedef void (*fptr_t1)(int x, int y);
typedef void (*fptr_t2)(double x, double y);

typedef int (*operation_func)(int, int);

int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);
void mismatch_operation();
#endif
