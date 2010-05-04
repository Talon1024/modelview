#include "stdafx.h"
#include "matrix.h"
#include "math.h"

void CMathStuff::Rotate_B_Point(int offset, FIXANG angle_xi,FIXANG angle_yi,FIXANG angle_zi, D2_MODEL *Model)
{
// segment is what we are rotating, offset is the offset we are using for it...
// float WireCube[25][3];

	long i;
	matrix_4x4 rotate;
	float temp_x, // used to hold intermediate results during rotation
		temp_y,
		temp_z;

	// test if we need to rotate at all

	if (angle_xi==0 && angle_yi==0 && angle_zi==0)
	{
		for (i=0;i<24;i++)
		{
			D2_WireCube[i][0] += Model->Sbackup[offset].x;
			D2_WireCube[i][1] += Model->Sbackup[offset].y;
			D2_WireCube[i][2] += Model->Sbackup[offset].z;
		}
		return;
	}

	Gen_Rotate_Matrix(rotate, angle_xi,angle_yi,angle_zi);

	for (i=0;i<24;i++)
	{
		temp_x=D2_WireCube[i][0] * rotate[0][0] +
				 D2_WireCube[i][1] * rotate[1][0] +
				 D2_WireCube[i][2] * rotate[2][0];

		temp_y=D2_WireCube[i][0] * rotate[0][1] +
				 D2_WireCube[i][1] * rotate[1][1] +
				 D2_WireCube[i][2] * rotate[2][1];

		temp_z=D2_WireCube[i][0] * rotate[0][2] +
				 D2_WireCube[i][1] * rotate[1][2] +
				 D2_WireCube[i][2] * rotate[2][2];

		D2_WireCube[i][0]=temp_x + Model->Sbackup[offset].x;
		D2_WireCube[i][1]=temp_y + Model->Sbackup[offset].y;
		D2_WireCube[i][2]=temp_z + Model->Sbackup[offset].z;
	}
} // end Rotate_point


void CMathStuff::Rotate_G_Point(int offset, FIXANG angle_xi,FIXANG angle_yi,FIXANG angle_zi, D2_MODEL *Model, D2_VMS_VECTOR *gunpnt)
{
// segment is what we are rotating, offset is the offset we are using for it...

//long i;
matrix_4x4 rotate;
float temp_x, // used to hold intermediate results during rotation
			temp_y,
			temp_z;

// test if we need to rotate at all

	if (angle_xi==0 && angle_yi==0 && angle_zi==0)
	{
//	 for (i=0;i<Model.Vcount;i++) {
//		 if (Model.Vsegment[i] == segment) {
//			 if (segment == offset) {
//				 Model.Vpoint[i].x -= Model.Soffset[offset].x;
//				 Model.Vpoint[i].y -= Model.Soffset[offset].y;
//				 Model.Vpoint[i].z -= Model.Soffset[offset].z;
//			 }
		gunpnt->x += Model->Sbackup[offset].x;
		gunpnt->y += Model->Sbackup[offset].y;
		gunpnt->z += Model->Sbackup[offset].z;
//		 }
//	 }
		return;
	}
	Gen_Rotate_Matrix(rotate, angle_xi,angle_yi,angle_zi);

	temp_x=gunpnt->x * rotate[0][0] +
				 gunpnt->y * rotate[1][0] +
				 gunpnt->z * rotate[2][0];

	temp_y=gunpnt->x * rotate[0][1] +
				 gunpnt->y * rotate[1][1] +
				 gunpnt->z * rotate[2][1];

	temp_z=gunpnt->x * rotate[0][2] +
				 gunpnt->y * rotate[1][2] +
				 gunpnt->z * rotate[2][2];

	gunpnt->x=(long)(temp_x + Model->Sbackup[offset].x);
	gunpnt->y=(long)(temp_y + Model->Sbackup[offset].y);
	gunpnt->z=(long)(temp_z + Model->Sbackup[offset].z);
} // end Rotate_point

