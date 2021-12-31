#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <mergesort.h>

#ifndef MAX_THREADS
#define MAX_THREADS 1
#endif

/* Returns a new sorted list. l and r are the leftmost and rightmost indexes
 * to sort, for example a 5 element list might have l=0, r=4 to sort the
 * whole list. cmp is a funcction that returns 0 when the two elements
 * should remain in the same place, otherwise the elements should be 
 * swapped. To sort in ascending order, cmp might be a function like
 * 
 * int is_greater(void* a, void* b){
 *  return *(int*)a > *(int*)b;
 * }
 * 
 * elem_size referes to the size of each element in bytes.
 */
void* mergesort(void *list, int l, int r, int (*cmp)(void*, void*), int elem_size){
    if (l == r){
        void *res = malloc(elem_size);
        memcpy(res, list + (l  * elem_size), elem_size);
        return res;
    }

    /* Find a midpoint to split the list */
    int m = (l + r) / 2;
    /* Sort each half of the list */
    void* list1 = mergesort(list, l, m, cmp, elem_size);
    void* list2 = mergesort(list, m+1, r, cmp, elem_size);

    /* If one list was empty, return the other one */
    if (list1 == NULL){
        return list2;
    }
    if (list2 == NULL){
        return list1;
    }

    void* ret_list = malloc(elem_size * (r - l + 1));
    
    /* merge each half of the list together */
    void *ret_index = ret_list;
    void *a = list1;
    void *b = list2;
    void *list1_end = list1 + (elem_size * (m - l + 1));
    void *list2_end = list2 + (elem_size * (r - m));
    /* While there are elements in each half of the list */
    while (a < list1_end && b < list2_end){
        if (cmp(a, b)){
            /* move b into the return list */
            memcpy(ret_index, b, elem_size);
            ret_index += elem_size;
            b += elem_size;
        } else {
            /* move a into the return list */
            memcpy(ret_index, a, elem_size);
            ret_index += elem_size;
            a += elem_size;
        }
    }
    /* Clean up any leftovers */
    while (a < list1_end){
        memcpy(ret_index, a, elem_size);
        ret_index += elem_size;
        a += elem_size;
    }
    while (b < list2_end){
        memcpy(ret_index, b, elem_size);
        ret_index += elem_size;
        b += elem_size;
    }
    free(list1);
    free(list2);
    return ret_list;
}

/* Simple multithreading */
void* simple_multithread_mergesort(void *voidptr){
    struct thread_args *args = (struct thread_args*) voidptr;
    if (args->l == args->r){
        void *res = malloc(args->elem_size);
        memcpy(res, args->list + (args->l  * args->elem_size), args->elem_size);
        return res;
    }

    /* Find a midpoint to split the list */
    int m = (args->l + args->r) / 2;
    /* Sort each half of the list */
    void *list1 = NULL;
    void *list2 = NULL;
    struct thread_args args1 = {args->list, args->l, m, args->cmp, args->elem_size};
    pthread_t list1_t;
    pthread_mutex_lock(args->thread_count_mutex);
    if (*(args->thread_count) < MAX_THREADS){
        pthread_create(&list1_t, NULL, simple_multithread_mergesort, &args1);
        *(args->thread_count) = *(args->thread_count) + 1;
        pthread_mutex_unlock(args->thread_count_mutex);
    } else {
        pthread_mutex_unlock(args->thread_count_mutex);
        list1 = mergesort(args->list, args->l, m, args->cmp, args->elem_size);
    }
    struct thread_args args2 = {args->list, m+1, args->r, args->cmp, args->elem_size};
    pthread_t list2_t;
    pthread_mutex_lock(args->thread_count_mutex);
    if (*(args->thread_count) < MAX_THREADS){
        pthread_create(&list2_t, NULL, simple_multithread_mergesort, &args2);
        *(args->thread_count) = *(args->thread_count) + 1;
        pthread_mutex_unlock(args->thread_count_mutex);
    } else {
        pthread_mutex_unlock(args->thread_count_mutex);
        list2 = mergesort(args->list, m+1, args->r, args->cmp, args->elem_size);
    }
    if (list1 == NULL){
        pthread_join(list1_t, &list1);
    }
    if (list1 == NULL){
        pthread_join(list2_t, &list2);
    }

    /* If one list was empty, return the other one */
    if (list1 == NULL){
        return list2;
    }
    if (list2 == NULL){
        return list1;
    }

    void* ret_list = malloc(args->elem_size * (args->r - args->l + 1));
    
    /* merge each half of the list together */
    void *ret_index = ret_list;
    void *a = list1;
    void *b = list2;
    void *list1_end = list1 + (args->elem_size * (m - args->l + 1));
    void *list2_end = list2 + (args->elem_size * (args->r - m));
    /* While there are elements in each half of the list */
    while (a < list1_end && b < list2_end){
        if (args->cmp(a, b)){
            /* move b into the return list */
            memcpy(ret_index, b, args->elem_size);
            ret_index += args->elem_size;
            b += args->elem_size;
        } else {
            /* move a into the return list */
            memcpy(ret_index, a, args->elem_size);
            ret_index += args->elem_size;
            a += args->elem_size;
        }
    }
    /* Clean up any leftovers */
    while (a < list1_end){
        memcpy(ret_index, a, args->elem_size);
        ret_index += args->elem_size;
        a += args->elem_size;
    }
    while (b < list2_end){
        memcpy(ret_index, b, args->elem_size);
        ret_index += args->elem_size;
        b += args->elem_size;
    }
    free(list1);
    free(list2);
    return ret_list;
}