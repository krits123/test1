#include "vecMat3.h"



vecMat3::vecMat3()
{
	m = 1;
	n = 1;

	arr = new float[1];

}

vecMat3::vecMat3(Mat4 T)
{


	m = 3;
	n = 3;
	arr = new float[m * n];
	for (int i = 0;i < m;i++)
		for (int j = 0;j < n;j++)
			arr[i * n + j] = T.m[i][j];

}

vecMat3::vecMat3(vec3 v)
{

	m = 3;
	n = 1;
	arr = new float[3];
	arr[0] = v.x;
	arr[1] = v.y;
	arr[2] = v.z;

}

vecMat3::vecMat3(float a)
{

	m = 1;
	n = 1;
	arr = new float[1];
	arr[0] = a;

}

vecMat3::vecMat3(const vecMat3& A)
{
	m = A.m;
	n = A.n;
	arr = new float[m * n];
	for (int i = 0;i < m;i++)
		for (int j = 0;j < n;j++)
			arr[i * n + j] = A.arr[i*n + j];
}

vecMat3& vecMat3::operator=(const vecMat3& A)
{
	// TODO: insert return statement here
	
	
	delete[] arr;

	m = A.m;
	n = A.n;

	arr = new float[m * n];

	for (int i = 0;i < m;i++)
		for (int j = 0;j < n;j++)
			arr[i * n + j] = A.arr[i * n + j];

	return *this;
}

vecMat3 vecMat3::operator+(const vecMat3& rhs)
{

	if (m == rhs.m && n == rhs.n) {

		vecMat3 res(m, n);

		for (int i = 0;i < m;i++)
			for (int j = 0;j < n;j++)
				res.arr[i * n + j] = arr[i * n + j] + rhs.arr[i * n + j];

		return res;
	}
	else{
		std::cout << " Matrix must have the same dimensions for addition" << std::endl;
		return vecMat3();
	}
}

vecMat3 vecMat3::operator-(const vecMat3& rhs)
{
	if (m == rhs.m && n == rhs.n) {

		vecMat3 res(m, n);

		for (int i = 0;i < m;i++)
			for (int j = 0;j < n;j++)
				res.arr[i * n + j] = arr[i * n + j] - rhs.arr[i * n + j];

		return res;
	}
	else {
		std::cout << " Matrix must have the same dimensions for addition" << std::endl;
		return vecMat3();
	}
}

vecMat3 vecMat3::operator*(const vecMat3& rhs)
{

	if ( n == rhs.m) {

		vecMat3 res(m, rhs.n);

		for (int i = 0;i < res.m;i++)
			for (int j = 0;j < res.n;j++) {
				res.arr[i * rhs.n + j] = 0.0f;
				for (int k = 0;k < n;k++)
					res.arr[i * rhs.n + j] += arr[i * n + k] * rhs.arr[k * rhs.n + j];
			}
		return res;
	}
	else {
		std::cout << " Matrix must have the same dimensions for multiplication" << std::endl;
		return vecMat3();
	}
	return vecMat3();
}

bool vecMat3::isScalar()
{
	return (m ==1 && n ==1);
}

float vecMat3::getScalar()
{
	return arr[0];
}

vecMat3 vecMat3::getTranspose()
{
	
	vecMat3 A(n, m);


	for (int i = 0;i < m;i++)
		for (int j = 0;j < n;j++)
			A.arr[j*m +i] = arr[i * n + j];
	return A;
}


vecMat3::vecMat3(int mm, int nn)
{
	m = mm;
	n = nn;

	arr = new float[m * n];


}

vecMat3::~vecMat3()
{
	delete[] arr;

}
