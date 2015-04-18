#include "constants.h"
#include "io.h"
#define DATASOURCE "tests/temp.data"
#define DATATARGET "datads3.dat"
#define QUERYSOURCE "tests/exp_query.txt"
#define QUERYTARGET "query3.dat"

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>

float data[datasize][D];
float query[querysize][D];

IO io;

void create_datafile() {
    std::ifstream ifile(DATASOURCE);

    // Load the points from the file and dump to data.dat
    long long lineCount = 0;
    for (std::string line; std::getline(ifile, line); ++lineCount) {
        std::istringstream lineStream(line);

        for (int i = 0; i < D; ++i) {
            lineStream >> data[lineCount][i];
        }
    }

    ifile.close();

    // Write to disk
    io.diskwrite_float(DATATARGET, data[0], datasize * D);
}

void create_queryfile() {
    std::ifstream ifile(QUERYSOURCE);

    // Load the points from the file and dump to data.dat
    long long lineCount = 0;

    // Temp variables to read unwanted info
    int temp;

    for (std::string line; std::getline(ifile, line); ++lineCount) {
        std::istringstream lineStream(line);

        // Read the query number
        lineStream >> temp;

        // Read the query point
        for (int i = 0; i < D; ++i) {
            lineStream >> query[lineCount][i];
        }

        // Read the value of k
        lineStream >> temp;
    }

    ifile.close();

    // Write to disk
    io.diskwrite_float(QUERYTARGET, query[0], querysize * D);
}

int main() {
    create_datafile();
    // create_queryfile();
    return 0;
}
