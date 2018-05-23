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

#include "Cor_obtutc.h"


int binarySearchSOE(double * vect, int n, const double v) {

  

  
	int min = -1;
	double value    = 1000000.;
	double valueTmp = 10000000.;

	
	for(int j = 0; j <n; j++) {
	  
	  	valueTmp = fabs(vect[j]-v);
		if ( valueTmp < value){
			value =  valueTmp;
			min = j;
			}
  
		}
	
	return min;


	
	}
	


















char* Trim(char* word){

	int l = strlen(word);
	char* beginPtr = &word[l-1];
	while (isspace(*beginPtr)){
		*beginPtr = '\0'; 
		--beginPtr;
		}
	beginPtr = word;		
	while (isspace(*beginPtr))
		++beginPtr;
		
		
	if (!strlen(beginPtr))
		beginPtr = 0;
	return beginPtr;
	}

	
// Cor_obtutc..................... starting Open3916
// Enter input file name [COR/PKP044681_1_3916_000.new.cor] : 
// Enter SOE file name [/storage1/agile/agile2/aux/data/SOE/2015_12/agile_2015_344_12_08.SOE] : 
// Enter output file name [!test.fits] : 
	
char * filetemp(char * name, double t1, double t2){  

	time_t seconds;
	seconds = time(&seconds);
	srand(clock());
 	int first = rand() % 32767;
// 	char name[2048];
	int second = rand() % 32767;
	srand(seconds);
 	int third = rand() % 32767;		
	sprintf(name,"/tmp/filetemp_%d_%d_%i_%i_%i", (int)t1, (int)t2, first,second,third);
	
	while (isfile(name) == 1){
		srand(clock());		
		first = rand() % 32767;
		second = rand() % 32767;
		srand(seconds);		
		third = rand() % 32767;		
		sprintf(name,"/tmp/filetemp_%d_%d_%i_%i_%i", (int)t1, (int)t2, first,second,third);
		}
			
	return name;
}



double greg_to_mjd(char * value) { 

// 2013-311-10:59:21.492
//25-07-14-14-15-35-310
//2014 7 25 12 55 48.886

	char *Ptr, *Co = NULL;
	
	char bufferTmp[2048];
	strcpy(bufferTmp, value);
	
	Ptr = strtok(bufferTmp,"-");
	double giorno = atof(Ptr);

	Ptr = strtok(NULL,"-");	
	double mese = atof(Ptr);
	
	Ptr = strtok(NULL,"-");	
	double anno = atof(Ptr)+2000;
	
	Ptr = strtok(NULL,"-");
	double ore =  atof(Ptr);
	
	
	/////////DA CAMBIARE!!!//////////////////////
	if (ore == -2) {ore = 22; giorno = giorno -1;} 
	if (ore == -1) {ore = 23; giorno = giorno -1;} 	
	/////////DA CAMBIARE!!!//////////////////////
	
	Ptr = strtok(NULL,"-");
	double minuti =  atof(Ptr);
	
	Ptr = strtok(NULL,"-");
	double secondi =  atof(Ptr);	

	Ptr = strtok(NULL,"-");
	double msecondi =  atof(Ptr);		

	secondi = secondi + msecondi/1000000.;
	
// 	cout<<anno<<" "<<mese<<" "<<giorno<<" "<<ore<<" "<<minuti<<" "<<secondi<<endl;
	

	long aa = (14 - mese)/12;
	long yy = anno + 4800 - aa;
	long mm = mese + 12*aa - 3;

	double jdn = giorno + (153 * mm + 2)/5 + 365*yy + yy/4 - yy/100 + yy/400 - 32045 - 0.5;
	double jdnt = jdn + (ore-12.)/24. + minuti/1440. + secondi/86400.;	
	
	
	double a,b,c,d,SPECTRUMGiuliana, offsetGiornata;
	if (mese == 1 || mese == 2) {
		anno = anno - 1;
		mese = mese + 12;
		}



	if (anno  < 1582) {
		a = 0;
		b = 0;
		}
	else {
		a = floor( anno / 100. );
		b = 2 - a + floor( a / 4. );
		}
// 	
	c = floor( 365.25 * anno );
	d = floor( 30.6001 *(mese+1));
// 	
// 	// attualmente si riferisce all'inizio della giornata, in più viene sottratto
// 	// un reset che permette di avere una migliore definizione della parte decimale
// 	// (limiti rappresentativi dipendenti dalle strutture dati); il reset indicato
// 	// si riferisce al 1 gennario 1900, ma è una scelta mia arbitraria
// 	
	SPECTRUMGiuliana = b + c + d + giorno + 1720994.50; 
// 	
// 	// ora la definizione del punto della gionata dipende dall'utilizzo, e se ne sceglie
// 	// solamente una delle seguenti
// 	
	// 1. definizione all'ora (non mi interessano i minuti e i secondi)
	offsetGiornata = ore / 24.;
	
	// 2. definizione al minuto (non mi interessano i secondi)
	offsetGiornata = ( 60. * ore + minuti ) / 1440.;
	
	// 3. definizione al secondo
	offsetGiornata = ( 3600. * ore + 60. * minuti + secondi ) / 86400.;
// 	
// 	// SPECTRUM giuliana completa
	double mjd = SPECTRUMGiuliana + offsetGiornata -2400000.5;
// 	printf("%f\n",mjd);
// 	printf("%f\n",SPECTRUMGiuliana + offsetGiornata -2400000.5);
// 	printf("%.8f\n",jdnt -2400000);
	
	return mjd;
	}

	
	
	
	
