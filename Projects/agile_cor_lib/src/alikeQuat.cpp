#include "alikeQuat.h"



static int WARNING_COUNT = 0;

void set_quat_warnings (int count)
{
	WARNING_COUNT = count;
}

static void maintain_quat (QUAT * q)
{
	double * p = q->p;
	double norm = sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3]);
	if (p[3] < 0 || fabs(norm - 1) > QUAT_ROUNDOFF_ERR / 10) {
		double invnorm = 1. / norm;
		double err = fabs(norm - 1);
		if (WARNING_COUNT > 0) {
			--WARNING_COUNT;
			printf("renormalizing QUAT %p with |1-norm|=%g\n", (void*) q, err);
		}
		if (p[3] < 0)
			invnorm = -invnorm;
		p[0] *= invnorm;
		p[1] *= invnorm;
		p[2] *= invnorm;
		p[3] *= invnorm;
	}
}


/***************************************************************
****************************************************************
* allocate space for a new quaternion structure
****************************************************************/
QUAT* allocateQuat(void)
{
QUAT* q;

q=(QUAT*)malloc(sizeof(QUAT));

return(q);

}

/***************************************************************
****************************************************************
* allocate space for an array of quaternion structures
* note allocateQuatArray(1) is the same as allocateQuat
****************************************************************/
QUAT* allocateQuatArray(int dimen) {
QUAT* q;

q=(QUAT*)malloc(sizeof(QUAT)*dimen);

return(q);

}

/*********************************************************************
*********************************************************************
* Change the size of a previously allocated 
********************************************************************/
QUAT* changeQuatArraySize(QUAT* q, int newdimen) {

q=(QUAT*)realloc((void*)q,sizeof(QUAT)*newdimen);

return(q);

}



/***************************************************************
****************************************************************
* free storage for a quaternion structure
****************************************************************/
void destroyQuat(QUAT* q)
{
free(q);
}

/**************************************************************************
***************************************************************************
* Set the four components of a quaternion and makes sure they are normalized
***************************************************************************/
void setQuat(QUAT* q, double q0, double q1, double q2, double q3) 
{
q->p[0]=q0;
q->p[1]=q1;
q->p[2]=q2;
q->p[3]=q3;

maintain_quat(q);
}



void renormalizeQuat (QUAT * q)
{
maintain_quat(q);
}



/**************************************************************************
***************************************************************************
* Set a quaternity to the identity quaternion (0,0,0,1)
***************************************************************************/
void setQuatToIdentity(QUAT* q) {

setQuat(q,0.,0.,0.,1.);

} /* end of setQuatToIdentity functiuon */



/**************************************************************************
***************************************************************************
* Report that there is an inconsistency in a quaternion
* This would usually be due to bad normalization
* The program prints a message to stderr and exits with status "1"
***************************************************************************/
void badQuatError(QUAT* q) 
{
fprintf(stderr,"Ill formed quaternion:\n");
fprintf(stderr,"(%g %g %g %g)\n",q->p[0],q->p[1],q->p[2],q->p[3]);
fprintf(stderr,"norm=%.14g\n",normOfQuat(q) );

exit(1);

}

/***************************************************************
****************************************************************
* give the magnitude of a quaternion
* attitude quaternions should be normalized to 1.
****************************************************************/
double normOfQuat(QUAT* q)
{
	double * p = q->p;
return(sqrt(p[0] * p[0] + 
            p[1] * p[1] +
            p[2] * p[2] + 
            p[3] * p[3]   ) );

}

