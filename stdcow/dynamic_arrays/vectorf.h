#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef VECTORF_H

#define VECTORF_H
#define INIT_VEC_CAP 32

typedef struct vectorf
{
    uint16_t length;
    uint16_t capacity;
    float *begin;
    float *end;
} vectorf;

vectorf *new_vectorf(float *arr, size_t len)
{
    vectorf *v = (vectorf *)malloc(sizeof(vectorf));
    v->length = (uint16_t)len;
    v->capacity = (len > INIT_VEC_CAP / 2) ? v->length * 2 : INIT_VEC_CAP;

    if (arr == NULL) v->length = 0, v->capacity = INIT_VEC_CAP;

    v->begin = (float *)malloc(sizeof(float) * v->capacity);
    v->end = &(v->begin[v->length]);

    if (arr != NULL) memcpy(v->begin, arr, len*sizeof(float));
    return v;
}

void pushf(vectorf *vec, float value)
{
    if (vec->length == vec->capacity) vec = (vectorf *)realloc(vec, vec->length * 2), vec->capacity *= 2;

    *(vec->end) = value;
    vec->length++;
    vec->end = &(vec->begin[vec->length]);
}

void appendf(vectorf *vec, float *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
        pushf(vec, arr[i]);
}

float atf(vectorf *vec, int idx)
{
    if (idx < 0) return vec->end[idx];
    return vec->begin[idx];
}

void deletef(vectorf *vec)
{
    free(vec->begin);
    vec->begin = NULL, vec->end = NULL;

    free(vec);
    vec = NULL;
}

#endif
