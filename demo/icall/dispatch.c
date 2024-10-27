#include <stdio.h>

typedef struct Shape Shape; 

typedef void (*DrawFunc) (int*); 

struct Shape{
	DrawFunc draw; 
} ;


// define two different functions 
void draw_circle(int* myint) {
	int * castint = (int*)myint;
	printf("draw circle print int is: %d\n", *castint);
}

void draw_rectangle(char* myint){
	int * castint = (int*)myint;
	printf("draw rectangle print int is: %d\n", *castint);
}



int main(){
	int test_val = 5;

	Shape Shape0 = {draw_circle};
	Shape Shape1 = {draw_rectangle};

	Shape * test0 = &Shape0;
	Shape * test1 = &Shape1;

	test0->draw(&test_val);
	test1->draw(&test_val);


	return 0;
}
