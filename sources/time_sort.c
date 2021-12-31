#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mergesort.h"

int is_greater(void *a, void *b){
    return *(int*)a > *(int*)b;
}

int main(){
    int num_items = 1 << 27;
    int i;
    srand(time(NULL));
    int *list = malloc(num_items * sizeof(int));
    printf("Unsorted:\n[");
    for (i = 0; i < num_items; i++){
        /* modulo by num_items to have numbers scale with array size 
         * so the sorting is clear
         */
        list[i] = rand() % num_items;
        /*
        printf("%i, ", list[i]);
        */
    }
    printf("\b\b]\nSorted:\n[");
    clock_t start_t, end_t;
    double total_t;
    start_t = clock();
    int *sorted = mergesort(list, 0, num_items - 1, is_greater, sizeof(int));
    end_t = clock();
    total_t = ((double)(end_t - start_t))/CLOCKS_PER_SEC;
    /*
    for (i = 0; i < num_items; i++){
        printf("%i, ", sorted[i]);
    }
    */
    printf("\b\b]\nTime taken: %f\n", total_t);
    free(sorted);
    return 0;
}