// Author   : Alessio Trois
// Version  :   16-Mar-05
// Version  :   11-Jul-11
// Version  :   23-May-12

#include "CorUtilities.h"
double mjdrefi = 53005.;
// 57204.00000000

double mjdreff      = 0.000754444444444;
double mjdreff_2009 = 0.000766018518519;
double mjdreff_2012 = 0.000777592592593;
double mjdreff_2015 = 0.000789166666667;
//secondo 1 Gennaio 2017 -> 410313600.0
double mjdreff_2017 = 0.000800740740741 ;


double obtoi = 54160.;
double obtof      = 0.000754444444444;
double obtof_2009 = 0.000766018518519;
double obtof_2012 = 0.000777592592593;
double obtof_2015 = 0.000789166666667;
double obtof_2017 = 0.000800740740741;
double limitobtleapsecond2017 = 412349434.;
//    Il vecchio numero era (ed e' per dati prima di 1/1/2009)
//    0.0007544444444444444444444 = 65.184 / 86400 . = 0.000754444444444
//    Ai nuovi dati oltre il 1 Luglio 2008 si aggiungerà il leap second 
//    Il nuovo quindi e' 66.184 / 86400 = 0.000766018518519
//    Ai nuovi dati oltre il 1 Gennaio 2012 si aggiungerà il leap second 
//    Il nuovo quindi e' 67.184 / 86400 = 0.000777592592593
//    Ai nuovi dati oltre il 30 Giugno 2015 si aggiungerà il leap second 
//    Il nuovo quindi e' 68.184 / 86400 = 0.000789166666667

int diffobtsectt     = 756950400;
    
int diffobtsectt2015 = 756873344;

double diffobtmsect = 0.0;

int diffobtsecttEVT =         -99792000;

int diffobtsecttEVT2015 =    -368227456;

int diffobtmsectEVT2015 = 4000; //microsecondi

int diffobtmsectEVT = 0;

int CorrTimeEVT(fitsfile * output, FILE *fp) {
	
	int oldstatus = 0;
	int status = 0;
	int i = 0;
	int colnum = 0;	
	int ncols = 0;		
	long nrows = 0;
	char unit[FLEN_KEYWORD];
	char * s = "s";	
	double tstart = 0;
	double tstop = 0;
	float timezero = 0;	
	
	fits_movabs_hdu(output, 3, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);
	fits_get_num_cols(output, &ncols, &status);
	
	double * time = new double[nrows];
	int * micsec = new int[nrows];
	int * sec = new int[nrows];

	float * obt_corr = new float[nrows];

	
	tstart = 0;
	tstop = 0;
	char str[100];

/////////passo 1
	
	strcpy(str, "TT");
	fits_update_key(output, TSTRING, "TIMESYS",  str, NULL, &status);

/////////passo 2
	oldstatus = status;
	fits_delete_key(output, "MJDREF", &status);
	if (status == 202) status = oldstatus;

/////////passo 8 - 9 - 10
	fits_get_colnum(output, 1, "TIME", &colnum, &status);	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, time, NULL, &status);		
		
	fits_get_colnum(output, 1, "MICSECTT", &colnum, &status);	
	fits_read_col(output, TUINT, colnum, 1, 1, nrows, NULL, micsec, NULL, &status);	
	
	fits_get_colnum(output, 1, "SECONDTT", &colnum, &status);	
	fits_read_col(output, TUINT, colnum, 1, 1, nrows, NULL, sec, NULL, &status);	
	
	fits_get_colnum(output, 1, "OBTCORR", &colnum, &status);	
	fits_read_col(output, TFLOAT, colnum, 1, 1, nrows, NULL, obt_corr, NULL, &status);		
		


	int orbit = 0;
	char orbitcomment[FLEN_KEYWORD];
  	fits_read_key(output, TINT, "OBS_ID", &orbit, orbitcomment,  &status);	     
// 	cout<<orbit<<endl;		
	
	for (i = 0; i < nrows; ++i) {

////////////////RICIRCOLI/////////////////////////////////////////////////
//timezero viene posto = a zero nei file COR
// il tempo in TT degli eventi sarà calcolato come (MJDREFI +MJDREFF)*86400. + TIME[i]

  
//PRIMO RICIRCOLO SOLO EVENTI 21176
//SECONDO RICIRCOLO EVENTI E PACCHETTI 43222
//NUOVO START MISSION		       43315

// int diffobtsecttEVT         =    -99792000;
// int diffobtsecttEVT2015 =    -368227456;

// int diffobtmsectEVT = 0;
// int diffobtmsectEVT2015 = 4000; //microsecondi


		if (orbit <= 21176) {
			sec[i] = sec[i] - diffobtsecttEVT;
		
			}
		else if (orbit > 21176 && orbit < 43222){
		 	sec[i]=sec[i]+(1<<27)- diffobtsecttEVT;;
			}
		else 	if (orbit >= 43222 && orbit < 43315){	
			sec[i]=sec[i]+(1<<28) - diffobtsecttEVT;			
			}
			
		else if (orbit >= 43315) {
			sec[i]=sec[i]- diffobtsecttEVT2015;
			}
/////////////////////////////////////////////////////////////////////////////////
			
//////////////////////////////////////////////////////////////////////////						
///////////////////obtcorr e microsecondi//////////////////////////////////////////
		micsec[i] = (int)(((double)micsec[i])*(1000000.0/(1000000.0+obt_corr[i])));
		
		if (orbit >= 43315 ) {
			micsec[i] = micsec[i] - diffobtmsectEVT2015;
			}		
		
		if ( micsec[i] >= 1000000) {
			micsec[i] = micsec[i] - 1000000;	
			sec[i] = sec[i] + 1;
			}		
		

		time[i] = sec[i] + (((double)micsec[i])/1000000);
                if ((time[i]>=410313600.0) && (time[i]< limitobtleapsecond2017)) time[i]+=1;                
		}


	fits_get_colnum(output, 1, "TIME", &colnum, &status);		
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);	

	
	fits_get_colnum(output, 1, "MICSECTT", &colnum, &status);
	fits_write_col(output, TUINT, colnum, 1, 1, nrows,  micsec, &status);	
		
	fits_get_colnum(output, 1, "SECONDTT", &colnum, &status);		
	fits_write_col(output, TUINT, colnum, 1, 1, nrows,  sec, &status);


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////passo 3
	fits_update_key(output, TDOUBLE, "MJDREFI",  &mjdrefi, NULL, &status);


	
/////////passo 5
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, str, &status);	

/////////passo 6
	fits_update_key(output, TDOUBLE, "OBT0I",  &obtoi, NULL, &status);

/////////passo 7
	if(time[0] < 157852800.00000) {
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof, NULL, &status);
		}
	else if((time[0] >= 157852800.00000) && (time[0] < 268185600.)){
	// 	cout<<"nuovo leap second pkt "<<mjdreff_2009<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2009, NULL, &status);
		}
	else if((time[0] >= 268185600) && (time[0] < 362793600)){
	// 	cout<<"nuovo leap second pkt "<<mjdreff_2009<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2012, NULL, &status);
		}
	else if((time[0] >= 362793600) && (time[0] < 410313600)) {
		cout<<"nuovo leap second pkt 2015 "<<mjdreff_2015<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2015, NULL, &status);
		}	
        else {
 		cout<<"nuovo leap second pkt 2017 "<<mjdreff_2017<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2017, NULL, &status);            
                }
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


		
////////passo 11
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	sprintf(unit, "TUNIT%d", colnum);
	fits_update_key(output, TSTRING, unit, s, NULL, &status);
	
	WriteTime(output, time[0], time[nrows-1]);
	fits_movabs_hdu(output, 1, NULL, &status);	
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, str, &status);
	WriteTime(output, time[0], time[nrows-1]);			


	delete[] time;
	delete[] micsec;
	delete[] sec;
	delete[] obt_corr;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	return status;
	}



int CorrTimeEVT3908(fitsfile * output, FILE *fp) {
//	int * obt_corr;
	int oldstatus = 0;
	int status = 0;
	int  i = 0;
	int colnum = 0;	
	int ncols = 0;		
	long nrows = 0;
	double * time = NULL;
	char unit[FLEN_KEYWORD];
	char * s = "s";	
	double tstart = 0;
	double tstop = 0;
	float timezero = 0;	
	
	fits_movnam_hdu(output, BINARY_TBL, "EVENTS", 0, &status);	
	fits_get_num_rows(output, &nrows, &status);
	fits_get_num_cols(output, &ncols, &status);
	
	time = new double[nrows];
	int * micsec = new int[nrows];
	int * sec = new int[nrows];

// 	obt_corr = new int[nrows];
// 
// 	
	tstart = 0;
	tstop = 0;
	char str[100];


/////////passo 1
	
	strcpy(str, "TT");
	fits_update_key(output, TSTRING, "TIMESYS",  str, NULL, &status);

/////////passo 2
	oldstatus = status;
	fits_delete_key(output, "MJDREF", &status);
	if (status == 202) status = oldstatus;

/////////passo 3
	fits_update_key(output, TDOUBLE, "MJDREFI",  &mjdrefi, NULL, &status);

	
	
/////////passo 5
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, str, &status);	

/////////passo 6
	fits_update_key(output, TDOUBLE, "OBT0I",  &obtoi, NULL, &status);

