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
//       3914.lv1       = grid event report
//
// OUTPUT 
//       3914.cor       = grid event report
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

#include "Cor3914_bis.h"



int Open3914(int argc, char ** argv, fitsfile ** input, fitsfile ** output, FILE *fp) {
	int status=0,pkttype=0,pktstype=0;
	char * infile = new char[FLEN_FILENAME];
	char * outfile = new char[FLEN_FILENAME];
	int numpar=0;
	const time_t tp = time(NULL);
	char *data=ctime(&tp);	
	
// 	fprintf(fp,"\n\nSTART EXECUTION:  %s",data);	
// 	fprintf(fp, "\n\n####################################################\n");
// 	fprintf(fp, "#########  Task Cor3914......... starting  #########\n");
// 	fprintf(fp, "####################################################\n\n\n");
// 	fprintf(fp, "Cor3914..................... starting Open3914\n");

	
	printf("\n\nSTART EXECUTION:  %s  ",data);		
	printf("####################################################\n");
	printf("#########  Task Cor3914......... starting  #########\n");
	printf("#########         %s         #########\n", version);
	printf("####################################################\n\n");
	printf("Cor3914..................... starting Open3914\n");

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
	
	
	if (pkttype!=39 || pktstype!=14) {
		printf("error packet type [39]   %d!!\n",pkttype );
		printf("error packet subtype [14] %d!!\n",pktstype );
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
// 		fprintf(fp, "Cor3914..................... exiting Cor3914 input ERROR:");
		printf("Cor3914..................... exiting Cor3914 input ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		return status;					
		}
	else {
// 		fprintf(fp, "Cor3914..................... exiting Cor3914\n");
		printf("Cor3914..................... exiting Cor3914\n");		
	
		}
	return status;
	}

	


	
int CorrTime(fitsfile * output, FILE *fp) {

	int oldstatus = 0;
	int status = 0;
// 	fprintf(fp, "\nCor3914..................... starting CorrTime\n");
	printf("\nCor3914..................... starting CorrTime\n");	
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
	
	
	double mjdrefi = 53005.;
	double mjdreff = 0.00075444444444444444;
	
		
///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////PACKETS////////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////

	CorrTimePKT(output, fp);

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////		
	fits_movabs_hdu(output, 2, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);
	int * paktnumbPKT = new int [nrows];
	
	double *timePKT = new double [nrows];	
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, timePKT, NULL, &status);	
	
	fits_get_colnum(output, 1, "PAKTNUMB", &colnum, &status);		
	fits_read_col(output, TINT32BIT, colnum, 1, 1, nrows, NULL, paktnumbPKT, NULL, &status);		
		
	fits_movabs_hdu(output, 3, NULL, &status);	
	
	fits_get_num_rows(output, &nrows, &status);
	fits_get_num_cols(output, &ncols, &status);	
		
	int  short * day            = new int short[nrows];
	int        * timemsec       = new int[nrows];
	double     * time           = new double[nrows];
	int        * microsecpktobt = new int[nrows];	
	int        * secpktobt      = new int[nrows];
	int        * paktnumb       = new int [nrows];	
	int        * evnumber       = new int [nrows];
	int        * ssid           = new int [nrows];
	int        * msgvali        = new int [nrows];			
	
	
	tstart = 0;
	tstop = 0;
	
/////////passo 1
	
	strcpy(str, "TT");
	fits_update_key(output, TSTRING, "TIMESYS",  str, NULL, &status);

/////////passo 2
	oldstatus = status;
	fits_delete_key(output, "MJDREF", &status);
	if (status == 202) status = oldstatus;

/////////passo 3
	fits_update_key(output, TDOUBLE, "MJDREFI",  &mjdrefi, NULL, &status);

/////////passo 4
	fits_update_key(output, TDOUBLE, "MJDREFF",  &mjdreff, NULL, &status);

/////////passo 5
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, NULL, &status);	

