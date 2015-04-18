#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

const int D = 54;
const int K = 20;
const int L = 50;
const int M = 20;
const int datasize = 100000;
const int querysize = 100;
const int familysize = 100;
//selective hashing specific
const int Alter = 1;
const float BaseR = 1.1;
const float c = 1.2;
const int bucketnum = 9973;
const int thresholdpoint = (int)(3*datasize/bucketnum);
const float thresholdtable = (int)(0.4*L);
const float ETRatio = 0.3;
//for basic LSH test
// const int Alter = 1;
// const double BaseR = 5;

#endif // CONSTANTS_H_INCLUDED
