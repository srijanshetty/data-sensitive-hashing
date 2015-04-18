#include "SHselection.h"
#include <iostream>
#include <memory.h>

using namespace std;

/**
 * FIXME: This piece of code is more or less useless in this implementation,
 * and only initializes decision with 0
 */
void SHSelection::radius_selection(string decision_file)
{
    memset(decisionsignal, 0, sizeof(decisionsignal));
    for(int i = 0; i < Alter - 1; i++)radius_test(i);
    for(int i = 0; i < datasize; i++)
    {
        if(decisionsignal[i] == 0)
        {
            decisionsignal[i] = 1;
            shg.decision[i] = Alter - 1;
        }
    }
    cout << "finished" <<endl;
    shg.decisionavailable = true;
    io.diskwrite_int(decision_file, shg.decision, datasize);
}

/**
 * FIXME: This part is not executed at all in this version
 */
void SHSelection::radius_test(int Rrank)
{
    cout << "SHSelection::radius_test " << Rrank << endl;
    memset(hashkeylength, 0, sizeof(hashkeylength));
    for(int k = 0; k < datasize; k++)
    {
        if(k%100000 == 0) cout << "current hashing data " << k << endl;
        shg.tableindex(shg.dataproduct[k],Rrank,shg.datahashresult[k]);
        for(int i = 0; i < L; i++) hashkeylength[i][shg.datahashresult[k][i]%bucketnum]++;
    }
    int sum = 0;
    for(int i = 0; i < datasize; i++)
    {
        int sumcount = 0;
        for(int j = 0; j < L; j++)
        {
            if(hashkeylength[j][shg.datahashresult[i][j]%bucketnum] >= thresholdpoint) sumcount++;
        }
        //cout<<"sumcount: "<<sumcount<<endl;
        //cout<<"thresholdtable: "<<thresholdtable<<endl;
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
