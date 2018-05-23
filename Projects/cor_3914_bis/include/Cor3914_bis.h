////////////////////////////////////////////////////////////////////////////////
// DESCRIPTION
//       correction pipeline I/O routine
//       Star sensor packet
//       Release: V0.2 -  24/feb/2006
//       Contributors: A.A., A.G., A.P., S.V., S.M.
//       Author: Alessio Trois (IASF-Milano)
//
//
// INPUT
//       3914.lv1       = grid event report
//
// OUTPUT 
//       3914.cor       = grid event report
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

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "fitsio.h"
#include <unistd.h>
#include <ctype.h>
#include <cstring>
#include <iostream>
#include "CorUtilities.h"
#include "alikeQuat.h"
#include "pil.h"

using namespace std;
const char * version = "version 0.0 BUILD ? 07/04/2011";
const char * packet = "3914";


int Open3914(int argc, char ** argv, fitsfile ** input, fitsfile ** output, FILE *fp);
int CorrTime(fitsfile output, FILE *fp) ;
int CorrAtt(fitsfile * input, FILE *fp);
int WriteHeader(fitsfile * output);
int Close3914(int argc, char ** argv,  fitsfile * input, fitsfile * output, FILE *fp);

