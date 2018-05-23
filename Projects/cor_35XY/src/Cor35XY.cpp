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


#include "Cor35XY.h"
using namespace std;

int Open35XY(int argc, char ** argv, fitsfile ** input, fitsfile ** output, FILE *fp) {

	int status=0, pkttype=0, pktstype=0;
	char * infile = new char[FLEN_FILENAME];
	char * outfile = new char[FLEN_FILENAME];
	int numpar=0;
	
	
	const time_t tp = time(NULL);
	char *data=ctime(&tp);

	fprintf(fp,"\n\nSTART EXECUTION:  %s",data);		
	fprintf(fp, "####################################################\n");
	fprintf(fp, "#########  Task Cor35XY......... starting  #########\n");
	fprintf(fp, "Cor35XY..................... starting Open35XY\n");

	
	printf("\n\nSTART EXECUTION:  %s",data);	
	printf("####################################################\n");
	printf("#########  Task Cor35XY......... starting  #########\n");
	printf("#########         %s         #########\n", version);
	printf("####################################################\n\n\n");
	printf("Cor35XY..................... starting Open35XY\n");

	status = PILInit(argc,argv);
	PILGetNumParameters(&numpar);
	
	if ( status != 0 || numpar!=2)
		return 1002;
		
	PILGetString("infile",infile);
	PILGetString("outfile",outfile);

	
	if (strcmp(infile,outfile)==0) {
		status = 101;
		return status;		
		}

	printf("..........opening input file: %s\n",infile);
	fprintf(fp,"..........opening input file: %s\n",infile);		
	fits_open_file(input, infile, READONLY, &status);
	if (status) {
		return status;
		}


	fits_read_key(input[0], TINT, "PKTTYPE", &pkttype, NULL, &status );
	fits_read_key(input[0], TINT, "PKTSTYPE", &pktstype, NULL, &status );
	
	if (pkttype!=35 || pktstype!=0) {
		printf("error packet type [35]   %d!!\n",pkttype );
		printf("error packet subtype [0] %d!!\n",pktstype );
		fits_close_file(input[0], &status);
		return 1001;
		}

	printf("..........opening output file: %s\n",outfile);
	fprintf(fp,"..........opening output file: %s\n",outfile);		
	fits_create_file(output, outfile, &status);
	if (status) {
		return status;
		}		
		
	PILClose(status);
	
	delete []infile;
	delete []outfile;		
		
	if ( status) {
		fprintf(fp, "Cor35XY..................... exiting Open35XY ERROR: %d", status);
		printf("Cor35XY..................... exiting Open35XY ERROR: %d", status);		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);	
		printf("\n");
		return status;			
		}			
	else {
		fprintf(fp, "Cor35XY..................... exiting Open35XY STATUS = 0\n\n");
		printf("Cor35XY..................... exiting Open35XY STATUS = 0\n\n");		
		}
	return status;	
	}