/////////passo 7
	if(time[0] < 157852800.00000) {
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof, NULL, &status);
		}
	else if((time[0] >= 157852800.00000) && (time[0] < 268185600.)){
	// 	cout<<"nuovo leap second pkt "<<mjdreff_2009<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2009, NULL, &status);
		}
	else if((time[0] >= 268185600) && (time[0] < 362793600)){
	// 	cout<<"nuovo leap second pkt "<<mjdreff_2009<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2012, NULL, &status);
		}
	else if((time[0] >= 362793600) && (time[0] < 410313600)) {
		cout<<"nuovo leap second pkt 2015 "<<mjdreff_2015<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2015, NULL, &status);
		}	
        else {
 		cout<<"nuovo leap second pkt 2017 "<<mjdreff_2017<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2017, NULL, &status);            
                }

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/////////passo 8 - 9 - 10
	fits_get_colnum(output, 1, "TIME", &colnum, &status);	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, time, NULL, &status);		
		
	fits_get_colnum(output, 1, "MICSECTT", &colnum, &status);	
	fits_read_col(output, TUINT, colnum, 1, 1, nrows, NULL, micsec, NULL, &status);	
	
	fits_get_colnum(output, 1, "SECONDTT", &colnum, &status);	
	fits_read_col(output, TUINT, colnum, 1, 1, nrows, NULL, sec, NULL, &status);	
	
/*	fits_get_colnum(output, 1, "OBTCORR", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, obt_corr, NULL, &status);*/		
	int orbit = 0;
	char orbitcomment[FLEN_KEYWORD];
  	fits_read_key(output, TINT, "OBS_ID", &orbit, orbitcomment,  &status);	     
// 	cout<<orbit<<endl;		
	for (i = 0; i < nrows; ++i) {
// 		int diffobtsecttEVT = 99791934;
// 		double diffobtmsectEVT = 816000.0;
// 		double microsec=MicSecCol*(1E-6)*(1000000.0/(1000000.0+ObtCor));

	
////////////////RICIRCOLI/////////////////////////////////////////////////
//timezero viene posto = a zero nei file COR
// il tempo in TT degli eventi sarà calcolato come (MJDREFI +MJDREFF)*86400. + TIME[i]

  
//PRIMO RICIRCOLO SOLO EVENTI 21176
//SECONDO RICIRCOLO EVENTI E PACCHETTI 43222
//NUOVO START MISSION		       43315

// int diffobtsecttEVT         =    -99792000;
// int diffobtsecttEVT2015 =    -368227456;

// int diffobtmsectEVT = 0;
// int diffobtmsectEVT2015 = 4000; //microsecondi


		if (orbit <= 21176) {
			sec[i] = sec[i] - diffobtsecttEVT;
		
			}
		else if (orbit > 21176 && orbit < 43222){
		 	sec[i]=sec[i]+(1<<27)- diffobtsecttEVT;;
			}
		else 	if ((orbit >= 43222) && (orbit <= 43315) && (time[i] < 119148309061696.)){	
			sec[i]=sec[i]+(1<<28) - diffobtsecttEVT;	
			}
			
		else {
			sec[i]=sec[i]- diffobtsecttEVT2015;
			}
/////////////////////////////////////////////////////////////////////////////////
			
//////////////////////////////////////////////////////////////////////////						
///////////////////obtcorr e microsecondi//////////////////////////////////////////


		if (orbit >= 43315 ) {
			micsec[i] = micsec[i] - diffobtmsectEVT2015;
			}
			
		if ( micsec[i] >= 1000000) {
			micsec[i] = micsec[i] - 1000000;	
			sec[i] = sec[i] + 1;
			}
		time[i] = sec[i] + (((double)micsec[i])/1000000);
                if ((time[i]>=410313600.0) && (time[i]< limitobtleapsecond2017)) time[i]+=1;   		

		}
	
	fits_get_colnum(output, 1, "TIME", &colnum, &status);		
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);	

	
	fits_get_colnum(output, 1, "MICSECTT", &colnum, &status);
	fits_write_col(output, TUINT, colnum, 1, 1, nrows,  micsec, &status);	
		
	fits_get_colnum(output, 1, "SECONDTT", &colnum, &status);		
	fits_write_col(output, TUINT, colnum, 1, 1, nrows,  sec, &status);

		
////////passo 11
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	sprintf(unit, "TUNIT%d", colnum);
	fits_update_key(output, TSTRING, unit, s, NULL, &status);
	

	WriteTime(output, time[0], time[nrows-1]);


	delete[] time;
	delete[] micsec;
	delete[] sec;
//	delete[] obt_corr;	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	return status;
	}



int CorrTimeEVT3914(fitsfile * output, FILE *fp) {
//	int * obt_corr;
	int oldstatus = 0;
	int status = 0;
	int  i = 0;
	int colnum = 0;	
	int ncols = 0;		
	long nrows = 0;
	char unit[FLEN_KEYWORD];
	char str[FLEN_KEYWORD];
                
	char * s = "s";	
	double tstart = 0;
	double tstop = 0;
	float timezero = 0;	
	fits_movabs_hdu(output, 2, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);
	int * paktnumbPKT = new int [nrows];
	
	double *timePKT = new double [nrows];	
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, timePKT, NULL, &status);	
	
	fits_get_colnum(output, 1, "PAKTNUMB", &colnum, &status);		
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, paktnumbPKT, NULL, &status);		
		
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



/////////passo 5
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, NULL, &status);		
	
/////////passo 6 - 7 - 8

	
	
	fits_get_colnum(output, 1, "TIMEDAY", &colnum, &status);	
	fits_read_col(output, TSHORT, colnum, 1, 1, nrows, NULL, day, NULL, &status);	

	fits_get_colnum(output, 1, "TIMEMSEC", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, timemsec, NULL, &status);		
		
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, time, NULL, &status);	
		
	int orbit = 0;
	char orbitcomment[FLEN_KEYWORD];
  	fits_read_key(output, TINT, "OBS_ID", &orbit, orbitcomment,  &status);	  
	
	for (int i=0; i < nrows; i++) {
		day[i] = day[i] - 16801;
		if (day[i] < 0) day[i] = 0;

		if (orbit >= 43222 && orbit <= 43315 ) {
			day[i] += 3106;
			timemsec[i] += 76896000;			
			if ( timemsec[i] > 86400000) {
				timemsec[i] = timemsec[i] - 86400000;	
				day[i] = day[i] + 1;
				}
			}
			
                time[i] = 86400.*(double)day[i] + ((double)timemsec[i])/1000.;
                if ((time[i]>=410313600.0) && (time[i]< limitobtleapsecond2017)) time[i]+=1;   		
               
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

	int st = nrows-1;
	double htstart = 0;
	double htstop = 0;
	
	while ( st > 0) {
	    if (msgvali[st] == 0) {
		htstop = time[st];
		break;
		}
	    else {
		cout<<"tstop non valido"<<endl;	      
		st--;
		}  
	    }

	st = 0;
	while ( st <= nrows-1) {
	    if (msgvali[st] == 0) {
		htstart = time[st];
		break;
		}
	    else {
		cout<<"tstart non valido"<<endl;
		st++;
		}  
	    }


	WriteTime(output, htstart, htstop);		
	fits_movabs_hdu(output, 1, NULL, &status);	
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, NULL, &status);
	WriteTime(output, htstart, htstop);

////////passo 12

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
	fits_movabs_hdu(output, 3, NULL, &status);	
	long removallist[nrows];
	long numbertoremove = 0;
	for(int i = 0; i <= nrows-2; i++) {
		if (time[i] == time[i+1] && msgvali[i]==0 && msgvali[i+1]==0){
			removallist[numbertoremove++] = i;
			removallist[numbertoremove++] = i+1;
			fits_get_colnum(output, 1, "MSGVALI", &colnum, &status);
			short int msgval = 6;				
			fits_write_col(output, TSHORT, colnum, i+1,   1,  1,  &msgval, &status);	
			fits_write_col(output, TSHORT, colnum, i+2, 1,  1,  &msgval, &status);
// 			cout << i+1 << " ; " <<i+2<< endl;							
			}
		}		

	if (numbertoremove > 0) {
		cout<<"TROVATI TEMPI DOPPI:"<<endl;
		cout << "Deleted " << numbertoremove << " rows ; status = " <<status<< endl;
		}	

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
	return status;
	}



int CorrTimeEVT3916(fitsfile * output, FILE *fp) {
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
        
	int orbit = -1;
	fits_read_key(output, TINT, "CONTNUMB", &orbit, NULL, &status );	       
 	fits_movabs_hdu(output, 3, NULL, &status);	
	
	fits_get_num_rows(output, &nrows, &status);
	fits_get_num_cols(output, &ncols, &status);	
		
	int * timeweek =  new  int[nrows];
	int * timesec =  new  int[nrows];
	int * timesecf = new  int[nrows];	
	time = new double[nrows];

	tstart = 0;
	tstop = 0;
	char str[100];

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

// si esegue alla fine con il WriteTime	

/////////passo 5
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, NULL, &status);	

/////////passo 6 7 8 9


	fits_get_colnum(output, 1, "TIMEWEEK", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, timeweek, NULL, &status);	
			
	fits_get_colnum(output, 1, "TIMESEC", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, timesec, NULL, &status);		

			
	fits_get_colnum(output, 1, "TIMESECF", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, timesecf, NULL, &status);	

					
	fits_get_colnum(output, 1, "TIME", &colnum, &status);	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, time, NULL, &status);		

	

