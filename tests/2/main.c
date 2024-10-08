// main.c
#include <stdio.h>
#include "operation.h"
#include "util.h"
#include "irrelevant.h"




// void NestFunc0(fptr_t2 fptr, double x, double y){
//     printf("NestFunc2, ID %f, %f\n", x,y);
//     fptr2(x,y);   // can call any function in table1 and table2
// }
// void NestFunc1(fptr_t1 fptr, int x, int y){
//     printf("NestFunc1, ID %d, %d\n", x, y);
//     NestFunc0((fptr_t2)fptr,x,y);
// }

// void fptrCast(int x, int y){
//     fptr_t1 fptr1 = int_count_ducks;
//     NestFunc1(fptr1, x, y);
// }   // Using TyPM, we can know NestedFunc


int main() {
    int a = 12, b = 4;

    // Function pointer array for operations
    operation_func operations[] = {add, subtract, multiply, divide};

    // Display operations
    print_operations();

    // Select and perform operations
    for (int i = 0; i < 4; i++) {
        int result = perform_operation(operations[i], a, b);
        print_result(i, result);
    }

    fptr_t1 fptr1=int_count_ducks; // int_count_ducks is address taken, and mismatched
    fptr1(1,2);


    return 0;
}
