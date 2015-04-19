#ifndef SHSELECTION_H_INCLUDED
#define SHSELECTION_H_INCLUDED

class SHSelection;
#include "constants.h"
#include "data.h"
#include <string>

using namespace std;

class SHSelection{
    public:

    private:
        int decisionsignal[datasize];                       /**< Store if we have found a radius ratio for a given point */
        int hashkeylength[L][bucketnum];                    /**< Number of points in each bucket of concatenative function */

    public:
        //SHSelection();
        void radius_selection(string decision_file);
    private:
        void radius_test(int);

};

#endif // SHSELECTION_H_INCLUDED
