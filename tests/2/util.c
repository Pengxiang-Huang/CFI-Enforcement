// utility.c
#include <stdio.h>
#include "util.h"
#include "operation.h"

void print_operations() {
    printf("Operations:\n");
    printf("0: Addition\n");
    printf("1: Subtraction\n");
    printf("2: Multiplication\n");
    printf("3: Division\n");
}

void print_result(int operation, int result) {
    const char* operation_names[] = {"Addition", "Subtraction", "Multiplication", "Division"};
    printf("%s Result: %d\n", operation_names[operation], result);
}

int perform_operation(operation_func op, int a, int b) {
    return op(a, b);
}
