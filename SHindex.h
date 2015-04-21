#ifndef SHINDEXING_H_INCLUDED
#define SHINDEXING_H_INCLUDED

class SHIndex;
#include "constants.h"
#include "data.h"
#include "myvector.h"
#include "myrandom.h"
#include <string>

using namespace std;

struct datakey
{
    int dataid;
    int key;
};

class SHIndex{
    protected:
        //for index
        int hashkeyindex[L][Alter][bucketnum];           /**< The bucketindex of maps to this (hashtable, layer, hashkey) */
        int hashkeylength[L][Alter][bucketnum];          /**< # of points that map to this (hashtable, layer, hashkey) */
        datakey temptable[datasize];                     /**< Helps create a priority queue */

        //for query
        float queryproduct[familysize];                  /**< Product of query and familyvectors */
        unsigned int querytableresult[Alter][L];         /**< Hashresults for each (layer, hashtable) */
        int queryid[datasize];                           /**< Indicates if a datapoint has been tested in a query */
        float query[querysize][D];                       /**< The query set */
        int queryresult[querysize][K];                   /**< Query Results */

    public:
        void index_construct(string decision_file);
        void index_write(string index_file);
        void index_load(string index_file);
        void query_load(string query_file);
        void query_execute(int);
        void result_write(string result_file);

    private:
        void normalknn(float [], int [], int,int);
        void entropyknn(float [], int [], int,int);
        void multiprobeknn(float [], int [], int,int);
};

#endif // SHINDEXING_H_INCLUDED