/////////passo 6 - 7 - 8

	
	
	fits_get_colnum(output, 1, "TIMEDAY", &colnum, &status);	
	fits_read_col(output, TSHORT, colnum, 1, 1, nrows, NULL, day, NULL, &status);	

	fits_get_colnum(output, 1, "TIMEMSEC", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, timemsec, NULL, &status);		
		
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, time, NULL, &status);		

	for (int i=0; i < nrows; i++) {
		day[i] = day[i] - 16801;
		if (day[i] < 0) day[i] = 0;
/*		timemsec[i] = timemsec[i] + 65184;	
		if ( timemsec[i] > 86400000) {
			timemsec[i] = timemsec[i] - 86400000;	
			day[i] = day[i] + 1;
			}	*/
 		time[i] = 86400.*(double)day[i] + ((double)timemsec[i])/1000.;
		}
		
		
		


	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);
	
	
	fits_get_colnum(output, 1, "PAKTNUMB", &colnum, &status);		
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, paktnumb, NULL, &status);	
	
	fits_get_colnum(output, 1, "EVNUMBER", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, evnumber, NULL, &status);

	fits_get_colnum(output, 1, "SSID", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, ssid, NULL, &status);	

	fits_get_colnum(output, 1, "MSGVALI", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, msgvali, NULL, &status);	

	
	int countevtcorr = 0;			
	
	
	double countpkt = 0;

		
	for(int i=nrows-2; i > 0; i--) {
	
		if (i==nrows-2){
			if (time[i+1] -timePKT[paktnumb[i]-1] > 0. && msgvali[i]==0) {
				int k = i+1;
				countpkt++;
				time[k] = time[k] -1;	
// 				cout<<"PKT = "<<paktnumb[i]<<" "<<k<<" "<<time[k]-time[k-1]<<endl;							
				while(k > 0 && (paktnumb[k]==paktnumb[k-1]) && (time[k]-time[k-1]<0.)) {
// 					cout<<"PKT = "<<paktnumb[i]<<" "<<k<<" "<<time[k]-time[k-1]<<endl;
					k--;
					time[k] = time[k] -1;
					countevtcorr++;	
					}
				i=k;						
				}
		
			}
		else if ((paktnumb[i]!=paktnumb[i+1]) && msgvali[i]==0) {
			
			if ( time[i] -timePKT[paktnumb[i]-1] > 0. ) {
/*				cout<<i<<endl;	*/
// 				cout<<"PKT = "<<paktnumb[i]<<" "<<endl;
				int k = i;
				int m = i+1;
				countpkt++;
				time[k] = time[k] -1;
				
				while(k > 0 && (paktnumb[k]==paktnumb[k-1]) && (time[k]-time[k-1]<0.)) {
					k--;
					time[k] = time[k] -1;
					countevtcorr++;	
					}
				i=k;				
				
				while( m<nrows-1  && (paktnumb[m+1]==paktnumb[m]) && msgvali[m]==0) {
					if (((time[m] - time[m-1]) >= 1.)  && ((time[m] - time[m-1]) < 2.)) {
						time[m] = time[m] -1;
/*						cout<<m<<endl;*/
						m++;
						countevtcorr++;
						}
					else {	
						m=nrows-1;
						}
					if((m<nrows-1  && (paktnumb[m+1]!=paktnumb[m]) && (((time[m+1] - time[m]) >= 1.)  && ((time[m+1] - time[m]) < 2.))) || (msgvali[m]!=0)) {
						time[m] = time[m] -1;	
// 						cout<<m<<endl;

						}
					}
				}
			}			
		}	
	
	double tpart = 0;
	tpart = time[0];	
	
	
	for(int i=nrows-2; i > 0; i--) {
		if ( ((time[i] - time[i-1]) < 0)) {
			time[i-1] = time[i-1] -1;
			countevtcorr++;
			}
		}
	for(int i=nrows-2; i > 0; i--) {
		if ( ((time[i] - time[i-1]) < 0)) {
			time[i-1] = time[i-1] -1;
			}
		}
	
	cout<<"DATI CORRETTI %  = "<<(countevtcorr/(double)nrows)*100<<endl;	
	cout<<"PKT ERRATI %     = "<<(countpkt/((double)(paktnumb[nrows-1])))*100<<endl;		
		
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);	

	
	
	fits_get_colnum(output, 1, "TIMEMSEC", &colnum, &status);
	fits_write_col(output, TINT, colnum, 1, 1, nrows,  timemsec, &status);	
	
	fits_get_colnum(output, 1, "TIMEDAY", &colnum, &status);		
	fits_write_col(output, TSHORT, colnum, 1, 1, nrows,  day, &status);		