int addfileSOE(fitsfile ** iFile, char * fileList, char * expr, double tmin, double tmax){
	char buffer[2048];
	int status = 0;
	FILE *fp = fopen(fileList, "r");
	if(!fp) {
		printf("Errore in apertura file '%s'\n", fileList);
		return 104;	
		}
		
	
	int hdutype = 0;
	int  find = 0;
	char *name=new char[FLEN_FILENAME];

	double t1 = 0, t2 = 0;
	
	void *memptrAnt;
	size_t memsize,deltasize;

	memsize = 1000000000;
	deltasize = 1000000;
	memptrAnt = malloc(1000000000); 	
	fits_create_memfile(&iFile[0], &memptrAnt,   &memsize, deltasize, NULL, &status)	;	
		
	while (fgets(buffer , 2048, fp)) {
		sscanf(buffer, "%s %lf %lf", name, &t1, &t2);

		if ( ((t2 > tmin && t2 < tmax)  || (t1 > tmin && t1 < tmax)  || (t1 <= tmin && t2 >= tmax))) {
			fitsfile *tempFits;
			if (find == 0) {	
							
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}	
				fits_copy_file(tempFits, iFile[0], 1, 1, 1, &status);
				fits_movabs_hdu(iFile[0], 2, &hdutype, &status);
				fits_select_rows(iFile[0], iFile[0], expr, &status);
				fits_close_file(tempFits, &status);							
				find++;		
				}
			else {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}
				fits_movabs_hdu(tempFits, 2, &hdutype, &status);					
				fits_select_rows(tempFits, iFile[0], expr, &status);					
				fits_close_file(tempFits, &status);		
				find++;									
				}
			cout<<	name<<endl;
			}
			
		}
	if (find == 0) {
		return 1005;
		}	

		
	fclose(fp);
	delete [] name;
	return status;
	}
	
	
	
	
	
	
	
	
int Open3916(int argc, char ** argv, fitsfile ** output, char * infileOBTUTC) {
	int status=0,pkttype=0,pktstype=0;
		
	char * outfile = new char[FLEN_FILENAME];
	int numpar=0;
	const time_t tp = time(NULL);
	char *data=ctime(&tp);	
	

	
	printf("\n\nSTART EXECUTION:  %s  ",data);		
	printf("####################################################\n");
	printf("#########  Task Cor_obtutc......... starting  #########\n");
	printf("#########         %s         #########\n", version);
	printf("####################################################\n\n");
	printf("Cor_obtutc..................... starting Open3916\n");

	status = PILInit(argc,argv);
	PILGetNumParameters(&numpar);

	if ( status != 0 || numpar!=2)
		return 1002;

	PILGetString("infileOBTUTC",infileOBTUTC);	
	PILGetString("outfile",outfile);


	

	PILClose(status);
	
	printf("..........opening output file: %s\n",outfile);
	fits_create_file(&output[0], outfile, &status);
	if (status) {
		return status;
		}	
		
	int tfields = 4;       /* table will have 20 columns */
		
	char *ttype[] = { "TIME","TIME_DIFF","DATE","ORBIT"};
	char *tform[] = { "1D","1D","32A","1J"};
	char *tunit[] = { "s","s","",""};
	
	
	fits_create_tbl( output[0], BINARY_TBL, 1, tfields, ttype, tform,   tunit, "OBTUTC", &status);
	delete []outfile;		
		
	
	
	if ( status) {
		printf("Cor_obtutc..................... exiting Cor_obtutc input ERROR:");		
		fits_report_error(stdout, status);
		printf("\n");
		return status;					
		}
	else {
		printf("Cor_obtutc..................... exiting Cor_obtutc\n");		
	
		}
	return status;
	}

