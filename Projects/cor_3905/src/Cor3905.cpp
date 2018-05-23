////////////////////////////////////////////////////////////////////////////////
// DESCRIPTION
//       correction pipeline I/O routine
//       Star sensor packet
//       Release: V0.1 -  01/Dec/2016
//       Contributors: Andrea Argan (IAPS)
//       Authora: Alessio Trois (OA-Cagliari), Ettore Del Monte (IAPS), Yuri Evangelista (IAPS)
//
//
// INPUT
//       3905.lv1       = grid event report
//
// OUTPUT 
//       3905.cor       = grid event report
//
//
// FILE HISTORY
//       First release: V1.0 16/Mar/2005
//
//       Last Release: V0.2 -  24/feb/2006
//                      Authors:       Alessio Trois (OA-Cagliari), Ettore Del Monte (IAPS), Yuri Evangelista (IAPS)

// NOTICE
//       Any information contained in this software
//       is property of the AGILE TEAM and is strictly
//       private and confidential.
//       All rights reserved.
////////////////////////////////////////////////////////////////////////////////////

#include "Cor3905.h"


int alikePolrec(double R, double A, double * X, double * Y, int degrees=0){
	double cf = 1.0;
	if (degrees) cf = 180.0/PI;
 
	*X = R*cos(A/cf);
	*Y = R*sin(A/cf);
	return 0;
}

int alikeRecpol(double x, double y, double *r, double *a, int degrees=0){
 
	if ((x != 0) || (y != 0)) {
		*a=atan2(y,x);
		if (*a < 0) 
			*a += 2*PI;
		}
	else 
		*a=0;
 
	*r = sqrt(x * x + y * y);
 
	if (degrees)
		*a *= 180.0/PI;
	
	return 0;
	} 

double alikeSphdist(double long1, double lat1, double long2, double lat2, int degrees=0){
	double cf = (degrees ? 180.0/PI : 1.0);
 
	double rxy, z1, x1, y1, z2, x2, y2, r, a;
	
	alikePolrec( 1.0, lat1/cf, &rxy, &z1);
	alikePolrec( rxy, long1/cf, &x1, &y1);
	alikePolrec( 1.0, lat2/cf, &rxy, &z2);
	alikePolrec( rxy, long2/cf, &x2, &y2);
 
	double cs = x1*x2 + y1*y2 + z1*z2;
 
//	;--- Compute the vector cross product for both points. ---
	double xc = y1*z2 - z1*y2;
	double yc = z1*x2 - x1*z2;
	double zc = x1*y2 - y1*x2;
	double sn = sqrt(xc*xc + yc*yc + zc*zc);
 
//	;--- Convert to polar.  ------
	alikeRecpol( cs, sn, &r, &a);
	return cf*a;

}



