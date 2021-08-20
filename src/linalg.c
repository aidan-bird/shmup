#include "./utils.h"
#include "./linalg.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define CACHE_LINE_WIDTH 64
#define STRIDE (CACHE_LINE_WIDTH / sizeof(float))

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * makes a new n by m matrix of floats.
 * returns NULL on error.
 */
FMatrix *
newFMatrix(size_t n, size_t m)
{
    size_t vectorSize;
    FMatrix *ret;
    
    vectorSize = sizeof(FMatrix) + n * m * sizeof(float);
    ret = malloc(vectorSize);
    if (!ret)
        return NULL;
    ret->n = n;
    ret->m = m;
    ret->start = (float *)((uint8_t *)ret + sizeof(FMatrix));
    return ret;
}

/*
 * REQUIRES
 * fmat is valid
 *
 * MODIFIES
 * fmat
 *
 * EFFECTS
 * replaces fmat with its transpose.
 */
void
fMatrixTranspose(FMatrix *fmat)
{
    float tmp[fmat->n * fmat->m];

    for (size_t i = 0; i < fmat->n ; i++)
        for (size_t j = 0; j < fmat->m ; j++)
            tmp[j * fmat->m + i] = linalg_get_matrix_element(fmat, i, j);
    memcpy(fmat->start, tmp, sizeof(tmp));
}

/*
 * REQUIRES
 * fmat is valid
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * duplicates fmat.
 */
FMatrix *
fMatrixDup(const FMatrix *fmat)
{
    FMatrix *ret;

    ret = newFMatrix(fmat->n, fmat->m);
    if (!ret)
        return NULL;
    memcpy(ret->start, fmat->start, linalg_sizeof_matrix(fmat));
    return ret;
}

/*
 * REQUIRES
 * fmat is valid
 *
 * MODIFIES
 * fmat
 *
 * EFFECTS
 * sets all elements in fmat to 1.0f
 */
void
fMatrixOnes(FMatrix *fmat)
{
    for (size_t i = 0; i < (fmat->n * fmat->m); i++)
        fmat->start[i] = 1.0f;
}

/*
 * REQUIRES
 * fmat is valid
 *
 * MODIFIES
 * fmat
 *
 * EFFECTS
 * sets all elements in fmat to 0
 */
void
fMatrixZeros(FMatrix *fmat)
{
    memset(fmat->start, 0, linalg_sizeof_matrix(fmat));
}

/*
 * REQUIRES
 * fmat is valid
 *
 * MODIFIES
 * fmat
 *
 * EFFECTS
 * sets fmat to be the identity matrix
 * fmat must be a square matrix
 * returns non-zero on error (i.e., fmat is not square)
 */
int
fMatrixEye(FMatrix *fmat)
{
    if (!linalg_is_matrix_square(fmat))
        return -1;
    fMatrixZeros(fmat);
    for (size_t i = 0; i < fmat->n; i++)
        linalg_get_matrix_element(fmat, i, i) = 1.0f;
    return 0;
}

/*
 * REQUIRES
 * fmat is valid
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * prints fmat
 */
void
fMatrixPrint(const FMatrix *fmat)
{
    for (size_t i = 0; i < fmat->n; i++) {
        for (size_t j = 0; j < fmat->m; j++)
            printf("%-12e ", linalg_get_matrix_element(fmat, i, j));
        putchar('\n');
    }
}

/*
 * REQUIRES
 * a, l, and u are valid.
 *
 * MODIFIES
 * l, u
 *
 * EFFECTS
 * computes the LU factorization of a.
 * the results are stored in l and u.
 * l and u must have the same dimensions as a.
 * returns non-zero on error
 */
int
fMatrixLUFactorize(const FMatrix *a, FMatrix *l, FMatrix *u)
{
    if (!linalg_dims_eq(a, l) || !linalg_dims_eq(a, u))
        return -1;
    fMatrixEye(l);
    fMatrixZeros(u);
    return 0;
}

/*
 * REQUIRES
 * a and b are valid
 *
 * MODIFIES
 * a
 *
 * EFFECTS
 * computes a * b and stores the result into a.
 * returns NULL on error
 */
