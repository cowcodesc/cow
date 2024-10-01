#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef VECTOR_T_H

#define VECTOR_T_H
#define INIT_VEC_CAP 32

typedef struct vector_t
{
    uint16_t length;
    uint16_t capacity;
    void *begin;
    void *end;
    size_t elem;
} vector_t;

vector_t *new_vector_t(void *arr, size_t len, size_t elem)
{
    vector_t *v = (vector_t *)malloc(sizeof(vector_t));
    v->elem = elem;
    v->length = (uint16_t)len;
    v->capacity = (len > INIT_VEC_CAP / 2) ? v->length * 2 : INIT_VEC_CAP;

    if (arr == NULL) v->length = 0, v->capacity = INIT_VEC_CAP;

    v->begin = (void *)malloc(elem * v->capacity);
    v->end = (v->begin + elem * v->length);

    if (arr != NULL) memcpy(v->begin, arr, len*elem);
    return v;
}

void push_t(vector_t *vec, void *value)
{
    if (vec->length == vec->capacity) vec = (vector_t *)realloc(vec, vec->length * 2), vec->capacity *= 2;

    memcpy(vec->end, value, vec->elem);
    vec->length++;
    vec->end = (vec->begin + vec->elem * vec->length);
}

void append_t(vector_t *vec, void *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
        push_t(vec, (arr + i*vec->elem));
}

void *at_t(vector_t *vec, int idx)
{
    if (idx < 0) return vec->end + idx * vec->elem;
    return vec->begin + idx * vec->elem;
}

void delete_t(vector_t *vec)
{
    free(vec->begin);
    vec->begin = NULL, vec->end = NULL;

    free(vec);
    vec = NULL;
}

#endif
