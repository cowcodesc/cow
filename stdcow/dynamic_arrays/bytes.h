#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef BYTES_H

#define BYTES_H
#define INIT_VEC_CAP 32

typedef struct bytes
{
    uint16_t length;
    uint16_t capacity;
    uint8_t *begin;
    uint8_t *end;
} bytes;

bytes *new_bytes(uint8_t *arr, size_t len)
{
    bytes *v = (bytes *)malloc(sizeof(bytes));
    v->length = (uint16_t)len;
    v->capacity = (len > INIT_VEC_CAP / 2) ? v->length * 2 : INIT_VEC_CAP;

    if (arr == NULL) v->length = 0, v->capacity = INIT_VEC_CAP;

    v->begin = (uint8_t *)malloc(sizeof(uint8_t) * v->capacity);
    v->end = &(v->begin[v->length]);

    if (arr != NULL) memcpy(v->begin, arr, len*sizeof(uint8_t));
    return v;
}

void pushb(bytes *vec, uint8_t value)
{
    if (vec->length == vec->capacity) vec = (bytes *)realloc(vec, vec->length * 2), vec->capacity *= 2;

    *(vec->end) = value;
    vec->length++;
    vec->end = &(vec->begin[vec->length]);
}

void appendb(bytes *vec, uint8_t *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
        pushb(vec, arr[i]);
}

uint8_t atb(bytes *vec, int idx)
{
    if (idx < 0) return vec->end[idx];
    return vec->begin[idx];
}

void deleteb(bytes *vec)
{
    free(vec->begin);
    vec->begin = NULL, vec->end = NULL;

    free(vec);
    vec = NULL;
}

#endif
