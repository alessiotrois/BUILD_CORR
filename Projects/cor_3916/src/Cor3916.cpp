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
//       3916.lv1       = grid event report
//
// OUTPUT 
//       3916.cor       = grid event report
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

#include "Cor3916.h"


int Open3916(int argc, char ** argv, fitsfile ** input, fitsfile ** output, FILE *fp) {
	int status=0,pkttype=0,pktstype=0;
	char * infile = new char[FLEN_FILENAME];
	char * outfile = new char[FLEN_FILENAME];
	int numpar=0;
	const time_t tp = time(NULL);
	char *data=ctime(&tp);	
	
	fprintf(fp,"\n\nSTART EXECUTION:  %s",data);	
	fprintf(fp, "\n\n####################################################\n");
	fprintf(fp, "#########  Task Cor3916......... starting  #########\n");
	fprintf(fp, "####################################################\n\n\n");
	fprintf(fp, "Cor3916..................... starting Open3916\n");

	
	printf("\n\nSTART EXECUTION:  %s  ",data);		
	printf("####################################################\n");
	printf("#########  Task Cor3916......... starting  #########\n");
	printf("#########         %s         #########\n", version);
	printf("####################################################\n\n");
	printf("Cor3916..................... starting Open3916\n");

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
	
	
	if (pkttype!=39 || pktstype!=16) {
		printf("error packet type [39]   %d!!\n",pkttype );
		printf("error packet subtype [16] %d!!\n",pktstype );
		fits_close_file(input[0], &status);
		return 1001;
		}

	printf("..........opening output file: %s\n",outfile);
	fprintf(fp,"..........opening output file: %s\n",outfile);	
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
		fprintf(fp, "Cor3916..................... exiting Cor3916 input ERROR:");
		printf("Cor3916..................... exiting Cor3916 input ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		return status;					
		}
	else {
		fprintf(fp, "Cor3916..................... exiting Cor3916\n");
		printf("Cor3916..................... exiting Cor3916\n");		
	
		}
	return status;
	}

