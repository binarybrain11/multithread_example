
struct thread_args{
    void *list;
    int l;
    int r;
    int (*cmp)(void*, void*);
    int elem_size;
    int *thread_count;
    pthread_mutex_t *thread_count_mutex;
};

void* mergesort(void *list, int l, int r, int (*cmp)(void*, void*), int elem_size);
void* simple_multithread_mergesort(void *voidptr);