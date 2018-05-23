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
//       35XY.lv1       = grid event report
//
// OUTPUT 
//       35XY.cor       = grid event report
//
//
// FILE HISTORY
//       First release: V1.0 16/Mar/2005
//
//       Last Release: V0.2 -  24/feb/2006
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
const char * packet = "35XY";



int Open35XY(int argc, char ** argv, fitsfile ** input, fitsfile ** output, FILE *fp);
int CorrTime(fitsfile * input, fitsfile * output, FILE *fp) ;
int WriteHeader(fitsfile * output);
int Close35XY(int argc, char **  argv, fitsfile * input, fitsfile * output, FILE *fp);

