// operations.c
#include "operation.h"

int add(int a, int b)
{
    return a + b;
}

int subtract(int a, int b)
{
    return a - b;
}

int multiply(int a, int b)
{
    return a * b;
}

int divide(int a, int b)
{
    if (b != 0)
    {
        return a / b;
    }
    else
    {
        return 0; // Return 0 if division by zero
    }
}

// here is a mismatch operation:
void NestFunc0(fptr_t2 fptr2, double x, double y)
{
    printf("NestFunc2, ID %f, %f\n", x, y);
    fptr2(x, y); // Using TyPM, we can know that all the callsites here cannot call function in irrelevant.h
                          // But still may call add, subtract, multiply, divide
                          // Using dataflow analysis can further solve fptr2 to add, not substraction, divide and multiply
};
void NestFunc1(fptr_t1 fptr, int x, int y)
{
    printf("NestFunc1, ID %d, %d\n", x, y);
    NestFunc0((fptr_t2)fptr, x, y);
};

void fptrCast(int x, int y)
{
    fptr_t1 fptr1 = add; // Using TyPM, we can know that all the callsites here cannot call function in irrelevant.h
                          // But still may call add, subtract, multiply, divide
                          // Using dataflow analysis can further solve fptr1 to add, not  
    NestFunc1(fptr1, x, y);
} 
void mismatch_operation()
{
    fptrCast(1, 2);
}