FMatrix *
fMatrixMult(const FMatrix *a, const FMatrix *b)
{
    FMatrix *ret;
    size_t ansRows;
    size_t ansCols;
    float bSubVector[STRIDE];

    if (a->m != b->n)
        return NULL;
    ansRows = a->n;
    ansCols = b->m;
    ret = newFMatrix(ansRows, ansCols);
    if (!ret)
        return NULL;
    fMatrixZeros(ret);
    /* cache efficient impl */
    for (size_t alpha = 0; alpha < ansCols; alpha++) {
        for (size_t i = 0; i < b->n; i += STRIDE) {
            /* load one cache-line's worth of column from b */
            for (size_t j = 0; j < MIN(STRIDE, b->n); j++)
                bSubVector[j] = linalg_get_matrix_element(b, i + j, alpha);
            /* work on a cache-line's worth of row from a for each row in a */
            for (size_t j = 0; j < a->n; j++) {
                for (size_t k = 0; k < MIN(STRIDE, a->m); k++) {

                    /* 
                     * TODO this can be more cache efficient by
                     * storing the multiplication result into another vector
                     * and then memcpy'ing later.
                     *
                     * anything that iterates over rows will not be cache 
                     * efficient
                     *
                     * right now this function is about a 2.5x speed up from
                     * the simple impl
                     */
                    linalg_get_matrix_element(ret, j, alpha) +=
                        linalg_get_matrix_element(a, j, i * STRIDE + k)
                        * bSubVector[k];
                }
            }
        }
    }
    return ret;
}

FMatrix *
fMatrixMultSimple(const FMatrix *a, const FMatrix *b)
{
    FMatrix *ret;
    size_t ansRows;
    size_t ansCols;

    if (a->m != b->n)
        return NULL;
    ansRows = a->n;
    ansCols = b->m;
    ret = newFMatrix(ansRows, ansCols);
    if (!ret)
        return NULL;
    fMatrixZeros(ret);
    /* simple impl */
    for (size_t i = 0; i < ansRows; i++) {
        for (size_t j = 0; j < ansCols; j++) {
            for (size_t k = 0; k < a->m; k++) {
                linalg_get_matrix_element(ret, i, j) += 
                    linalg_get_matrix_element(a, i, k)
                    * linalg_get_matrix_element(b, k, j);
            }
        }
    }
    return ret;
}

/*
 * REQUIRES
 * none
 *
 * MODIFIES
 * none
 *
 * EFFECTS
 * used for testing/debugging linalg.c
 */
void
testLinalg()
{
    FMatrix *mat;

    mat = newFMatrix(5, 5);

    puts("zeros");
    fMatrixZeros(mat);
    fMatrixPrint(mat);
    putchar('\n');

    puts("ones");
    fMatrixOnes(mat);
    fMatrixPrint(mat);
    putchar('\n');

    puts("identity");
    fMatrixEye(mat);
    fMatrixPrint(mat);
    putchar('\n');

    puts("identity transpose");
    fMatrixTranspose(mat);
    fMatrixPrint(mat);
    putchar('\n');

    puts("mat(4, 0) = 10.5");
    linalg_get_matrix_element(mat, (mat->n - 1), 0) = 10.5f;
    fMatrixPrint(mat);
    putchar('\n');

    puts("transpose");
    fMatrixTranspose(mat);
    fMatrixPrint(mat);
    putchar('\n');


    FMatrix *a;
    FMatrix *b;
    FMatrix *c;
    a = newFMatrix(3, 3);
    b = newFMatrix(3, 1);
    float adat[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    float bdat[] = {9, 6, 3};
    memcpy(a->start, adat, sizeof(adat));
    memcpy(b->start, bdat, sizeof(bdat));
    puts("a");
    fMatrixPrint(a);
    putchar('\n');
    puts("b");
    fMatrixPrint(b);
    putchar('\n');
    puts("a * b");
    c = fMatrixMult(a, b);
    fMatrixPrint(c);
    putchar('\n');

    FMatrix *large;
    large = newFMatrix(100, 100);
    size_t k = 0;
    for (size_t i = 0; i < large->n; i++)
        for (size_t j = 0; j < large->m; j++) 
            linalg_get_matrix_element(large, i, j) = k++;
    double tmp;
    getExecTime(fMatrixMult(large, large), &tmp);
    printf("fMatrixMult(large, large): %f\n", tmp);
    getExecTime(fMatrixMultSimple(large, large), &tmp);
    printf("fMatrixMultSimple(large, large): %f\n", tmp);
    if (mat)
        free(mat);
    if (a)
        free(a);
    if (b)
        free(b);
    if (c)
        free(c);
    if (large)
        free(large);
}

