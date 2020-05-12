#pragma once
#include "vec3.h"


class Mat4
{

public :


	float m[4][4];



public:
	
	// Copy Constructor
	Mat4(const Mat4& mat);
	Mat4(){
		for (int i = 0;i < 4;i++)
			for (int j = 0; j < 4; j++)
				m[i][j] = 0.0f;
	}
	// Set usefull Matrixes
	void setIdentity();
	void setRotX(const float theta);
	void setRotY(const float theta);
	void setRotZ(const float theta);
	void setTransMat(vec3 P);
	void setTransPart(vec3 P);
	void setProjMat(float fov, float ar, float zNear, float zFar);
	void reset();
	
	// Operations

	void MatMatMult(const Mat4& A, const Mat4& B);
	void vecMatMult(const vec3& i, vec3& o);
	void Inverse(const Mat4& i);
	
	void printMat();
};