void CMathStuff::Gen_Rotate_Matrix(matrix_4x4 rotate, FIXANG angle_xi,FIXANG angle_yi,FIXANG angle_zi)
{
	// this function creates a 4x4 identity matrix

	matrix_4x4 rotate_x,    // the x,y and z rotation matrices
			 rotate_y,
			 rotate_z,
//			 rotate,      // the final rotation matrix
			 temp;        // temporary working matrix

	float angle_x,angle_y,angle_z;

	angle_x=(float) angle_xi/(float)0x100;
	angle_y=(float) angle_yi/(float)0x100;
	angle_z=(float) angle_zi/(float)0x100;

	angle_x=(float) (3.14159*(float)angle_x/(float)128);  //180
	angle_y=(float) (3.14159*(float)angle_y/(float)128);
	angle_z=(float) (3.14159*(float)angle_z/(float)128);


	// create identity matrice
	Mat_Identity_4x4(rotate);
	int product=0;   // used to determine which matrices need multiplying

	// create X rotation matrix
	if (angle_x)
	{
		Mat_Identity_4x4(rotate_x);
		rotate_x[1][1]=(float)( cos(angle_x));
		rotate_x[1][2]=(float)( sin(angle_x));
		rotate_x[2][1]=(float)(-sin(angle_x));
		rotate_x[2][2]=(float)( cos(angle_x));
		product|=4; // if bit 2 of product is 1 then there is an x rotation
	}

	// create Y rotation matrix
	if (angle_y)
	{
		Mat_Identity_4x4(rotate_y);
		rotate_y[0][0]=(float)( cos(angle_y));
		rotate_y[0][2]=(float)(-sin(angle_y));
		rotate_y[2][0]=(float)( sin(angle_y));
		rotate_y[2][2]=(float)( cos(angle_y));
		product|=2; // if bit 1 of product is 1 then there is an y rotation
	}

	// create Z rotation matrix
	if(angle_z)
	{
		Mat_Identity_4x4(rotate_z);
		rotate_z[0][0]=(float)( cos(angle_z));
		rotate_z[0][1]=(float)( sin(angle_z));
		rotate_z[1][0]=(float)(-sin(angle_z));
		rotate_z[1][1]=(float)( cos(angle_z));
		product|=1; // if bit 0 of product is 1 then there is an z rotation
	}


	// compute final rotation matrix, determine the proper product of matrices
	// use a switch statement along with a bit pattern to determine which
	// matrices need multiplying, this is worth the time it would take
	// to concatenate matrices together that don't have any effect

	// compute proper final rotation matrix

	switch(product)
	{
	case 0: // do nothing there isn't any rotation
		ASSERT(FALSE);
		break;

	case 1: // final matrix=z
		Mat_Copy_4x4(rotate_z,rotate);
		break;

	case 2: // final matrix=y
		Mat_Copy_4x4(rotate_y,rotate);
		break;

	case 3: // final matrix=y*z
		Mat_Mul_4x4_4x4(rotate_y,rotate_z,rotate);
		break;

	case 4: // final matrix=x
		Mat_Copy_4x4(rotate_x,rotate);
		break;

	case 5: // final matrix=x*z
//		Mat_Mul_4x4_4x4(rotate_x,rotate_z,rotate);
		Mat_Mul_4x4_4x4(rotate_z,rotate_x,rotate);
		break;

	case 6: // final matrix=x*y
//		Mat_Mul_4x4_4x4(rotate_x,rotate_y,rotate);
		Mat_Mul_4x4_4x4(rotate_y,rotate_x,rotate);
		break;

	case 7: // final matrix=x*y*z
//		Mat_Mul_4x4_4x4(rotate_x,rotate_y,temp);
//		Mat_Mul_4x4_4x4(temp,rotate_z,rotate);
		Mat_Mul_4x4_4x4(rotate_y,rotate_z,temp);
		Mat_Mul_4x4_4x4(temp,rotate_x,rotate);
		break;

	default:
		break;

	} // end switch
} // end Gen_rotation_matrix


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

//////////////////////////////////////////////////////////////////////////////


