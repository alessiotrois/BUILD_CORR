////////////////////////////////////////////////////////////////////////////////
// DESCRIPTION
//       correction pipeline I/O routine
//       GRID event report
//       Release: V1.0 -  24/feb/2006
//       Contributors: A.A., A.G., A.P., S.V.
//       Author: Alessio Trois (IASF-Milano)
//
//
// INPUT 
//       3901.lv1       = grid event report
//
// OUTPUT 
//       3901.cor       = grid event report
//
//
// FILE HISTORY
//       First release: V0.0 16/Mar/2005                
//       Last release: V0.1 24/feb/2006
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
const char * version = "version 0.0";
const char * packet = "3911";

