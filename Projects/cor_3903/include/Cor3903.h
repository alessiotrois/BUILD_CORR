////////////////////////////////////////////////////////////////////////////////
// DESCRIPTION
//       correction pipeline I/O routine
//       Pedestal report
//       Release: V0.0 -  24/feb/2006
//       Contributors: A.A., A.G., A.P., S.V.
//       Author: Alessio Trois (IASF-Milano)
//
//
// INPUT 
//       3903.lv1       = grid event report
//
// OUTPUT 
//       3903.cor       = grid event report
//
//
// FILE HISTORY
//       First release: V0.0 24/feb/2006                
//       Last release: V0.0 24/feb/2006
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
#include <fcntl.h>
#include <math.h>
#include "fitsio.h"
#include <unistd.h>
#include <ctype.h>
#include <cstring>
#include "CorUtilities.h"
#include "pil.h"

using namespace std;
const char * version = "version 0.1";
const char * packet = "3903";


int Open3903(int argc, char ** argv, fitsfile ** input, fitsfile ** output, FILE *fp);
int CorrAtt(fitsfile * input, fitsfile * output, FILE *fp);
int WriteHeader(fitsfile * output);
int Close3903(int argc, char ** argv, fitsfile * input, fitsfile * output, FILE *fp);

