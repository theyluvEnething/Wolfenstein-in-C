#include<stdio.h>
#include<stdlib.h>

struct person {
   int age;
   float weight;
};

int main()
{
    // Declare a pointer to the struct and allocate memory
    struct person *ptr;
    ptr = (struct person*)malloc(sizeof(struct person));
    // Fill in the struct with data.
    ptr->age = 10;
    ptr->weight = 55.5;

    printf("%i\n", ptr->age);
    printf("%f\n", ptr->weight);
}