/***************************************************************
****************************************************************
* Computes the quaternion product q = q1 q2
* This gives the result of sucessive rotations by q1 and then q2
* Note that the quaternion product does not commute.
****************************************************************/
void productOfQuats(QUAT* q, QUAT* q1, QUAT* q2)
{
double* p1;
double* p2;

p1=q1->p;
p2=q2->p;

q->p[0] =  p2[3]*p1[0] + p2[2]*p1[1] - p2[1]*p1[2] + p2[0]*p1[3];
q->p[1] = -p2[2]*p1[0] + p2[3]*p1[1] + p2[0]*p1[2] + p2[1]*p1[3];
q->p[2] =  p2[1]*p1[0] - p2[0]*p1[1] + p2[3]*p1[2] + p2[2]*p1[3];
q->p[3] = -p2[0]*p1[0] - p2[1]*p1[1] - p2[2]*p1[2] + p2[3]*p1[3];
//	temp[3] = Q_1[3]*Q_2[3] -  Q_1[0]*Q_2[0] - Q_1[1]*Q_2[1] - Q_1[2]*Q_2[2];
//	temp[0] = Q_1[0]*Q_2[3] +  Q_1[3]*Q_2[0] + Q_1[1]*Q_2[2] - Q_1[2]*Q_2[1];
//	temp[1] = Q_1[3]*Q_2[1] -  Q_1[0]*Q_2[2] + Q_1[1]*Q_2[3] + Q_1[2]*Q_2[0];
//	temp[2] = Q_1[3]*Q_2[2] +  Q_1[0]*Q_2[1] - Q_1[1]*Q_2[0] + Q_1[2]*Q_2[3];


//maintain_quat(q);
}

/***************************************************************
****************************************************************
* determine the quaternion q such that q1*q=q2
* In terms of rotations, this gives the quaternion of the rotation
* between the orientations specified by q1 and q2.
* Intuitively it's the "difference" - though mathematically it's
* more like division: q=inv(q1)*q2.
* Note this can also be done using invertQuat and productofQuats,
* but this function is more efficient since it is coded directly 
* with the quaternion components.
****************************************************************/
void getQuatOfChange(QUAT* q, QUAT* q1, QUAT* q2)
{
double* p1;
double* p2;

p1=q1->p;
p2=q2->p;

q->p[0] = -p2[3]*p1[0] - p2[2]*p1[1] + p2[1]*p1[2] + p2[0]*p1[3];
q->p[1] =  p2[2]*p1[0] - p2[3]*p1[1] - p2[0]*p1[2] + p2[1]*p1[3];
q->p[2] = -p2[1]*p1[0] + p2[0]*p1[1] - p2[3]*p1[2] + p2[2]*p1[3];
q->p[3] =  p2[0]*p1[0] + p2[1]*p1[1] + p2[2]*p1[2] + p2[3]*p1[3];


maintain_quat(q);

#ifdef DEBUG
printf("getQuatOfChange: q1=(%g %g %g %g)\n",p1[0],p1[1],p1[2],p1[3]);
printf("getQuatOfChange: q2=(%g %g %g %g)\n",p2[0],p2[1],p2[2],p2[3]);
printf("getQuatOfChange: q =(%g %g %g %g)\n",q->p[0],q->p[1],q->p[2],q->p[3]);

printf("getQuatOfChange: norm(q1)-1 = %g norm(q2)-1 = %g\n",
       normOfQuat(q1)-1.0, normOfQuat(q2)-1.0 );
#endif

}


/***************************************************************
****************************************************************
* calculate the compliment of a quaternion. The compliment of a
* quaternion represents a rotation in the opposite direction.
****************************************************************/
void invertQuat(QUAT* q, QUAT* q1)
{
q->p[0]=-q1->p[0];
q->p[1]=-q1->p[1];
q->p[2]=-q1->p[2];
q->p[3]= q1->p[3];

}

/***************************************************************
****************************************************************
* calculate the compliment of a quaternion. The compliment of a 
* quaternion represents a rotation in the opposite direction.
****************************************************************/
void invertQuatInPlace(QUAT* q)
{
q->p[0]=-q->p[0];
q->p[1]=-q->p[1];
q->p[2]=-q->p[2];

}





/***************************************************************
****************************************************************
* Calculates the angle in radians of the rotation represented by the
* quaternion q divided by 2.
* This routine assumes the quaternion is properly normalized
* also note that by convention, the rotation angle is always
* positive.
****************************************************************/
double getQuatHalfRotation(QUAT* q)
{

double p3;

p3 = q->p[3];

if (fabs(p3) <= 1.) return( acos(p3) );

/************************************************************
* only get here due to round-off error or an unnormalized
* or ill-formed quaternion
*************************************************************/
if (fabs(p3) - 1 > QUAT_ROUNDOFF_ERR ) badQuatError(q);

/************************************************************
* if we get here, it's just roundoff error so we compensate *
************************************************************/
if (p3 > 1) return(0.);
else        return(M_PI);

}