int CorrTime(fitsfile * output, FILE *fp) {

	int oldstatus = 0;
	int status = 0;
	fprintf(fp, "\nCor3916..................... starting CorrTime\n");
	printf("\nCor3916..................... starting CorrTime\n");

 	long nrows = 0;
	int ncols = 0;
	int colnum = 0;
	double * time = NULL;	
	char unit[FLEN_KEYWORD];
	char * s = "s";		
	double tstart = 0;
	double tstop = 0;
	float timezero = 0;	
	
	
	double mjdrefi      = 53005.;
	double mjdreff      = 0.000754444444444;
	double mjdreff_2009 = 0.000766018518519;
	double mjdreff_2012 = 0.000777592592593;
	double mjdreff_2015 = 0.000789166666667;	
	
	
	const int utc_gps_offset = 14;
	
	
	
		
///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////PACKETS////////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////

	CorrTimePKT(output, fp);

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////		
		
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////EVENTS//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	CorrTimeEVT3916(output, fp);	
	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	
	if ( status) {
		fprintf(fp, "Cor3916..................... exiting CorrTime ERROR:");
		printf("Cor3916..................... exiting CorrTime ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		printf("\n");		
		return status;		
		}
	else {
		fprintf(fp, "Cor3916..................... exiting CorrTime STATUS = 0\n\n");
		printf("Cor3916..................... exiting CorrTime STATUS = 0\n\n");		
		}	
	
	
	return status;
	}

int CorrAtt(fitsfile * output, FILE *fp) {

	fprintf(fp, "\nCor3916..................... starting CorrAtt\n");
	printf("\nCor3916..................... starting CorrAtt\n");
	
	int status = 0, i = 0;  /* MUST initialize status */
	long nrows=0;
	int ncols = 0;
	int colnum = 0;
	
	fits_movabs_hdu(output, 3, NULL, &status);
	fits_get_num_rows(output, &nrows, &status);	
	fits_get_num_cols(output, &ncols, &status);		

	double * rho = new double[nrows];	
	double * phi = new double[nrows];
	double * theta = new double[nrows];
	double * x = new double[nrows];	
	double * y = new double[nrows];
	double * z = new double[nrows];			
    
	fits_get_colnum(output, 1, "SEQUAT1", &colnum, &status);	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, x, NULL, &status);	
	
	fits_get_colnum(output, 1, "SEQUAT2", &colnum, &status);	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, z, NULL, &status);	
	
	fits_get_colnum(output, 1, "SEQUAT3", &colnum, &status);	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, y, NULL, &status);			    

	for (i = 0; i < nrows; ++i) {
		rho[i] = sqrt(x[i]*x[i] + y[i]*y[i] + z[i]*z[i]);
		phi[i] = atan2d(y[i],-x[i]);
		if (phi[i] < 0)
			phi[i] += 360;
		theta[i] = acosd(z[i]/rho[i]);
		}
	fits_insert_col(output, 33, "EARTH_PHI", "1D", &status);
	fits_insert_col(output, 33, "EARTH_THETA", "1D", &status);
	fits_get_colnum(output, 1, "EARTH_PHI", &colnum, &status);	
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  phi, &status);
	fits_get_colnum(output, 1, "EARTH_THETA", &colnum, &status);	
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  theta, &status);
	
		
	delete [] rho;
	delete [] phi;
	delete [] theta;	
	delete [] x;	
	delete [] y;	
	delete [] z;	
	
	if ( status) {
		fprintf(fp, "Cor3916..................... exiting CorrTime ERROR:");
		printf("Cor3916..................... exiting CorrTime ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		return status;		
		}
	else {
		fprintf(fp, "Cor3916..................... exiting CorrTime STATUS = 0\n\n");
		printf("Cor3916..................... exiting CorrTime STATUS = 0\n\n");		
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
		
		strcpy(filename, "Cor3916");
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

int Close3916(int argc, char ** argv, fitsfile * input, fitsfile * output, FILE *fp) {

	int status=0;
	int hdnum = 0;
	
	fprintf(fp, "Cor3916..................... starting Close3916\n");
	printf("Cor3916..................... starting Close3916\n");	
	
	
	
	
	
		
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
		fprintf(fp, "Cor3916..................... exiting Close3916 ERROR:");
		printf("Cor3916..................... exiting Close3916 input ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);	
		printf("\n");
		return status;
		}			


			
	fprintf(fp, "Cor3916..................... exiting Close3916 STATUS = 0\n\n");
	printf("Cor3916..................... exiting Close3916 STATUS = 0\n\n");		

	
	fprintf(fp,"TIME MINIMUM:  %f\n",tmin);		
	fprintf(fp,"TIME MAXIMUM:  %f\n",tmax);		
	printf("TIME MINIMUM:  %f\n",tmin);		
	printf("TIME MAXIMUM:  %f\n",tmax);	
			
	printf("\n\n####################################################\n");
	printf("#########  Task Cor3916......... exiting #########\n");
	printf("####################################################\n\n\n");					
	fprintf(fp, "\n\n####################################################\n");
	fprintf(fp, "#########  Task Cor3916......... exiting #########\n");
	fprintf(fp, "####################################################\n\n\n");		
	
	return status;
	}

int main(int argc,char **argv)
{
int status=0;

fitsfile ** input = new fitsfile*[1];
fitsfile ** output = new fitsfile*[1];

FILE *fp;

fp = fopen("/dev/null", "a+");

status = Open3916(argc, argv, input, output, fp);

 if (status ==0)
 	status = CorrTime(output[0], fp);
 if (status ==0)	
	WriteHeader(output[0]);

	
if (status ==0)
	status = CorrAtt(output[0], fp);

if (status ==0)
	status = Close3916(argc, argv, input[0], output[0], fp);

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
fclose(fp);
delete []input;
delete []output;
return status;
}




