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
        float R[Alter];                                     // FIXME: Radius rations (predefined)
        float dataproduct[datasize][familysize];            // dotproduct of data and familyvectors
        float familyvector[familysize][D+1];                // vectors of the family
        int hashtableindex[L][M];                           // Stores the index of familyvectors for each concatenation function
        unsigned int datahashresult[datasize][L];           // hashresult: point -> table
        int datahashtable[L][datasize];                     // Hashresult: table -> point
        int decision[datasize];                             // FIXME: Always a zero vector in this implementation
        bool decisionavailable;
        bool isinit;

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
