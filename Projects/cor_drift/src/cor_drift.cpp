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
//       Cor_drift.lv1       = grid event report
//
// OUTPUT 
//       Cor_drift.cor       = grid event report
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

#include "cor_drift.h"


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
	
int binarySearchdrift(double * vect, int n, const double v) {

  

  
	int min = -1;
	double value    = 1000000.;
	double valueTmp = 10000000.;

// 	printf("%d\n",n);
	for(int j = 0; j <n; j++) {
		
// 		printf("%f\n",vect[j]);
	  	valueTmp = fabs(vect[j]-v);
		if ( valueTmp < value){
			value =  valueTmp;
			min = j;
			}
  
		}

	if (value  > 6000) {
// 		cout<<"MANCA drift RAVVICINATO "<<vect[min]-v<<endl;	
	  
		printf("MANCA drift RAVVICINATO %f %f %f\n",vect[min]-v,vect[min],v );
		return -1;
		}
	else {
// 	      cout<<"Distanza minima evento - drift "<<vect[min]-v<<endl	;  
	  
	      }
	return min;


	
	}
	


int binarySearchBis(double * vect, int from, int to, const double v, float diff) {
        if (from > to)  
		return -1;
	
        int mid = (from + to) / 2;
// 	if (v < 0)
// 		v = 0;
	double value = fabs(vect[mid]-v);
	
	if (value < diff) {
		if ((vect[mid]-v) < 0) {
// 			cout<<fabs(vect[mid-1]-v)<<" "<<fabs(vect[mid]-v)<<" "<<fabs(vect[mid+1]-v)<<" "<<mid<<" "<<v<<endl;		

			if (fabs(vect[mid+1]-v) < (diff*2)){
	// 			cout<<fabs(vect[mid-1]-v)<<" "<<fabs(vect[mid]-v)<<" "<<fabs(vect[mid+1]-v)<<" "<<mid<<" "<<v<<endl;		
				mid = mid + 1;
				}	
			}
		}	

//   	double value = vect[mid]-v;  
//    	cout<<fabs(vect[mid-1]-v)<<" "<<fabs(vect[mid]-v)<<" "<<fabs(vect[mid+1]-v)<<endl;  	 
	if (value <= diff ) {     // a[mid] == v
// 		cout<<value<<" "<<mid<<" "<<vect[mid]<<endl;
// 		cout<<fabs(vect[mid-1]-v)<<" "<<fabs(vect[mid]-v)<<" "<<fabs(vect[mid+1]-v)<<" "<<v<<" "<<vect[mid]<<endl;    
                return mid;
		}
        else {
		value = vect[mid]-v;
		if (value < 0)     // a[mid] < v
			return binarySearchBis(vect, mid + 1, to, v, diff);
		else
			return binarySearchBis(vect, from, mid - 1, v, diff);
		}
	return -1;
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

	
// cor_drift..................... starting OpenCor_drift
// Enter input file name [COR/PKP044681_1_Cor_drift_000.new.cor] : 
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

	char *Ptr;
	
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
// 	double jdnt = jdn + (ore-12.)/24. + minuti/1440. + secondi/40000.;	
	
	
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
	offsetGiornata = ( 3600. * ore + 60. * minuti + secondi ) / 40000.;
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
	
// 	void *memptrAnt;
// 	size_t memsize,deltasize;
// 
// 	memsize = 1000000000;
// 	deltasize = 1000000;
// 	memptrAnt = malloc(1000000000); 	
// 	fits_create_memfile(&iFile[0], &memptrAnt,   &memsize, deltasize, NULL, &status)	;	
		
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
// 				cout<<	name<<endl;
				fits_movabs_hdu(iFile[0], 2, &hdutype, &status);
				fits_select_rows(iFile[0], iFile[0], expr, &status);
/*				cout<<	expr<<endl;
				cout<<	status<<endl;	*/			
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
// 			cout<<	name<<endl;
			}
// 		cout<<	buffer<<endl;	
		}
	if (find == 0) {
		return 1005;
		}	
	fits_flush_file(iFile[0], &status);	
		
	fclose(fp);
	delete [] name;
	return status;
	}
	
	
	
	
	
	
	
	