int CorrTime(fitsfile * output, char * infileOBTUTC) {

	int status = 0;
	printf("\nCor_obtutc..................... starting CorrTime\n");

 	long nrows = 0;
	int colnum = 0;
	double * time = NULL;	

	double tstart = 0;
	double tstop = 0;
	char buffer[2048];
	char buffertmp[2048];
	char buffertmp2[2048];
///////////////////// ESTRAE LA DIFF TRA OBDH E REALTà//////////////////////////////////
	char *Ptr;
	FILE * fp;
	fp = fopen(infileOBTUTC, "r");
	if (!fp) {
		printf("COULD NOT OPEN THE NAMED FILE:  %s\n", infileOBTUTC);
		return 1003;
		}
// 	
// 	float bw = 0;
	int orbit = 0;
	int index = 0;
	int i = 0;
	int year = 0;
	int month = 0;
	int day = 0;	
	int hour = 0;
	int minute = 0;
	int second = 0;
	double msecond = 0;
	char bufferData[2048];
	char bufferDataTMP[2048];
	
		fgets(buffer , 2048, fp);	
		fgets(buffer , 2048, fp);
		Ptr = strtok(buffer,":");
		Ptr = strtok(NULL,":");	
		orbit=atoi(Ptr);
		cout<<"ORBIT "<<orbit<<endl;		
	while (fgets(buffer , 2048, fp)) {

		fgets(buffer , 2048, fp);		

		strcpy(buffertmp,buffer);
		strcpy(buffertmp2,buffer);	

		Ptr = NULL;
		Ptr = strtok(buffertmp,"-");
		double checktest = atof(Ptr);
		 
		if ((checktest > 2006 && checktest < 2020)) {
		  

// 			cout<<"BUFFER "<<buffer<<endl;
			
			Ptr = strtok(buffer,">\t'");
			
// 			cout<<Ptr<<endl;
			fits_get_colnum(output, 1, "DATE", &colnum, &status);
			fits_write_col(output, TSTRING, colnum, i+1, 1, 1, &Ptr, &status);

// 		        2008-12-10T23:15:21.293283	1228950913.057000	1228950921.295200	1228950921.295100   
			strcpy(bufferDataTMP,Ptr);
// 			cout<<bufferDataTMP<<endl;
			year = atoi(strtok(bufferDataTMP,"-"));
			month = atoi(strtok(NULL,"-"));
			day = atoi(strtok(NULL,"T"));
			
			hour = atoi(strtok(NULL," :"));
			minute = atoi(strtok(NULL,":"));
			second = atoi(strtok(NULL,":."));			
			msecond = atof(strtok(NULL,":."));				
			
			sprintf(bufferData,"%02d-%02d-%02d-%02d-%02d-%02d-%d", day,  month, year-2000, hour, minute, second, (int)msecond);
// 			cout<<bufferData<<endl;
			
 			double mjd = greg_to_mjd(bufferData);	
// 			cout<<bufferData<<endl;
// 
			
			
//  			
 			double timereal = (mjd-53005.)*86400.;	

			
			Ptr = strtok(buffertmp2,">\t'");
			Ptr = strtok(NULL," \t'");

			double timeobdh = atof(Ptr);
			Ptr = strtok(NULL," \t'");
			Ptr = strtok(NULL," \t'");
			double timeobdhreal = atof(Ptr);

			

			double timetest = timeobdh - timeobdhreal;
			double timediffobhreal = timeobdh - timeobdhreal;
			
		      if (timediffobhreal > 50 || timediffobhreal  < -50) {
			cout<<"trovato "<<timediffobhreal<<endl;
			continue;
		      }
		      fits_get_colnum(output, 1, "TIME", &colnum, &status);
		      fits_write_col(output, TDOUBLE, colnum, i+1, 1, 1, &timereal, &status);	
		      fits_get_colnum(output, 1, "TIME_DIFF", &colnum, &status);
		      fits_write_col(output, TDOUBLE, colnum, i+1, 1, 1, &timediffobhreal, &status);	
		      fits_get_colnum(output, 1, "ORBIT", &colnum, &status);
		      fits_write_col(output, TINT32BIT, colnum, i+1, 1, 1, &orbit, &status);			      
		      
		
			i++;
			}
	  
		}

	fclose(fp);
	
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_get_num_rows(output, &nrows, &status);
	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, 1, NULL, &tstart, NULL, &status);
	fits_read_col(output, TDOUBLE, colnum, nrows, 1, 1, NULL, &tstop, NULL, &status);
	
	WriteTime(output, tstart, tstop);

	
	
	
	delete [] time;	

	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	
	if ( status) {
		printf("Cor_obtutc..................... exiting CorrTime ERROR:");		
		fits_report_error(stdout, status);
		printf("\n");
		printf("\n");		
		return status;		
		}
	else {
		printf("Cor_obtutc..................... exiting CorrTime STATUS = 0\n\n");		
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
		
		strcpy(filename, "Cor_obtutc");
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

int Close3916(int argc, char ** argv, fitsfile * output) {

	int status=0;
	int hdnum = 0;
	
	printf("Cor_obtutc..................... starting Close3916\n");	
	
	fits_movabs_hdu(output, 1, NULL, &status);	
	double tmin=0;
	double tmax=0;
	fits_read_key(output, TDOUBLE, "TSTART", &tmin, NULL, &status );
	fits_read_key(output, TDOUBLE, "TSTOP", &tmax, NULL, &status );	
// 	cout<<argv[2]<<endl;
	if( argc==4) {
		FILE *fp4;
		if(checkfile(argv[3], argv[2], tmin, tmax, (char *)packet) == 0) {
			fp4 = fopen(argv[3], "a+");
// 			cout<<argv[2]<<endl;
			if (argv[2][0] == '!')
				fprintf(fp4,"%s  %f  %f %s\n", &argv[2][1], tmin, tmax, packet);
			else
				fprintf(fp4,"%s  %f  %f %s\n", argv[2], tmin, tmax, packet);
			fclose(fp4);
			}
		}
		
		
	
		
		
	

	fits_get_num_hdus(output, &hdnum, &status);
	for (int i=1; i<=hdnum; ++i) {
		fits_movabs_hdu(output, i, NULL, &status);
		fits_write_chksum(output,&status);
		}
	fits_close_file(output, &status);
				

	
	if ( status) {
		printf("Cor_obtutc..................... exiting Close3916 input ERROR:");		
		fits_report_error(stdout, status);	
		printf("\n");
		return status;
		}			


			
	printf("Cor_obtutc..................... exiting Close3916 STATUS = 0\n\n");		

	
	printf("TIME MINIMUM:  %f\n",tmin);		
	printf("TIME MAXIMUM:  %f\n",tmax);	
			
	printf("\n\n####################################################\n");
	printf("#########  Task Cor_obtutc......... exiting #########\n");
	printf("####################################################\n\n\n");					
	
	return status;
	}

int main(int argc,char **argv)
{
int status=0;


fitsfile ** output = new fitsfile*[1];

char * infileOBTUTC = new char[FLEN_KEYWORD];


status = Open3916(argc, argv, output, infileOBTUTC);

// cout<<infileSOE<<endl;

 if (status ==0)
 	status = CorrTime(output[0],infileOBTUTC);
 if (status ==0)	
	WriteHeader(output[0]);

	
// if (status ==0)
// 	status = CorrAtt(output[0]);

if (status ==0)
	status = Close3916(argc, argv, output[0]);

if (status) {
	if(status >=1000) {
		getError(status);
		}	
	else {
		fits_report_error(stderr, status);
		}
	}
else {	
	printf("STATUS %d\n",status);
	}	

delete []output;
return status;
}




