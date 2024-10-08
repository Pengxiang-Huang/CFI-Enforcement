#include "irrelevant.h"
#include <stdio.h>

// Irrelevant function implementation
int int_count_ducks(int number_of_ducks, int counter) {
    printf("Counter: %d, There are %d ducks quacking!\n", counter, number_of_ducks);
    return number_of_ducks * 2;  // Totally irrelevant calculation
}

// Irrelevant function to print a random message
void print_irrelevant_message() {
    printf("This is an irrelevant message. Have a nice day!\n");
}
