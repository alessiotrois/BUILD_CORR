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

#include "Cor3901.h"
#include "TH1.h"
#include "TF1.h"
#include <iostream>
#include <vector>
#include "TCanvas.h"
using namespace std;



	
int openPedestal(char *fileped, char * fileList, double tsearch, char * pkt){



	
	char buffer[1024];
	int status = 0;
	FILE *fp = fopen(fileList, "r");
	if(!fp) {
		printf("Errore in apertura file '%s'\n", fileList);
		return 104;	
		}
		
// 	fitsfile *tempFits;	
// 	int hdutype = 0;
// 	int  find = 0;
	char *name=new char[FLEN_FILENAME];
	char *nameold=new char[FLEN_FILENAME];	
	char *type=new char[FLEN_FILENAME];
	double t1,t2;

	while (fgets(buffer , 40960, fp)) {
		sscanf(buffer, "%s %lf %lf %s", name, &t1, &t2, type);
// 		cout<<name<<" "<<t1<<" "<<t2<<endl;
		if (tsearch > t1) {
			strcpy(nameold,name);
			}
		}
	
	strcpy(fileped,nameold);	

	
	delete [] name;
	delete [] type;
// 	if (find == 0) {
// 		return 1005;
// 		}	
	fclose(fp);
	return status;
	}






