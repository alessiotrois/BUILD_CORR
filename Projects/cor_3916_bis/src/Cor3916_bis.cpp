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

#include "Cor3916_bis.h"

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

	
// Cor3916_bis..................... starting Open3916
// Enter input file name [COR/PKP044681_1_3916_000.new.cor] : 
// Enter OPM file name [/storage1/agile/agile2/aux/data/OPM/2015_12/agile_2015_344_12_08.OPM] : 
// Enter output file name [!test.fits] : 
	

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

	
	
	
	
	
	
	
	
	
	
	
	
int Open3916(int argc, char ** argv, fitsfile ** input, fitsfile ** output, char * infileOPM) {
	int status=0,pkttype=0,pktstype=0;
	char * infile = new char[FLEN_FILENAME];
		
	char * outfile = new char[FLEN_FILENAME];
	int numpar=0;
	const time_t tp = time(NULL);
	char *data=ctime(&tp);	
	

	
	printf("\n\nSTART EXECUTION:  %s  ",data);		
	printf("####################################################\n");
	printf("#########  Task Cor3916_bis......... starting  #########\n");
	printf("#########         %s         #########\n", version);
	printf("####################################################\n\n");
	printf("Cor3916_bis..................... starting Open3916\n");

	status = PILInit(argc,argv);
	PILGetNumParameters(&numpar);

	if ( status != 0 || numpar!=3)
		return 1002;

	PILGetString("infile",infile);
	PILGetString("infileOPM",infileOPM);
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
	
	
	if (pkttype!=39 || pktstype!=16) {
		printf("error packet type [39]   %d!!\n",pkttype );
		printf("error packet subtype [16] %d!!\n",pktstype );
		fits_close_file(input[0], &status);
		return 1001;
		}

	printf("..........opening output file: %s\n",outfile);
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
		printf("Cor3916_bis..................... exiting Cor3916_bis input ERROR:");		
		fits_report_error(stdout, status);
		printf("\n");
		return status;					
		}
	else {
		printf("Cor3916_bis..................... exiting Cor3916_bis\n");		
	
		}
	return status;
	}

int CorrTime(fitsfile * output, char * infileOPM) {

	int oldstatus = 0;
	int status = 0;
	printf("\nCor3916_bis..................... starting CorrTime\n");

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
	
	fits_movabs_hdu(output, 1, NULL, &status);

// 	fits_write_key(output, TSTRING, "FILENAMEOPM",  NULL, "OPM Filename", &status);
	fits_update_key(output, TSTRING, "FILEOPM",  infileOPM, "OPM Filename", &status);	
// fits_write_history(output, infileOPM, &status);	
// 	fits_write_record(output, "TEST PIPPO", &status);
	cout<<infileOPM<<endl;	
///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////PACKETS////////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////
// 	fits_movabs_hdu(output, 2, NULL, &status);
// 	fits_select_rows(output, output, "#ROW == 1", &status);
	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////		
		
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
		
	fits_movabs_hdu(output, 3, NULL, &status);	

	
	
	
// 		
	time = new double[nrows];
	
// 	double tstart = 0, tstop = 0;
	fits_select_rows(output, output, "TIME > 110553540.000", &status);	
	fits_get_num_rows(output, &nrows, &status);
	fits_get_num_cols(output, &ncols, &status);	
	
	fits_get_colnum(output, 1, "TIME", &colnum, &status);	
	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, 1, NULL, &tstart, NULL, &status);
	fits_read_col(output, TDOUBLE, colnum, nrows, 1, 1, NULL, &tstop, NULL, &status);

	fits_select_rows(output, output, "#ROW < 1", &status);
	
	
////////////////////////////////////////////////////////////////////////////
///////////////TEST/////////////////////////////////////////////////////////

	FILE * fp;
	fp = fopen(infileOPM, "r");
	if (!fp) {
		printf("COULD NOT OPEN THE NAMED FILE:  %s\n", infileOPM);
		return 1003;
		}
