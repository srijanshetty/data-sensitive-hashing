#include "myvector.h"
#include <iostream>

using namespace std;

/**
 * Compute the dot product of two points
 * @param dim      The number of dimensions
 * @param id1      First point
 * @param id2      Second point
 * @return dotproduct
 */
float MyVector::dotproduct(int dim, float id1[], float id2[])
{
    float result = 0;
    for (int i = 0; i < dim; i++)
    {
        result += id1[i] * id2[i];
    }
    return result;
}

/**
 * Compute the L2 norm distance between two points
 * @param dim      The number of dimensions
 * @param id1      First point
 * @param id2      Second point
 * @param bound    FIXME: Nothing in this version
 * @return L2 norm
 */
float MyVector::distancel2sq(int dim, float id1[], float id2[], float bound)
{
    // to be optimized: use bound to filt
    float result = 0;
    for (int i = 0; i < dim; i++)
    {
        result += (id1[i] - id2[i]) * (id1[i] - id2[i]);
    }
    if(result < 0) cout<<"error negative distance" <<endl;
    return result;
}