/// 6. subtracting 1252 from all the values in the TIMEWEEK column
// 
// 7. adding 259186 to all the values in the TIMESEC column (subtracting 604800 and adding 1 to
// 
// TIMEWEEK for rows in which TIMESEC becomes greater than 604800);
// 
// 8. changing all appearances of 999999 in the TIMESECF column to 1000000;
// 
// 9. setting the values in the TIME column equal to 604800.0× TIMEWEEK + TIMEMSEC +10−6×
// 
// TIMESECF;
// 
// 10. replacing the corresponding TUNIT keyword with ’s’
// 
// 11. changing TSTART to the first value of the column TIME
// 
// 12. changing TSTOP to the last value of the column TIME
// // 	
	
	for (int i=0; i < nrows; i++) {

		if ( timesecf[i] == 999999) {
			timesec[i] = timesec[i] + 1;	
			timesecf[i] = 0;
			}

		if (orbit > 55621) {
			timeweek[i] = timeweek[i] -1252 +1024;
			}
		else {
			timeweek[i] = timeweek[i] -1252;
			}
		if (timeweek[i] < 0) timeweek[i] = 0;
		
		time[i] = (double)(timeweek[i]*604800.) + timesec[i] + ((double)timesecf[i])/1000000;	
		
		//AGGIUNGIAMO I LEAP SECOND
// 		cout<<"ENTRATO QUI"<<endl;
		if (time[i] < 157852800.00000) {
			timesec[i] = timesec[i] + 259200 - utc_gps_offset;	
			}
		else  if((time[i] >= 157852800.00000) && (time[i] < 268185600.)) {
		
			timesec[i] = timesec[i] + 259200 - utc_gps_offset -1;
			}
		else if ((time[i] >= 268185600.) && (time[i] < 362793600.) ){		
			timesec[i] = timesec[i] + 259200 - utc_gps_offset -2;		
			}
		else if ((time[i] >= 362793600.) && (time[i] < 410313600.) ){		
			timesec[i] = timesec[i] + 259200 - utc_gps_offset -3;		
			}				
		else {
			//1 Luglio 2015
			timesec[i] = timesec[i] + 259200 - utc_gps_offset -4;
			}
				
		
		
		
		if ( timesec[i] > 604800) {
			timesec[i] = timesec[i] - 604800;	
			timeweek[i] = timeweek[i] + 1;
			}
			
 		time[i] = (double)(timeweek[i]*604800.) + timesec[i] + ((double)timesecf[i])/1000000;
		
		

                if ((time[i]>=410313600.0) && (time[i]< limitobtleapsecond2017)) time[i]+=1;   		
			
			
		}
				
	
	fits_get_colnum(output, 1, "TIME", &colnum, &status);	
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);
				
	fits_get_colnum(output, 1, "TIMEWEEK", &colnum, &status);
	fits_write_col(output, TINT, colnum, 1, 1, nrows,  timeweek, &status);	
	
	fits_get_colnum(output, 1, "TIMESEC", &colnum, &status);		
	fits_write_col(output, TINT, colnum, 1, 1, nrows,  timesec, &status);		
	
	fits_get_colnum(output, 1, "TIMESECF", &colnum, &status);		
	fits_write_col(output, TINT, colnum, 1, 1, nrows,  timesecf, &status);	

////////passo 10
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	sprintf(unit, "TUNIT%d", colnum);
	fits_update_key(output, TSTRING, unit, s, NULL, &status);

/////////passo 11 12
	WriteTime(output, time[0], time[nrows-1]);


	
	delete [] time;	
	delete [] timeweek;	
	delete [] timesec;	
	delete [] timesecf;	
        
        
        
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	return status;
	}


	
int CorrTimeEVT3904(fitsfile * output, FILE *fp) {
//	int * obt_corr;
	int oldstatus = 0;
	int status = 0;
	int  i = 0;
	int colnum = 0;	
	int ncols = 0;		
	long nrows = 0;
	double * time = NULL;
	char unit[FLEN_KEYWORD];
	char * s = "s";	
	double tstart = 0;
	double tstop = 0;
	float timezero = 0;	
	
	fits_movnam_hdu(output, BINARY_TBL, "EVENTS", 0, &status);	
	fits_get_num_rows(output, &nrows, &status);
	fits_get_num_cols(output, &ncols, &status);
	
	time = new double[nrows];
	int * micsec = new int[nrows];
	int * sec = new int[nrows];

// 	obt_corr = new int[nrows];
// 
// 	
	tstart = 0;
	tstop = 0;
	char str[100];


/////////passo 1
	
	strcpy(str, "TT");
	fits_update_key(output, TSTRING, "TIMESYS",  str, NULL, &status);

/////////passo 2
	oldstatus = status;
	fits_delete_key(output, "MJDREF", &status);
	if (status == 202) status = oldstatus;

/////////passo 3
	fits_update_key(output, TDOUBLE, "MJDREFI",  &mjdrefi, NULL, &status);


	
/////////passo 5
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, str, &status);	

/////////passo 6
	fits_update_key(output, TDOUBLE, "OBT0I",  &obtoi, NULL, &status);

/////////passo 7
	if(time[0] < 157852800.00000) {
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof, NULL, &status);
		}
	else if((time[0] >= 157852800.00000) && (time[0] < 268185600.)){
	// 	cout<<"nuovo leap second pkt "<<mjdreff_2009<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2009, NULL, &status);
		}
	else if((time[0] >= 268185600) && (time[0] < 362793600)){
	// 	cout<<"nuovo leap second pkt "<<mjdreff_2009<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2012, NULL, &status);
		}
	else if((time[0] >= 362793600) && (time[0] < 410313600)) {
		cout<<"nuovo leap second pkt 2015 "<<mjdreff_2015<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2015, NULL, &status);
		}	
        else {
 		cout<<"nuovo leap second pkt 2017 "<<mjdreff_2017<<endl;
		fits_update_key(output, TDOUBLE, "OBT0F",  &obtof_2017, NULL, &status);            
                }
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/////////passo 8 - 9 - 10
	fits_get_colnum(output, 1, "TIME", &colnum, &status);	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, time, NULL, &status);		
	
/*	fits_get_colnum(output, 1, "OBTCORR", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, obt_corr, NULL, &status);*/		
		
	for (i = 0; i < nrows; ++i) {
		time[i] = time[i]/1000 -diffobtsecttEVT;
                if ((time[i]>=410313600.0) && (time[i]< limitobtleapsecond2017)) time[i]+=1;                   
		}
	
	fits_get_colnum(output, 1, "TIME", &colnum, &status);		
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);	

	
	fits_get_colnum(output, 1, "MICSECTT", &colnum, &status);
	fits_write_col(output, TUINT, colnum, 1, 1, nrows,  micsec, &status);	
		
	fits_get_colnum(output, 1, "SECONDTT", &colnum, &status);		
	fits_write_col(output, TUINT, colnum, 1, 1, nrows,  sec, &status);

		
////////passo 11
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	sprintf(unit, "TUNIT%d", colnum);
	fits_update_key(output, TSTRING, unit, s, NULL, &status);
	

	WriteTime(output, time[0], time[nrows-1]);	
	fits_movabs_hdu(output, 1, NULL, &status);	
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, str, &status);
	WriteTime(output, time[0], time[nrows-1]);			


	delete[] time;
	delete[] micsec;
	delete[] sec;
//	delete[] obt_corr;	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	return status;
	}



int CorrTimePKT(fitsfile * output, FILE *fp) {
	int oldstatus = 0;
	int status = 0;
	int  i = 0;
	int colnum = 0;	
	int ncols = 0;		
	int * secpktobt;
	int * microsecpktobt;	
	long nrows = 0;
	double * time = NULL;
	char unit[FLEN_KEYWORD];
	char * s = "s";	
	int PKTTYPE = 0;
	int PKTSTYPE = 0;	
///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////PACKETS//////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////
	
	fits_movabs_hdu(output, 2, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);
	fits_get_num_cols(output, &ncols, &status);	
		
	time = new double[nrows];
	microsecpktobt = new int[nrows];	
	secpktobt = new int[nrows];

	char str[100];

	int orbit = 0;
	char orbitcomment[FLEN_KEYWORD];
  	fits_read_key(output, TINT, "OBS_ID", &orbit, orbitcomment,  &status);	    	

	fits_read_key(output, TINT, "PKTTYPE", &PKTTYPE, NULL, &status );
	fits_read_key(output, TINT, "PKTSTYPE", &PKTSTYPE, NULL, &status );	
		
	
/////////passo 1
	
	strcpy(str, "TT");
	fits_update_key(output, TSTRING, "TIMESYS",  str, NULL, &status);

/////////passo 2
	oldstatus = status;
	fits_delete_key(output, "MJDREF", &status);
	if (status == 202) status = oldstatus;
	
/////////passo 3
	fits_update_key(output, TDOUBLE, "MJDREFI",  &mjdrefi, NULL, &status);	
	
/////////passo 5
	float timezero = 0;
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, str, &status);	

