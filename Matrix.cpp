#include "stdafx.h"
#include "matrix.h"
#include "math.h"

//Create a 4x4 identity matrix
void CMathStuff::Mat_Identity_4x4(matrix_4x4 a)
{
	for(int i1=0;i1<4;i1++)
		for(int i2=0;i2<4;i2++)
		{
			if(i1==i2)
				a[i1][i2]=1;	// set main diagonal to 1's
			else
				a[i1][i2]=0;
		}
}

       
//Copy a 4x4 matrix to another
void CMathStuff::Mat_Copy_4x4(matrix_4x4 source, matrix_4x4 destination)
{
	for(int i1=0; i1<4; i1++)
	{
	   for(int i2=0; i2<4; i2++)
		    destination[i1][i2]=source[i1][i2];
	}
}

//Mult two 4x4
void CMathStuff::Mat_Mul_4x4_4x4(matrix_4x4 a,matrix_4x4 b,matrix_4x4 result)
{
// this function multiplies a 4x4 by a 4x4 and stores the result in a 4x4

	for (int i1=0; i1<4; i1++)
	{
		for (int i2=0; i2<4; i2++)
        {
	        float sum=0;
	        for (int i3=0; i3<4; i3++)
				sum+=a[i1][i3]*b[i3][i2];
	        result[i1][i2]=sum;
		}
	}

} // end Mat_Mul_4x4_4x4
