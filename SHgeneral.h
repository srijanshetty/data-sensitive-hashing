#ifndef SHGENERAL_H_INCLUDED
#define SHGENERAL_H_INCLUDED

class SHGeneral;

#include "constants.h"
#include "data.h"
#include "myvector.h"
#include "myrandom.h"
#include <string>
#include <stdlib.h>

using namespace std;

class SHGeneral{
    private:
        float R[Alter];                                  /**< Different Radius ratios for alternative hashtables */
        float dataproduct[datasize][familysize];         /**< dotproduct of data and familyvectors */
        float familyvector[familysize][D+1];             /**< vectors of the family */
        int hashtableindex[L][M];                        /**< Stores the index of familyvectors for each concatenation function */
        unsigned int datahashresult[datasize][L];        /**< The hasvalues of a datapoint in each hash table */
        int datahashtable[L][datasize];                  /**< This stores the points in sorted order. Refer to README */
        int decision[datasize];                          /**< The radius ratio for each of the points */
        bool decisionavailable;                          /**< Indicates if decision is loaded into memory */
        bool isinit;                                     /**< Indicator for making init idempotent */

    public:
        friend class SHSelection;
        friend class SHIndex;

    public:
        SHGeneral();
        void init();
        void tableindex(float [], int, unsigned int []);

    private:
        void family_generator();
        void generate_hashtableindex();
        void familysample(int result[], int size, int needsize);
        void productcomputer();
};

#endif // SHGENERAL_H_INCLUDED
