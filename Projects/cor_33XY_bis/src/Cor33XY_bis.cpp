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

#include "Cor33XY_bis.h"


int binarySearchTCSP(double * vect, int n, const double v) {

  

  
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
	
int binarySearchOBTUTC(double * vect, int n, const double v) {
  
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
// 		cout<<"MANCA OBTUTC RAVVICINATO "<<vect[min]-v<<endl;	
	  
// 		printf("MANCA OBTUTC RAVVICINATO %f %f %f\n",vect[min]-v,vect[min],v );
		return -1;
		}
	else {
// 	      cout<<"Distanza minima evento - OBTUTC "<<vect[min]-v<<endl	;  
	  
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

	
// Cor33XY_bis..................... starting Open3916
// Enter input file name [COR/PKP044681_1_3916_000.new.cor] : 
// Enter TCSP file name [/storage1/agile/agile2/aux/data/TCSP/2015_12/agile_2015_344_12_08.TCSP] : 
// Enter output file name [!test.fits] : 
	
char * filetemp(char * name, double t1, double t2){  

 	int first = rand() % 32767;
	int second = rand() % 32767;
 	int third = rand() % 32767;	
	
	sprintf(name,"/tmp/filetemp_%d_%d_%i_%i_%i", (int)t1, (int)t2, first,second,third);
	
	while (isfile(name) == 1){
 		int first = rand() % 32767;
		int second = rand() % 32767;
 		int third = rand() % 32767;		
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
	double jdnt = jdn + (ore-12.)/24. + minuti/1440. + secondi/40000.;	
	
	
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

	
	
	
	
int addfileTCSP(fitsfile ** iFile, char * fileList, char * expr, double tmin, double tmax){
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
				//cout<<	name<<endl;
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
				//cout<<	name<<endl;
				fits_movabs_hdu(tempFits, 2, &hdutype, &status);					
				fits_select_rows(tempFits, iFile[0], expr, &status);					
				fits_close_file(tempFits, &status);		
				find++;									
				}
 	//		cout<<	name<<endl;
			}
// 		cout<<	buffer<<endl;	
		}
//	cout<<"FIND TROVATI = "<<find<<endl;
	if (find == 0) {
		return 1005;
		}	
	fits_flush_file(iFile[0], &status);	
		
	fclose(fp);
	delete [] name;
	return status;
	}
	
	
	
	
	
	
	
	
int Open3916(int argc, char ** argv, fitsfile ** input, fitsfile ** output, char * infileTCSP, char * infileOBTUTC) {
	int status=0,pkttype=0,pktstype=0;
	char * infile = new char[FLEN_FILENAME];
		
	char * outfile = new char[FLEN_FILENAME];
	int numpar=0;
	const time_t tp = time(NULL);
	char *data=ctime(&tp);	
	

	
	printf("\n\nSTART EXECUTION:  %s  ",data);		
	printf("####################################################\n");
	printf("#########  Task Cor33XY_bis......... starting  #########\n");
	printf("#########         %s         #########\n", version);
	printf("####################################################\n\n");
	printf("Cor33XY_bis..................... starting Open3916\n");

	status = PILInit(argc,argv);
	PILGetNumParameters(&numpar);

	if ( status != 0 || numpar!=4)
		return 1002;

	PILGetString("infile",infile);
	PILGetString("infileTCSP",infileTCSP);
	PILGetString("infileOBTUTC",infileOBTUTC);	
	PILGetString("outfile",outfile);

	if (strcmp(infile,outfile)==0) {
		status = 101;
		return status;
		}

	printf("..........opening input file: %s\n",infile);
	fits_open_file(input, infile, READONLY, &status);
	if (status) {
		return status;
		}

	fits_read_key(input[0], TINT, "PKTTYPE", &pkttype, NULL, &status );
	fits_read_key(input[0], TINT, "PKTSTYPE", &pktstype, NULL, &status );
	
	
	if (pkttype!=33 || pktstype!=0) {
		printf("error packet type [33]   %d!!\n",pkttype );
		printf("error packet subtype [0] %d!!\n",pktstype );
		fits_close_file(input[0], &status);
		return 1001;
		}

	printf("..........opening output file: %s\n",outfile);
	fits_create_file(&output[0], outfile, &status);
	if (status) {
		return status;
		}		

	PILClose(status);
	
	

	
	fits_movabs_hdu(input[0], 1, NULL, &status);
	fits_copy_file(input[0], output[0], 1, 1, 1, &status);


	delete []infile;
	delete []outfile;		
		
	if ( status) {
		printf("Cor33XY_bis..................... exiting Cor33XY_bis input ERROR:");		
		fits_report_error(stdout, status);
		printf("\n");
		return status;					
		}
	else {
		printf("Cor33XY_bis..................... exiting Cor33XY_bis\n");		
	
		}
	return status;
	}






int CorrTime(fitsfile * output, char * infileTCSP, char * infileOBTUTC) {

	int status = 0;
	printf("\nCor33XY_bis..................... starting CorrTime\n");

 	long nrows = 0;
	int colnum = 0;
		

	double tstart = 0;
	double tstop = 0;
	char buffer[2048];
	double timediffobhreal = 0;
	char expr[1024];
	char expr2[1024];	

///////////////////////////////////////////////////////////////////////////////////////////	
	
	
	fits_movabs_hdu(output, 1, NULL, &status);
	fits_update_key(output, TSTRING, "FILETCSP",  infileTCSP, "TCSP Filename", &status);	

	int orbit = 0;
	fits_read_key(output, TINT, "CONTNUMB", &orbit, NULL, &status );	
	

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
		
	fits_movabs_hdu(output, 2, NULL, &status);	

	fits_select_rows(output, output, "TMTYPE==33 && TMSTYPE==1 && TCSTYPE != 28", &status);

	fits_get_num_rows(output, &nrows, &status);
	double * time = new double[nrows];
	int * tctype = new int[nrows];
	int * tcstype = new int[nrows];

	fits_get_colnum(output, 1, "TIME", &colnum, &status);	
	fits_read_col(output, TDOUBLE, colnum,  1, 1, nrows, NULL, time, NULL, &status);	
	fits_get_colnum(output, 1, "TCTYPE", &colnum, &status);	
	fits_read_col(output, TINT, colnum,  1, 1, nrows, NULL, tctype, NULL, &status);
	fits_get_colnum(output, 1, "TCSTYPE", &colnum, &status);	
	fits_read_col(output, TINT, colnum,  1, 1, nrows, NULL, tcstype, NULL, &status);	


	tstart=time[0];
        tstop=time[nrows-1];

//	for(int k=0;k<nrows;k++)printf("%f %d %d \n",time[k],tctype[k],tcstype[k]);	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
 	




	char * tempfilename = new char[FLEN_FILENAME];	
	char * tempfilename2 = new char[FLEN_FILENAME];	
		
	
	filetemp(tempfilename, tstart, tstop);



	filetemp(tempfilename2, tstart, tstop);	


	long nrowsTCSP = 0; 


	fitsfile ** inputTCSP    = new fitsfile*[1];;
	fitsfile ** inputOBTUTC = new fitsfile*[1];;	
	
	fits_create_file(&inputTCSP[0], tempfilename, &status);
	fits_create_file(&inputOBTUTC[0], tempfilename2, &status);	
	
	if (status) {
		return status;
		}	
		

	sprintf(expr, "TCTYPE!=28", tstart-10000, tstop+10000);	

 	status = addfileTCSP(&inputTCSP[0], infileTCSP, expr, tstart-500000, tstop+2000000);

	if (status == 1005) {
		printf("ERROR!! TCSP DOESN'T FOUND\n\n\n");

		printf("\n\n####################################################\n");
		printf("#########  Task Cor33XY_bis......... exiting #########\n");
		printf("####################################################\n\n\n");	
		return status;
		}


	fits_movabs_hdu(inputTCSP[0], 2, NULL, &status);	
	sprintf(expr, "TIME >= %f && TIME <= %f", tstart-500, tstop+500);

	fits_select_rows(inputTCSP[0], inputTCSP[0],  expr, &status);

	fits_get_num_rows(inputTCSP[0], &nrowsTCSP, &status);
	

	
	double * timeTCSP = new double[nrowsTCSP];  
	int * tctypeTCSP = new int[nrowsTCSP];
	int * tcstypeTCSP = new int[nrowsTCSP];

	fits_get_colnum(inputTCSP[0], 1, "TIME", &colnum, &status);
	
	fits_read_col(inputTCSP[0], TDOUBLE, colnum,  1, 1, nrowsTCSP, NULL, timeTCSP, NULL, &status);
	fits_get_colnum(inputTCSP[0], 1, "TCTYPE", &colnum, &status);
	
	fits_read_col(inputTCSP[0], TINT, colnum,  1, 1, nrowsTCSP, NULL, tctypeTCSP, NULL, &status);
	fits_get_colnum(inputTCSP[0], 1, "TCSTYPE", &colnum, &status);

	fits_read_col(inputTCSP[0], TINT, colnum,  1, 1, nrowsTCSP, NULL, tcstypeTCSP, NULL, &status);


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	sprintf(expr, "TIME >= %f && TIME <= %f", tstart-86400, tstop+864000);
	status = addfileTCSP(&inputOBTUTC[0], infileOBTUTC, expr, tstart-86400, tstop+86400);	
	
	long nrowsOBTUTC = 0;
	
	fits_movabs_hdu(inputOBTUTC[0], 2, NULL, &status);	
	fits_get_num_rows(inputOBTUTC[0], &nrowsOBTUTC, &status);

// 	cout<<"nrowsOBTUTC "<<nrowsOBTUTC<<endl;	
	
	double * timeOBTUTC = new double[nrowsOBTUTC];
	double * timeDIFF   = new double[nrowsOBTUTC];	
	int * orbitOBTUTC   = new int[nrowsOBTUTC];	


	
	fits_get_colnum(inputOBTUTC[0], 1, "TIME", &colnum, &status);					
	fits_read_col(inputOBTUTC[0], TDOUBLE, colnum,  1, 1, nrowsOBTUTC, NULL, timeOBTUTC, NULL, &status);	
	fits_get_colnum(inputOBTUTC[0], 1, "TIME_DIFF", &colnum, &status);					
	fits_read_col(inputOBTUTC[0], TDOUBLE, colnum,  1, 1, nrowsOBTUTC, NULL, timeDIFF, NULL, &status);
	fits_get_colnum(inputOBTUTC[0], 1, "ORBIT", &colnum, &status);	
	fits_read_col(inputOBTUTC[0], TINT, colnum,  1, 1, nrowsOBTUTC, NULL, orbitOBTUTC, NULL, &status);
	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	




	fits_insert_col(output, 1, "DIFFTIME", "1D", &status);	
	fits_insert_col(output, 2, "TIME_TCSP", "1D", &status);	
	fits_insert_col(output, 3, "DATE_TCSP", "32A", &status);	
	fits_insert_col(output, 4, "ORBIT", "1J", &status);	
// 	cout<<nrowsOBTUTC<<endl;

	fits_flush_file(output, &status);
	fits_get_num_rows(output, &nrows, &status);
	int numeribuoni = 0;
	char * linea[128];
	char * datacal = new char [32];	
		
	for (int i=0; i<nrows; ++i) {
	  
		//int it =binarySearchTCSP(timeTCSP, nrowsTCSP, time[i]);
		int it=-1;
		int it2 =-1; 
		int diffvalue = 200000;
	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	


		for (int k=0; k<nrowsTCSP; ++k) {	

			if((tctypeTCSP[k] == tctype[i]) &&(tcstypeTCSP[k]==tcstype[i])) {
				if ((abs(timeTCSP[k]-time[i])) < 500 && (diffvalue > abs(timeTCSP[k]-time[i]))) {
					it=k;
					diffvalue = abs(timeTCSP[k]-time[i]);
					}
				
				}

			}


		if (it == -1) {
			printf("TIME %f [%d] --- doesn't found in the TCSP index file. TCTYPE=%d TCSTYPE=%d\n\n",time[i],i,tctype[i],tcstype[i]);
			continue;
			}
		else {
			printf("TIME %f [%d] --- TCTYPE=%d TCSTYPE=%d\n",time[i],i,tctype[i],tcstype[i]);
			}
	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	



		for (int k=0; k<nrowsOBTUTC; ++k) {
		//	printf("\ntempo cercato %f  --- tempo trovato nel OBTUTC %f -- %d\n",time[i],timeOBTUTC[k],k);	
			if (time[i] > timeOBTUTC[k]) { 
				it2 = k;
				}
			}

		if (it2 == -1 || it2==0 || (it2 == nrowsOBTUTC-1)) {
			printf("Time %f [%d] --- doesn't found in the OBTUTC index file. TCTYPE=%d TCSTYPE=%d\n\n",time[i],i,tctype[i],tcstype[i]);
			continue;
			}
//	cout<<it2<<" ---- "<<nrowsOBTUTC<<endl;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	



		timediffobhreal = (((timeDIFF[it2+1] -timeDIFF[it2])/(timeOBTUTC[it2+1]-timeOBTUTC[it2]))*( time[i] - timeOBTUTC[it2]) + timeDIFF[it2]);


		if (timediffobhreal == 0) {
			printf("Time %f [%d] --- doesn't found in the OBTUTC index file. TCTYPE=%d TCSTYPE=%d\n\n",time[i],i,tctype[i],tcstype[i]);
			continue;
			}		
		if (timediffobhreal> 15 || timediffobhreal < -15) {
			printf("Error! real time - OBDH too big....skip!\n\n");
			continue;
			}			



		double drift = (time[i]+timediffobhreal)-timeTCSP[it];
		


		printf("TC DRIFT = %f\n",drift);				
		printf("PDHU TIME = %f \nDelay UTC-OBDH = %f \nTCSP TIME = %f \n\n",time[i], timediffobhreal, timeTCSP[it]);
		

		fits_get_colnum(output, 1, "TIME_TCSP", &colnum, &status);
		fits_write_col(output, TDOUBLE, colnum, i+1, 1, 1, &timeTCSP[it], &status);	
		
		
		fits_get_colnum(output, 1, "DIFFTIME", &colnum, &status);
		fits_write_col(output, TDOUBLE, colnum, i+1, 1, 1, &drift, &status);	

		fits_get_colnum(output, 1, "ORBIT", &colnum, &status);
		fits_write_col(output, TINT32BIT, colnum, i+1, 1, 1, &orbit, &status);			
	
		fits_get_colnum(inputTCSP[0], 1, "DATE", &colnum, &status);					
		fits_read_col(inputTCSP[0], TSTRING, colnum,  it+1, 1, 1, NULL, &datacal, NULL, &status);		

		fits_get_colnum(output, 1, "DATE_TCSP", &colnum, &status);
		fits_write_col(output, TSTRING, colnum, i+1, 1, 1, &datacal, &status);		
	
		numeribuoni++;
		}
	cout<<"##################################"<<endl;		
	cout<<"Good Events = "<<numeribuoni<<" / "<< nrows<<endl;	
	cout<<"##################################"<<endl<<endl<<endl;	
	if (numeribuoni==0) {

		cout<<"Drift time not calculated!!"<<endl;	

		fits_delete_file(inputTCSP[0],&status);

		fits_delete_file(inputOBTUTC[0],&status);

// 		fits_delete_file(output,&status);
		status=-1;
		return status;
		}		
	

	fits_select_rows(output, output, "ORBIT > 1100", &status);


	fits_delete_file(inputTCSP[0],&status);
	fits_delete_file(inputOBTUTC[0],&status);	

	

	fits_get_num_rows(output, &nrows, &status);
	fits_get_colnum(output, 1, "TIME", &colnum, &status);	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, 1, NULL, &tstart, NULL, &status);
	fits_read_col(output, TDOUBLE, colnum, nrows, 1, 1, NULL, &tstop, NULL, &status);

	WriteTime(output, tstart, tstop);		
	
	delete [] time;	
	delete [] tempfilename;	
	delete [] tempfilename2;		
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	
	if ( status) {
		printf("Cor33XY_bis..................... exiting CorrTime ERROR:");		
		fits_report_error(stdout, status);
		printf("\n");
		printf("\n");		
		return status;		
		}
	else {
		printf("Cor33XY_bis..................... exiting CorrTime STATUS = 0\n\n");		
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
		
		strcpy(filename, "Cor33XY_bis");
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

int Cor33XY_bis(int argc, char ** argv, fitsfile * input, fitsfile * output, char * infileTCSP) {

	int status=0;
	int hdnum = 0;
	
	printf("Cor33XY_bis..................... starting Cor33XY_bis\n");	

	fits_movabs_hdu(output, 1, NULL, &status);	
	double tmin=0;
	double tmax=0;
	fits_read_key(output, TDOUBLE, "TSTART", &tmin, NULL, &status );
	fits_read_key(output, TDOUBLE, "TSTOP", &tmax, NULL, &status );	
	if( argc==6) {
		FILE *fp4;

		if(checkfile(argv[5], &argv[4][1], tmin, tmax, (char *)packet) == 0) {
			fp4 = fopen(argv[5], "a+");
			if (argv[4][0] == '!')
				fprintf(fp4,"%s  %f  %f %s\n", &argv[4][1], tmin, tmax, packet);
			else
				fprintf(fp4,"%s  %f  %f %s\n", &argv[4][1], tmin, tmax, packet);
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
		printf("Cor33XY_bis..................... exiting Cor33XY_bis input ERROR:");		
		fits_report_error(stdout, status);	
		printf("\n");
		return status;
		}			


			
	printf("Cor33XY_bis..................... exiting Cor33XY_bis STATUS = 0\n\n");		

	
	printf("TIME MINIMUM:  %f\n",tmin);		
	printf("TIME MAXIMUM:  %f\n",tmax);	
			
	printf("\n\n####################################################\n");
	printf("#########  Task Cor33XY_bis......... exiting #########\n");
	printf("####################################################\n\n\n");					
	
	return status;
	}

int main(int argc,char **argv)
{
int status=0;

fitsfile ** input = new fitsfile*[1];
fitsfile ** output = new fitsfile*[1];

char * infileTCSP = new char[FLEN_KEYWORD];
char * infileOBTUTC = new char[FLEN_KEYWORD];


status = Open3916(argc, argv, input, output, infileTCSP,infileOBTUTC);

// cout<<infileTCSP<<endl;

 if (status ==0)
 	status = CorrTime(output[0],infileTCSP,infileOBTUTC);
 if (status ==0)	
	WriteHeader(output[0]);

	
// if (status ==0)
// 	status = CorrAtt(output[0]);

if (status ==0)
	status = Cor33XY_bis(argc, argv, input[0], output[0],infileTCSP);

if (status) {
  

	fits_delete_file(output[0],&status);
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

delete []input;
delete []output;
return status;
}