////////passo 9
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	sprintf(unit, "TUNIT%d", colnum);
	fits_update_key(output, TSTRING, unit, s, NULL, &status);

////////passo 10 -11
	WriteTime(output, time[0], time[nrows-1]);

		
	fits_movabs_hdu(output, 1, NULL, &status);	
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, NULL, &status);
	WriteTime(output, time[0], time[nrows-1]);

	
	
	
	
	
	delete []  paktnumb;	
	delete []  evnumber;
	delete []  ssid    ;
	delete []  msgvali ;	
	delete [] paktnumbPKT;	
	delete [] timePKT;
	delete [] time;	
	delete [] day;	
	delete [] microsecpktobt;	
	delete [] timemsec;
	delete [] secpktobt;	
	
	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////		
		
	if ( status) {
// 		fprintf(fp, "Cor3914..................... exiting CorrTime ERROR:");
		printf("Cor3914..................... exiting CorrTime ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		printf("\n");		
		return status;		
		}
	else {
// 		fprintf(fp, "Cor3914..................... exiting CorrTime STATUS = 0\n\n");
		printf("Cor3914..................... exiting CorrTime STATUS = 0\n\n");		
		}	
	
	
	return status;
	}

	

	
	
	
	
	

int CorrAtt(fitsfile * output, FILE *fp) {


	printf("\nCor3914..................... starting CorrAtt\n");
	int status = 0;
	long i = 0;  /* MUST initialize status */

// 	float  Qss1[4]={-0.5,-0.5,-0.5,0.5}, Qss2[4]={0.5, 0.5, -0.5, 0.5};


	long nrows=0;
	int ncols = 0;

//	short * SS;			
	int colnum = 0;

	fits_movabs_hdu(output, 3, NULL, &status);
	fits_get_num_rows(output, &nrows, &status);	
	fits_get_num_cols(output, &ncols, &status);		

	////nella prima extension(packets) aggiunge 6 nuova colonne con le cordinate radec dei versore del sistema di riferimento del paylod

	float Qread[4][nrows];
	short SS[nrows];
	short msgval[nrows];
	
	
	
// 	fits_insert_col(output, 100, "Q1SS", "1E", &status);
// 	
// 	fits_insert_col(output, 100, "Q2SS", "1E", &status);
// 
// 	fits_insert_col(output, 100, "Q3SS", "1E", &status);	
// 	
// 	fits_insert_col(output, 100, "Q4SS", "1E", &status);
		
	
	
	fits_get_colnum(output, 1, "Q1SS", &colnum, &status);		
	fits_read_col(output, TFLOAT, colnum, 1, 1, nrows, NULL, Qread[0], NULL, &status);
	
	fits_get_colnum(output, 1, "Q2SS", &colnum, &status);					
	fits_read_col(output, TFLOAT, colnum, 1, 1, nrows, NULL, Qread[1], NULL, &status);
		
	fits_get_colnum(output, 1, "Q3SS", &colnum, &status);			
	fits_read_col(output, TFLOAT, colnum, 1, 1, nrows, NULL, Qread[2], NULL, &status);
		
	fits_get_colnum(output, 1, "Q4SS", &colnum, &status);			
	fits_read_col(output, TFLOAT, colnum, 1, 1, nrows, NULL, Qread[3], NULL, &status);		
	
	fits_get_colnum(output, 1, "SSID", &colnum, &status);
	fits_read_col(output, TSHORT, colnum, 1, 1, nrows, NULL, SS, NULL, &status);
	

	
	
	
 	ncols++;
	char * str = "deg";
	char unit[15];	
		
// 	sprintf(unit, "TUNIT%d",ncols);
// 	fits_insert_col(output, ncols++, "X_RA", "1E", &status);
// 	fits_update_key(output, TSTRING, unit, str, NULL, &status);
// 			
// 	sprintf(unit, "TUNIT%d",ncols);	
// 	fits_insert_col(output, ncols++, "X_DEC", "1E", &status);
// 	fits_update_key(output, TSTRING, unit, str, NULL, &status);
// 			
// 	sprintf(unit, "TUNIT%d",ncols);		
// 	fits_insert_col(output, ncols++, "Y_RA", "1E", &status);
// 	fits_update_key(output, TSTRING, unit, str, NULL, &status);
// 			
// 	sprintf(unit, "TUNIT%d",ncols);		
// 	fits_insert_col(output, ncols++, "Y_DEC", "1E", &status);
// 	fits_update_key(output, TSTRING, unit, str, NULL, &status);
// 			
// 	sprintf(unit, "TUNIT%d",ncols);		
// 	fits_insert_col(output, ncols++, "Z_RA", "1E", &status);
// 	fits_update_key(output, TSTRING, unit, str, NULL, &status);
// 			
// 	sprintf(unit, "TUNIT%d",ncols);		
// 	fits_insert_col(output, ncols++, "Z_DEC", "1E", &status);
// 	fits_update_key(output, TSTRING, unit, str, NULL, &status);
			

	float Xa[nrows];
	float Xd[nrows];
	float Ya[nrows];
	float Yd[nrows];
	float Za[nrows];
	float Zd[nrows];
	float q1[nrows];
	float q2[nrows];
	float q3[nrows];
	float q4[nrows];
	cout << "Number of rows = " << nrows << endl;
	
	
	
	QUAT Qss1, Qss2, Qjp, Qjpneg, Xp, Yp , Zp, Qjs, XX, YY, ZZ, X, Y,Z;
	setQuat(&Qss1, -0.5003349,-0.5005058,-0.4992172,0.4999412);
	setQuat(&Qss2, 0.4994995, 0.5006291, -0.4995146, 0.5003557);
	setQuat(&X, 1,0,0,0);
	setQuat(&Y, 0,1,0,0);
	setQuat(&Z, 0,0,1,0);
// 	float X[4]={1,0,0,0}, Y[4]={0,1,0,0}, Z[4]={0,0,1,0};	
	
	
// 	float   Qss1[4]={-0.5003349,-0.5005058,-0.4992172,0.4999412};
// 	float   Qss2[4]={0.4994995, 0.5006291, -0.4995146, 0.5003557};
	
// 	float * Qjp = new float[4];
// 	float * Xp = new float[4];
// 	float * Yp = new float[4];
// 	float * Zp = new float[4];	
// 	float Qjs[4];
	
		
	for(i=0;i<nrows;i++) {
		Qjs.p[0] = Qread[0][i];
		Qjs.p[1] = Qread[1][i];
		Qjs.p[2] = Qread[2][i];
		Qjs.p[3] = Qread[3][i];		

//		cout <<"Qjs "<<Qjs[0]<<" "<<Qjs[1]<<" "<<Qjs[2]<<" "<<Qjs[3]<<endl;
//		cout <<"Qss1 "<<Qss1[0]<<" "<<Qss1[1]<<" "<<Qss1[2]<<" "<<Qss1[3]<<endl;		
		if (SS[i]==0) {
			productOfQuats(&Qjp, &Qjs, &Qss1);
			}
		else {
			productOfQuats(&Qjp, &Qjs, &Qss2);
			}
	
// 					invertQuatInPlace(&Qjp);
// 					productOfQuats(&Q, &Y, &Qjp);
// 					invertQuatInPlace(&Qjp);
// 					productOfQuats(&Yp, &Qjp, &Q);
// 					renormalizeQuat(&Qjp);
//		cout <<"Qjp "<<Qjp[0]<<" "<<Qjp[1]<<" "<<Qjp[2]<<" "<<Qjp[3]<<endl;			


		invertQuat(&Qjpneg,&Qjp);

		
		productOfQuats(&XX, &X, &Qjpneg);
		productOfQuats(&YY, &Y, &Qjpneg);
		productOfQuats(&ZZ, &Z, &Qjpneg);				

		productOfQuats(&Xp, &Qjp, &XX);
		productOfQuats(&Yp, &Qjp, &YY);
		productOfQuats(&Zp, &Qjp, &ZZ);		
		

		
		
// 	double r = sqrt( Xp[0]*Xp[0] + Xp[1]*Xp[1] + Xp[2]*Xp[2]);
// 	
// 	*dec = 90 - acosd(Xp[2]/r);
// 
// 	if ( *dec == 90 || *dec == -90){
// 		*ra  = 0;
// 		}
// 	else {
// 		*ra  = atan2d(Xp[1],Xp[0]);
// 		}
// /*
		
		
		
		double r = sqrt( Xp.p[0]*Xp.p[0] + Xp.p[1]*Xp.p[1] + Xp.p[2]*Xp.p[2]);

		Xd[i] = 90 - acosd(Xp.p[2]/r);

		if ( Xd[i] == 90 || Xd[i] == -90){
			Xa[i]  = 0;
			}
		else {
			Xa[i]  = atan2d(Xp.p[1],Xp.p[0]);
			}		
		
			
		r = sqrt( Yp.p[0]*Yp.p[0] + Yp.p[1]*Yp.p[1] + Yp.p[2]*Yp.p[2]);

		Yd[i] = 90 - acosd(Yp.p[2]/r);

		if ( Yd[i] == 90 || Yd[i] == -90){
			Ya[i]  = 0;
			}
		else {
			Ya[i]  = atan2d(Yp.p[1],Yp.p[0]);			
			}					
			
		r = sqrt( Zp.p[0]*Zp.p[0] + Zp.p[1]*Zp.p[1] + Zp.p[2]*Zp.p[2]);

		Zd[i] = 90 - acosd(Zp.p[2]/r);

		if ( Zd[i] == 90 || Zd[i] == -90){
			Za[i]  = 0;
			}
		else {
			Za[i]  = atan2d(Zp.p[1],Zp.p[0]);
			}				
			
		q1[i] = Qjp.p[0];
		q2[i] = Qjp.p[1];
		q3[i] = Qjp.p[2];
		q4[i] = Qjp.p[3];


		}

	fits_get_colnum(output, 1, "X_RA", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Xa, &status);	
	
	fits_get_colnum(output, 1, "X_DEC", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Xd, &status);	
	
	fits_get_colnum(output, 1, "Y_RA", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Ya, &status);	
	
	fits_get_colnum(output, 1, "Y_DEC", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Yd, &status);	
	
	fits_get_colnum(output, 1, "Z_RA", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Za, &status);	
	
	fits_get_colnum(output, 1, "Z_DEC", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Zd, &status);	
	
/*	fits_get_colnum(output, 1, "X_RA", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Xa, &status);*/							

	
	fits_get_colnum(output, 1, "Q1", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  q1, &status);	
	
	fits_get_colnum(output, 1, "Q2", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  q2, &status);	
	
	fits_get_colnum(output, 1, "Q3", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  q3, &status);	
	
	fits_get_colnum(output, 1, "Q4", &colnum, &status);					
	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  q4, &status);	

	
