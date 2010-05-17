#include "stdafx.h"

#ifndef MATRIXH
#define MATRIXH

//#include "d2-extra.h"

class CMathStuff
{
public:
	float D2_WireCube[25][3];

	typedef float matrix_4x4[4][4];
	void Gen_Rotate_Matrix(matrix_4x4 rotate, short angle_xi,short angle_yi,short angle_zi);
	void Mat_Copy_4x4(matrix_4x4 source, matrix_4x4 destination);
	void Mat_Identity_4x4(matrix_4x4 a);
	void Mat_Mul_4x4_4x4(matrix_4x4 a, matrix_4x4 b, matrix_4x4 result);
};

#endif
