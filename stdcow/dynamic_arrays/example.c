#include <stdio.h>
#include "vector.h"
#include "vectorf.h"

int main(void)
{
    vector *vec = new_vector(NULL, 0);
    vectorf *vecf = new_vectorf(NULL, 0);
    for (int i = 0; i < 20; i++)
        pushf(vecf, (float)i/10), push(vec, i);

    printf("len::%d, cap::%d\n", vec->length, vec->capacity);
    printf("len::%d, cap::%d\n", vecf->length, vecf->capacity);

    printf("\n");
    for (int i = 0; i < 20; i++)
        printf("int::%2d, float::%2.2f\n", at(vec, i), atf(vecf, i));

    delete(vec);
    deletef(vecf);
}
