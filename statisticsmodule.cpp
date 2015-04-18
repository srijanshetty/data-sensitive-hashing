#include "statisticsmodule.h"
#include <iostream>
#include <cmath>
#include <algorithm>

void StatisticsModule::stat_output(string query_file,string groundtruth_file,string result_file,string output_file,int Lused)
{
	ofstream fout;
    fout.open(output_file.c_str(), ios::out | ios::app);
    if(!fout)
	{
	    cout << "Cannot open file!" << endl;;
		exit(1);
	}
    //fout<<"time used: "<<finish_ - start_<<endl;
    //cout<<"time used: "<<finish_ - start_<<endl;
    io.diskread_float(query_file.c_str(), query[0], querysize*D);
    io.diskread_int(groundtruth_file.c_str(), groundtruth[0], querysize*K);
    io.diskread_int(result_file.c_str(), result[0], querysize*K);

    float sumerrorrate = 0;
    float aveerrorate;
    int sumrecall = 0;
    float testdist[K],gtdist[K];
    for(int i = 0; i < querysize; i++)
    {
		 for(int j = 0; j < K; j++)
		 {
			 testdist[j] = MyVector::distancel2sq(D,data[result[i][j]],query[i],100);
			 testdist[j] = sqrt(testdist[j]);
			 gtdist[j] = MyVector::distancel2sq(D,data[groundtruth[i][j]],query[i],100);
			 gtdist[j] = sqrt(gtdist[j]);
		 }
		 sort(testdist,testdist+K);
		 sort(gtdist,gtdist+K);
		 for(int j = 0; j < K; j++)
		 {
		     if(testdist[j] <= gtdist[K-1] + 0.0001)sumrecall++;
			 if(testdist[j] <= gtdist[j] + 0.0001) continue;
			 double temp = (testdist[j] - gtdist[j])/ gtdist[j];
			 if(temp > 4) temp = 4;
			 if(temp < -0.01) cout<<"error perform better than optimal!"<<endl;
			 sumerrorrate += temp;
		 }
	}
	float recall = (float)sumrecall/ (float)(querysize*K);
    aveerrorate = sumerrorrate / (querysize*K);
    //fout<<"recall is "<<recall<<endl;
    //cout<<"recall is "<<recall<<endl;
    //fout<<"error rate is "<<aveerrorate<<endl;
    //cout<<"error rate is "<<aveerrorate<<endl;
    float tempfl = (float)sumcheck;
    tempfl = tempfl*50/((float)(querysize)*datasize);
    //fout<<"check rate is (%) "<<tempfl<<endl;
    //cout<<"check rate is (%) "<<tempfl<<endl;
    fout<<Lused<<" "<<finish_ - start_<<" ";
    fout<<recall<<" "<<aveerrorate<<" "<<tempfl<<endl;
    cout<<Lused<<" "<<finish_ - start_<<" ";
    cout<<recall<<" "<<aveerrorate<<" "<<tempfl<<endl;
	fout.close();
}

void StatisticsModule::gen_query_and_groundtruth(string query_file, string groundtruth_file)
{
    sample_query();
    cout<<"query sampled"<<endl;
    io.diskwrite_float(query_file.c_str(), query[0], querysize*D);
    cout<<"query write to disk"<<endl;
    batch_linear_scan();
    cout<<"linear scan finished"<<endl;
    io.diskwrite_int(groundtruth_file.c_str(), groundtruth[0], querysize*K);
    cout<<"groundtruth write to disk"<<endl;
}

//sample querysize query points from data and store in query
void StatisticsModule::sample_query()
{
    for(int i = 0; i < querysize; i++)
    {
        int temp = MyRandom::int_random(datasize);
        for(int j = 0; j < D; j++) query[i][j] = data[temp][j];
    }
    return;
}

//do batch check for all points
void StatisticsModule::batch_linear_scan()
{
    for(int i = 0; i < querysize; i++)
    {
        knn.linear_scan(data, query[i]);
        for(int j = 0; j < K; j++) groundtruth[i][j] = knn.knnlist[j];
        cout<<i<<"   "<<knn.sqrtbound<<endl;
    }
    return;
}
