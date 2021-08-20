#ifndef SHMUP_LINALG_H
#define SHMUP_LINALG_H

#include <stddef.h>

#define linalg_dims_eq(A_PTR, B_PTR) \
    ((A_PTR)->n == (B_PTR)->n && (A_PTR)->m == (B_PTR)->m)

#define linalg_addr_of_matrix_element(FMATRIX_PTR, ROW, COL) \
    (float *)((FMATRIX_PTR)->start + (ROW) * (FMATRIX_PTR)->m + (COL))

#define linalg_get_matrix_element(FMATRIX_PTR, ROW, COL) \
    (FMATRIX_PTR)->start[(ROW) * (FMATRIX_PTR)->m + (COL)]

/* does not include the size of the FMatrix struct itself */
#define linalg_sizeof_matrix(FMATRIX_PTR) \
    sizeof(float) * (FMATRIX_PTR)->n * (FMATRIX_PTR)->m \

#define linalg_sizeof_matrix_total(FMATRIX_PTR) \
    (sizeof(FMatrix) + sizeof(float) * (FMATRIX_PTR)->n * (FMATRIX_PTR)->m) \

#define linalg_is_matrix_square(FMATRIX_PTR) \
    ((FMATRIX_PTR)->n == (FMATRIX_PTR)->m)

typedef struct FMatrix FMatrix;

struct FMatrix
{
    size_t n;
    size_t m;
    float *start;
};

FMatrix *newFMatrix(size_t n, size_t m);
void fMatrixTranspose(FMatrix *fmat);
FMatrix *fMatrixDup(const FMatrix *fmat);
void fMatrixOnes(FMatrix *fmat);
void fMatrixZeros(FMatrix *fmat);
int fMatrixEye(FMatrix *fmat);
void fMatrixPrint(const FMatrix *fmat);
int fMatrixLUFactorize(const FMatrix *a, FMatrix *l, FMatrix *u);
FMatrix *fMatrixMult(const FMatrix *a, const FMatrix *b);

#endif
