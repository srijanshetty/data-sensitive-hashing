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
    cout << "data read from disk" << endl;

    // Generate a randomized query and the groundtruth by a linear scan.
    st.gen_query_and_groundtruth("query3.dat","groundtruth3.dat");

    // Create the index and store to disk
    shg.init();
    shs.radius_selection("decision.dat");
    shi.index_construct("decision.dat");
    shi.index_write("index.dat");
    shi.index_load("index.dat");

    // We use different number of hashtables and check accuracy
    int Ltemp[] = {5, 8, 10, 12, 15, 17, 20, 23, 26, 30, 35, 40, 45, 50};
    // for(int Lused = 5; Lused <= 50; ++Lused)
    for(int Lused : Ltemp)
    {
        // Load the query to memory
        shi.query_load("query3.dat");

        // time the query
        st.begin();
        shi.query_execute(Lused);
        st.finish();

        // write out the results to disk
        shi.result_write("result.dat");

        // Write out the statistics
        st.stat_output("query3.dat", "groundtruth3.dat", "result.dat", "statds3slshnew.txt", Lused);
    }

    cout << "program finished" << endl;
    return 0;
}
