#include "SHindex.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iterator>

using namespace std;

bool datakeycompare(datakey a,datakey b)
{
    if(a.key < b.key) return true;
    if(a.key == b.key)
    {
        if(a.dataid < b.dataid) return true;
    }
    return false;
}

/**
 * Construct the necessary data structures which serve as the index
 * @param decision_file     File storing optimum Rrank values
 */
void SHIndex::index_construct(string decision_file)
{
    // shg contains the data structures needed
    shg.init();

    // Load the decision file if not in memory
    if(!shg.decisionavailable) io.diskread_int(decision_file.c_str(), shg.decision, datasize);

    // Compute an initial tableindex for all the datapoints
    for(int k = 0; k < datasize; k++)
    {
        if(k % 20000 == 0) cout<< "current hashing data " << k << endl;
        shg.tableindex(shg.dataproduct[k], shg.decision[k], shg.datahashresult[k]);
    }

    for(int j = 0; j < L; j++)
    {
        int hashkey;

        /* We compute the hashvalues of all datapoints for this hashtable and sort
           it, with points at a higher layer having a larger hashkey */
        for(int i = 0; i < datasize; i++)
        {
            temptable[i].dataid = i;

            // the (hashresult % bucketnum) gives us the bucket index of a point for a given hashtable
            hashkey = shg.datahashresult[i][j] % bucketnum;

            // The multiplicative factor ensures that we get the proper layer to which this point belongs to
            temptable[i].key = shg.decision[i] * bucketnum + hashkey;
        }

        /* temptable stores the bucketindices of the datapoints, note that
           the points with a larger ratio will have a larger bucket index
           due to the multiplicative factor involved */
        sort(temptable, temptable + datasize, datakeycompare);

        int q = 0;
        hashkeyindex[j][0][0] = 0;
        hashkeylength[j][0][0] = 0;
        for(int i = 0; i < datasize; i++)
        {
            /* Since the data is not representative of the entire
               distribution and we have to make sure that we are ready
               for all the keys.
               So for the points that are not in the sorted queue,
               we need to assign them a bucketindex.*/
            while(q != temptable[i].key)
            {
                q++;
                /* For the points not in the array, we have to assign
                   the bucket for the hashkey. We assign the bucket
                   index equal to the next point in the array which
                   is temptable[i].dataid.*/
                hashkeyindex[j][q/bucketnum][q%bucketnum] = i;
                hashkeylength[j][q/bucketnum][q%bucketnum] = 0;
            }

            /* The multiplicative factor earlier ensured correct layer as
               q/bucketnum = Rrank */
            hashkeylength[j][q/bucketnum][q%bucketnum]++;

            /* Given a hashkey, we get the bucketindex by hashkeyindex[][][hashkey]
               and we use this to find the points which are stored in order
               at datahashtable[j], so for a bucketlength b and bucketindex, we
               have values at datahashtable[j][b +j] where j varies from 0 to bucketlength*/
            shg.datahashtable[j][i] = temptable[i].dataid;
        }

        // Print the number of Buckets which have 0 values and which are normal
        int nullsum = 0;
        int normalsum = 0;
        for(int i = 0; i < Alter * bucketnum; i++)
        {
            if(hashkeylength[j][i/bucketnum][i%bucketnum] == 0) nullsum++;
            else normalsum++;
        }
        cout << "table " << j << " construted" << endl;
        cout << nullsum << " " << normalsum << endl;
    }

    //for test
    double sumchecknum = 0;
    double optimized = 0;
    double avechecknum;
    for(int i = 0; i < L; i++)
    {
        for(int k = 0; k < Alter; k++)
        {
            for(int j = 0; j < bucketnum; j++)
            {
                sumchecknum += hashkeylength[i][k][j] * hashkeylength[i][k][j] / (double) datasize;
                optimized += hashkeylength[i][k][j] / (double) bucketnum;
            }
        }
    }
    avechecknum = sumchecknum / L;
    cout << "bucket balance index is " << avechecknum << endl;
    cout << "optimized bucket balance is " << optimized / L << endl;
}

/**
 * Serialize the index from disk
 * @param index_file filename
 */
void SHIndex::index_write(string index_file)
{
    FILE *fp;
    fp = fopen(index_file.c_str(),"wb");
    if(fp == NULL)
    {
        cout << "Cannot open file!" << endl;;
        exit(1);
    }
    fwrite(shg.familyvector[0], sizeof(float), familysize*(D+1), fp);
    fwrite(shg.hashtableindex[0], sizeof(int), L*M, fp);
    fwrite(shg.datahashresult[0], sizeof(unsigned int), datasize*L, fp);
    fwrite(shg.datahashtable[0], sizeof(int), L*datasize, fp);
    fwrite(hashkeyindex[0][0], sizeof(int), L * Alter * bucketnum, fp);
    fwrite(hashkeylength[0][0], sizeof(int), L * Alter * bucketnum, fp);
    fclose(fp);
}