/////////passo 6 - 7 - 8
	colnum = 20;
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, time, NULL, &status);
	
	fits_get_colnum(output, 1, "OBTMSETT", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, microsecpktobt, NULL, &status);
	
	fits_get_colnum(output, 1, "OBTSECTT", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, secpktobt, NULL, &status);	

	
	
	for (i = 0; i < nrows; ++i) {
		// int diffobtsectt = 756950334;
		// double diffobtmsect = 816000.0;
		microsecpktobt[i] = microsecpktobt[i] - (int)diffobtmsect;

		if (orbit >= 43222 && orbit <= 43315 && (time[i] < 1.12510085551597E+15 )){		
			if (PKTTYPE==1 && PKTSTYPE ==1) {
				secpktobt[i] = secpktobt[i] -diffobtsectt;
				}
			else {
				secpktobt[i] = secpktobt[i]  +(1<<28) -diffobtsectt;
				}
		
			
			}		
		else 	if (orbit >= 43315){		        
			
			if (PKTTYPE==1 && PKTSTYPE ==1) {
// 				cout<<"entrato qui"<<endl;
				secpktobt[i] = secpktobt[i] -diffobtsectt2015 -77056;
				}
			else {
				secpktobt[i] = secpktobt[i]   -diffobtsectt2015;
				}
			}
		else {	
			secpktobt[i] = secpktobt[i] - diffobtsectt;
			}
		if ( microsecpktobt[i] < 0) {
			microsecpktobt[i] = microsecpktobt[i] + 1000000;	
			secpktobt[i] = secpktobt[i] - 1;
			}
		time[i] = secpktobt[i] + (((double)microsecpktobt[i])/1000000);
                
                //gestione Leap Second 1 Gennaio 2017
                if ((time[i]>=410313600.0) && (time[i]< limitobtleapsecond2017)) {
                    if (i==0) printf("Post Leap Second 2017 before Switch On/Off GPS -> %f ",time[i]);
                    time[i]+=1;
                    if (i==0) printf("- %f \n",time[i]);                    
                    }
                
		}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////cor_3908/////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

	
	fits_get_colnum(output, 1, "OBTMSETT", &colnum, &status);
	fits_write_col(output, TINT, colnum, 1, 1, nrows,  microsecpktobt, &status);	
	
	fits_get_colnum(output, 1, "OBTSECTT", &colnum, &status);		
	fits_write_col(output, TINT, colnum, 1, 1, nrows,  secpktobt, &status);	

	colnum = 20;
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);

/////////passo 9
	sprintf(unit, "TUNIT%d", colnum);
	fits_update_key(output, TSTRING, unit, s, NULL, &status);

/////////passo 10 - 11
	WriteTime(output, time[0], time[nrows-1]);
	fits_movabs_hdu(output, 1, NULL, &status);	
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, str, &status);
	WriteTime(output, time[0], time[nrows-1]);	


	delete [] secpktobt;	
	delete [] time;	
	delete [] microsecpktobt;		
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	return status;
}



int CorrTimePKT3911(fitsfile * output, FILE *fp) {

	int oldstatus = 0;
	int status = 0;
	int  i = 0;
	int colnum = 0;	
	int ncols = 0;		
	int * secpktobt;
	int * microsecpktobt;	
	long nrows = 0;
	double * time = NULL;
	char unit[FLEN_KEYWORD];
	char * s = "s";	

	char str[100];

	int orbit = 0;
	char orbitcomment[FLEN_KEYWORD];
  	fits_read_key(output, TINT, "OBS_ID", &orbit, orbitcomment,  &status);	   	
	
for (i = 3; i <= 4; ++i) {	
///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////PACKETS//////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////



	fits_movabs_hdu(output, i, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);
	fits_get_num_cols(output, &ncols, &status);	
	cout<<nrows<<endl;		
	time = new double[nrows];
	microsecpktobt = new int[nrows];	
	secpktobt = new int[nrows];

	char str[100];

/////////passo 1
	
	strcpy(str, "TT");
	fits_update_key(output, TSTRING, "TIMESYS",  str, NULL, &status);

/////////passo 2
	oldstatus = status;
	fits_delete_key(output, "MJDREF", &status);
	if (status == 202) status = oldstatus;
	
/////////passo 3
	fits_update_key(output, TDOUBLE, "MJDREFI",  &mjdrefi, NULL, &status);

	
/////////passo 5
	float timezero = 0;
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, str, &status);	

/////////passo 6 - 7 - 8
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, time, NULL, &status);
	
	for (int k = 0; k < nrows; ++k) {
	
		time[k] = (time[k]/1000000.);
		
		if (orbit >= 43315){		        
			time[k]=time[k]+(1<<28)+77056;
			}
		time[k] = time[k] - diffobtsectt;	
                
                if ((time[k]>=410313600.0) && (time[k]< limitobtleapsecond2017)) time[k]+=1;		
		}

	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);

/////////passo 9
	sprintf(unit, "TUNIT%d", colnum);
	fits_update_key(output, TSTRING, unit, s, NULL, &status);

/////////passo 10 - 11
	WriteTime(output, time[0], time[nrows-1]);
	fits_movabs_hdu(output, 1, NULL, &status);	
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, str, &status);
	WriteTime(output, time[0], time[nrows-1]);	


	delete [] secpktobt;	
	delete [] time;	
	delete [] microsecpktobt;
}			
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	



	return status;
}



int CorrTimePKT3908(fitsfile * output, FILE *fp) {

	int oldstatus = 0;
	int status = 0;
	int  i = 0;
	int colnum = 0;	
	int ncols = 0;		
	int * secpktobt;
	int * microsecpktobt;	
	long nrows = 0;
	double * time = NULL;
	char unit[FLEN_KEYWORD];
	char * s = "s";	


///////////////////////////////////////////////////////////////////////////






	fits_movabs_hdu(output, 2, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);

	int * bstarthi = new int[nrows];
	int * bstartlo = new int[nrows];
	int * bstophi = new int[nrows];
	int * bstoplo = new int[nrows];
	short * obt_corr = new short[nrows];
	double * bstart = new double[nrows];
	double * bstop  =  new double[nrows];

	fits_get_num_cols(output, &ncols, &status);

	fits_get_colnum(output, 1, "BSTARTHI", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, bstarthi, NULL, &status);
	
	fits_get_colnum(output, 1, "BSTARTLO", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, bstartlo, NULL, &status);

	fits_get_colnum(output, 1, "BSTOPHI", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, bstophi, NULL, &status);
	
	fits_get_colnum(output, 1, "BSTOPLO", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, bstoplo, NULL, &status);

	fits_get_colnum(output, 1, "OBTCORR", &colnum, &status);	
	fits_read_col(output, TSHORT, colnum, 1, 1, nrows, NULL, obt_corr, NULL, &status);	

	char str[100];

	int orbit = 0;
	char orbitcomment[FLEN_KEYWORD];
  	fits_read_key(output, TINT, "OBS_ID", &orbit, orbitcomment,  &status);	   
	
	for (i = 0; i < nrows; ++i) {
// 		int diffobtsecttEVT = 99791934;
// 		double diffobtmsectEVT = 816000.0;
// 		double microsec=MicSecCol*(1E-6)*(1000000.0/(1000000.0+ObtCor));
	  
	  
///////////////////obtcorr//////////////////////////////////////////
		bstartlo[i] = (int)(((double)bstartlo[i])*(1000000.0/(1000000.0+obt_corr[i])));
		if ( bstartlo[i] < 0) {
			bstartlo[i] = bstartlo[i] + 1000000;	
			bstarthi[i] = bstarthi[i] - 1;
			}
		if ( bstartlo[i] > 1000000) {
			bstartlo[i] = bstartlo[i] - 1000000;	
			bstarthi[i] = bstarthi[i] + 1;
			}
			
		bstoplo[i] = (int)(((double)bstoplo[i])*(1000000.0/(1000000.0+obt_corr[i])));
		if ( bstoplo[i] < 0) {
			bstoplo[i] = bstoplo[i] + 1000000;	
			bstophi[i] = bstophi[i] - 1;
			}	  
		if ( bstoplo[i] > 1000000) {
			bstoplo[i] = bstoplo[i] - 1000000;	
			bstophi[i] = bstophi[i] + 1;
			}	  
	  
/////////////////////////////////////////////////////////////
	  
		if (orbit < 43222) {
			bstarthi[i] = bstarthi[i] - diffobtsecttEVT;
			bstophi[i]  = bstophi[i] - diffobtsecttEVT;
			bstartlo[i] = bstartlo[i]- diffobtmsectEVT;
			bstoplo[i]  = bstoplo[i] - diffobtmsectEVT;	  
			}
		else 	if (orbit >= 43222 && orbit <= 43315){	
// 			sec[i]=sec[i]+(1<<28) - diffobtsecttEVT;
			bstarthi[i] = bstarthi[i] +(1<<28)- diffobtsecttEVT;
			bstophi[i]  = bstophi[i] +(1<<28)- diffobtsecttEVT;
			bstartlo[i] = bstartlo[i]- diffobtmsectEVT;
			bstoplo[i]  = bstoplo[i] - diffobtmsectEVT;				
			}
// 			
		else if (orbit >= 43315) {
			bstarthi[i] = bstarthi[i] - diffobtsecttEVT2015;
			bstophi[i]  = bstophi[i] - diffobtsecttEVT2015;
			bstartlo[i] = bstartlo[i] - diffobtmsectEVT2015;
			bstoplo[i]  = bstoplo[i] - diffobtmsectEVT2015;		
			}


		bstart[i] = bstarthi[i] + (((double)bstartlo[i])/1000000);
		bstop[i] = bstophi[i] + (((double)bstoplo[i])/1000000);

                if ((bstart[i]>=410313600.0) && (bstart[i]< limitobtleapsecond2017)) {
                        bstart[i]+=1;
                        }
                if ((bstop[i]>=410313600.0) && (bstop[i]< limitobtleapsecond2017)) {
                        bstop[i]+=1;
                        }                   
		
		}

	fits_insert_col(output, ncols+5, "BSTART", "1D", &status);
	fits_insert_col(output,  ncols+5, "BSTOP" , "1D", &status);


	fits_get_colnum(output, 1, "BSTART", &colnum, &status);
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  bstart, &status);

	fits_get_colnum(output, 1, "BSTOP", &colnum, &status);
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  bstop, &status);