int OpenCor_drift(int argc, char ** argv) {
	int status=0;
	char * infile = new char[FLEN_FILENAME];
		
	char * outfile = new char[FLEN_FILENAME];
	char * infiledrift = new char[FLEN_FILENAME];
	int numpar=0;
	const time_t tp = time(NULL);
	char *data=ctime(&tp);	
	double driftcorr = 0;
	fitsfile * input;
	fitsfile * inputdrift;	
	fitsfile * output;
	int hdnum = 0;
	char expr[1024];
	long nrows = 0;
	int colnum = 0;
	printf("\n\nSTART EXECUTION:  %s  ",data);		
	printf("####################################################\n");
	printf("#########  Task cor_drift......... starting  #########\n");
	printf("#########         %s         #########\n", version);
	printf("####################################################\n\n");
	printf("cor_drift..................... starting OpenCor_drift\n");

	status = PILInit(argc,argv);
	PILGetNumParameters(&numpar);

	if ( status != 0 || numpar!=3)
		return 1002;

	PILGetString("infile",infile);
	PILGetString("infiledrift",infiledrift);	
	PILGetString("outfile",outfile);
	PILClose(status);
	


	printf("..........opening input file: %s\n",infile);
	fits_open_file(&input, infile, READONLY, &status);
	if (status) {
		return status;
		}

		
	
	
	fits_read_key(input, TDOUBLE, "DRIFTCORR", &driftcorr, NULL, &status );


	
	if (status != 0) { 
	  
		status = 0;
		int  pkttype=0, pktstype=0;
// 		
		//////////////////////////////////////////////////////////////////////////	
		//////////////////////////////////////////////////////////////////////////		
		//////////////////////////////////////////////////////////////////////////
		
		printf("..........opening input drift file: %s\n",infiledrift);
		
		fits_open_file(&inputdrift, infiledrift, READONLY, &status);
		if (status) {
			return status;
			}
		int orbit = -1;
		fits_read_key(input, TINT, "CONTNUMB", &orbit, NULL, &status );	

	
		fits_movabs_hdu(inputdrift, 2, NULL, &status);	
		fits_get_num_rows(inputdrift, &nrows, &status);


		double * difftime = new double[nrows];
		int * tctype  = new int[nrows];
		int * tcstype = new int[nrows];
		double meandifftime = 0;

		fits_get_colnum(inputdrift, 1, "DIFFTIME", &colnum, &status);	
		fits_read_col(inputdrift, TDOUBLE, colnum,  1, 1, nrows, NULL, difftime, NULL, &status);
		
		fits_get_colnum(inputdrift, 1, "TCTYPE", &colnum, &status);	
		fits_read_col(inputdrift, TINT, colnum,  1, 1, nrows, NULL, tctype, NULL, &status);

		fits_get_colnum(inputdrift, 1, "TCSTYPE", &colnum, &status);	
		fits_read_col(inputdrift, TINT, colnum,  1, 1, nrows, NULL, tcstype, NULL, &status);
		int conta = 0;



		for (int i=0; i<nrows; ++i) {
			if(tctype[i]==33 && tcstype[i]==23){
				meandifftime+=difftime[i]-0.153;
				conta++;
				}
			else if(tctype[i]==33 && tcstype[i]==24){
				meandifftime+=difftime[i]-0.157;
				conta++;
				}
			}
		if (conta == 0) {
		for (int i=0; i<nrows; ++i) {
			if(tctype[i]==36 && tcstype[i]==5){
				meandifftime+=difftime[i]-0.153;
				conta++;
				}
			}
			}
		if (conta == 0) {
			printf("\n\n There are not TC events! EXIT\n");
			printf("cor_drift..................... exiting cor_drift\n\n");	
		
			printf("####################################################\n");
			printf("#########  Task cor_drift......... exiting  #########\n");
			printf("#########   STATUS = %d         #########\n", status);
			printf("####################################################\n\n");	
			exit(-1);
			}

		meandifftime = (meandifftime/(1.*conta));


		if (orbit > 55621) {
			meandifftime = (meandifftime - floor(meandifftime) > 0.5) ? ceil(meandifftime) : floor(meandifftime);
			meandifftime-=0.004;
			}

		cout<<"ORBIT -> "<<orbit<<endl;
		cout<<"DRIFT APPLIED -> "<<meandifftime<<endl;


		//////////////////////////////////////////////////////////////////////////	
		//////////////////////////////////////////////////////////////////////////		
		//////////////////////////////////////////////////////////////////////////				

// 		

		printf("..........opening output file: %s\n",outfile);
		fits_create_file(&output, outfile, &status);
		if (status) {
			return status;
			}		

		fits_movabs_hdu(input, 1, NULL, &status);
		fits_copy_file(input, output, 1, 1, 1, &status);
	
		
		fits_read_key(input, TINT, "PKTTYPE", &pkttype, NULL, &status );
		fits_read_key(input, TINT, "PKTSTYPE", &pktstype, NULL, &status );
	

		fits_get_num_hdus(output, &hdnum, &status);

		fits_movabs_hdu(output, 1, NULL, &status);
		fits_update_key(output, TDOUBLE, "DRIFTCORR",  &meandifftime, "drift (s)", &status);	

		if (meandifftime >= 0)
			sprintf(expr,"TIME %f", -meandifftime);
		else
			sprintf(expr,"TIME + %f", -meandifftime);			


		for (int i=2; i<=hdnum; ++i) {
			
			fits_movabs_hdu(output, i, NULL, &status);
			fits_update_key(output, TDOUBLE, "DRIFTCORR",  &meandifftime, "drift (s)", &status);

			fits_insert_col(output, 1000, "TIMEORI", "1D", &status);
			sprintf(expr,"TIME");
			fits_calculator(output, expr, output, "TIMEORI", NULL, &status);


			if (meandifftime >= 0)
				sprintf(expr,"TIME %f", -meandifftime);
			else
				sprintf(expr,"TIME + %f", -meandifftime);				
			fits_calculator(output, expr, output, "TIME", NULL, &status);


			if ((pkttype==39) && (pktstype==8) && (i==2)) {
// 				cout<<"ENTRATO QUI"<<endl;
				if (meandifftime >= 0)
					sprintf(expr,"BSTART %f", -meandifftime);
				else
					sprintf(expr,"BSTART + %f", -meandifftime);				
				fits_calculator(output, expr, output, "BSTART", NULL, &status);			
			
				if (meandifftime >= 0)
					sprintf(expr,"BSTOP %f", -meandifftime);
				else
					sprintf(expr,"BSTOP + %f", -meandifftime);				
				fits_calculator(output, expr, output, "BSTOP", NULL, &status);
				}
			
			
			
			
			double tstart = 0;
			double tstop = 0;
                        
                        fits_read_key(output, TDOUBLE, "TSTART", &tstart, NULL, &status );
                        fits_read_key(output, TDOUBLE, "TSTOP", &tstop, NULL, &status );  
                        
                        tstart = tstart-meandifftime;
                        tstop  = tstop-meandifftime;    

			WriteTime(output, tstart, tstop);			

                        if (i==hdnum) {
                            fits_movabs_hdu(output, 1, NULL, &status);
                            WriteTime(output, tstart, tstop);
                            }

			}	


		fits_close_file(input, &status);
		fits_close_file(inputdrift, &status);
		fits_close_file(output, &status);

		
		
		}
	else {
	  
		return -1;
		}

		

	if ( status) {
		printf("cor_drift..................... exiting cor_drift input ERROR:");		
		fits_report_error(stdout, status);
		printf("\n");
		return status;					
		}
	else {
		printf("cor_drift..................... exiting cor_drift\n\n");	
		
		printf("####################################################\n");
		printf("#########  Task cor_drift......... exiting  #########\n");
		printf("#########   STATUS = %d         #########\n", status);
		printf("####################################################\n\n");		
		
	
		}
	return status;
	}

int main(int argc,char **argv)
{
int status=0;



status = OpenCor_drift(argc, argv);


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


return status;
}




