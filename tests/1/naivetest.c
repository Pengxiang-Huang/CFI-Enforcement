#include <stdio.h>
#include <stdlib.h>


typedef void(*fptr_t1)(struct A*);
typedef void(*fptr_t2)(struct B*);


struct A {
    int id;
};
struct B {
    double id;
};

void function1a(struct A* a){
    printf("function1, ID %d\n", a->id);
}

void function1b(struct A* a){
    printf("function2, ID %d\n", a->id);
}

void function2a(struct B* a){
    printf("function2, ID %f\n", a->id);
}
void function2b(struct B* a){
    printf("function2, ID %f\n", a->id);
}

void NestFunc0(fptr_t2 fptr, struct B* b){
    printf("NestFunc2, ID %f\n", b->id);
    fptr2(b);   // can call any function in table1 and table2
}
void NestFunc1(fptr_t1 fptr, struct A* a){
    struct B b1={2.1};
    printf("NestFunc1, ID %d\n", a->id);
    NestFunc0((fptr_t2)fptr,&b1);  // should be a cast here
                                   // bitcast void (%struct.A.1*)* %11 to void (%struct.B.0*)*
                                   // So if a callsite is of type void (%struct.B.0*)*, it can also call all the functions of type (%struct.A.1*)* 
                                  // The callsite affected by this bitcast can be refined using data-flow analysis and typm.
}
int main(){
    struct A a1={1};
    struct B b1={1.1};
    
    fptr_t1 fptr1=function2a; // add function2a to jump table for fptr_t1
    fptr_t1 fptr2=function2b; // add function2b to jump table for fptr_t1
    fptr_t1 fptr_benign=function1a; 

    fptr1(&a1); // mismatch here, can call function2a,function2b. Refer to table1
    fptr2(&b1); // mismatch here, can call function2a,function2b  Refer to table1

    NestedFunc1(fptr_benign,&a1); // mismatch here, can call function2a,function2b
    
}

// look into test.ll
// jump table
/*
table1
Type void(*fptr_t1)(struct A*);
function1a
function2a  (added mismatch)
function2b  (added mismatch)

table2
Type void(*fptr_t2)(struct B*);
function2a
function2b
*/



//makefile

//data flow and bitcast analysis 