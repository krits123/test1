#pragma once
#include <iostream>
#include "Mat4.h"
class vecMat3
{
public:
	int m, n;
	float *arr;

public:

	// Constructos 
	vecMat3(int mm,int nn);
	vecMat3();
	vecMat3(Mat4 T);
	vecMat3(vec3 v);
	vecMat3(float a);
	vecMat3(const vecMat3& A);





	// Operators
	vecMat3& operator =(const vecMat3& A);
	vecMat3 operator + (const vecMat3& rhs);
	vecMat3 operator - (const vecMat3& rhs);
	vecMat3 operator * (const vecMat3& rhs);
	


	bool isScalar();
	float getScalar();
	vecMat3 getTranspose();


	void print() {

		std::cout << std::endl;
		for (int i = 0;i < m;i++)
		{
			std::cout << std::endl;
			for (int j = 0;j < n;j++)
				std::cout << "    " << arr[i * n + j];
		}
		std::cout << std::endl;
	}

	~vecMat3();




};