// 	
// 	float bw = 0;
	char * date1 = new char[128];
	char * date2 = new char[128];	
	int year = 0;
	int month = 0;
	int day = 0;	
	int hour = 0;
	int minute = 0;
	int second = 0;
	float msecond = 0;
	char buffer[1024];
	char bufferData[2048];	
	double mjd = 0;
	
	
	int gpsxpos = 0;
	int gpsypos = 0;
	int gpszpos = 0;
	short datavali = 0;
	short posvali  = 0;
	short seuvali  = 0;
  
		
	
	long riga = 0;
	
	
	
	while (fgets(buffer , 2048, fp)) {
// 		
		char *Ptr, *Co = NULL;
		Ptr = strtok(buffer,"> ");
// 		Ptr = strtok(NULL," ");
		
		
		if (strcmp(Ptr,"<Epoch")==0)  	{  
			Ptr = strtok(NULL," ");
			
			strcpy(date1, Trim(Ptr));
			
			Ptr = strtok(NULL,"</Epoch>");
			strcpy(date2, Trim(Ptr));
			
			year = atoi(strtok(date1,"/"));
			month = atoi(strtok(NULL,"/"));
			day = atoi(strtok(NULL,"/"));
			
			hour = atoi(strtok(date2,":"));
			minute = atoi(strtok(NULL,":"));
			second = atoi(strtok(NULL,":."));			
			msecond = atof(strtok(NULL,":."));
// 			cout<<year<<" "<<month<<" "<<day<<" "<<hour<<" "<<minute<<" "<<second<<" "<<msecond<<endl;
			
			sprintf(bufferData,"%02d-%02d-%02d-%02d-%02d-%02d-%d", day,  month, year-2000, hour, minute, second, (int)msecond);
			mjd = greg_to_mjd(bufferData);
// 			cout<<bufferData<<endl;
			
			
			
			
			double timeOBT = round((mjd-53005.)*86400.);
			
			
			if (timeOBT >= tstart && timeOBT <= tstop) {
			
// 				printf("%f  ,  %f\n\n",mjd, (mjd-53005.)*86400.);
				
				fgets(buffer , 1024, fp);
				fgets(buffer , 1024, fp);
				
				Ptr = Trim(strtok(buffer,"<State_Vector>	"));
				gpsxpos = (int)(atof(Ptr)*1e5);

				Ptr = Trim(strtok(NULL,"	"));
				gpsypos = (int)(atof(Ptr)*1e5);

				Ptr = Trim(strtok(NULL,"	"));			
				gpszpos = (int)(atof(Ptr)*1e5);
							

				
				riga++;
				int pkt = 1;
				fits_get_colnum(output, 1, "TIME", &colnum, &status);					
				fits_write_col(output, TDOUBLE, colnum, riga, 1, 1, &timeOBT, &status);
				
				fits_get_colnum(output, 1, "PAKTNUMB", &colnum, &status);					
				fits_write_col(output, TINT, colnum, riga, 1, 1, &pkt, &status);
					
				fits_get_colnum(output, 1, "EVNUMBER", &colnum, &status);					
				fits_write_col(output, TINT, colnum, riga, 1, 1, &riga, &status);	
				
				fits_get_colnum(output, 1, "GPSXPOS", &colnum, &status);					
				fits_write_col(output, TINT, colnum, riga, 1, 1, &gpsxpos, &status);			
				fits_get_colnum(output, 1, "GPSYPOS", &colnum, &status);					
				fits_write_col(output, TINT, colnum, riga, 1, 1, &gpsypos, &status);			
				fits_get_colnum(output, 1, "GPSZPOS", &colnum, &status);					
				fits_write_col(output, TINT, colnum, riga, 1, 1, &gpszpos, &status);

				int intValue = 0;
				fits_get_colnum(output, 1, "TIMEWEEK", &colnum, &status);
				fits_write_col(output, TINT, colnum, riga, 1, 1,  &intValue, &status);	
				
				fits_get_colnum(output, 1, "TIMESEC", &colnum, &status);		
				fits_write_col(output, TINT, colnum, riga, 1, 1,  &intValue, &status);		
				
				fits_get_colnum(output, 1, "TIMESECF", &colnum, &status);		
				fits_write_col(output, TINT, colnum, riga, 1, 1,  &intValue, &status);					
								
				short posvali = 2;
				fits_get_colnum(output, 1, "POSVALI", &colnum, &status);
				fits_write_col(output, TBYTE, colnum, riga, 1, 1, &posvali, &status);	
				
				}
			}  
		}
		

	fclose(fp);	
