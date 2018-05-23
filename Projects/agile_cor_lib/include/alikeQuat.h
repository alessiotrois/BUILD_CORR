#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#ifndef QUAT_INCLUDED



/**********************************************************
* roudoff error tolerance used in quaternion calculations *
**********************************************************/
#define QUAT_ROUNDOFF_ERR 1e-14
#define EULER_ROUNDOFF_ERR 1e-15

/*************************************************************
* Quat structure
**************************************************************/
typedef struct {

double p[4];

} QUAT;

typedef struct {

double m[3][3];

} ROTMATRIX;

/***************************************************************
****************************************************************
* allocate space for a new quaternion structure
****************************************************************/
QUAT* allocateQuat();

/***************************************************************
****************************************************************
* allocate space for an array of quaternion structures
* note allocateQuatArray(1) is the same as allocateQuat
****************************************************************/
QUAT* allocateQuatArray(int dimen);

/*********************************************************************
*********************************************************************
* Change the size of a previously allocated quaternion array
********************************************************************/
QUAT* changeQuatArraySize(QUAT* q, int newdimen);


/***************************************************************
****************************************************************
* free storage for a quaternion structure
****************************************************************/
void destroyQuat(QUAT* q);

/**************************************************************************
***************************************************************************
* Set the four components of a quaternion and makes sure they are normalized
***************************************************************************/
void setQuat(QUAT* q, double q0, double q1, double q2, double q3);

/**************************************************************************
***************************************************************************
* Set a quaternity to the identity quaternion (0,0,0,1)
***************************************************************************/
void setQuatToIdentity(QUAT* q);

void renormalizeQuat(QUAT* q);

/**************************************************************************
***************************************************************************
* Report that there is an inconsistency in a quaternion
* This would usually be due to bad normalization
* The program prints a message to stderr and exits with status "1"
***************************************************************************/
void badQuatError(QUAT* q);

/***************************************************************
****************************************************************
* give the magnitude of a quaternion
* attitude quaternions should be normalized to 1.
****************************************************************/
double normOfQuat(QUAT* q);


/***************************************************************
****************************************************************
* Computes the quaternion product q = q1 q2
* This gives the result of sucessive rotations by q1 and then q2
* Note that the quaternion product does not commute.
****************************************************************/
void productOfQuats(QUAT* q, QUAT* q1, QUAT* q2);

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
void getQuatOfChange(QUAT* q, QUAT* q1, QUAT* q2);

/***************************************************************
****************************************************************
* calculate the compliment of a quaternion. The compliment of a 
* quaternion represents a rotation in the opposite direction.
****************************************************************/
void invertQuat(QUAT* q, QUAT* q1);

/***************************************************************
****************************************************************
* calculate the compliment of a quaternion. The compliment of a 
* quaternion represents a rotation in the opposite direction.
****************************************************************/
void invertQuatInPlace(QUAT* q);

/***************************************************************
****************************************************************
* Calculates the angle in radians of the rotation represented by the
* quaternion q divided by 2.
* This routine assumes the quaternion is properly normalized
* also note that by convention, the rotation angle is always
* positive.
****************************************************************/
double getQuatHalfRotation(QUAT* q);

/***************************************************************
****************************************************************
* q represents a rotation about the same axis as q1, but through 
* a rotation angle of a times the rotation angle of q1
* This is useful for interpolation and extrapolation.
* Note this routine assumes the quaternions are normalized 
* to within roundoff error. 
****************************************************************/
void multiplyQuatByScalar(QUAT* q, QUAT* q1,double a);


/***************************************************************************
****************************************************************************
* get the quaternion which will give the shortest "great circle" rotation
* which will transform unit vector hat1 into unit vector hat2
***************************************************************************/
void greatCircleQuat(QUAT* q, double hat1[3], double hat2[3]);

void convertQuatToRotMatrix(ROTMATRIX* rot,QUAT* q);

void convertRotMatrixToQuat(QUAT* q, ROTMATRIX* rot);


/***************************************************************************
****************************************************************************
* convert a quaternion to a set of euler angles
* This routine achieves the same result as first converting to a rotation
* matrix and then converting to euler angles, but is more efficient
* since not all the rotation matrix elements are needed.
* On the other hand if you already have the rotation matrix available
* it is more efficient to convert that into the euler angles.
***************************************************************************/
void convertQuatToEuler(double *theta, double *phi, double *psi, QUAT* q);

/***************************************************************************
****************************************************************************
* convert  a set of Euler angles to a quaternion
* This routine does exactly the same calculation ar first converting
* to a rotation matrix and then to a quaternion, but it saves having
* to handle the intermediate storage.
* If you already have the rotation matrix, then it is much more
* efficient to convert that to a quaternion.
*****************************************************************************/
void convertEulerToQuat(QUAT* q, double *theta, double *phi, double *psi);

#define QUAT_INCLUDED
#endif /* QUAT_INCLUDED */