int CorrTime(fitsfile * input, fitsfile * output, FILE *fp) {
	

	int status = 0;

			
	fprintf(fp, "\nCor3901..................... starting CorrTime\n");
	printf("\nCor3901..................... starting CorrTime\n");	

	fits_copy_file(input, output, 1, 1, 1, &status);
	
	
	
		
///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////PACKETS////////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////

	CorrTimePKT(output, fp);

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	
						
	if ( status) {
		fprintf(fp, "Cor35XY..................... exiting CorrTime ERROR:");
		printf("Cor35XY..................... exiting CorrTime ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		return status;		
		}
	else {
		fprintf(fp, "Cor35XY..................... exiting CorrTime STATUS = 0\n\n");
		printf("Cor35XY..................... exiting CorrTime STATUS = 0\n\n");		
		}
		
	return status;		
	}



int WriteHeader(fitsfile * output){
	int status=0;
	char filename[512];
	int hdnum = 0;
	char corvers[100];
	sprintf(corvers,"Cor%s %s",packet, version);		
	fits_get_num_hdus(output, &hdnum, &status);
	
	for (int i=1; i<=hdnum; ++i) {
		fits_movabs_hdu(output, i, NULL, &status);

		fits_file_name(output, filename, &status);	
		fits_update_key(output, TSTRING, "FILENAME",  filename, NULL, &status);
		
		strcpy(filename, "Cor35XY");
		fits_update_key(output, TSTRING, "CREATOR",  filename, NULL, &status);	
		
		
		strcpy(filename, "IASF-MILANO");	
		fits_update_key(output, TSTRING, "ORIGIN",  filename, NULL, &status);		
		
		int timeref = 0;
		fits_get_system_time(filename, &timeref, &status);
		fits_update_key(output, TSTRING, "DATE",  filename, NULL, &status);
		
		fits_update_key(output, TSTRING, "COR_VERS",  corvers, NULL, &status);				
		}
	return status;

	}

	
	
	
int Close35XY(int argc, char ** argv, fitsfile * input, fitsfile * output, FILE *fp) {

	printf("Cor35XY..................... starting Close35XY\n");
	fprintf(fp, "Cor35XY..................... starting Close35XY\n");
	int status = 0;
	int hdnum = 0;
	
	
	
		
	double tmin=0;
	double tmax=0;
	fits_read_key(output, TDOUBLE, "TSTART", &tmin, NULL, &status );
	fits_read_key(output, TDOUBLE, "TSTOP", &tmax, NULL, &status );	
	if( argc==4) {
		FILE *fp4;
		if(checkfile(argv[3], argv[2], tmin, tmax, (char *)packet) == 0) {
			fp4 = fopen(argv[3], "a+");
			if (argv[2][0] == '!')
				fprintf(fp4,"%s  %f  %f %s\n", argv[2], tmin, tmax, packet);
			else
				fprintf(fp4,"%s  %f  %f %s\n", argv[2], tmin, tmax, packet);
			fclose(fp4);
			}
		}
		
		
		
		
		
		
	fits_close_file(input, &status);
	
	fits_get_num_hdus(output, &hdnum, &status);
	for (int i=1; i<=hdnum; ++i) {
		fits_movabs_hdu(output, i, NULL, &status);
		fits_write_chksum(output,&status);
		}
	fits_close_file(output, &status);
	if ( status) {
		fprintf(fp, "Cor35XY..................... exiting Close35XY ERROR:");
		printf("Cor35XY..................... exiting Close35XY ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);	
		printf("\n");
		return status;			
		}			

	fprintf(fp, "Cor35XY..................... exiting Close35XY STATUS = 0\n\n");
	printf("Cor35XY..................... exiting Close35XY STATUS = 0\n\n");		

		
		
	fprintf(fp,"TIME MINIMUM:  %f\n",tmin);		
	fprintf(fp,"TIME MAXIMUM:  %f\n",tmax);		
	printf("TIME MINIMUM:  %f\n",tmin);		
	printf("TIME MAXIMUM:  %f\n",tmax);		
			
	printf("\n\n####################################################\n");
	printf("#########  Task Cor35XY........... exiting #########\n");
	printf("####################################################\n\n\n");	
	
		
	fprintf(fp, "\n\n####################################################\n");
	fprintf(fp, "#########  Task Cor35XY........... exiting #########\n");
	fprintf(fp, "####################################################\n\n\n");			
		
	return status;
	}

	
int main(int argc,char **argv)
{
int status=0;
fitsfile ** input = new fitsfile*[1];
fitsfile ** output = new fitsfile*[1];


FILE *fp;
fp = fopen("/dev/null", "w");
status = Open35XY(argc, argv, input, output, fp);

if (status ==0)
	status = CorrTime(input[0], output[0], fp);
	
if (status ==0)	
	status = WriteHeader(output[0]);
	
if (status ==0)	
	status = Close35XY( argc, argv, input[0], output[0], fp);

	
if (status) {
	if(status >=1000) {
		getError(status);
		fpgetError(fp, status);
		}	
	else {
		fits_report_error(stderr, status);
		fits_report_error(fp, status);
		}
	}
else {	
	printf("STATUS %d\n",status);
	}		
delete []input;
delete []output;
exit(status);
}