//////////////////////////FINE TEST/////////////////////////////////
//////////////////////////////////////////////////////////////////

	fits_movabs_hdu(output, 2, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);

	fits_get_colnum(output, 1, "TIME", &colnum, &status);		
	fits_read_col(output, TDOUBLE, colnum, 1, 1, 1, NULL, &tstart, NULL, &status);
	fits_read_col(output, TDOUBLE, colnum, nrows, 1, 1, NULL, &tstop, NULL, &status);	
	
	
	WriteTime(output, tstart, tstop);


	fits_movabs_hdu(output, 3, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);

	fits_get_colnum(output, 1, "TIME", &colnum, &status);		
	fits_read_col(output, TDOUBLE, colnum, 1, 1, 1, NULL, &tstart, NULL, &status);
	fits_read_col(output, TDOUBLE, colnum, nrows, 1, 1, NULL, &tstop, NULL, &status);	
	WriteTime(output, tstart, tstop);	
	delete [] time;	

	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	
	if ( status) {
		printf("Cor3916_bis..................... exiting CorrTime ERROR:");		
		fits_report_error(stdout, status);
		printf("\n");
		printf("\n");		
		return status;		
		}
	else {
		printf("Cor3916_bis..................... exiting CorrTime STATUS = 0\n\n");		
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
		
		strcpy(filename, "Cor3916_bis");
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

int Close3916(int argc, char ** argv, fitsfile * input, fitsfile * output, char * infileOPM) {

	int status=0;
	int hdnum = 0;
	
	printf("Cor3916_bis..................... starting Close3916\n");	
	
		
	double tmin=0;
	double tmax=0;
	fits_read_key(output, TDOUBLE, "TSTART", &tmin, NULL, &status );
	fits_read_key(output, TDOUBLE, "TSTOP", &tmax, NULL, &status );	
	if( argc==5) {
		FILE *fp4;
		if(checkfile(argv[4], argv[3], tmin, tmax, (char *)packet) == 0) {
			fp4 = fopen(argv[4], "a+");
			if (argv[3][0] == '!')
				fprintf(fp4,"%s  %f  %f %s\n", argv[3], tmin, tmax, packet);
			else
				fprintf(fp4,"%s  %f  %f %s\n", argv[3], tmin, tmax, packet);
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
		printf("Cor3916_bis..................... exiting Close3916 input ERROR:");		
		fits_report_error(stdout, status);	
		printf("\n");
		return status;
		}			


			
	printf("Cor3916_bis..................... exiting Close3916 STATUS = 0\n\n");		

	
	printf("TIME MINIMUM:  %f\n",tmin);		
	printf("TIME MAXIMUM:  %f\n",tmax);	
			
	printf("\n\n####################################################\n");
	printf("#########  Task Cor3916_bis......... exiting #########\n");
	printf("####################################################\n\n\n");					
	
	return status;
	}

int main(int argc,char **argv)
{
int status=0;

fitsfile ** input = new fitsfile*[1];
fitsfile ** output = new fitsfile*[1];

char * infileOPM = new char[FLEN_KEYWORD];



status = Open3916(argc, argv, input, output, infileOPM);

// cout<<infileOPM<<endl;

 if (status ==0)
 	status = CorrTime(output[0],infileOPM);
 if (status ==0)	
	WriteHeader(output[0]);

	
// if (status ==0)
// 	status = CorrAtt(output[0]);

if (status ==0)
	status = Close3916(argc, argv, input[0], output[0],infileOPM);

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

delete []input;
delete []output;
return status;
}




