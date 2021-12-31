#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mergesort.h"

#define PRINT_LIST 0
#define MAX_THREADS 4


int is_greater(void *a, void *b){
    return *(int*)a > *(int*)b;
}

int main(){
    /* magic number, takes about a minute to sort on my machine */
    int num_items = 1 << 27;
    int i;
    srand(time(NULL));
    int *list = malloc(num_items * sizeof(int));
    if(PRINT_LIST){
        printf("Unsorted:\n[");
    } else {
        printf("Creating list of %i elements...\n", num_items);
    }
    for (i = 0; i < num_items; i++){
        /* modulo by num_items to have numbers scale with array size 
         * so the sorting is clear if the list is printed
         */
        list[i] = rand() % num_items;
        if (PRINT_LIST){
            printf("%i, ", list[i]);
        }
    }
    if (PRINT_LIST){
        printf("\b\b]\nSorted:\n[");
    } else {
        printf("Sorting list with simple mergesort...\n");
    }
    clock_t start_t, end_t;
    double total_t;
    start_t = clock();
    int *sorted = mergesort(list, 0, num_items - 1, is_greater, sizeof(int));
    end_t = clock();
    total_t = ((double)(end_t - start_t))/CLOCKS_PER_SEC;
    if (PRINT_LIST){
        for (i = 0; i < num_items; i++){
            printf("%i, ", sorted[i]);
        }
        printf("\b\b]\n");
    }
    printf("Time taken: %f\n", total_t);
    free(sorted);

    printf("Sorting list with simple multithreaded mergesort...\n");
    int thread_count = 1;
    pthread_mutex_t thread_count_mutex;
    pthread_mutex_init(&thread_count_mutex, NULL);
    struct thread_args args = {list, 0, num_items - 1, is_greater, sizeof(int), &thread_count, &thread_count_mutex};
    start_t = clock();
    sorted = simple_multithread_mergesort(&args);
    end_t = clock();
    total_t = ((double)(end_t - start_t))/CLOCKS_PER_SEC;
    printf("Time taken: %f\n", total_t);
    free(sorted);

    return 0;
}