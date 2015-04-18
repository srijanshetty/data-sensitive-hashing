#ifndef KNN_H_INCLUDED
#define KNN_H_INCLUDED

#include "constants.h"
#include "myvector.h"

class Knn{
    public:
    int knnlist[K];
    float distlist[K];
    float sqrtbound;
    private:
    int tochange;
    float bound;

    public:
    void init();
    void linear_scan(float [][D], float []);
    void addvertex(float [][D], int, float []);

    private:
    void computebound();
};

#endif // KNN_H_INCLUDED