/**
 * Deserialize the index from disk
 * @param index_file filename
 */
void SHIndex::index_load(string index_file)
{
    FILE *fp;
    fp = fopen(index_file.c_str(),"rb");
    if(fp == NULL)
    {
        cout << "Cannot open file!" << endl;;
        exit(1);
    }
    fread(shg.familyvector[0], sizeof(float), familysize*(D+1), fp);
    fread(shg.hashtableindex[0], sizeof(int), L*M, fp);
    fread(shg.datahashresult[0], sizeof(unsigned int), datasize*L, fp);
    fread(shg.datahashtable[0], sizeof(int), L*datasize, fp);
    fread(hashkeyindex[0][0], sizeof(int), L * Alter * bucketnum, fp);
    fread(hashkeylength[0][0], sizeof(int), L * Alter * bucketnum, fp);
    fclose(fp);
}

/**
 * Load query_file and initialize queryid to zeroes
 * @param query_file
 */
void SHIndex::query_load(string query_file)
{
    io.diskread_float(query_file.c_str(), query[0], querysize*D);
    for(int i = 0; i < datasize; i++)
    {
        queryid[i] = -1;
    }

    // Number of points checked for each query
    st.sumcheck = 0;
}

/**
 * Execute the query
 * @param Lused The number of hashtables used for a query
 */
void SHIndex::query_execute(int Lused)
{
    for(int i = 0; i < querysize; i++)
    {
        normalknn(query[i], queryresult[i], i, Lused);
        // entropyknn(query[i], queryresult[i], i, Lused);
        // multiprobeknn(query[i], queryresult[i], i, Lused);
        //cout << "doing query id: " << i << " " << st.sumcheck << endl;
    }
}

/**
 * Write the result file to disk
 * @param result_file the name of the result file
 */
void SHIndex::result_write(string result_file)
{
    io.diskwrite_int(result_file.c_str(), queryresult[0], querysize*K);
}

/**
 * Compute the knn neighbours of a given point
 * @param querypoint            The querypoint
 * @param result[]              The vector of results
 * @param id                    Query ID
 * @param Lused                 The number of hash tables to use
 */
void SHIndex::normalknn(float querypoint[], int result[], int id, int Lused)
{
    float queryproduct[familysize];

    // Compute the product of query with the familyvector
    for(int i = 0; i < familysize; i++) queryproduct[i] = MyVector::dotproduct(D, querypoint, shg.familyvector[i]);

    // Compute the hashresults for every possible value of ratio
    for(int i = 0; i < Alter; i++) shg.tableindex(queryproduct, i, querytableresult[i]);

    // Initialize the knn structure
    knn.init();

    int hashkey, bucketindex, bucketlength, tocheck;
    for(int n = 0; n < Alter; n++)
    {
        // Like LSH, we break when we reach a certain level of sqrtbound
        if(knn.sqrtbound < ETRatio * shg.R[n]) break;

        // We check only 'Lused' number of hashtables for results
        for(int i = 0; i < Lused; i++)
        {
            /** Compute the bucketindex of the querypoint and add the
              elements of that bucket to the knn structure */
            hashkey = querytableresult[n][i] % bucketnum;
            bucketindex = hashkeyindex[i][n][hashkey];
            bucketlength = hashkeylength[i][n][hashkey];
            for(int j = 0; j < bucketlength; j++)
            {
                /* Increment the number of points that we have checked */
                st.sumcheck++;

                /* tocheck denotes the point for which we want to test knn membership.
                   Obviously if the hashvalues don't match, we exit */
                tocheck = shg.datahashtable[i][bucketindex + j];
                if (shg.datahashresult[tocheck][i] != querytableresult[n][i]) continue;

                /* Check if the point has been tested before, if not mark it as tested */
                if (queryid[tocheck] == id) continue;
                queryid[tocheck] = id;

                /* Delegate knn computation to knn module */
                knn.addvertex(data, tocheck, querypoint);
            }
        }
    }

    // Copy all the results from the knn structure to result
    for(int i = 0; i < K; i++) result[i] = knn.knnlist[i];
}

/**
 * Compute the knn neighbours of a given point using the idea of LSH entropy
 * @param querypoint            The querypoint
 * @param result[]              The vector of results
 * @param id                    Query ID
 * @param Lused                 The number of hash tables to use
 */
