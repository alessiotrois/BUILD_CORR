////////////////////////////////////////////////////////////////////////////////
// DESCRIPTION
//       correction pipeline I/O routine
//       CREATE THE LIST OF TELECOMMANDS REPORT
//       Release: V0.1 24/feb/2006
//       Contributors: A.A., A.G., A.P., S.V.
//       Author: Alessio Trois (IASF-Milano)
//
//
// INPUT 
//      *3301.lv1       = successful command acceptance report
//      *3302.lv1       = unsuccessful command acceptance report
//      *3303.lv1       = successful command start execution report
//      *3304.lv1       = unsuccessful command start execution report
//      *3307.lv1       = successful command execution report
//      *3308.lv1       = unsuccessful command execution report
//
// OUTPUT 
//       3300.cor       = list of telecommands report for a contact
//
// FILE HISTORY
//       First release: V0.0 16/Mar/2005
//       Last release: V0.1 24/feb/2006                     
//                      Authors:       Alessio Trois (IASF-Milano)
//
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
#include <iostream>
#include <ctype.h>
#include <cstring>
#include "CorUtilities.h"
#include "pil.h"

using namespace std;

const char * version = "version 0.2";
const char * packet = "3300";

int Open3300(int argc, char ** argv, fitsfile ** input, fitsfile ** output, FILE *fp);
int CorrAcp(fitsfile * input, fitsfile * output, FILE *fp);
int CreateTable3300(fitsfile * input, fitsfile * output, FILE *fp);
int WriteHeaderCor3300(fitsfile * input,fitsfile * output);
int Close3300(int argc, char **argv, fitsfile * input, fitsfile * output, int incount, FILE *fp);