/***************************************************************
****************************************************************
* q represents a rotation about the same axis as q1, but through 
* a rotation angle of a times the rotation angle of q1
* This is useful for interpolation and extrapolation.
* Note this routine assumes the quaternions are normalized 
* to within roundoff error. 
****************************************************************/
void multiplyQuatByScalar(QUAT* q, QUAT* q1,double a)
{
double * p = q->p;
double * p1 = q1->p;
double pi[4];
double err;

if (p1[3] < 0) {
	/* this keeps the function sane for angles near -pi */
	pi[0] = -p1[0];
	pi[1] = -p1[1];
	pi[2] = -p1[2];
	pi[3] = -p1[3];
	p1 = pi;
}

/****************************************
* dummy variable to avoid trouble with  
* the acos argument being out of bounds
****************************************/
err = p1[3] - 1;

/**************
* normal case *
**************/
if (err < 0) {

	double ratio;
    double halfangle = acos(p1[3]);

    ratio = sin(a * halfangle) / sin(halfangle);

    p[0] = p1[0] * ratio; 
    p[1] = p1[1] * ratio;
    p[2] = p1[2] * ratio; 
    p[3] = cos(a * halfangle);

} else if (err < QUAT_ROUNDOFF_ERR) {
    /*****************************************************************
    * assuming the quaternion is normalized, this must mean q1 has
    * halfangle=0 so just return the value of q1
    *****************************************************************/
    *q=*q1;

} else {
    /***************************************
    * something wrong with the quaternion *
    ***************************************/
    badQuatError(q);
}

maintain_quat(q);

} /* end of multiplyQuatByScalar function */

/***************************************************************************
****************************************************************************
* get the quaternion which will give the shortest "great circle" rotation
* which will transform unit vector hat1 into unit vector hat2
***************************************************************************/
void greatCircleQuat(QUAT* q, double hat1[3], double hat2[3]) {
double qq;

/*******************************************
* the vector part of the quaternion 
* (components 0, 1, 2) is just hat1 x hat2
*******************************************/
q->p[0]=  hat1[1]*hat2[2] - hat1[2]*hat2[1];
q->p[1]=-(hat1[0]*hat2[2] - hat1[2]*hat2[0]);
q->p[2]=  hat1[0]*hat2[1] - hat1[1]*hat2[0];

/********************************************
* then we set the scaler part of the quat to 
* make the whole thing normalized
*********************************************/
qq=1.- q->p[0]*q->p[0] - q->p[1]*q->p[1] - q->p[2]*q->p[2];
/*
printf("q0=%g q1=%g q2=%g qq=%g\n",q->p[0],q->p[1],q->p[2],qq);
*/

if     (qq>=0.               ) q->p[3]=sqrt(qq);
else if(qq>-QUAT_ROUNDOFF_ERR) q->p[3]=0.;
else {
    fprintf(stderr,"unit vectors not normalized in greatCircleQuat\n");

    fprintf(stderr,"|hat1| = |%g %g %g| = %g\n",
            hat1[0],hat1[1],hat1[2],
            sqrt(hat1[0]*hat1[0] + hat1[1]*hat1[1] + hat1[2]*hat1[2]) );

    fprintf(stderr,"|hat2| = |%g %g %g| = %g\n",
            hat2[0],hat2[1],hat2[2],
            sqrt(hat2[0]*hat2[0] + hat2[1]*hat2[1] + hat2[2]*hat2[2]) );

    exit(1);
}

maintain_quat(q);

} /* end of greatCircleQuat function */


