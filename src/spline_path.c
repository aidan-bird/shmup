#include <stdlib.h>

#include "./spline_path.h"


SplinePath *
newSplinePath(const PathDef *def)
{
    SplinePath *ret;
    size_t vectorSize;
    size_t n;

    n = def->count - 1;
    vectorSize = sizeof(SplinePath) + n * (4 * sizeof(float));
    ret = malloc(vectorSize);
    if (!ret)
        goto error1;
    ret->coeffA = (float *)((uint8_t *)ret + sizeof(SplinePath));
    ret->coeffB = (float *)((uint8_t *)ret->coeffA + n * sizeof(float));
    ret->coeffC = (float *)((uint8_t *)ret->coeffB + n * sizeof(float));
    ret->coeffD = (float *)((uint8_t *)ret->coeffC + n * sizeof(float));
    for (size_t i = 0; i < n; i++) {
        /* compute the coeffC */


        /* used coeffC to compute the other coefficients */

    }
    return ret;
error2:;
    free(ret);
error1:;
    return NULL;
}
