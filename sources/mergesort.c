#include <string.h>
#include <stdlib.h>
#include <pthread.h>

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

/* Simple stupid multithreading */

void* multimergesort(void *list, int l, int r, int (*cmp)(void*, void*), int elem_size){
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