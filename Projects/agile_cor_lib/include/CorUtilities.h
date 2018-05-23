// Author   : Alessio Trois
// Version  :   25-Feb-16
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "fitsio.h"
#include <unistd.h>
#include <iostream>
#include <ctype.h>
#include <wcstrig.h>
#include <wcsmath.h>

using namespace std;
int CorrTimeEVT(fitsfile * output, FILE *fp);
int CorrTimePKT(fitsfile * output, FILE *fp);

int CorrTimeEVT3904(fitsfile * output, FILE *fp);
int CorrTimeEVT3908(fitsfile * output, FILE *fp);
int CorrTimeEVT3914(fitsfile * output, FILE *fp);
int CorrTimeEVT3916(fitsfile * output, FILE *fp);


int CorrTimePKT3908(fitsfile * output, FILE *fp);
int CorrTimePKT3911(fitsfile * output, FILE *fp);
int euler(double ai, double bi, double * ao, double * bo, int select);
float * ProdQuat(float *Q_1, float *Q_2);
float * ConQuat(float *Q_1);
int max(double A[], int n);
int min(double A[], int n);
int maxint(int A[], int n);
int minint(int A[], int n);
int binarySearch(double * vect, int from, int to, double v, float diff);
void carxradec(float * Xp, float * dec, float * ra);
int addfile(fitsfile *iFile, char * fileList, char * expr, double tmin, double tmax);
int addfile3914(fitsfile *iFile, char * fileList, char * expr, double tmin, double tmax, char * pkt);
int WriteTime(fitsfile * input, double t1, double t2);
int addfile3201(fitsfile *iFile, char * fileList, char * expr, double tmin, double tmax, char * pkt);
int checkfile(char * fileList, char * filename, double tmin, double tmax, char * packet);
void getError(int status);
void fpgetError(FILE * fp, int status);
int addfileFLG(fitsfile *iFile, char * fileList, char * expr, char * pkt);
int orderFileIndex(char * fileList);
int addfile3916(fitsfile *iFile, char * fileList, char * expr, double tmin, double tmax, char * pkt);
int isfile (const char *filename);
char * filetemp(char * name);
int CorrTimePKTSAEVT(fitsfile * output, FILE *fp);	
int CorrTimePKTSA(fitsfile * output, FILE *fp);


struct lineLineStruct{
char * name;
char * type;
double tempo1;
double tempo2;
};




// struct lineLineStruct{
// char name[200];
// char type[200];
// double tempo1;
// double tempo2;
// };