int Open3901(int argc, char ** argv, fitsfile ** input, fitsfile ** output, FILE *fp) {

	int status=0, pkttype=0, pktstype=0;
	char * infile = new char[FLEN_FILENAME];
	char * outfile = new char[FLEN_FILENAME];
	char * index3903 = new char[FLEN_FILENAME];	
	int numpar=0;
	const time_t tp = time(NULL);
	char *data=ctime(&tp);
	
	
	fprintf(fp,"\n\nSTART EXECUTION:  %s",data);	
	fprintf(fp, "\n\n####################################################\n");
	fprintf(fp, "#########  Task Cor3901......... starting  #########\n");
	fprintf(fp, "####################################################\n\n\n");
	fprintf(fp, "Cor3901..................... starting Open3901\n");

	printf("\n\nSTART EXECUTION:  %s",data);	
	printf("####################################################\n");
	printf("#########  Task Cor3901......... starting  #########\n");
	printf("#########         %s         #########\n", version);
	printf("####################################################\n\n\n");
	printf("Cor3901..................... starting Open3901\n");
	
	status = PILInit(argc,argv);
	PILGetNumParameters(&numpar);

	if ( status != 0 || numpar!=3)
		return 1002;

	PILGetString("infile",infile);
	PILGetString("outfile",outfile);
	PILGetString("index3903",index3903);
	
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
	
	
	if (pkttype!=39 || pktstype!=1) {
		printf("error packet type [39]   %d!!\n",pkttype );
		printf("error packet subtype [1] %d!!\n",pktstype );
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


	delete []infile;
	delete []outfile;	
			
	if ( status) {
		fprintf(fp, "Cor3901..................... exiting Open3901 input ERROR:");
		printf("Cor3901..................... exiting Open3901 input ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		return status;					
		}
	else {
		fprintf(fp, "Cor3901..................... exiting Open3901\n");
		printf("Cor3901..................... exiting Open3901\n");		
	
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

	status = CorrTimePKT(output, fp);

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	



///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////EVENTS////////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////	
	status = CorrTimeEVT(output, fp);
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
	
	if ( status) {
		fprintf(fp, "Cor3901..................... exiting CorrTime ERROR:");
		printf("Cor3901..................... exiting CorrTime ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		return status;			
		}			
	else {
		fprintf(fp, "Cor3901..................... exiting CorrTime STATUS = 0\n\n");
		printf("Cor3901..................... exiting CorrTime STATUS = 0\n\n");		
		}
		
	return status;		
	}

int CorrCluster(int argc, char ** argv, fitsfile * output, FILE *fp) {

	int status = 0;
// 	int oldstatus = 0;
	int i = 0;
	int colnum = 0;	
	int ncols = 0;		
	long nrows = 0;

	
// 	double tstart = 0;
// 	double tstop = 0;
// 	float timezero = 0;	
// cout<<"ok 1"<<status<<endl;	
	fits_movabs_hdu(output, 3, NULL, &status);
	
	fits_get_num_rows(output, &nrows, &status);
// cout<<"ok 2"<<status<<endl;
	fits_get_num_cols(output, &ncols, &status);
// cout<<"ok 3"<<status<<endl;	
	int febx[nrows][48];
	int chipx[nrows][48];	
	int stripx[nrows][48];	
	unsigned int totcharx[nrows][48];
	int stripc1x[nrows][48];
	int stripc2x[nrows][48];		
	int stripc3x[nrows][48];	
	int stripc4x[nrows][48];		
	int stripc5x[nrows][48];	
	long totwidtx[nrows][48];
	unsigned short int nclustx[nrows];	
// cout<<"ok 4"<<status<<endl;
				
	int febz[nrows][48];
	int chipz[nrows][48];	
	int stripz[nrows][48];	
	unsigned int totcharz[nrows][48];
	int stripc1z[nrows][48];
	int stripc2z[nrows][48];		
	int stripc3z[nrows][48];	
	int stripc4z[nrows][48];		
	int stripc5z[nrows][48];	
	long totwidtz[nrows][48];
	unsigned short int nclustz[nrows];	
	
		
int stridisablebis[792][4] = {{0,0,2,100},{0,0,2,122},{0,0,2,126},{0,0,2,127},{0,0,3,89},{0,0,4,34},{0,0,4,35},{0,0,4,116},{0,0,4,117},{0,0,4,118},{0,0,4,119},{0,0,4,120},{0,0,4,121},{0,0,4,122},{0,0,4,123},{0,0,4,124},{0,0,4,125},{0,0,4,126},{0,0,5,126},{0,0,5,127},{0,0,10,35},{0,0,11,8},{0,0,11,78},{0,0,12,84},{0,0,13,56},{0,0,13,74},{0,0,13,96},{0,0,13,97},{0,0,13,117},{0,0,15,15},{0,0,15,16},{0,0,15,17},{0,0,15,18},{0,0,15,19},{0,0,15,20},{0,0,15,21},{0,0,15,25},{0,0,15,26},{0,0,15,27},{0,0,18,24},{0,0,18,25},{0,0,18,26},{0,0,18,27},{0,0,18,28},{0,0,18,29},{0,0,20,0},{0,0,20,1},{0,0,20,2},{0,0,20,3},{0,0,20,4},{0,0,20,5},{0,0,20,6},{0,0,20,7},{0,0,20,8},{0,0,20,9},{0,0,20,10},{0,0,20,11},{0,0,20,12},{0,0,20,13},{0,0,20,14},{0,0,20,15},{0,0,20,16},{0,0,20,17},{0,0,20,18},{0,0,20,19},{0,0,20,20},{0,0,20,21},{0,0,20,22},{0,0,20,23},{0,0,20,24},{0,0,20,25},{0,0,20,26},{0,0,20,27},{0,0,20,28},{0,0,20,29},{0,0,20,30},{0,0,20,31},{0,0,20,32},{0,0,20,33},{0,0,20,34},{0,0,20,35},{0,0,20,36},{0,0,20,37},{0,0,20,38},{0,0,20,39},{0,0,20,40},{0,0,20,41},{0,0,20,42},{0,0,20,43},{0,0,20,44},{0,0,20,45},{0,0,20,46},{0,0,20,47},{0,0,20,48},{0,0,20,49},{0,0,20,50},{0,0,20,51},{0,0,20,52},{0,0,20,53},{0,0,20,54},{0,0,20,55},{0,0,20,56},{0,0,20,57},{0,0,20,58},{0,0,20,59},{0,0,20,60},{0,0,20,61},{0,0,20,62},{0,0,20,63},{0,0,20,64},{0,0,20,65},{0,0,20,66},{0,0,20,67},{0,0,20,68},{0,0,20,69},{0,0,20,70},{0,0,20,71},{0,0,20,72},{0,0,20,73},{0,0,20,74},{0,0,20,75},{0,0,20,76},{0,0,20,77},{0,0,20,78},{0,0,20,79},{0,0,20,80},{0,0,20,81},{0,0,20,82},{0,0,20,83},{0,0,20,84},{0,0,20,85},{0,0,20,86},{0,0,20,87},{0,0,20,88},{0,0,20,89},{0,0,20,90},{0,0,20,91},{0,0,20,92},{0,0,20,93},{0,0,20,94},{0,0,20,95},{0,0,20,96},{0,0,20,97},{0,0,20,98},{0,0,20,99},{0,0,20,100},{0,0,20,101},{0,0,20,102},{0,0,20,103},{0,0,20,104},{0,0,20,105},{0,0,20,106},{0,0,20,107},{0,0,20,108},{0,0,20,109},{0,0,20,110},{0,0,20,111},{0,0,20,112},{0,0,20,113},{0,0,20,114},{0,0,20,115},{0,0,20,116},{0,0,20,117},{0,0,20,118},{0,0,20,119},{0,0,20,120},{0,0,20,121},{0,0,20,122},{0,0,20,123},{0,0,20,124},{0,0,20,125},{0,0,20,126},{0,0,20,127},{0,0,23,32},{0,0,23,35},{0,0,23,79},{0,1,0,5},{0,1,0,7},{0,1,1,6},{0,1,1,16},{0,1,1,31},{0,1,1,37},{0,1,1,41},{0,1,1,46},{0,1,1,49},{0,1,1,51},{0,1,1,52},{0,1,1,54},{0,1,1,55},{0,1,1,56},{0,1,1,58},{0,1,1,60},{0,1,1,62},{0,1,1,63},{0,1,1,64},{0,1,1,66},{0,1,1,67},{0,1,1,68},{0,1,1,69},{0,1,1,70},{0,1,1,71},{0,1,1,72},{0,1,1,76},{0,1,1,77},{0,1,1,78},{0,1,1,79},{0,1,1,81},{0,1,1,83},{0,1,1,84},{0,1,1,86},{0,1,1,87},{0,1,1,88},{0,1,1,89},{0,1,1,90},{0,1,1,91},{0,1,1,92},{0,1,1,93},{0,1,1,94},{0,1,1,96},{0,1,1,97},{0,1,1,98},{0,1,1,99},{0,1,1,103},{0,1,1,107},{0,1,1,109},{0,1,2,116},{0,1,3,95},{0,1,3,96},{0,1,5,126},{0,1,5,127},{0,1,7,16},{0,1,7,17},{0,1,7,20},{0,1,7,54},{0,1,7,78},{0,1,8,67},{0,1,8,68},{0,1,9,94},{0,1,11,7},{0,1,11,99},{0,1,11,125},{0,1,15,48},{0,1,16,84},{0,1,16,85},{0,1,16,86},{0,1,19,0},{0,1,19,14},{0,1,19,126},{0,1,19,127},{0,1,20,0},{0,1,20,1},{0,1,21,38},{0,1,23,112},{0,1,23,113},{0,1,23,114},{0,1,23,115},{0,1,23,116},{0,1,23,127},{0,2,0,126},{0,2,0,127},{0,2,1,0},{0,2,1,1},{0,2,2,126},{0,2,2,127},{0,2,4,0},{0,2,5,28},{0,2,5,29},{0,2,5,30},{0,2,5,31},{0,2,5,32},{0,2,5,74},{0,2,5,75},{0,2,5,76},{0,2,5,77},{0,2,5,78},{0,2,5,126},{0,2,5,127},{0,2,6,0},{0,2,6,6},{0,2,6,119},{0,2,6,120},{0,2,6,121},{0,2,6,122},{0,2,6,123},{0,2,6,127},{0,2,7,0},{0,2,7,126},{0,2,7,127},{0,2,8,0},{0,2,8,90},{0,2,8,91},{0,2,8,92},{0,2,8,93},{0,2,8,94},{0,2,8,127},{0,2,9,0},{0,2,9,127},{0,2,10,0},{0,2,10,102},{0,2,10,114},{0,2,10,126},{0,2,10,127},{0,2,11,0},{0,2,11,15},{0,2,11,126},{0,2,11,127},{0,2,12,0},{0,2,13,0},{0,2,15,25},{0,2,15,26},{0,2,15,27},{0,2,17,126},{0,2,17,127},{0,2,18,0},{0,2,18,13},{0,2,18,14},{0,2,18,116},{0,2,18,117},{0,2,19,110},{0,2,20,127},{0,3,2,52},{0,3,2,123},{0,3,3,14},{0,3,3,28},{0,3,3,29},{0,3,3,30},{0,3,4,101},{0,3,7,103},{0,3,8,45},{0,3,8,46},{0,3,8,47},{0,3,8,48},{0,3,10,7},{0,3,10,8},{0,3,10,40},{0,3,11,127},{0,3,12,17},{0,3,12,18},{0,3,12,19},{0,3,12,20},{0,3,12,21},{0,3,13,127},{0,3,15,97},{0,3,19,89},{0,3,19,91},{0,3,19,93},{0,3,19,111},{0,3,19,112},{0,3,19,113},{0,3,19,114},{0,3,19,115},{0,3,21,106},{0,3,21,107},{0,3,21,108},{0,3,21,109},{0,3,21,110},{0,3,21,111},{0,3,21,127},{0,3,23,20},{0,3,23,21},{0,4,0,125},{0,4,0,126},{0,4,0,127},{0,4,1,0},{0,4,1,1},{0,4,1,47},{0,4,1,48},{0,4,1,49},{0,4,1,50},{0,4,1,51},{0,4,1,52},{0,4,1,53},{0,4,1,108},{0,4,1,109},{0,4,1,110},{0,4,1,111},{0,4,1,112},{0,4,1,113},{0,4,1,126},{0,4,2,1},{0,4,2,9},{0,4,2,11},{0,4,2,41},{0,4,2,43},{0,4,2,51},{0,4,2,76},{0,4,2,108},{0,4,2,109},{0,4,2,110},{0,4,2,111},{0,4,2,112},{0,4,2,124},{0,4,5,126},{0,4,5,127},{0,4,6,0},{0,4,7,40},{0,4,8,0},{0,4,8,4},{0,4,11,38},{0,4,11,73},{0,4,12,8},{0,4,12,11},{0,4,12,43},{0,4,12,49},{0,4,12,66},{0,4,12,108},{0,4,12,123},{0,4,13,15},{0,4,13,33},{0,4,14,40},{0,4,14,95},{0,4,14,101},{0,4,14,111},{0,4,14,126},{0,4,14,127},{0,4,15,0},{0,4,15,28},{0,4,15,29},{0,4,15,30},{0,4,16,120},{0,4,16,125},{0,4,17,110},{0,4,17,111},{0,4,17,112},{0,4,17,113},{0,4,17,114},{0,4,17,115},{0,4,17,116},{0,4,17,119},{0,4,17,126},{0,4,17,127},{0,4,18,5},{0,4,18,8},{0,4,18,11},{0,4,18,12},{0,4,18,13},{0,4,18,32},{0,4,18,63},{0,4,18,74},{0,4,18,75},{0,4,18,77},{0,4,19,2},{0,4,20,27},{0,4,20,28},{0,4,20,29},{0,4,20,30},{0,4,20,31},{0,4,21,20},{0,4,22,59},{0,4,22,60},{0,4,22,61},{0,4,22,66},{0,4,23,0},{0,4,23,5},{0,4,23,6},{0,4,23,10},{0,4,23,18},{0,4,23,29},{0,4,23,32},{0,4,23,33},{0,4,23,34},{0,4,23,45},{0,4,23,52},{0,4,23,56},{0,4,23,61},{0,4,23,74},{0,4,23,83},{0,4,23,86},{0,4,23,87},{0,4,23,90},{0,4,23,105},{0,4,23,106},{0,4,23,121},{0,4,23,122},{0,5,2,31},{0,5,2,32},{0,5,7,98},{0,5,7,99},{0,5,7,100},{0,5,8,83},{0,5,10,0},{0,5,13,106},{0,5,13,107},{0,5,13,108},{0,5,13,109},{0,5,13,110},{0,5,13,111},{0,5,13,112},{0,5,13,113},{0,5,13,114},{0,5,16,49},{0,5,17,16},{0,5,17,127},{0,5,18,0},{0,5,20,42},{0,5,20,43},{0,5,20,44},{0,5,20,45},{0,5,20,56},{0,5,20,57},{0,5,23,125},{0,5,23,126},{1,0,3,31},{1,0,4,95},{1,0,4,96},{1,0,4,97},{1,0,4,98},{1,0,4,99},{1,0,5,65},{1,0,6,24},{1,0,8,0},{1,0,8,10},{1,0,8,18},{1,0,9,3},{1,0,16,94},{1,0,16,95},{1,0,16,96},{1,0,16,97},{1,0,16,98},{1,0,16,99},{1,0,16,100},{1,0,17,32},{1,0,18,35},{1,0,18,42},{1,0,18,67},{1,0,19,95},{1,0,20,40},{1,0,20,110},{1,1,2,2},{1,1,4,86},{1,1,4,87},{1,1,4,88},{1,1,9,88},{1,1,10,21},{1,1,10,26},{1,1,14,111},{1,1,17,83},{1,1,17,109},{1,1,17,126},{1,1,21,4},{1,1,21,6},{1,1,21,12},{1,1,21,13},{1,1,21,34},{1,1,21,35},{1,1,21,38},{1,1,21,44},{1,1,21,69},{1,1,21,73},{1,1,23,55},{1,1,23,125},{1,1,23,126},{1,2,0,1},{1,2,12,4},{1,2,12,82},{1,2,13,10},{1,2,13,12},{1,2,13,26},{1,2,13,41},{1,2,13,42},{1,2,13,43},{1,2,13,44},{1,2,13,45},{1,2,13,46},{1,2,13,57},{1,2,13,68},{1,2,13,72},{1,2,13,83},{1,2,13,88},{1,2,13,94},{1,2,13,95},{1,2,13,102},{1,2,13,104},{1,2,13,114},{1,2,14,3},{1,2,14,4},{1,2,14,7},{1,2,14,9},{1,2,14,26},{1,2,14,28},{1,2,14,29},{1,2,14,30},{1,2,14,31},{1,2,14,68},{1,2,14,72},{1,2,14,84},{1,2,14,105},{1,2,14,114},{1,2,14,120},{1,2,16,83},{1,2,16,84},{1,2,16,85},{1,2,16,86},{1,2,17,71},{1,2,18,17},{1,2,18,25},{1,2,18,34},{1,2,19,4},{1,2,19,81},{1,2,19,82},{1,2,20,14},{1,2,20,15},{1,2,20,16},{1,2,20,17},{1,2,20,18},{1,2,20,111},{1,2,21,15},{1,2,23,3},{1,2,23,5},{1,2,23,6},{1,2,23,18},{1,2,23,24},{1,2,23,37},{1,2,23,105},{1,3,0,126},{1,3,1,0},{1,3,1,115},{1,3,1,116},{1,3,1,117},{1,3,1,118},{1,3,5,4},{1,3,6,57},{1,3,7,4},{1,3,7,33},{1,3,7,86},{1,3,9,6},{1,3,9,7},{1,3,11,125},{1,3,12,68},{1,3,14,22},{1,3,17,0},{1,3,19,4},{1,3,20,31},{1,3,22,19},{1,3,22,20},{1,3,22,21},{1,3,22,22},{1,3,22,23},{1,3,22,71},{1,3,22,72},{1,3,22,73},{1,3,22,74},{1,3,22,75},{1,3,22,76},{1,4,2,14},{1,4,2,96},{1,4,3,51},{1,4,3,82},{1,4,3,83},{1,4,4,92},{1,4,4,93},{1,4,4,94},{1,4,4,95},{1,4,4,125},{1,4,4,126},{1,4,4,127},{1,4,5,22},{1,4,9,5},{1,4,9,6},{1,4,9,7},{1,4,9,8},{1,4,9,20},{1,4,9,28},{1,4,9,37},{1,4,10,48},{1,4,10,49},{1,4,19,98},{1,4,19,99},{1,4,19,100},{1,4,19,122},{1,5,0,1},{1,5,0,2},{1,5,0,3},{1,5,1,13},{1,5,1,21},{1,5,1,56},{1,5,2,3},{1,5,2,4},{1,5,2,56},{1,5,2,75},{1,5,3,3},{1,5,3,10},{1,5,3,12},{1,5,3,13},{1,5,3,14},{1,5,3,15},{1,5,3,16},{1,5,3,17},{1,5,3,20},{1,5,3,21},{1,5,3,25},{1,5,3,26},{1,5,3,27},{1,5,3,28},{1,5,3,29},{1,5,3,31},{1,5,3,32},{1,5,3,33},{1,5,3,34},{1,5,3,35},{1,5,3,41},{1,5,3,43},{1,5,3,44},{1,5,3,45},{1,5,3,47},{1,5,3,48},{1,5,3,49},{1,5,3,50},{1,5,3,52},{1,5,3,53},{1,5,3,55},{1,5,3,61},{1,5,3,68},{1,5,3,69},{1,5,3,71},{1,5,3,72},{1,5,3,74},{1,5,3,78},{1,5,3,79},{1,5,3,81},{1,5,3,82},{1,5,3,85},{1,5,3,88},{1,5,3,93},{1,5,3,98},{1,5,3,104},{1,5,3,105},{1,5,3,107},{1,5,3,109},{1,5,3,113},{1,5,3,115},{1,5,3,116},{1,5,3,118},{1,5,3,119},{1,5,3,122},{1,5,3,123},{1,5,5,58},{1,5,5,63},{1,5,7,12},{1,5,9,4},{1,5,9,16},{1,5,10,103},{1,5,10,104},{1,5,10,105},{1,5,11,5},{1,5,11,6},{1,5,11,14},{1,5,11,36},{1,5,11,50},{1,5,11,51},{1,5,11,52},{1,5,11,81},{1,5,11,82},{1,5,11,83},{1,5,11,84},{1,5,11,85},{1,5,11,86},{1,5,11,87},{1,5,11,88},{1,5,11,89},{1,5,11,90},{1,5,11,91},{1,5,11,92},{1,5,11,93},{1,5,18,4},{1,5,18,8},{1,5,19,1},{1,5,19,21},{1,5,19,23},{1,5,19,26},{1,5,19,28},{1,5,19,38},{1,5,19,76},{1,5,19,121},{1,5,19,122},{1,5,19,123},{1,5,19,124},{1,5,19,125},{1,5,20,52},{1,5,20,53},{1,5,20,54},{1,5,20,124},{1,5,21,14},{1,5,21,17},{1,5,22,5},{1,5,22,6},{1,5,22,35},{1,5,22,36},{1,5,22,37},{1,5,22,38},{1,5,23,72},{1,5,23,74}};


int stridisableter[24][4] = {{0,0,20,0},{0,1,19,0},{0,1,20,0},{0,2,1,0},{0,2,4,0},{0,2,6,0},{0,2,7,0},{0,2,8,0},{0,2,9,0},{0,2,10,0},{0,2,11,0},{0,2,12,0},{0,2,13,0},{0,2,18,0},{0,4,1,0},{0,4,6,0},{0,4,8,0},{0,4,15,0},{0,4,23,0},{0,5,10,0},{0,5,18,0},{1,0,8,0},{1,3,1,0},{1,3,17,0}};

///////////////////////////////////////////////////	
///////////////////////////////////////////////////	
///////////////////////////////////////////////////	
///////////////////////////////////////////////////	
	int numpar=0;
	fitsfile * iFilePed;
	char * index3903 = new char[FLEN_FILENAME];
	char * fileped = new char[FLEN_FILENAME];	
	double tsearch = 0;
	long nrowsped = 0;
	
	status = PILInit(argc,argv);
	PILGetNumParameters(&numpar);

	if ( status != 0 || numpar!=3)
		return 1002;

	PILGetString("index3903",index3903);
	
	if (index3903[0] == '@') index3903 = index3903+1; else index3903 = index3903;		
	char * tempindex = new char[FLEN_FILENAME];
	filetemp(tempindex);
	char * tempcommand = new char[2048];
	sprintf(tempcommand, "cp %s %s",index3903,tempindex);
	system(tempcommand);
	orderFileIndex(tempindex);		

	fits_read_key(output, TDOUBLE, "TSTART", &tsearch, NULL, &status );
// 	fits_read_key(output, TDOUBLE, "TSTOP", &tmax, NULL, &status );		
	
	
	status = openPedestal(fileped, tempindex, tsearch, "3903");
	remove(tempindex);
// 	cout<<	fileped<<endl;
	if ( fits_open_file(&iFilePed, fileped, READONLY, &status) != 0 ) {
		printf("Errore in apertura file '%s'\n", fileped);
		return status;
		}

			
	fits_movabs_hdu(iFilePed, 2, NULL, &status);
	fits_get_num_rows(iFilePed, &nrowsped, &status);

	int ped_ftb[nrowsped];
	int ped_feb[nrowsped];
	int ped_add[nrowsped];		
	float ped_strip[nrowsped][128];	
	
	fits_get_colnum(iFilePed, 1, "EVFTBADD", &colnum, &status);	
	fits_read_col(iFilePed, TINT, colnum, 1, 1, nrowsped, NULL, ped_ftb, NULL, &status);		
		
	fits_get_colnum(iFilePed, 1, "EVFEBADD", &colnum, &status);	
	fits_read_col(iFilePed, TINT, colnum, 1, 1, nrowsped, NULL, ped_feb, NULL, &status);	
	
	fits_get_colnum(iFilePed, 1, "EVTA1ADD", &colnum, &status);	
	fits_read_col(iFilePed, TINT, colnum, 1, 1, nrowsped, NULL, ped_add, NULL, &status);

	fits_get_colnum(iFilePed, 1, "PEDSIGMA", &colnum, &status);	
	
		
	int count_stripdisabled = 0;
	for (i = 0; i < nrowsped; ++i) {
	fits_read_col(iFilePed, TFLOAT, colnum, 1, 1, nrowsped*128, NULL, ped_strip, NULL, &status);
		for (int j = 0; j < 128; ++j) {
			if (ped_strip[i][j] < 13) {
				for (int k = 0; k < 792; ++k) {
					if ((stridisablebis[k][0] == ped_ftb[i]) &&  (stridisablebis[k][1] == ped_feb[i]) && (stridisablebis[k][2] == ped_add[i]) && (stridisablebis[k][3] == j) ) {
// 						cout<<ped_ftb[i]<<" "<<ped_feb[i]<<" "<<ped_add[i]<<" "<<j<<" "<<ped_strip[i][j]<<endl;	
						count_stripdisabled++;
						}	
					}				

				}
			}				
		}	
	
		
	int count= 0;	
	int stridisable[count_stripdisabled+24][4];
	for (int i = 0; i < nrowsped; ++i) {
		fits_read_col(iFilePed, TFLOAT, colnum, 1, 1, nrowsped*128, NULL, ped_strip, NULL, &status);
		for (int j = 0; j < 128; ++j) {
			if (ped_strip[i][j] < 13) {
				for (int k = 0; k < 792; ++k) {
					if ((stridisablebis[k][0] == ped_ftb[i]) &&  (stridisablebis[k][1] == ped_feb[i]) && (stridisablebis[k][2] == ped_add[i]) && (stridisablebis[k][3] == j) ) {
						stridisable[count][0] = ped_ftb[i];
						stridisable[count][1] = ped_feb[i];
						stridisable[count][2] = ped_add[i];
						stridisable[count][3] = j;
// 						cout<<ped_ftb[i]<<" "<<ped_feb[i]<<" "<<ped_add[i]<<" "<<j<<" "<<ped_strip[i][j]<<endl;	
// 						cout<<stridisable[count][0]<<" "<<stridisable[count][1]<<" "<<stridisable[count][2]<<" "<<j<<" "<<stridisable[count][3]<<endl;
						count++;							
						}	
					}										
				}
			}				
		}	
	
	for (int k = 0; k < 24; ++k) {
		stridisable[count_stripdisabled+k][0] = stridisableter[k][0];
		stridisable[count_stripdisabled+k][1] = stridisableter[k][1];
		stridisable[count_stripdisabled+k][2] = stridisableter[k][2];
		stridisable[count_stripdisabled+k][3] = stridisableter[k][3];
		}	
	count_stripdisabled += 24;	
// 	cout<<count_stripdisabled<<endl;		
	
// 	for (int k = 0; k < count_stripdisabled; ++k) {
// 		cout<<stridisable[k][0]<<" "<<stridisable[k][1]<<" "<<stridisable[k][2]<<" "<<stridisable[k][3]<<endl;
// 		}			
// 	cout<<"##################################"<<endl;	
		
	PILClose(status);	
	fits_close_file(iFilePed,&status);
	
///////////////////////////////////////////////////	
///////////////////////////////////////////////////	
///////////////////////////////////////////////////	
///////////////////////////////////////////////////	
///////////////////////////////////////////////////


int stridisable1[count_stripdisabled][4];



for (i = 0; i < count_stripdisabled; ++i) {
	stridisable1[i][0] = -1;
	stridisable1[i][1] = -1;	
	stridisable1[i][2] = -1;	
	stridisable1[i][3] = -1;	
	}





	fprintf(fp, "\nCor3901..................... starting CorrCluster\n");
	printf("\nCor3901..................... starting CorrCluster\n");	

	fits_get_colnum(output, 1, "XCLCNFEB", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, febx, NULL, &status);		
		
	fits_get_colnum(output, 1, "XCLCNCHP", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, chipx, NULL, &status);	
	
	fits_get_colnum(output, 1, "XCLCNSTR", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripx, NULL, &status);

	fits_get_colnum(output, 1, "XTOTCHAR", &colnum, &status);	
	fits_read_col(output, TULONG, colnum, 1, 1, nrows*48, NULL, totcharx, NULL, &status);	

	
	fits_get_colnum(output, 1, "XSTRIPC1", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripc1x, NULL, &status);	
	
	fits_get_colnum(output, 1, "XSTRIPC2", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripc2x, NULL, &status);	
	
	fits_get_colnum(output, 1, "XSTRIPC3", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripc3x, NULL, &status);	
	
	fits_get_colnum(output, 1, "XSTRIPC4", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripc4x, NULL, &status);		
	
	fits_get_colnum(output, 1, "XSTRIPC5", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripc5x, NULL, &status);	
			
	fits_get_colnum(output, 1, "XTOTWIDT", &colnum, &status);	
	fits_read_col(output, TLONG, colnum, 1, 1, nrows*48, NULL, totwidtx, NULL, &status);
	
	fits_get_colnum(output, 1, "NCLUST_X", &colnum, &status);	
	fits_read_col(output, TUSHORT, colnum, 1, 1, nrows, NULL, nclustx, NULL, &status);	
	
	
	fits_get_colnum(output, 1, "ZCLCNFEB", &colnum, &status);	
	fits_read_col(output, TINT32BIT, colnum, 1, 1, nrows*48, NULL, febz, NULL, &status);		
		
	fits_get_colnum(output, 1, "ZCLCNCHP", &colnum, &status);	
	fits_read_col(output, TINT32BIT, colnum, 1, 1, nrows*48, NULL, chipz, NULL, &status);	
	
	fits_get_colnum(output, 1, "ZCLCNSTR", &colnum, &status);	
	fits_read_col(output, TINT32BIT, colnum, 1, 1, nrows*48, NULL, stripz, NULL, &status);		

	fits_get_colnum(output, 1, "ZTOTCHAR", &colnum, &status);	
	fits_read_col(output, TULONG, colnum, 1, 1, nrows*48, NULL, totcharz, NULL, &status);	
	
	fits_get_colnum(output, 1, "ZSTRIPC1", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripc1z, NULL, &status);	
	
	fits_get_colnum(output, 1, "ZSTRIPC2", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripc2z, NULL, &status);		
	
	fits_get_colnum(output, 1, "ZSTRIPC3", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripc3z, NULL, &status);		
	
	fits_get_colnum(output, 1, "ZSTRIPC4", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripc4z, NULL, &status);	
	
	fits_get_colnum(output, 1, "ZSTRIPC5", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows*48, NULL, stripc5z, NULL, &status);	
	
	fits_get_colnum(output, 1, "ZTOTWIDT", &colnum, &status);	
	fits_read_col(output, TLONG, colnum, 1, 1, nrows*48, NULL, totwidtz, NULL, &status);
	
	fits_get_colnum(output, 1, "NCLUST_Z", &colnum, &status);	
	fits_read_col(output, TUSHORT, colnum, 1, 1, nrows, NULL, nclustz, NULL, &status);
	



int ncl_errorx = 0;
int ncl_errorz = 0;
int 	nclxtot = 0;
int 	nclztot = 0;	



TH1I *h1_x = new TH1I("h1x","h1x title",18432, 0, 18432);		
TH1I *h1_z = new TH1I("h1z","h1z title",18432, 0, 18432);			
		

	
int cnt1 = 0;
int cnt2 = 0;



for (i = 0; i < nrows; ++i) {
	for (int j = 0; j < nclustx[i]; ++j) {
		if(stripc3x[i][j] > 0) {
			Int_t stripxfill = febx[i][j]*24*128+ chipx[i][j]*128 + stripx[i][j];
			h1_x->Fill(stripxfill);
			}	
		}	
	}	
	
for (i = 0; i < nrows; ++i) {
	for (int j = 0; j < nclustz[i]; ++j) {
		if(stripc3z[i][j] > 0) {
			Int_t stripzfill = febz[i][j]*24*128+ chipz[i][j]*128 + stripz[i][j];
			h1_z->Fill(stripzfill);
			}	
		}	
	}	

	
////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////			
double meanx = h1_x->GetEffectiveEntries()/18432;
double meanz = h1_z->GetEffectiveEntries()/18432;

double sigmax =0;
double  sigmaz =0;

for (i = 0; i < 36863; ++i) {	
	sigmax += (h1_x->GetBinContent(i) - meanx)*(h1_x->GetBinContent(i) - meanx);
	sigmaz += (h1_z->GetBinContent(i) - meanz)*(h1_z->GetBinContent(i) - meanz);	
	}	

sigmax = sqrt(sigmax/18432);
sigmaz = sqrt(sigmaz/18432);
cout<<"meanx "<<meanx<<endl;
cout<<"meanz "<<meanz<<endl;	
cout<<"sigmax "<<sigmax<<endl;
cout<<"sigmaz "<<sigmaz<<endl;	
////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////

double limit_noisex = (sigmax*3) + meanx;
double limit_noisez = (sigmaz*3) + meanz;

cout<<"limit_noisex "<<limit_noisex<<endl;
cout<<"limit_noisez "<<limit_noisez<<endl;		
////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////	


	
for (i = 0; i < 36863; ++i) {	
// 	meanx += h1_x->GetBinContent(i);
	if((h1_x->GetBinContent(i)) > limit_noisex) {
		int ftb    =  0;
		int fb     =  (i-1)/(24*128);	
		int chp    =  int(fmod((double)(i-1),(double)3072)/128.);		
		int strp   =  i - 1 - fb*24.*128. - (chp)*128;	
		
			
// 		cout<<ftb<<" "<<fb<<" "<<chp<<" "<<strp<<" "<<h1_x->GetBinContent(i)<<" "<<i<<endl;	
		
		stridisable1[cnt1][0] = ftb;
		stridisable1[cnt1][1] = fb;	
		stridisable1[cnt1][2] = chp;	
		stridisable1[cnt1][3] = strp;
		cnt1++;	
		}		
	}	
	
for (i = 0; i < 36863; ++i) {	
// 	meanz += h1_z->GetBinContent(i);
	if((h1_z->GetBinContent(i)) >  limit_noisez) {
		int ftb    =  1;
		int fb     =  (i-1)/(24*128);	
		int chp    =  int(fmod((double)(i-1),(double)3072)/128.);		
		int strp   =  i - 1 - fb*24.*128. - (chp)*128;	
// 		cout<<ftb<<" "<<fb<<" "<<chp<<" "<<strp<<" "<<h1_x->GetBinContent(i)<<" "<<i<<endl;
		
		stridisable1[cnt1][0] = ftb;
		stridisable1[cnt1][1] = fb;	
		stridisable1[cnt1][2] = chp;	
		stridisable1[cnt1][3] = strp;
		cnt2++;	
		cnt1++;				
		}		
	}	
	
	
// 	cout<<"meanx noisy = "<<meanx<<endl;
// 	cout<<"meanz noisy = "<<meanz<<endl;
// 	cout<<"sigmax noisy = "<<sigmax<<endl;
// 	cout<<"sigmaz noisy = "<<sigmaz<<endl;	
	cout<<"stripx noisy = "<<cnt1-cnt2-1<<endl;
	cout<<"stripz noisy = "<<cnt2-1<<endl;
	

	
	
				
	for (i = 0; i < nrows; ++i) {
		nclxtot += nclustx[i];
		for (int j = 0; j < 48; ++j) {
// 			if (febx[i][j]==255 || totwidtx[i][j]!=1)
// 				continue;

			

			if (febx[i][j]==255)
				continue;				
			if(febx[i][j] ==  5 && chipx[i][j] == 2 && (stripx[i][j] ==  31 ||  stripx[i][j] ==  32) && totwidtx[i][j]>2)
				continue;	
			

			for(int k = 0; k < count_stripdisabled; ++k){

				if(((stridisable[k][0] == 0 && febx[i][j] ==  stridisable[k][1] && chipx[i][j] ==  stridisable[k][2] && stripx[i][j] ==  stridisable[k][3] ) && totwidtx[i][j]==1) ||  (stridisable1[k][0] == 0 && febx[i][j] ==  stridisable1[k][1] && chipx[i][j] ==  stridisable1[k][2] && stripx[i][j] ==  stridisable1[k][3] )) {								
// 				if((stridisable1[k][0] == 0 && febx[i][j] ==  stridisable1[k][1] && chipx[i][j] ==  stridisable1[k][2] && stripx[i][j] ==  stridisable1[k][3] )) {
				
/*				if(((stridisable[k][0] == 0 && febx[i][j] ==  stridisable[k][1] && chipx[i][j] ==  stridisable[k][2] && stripx[i][j] ==  stridisable[k][3] ) && totwidtx[i][j]==1)) {*/								
					ncl_errorx++;
							
					int z = j;

					while (z<47){					
						febx[i][z] = febx[i][z+1];
						chipx[i][z] = chipx[i][z+1];
						stripx[i][z] = stripx[i][z+1];
						totcharx[i][z] = totcharx[i][z+1];
						totwidtx[i][z] = totwidtx[i][z+1];
						
						stripc1x[i][z] = stripc1x[i][z+1];						
						stripc2x[i][z] = stripc2x[i][z+1];						
						stripc3x[i][z] = stripc3x[i][z+1];						
						stripc4x[i][z] = stripc4x[i][z+1];	
						stripc5x[i][z] = stripc5x[i][z+1];						
						z++;
						if(z==47){
							febx[i][z] = 255;
							febx[i][z] = 255;
							chipx[i][z] = 255;
							stripx[i][z] = 255;
							totcharx[i][z] = 255;
							totwidtx[i][z] = 255;
							
							stripc1x[i][z] = 255;
							stripc2x[i][z] = 255;
							stripc3x[i][z] = 255;
							stripc4x[i][z] = 255;	
							stripc5x[i][z] = 255;	
						
							}
							
						}
					nclustx[i]--;
					
					if (j < 46)
						j--;				
					continue;
					}
	
				}
				 
		
			}	
		}

	i = 0;
	for (i = 0; i < nrows; ++i) {
		nclztot += nclustz[i];
		for (int j = 0; j < 48; ++j) {
// 			if (febz[i][j]==255 || totwidtz[i][j]!=1)
// 				continue;	

			
			if (febz[i][j]==255)
				continue;					
					
			for(int k = 0; k < count_stripdisabled; ++k){
				
				if(((stridisable[k][0] == 1 && febz[i][j] ==  stridisable[k][1] && chipz[i][j] ==  stridisable[k][2] && stripz[i][j] ==  stridisable[k][3] ) && totwidtz[i][j]==1) ||  (stridisable1[k][0] == 1 && febz[i][j] ==  stridisable1[k][1] && chipz[i][j] ==  stridisable1[k][2] && stripz[i][j] ==  stridisable1[k][3] )) {
				
// 				if((stridisable1[k][0] == 1 && febz[i][j] ==  stridisable1[k][1] && chipz[i][j] ==  stridisable1[k][2] && stripz[i][j] ==  stridisable1[k][3] )) {
				
// 				if(((stridisable[k][0] == 1 && febz[i][j] ==  stridisable[k][1] && chipz[i][j] ==  stridisable[k][2] && stripz[i][j] ==  stridisable[k][3] ) && totwidtz[i][j]==1)) {
					ncl_errorz++;

					int z = j;
					while (z<47){
						febz[i][z] = febz[i][z+1];
						chipz[i][z] = chipz[i][z+1];
						stripz[i][z] = stripz[i][z+1];
						totcharz[i][z] = totcharz[i][z+1];
						totwidtz[i][z] = totwidtz[i][z+1];
						
						stripc1z[i][z] = stripc1z[i][z+1];						
						stripc2z[i][z] = stripc2z[i][z+1];						
						stripc3z[i][z] = stripc3z[i][z+1];	
						stripc4z[i][z] = stripc4z[i][z+1];						
						stripc5z[i][z] = stripc5z[i][z+1];					
						z++;
						if(z==47){
							febz[i][z] = 255;
							febz[i][z] = 255;
							chipz[i][z] = 255;
							stripz[i][z] = 255;
							totcharz[i][z] = 255;
							totwidtz[i][z] = 255;
							
							stripc1z[i][z] = 255;
							stripc2z[i][z] = 255;
							stripc3z[i][z] = 255;
							stripc4z[i][z] = 255;	
							stripc5z[i][z] = 255;	
						
							}						
						}
					nclustz[i]--;
					if (j < 46)
						j--;					
					continue;
					}
				}
				 
		
			}	
		}



  	
			
	cout<<"%cluster x fake ="<<(double(ncl_errorx)/double(nclxtot))*100<<endl;
	cout<<"%cluster z fake ="<<(double(ncl_errorz)/double(nclztot))*100<<endl;	
	
				
	fits_get_colnum(output, 1, "XCLCNFEB", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, febx, &status);		
		
	fits_get_colnum(output, 1, "XCLCNCHP", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, chipx, &status);	
	
	fits_get_colnum(output, 1, "XCLCNSTR", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripx, &status);
		
	fits_get_colnum(output, 1, "XSTRIPC1", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripc1x,&status);	
	
	fits_get_colnum(output, 1, "XSTRIPC2", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripc2x,&status);
	
	fits_get_colnum(output, 1, "XSTRIPC3", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripc3x,&status);
	
	fits_get_colnum(output, 1, "XSTRIPC4", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripc4x,&status);
				
	fits_get_colnum(output, 1, "XSTRIPC5", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripc5x,&status);
	
	fits_get_colnum(output, 1, "XTOTCHAR", &colnum, &status);	
	fits_write_col(output, TULONG, colnum, 1, 1, nrows*48, totcharx, &status);	

	fits_get_colnum(output, 1, "XTOTWIDT", &colnum, &status);	
	fits_write_col(output, TLONG, colnum, 1, 1, nrows*48, totwidtx, &status);
	
	fits_get_colnum(output, 1, "NCLUST_X", &colnum, &status);	
	fits_write_col(output, TUSHORT, colnum, 1, 1, nrows, nclustx, &status);		
		
	fits_get_colnum(output, 1, "ZCLCNFEB", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, febz, &status);		
		
	fits_get_colnum(output, 1, "ZCLCNCHP", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, chipz, &status);	
	
	fits_get_colnum(output, 1, "ZCLCNSTR", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripz, &status);		

	fits_get_colnum(output, 1, "ZTOTCHAR", &colnum, &status);	
	fits_write_col(output, TULONG, colnum, 1, 1, nrows*48, totcharz, &status);	
	
	fits_get_colnum(output, 1, "ZSTRIPC1", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripc1z,&status);	
	
	fits_get_colnum(output, 1, "ZSTRIPC2", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripc2z,&status);		
	
	fits_get_colnum(output, 1, "ZSTRIPC3", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripc3z,&status);		
	
	fits_get_colnum(output, 1, "ZSTRIPC4", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripc4z,&status);		
	
	fits_get_colnum(output, 1, "ZSTRIPC5", &colnum, &status);	
	fits_write_col(output, TINT, colnum, 1, 1, nrows*48, stripc5z,&status);		
		
	fits_get_colnum(output, 1, "ZTOTWIDT", &colnum, &status);	
	fits_write_col(output, TLONG, colnum, 1, 1, nrows*48, totwidtz, &status);	

	fits_get_colnum(output, 1, "NCLUST_Z", &colnum, &status);	
	fits_write_col(output, TUSHORT, colnum, 1, 1, nrows, nclustz, &status);		

	
			
	
	if ( status) {
		fprintf(fp, "Cor3901..................... exiting CorrCluster ERROR:");
		printf("Cor3901..................... exiting CorrCluster ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		return status;			
		}			
	else {
		fprintf(fp, "Cor3901..................... exiting CorrCluster STATUS = 0\n\n");
		printf("Cor3901..................... exiting CorrCluster STATUS = 0\n\n");		
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
		
		strcpy(filename, "Cor3901");
		fits_update_key(output, TSTRING, "CREATOR",  filename, NULL, &status);	
		
		strcpy(filename, "39.1");		
		fits_update_key(output, TSTRING, "CREAT_ID",  filename, NULL, &status);	
		
		strcpy(filename, "IASF-MILANO");	
		fits_update_key(output, TSTRING, "ORIGIN",  filename, NULL, &status);		
		
		int timeref = 0;
		fits_get_system_time(filename, &timeref, &status);
		fits_update_key(output, TSTRING, "DATE",  filename, NULL, &status);
		
		fits_update_key(output, TSTRING, "COR_VERS",  corvers, NULL, &status);
				
		}
	return status;
	}

	
int Close3901(int argc, char ** argv, fitsfile * input, fitsfile * output, FILE *fp) {

	int status=0;
	int hdnum = 0;
		
	printf("Cor3901..................... starting Close3901\n");
	fprintf(fp, "Cor3901..................... starting Close3901\n");

	
	
	
	
		
	double tmin=0;
	double tmax=0;
	fits_read_key(output, TDOUBLE, "TSTART", &tmin, NULL, &status );
	fits_read_key(output, TDOUBLE, "TSTOP", &tmax, NULL, &status );	
	if( argc==5) {
		FILE *fp4;
		if(checkfile(argv[3], argv[2], tmin, tmax, (char *)packet) == 0) {
			fp4 = fopen(argv[4], "a+");
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
		fprintf(fp, "Cor3901..................... exiting Close3901 ERROR:");
		printf("Cor3901..................... exiting Close3901 input ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);	
		printf("\n");
		return status;
		}			

		
		
	fprintf(fp, "Cor3901..................... exiting Close3901 STATUS = 0\n\n");
	printf("Cor3901..................... exiting Close3901 STATUS = 0\n\n");		

	
	fprintf(fp,"TIME MINIMUM:  %f\n",tmin);		
	fprintf(fp,"TIME MAXIMUM:  %f\n",tmax);		
	printf("TIME MINIMUM:  %f\n",tmin);		
	printf("TIME MAXIMUM:  %f\n",tmax);
				
	printf("\n\n####################################################\n");
	printf("#########  Task Cor3901......... exiting #########\n");
	printf("####################################################\n\n\n");					
	fprintf(fp, "\n\n####################################################\n");
	fprintf(fp, "#########  Task Cor3901......... exiting #########\n");
	fprintf(fp, "####################################################\n\n\n");		
	
	return status;
	}

int main(int argc,char **argv)
{
int status=0;
fitsfile ** input = new fitsfile* [1];
fitsfile ** output = new fitsfile*[1];

FILE *fp;
fp = fopen("/dev/null", "a+");
status = Open3901(argc, argv, input, output, fp);

if (status ==0)
	status = CorrTime(input[0], output[0], fp);
	
if (status ==0)	
	status = WriteHeader(output[0]);

if (status == 0	)
	status = CorrCluster( argc, argv,output[0], fp);	
	
	
	
	
	
if (status ==0)	
	status = Close3901( argc, argv, input[0], output[0], fp);
	
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
exit(status);
}


