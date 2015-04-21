#include "myrandom.h"
#include <iostream>
#include <random>
using namespace std;

/**
 * Generate a random num from 0 to max-1 (max may be any unsigned int)
 */
int MyRandom::int_random(int max)
{
    long long int longresult;
    int i = 1;
    longresult = rand();
    while (longresult < 16 * max)
    {
        i = 16 * i;
        longresult = 16 * longresult + rand();
    }
    int result = longresult%max;
    return result;
}

/**
 * Generate a simple multi gaussian distribution n dimensions variance matrix is variance * I
 * @param variance          variance
 */
void MyRandom::rand_multi_gaussian(float array[], int n, float variance)
{
    for (int i = 0; i < n; i++) array[i] = rand_single_gaussian(variance);
}

/**
 * Generate a random variable follows Gaussian distribution where mean = 0 varaince = variance
 * @param variance      The required variance
 */
float MyRandom::rand_single_gaussian(float variance)
{
    std::random_device rd;
    std::default_random_engine generator;
    generator.seed( rd() );
    std::normal_distribution<double> distribution(0, variance);
    return distribution(generator);
}