void CMathStuff::D2_BuildWireCube(D2_VMS_VECTOR min, D2_VMS_VECTOR max)
{
	D2_WireCube[0][0]=(float)max.x; D2_WireCube[0][1]=(float)max.y; D2_WireCube[0][2]=(float)max.z;
	D2_WireCube[1][0]=(float)min.x; D2_WireCube[1][1]=(float)max.y; D2_WireCube[1][2]=(float)max.z;
	D2_WireCube[2][0]=(float)max.x; D2_WireCube[2][1]=(float)max.y; D2_WireCube[2][2]=(float)max.z;
	D2_WireCube[3][0]=(float)max.x; D2_WireCube[3][1]=(float)min.y; D2_WireCube[3][2]=(float)max.z;
	D2_WireCube[4][0]=(float)max.x; D2_WireCube[4][1]=(float)max.y; D2_WireCube[4][2]=(float)max.z;
	D2_WireCube[5][0]=(float)max.x; D2_WireCube[5][1]=(float)max.y; D2_WireCube[5][2]=(float)min.z;

	D2_WireCube[6][0]=(float)min.x; D2_WireCube[6][1]=(float)max.y; D2_WireCube[6][2]=(float)max.z;
	D2_WireCube[7][0]=(float)min.x; D2_WireCube[7][1]=(float)min.y; D2_WireCube[7][2]=(float)max.z;
	D2_WireCube[8][0]=(float)min.x; D2_WireCube[8][1]=(float)max.y; D2_WireCube[8][2]=(float)max.z;
	D2_WireCube[9][0]=(float)min.x; D2_WireCube[9][1]=(float)max.y; D2_WireCube[9][2]=(float)min.z;

	D2_WireCube[10][0]=(float)max.x; D2_WireCube[10][1]=(float)min.y; D2_WireCube[10][2]=(float)max.z;
	D2_WireCube[11][0]=(float)min.x; D2_WireCube[11][1]=(float)min.y; D2_WireCube[11][2]=(float)max.z;
	D2_WireCube[12][0]=(float)max.x; D2_WireCube[12][1]=(float)min.y; D2_WireCube[12][2]=(float)max.z;
	D2_WireCube[13][0]=(float)max.x; D2_WireCube[13][1]=(float)min.y; D2_WireCube[13][2]=(float)min.z;

	D2_WireCube[14][0]=(float)max.x; D2_WireCube[14][1]=(float)max.y; D2_WireCube[14][2]=(float)min.z;
	D2_WireCube[15][0]=(float)min.x; D2_WireCube[15][1]=(float)max.y; D2_WireCube[15][2]=(float)min.z;
	D2_WireCube[16][0]=(float)max.x; D2_WireCube[16][1]=(float)max.y; D2_WireCube[16][2]=(float)min.z;
	D2_WireCube[17][0]=(float)max.x; D2_WireCube[17][1]=(float)min.y; D2_WireCube[17][2]=(float)min.z;

	D2_WireCube[18][0]=(float)max.x; D2_WireCube[18][1]=(float)min.y; D2_WireCube[18][2]=(float)min.z;
	D2_WireCube[19][0]=(float)min.x; D2_WireCube[19][1]=(float)min.y; D2_WireCube[19][2]=(float)min.z;
	D2_WireCube[20][0]=(float)min.x; D2_WireCube[20][1]=(float)max.y; D2_WireCube[20][2]=(float)min.z;
	D2_WireCube[21][0]=(float)min.x; D2_WireCube[21][1]=(float)min.y; D2_WireCube[21][2]=(float)min.z;
	D2_WireCube[22][0]=(float)min.x; D2_WireCube[22][1]=(float)min.y; D2_WireCube[22][2]=(float)max.z;
	D2_WireCube[23][0]=(float)min.x; D2_WireCube[23][1]=(float)min.y; D2_WireCube[23][2]=(float)min.z;
}





/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void CMathStuff::Rotate_Object(int offset, int segment, FIXANG angle_xi,FIXANG angle_yi,FIXANG angle_zi,D2_MODEL *Model)
{
// segment is what we are rotating, offset is the offset we are using for it...

	long i;
	matrix_4x4 rotate;
	float temp_x, // used to hold intermediate results during rotation
		temp_y,
		temp_z;

	// test if we need to rotate at all
	if ((angle_xi==0) && (angle_yi==0) && (angle_zi==0))
	{
		for (i=0;i<Model->Vcount;i++)
		{
			if (Model->Vsegment[i]==segment)
			{
				if (segment==offset)
				{
					Model->Vpoint[i].x-=Model->Soffset[offset].x;
					Model->Vpoint[i].y-=Model->Soffset[offset].y;
					Model->Vpoint[i].z-=Model->Soffset[offset].z;
				}
				Model->Vpoint[i].x+=Model->Sbackup[offset].x;
				Model->Vpoint[i].y+=Model->Sbackup[offset].y;
				Model->Vpoint[i].z+=Model->Sbackup[offset].z;
			}
		}
		return;
	}

	Gen_Rotate_Matrix(rotate,angle_xi,angle_yi,angle_zi);

	for (i=0;i<Model->Vcount;i++)
	{
		if (Model->Vsegment[i] == segment)
		{
			if (segment == offset)
			{
				Model->Vpoint[i].x -= Model->Soffset[offset].x;
				Model->Vpoint[i].y -= Model->Soffset[offset].y;
				Model->Vpoint[i].z -= Model->Soffset[offset].z;
			}

			temp_x=Model->Vpoint[i].x * rotate[0][0] +
				 Model->Vpoint[i].y * rotate[1][0] +
				 Model->Vpoint[i].z * rotate[2][0];

			temp_y=Model->Vpoint[i].x * rotate[0][1] +
				 Model->Vpoint[i].y * rotate[1][1] +
				 Model->Vpoint[i].z * rotate[2][1];

			temp_z=Model->Vpoint[i].x * rotate[0][2] +
				 Model->Vpoint[i].y * rotate[1][2] +
				 Model->Vpoint[i].z * rotate[2][2];

			Model->Vpoint[i].x=(long)(temp_x+Model->Sbackup[offset].x);
			Model->Vpoint[i].y=(long)(temp_y+Model->Sbackup[offset].y);
			Model->Vpoint[i].z=(long)(temp_z+Model->Sbackup[offset].z);
		}

	} // end for index

} // end Rotate_Object
