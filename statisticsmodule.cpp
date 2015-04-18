#include "statisticsmodule.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iterator>

/**
 * Compare groundtruth and the computed result
 * @param query_file
 * @param groundtruth_file
 * @param output_file
 * @param Lused                Number of hashtables used to compute the result
 */
void StatisticsModule::stat_output(string query_file, string groundtruth_file, string result_file, string output_file, int Lused)
{
    ofstream fout;
    fout.open(output_file.c_str(), ios::out | ios::app);
    if(!fout)
    {
        cout << "Cannot open file!" << endl;;
        exit(1);
    }

    io.diskread_float(query_file.c_str(), query[0], querysize*D);
    io.diskread_int(groundtruth_file.c_str(), groundtruth[0], querysize*K);
    io.diskread_int(result_file.c_str(), result[0], querysize*K);

    float sumerrorrate = 0;
    float aveerrorate;
    int sumrecall = 0;
    float testdist[K], gtdist[K];

    // Compute statistics for each query
    for(int i = 0; i < querysize; i++)
    {
        // Compute the distances of querypoint for groundtruth and computed result
        for(int j = 0; j < K; j++)
        {
            testdist[j] = MyVector::distancel2sq(D, data[result[i][j]], query[i], 100);
            testdist[j] = sqrt(testdist[j]);
            gtdist[j] = MyVector::distancel2sq(D, data[groundtruth[i][j]], query[i], 100);
            gtdist[j] = sqrt(gtdist[j]);
        }

        // Sort both the vectors for comparison
        sort(testdist, testdist + K);
        sort(gtdist, gtdist + K);

        // Print the distances
        cout << "Query: " << i << endl;
        copy(testdist, testdist + K, ostream_iterator<float>(cout, " "));
        cout << endl;
        copy(gtdist, gtdist + K, ostream_iterator<float>(cout, " "));
        cout << endl;

        for(int j = 0; j < K; j++)
        {
            // If any result vector is closer than the farthest groundtruth vector, we consider it a match
            if(testdist[j] <= gtdist[K-1] + 0.0001) sumrecall++;

            // No error is the testdist is within acceptable bounds
            if(testdist[j] <= gtdist[j] + 0.0001) continue;

            // Compute the error rate
            double temp = (testdist[j] - gtdist[j])/ gtdist[j];
            if(temp > 4) temp = 4;
            if(temp < -0.01) cout << "error perform better than optimal!" << endl;
            sumerrorrate += temp;
        }
    }

    // Compute overeall recall and error rate
    float recall = (float)sumrecall/ (float)(querysize * K);
    aveerrorate = sumerrorrate / (querysize * K);

    float tempfl = (float)sumcheck;
    tempfl = tempfl*50/((float)(querysize) * datasize);

    // Print statistics
    fout << Lused << " " << finish_ - start_ << " ";
    fout << recall << " " << aveerrorate << " " << tempfl << endl;
    cout << Lused << " " << finish_ - start_ << " ";
    cout << recall << " " << aveerrorate << " "<< tempfl << endl;

    fout.close();
}

/**
 * Generate querypoints by random sampling and also generate the groundtruth for the same
 */
void StatisticsModule::gen_query_and_groundtruth(string query_file, string groundtruth_file)
{
    sample_query();
    cout<<"query sampled"<<endl;
    io.diskwrite_float(query_file.c_str(), query[0], querysize * D);
    cout<<"query write to disk"<<endl;
    batch_linear_scan();
    cout<<"linear scan finished"<<endl;
    io.diskwrite_int(groundtruth_file.c_str(), groundtruth[0], querysize*K);
    cout<<"groundtruth write to disk"<<endl;
}

/**
 * Sample the dataset to generate querypoint
 */
void StatisticsModule::sample_query()
{
    for(int i = 0; i < querysize; i++)
    {
        int temp = MyRandom::int_random(datasize);
        for(int j = 0; j < D; j++) query[i][j] = data[temp][j];
    }
    return;
}

/**
 * Compute knn by a linear scan of the entire dataset
 */
void StatisticsModule::batch_linear_scan()
{
    for(int i = 0; i < querysize; i++)
    {
        knn.linear_scan(data, query[i]);
        for(int j = 0; j < K; j++) groundtruth[i][j] = knn.knnlist[j];
        cout << i << "   " << knn.sqrtbound << endl;
    }
    return;
}