// 	fits_get_colnum(output, 1, "Q1SS", &colnum, &status);					
// 	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Qread[0], &status);	
// 	
// 	fits_get_colnum(output, 1, "Q2SS", &colnum, &status);					
// 	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Qread[1], &status);	
// 	
// 	fits_get_colnum(output, 1, "Q3SS", &colnum, &status);					
// 	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Qread[2], &status);	
// 	
// 	fits_get_colnum(output, 1, "Q4SS", &colnum, &status);					
// 	fits_write_col(output, TFLOAT, colnum, 1, 1, nrows,  Qread[3], &status);	
	
///////////////////////////////////////////////////////////////////////////////////////		


	fits_get_colnum(output, 1, "MSGVALI", &colnum, &status);
	fits_read_col(output, TSHORT, colnum, 1, 1, nrows, NULL, msgval, NULL, &status);		
	
	
	int X_RAcolnum = 0;
	int X_DECcolnum = 0;
	int Y_RAcolnum = 0;	
	int Y_DECcolnum = 0;
	int Z_RAcolnum = 0;
	int Z_DECcolnum = 0;		
	int Q1colnum = 0;
	int Q2colnum = 0;
	int Q3colnum = 0;		
	int Q4colnum = 0;		
	
			
	fits_get_colnum(output, 1, "X_RA", &X_RAcolnum, &status);				
	fits_get_colnum(output, 1, "X_DEC", &X_DECcolnum, &status);
	fits_get_colnum(output, 1, "Y_RA", &Y_RAcolnum, &status);							
	fits_get_colnum(output, 1, "Y_DEC", &Y_DECcolnum, &status);						
	fits_get_colnum(output, 1, "Z_RA", &Z_RAcolnum, &status);						
	fits_get_colnum(output, 1, "Z_DEC", &Z_DECcolnum, &status);						
	fits_get_colnum(output, 1, "Q1", &Q1colnum, &status);							
	fits_get_colnum(output, 1, "Q2", &Q2colnum, &status);							
	fits_get_colnum(output, 1, "Q3", &Q3colnum, &status);										
	fits_get_colnum(output, 1, "Q4", &Q4colnum, &status);						
			
			
			
			
			
			
					
	for(i=0;i<nrows;i++) {
		if (msgval[i]!=0){
			fits_write_col_null(output, X_RAcolnum, i+1, 1, 1, &status);	
			fits_write_col_null(output, X_DECcolnum, i+1, 1, 1, &status);
			fits_write_col_null(output, Y_RAcolnum, i+1, 1, 1, &status);	
			fits_write_col_null(output, Y_DECcolnum, i+1, 1, 1, &status);
			fits_write_col_null(output, Z_RAcolnum, i+1, 1, 1, &status);	
			fits_write_col_null(output, Z_DECcolnum, i+1, 1, 1, &status);
			fits_write_col_null(output, Q1colnum, i+1, 1, 1, &status);	
			fits_write_col_null(output, Q2colnum, i+1, 1, 1, &status);
			fits_write_col_null(output, Q3colnum, i+1, 1, 1, &status);
			fits_write_col_null(output, Q4colnum, i+1, 1, 1, &status);
			}
		}		
