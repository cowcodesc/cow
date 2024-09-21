#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef VECTOR_H

#define VECTOR_H
#define INIT_VEC_CAP 32

typedef struct vector
{
    uint16_t length;
    uint16_t capacity;
    int *begin;
    int *end;
} vector;

vector *new_vector(int *arr, size_t len)
{
    vector *v = (vector *)malloc(sizeof(vector));
    v->length = (uint16_t)len;
    v->capacity = (len > INIT_VEC_CAP / 2) ? v->length * 2 : INIT_VEC_CAP;

    if (arr == NULL) v->length = 0, v->capacity = INIT_VEC_CAP;

    v->begin = (int *)malloc(sizeof(int) * v->capacity);
    v->end = &(v->begin[v->length]);

    if (arr != NULL) memcpy(v->begin, arr, len*sizeof(int));
    return v;
}

void push(vector *vec, int value)
{
    if (vec->length == vec->capacity) vec = (vector *)realloc(vec, vec->length * 2), vec->capacity *= 2;

    *(vec->end) = value;
    vec->length++;
    vec->end = &(vec->begin[vec->length]);
}

void append(vector *vec, int *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
        push(vec, arr[i]);
}

int at(vector *vec, int idx)
{
    if (idx < 0) return vec->end[idx];
    return vec->begin[idx];
}

void delete(vector *vec)
{
    free(vec->begin);
    vec->begin = NULL, vec->end = NULL;

    free(vec);
    vec = NULL;
}

#endif