int Open3905(int argc, char ** argv, fitsfile ** input, fitsfile ** output, FILE *fp) {
	int status=0,pkttype=0,pktstype=0;
	char * infile = new char[FLEN_FILENAME];
	char * outfile = new char[FLEN_FILENAME];
	int numpar=0;
	const time_t tp = time(NULL);
	char *data=ctime(&tp);	
	

	
	printf("\n\nSTART EXECUTION:  %s  ",data);		
	printf("####################################################\n");
	printf("#########  Task Cor3905......... starting  #########\n");
	printf("#########         %s         #########\n", version);
	printf("####################################################\n\n");
	printf("Cor3905..................... starting Open3905\n");

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
// 	fprintf(fp,"..........opening input file: %s\n",infile);	
	fits_open_file(input, infile, READONLY, &status);
	if (status) {
		return status;
		}

	fits_read_key(input[0], TINT, "PKTTYPE", &pkttype, NULL, &status );
	fits_read_key(input[0], TINT, "PKTSTYPE", &pktstype, NULL, &status );
	
	
	if (pkttype!=39 || pktstype!=5) {
		printf("error packet type [39]   %d!!\n",pkttype );
		printf("error packet subtype [5] %d!!\n",pktstype );
		fits_close_file(input[0], &status);
		return 1001;
		}

	printf("..........opening output file: %s\n",outfile);
// 	fprintf(fp,"..........opening output file: %s\n",outfile);	
	fits_create_file(&output[0], outfile, &status);
	if (status) {
		return status;
		}		
			
	PILClose(status);
	
	fits_movabs_hdu(input[0], 3, NULL, &status);
	fits_copy_file(input[0], output[0], 1, 1, 1, &status);
	fits_flush_file(output[0], &status);	
	
	delete []infile;
	delete []outfile;		
		
	if ( status) {
// 		fprintf(fp, "Cor3905..................... exiting Cor3905 input ERROR:");
		printf("Cor3905..................... exiting Cor3905 input ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		return status;					
		}
	else {
// 		fprintf(fp, "Cor3905..................... exiting Cor3905\n");
		printf("Cor3905..................... exiting Cor3905\n");		
	
		}
	return status;
	}

	


	
int CorrTime(fitsfile * output, FILE *fp) {

	int oldstatus = 0;
	int status = 0;
// 	fprintf(fp, "\nCor3905..................... starting CorrTime\n");
	printf("\nCor3905..................... starting CorrTime\n");	
	long nrows = 0;
	int ncols = 0;


	int colnum = 0;

// 	double * time = NULL;	
// 	int * secpktobt;
// 	int * microsecpktobt;	
	char unit[FLEN_KEYWORD];
	char * s = "s";		
	double tstart = 0;
	double tstop = 0;
	float timezero = 0;	
	char str[100];
	
	
	double mjdrefi      = 53005.;
	double mjdreff      = 0.000754444444444;
	double mjdreff_2009 = 0.000766018518519;
	double mjdreff_2012 = 0.000777592592593;
	double mjdreff_2015 = 0.000789166666667;	
		
///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////PACKETS////////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////

	CorrTimePKTSA(output, fp);

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////		

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////EVENTS//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	

	CorrTimePKTSAEVT(output, fp);

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

	
		
	if ( status) {
// 		fprintf(fp, "Cor3905..................... exiting CorrTime ERROR:");
		printf("Cor3905..................... exiting CorrTime ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		printf("\n");		
		return status;		
		}
	else {
// 		fprintf(fp, "Cor3905..................... exiting CorrTime STATUS = 0\n\n");
		printf("Cor3905..................... exiting CorrTime STATUS = 0\n\n");		
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
		
		strcpy(filename, "Cor3905");
		fits_update_key(output, TSTRING, "CREATOR",  filename, NULL, &status);	
		
		strcpy(filename, "39.05");		
		fits_update_key(output, TSTRING, "CREAT_ID",  filename, NULL, &status);	
		
		strcpy(filename, "IAPS-ROMA");	
		fits_update_key(output, TSTRING, "ORIGIN",  filename, NULL, &status);		
		
		int timeref = 0;
		fits_get_system_time(filename, &timeref, &status);
		fits_update_key(output, TSTRING, "DATE",  filename, NULL, &status);
		
		fits_update_key(output, TSTRING, "COR_VERS",  corvers, NULL, &status);	
					
		}
	return status;
	}



int Close3905(int argc, char ** argv, fitsfile * input, fitsfile * output, FILE *fp) {

	int status=0;
	int hdnum = 0;
	

	printf("Cor3905..................... starting Close3905\n");	

	
	
		
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
		printf("Cor3905..................... exiting Close3905 input ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);	
		printf("\n");
		return status;
		}			


			
	printf("Cor3905..................... exiting Close3905 STATUS = 0\n\n");
	printf("TIME MINIMUM:  %f\n",tmin);		
	printf("TIME MAXIMUM:  %f\n",tmax);	
	printf("\n\n####################################################\n");
	printf("#########  Task Cor3905......... exiting #########\n");
	printf("####################################################\n\n\n");					

	return status;
	}

int main(int argc,char **argv)
{

int status=0;

fitsfile ** input = new fitsfile*[1];
fitsfile ** output = new fitsfile*[1];

FILE *fp;

// fp = fopen("/dev/null", "a+");

status = Open3905(argc, argv, input, output, fp);

if (status ==0)
	status = CorrTime(output[0], fp);
	
if (status ==0)	
	WriteHeader(output[0]);
	

if (status ==0)
	status = Close3905(argc, argv, input[0], output[0], fp);

if (status) {
	if(status >=1000) {
		getError(status);
// 		fpgetError(fp, status);
		}	
	else {
		fits_report_error(stderr, status);
// 		fits_report_error(fp, status);
		}
	}
else {	
	printf("STATUS %d\n",status);
	}	
// fclose(fp);
delete []input;
delete []output;
return status;
}




