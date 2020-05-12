#include "Mat4.h"
#include <math.h>
#include <iostream>



Mat4::Mat4(const Mat4& mat)
{
	for (int i = 0;i < 4;i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = mat.m[i][j];

}


void Mat4::reset() {

	for (int i = 0;i < 4;i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = 0;


}
void Mat4::setIdentity()
{
	reset();
	for (int i = 0;i < 4;i++)
		m[i][i] = 1.0f;

}

void Mat4::setRotX(const float theta)
{
	reset();
	m[0][0] = 1.0f;
	m[1][1] = cos(theta);
	m[1][2] = -sin(theta);
	m[2][1] = sin(theta);
	m[2][2] = cos(theta);
	m[3][3] = 1.0f;

}

void Mat4::setRotY(const float theta)
{
	reset();
	m[1][1] = 1.0f;
	m[0][0] = cos( theta);
	m[2][0] = -sin( theta);
	m[0][2] = sin( theta);
	m[2][2] = cos( theta);
	m[3][3] = 1.0f;

}

void Mat4::setRotZ(const float theta)
{
	reset();
	m[2][2] = 1.0f;
	m[0][0] = cos(theta);
	m[0][1] = -sin( theta);
	m[1][0] = sin(theta);
	m[1][1] = cos( theta);
	m[3][3] = 1.0f;

}

void Mat4::setTransMat(vec3 P)
{

	setIdentity();

	m[0][3] = P.x;
	m[1][3] = P.y;
	m[2][3] = P.z;
	
}

void Mat4::setTransPart(vec3 P)
{


	m[0][3] = P.x;
	m[1][3] = P.y;
	m[2][3] = P.z;


}



void Mat4::setProjMat(float fov, float ar, float zNear, float zFar)
{
	float fov_coef = 1.0f / (float)tan(fov * 0.5 * 3.14159f / 180.0f);

	m[0][0] = ar * fov_coef;
	m[1][1] = fov_coef;
	m[2][2] = zFar / (zFar - zNear);
	m[2][3] = -zNear * zFar / (zFar - zNear);
	m[3][2] = 1.0f;


}

void Mat4::MatMatMult(const Mat4& A, const Mat4& B)
{
	for (int i = 0;i < 4;i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = A.m[i][0] * B.m[0][j] + A.m[i][1] * B.m[1][j] + A.m[i][2] * B.m[2][j] + A.m[i][3] * B.m[3][j];

}

void Mat4::vecMatMult(const vec3& i, vec3& o)
{

	o.x = m[0][0] * i.x + m[0][1] * i.y + m[0][2] * i.z + m[0][3] * i.w;
	o.y = m[1][0] * i.x + m[1][1] * i.y + m[1][2] * i.z + m[1][3] * i.w;
	o.z = m[2][0] * i.x + m[2][1] * i.y + m[2][2] * i.z + m[2][3] * i.w;
	o.w = m[3][0] * i.x + m[3][1] * i.y + m[3][2] * i.z + m[3][3] * i.w;

}

void Mat4::Inverse(const Mat4& i)
{
	// Inverse rotation
	m[0][0] = i.m[0][0]; m[0][1] = i.m[1][0]; m[0][2] = i.m[2][0];
	m[1][0] = i.m[0][1]; m[1][1] = i.m[1][1]; m[1][2] = i.m[2][1];
	m[2][0] = i.m[0][2]; m[2][1] = i.m[1][2]; m[2][2] = i.m[2][2];

	// Make sure last row is in the right form
	m[3][0] = 0;m[3][1] = 0;m[3][2] = 0;m[3][3] = 1;

	// Inverse Vector part
	m[0][3] = -(m[0][0] * i.m[0][3] + m[0][1] * i.m[1][3] +  m[0][2] * i.m[2][3]);
	m[1][3] = -(m[1][0] * i.m[0][3] + m[1][1] * i.m[1][3] +  m[1][2] * i.m[2][3]);
	m[2][3] = -(m[2][0] * i.m[0][3] + m[2][1] * i.m[1][3] +  m[2][2] * i.m[2][3]);
}

void Mat4::printMat()
{
	std::cout << "Array :";
	for (int i = 0;i < 4;i++) {
		std::cout << std::endl;
		for (int j = 0; j < 4; j++)
			std::cout << m[i][j] << "\t";
	}

	std::cout << std::endl;
}



