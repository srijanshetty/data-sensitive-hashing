#include "SHselection.h"
#include <iostream>
#include <memory.h>

using namespace std;

/**
 * Compute optimum values of Rrank for each of the points by iterating
 * over all possibilities.
 */
void SHSelection::radius_selection(string decision_file)
{
    memset(decisionsignal, 0, sizeof(decisionsignal));

    /* Check which Alternate value of Rrank is good for the points */
    for(int i = 0; i < Alter - 1; i++) radius_test(i);

    for(int i = 0; i < datasize; i++)
    {
        /* If we couldn't find a good Rrank, we assign
           it be the maximum possible, i.e., the tightest grid */
        if(decisionsignal[i] == 0)
        {
            decisionsignal[i] = 1;
            shg.decision[i] = Alter - 1;
        }
    }
    cout << "finished" <<endl;

    // Now store the radius decision and that the decision is avaliable
    shg.decisionavailable = true;
    io.diskwrite_int(decision_file, shg.decision, datasize);
}

/**
 * Select the optimum radius ratio which ensures that the grids are stable.
 * We start with a larger intial grid size, and provide Alter possible grid sizes
 * for each point. Now, if a higher layer is very densely populated (denoted by threshold)
 * we use a smaller grid for that point and a larger Rrank.
 * Algorithm: We hash every point and then compute the number of points in the buckets of
 * each concatenative function. If for this given setting of Rrank, we
 * get a data point 'k' for which the each of the concatenative functions have
 * a large number of points nearby, we use this value of Rrank for all
 * computations of hashresult for 'k' and thereby use a tighter grid.
 * @param Rank         The radius ratio we want to test
 */
void SHSelection::radius_test(int Rrank)
{
    cout << "SHSelection::radius_test " << Rrank << endl;
    memset(hashkeylength, 0, sizeof(hashkeylength));

    /* We want to count the number of points hashed to each bucket
       of each concatenative function, hashkeylength stores this value.*/
    for(int k = 0; k < datasize; k++)
    {
        if(k % 100000 == 0) cout << "current hashing data " << k << endl;

        // Generate the hashtable for datapoint 'k' with given radius ratio index
        shg.tableindex(shg.dataproduct[k], Rrank, shg.datahashresult[k]);

        // Count the number of points in each bucket of each concatenative function
        for(int i = 0; i < L; i++) hashkeylength[i][shg.datahashresult[k][i] % bucketnum]++;
    }

    int sum = 0;        // Keeps a count of number of points for this current Rrank
    for(int i = 0; i < datasize; i++)
    {
        /* Compute the number of concatenative functions which have
           more than the threshold number of points */
        int sumcount = 0;
        for(int j = 0; j < L; j++)
        {
            if(hashkeylength[j][shg.datahashresult[i][j] % bucketnum] >= thresholdpoint) sumcount++;
        }

        //cout << "sumcount: " << sumcount << endl;
        //cout << "thresholdtable: " << thresholdtable << endl;

        /* If the number of concatenative functions passing the thresholdpoints
           is greater than the threshhold for concatenative functions, we set
           decision to Rrank */
        if(sumcount >= thresholdtable)
        {
            //cout<<"entered"<<endl;
            if(decisionsignal[i] == 0)
            {
                //cout<<"entered insert"<<endl;
                sum++;
                decisionsignal[i] = 1;
                shg.decision[i] = Rrank;
            }
        }
    }
    cout << sum << " points qualified for Round" << Rrank <<endl;
    //int forc;cin>>forc;
}