///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////PACKETS//////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////



	fits_movabs_hdu(output, 3, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);
	fits_get_num_cols(output, &ncols, &status);	
	
	time = new double[nrows];
	microsecpktobt = new int[nrows];	
	secpktobt = new int[nrows];


/////////passo 1
	
	strcpy(str, "TT");
	fits_update_key(output, TSTRING, "TIMESYS",  str, NULL, &status);

/////////passo 2
	oldstatus = status;
	fits_delete_key(output, "MJDREF", &status);
	if (status == 202) status = oldstatus;
	
/////////passo 3
	fits_update_key(output, TDOUBLE, "MJDREFI",  &mjdrefi, NULL, &status);

/////////passo 5
	float timezero = 0;
	fits_update_key(output, TFLOAT, "TIMEZERO",  &timezero, str, &status);	

/////////passo 6 - 7 - 8
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, time, NULL, &status);
	
	for (int k = 0; k < nrows; ++k) {
	
		time[k] = (time[k]/1000000);
		
		
		if (orbit >= 43222 && orbit < 43315){		        
			time[k] = time[k] +(1<<28) - diffobtsectt;			
			}		
		
		else if (orbit >= 43315){		        
			time[k] = time[k] - diffobtsectt2015;
			}
		else {	
			time[k] = time[k] - diffobtsectt;
			}
			
		if ((time[k]>=410313600.0) && (time[k]< limitobtleapsecond2017)) time[k]+=1;		
			
		}

	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);

/////////passo 9
	sprintf(unit, "TUNIT%d", colnum);
	fits_update_key(output, TSTRING, unit, s, NULL, &status);

/////////passo 10 - 11
	WriteTime(output, time[0], time[nrows-1]);

	delete [] secpktobt;	
	delete [] time;	
	delete [] microsecpktobt;
		
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	



	return status;
}



int CorrTimePKTSA(fitsfile * output, FILE *fp) {
	int oldstatus = 0;
	int status = 0;
	int colnum = 0;	
	int ncols = 0;	
	int orbit = 0;
	int * secpktobt;
	int * microsecpktobt;	
	long nrows = 0;
	char unit[FLEN_KEYWORD];
	char * s = "s";	
	int PKTTYPE = 0;
	int PKTSTYPE = 0;

	char orbitcomment[FLEN_KEYWORD];	
///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////PACKETS//////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////
	
	fits_movabs_hdu(output, 2, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);
	fits_get_num_cols(output, &ncols, &status);	

	
	
	fits_read_key(output, TINT, "OBS_ID", &orbit, orbitcomment,  &status);		
	
// 	cout<<orbit<<endl;
	int * OBTFSDH = new int[nrows];
	int * OBTSDGH = new int[nrows];	
	
	int * OBTSDGL = new int[nrows];		
	int * OBTFSDL = new int[nrows];
	

	int * OBTSDGUL = new int[nrows];
	double * time = new double[nrows];
	double * SASTART = new double[nrows];
	
	fits_get_colnum(output, 1, "OBTSDGH", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, OBTSDGH, NULL, &status);	
	
	fits_get_colnum(output, 1, "OBTSDGL", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, OBTSDGL, NULL, &status);	
	
	
	fits_get_colnum(output, 1, "OBTFSDH", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, OBTFSDH, NULL, &status);		
	
	fits_get_colnum(output, 1, "OBTFSDL", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, OBTFSDL, NULL, &status);	
	
	fits_get_colnum(output, 1, "OBTSDGUL", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, OBTSDGUL, NULL, &status);	
	
/*;       v5.2: following the email by LN (4/12/2007), if OBTSDGUL>31250
;             (corresponding to 0.5 s), the offset_time
;             (i.e. start_observation) is truncated +1 s and if
;             OBTSDGUL<=31250 the offset_time remains truncated, 
;             EDM, 19/12/2007;    */     

//          if offset_ulow le 31250 then $
//            offset_time=OBT_reconstruct(offset_high, offset_low, 0) ; s
// 
//          if offset_ulow gt 31250 then $
//            offset_time=OBT_reconstruct(offset_high, offset_low, 0)+1.d ; s

        
	for (int i = 0; i < nrows; ++i) {
// 		time[i]=65536.*OBTSDGH[i]+1.*OBTSDGL[i]+16e-6.*OBTSDGUL[i] -diffobtsectt2015;
		if (orbit>43315) {
			SASTART[i]    = 65536.*OBTSDGH[i]+1.*OBTSDGL[i]-diffobtsecttEVT2015;
			time[i] = 65536.*OBTFSDH[i]+1.*OBTFSDL[i] -diffobtsecttEVT2015;	
			}
		else {
			SASTART[i]    = 65536.*OBTSDGH[i]+1.*OBTSDGL[i] -diffobtsecttEVT;
			time[i] = 65536.*OBTFSDH[i]+1.*OBTFSDL[i] -diffobtsecttEVT;		  
		  
			}
			
		if(OBTSDGUL[i] >31250) {
//                     cout<<OBTSDGUL[i]<<endl;
                        SASTART[i]    += 1;
                        time[i] += 1;	                   
                        }
		
		if ((time[i]>=410313600.0) && (time[i]< limitobtleapsecond2017)) {time[i]+=1; SASTART[i]+=1;}		
  
		}
		
// 	printf("%f %f %d\n", time[0], time[nrows-1], diffobtsecttEVT2015);	
	
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);	
	fits_get_colnum(output, 1, "PKTOBT", &colnum, &status);
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);		
	fits_get_colnum(output, 1, "SASTART", &colnum, &status);
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  SASTART, &status);	
	
	
	WriteTime(output, time[0], time[nrows-1]);	
	

	delete [] OBTFSDH;	
	delete [] OBTSDGH;	
	delete [] OBTSDGL;
	
	delete [] OBTFSDL;	
	delete [] OBTSDGUL;	
	delete [] time;	
	delete [] SASTART;		
	
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	return status;
}



int CorrTimePKTSAEVT(fitsfile * output, FILE *fp) {
	int oldstatus = 0;
	int status = 0;
	int colnum = 0;	
	int ncols = 0;		
	int * secpktobt;
	int * microsecpktobt;	
	long nrows = 0;
	char unit[FLEN_KEYWORD];
	char * s = "s";	
	int PKTTYPE = 0;
	int PKTSTYPE = 0;	
///////////////////////////////////////////////////////////////////////////////////////////////			
//////////////////////////////PACKETS//////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////////////////////////////
	
	
	fits_movabs_hdu(output, 2, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);
	
		

	
	double * SASTART = new double[nrows];		
	
	
	fits_get_colnum(output, 1, "SASTART", &colnum, &status);	
	fits_read_col(output, TDOUBLE, colnum, 1, 1, nrows, NULL, SASTART, NULL, &status);	
	
	fits_movabs_hdu(output, 3, NULL, &status);	
	fits_get_num_rows(output, &nrows, &status);		
	
	
	
	double  * time = new double[nrows];
	int * ABTIME_S = new int[nrows];		
	int * ABTIME_U = new int[nrows];	
	int * PAKTNUMB = new int[nrows];
	int * EVTYPE   = new int[nrows];
	int * SAIE_ID   = new int[nrows];        
	int * DIFFTIME   = new int[nrows];   	
	double partial_time_0 = 0;
	double partial_time_1 = 0;        
	double abs_time_0     = 0; 
	double abs_time_1     = 0;         
	fits_get_colnum(output, 1, "PAKTNUMB", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, PAKTNUMB, NULL, &status);
	
	fits_get_colnum(output, 1, "ABTIME_S", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, ABTIME_S, NULL, &status);		
	
	fits_get_colnum(output, 1, "ABTIME_U", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, ABTIME_U, NULL, &status);	
	
	fits_get_colnum(output, 1, "EVTYPE", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, EVTYPE, NULL, &status);

	fits_get_colnum(output, 1, "SAIE_ID", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, SAIE_ID, NULL, &status);        
	fits_get_colnum(output, 1, "DIFFTIME", &colnum, &status);	
	fits_read_col(output, TINT, colnum, 1, 1, nrows, NULL, DIFFTIME, NULL, &status);        

        int firstevt4= 0;
	for (int i = 0; i < nrows; ++i) {
	  
		int indPkt = PAKTNUMB[i]-1;
                
                if (EVTYPE[i]==4) {
                     if (firstevt4==0) firstevt4=i;
                     if(SAIE_ID[i]==0) {
                            double abs_prev = abs_time_0;
                            abs_time_0=  ABTIME_S[i] + (((double)ABTIME_U[i])/1000000);
                            partial_time_0=0;
                            if(abs_time_0<abs_prev && abs_prev>32768) printf("RECICLE!:     %f %f %d\n  ",abs_prev,abs_time_0,i);
                            time[i] = abs_time_0+ SASTART[indPkt];
                            }
                     else {
                            double abs_prev = abs_time_1;
                            abs_time_1=  ABTIME_S[i] + (((double)ABTIME_U[i])/1000000);
                            partial_time_1=0;
                            if(abs_time_1<abs_prev && abs_prev>32768) printf("RECICLE!:     %f %f %d\n  ",abs_prev,abs_time_1,i);
                            time[i] = abs_time_1+ SASTART[indPkt];                
                        }
                    }
                if (EVTYPE[i]==3 || EVTYPE[i]==1) {
                            if (i>0)  time[i] =  time[i-1];
                            else   time[i] = ABTIME_S[i] + (((double)ABTIME_U[i])/1000000)+ SASTART[indPkt];                
                    }                    
                    
                 if (EVTYPE[i]==0 || EVTYPE[i]==2) {
                     if(SAIE_ID[i]==0) {
                            partial_time_0+=DIFFTIME[i]*2;  
                            time[i] = abs_time_0+(partial_time_0/1e6) + SASTART[indPkt];
                            }
                     else {
                            partial_time_1+=DIFFTIME[i]*2;  
                            time[i] = abs_time_1+(partial_time_1/1e6) + SASTART[indPkt];   
                     }
                    }                   
                    
		

		}

	
	fits_get_colnum(output, 1, "TIME", &colnum, &status);
	fits_write_col(output, TDOUBLE, colnum, 1, 1, nrows,  time, &status);		
	WriteTime(output, time[firstevt4], time[nrows-1]);	
	

        
        
 	delete [] EVTYPE;	
	delete [] SAIE_ID;	       
	delete [] DIFFTIME;	
	delete [] SASTART;	
	delete [] time;	
	delete [] ABTIME_S;	
	delete [] ABTIME_U;	
	delete [] PAKTNUMB;
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////	
	return status;
}