void SHIndex::entropyknn(float querypoint[], int result[], int id, int Lused)
{
    // eknn maintains the top-k vectors for the final result
    Knn eknn;
    eknn.init();

    // First perform knn for the qerypoint and get the results
    int temp_result[K];
    normalknn(querypoint, temp_result, id, Lused);
    for (auto tocheck: temp_result) {
        eknn.addvertex(data, tocheck, querypoint);
    }

    float perturbation[D];
    float newquery[D];
    for (int i = 1; i < Eperturbations; ++i) {
        MyRandom::rand_multi_gaussian(perturbation, D, BaseR);
        MyVector::sum(D, querypoint, perturbation, newquery);

        /* Perform knn with the perturbed point and check for knn */
        normalknn(newquery, temp_result, id, Lused);
        for (auto tocheck: temp_result) {
            eknn.addvertex(data, tocheck, querypoint);
        }
    }

    // Finally, we copy the top-k vectors to the original result list
    for(int i = 0; i < K; i++) result[i] = eknn.knnlist[i];
}

/**
 * Compute the knn neighbours of a given point using the idea of multiprobe LSH
 * @param querypoint            The querypoint
 * @param result[]              The vector of results
 * @param id                    Query ID
 * @param Lused                 The number of hash tables to use
 */
void SHIndex::multiprobeknn(float querypoint[], int result[], int id, int Lused)
{
    float queryproduct[familysize];

    // Compute the product of query with the familyvector
    for(int i = 0; i < familysize; i++) queryproduct[i] = MyVector::dotproduct(D, querypoint, shg.familyvector[i]);

    // Compute the hashresults for every possible value of ratio
    for(int i = 0; i < Alter; i++) shg.tableindex(queryproduct, i, querytableresult[i]);

    // Initialize the knn structure
    knn.init();

    int hashkey, bucketindex, bucketlength, tocheck;
    for(int n = 0; n < Alter; n++)
    {
        // Like LSH, we break when we reach a certain level of sqrtbound
        if(knn.sqrtbound < ETRatio * shg.R[n]) break;

        // We check only 'Lused' number of hashtables for results
        for(int i = 0; i < Lused; i++)
        {
            /** Compute the bucketindex of the querypoint and add the
              elements of that bucket to the knn structure */
            hashkey = querytableresult[n][i] % bucketnum;
            bucketindex = hashkeyindex[i][n][hashkey];
            bucketlength = hashkeylength[i][n][hashkey];

            for(int j = 0; j < bucketlength; j++)
            {
                /* Increment the number of points that we have checked */
                st.sumcheck++;

                /* tocheck denotes the point for which we want to test knn membership.
                   Obviously if the hashvalues don't match, we exit */
                tocheck = shg.datahashtable[i][bucketindex + j];
                if (shg.datahashresult[tocheck][i] != querytableresult[n][i]) continue;

                /* Check if the point has been tested before, if not mark it as tested */
                if (queryid[tocheck] == id) continue;
                queryid[tocheck] = id;

                /* Delegate knn computation to knn module */
                knn.addvertex(data, tocheck, querypoint);
            }

            /** Now search the next bucket for the results */
            while (hashkey < bucketnum && hashkeyindex[i][n][hashkey] == bucketindex + 1) {
                hashkey++;
            }

            // In this case there is no next bucket to find
            if (hashkey != bucketnum) {
                bucketindex = hashkeyindex[i][n][hashkey];
                bucketlength = hashkeylength[i][n][bucketlength];
                for(int j = 0; j < bucketlength; j++)
                {
                    /* Increment the number of points that we have checked */
                    st.sumcheck++;

                    /* tocheck denotes the point for which we want to test knn membership.
                       Obviously if the hashvalues don't match, we exit */
                    tocheck = shg.datahashtable[i][bucketindex + j];
                    if (shg.datahashresult[tocheck][i] != querytableresult[n][i]) continue;

                    /* Check if the point has been tested before, if not mark it as tested */
                    if (queryid[tocheck] == id) continue;
                    queryid[tocheck] = id;

                    /* Delegate knn computation to knn module */
                    knn.addvertex(data, tocheck, querypoint);
                }
            }

            /** Now search the previous bucket for the results */
            while (hashkey >= 0 && hashkeyindex[i][n][hashkey] == bucketindex - 1) {
                hashkey--;
            }

            // No previous bucket in this case
            if (hashkey != -1) {
                bucketindex = hashkeyindex[i][n][hashkey];
                bucketlength = hashkeylength[i][n][bucketlength];
                for(int j = 0; j < bucketlength; j++)
                {
                    /* Increment the number of points that we have checked */
                    st.sumcheck++;

                    /* tocheck denotes the point for which we want to test knn membership.
                       Obviously if the hashvalues don't match, we exit */
                    tocheck = shg.datahashtable[i][bucketindex + j];
                    if (shg.datahashresult[tocheck][i] != querytableresult[n][i]) continue;

                    /* Check if the point has been tested before, if not mark it as tested */
                    if (queryid[tocheck] == id) continue;
                    queryid[tocheck] = id;

                    /* Delegate knn computation to knn module */
                    knn.addvertex(data, tocheck, querypoint);
                }
            }
        }
    }

    // Finally, we copy the top-k vectors to the original result list
    for(int i = 0; i < K; i++) result[i] = knn.knnlist[i];
}
