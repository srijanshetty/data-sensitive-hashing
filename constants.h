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
const int Alter = 10;                                       /**< Alternate versions of the hash function for differnt R values */
const float BaseR = 20;                                     /**< The hash functions are (p, q, k, R * k) sesitive */
const float c = 1.2;                                        /**< Each Alter version has a c^i * R tolerance approximation */
const int bucketnum = 9973;                                 /**< Number of buckets */
const int thresholdpoint = (int)(3 * datasize/bucketnum);   /**< # of points in a bucket for it to qualify for a radius test */
const float thresholdtable = (int)(0.4 * L);                /**< # of tables for a radius to suceed */
const float ETRatio = 0.3;
//for basic LSH test
// const int Alter = 1;
// const double BaseR = 5;

#endif // CONSTANTS_H_INCLUDED