int euler(double ai, double bi, double * ao, double * bo, int select) {
	double twopi   =   2*PI;
	double fourpi  =   4*PI;
	double deg_to_rad = 360/twopi;
	
	double psi[6]   = { 0.57477043300, 4.9368292465, 0.00000000000, 0.0000000000, 0.11142137093, 4.71279419371};     
	double stheta[6] ={ 0.88998808748,-0.88998808748, 0.39777715593,-0.39777715593, 0.86766622025,-0.86766622025};   
	double ctheta[6] ={ 0.45598377618, 0.45598377618, 0.91748206207, 0.91748206207, 0.49714719172, 0.49714719172};   
	double phi[6]  = { 4.9368292465, 0.57477043300, 0.0000000000, 0.00000000000, 4.71279419371, 0.11142137093};

	int i  = select - 1;   
	double a  = ai/deg_to_rad - phi[i];
	double b = bi/deg_to_rad;
	double sb = sin(b);
	double cb = cos(b);
	double cbsa = cb * sin(a);
 	b  = -stheta[i] * cbsa + ctheta[i] * sb;
	*bo    = asin(b<1.0?b:1.0)*deg_to_rad;
 	a =  atan2( ctheta[i] * cbsa + stheta[i] * sb, cb * cos(a) );
	double x = fmod(a+psi[i]+fourpi,  twopi);
 	*ao = x*deg_to_rad;
	return 0;
	}


int min(double A[], int n){
	int i=0;
	double min = A[0];
	int jmin = 0;
	for (i=1; i<n; i++) {
		if (A[i] < min) {
			min = A[i];
			jmin = i;
			}
		}
	return(jmin);
	}

int max(double A[], int n){
	int i=0;
	double max = A[0];
	int jmax = 0;

	for (i=1; i<n; i++) {
		if (A[i] > max) {
			max = A[i];
			jmax = i;
			}
		}
	return(jmax);
	}

int minint(int A[], int n){
	int i=0;
	int min = A[0];
	int jmin = 0;
	for (i=1; i<n; i++) {
		if (A[i] < min) {
			min = A[i];
			jmin = i;
			}
		}
	return(jmin);
	}

int maxint(int A[], int n){
	int i=0;
	int max = A[0];
	int jmax = 0;

	for (i=1; i<n; i++) {
		if (A[i] > max) {
			max = A[i];
			jmax = i;
			}
		}
	return(jmax);
	}

int binarySearch(double * vect, int from, int to, double v, float diff) {
        if (from > to)  
		return -1;
	
        int mid = (from + to) / 2;
	if (v < 0)
		v = 0;
	double value = fabs(vect[mid]-v);

        if (value <= diff) {     // a[mid] == v
                return mid;
			}
        else {
		value = vect[mid]-v;
		if (value < 0)     // a[mid] < v
			return binarySearch(vect, mid + 1, to, v, diff);
		else
			return binarySearch(vect, from, mid - 1, v, diff);
		}
	return 0;
	}
	
void carxradec(float * Xp, float * dec, float * ra){


	double r = sqrt( Xp[0]*Xp[0] + Xp[1]*Xp[1] + Xp[2]*Xp[2]);
	
	*dec = 90 - acosd(Xp[2]/r);

	if ( *dec == 90 || *dec == -90){
		*ra  = 0;
		}
	else {
		*ra  = atan2d(Xp[1],Xp[0]);
		}
/*






	if (Xp[0]==0 && Xp[1]==0) {
		*ra=0;
		}	
	else if (Xp[1]==0) {
		if(Xp[0]<0)
			*ra = 180;
		else
			*ra = 0;
		}
	else if (Xp[0]==0 && Xp[1]!=0 ) {
		if(Xp[1]<0)
			*ra = 270;
		else
			*ra = 90;
		}
	else if (Xp[0] < 0){
		*ra = atand( Xp[1]/Xp[0]) + 180;
//		*ra = (90/(asin(1))) * atan( Xp[1]/Xp[0]) + 180;		
		}
	else if (Xp[0] > 0 && Xp[1] < 0){
		*ra = (90/(asin(1)))*atan(Xp[1]/Xp[0]) + 360;
		}
	else if (Xp[0] > 0 && Xp[1] > 0){
		*ra = (90/(asin(1)))*atan(Xp[1]/Xp[0]);
		}			
	*dec=(90/(asin(1)))*asin(Xp[2]/sqrt(Xp[0]*Xp[0]+Xp[1]*Xp[1]+Xp[2]*Xp[2]));*/
	}	
    
float * ProdQuat(float *Q_1, float *Q_2) {
	float * temp=new float[4];

	temp[3] = Q_1[3]*Q_2[3] -  Q_1[0]*Q_2[0] - Q_1[1]*Q_2[1] - Q_1[2]*Q_2[2];
	temp[0] = Q_1[0]*Q_2[3] +  Q_1[3]*Q_2[0] + Q_1[1]*Q_2[2] - Q_1[2]*Q_2[1];
	temp[1] = Q_1[3]*Q_2[1] -  Q_1[0]*Q_2[2] + Q_1[1]*Q_2[3] + Q_1[2]*Q_2[0];
	temp[2] = Q_1[3]*Q_2[2] +  Q_1[0]*Q_2[1] - Q_1[1]*Q_2[0] + Q_1[2]*Q_2[3];

	return temp;
	}

float * ConQuat(float *Q_1) {
	float * temp=new float[4];
	temp[0] = -Q_1[0];
	temp[1] = -Q_1[1];
	temp[2] = -Q_1[2];
	temp[3] =  Q_1[3];
	return temp;
	}


