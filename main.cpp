#include "statisticsmodule.h"
#include "SHindex.h"
#include "SHgeneral.h"
#include "SHselection.h"
#include "data.h"
#include <iostream>

SHGeneral shg;
SHSelection shs;
SHIndex shi;
StatisticsModule st;

float data[datasize][D];

IO io;
Knn knn;

int main()
{
	io.diskread_float("datads3.dat", data[0], datasize*D);
    cout<<"data read from disk"<<endl;
	st.gen_query_and_groundtruth("query3.dat","groundtruth3.dat");
	shg.init();
	shs.radius_selection("decision.dat");
    shi.index_construct("decision.dat");
    shi.index_write("index.dat");
    shi.index_load("index.dat");
    //int Ltemp[16] = {5,6,7,8,9,10,11,12,13,14,15,16,17,18,20,22,23,26,30,35,40,45,50};
    for(int i = 5; i <= 30; i++)
    {
    shi.query_load("query3.dat");
    st.begin();
    shi.query_execute(i);
    st.finish();
    shi.result_write("result.dat");
    st.stat_output("query3.dat","groundtruth3.dat","result.dat","statds3slshnew.txt",i);
    }
    cout<<"program finished"<<endl;
    int forcin; cin>>forcin;
	return 0;
}
