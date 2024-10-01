#include <stdio.h>
#include "vector.h"
#include "vectorf.h"
#include "vector_t.h"

int main(void)
{
    vector *vec = new_vector(NULL, 0);
    vectorf *vecf = new_vectorf(NULL, 0);
    vector_t *vect = new_vector_t(NULL, 0, sizeof(char));

    const char *greet = "____Hello World!____";
    for (int i = 0; i < 20; i++)
        push_t(vect, (void *)&greet[i]), pushf(vecf, (float)i/10), push(vec, i);

    double arr[] = {0.2, 0.4, 0.8, 0.9, 1.1};
    vector_t *vecd = new_vector_t(arr, 5, sizeof(double));
    for (double i = 1.1; i < 3.14; i += 0.1)
        push_t(vecd, &i);

    printf("len::%d, cap::%d\n", vec->length, vec->capacity);
    printf("len::%d, cap::%d\n", vecf->length, vecf->capacity);
    printf("len::%d, cap::%d\n", vect->length, vect->capacity);
    printf("len::%d, cap::%d\n", vecd->length, vecd->capacity);

    printf("\n");
    for (int i = 0; i < vec->length; i++)
        printf("int::%2d, float::%2.2f, generic::%c\n", at(vec, i), atf(vecf, i), *(char *)at_t(vect, i));

    for (int i = 0; i < vecd->length; i++)
        printf("generic::%2.2f\n", *(double *)at_t(vecd, i));

    delete(vec);
    deletef(vecf);
    delete_t(vect);
    delete_t(vecd);
}