int WriteTime(fitsfile * input, double t1, double t2) {

	int status = 0;
	int hdnum = 0;
	fits_get_num_hdus(input, &hdnum, &status);
	
	int i;
	fits_get_hdu_num(input, &i);

	char str[100];
	strcpy(str, "TT");
	float timezero = 0.0;
	char * secstr = "s";
	
	fits_update_key(input, TDOUBLE, "TSTART", &t1,  "[OBT]first event time", &status);
	fits_update_key(input, TDOUBLE, "TSTOP", &t2,  "[OBT]last event time", &status);
	fits_update_key(input, TSTRING, "TIMESYS",  str, NULL, &status);
	fits_update_key(input, TSTRING, "TIMEUNIT",  secstr, NULL, &status);
	int oldstatus = status;
	
	double mjdreff_loc =  0;
	
/////////passo 4

	if(t1 < 157852800.00000) {
	// 	cout<<"vecchio leap second pkt "<<time[0]<<endl;
		mjdreff =  mjdreff;
		}
	else if((t1 >= 157852800.00000) && (t1 < 268185600.)){
		cout<<"WriteTime Last Leap Second 2009 - MJDREFF ="<<mjdreff_2009<<endl;
		mjdreff =  mjdreff_2009;		}
	else if((t1 >= 268185600) && (t1 < 362793600)){
		cout<<"WriteTime Last Leap Second 2012 - MJDREFF ="<<mjdreff_2012<<endl;
		mjdreff =  mjdreff_2012;
		}		
	else if((t1>= 362793600) && (t1 < 410313600)) {
		cout<<"WriteTime Last Leap Second 2015 - MJDREFF ="<<mjdreff_2015<<endl;
		mjdreff =  mjdreff_2015;
		}	
        else {
 		cout<<"WriteTime Last Leap Second 2017 - MJDREFF ="<<mjdreff_2017<<endl;
		mjdreff =  mjdreff_2017;
                }
	
	
	fits_delete_key(input, "MJDREF", &status);
	if (status == 202) status = oldstatus;
	fits_update_key(input, TDOUBLE, "MJDREFI",  &mjdrefi, NULL, &status);
	fits_update_key(input, TDOUBLE, "MJDREFF",  &mjdreff, NULL, &status);
	fits_update_key(input, TFLOAT, "TIMEZERO",  &timezero, str, &status);			
		
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////	
	
	fits_update_key(input, TFLOAT, "TIMEZERO",  &timezero, str, &status);	

	time_t tt = 0;
	tm * tstr = gmtime(&tt);
	tstr->tm_year = 107;
	tstr->tm_mon = 0;
	tstr->tm_mday = 1;
	tstr->tm_hour = 0;
	tstr->tm_min = 0;
	tstr->tm_sec = 0;
	tstr->tm_isdst = 0;
	
	
	tt = timegm(tstr);
	
	double tinc = difftime(tt+1,tt);

	tt += (time_t)(((mjdrefi - 54101 + mjdreff) * 86400. + timezero + t1) / tinc) ;
	
	tstr = gmtime(&tt);
	
	double inttstart = 0;

	
	double sec = tstr->tm_sec + modf(t1, &inttstart);
	
	double test = ((mjdrefi-54101+mjdreff)*86400.+t1);
	
	double millisec = tstr->tm_sec +modf(test, &inttstart);
// 	print
// 	printf ("%f = %f + %f \n", test, inttstart, millisec);

	char datestr[FLEN_KEYWORD];
	char datestr1[FLEN_KEYWORD];	

	
	sprintf(datestr,"%04d-%02d-%02dT%02d:%02d:%06.3f",tstr->tm_year + 1900, tstr->tm_mon + 1, tstr->tm_mday, tstr->tm_hour, tstr->tm_min, millisec);

	
	fits_update_key(input, TSTRING, "DATE-OBS",  datestr, "start date and time of the observation(TT)", &status);
	
	
// 	cout<<datestr<<endl;	
	tstr->tm_year = 107;
	tstr->tm_mon = 0;
	tstr->tm_mday = 1;
	tstr->tm_hour = 0;
	tstr->tm_min = 0;
	tstr->tm_sec = 0;
	tstr->tm_isdst = 0;
	
	
	tt = timegm(tstr);	
	tt += (time_t)(((mjdrefi - 54101 + mjdreff) * 86400 + timezero + t2) / tinc) ;
	tstr = gmtime(&tt);
	test = ((mjdrefi-54101+mjdreff)*86400.+t2);
	millisec = tstr->tm_sec +modf(test, &inttstart); 

	
	sprintf(datestr1,"%04d-%02d-%02dT%02d:%02d:%06.3f",tstr->tm_year + 1900, tstr->tm_mon + 1, tstr->tm_mday, tstr->tm_hour, tstr->tm_min, millisec);
	fits_update_key(input, TSTRING, "DATE-END",  datestr1, "end date and time of the observation(TT)", &status);	
	
	
// 	cout<<datestr1<<endl;

	if (i == hdnum) {
		fits_movabs_hdu(input, 1, NULL, &status);		
		fits_update_key(input, TDOUBLE, "TSTART", &t1,  "[OBT]first event time", &status);
		fits_update_key(input, TDOUBLE, "TSTOP", &t2,  "[OBT]last event time", &status);
		fits_update_key(input, TSTRING, "TIMESYS",  str, NULL, &status);
		fits_update_key(input, TSTRING, "TIMEUNIT",  secstr, NULL, &status);
		oldstatus = status;
		fits_delete_key(input, "MJDREF", &status);
		if (status == 202) status = oldstatus;
		fits_update_key(input, TDOUBLE, "MJDREFI",  &mjdrefi, NULL, &status);
		fits_update_key(input, TDOUBLE, "MJDREFF",  &mjdreff, NULL, &status);
		fits_update_key(input, TFLOAT, "TIMEZERO",  &timezero, str, &status);	
		fits_update_key(input, TSTRING, "DATE-OBS",  datestr, "start date and time of the observation(TT)", &status);
		fits_update_key(input, TSTRING, "DATE-END",  datestr1, "end date and time of the observation(TT)", &status);	
		fits_movabs_hdu(input, i, NULL, &status);		
		}
	return status;	
	}
	
int addfile(fitsfile *iFile, char * fileList, char * expr, double tmin, double tmax){
	char buffer[1024];
	int status = 0;
	FILE *fp = fopen(fileList, "r");
	if(!fp) {
		printf("Errore in apertura file '%s'\n", fileList);
		return 104;	
		}
		
	fitsfile *tempFits;	
	int hdutype = 0;
	int  find = 0;
	char *name=new char[FLEN_FILENAME];

	double t1 = 0, t2 = 0;


			
	while (fgets(buffer , 40960, fp)) {
		sscanf(buffer, "%s %lf %lf", name, &t1, &t2);
		
		if ( (tmin <= t1 &&  tmax >= t1) || (tmin <= t2 && tmin >= t1)) {

			
			if (find == 0) {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}	
				fits_copy_file(tempFits, iFile, 1, 1, 1, &status);
				fits_select_rows(iFile, iFile, expr, &status);
				fits_close_file(tempFits, &status);							
				find++;		
				}
			else {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}
				fits_movabs_hdu(tempFits, 2, &hdutype, &status);					
				fits_select_rows(tempFits, iFile, expr, &status);					
				fits_close_file(tempFits, &status);			
				find++;									
				}
			}
		}
	if (find == 0) {
		return 1005;
		}	
	fclose(fp);
	return status;
	}
	
	
	
int addfile3914(fitsfile *iFile, char * fileList, char * expr, double tmin, double tmax, char * pkt){
	char buffer[1024];
	int status = 0;
	FILE *fp = fopen(fileList, "r");
	if(!fp) {
		printf("Errore in apertura file '%s'\n", fileList);
		return 104;	
		}
		
	fitsfile *tempFits;	
	int hdutype = 0;
	int  find = 0;
	char *name=new char[FLEN_FILENAME];
	char *type=new char[FLEN_FILENAME];
	double t1,t2;

	while (fgets(buffer , 40960, fp)) {
		sscanf(buffer, "%s %lf %lf %s", name, &t1, &t2, type);

		if ( ((tmin <= t1 &&  tmax >= t1) || (tmin <= t2 && tmin >= t1)) && (strcmp(type,"3914")==0)) {
		
			if (find == 0) {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}
				fits_movabs_hdu(tempFits, 3, &hdutype, &status);	
				fits_copy_file(tempFits, iFile, 0, 1, 0, &status);
				fits_select_rows(iFile, iFile, expr, &status);
				fits_close_file(tempFits, &status);							
				find++;		
				}
			else {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}
				fits_movabs_hdu(tempFits, 3, &hdutype, &status);					
				fits_select_rows(tempFits, iFile, expr, &status);					
				fits_close_file(tempFits, &status);			
				find++;									
				}
			}
		}
	delete [] name;
	delete [] type;
	if (find == 0) {
		return 1005;
		}	
	fclose(fp);
	return status;
	}
	
int addfile3201(fitsfile *iFile, char * fileList, char * expr, double tmin, double tmax, char * pkt){
	char buffer[1024];
	int status = 0;
	FILE *fp = fopen(fileList, "r");
	if(!fp) {
		printf("Errore in apertura file '%s'\n", fileList);
		return 104;	
		}
		
	fitsfile *tempFits;	
	int hdutype = 0;
	int  find = 0;
	char *name=new char[FLEN_FILENAME];
	char *type=new char[FLEN_FILENAME];
	double t1,t2;

	while (fgets(buffer , 40960, fp)) {
		sscanf(buffer, "%s %lf %lf %s", name, &t1, &t2, type);

		if ( ((tmin <= t1 &&  tmax >= t1) || (tmin <= t2 && tmin >= t1)) && (strcmp(type,"3201")==0)) {
		
			if (find == 0) {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}
				fits_movabs_hdu(tempFits, 2, &hdutype, &status);	
				fits_copy_file(tempFits, iFile, 0, 1, 0, &status);
				fits_select_rows(iFile, iFile, expr, &status);
				fits_close_file(tempFits, &status);							
				find++;		
				}
			else {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}
				fits_movabs_hdu(tempFits, 2, &hdutype, &status);					
				fits_select_rows(tempFits, iFile, expr, &status);					
				fits_close_file(tempFits, &status);			
				find++;									
				}
			}
		}
	delete [] name;
	delete [] type;
	if (find == 0) {
		return 1005;
		}	
	fclose(fp);
	return status;
	}
	