/***************************************************************************
****************************************************************************
* convert a quaternion to a direction cosine rotation matrix
***************************************************************************/
void convertQuatToRotMatrix(ROTMATRIX* rot,QUAT* q)
{

double* p;
p=q->p;

/********************
* diagonal elements *
********************/
rot->m[0][0] =  p[0]*p[0] - p[1]*p[1] - p[2]*p[2] + p[3]*p[3];
rot->m[1][1] = -p[0]*p[0] + p[1]*p[1] - p[2]*p[2] + p[3]*p[3];
rot->m[2][2] = -p[0]*p[0] - p[1]*p[1] + p[2]*p[2] + p[3]*p[3];

/***************
* off diagonal *
***************/
rot->m[0][1] = 2.*(p[0]*p[1] + p[2]*p[3]);
rot->m[1][0] = 2.*(p[0]*p[1] - p[2]*p[3]);

rot->m[0][2] = 2.*(p[0]*p[2] - p[1]*p[3]);
rot->m[2][0] = 2.*(p[0]*p[2] + p[1]*p[3]);

rot->m[1][2] = 2.*(p[1]*p[2] + p[0]*p[3]);
rot->m[2][1] = 2.*(p[1]*p[2] - p[0]*p[3]);

}

/***************************************************************************
****************************************************************************
* convert  a direction cosine rotation matrix to a quaternion
***************************************************************************/
void convertRotMatrixToQuat(QUAT* q, ROTMATRIX* rot)
{

double diag_sum[4];
double recip;
int i,maxi;

/****************************************************************
* there are four equivalent ways of calculating this.
* we pick the one with the greatest numerical accuracy
* for this matrix
*****************************************************************/
diag_sum[0]=1+rot->m[0][0]-rot->m[1][1]-rot->m[2][2];
diag_sum[1]=1-rot->m[0][0]+rot->m[1][1]-rot->m[2][2];
diag_sum[2]=1-rot->m[0][0]-rot->m[1][1]+rot->m[2][2];
diag_sum[3]=1+rot->m[0][0]+rot->m[1][1]+rot->m[2][2];

#ifdef DEBUG
printf("convertRotMatrixToQuat: %d diag_sum=%g\n",0,diag_sum[0]);
#endif /* DEBUG */

maxi=0;
for(i=1;i<4;++i) {
    #ifdef DEBUG
    printf("convertRotMatrixToQuat: %d diag_sum=%g\n",i,diag_sum[i]);
    #endif /* DEBUG */
    if(diag_sum[i]>diag_sum[maxi]) maxi=i;
}

#ifdef DEBUG
printf("convertRotMatrixToQuat: maxi=%d\n",maxi);
#endif /* DEBUG */

q->p[maxi]=0.5*sqrt(diag_sum[maxi]);
recip=1./(4.*q->p[maxi]);

if(maxi==0) {
    q->p[1]=recip*(rot->m[0][1]+rot->m[1][0]);
    q->p[2]=recip*(rot->m[2][0]+rot->m[0][2]);
    q->p[3]=recip*(rot->m[1][2]-rot->m[2][1]);

} else if(maxi==1) {
    q->p[0]=recip*(rot->m[0][1]+rot->m[1][0]);
    q->p[2]=recip*(rot->m[1][2]+rot->m[2][1]);
    q->p[3]=recip*(rot->m[2][0]-rot->m[0][2]);

} else if(maxi==2) {
    q->p[0]=recip*(rot->m[2][0]+rot->m[0][2]);
    q->p[1]=recip*(rot->m[1][2]+rot->m[2][1]);
    q->p[3]=recip*(rot->m[0][1]-rot->m[1][0]);

} else if(maxi==3) {
    q->p[0]=recip*(rot->m[1][2]-rot->m[2][1]);
    q->p[1]=recip*(rot->m[2][0]-rot->m[0][2]);
    q->p[2]=recip*(rot->m[0][1]-rot->m[1][0]);
}

maintain_quat(q);

} /* end of convertQuatToRotMatrix function */

