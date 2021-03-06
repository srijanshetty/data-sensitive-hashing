#include "io.h"
#include <iostream>
#include <fstream>

using namespace std;

/**
 * Read an array of float from file
 * @param filename
 * @param array
 * @param size
 */
void IO::diskread_float(string filename, float array[], int size)
{
    FILE *fp;
    fp = fopen(filename.c_str(),"rb");
    if(fp == NULL)
    {
        cout << "Cannot open file!" << endl;;
        exit(1);
    }
    fread(array, sizeof(float), size, fp);
    fclose(fp);
}

/**
 * Write an array of float to file
 * @param filename
 * @param array
 * @param size
 */
void IO::diskwrite_float(string filename, float array[], int size)
{
    FILE *fp;
    fp = fopen(filename.c_str(),"wb");
    if(fp == NULL)
    {
        cout << "Cannot open file!" << endl;;
        exit(1);
    }
    fwrite(array, sizeof(float), size, fp);
    fclose(fp);
}

/**
 * Read an array of int from file
 * @param filename
 * @param array
 * @param size
 */
void IO::diskread_int(string filename, int array[], int size)
{
    FILE *fp;
    fp = fopen(filename.c_str(),"rb");
    if(fp == NULL)
    {
        cout << "Cannot open file!" << endl;;
        exit(1);
    }
    fread(array, sizeof(int), size, fp);
    fclose(fp);
}

/**
 * Write an array of int to file
 * @param filename
 * @param array
 * @param size
 */
void IO::diskwrite_int(string filename, int array[], int size)
{
    FILE *fp;
    fp = fopen(filename.c_str(),"wb");
    if(fp == NULL)
    {
        cout << "Cannot open file!" << endl;;
        exit(1);
    }
    fwrite(array, sizeof(int), size, fp);
    fclose(fp);
}