int addfile3916(fitsfile *iFile, char * fileList, char * expr, double tmin, double tmax, char * pkt){
	char buffer[1024];
	int status = 0;
	FILE *fp = fopen(fileList, "r");
	if(!fp) {
		printf("Errore in apertura file '%s'\n", fileList);
		return 104;	
		}
		
	fitsfile *tempFits;	
	int hdutype = 0;
	int  find = 0;
	char *name=new char[FLEN_FILENAME];
	char *type=new char[FLEN_FILENAME];
	double t1,t2;

	while (fgets(buffer , 40960, fp)) {
		sscanf(buffer, "%s %lf %lf %s", name, &t1, &t2, type);

		if ( ((tmin <= t1 &&  tmax >= t1) || (tmin <= t2 && tmin >= t1)) && (strcmp(type,"3916")==0)) {
		
			if (find == 0) {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}
				fits_movabs_hdu(tempFits, 3, &hdutype, &status);	
				fits_copy_file(tempFits, iFile, 0, 1, 0, &status);
				fits_select_rows(iFile, iFile, expr, &status);
				fits_close_file(tempFits, &status);							
				find++;		
				}
			else {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}
				fits_movabs_hdu(tempFits, 3, &hdutype, &status);					
				fits_select_rows(tempFits, iFile, expr, &status);					
				fits_close_file(tempFits, &status);			
				find++;									
				}
			}
		}
	delete [] name;
	delete [] type;
	if (find == 0) {
		return 1005;
		}	
	fclose(fp);
	return status;
	}
	
	
	
int addfileFLG(fitsfile *iFile, char * fileList, char * expr, char * pkt){
	char buffer[1024];
	int status = 0;
	FILE *fp = fopen(fileList, "r");
	if(!fp) {
		printf("COULD NOT OPEN THE NAMED FILE:  %s\n", fileList);
		return 1003;
		}
		
	fitsfile *tempFits;	
	int hdutype = 0;
	int  find = 0;
	char *name=new char[FLEN_FILENAME];
	char *type=new char[FLEN_FILENAME];
	double t1,t2;
	int  pkttype = 0;
	int  pktstype = 0;	

	while (fgets(buffer , 40960, fp)) {
		sscanf(buffer, "%s %lf %lf %s", name, &t1, &t2, type);

		if ( strcmp(type,"FLG")==0) {
//			printf("..........opening input file: %s\n",name);	
			if (find == 0) {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}
				
					
				fits_read_key(tempFits, TINT, "PKTTYPE", &pkttype, NULL, &status );
				fits_read_key(tempFits, TINT, "PKTSTYPE", &pktstype, NULL, &status );
				if (pkttype!=39 || ((pktstype!=1) && (pktstype!=2))) {
					printf("error packet type [39]   %d!!\n",pkttype );
					printf("error packet subtype [1,2] %d!!\n",pktstype );
					fits_close_file(tempFits, &status);
					return 1001;
					}					
					
						
				fits_copy_file(tempFits, iFile, 1, 1, 1, &status);
				fits_select_rows(iFile, iFile, expr, &status);
				fits_close_file(tempFits, &status);							
				find++;		
				}
			else {
				if ( fits_open_file(&tempFits, name, READONLY, &status) != 0 ) {
					printf("Errore in apertura file '%s'\n", name);
					return status;
					}
				fits_read_key(tempFits, TINT, "PKTTYPE", &pkttype, NULL, &status );
				fits_read_key(tempFits, TINT, "PKTSTYPE", &pktstype, NULL, &status );
				if (pkttype!=39 || pktstype!=1) {
					printf("error packet type [39]   %d!!\n",pkttype );
					printf("error packet subtype [1] %d!!\n",pktstype );
					fits_close_file(tempFits, &status);
					return 1001;
					}					
				fits_movabs_hdu(tempFits, 2, &hdutype, &status);					
				fits_select_rows(tempFits, iFile, expr, &status);					
				fits_close_file(tempFits, &status);			
				find++;									
				}
			}
		}
	delete [] name;
	delete [] type;
	if (find == 0) {
		return 1005;
		}	
	fclose(fp);
	return status;
	}
	
	
int orderFileIndex(char * fileList){
	char buffer[4096];
	int status = 0;
	FILE *fp = fopen(fileList, "r");
	if(!fp) {
		printf("COULD NOT OPEN THE NAMED FILE:  %s\n", fileList);
		return 1003;
		}

	double t1 = 0, t2 = 0;

	int ccounter = 0;	
	while (fgets(buffer , 4096, fp)) {
		ccounter++;	
		}
	fseek(fp,0,SEEK_SET);		
	lineLineStruct A[ccounter];
	int i = 0;
	
	while (fgets(buffer , 4096, fp)) {
		A[i].name = new char[FLEN_FILENAME];
		A[i].type = new char[8];		
		sscanf(buffer, "%s %lf %lf %s", A[i].name, &t1, &t2, A[i].type);
		A[i].tempo1 = t1;
		A[i].tempo2 = t2;
		i++;	
		}
			
	int n =1;
	int p =0;
	while (p < i-1)	{
		n = p +1;
		
		while (n < i) {
			if ((A[n].tempo1 < A[p].tempo1)) {
							
				lineLineStruct temp;
				temp.name = new char[FLEN_FILENAME];
				temp.type = new char[8];					
				strcpy(temp.name,A[p].name);
				temp.tempo1 = A[p].tempo1;
				temp.tempo2 = A[p].tempo2;
							
				strcpy(temp.type,A[p].type);	
							
				strcpy(A[p].name,A[n].name);
				A[p].tempo1 = A[n].tempo1;
				A[p].tempo2 = A[n].tempo2;				
				strcpy(A[p].type,A[n].type);
				
				strcpy(A[n].name,temp.name);
				A[n].tempo1 = temp.tempo1;
				A[n].tempo2 = temp.tempo2;
				strcpy(A[n].type,temp.type);
				
				delete [] temp.name;
				delete [] temp.type;	
											
				}
			n++;
			}
		p++;		
		}

	fclose(fp);
	remove(fileList);
	
	fp = fopen(fileList, "w");
	if(!fp) {
		printf("COULD NOT OPEN THE NAMED FILE:  %s\n", fileList);
		return 1003;
		}
	n = 0;	
	while (n < i) {
		fprintf(fp, "%s %f %f %s\n", A[n].name,  A[n].tempo1 ,A[n].tempo2, A[n].type);
		delete [] A[n].name;
		delete [] A[n].type;			
		n++;	
		}	
	
	fclose(fp);		
	return status;
	}
	
	
	
int checkfile(char * fileList, char * filename, double tmin, double tmax, char * packet){
	char *buffer = new char[4096];	
	char *name=new char[FLEN_FILENAME];
	char *type=new char[10];	
	float t1 = 0, t2 = 0;
		
	FILE *fp = fopen(fileList, "r");
	if(!fp) {
		return 0;	
		}
	
	while (fgets(buffer , 4096, fp)) {
		sscanf(buffer, "%s %f %f %s", name, &t1, &t2, type);
		if ( strcmp(name,filename)==0) {
			delete [] buffer;
			delete [] name;	
			delete [] type;					
			return 1;					
			}
		}	
	fclose(fp);
	
	delete [] buffer;
	delete [] name;	
	delete [] type;		
	return 0;
	}	
	
void getError(int status) {
	char error_array[100][200];
	sprintf(error_array[0], "FILE TYPE WRONG FOR THIS TASK");
	sprintf(error_array[1], "ERROR IN PARAMETER FILE");
	sprintf(error_array[2], ".");	
	sprintf(error_array[3], "WRONG INDEX FILE..list corrected is: filename time_init time_stop type_packet");	
	sprintf(error_array[4], "FILE(S) NOT FOUND BETWEEN TMIN AND TMAX");		
	sprintf(error_array[5], "INPUT FILE SHOULD BE A LIST");	
	sprintf(error_array[6], "MAXIMUM TIME BEFORE MINIMUM TIME");
	sprintf(error_array[7], "NOT MATCHING TMIN AND TMAX");
	printf("%s.....STATUS: %d\n",error_array[status-1001], status);
	}


void fpgetError(FILE * fp, int status)  {
	char error_array[100][200];
	sprintf(error_array[0], "FILE TYPE WRONG FOR THIS TASK");
	sprintf(error_array[1], "ERROR IN PARAMETER FILE");
	sprintf(error_array[2], ".");	
	sprintf(error_array[3], "WRONG INDEX FILE..list corrected is: filename time_init time_stop type_packet");	
	sprintf(error_array[4], "FILE(S) NOT FOUND BETWEEN TMIN AND TMAX");		
	sprintf(error_array[5], "INPUT FILE SHOULD BE A LIST");	
	sprintf(error_array[6], "MAXIMUM TIME BEFORE MINIMUM TIME");
	sprintf(error_array[7], "NOT MATCHING TMIN AND TMAX");			
	fprintf(fp, "%s.....STATUS: %d\n",error_array[status-1001], status);
	}




int isfile (const char *filename)
{
  /* Returns a true value if filename exists as a file, or false if not. */

  FILE *stream;

  /* Open the file */
// cout<<"filename = "<<filename<<endl;
  stream = fopen (filename, "r");
  if (stream == NULL)
    {
//     cout<<"filename non c'è"<<endl;
      /* Failed */
      return (0);
    }
    else {
//      cout<<"filename c'è"<<endl;   
    }

  /* Close the file, and return success */

  fclose (stream);
  return (1);
}
 
	
	
char * filetemp(char * name){  

	time_t seconds;
	seconds = time(&seconds);
	srand(seconds);
 	int first = rand() % 32767;
	int second = rand() % 32767;
 	int third = rand() % 32767;		
	sprintf(name,"/tmp/filetemp%i_%i_%i",first,second,third);
	int c = 1;
	while (isfile(name) == 1){
		srand(c);
		first = rand() % 32767;
		second = rand() % 32767;
		third = rand() % 32767;		
		sprintf(name,"/tmp/filetemp%i_%i_%i",first,second,third); 
		c++;
		}
			
// 	cout<<name<<endl;
	return name;
}
	











