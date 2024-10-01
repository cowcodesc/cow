#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 60

typedef struct node
{
    int arr[BLOCK_SIZE];
    struct node *next;
    struct node *prev;
} node;

typedef struct list
{
    uint8_t offset;
    uint16_t length;
    struct node *head;
    struct node *tail;
} list;

list *new_list(int *arr, size_t len)
{
    list *l = (list *)malloc(sizeof(list));

    l->offset = 0, l->length = 0;
    l->head = NULL, l->tail = NULL;
    if (arr == NULL) return l;

    l->length = len / BLOCK_SIZE;
    l->offset = len - l->length * BLOCK_SIZE;

    l->head = (node *)malloc(sizeof(node));
    l->head->next = NULL, l->head->prev = NULL;
    l->tail = l->head;

    size_t offset = 0;
    while (len > 0)
    {
        l->tail->next = NULL;
        if (len >= BLOCK_SIZE) memcpy(l->tail->arr, &arr[offset], BLOCK_SIZE*sizeof(int));
        else
        {
            memcpy(l->tail->arr, &arr[offset], len*sizeof(int));
            break;
        }

        l->tail->next = (node *)malloc(sizeof(node));
        l->tail->next->prev = l->tail;
        l->tail = l->tail->next;

        len -= BLOCK_SIZE;
        offset += BLOCK_SIZE;
    }
    return l;
}

void append_list(list *l, int *arr, size_t len)
{
    if (l->offset)
        (len > BLOCK_SIZE - l->offset) ?
            memcpy(&(l->tail->arr[l->offset]), arr, (BLOCK_SIZE - l->offset)*sizeof(int)) :
            memcpy(&(l->tail->arr[l->offset]), arr, len*sizeof(int));
    int *begin = &arr[(BLOCK_SIZE - l->offset)];
}

void push_list()
{
}

void pop_list()
{
}

void print_list(list *l)
{
    node *n = l->head;
    while (n->next)
    {
        for (size_t i = 0; i < BLOCK_SIZE; ++i) printf("%d ", n->arr[i]);
        n = n->next;
    }
    for (size_t i = 0; i < l->offset; ++i) printf("%d ", n->arr[i]);
    printf("\n");
}

void r_iterate()
{
}

void at_list()
{
}

int main(void)
{
    const int n = 64;
    int arr[n];

    for (size_t i = 0; i < n; ++i) arr[i] = i;

    list *l = new_list(arr, n);
    printf("length :: %u, offset :: %u\n", l->length, l->offset);
    iterate(l);
    free(l->head);
    free(l);
    return 0;
}