/***************************************************************************
****************************************************************************
* convert a quaternion to a set of euler angles
* This routine achieves the same result as first converting to a rotation
* matrix and then converting to euler angles, but is more efficient
* since not all the rotation matrix elements are needed.
* On the other hand if you already have the rotation matrix available
* it is more efficient to convert that into the euler angles.
***************************************************************************/
void convertQuatToEuler(double *theta, double *phi, double *psi, QUAT* q)
{

double m22;

double* p;
p=q->p;

/*****************
* matrix element *
*****************/
m22=-p[0]*p[0] - p[1]*p[1] + p[2]*p[2] + p[3]*p[3];

if(fabs(m22)<1.-QUAT_ROUNDOFF_ERR ) {
    /**************
    * normal case *
    **************/
    double m20,m21;
    double m02,m12;

    *theta=acos(m22);

    /************************************************************
    * more matrix elements - though we've dropped a factor of 2 *
    ************************************************************/
    m20=p[0]*p[2] + p[1]*p[3];
    m02=p[0]*p[2] - p[1]*p[3];

    m21=p[1]*p[2] - p[0]*p[3];
    m12=p[1]*p[2] + p[0]*p[3];

    if((m20==0. && m21==0.) || 
       (m02==0. && m12==0.)   ) badQuatError(q);

    *phi=atan2(m21, m20);
    *psi=atan2(m12,-m02);

} else if(fabs(m22) > 1.+EULER_ROUNDOFF_ERR) {
    /*************************
    * rotation matrix is bad *
    *************************/ 
    badQuatError(q);

} else {
    /**********************************
    * just a roundoff problem at pole *
    **********************************/
    double m00,m01;

    if(m22>0.) *theta=0.;
    else       *theta=M_PI;

    m00=p[0]*p[0] - p[1]*p[1] - p[2]*p[2] + p[3]*p[3];
    m01=2.*(p[0]*p[1] + p[2]*p[3]);
 
    if(m01==0. && m00==0.) badQuatError(q);

    if(m22>0.) *phi=atan2( m01, m00);
    else       *phi=atan2(-m01,-m00);

    *psi = 0.;
   
}


} /* end of convertQuatToEuler function */

/***************************************************************************
****************************************************************************
* convert  a set of Euler angles to a quaternion
* This routine does exactly the same calculation as first converting
* to a rotation matrix and then to a quaternion, but it saves having
* to handle the intermediate storage.
* If you already have the rotation matrix, then it is more
* efficient to convert that to a quaternion.
*****************************************************************************/
void convertEulerToQuat(QUAT* q, double *theta, double *phi, double *psi)
{

double sinphi, sintheta, sinpsi;
double cosphi, costheta, cospsi;

double m00,m01,m02;
double m10,m11,m12;
double m20,m21,m22;

double diag_sum[4];

int i,maxi;
double recip;

/***************************
* calculate trig functions *
***************************/
sinphi=sin(*phi);
cosphi=cos(*phi);

sintheta=sin(*theta);
costheta=cos(*theta);

sinpsi=sin(*psi);
cospsi=cos(*psi);

/****************************
* calculate matrix elements *
****************************/
m00= cospsi*costheta*cosphi - sinpsi*sinphi;
m01= cospsi*costheta*sinphi + sinpsi*cosphi;

m10=-sinpsi*costheta*cosphi - cospsi*sinphi;
m11=-sinpsi*costheta*sinphi + cospsi*cosphi;

m02=-cospsi*sintheta;      
m12= sinpsi*sintheta;

m20=sintheta*cosphi;
m21=sintheta*sinphi;

m22=costheta;

/***********************************************************
* now do the conversion from rotation matrix to quaternion *
***********************************************************/
diag_sum[0]=1+m00-m11-m22;
diag_sum[1]=1-m00+m11-m22;
diag_sum[2]=1-m00-m11+m22;
diag_sum[3]=1+m00+m11+m22;

maxi=0;
for(i=1;i<4;++i) {
    if(diag_sum[i]>diag_sum[maxi]) maxi=i;
}

q->p[maxi]=0.5*sqrt(diag_sum[maxi]);
recip=1./(4.*q->p[maxi]);

if(maxi==0) {
    q->p[1]=recip*(m01+m10);
    q->p[2]=recip*(m20+m02);
    q->p[3]=recip*(m12-m21);

} else if(maxi==1) {
    q->p[0]=recip*(m01+m10);
    q->p[2]=recip*(m12+m21);
    q->p[3]=recip*(m20-m02);

} else if(maxi==2) {
    q->p[0]=recip*(m20+m02);
    q->p[1]=recip*(m12+m21);
    q->p[3]=recip*(m01-m10);

} else if(maxi==3) {
    q->p[0]=recip*(m12-m21);
    q->p[1]=recip*(m20-m02);
    q->p[2]=recip*(m01-m10);
}


maintain_quat(q);

} /* end of convertEulerToQuat function */

