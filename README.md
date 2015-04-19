General Information
-------------------

Here is our latest best implementation of high-dimensional k-nn search.
The algorithm continuously improves and may differ from what has been described in paper.
Please visit our website frequently to get our latest version.

If you have any question, pls contact Jinyang.gao@comp.nus.edu.sg for discussion.
The latest version is V2014.9.1.

Overview
--------

**Hash family**
- We generate a hash family, wherein each hash function is a linear separator.
- These linear separators are stored as *famliyvectors* which are D+1 dimensional.

**Concatenation function**
- The hash family is weak and hence we use concatenation to generate stronger hash functions.
- We finally use 'L' concatenation functions which have 'M' random functions from the hash family each.
- Concatenation function is used synonymously with **hashtable**.

**Rrank**
- This indicates the optimum grid size of each point.
- DSH allows the grid sizes to be variable and this is enabled by Rrank.

**Computing Hash of a new point**
- We get the hashresults of pre-computed data points in *datahashresult*.
- The *hashkey* equals *datahashresult % bucketnum*.
- Now we get the *bucketindex* as *hashkeyindex[][][hashkey]*
- The *bucketlength* is *hashkeylength[][][hashkey]*
- To get the points of the *bucket* iterate over *datahashresult[Lused][bucketindex +j]*

Parameter settings
------------------

In current version, there following parameters are required to be set by users:

- **Datasize**:  the number of data points.
- **Querysize**: the number of queries.
- **D**:  the number of dimensions of the data.
- **K**:  the top-k results that are expected to return.
- **R**:  the ratio distance, depends on the k-nn distance of your dataset
- **L**:  the number of hash tables, this parameter is expected to be set 40 ¨C 200 to get a good performance. Usually, a larger value will improve the efficiency. However, it will induce more memory cost.
- **M**: the number of concatenation functions, this value is supposed to be set between 10 and 20. M should be set larger if: a) the hash function is not balanced b) a larger L is chosen c) efficiency is considered more than recall.
- **Bucketnum**: in our implementation, we do not build a universal hashing function. Instead, we rehash the data directly into Bucketnum buckets. Usually, Bucketnum should be set to a prime between 10K and 20K. However, when the datasize is large, or M is large, then Bucketnum should be set to a larger value. The collisions will introduce little cost, as there is a check procedure about the exact hashkey.

There are also some other parameters. However, we do *NOT* recommend you to change it unless you fully understand the code.

- **Familysize**: the number of hashing functions used in all. We do not randomly generate M hash functions for each hash table, instead we generate a set of functions for all. Thus the hash functions may not be completely random. However, this will reduce the cost of hashing procedure in both index and query part. To make sure that the hashing functions are random enough, familysize should be set at least more than the number of dimensions of the dataset. In our implementation, it is set to 200.
- **thresholdpoint** : to be edited in next version
- **thresholdtable** : to be edited in next version
- **Alter**: to be edited in next version
- **RI**: to be edited in next version

Data Structures
--------------

## SHgeneral.h

**familyvector**
- The hash functions (which are linear classifiers).

**hashtableindex**
- The *M* random hash functions corresponding to each hashtable or concatenation function.

**decision**
- The optimum radius ratio for a give point *i*.

**datahashresult**
- Given the point and *hashtable*, this returns the *hashvalue*.
- Note: The *hashkey* is (datahashtable[L][i] % bucketnum).

**datahashtable**
- Stores the points in an order for ease of traversal using bucketindex and bucketlength.

## SHindex.h
**hashkeyindex and hashkeylength**
- The bucketindex and bucketlength for a data point.

**querytableresult**
- Hashvalues for the query for each setting of ratio and for each hashtable.

**queryid**
-

Running procedure
-----------------

## Input & Output:
We use binary array i/o in our implementation. Those arrays are read/written in the following way:
fread(array, sizeof(double), size, fp);

There are 3 arrays that are needed:

- Data array: should be stored in data.dat in the same folder.
- Query array: should be stored in query.dat in the same folder.
- Groundtruth array: should be stored in groundtruth.dat in the same folder (you can also remove the static module, this is only for performance measurement).

## How to Run:
First, run the pre-processing to get the decision.dat. Then run the main program.
The main program contains index module, query module and statistic module. After the indexing module the whole index will be build. Then you can run any other module separately. However, if you want to change the parameter settings, you have to rebuild the index. Of course, you can edit the code yourself to make your own uses. We regret that this version do not support incremental maintenance. A more scalable version will be updated in 2014.6.

Results
-------

The results are stored in results.dat. It is also outputted as a binary array.



