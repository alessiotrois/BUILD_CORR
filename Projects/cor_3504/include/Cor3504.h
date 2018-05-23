////////////////////////////////////////////////////////////////////////////////
// DESCRIPTION
//       correction pipeline I/O routine
//       GRID event report
//       Release: V0.0 -  16/mar/2005
//       Contributors: A.A., A.G., A.P., S.V.
//       Author: Alessio Trois (IASF-Milano)
//
//
// INPUT 
//       3504.lv1       = grid event report
//
// OUTPUT 
//       3504.cor       = grid event report
//
//
// FILE HISTORY
//       First release: V0.0 24/Jan/2005
//
//                      Authors:       Alessio Trois (IASF-Milano)
// NOTICE
//       Any information contained in this software
//       is property of the AGILE TEAM and is strictly
//       private and confidential.
//       All rights reserved.
////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <math.h>
#include "fitsio.h"
#include <unistd.h>
#include <ctype.h>
#include <cstring>
#include "CorUtilities.h"
#include "pil.h"
using namespace std;
const char * version = "version 0.0";
const char * packet = "3504";



int Open3504(int argc, char ** argv, fitsfile ** input, fitsfile ** output, FILE *fp);
int CorrTime(fitsfile * input, fitsfile * output, FILE *fp) ;
int WriteHeader(fitsfile * output);
int Close3504(int argc, char **  argv, fitsfile * input, fitsfile * output, FILE *fp);

