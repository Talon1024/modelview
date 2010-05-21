#include "stdafx.h"

#ifndef MATRIXH
#define MATRIXH

#include "d2-extra.h"

class CMathStuff
{
public:
	float D2_WireCube[25][3];

	typedef float matrix_4x4[4][4];
	void Gen_Rotate_Matrix(matrix_4x4 rotate, FIXANG angle_xi,FIXANG angle_yi,FIXANG angle_zi);
	void Rotate_B_Point(int offset, FIXANG angle_xi,FIXANG angle_yi,FIXANG angle_zi, D2_MODEL *Model);
	void Rotate_G_Point(int offset, FIXANG angle_xi,FIXANG angle_yi,FIXANG angle_zi, D2_MODEL *Model, D2_VMS_VECTOR *gunpnt);
	void Mat_Copy_4x4(matrix_4x4 source, matrix_4x4 destination);
	void Mat_Identity_4x4(matrix_4x4 a);
	void Mat_Mul_4x4_4x4(matrix_4x4 a, matrix_4x4 b, matrix_4x4 result);
	void D2_BuildWireCube(D2_VMS_VECTOR min, D2_VMS_VECTOR max);
	void Rotate_Object(int offset, int segment, FIXANG angle_xi,FIXANG angle_yi,FIXANG angle_zi,D2_MODEL *Model);
};

#endif