////////////////////////////////////////////////////////////		
					
	
	
// 	delete [] Qjp;
// 	delete [] Xp;	
// 	delete [] Yp;	
// 	delete [] Zp;
/*	delete [] SS;
	delete [] Xa;	
	delete [] Xd;	
	delete [] Ya;
	delete [] Yd;	
	delete [] Za;
	delete [] Zd;
	delete [] q1;	
	delete [] q2;	
	delete [] q3;	
	delete [] q4;	*/
	
	if ( status) {
		printf("Cor3914..................... exiting CorrTime ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);
		printf("\n");
		return status;		
		}
	else {
		printf("Cor3914..................... exiting CorrTime STATUS = 0\n\n");		
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
		
		strcpy(filename, "Cor3914");
		fits_update_key(output, TSTRING, "CREATOR",  filename, NULL, &status);	
		
		strcpy(filename, "39.14");		
		fits_update_key(output, TSTRING, "CREAT_ID",  filename, NULL, &status);	
		
		strcpy(filename, "IASF-MILANO");	
		fits_update_key(output, TSTRING, "ORIGIN",  filename, NULL, &status);		
		
		int timeref = 0;
		fits_get_system_time(filename, &timeref, &status);
		fits_update_key(output, TSTRING, "DATE",  filename, NULL, &status);
		
		fits_update_key(output, TSTRING, "COR_VERS",  corvers, NULL, &status);	
		
		strcpy(filename, "GRID");		
		fits_update_key(output, TSTRING, "Q_SYS",  &filename, NULL, &status);		
					
		}
	return status;
	}



int Close3914(int argc, char ** argv, fitsfile * input, fitsfile * output, FILE *fp) {

	int status=0;
	int hdnum = 0;
	

	printf("Cor3914..................... starting Close3914\n");	

	
	
		
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
		printf("Cor3914..................... exiting Close3914 input ERROR:");		
		fits_report_error(fp, status);
		fits_report_error(stdout, status);	
		printf("\n");
		return status;
		}			


			
	printf("Cor3914..................... exiting Close3914 STATUS = 0\n\n");
	printf("TIME MINIMUM:  %f\n",tmin);		
	printf("TIME MAXIMUM:  %f\n",tmax);	
	printf("\n\n####################################################\n");
	printf("#########  Task Cor3914......... exiting #########\n");
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

status = Open3914(argc, argv, input, output, fp);

// if (status ==0)
// 	status = CorrTime(output[0], fp);
	
if (status ==0)	
	WriteHeader(output[0]);
	
if (status ==0)
	status = CorrAtt(output[0], fp);

if (status ==0)
	status = Close3914(argc, argv, input[0], output[0], fp);

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




