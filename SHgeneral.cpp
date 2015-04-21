#include "SHgeneral.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <memory.h>

using namespace std;

SHGeneral::SHGeneral()
{
    cout<<"SHGeneral::SHGeneral"<<endl;
    memset(dataproduct, 0, sizeof(dataproduct));
    memset(familyvector, 0, sizeof(familyvector));
    memset(hashtableindex, 0, sizeof(hashtableindex));
    memset(datahashtable, 0, sizeof(datahashtable));
    memset(datahashresult, 0, sizeof(datahashresult));
    memset(decision, 0, sizeof(decision));
    R[0] = BaseR;
    for(int i = 1; i < Alter; i++) R[i] = c * R[i-1];
    isinit = false;
    decisionavailable = false;
}

void SHGeneral::init()
{
	if(isinit) return;
    isinit = true;
	cout<<"SHGeneral::init"<<endl;
	family_generator();                     // Generate random familtyvectors
	generate_hashtableindex();              // Create the concatenation functions by sampling familyvectors
	productcomputer();                      // Compute product of data and familyvectors
	cout<<"SHGeneral::init--END"<<endl;
}

/**
  * Intitalizes each famliyvector with a random multi gaussian
  */
void SHGeneral::family_generator()
{
    cout<<"SHGeneral::family_generator"<<endl;
    for(int i = 0; i < familysize; i++)
    {
        // We need a rand_multi_gaussian with variance matric Identity
        MyRandom::rand_multi_gaussian(familyvector[i], D+1, 1);
        for (int j = 0; j < D; j++) familyvector[i][j] =  familyvector[i][j]/sqrt(D);
        //for test
        //float sum = 0;
        //for (int j = 0; j < D; j++)sum+= familyvector[i][j]*familyvector[i][j];
        //cout<<"gaussian"<<sum<<endl;
    }
    cout<<"SHGeneral::family_generator--END"<<endl;
}

/**
  * Each hashtable is a concatenation function of 'M' random hash family functions,
  * this function samples 'M' random familyvectors and stores the indexes of those
  * functions in each hashtableindex
  */
void SHGeneral::generate_hashtableindex()
{
     cout<<"SHGeneral::generate_hashtableindex"<<endl;
     for(int i = 0; i < L; i++)
     {
             familysample(hashtableindex[i], familysize, M);
     }
     cout<<"SHGeneral::generate_hashtableindex--END"<<endl;
}

/**
  * Sample needsize points from size and fill it in result
  * @param result       the array to hold the sample
  * @param size         the total size of population
  * @param needsize     sample size
  */
void SHGeneral::familysample(int result[], int size, int needsize)
{
     vector<int> forchoose;
     forchoose.clear();
     for (int i = 0; i < size; i++)
		forchoose.push_back(i);
     int forswap, temp;
	 for (int i = 0; i < needsize; i++)
	 {
		forswap = MyRandom::int_random(size - i) + i;
		temp = forchoose[i];
		forchoose[i] = forchoose[forswap];
		forchoose[forswap] = temp;
		result[i] = forchoose[i];
	 }
	 forchoose.clear();
}

/**
  * Compute dotproduct of data with familyvector
  */
void SHGeneral::productcomputer()
{
    cout<<"SHGeneral::productcomputer"<<endl;
    for(int i = 0; i < datasize; i++)
    {
        for(int j = 0; j < familysize; j++)
        {
            dataproduct[i][j] = MyVector::dotproduct(D, data[i], familyvector[j]);
        }

        if(i%20000 == 0)
        {
            cout << "computing product for " << i << endl;
            // cout << dataproduct[i][5] << endl;
        }

    }
    cout<<"SHGeneral::productcomputer--END"<<endl;
}

/**
  * Computes the hashvalue for each hashtables for a given data point
  * @param product  dotproduct of data with all the familyvectors
  * @param Rrank    The index in R of the radius ratio to use
  * @param table    Hashresults of data with all 'L' hashtables
  */
void SHGeneral::tableindex(float product[], int Rrank, unsigned int table[])
{
    int familyint[familysize] = {};

    // Choose the Radius Ratio according to passed index
    float ratio = R[Rrank];

    /* Apply the linear classifier corresponding to each function.
       Linear classifier f() = w.x + w0
       product = w.x
       familyvector[i][D] = w0

       The hash function is a p-stable distribution hash function with
       the ratio indicating the slot/grid width. Larger the ratio, larger
       the slot width.
       */
    for(int i = 0; i < familysize; i++)
    {
        float temp = product[i];
        temp /=  ratio;                         // Standard method for LSH
        temp += familyvector[i][D];

        // Cast to an integer
        familyint[i] = (int)temp;
    }

    /* Now we have to compute the value of the hash function for each of
       the concatenative functions. Now, we know what 'M' random hashes
       consititute each concatenative function which is stored in hashtableindex.
       The rest is simple computation */
    for(int l = 0; l < L; l++)
    {
        table[l] = 0;
        for(int i = 0; i < M; i++)
        {
            // Start way to concatenate hash family
            table[l] ^= familyint[hashtableindex[l][i]] + 0x9e3779b9 + (table[l] << 6) + (table[l] >> 2);
        }
    }
}
