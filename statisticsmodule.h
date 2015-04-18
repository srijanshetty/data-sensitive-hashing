#ifndef STAT_H_INCLUDED
#define STAT_H_INCLUDED

class StatisticsModule;

#include "constants.h"
#include "data.h"
#include <string>
#include <fstream>

class StatisticsModule{
    public:
    void begin(){start_ = clock();};
    void finish(){finish_ = clock();};
    void stat_output(string query_file,string groundtruth_file,string result_file,string output_file,int);
    void gen_query_and_groundtruth(string query_file, string groundtruth_file);
    private:
    void sample_query();
    void batch_linear_scan();

    private:
    int start_,finish_;
    int result[querysize][K];
    int groundtruth[querysize][K];
    float query[querysize][D];

    public:
    int sumcheck;
};

#endif // STAT_H_INCLUDED
