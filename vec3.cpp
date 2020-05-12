#include "vec3.h"
#include <math.h>

// Constructors
vec3::vec3() :
	x(0),y(0),z(0),w(1.0f)
{
}
vec3::vec3(float x_, float y_, float z_) :
	x(x_),y(y_),z(z_),w(1)
{}
vec3::vec3(const vec3& v) : 
	x(v.x),y(v.y),z(v.z),w(v.w)
{}

// Bais Operations

float vec3::mag() {


	return sqrt(x * x + y * y + z * z);
}

vec3 vec3::get_unit()
{
	float r = 1 / mag();
	return vec3(x*r,y*r,z*r);
}

float vec3::dot_product(const vec3& v)
{
	return (this->x*v.x + this->y*v.y + this->z*v.z);
}

vec3 vec3::cross_product(const vec3& v2)
{	

	float x_ = y * v2.z - z * v2.y;
	float y_ = z * v2.x - x * v2.z;
	float z_ = x * v2.y - y * v2.x;


	return vec3(x_,y_,z_);
}

vec3 vec3::get_normal(const vec3& v)
{
	vec3 v_out;

	v_out = cross_product(v);

	v_out.normalize();

	return v_out;
}

void vec3::normalize()
{
	float r = 1/mag();

	this->x *= r;
	this->y *= r;
	this->z *= r;
}

vec3 vec3::operator+(const vec3& rhs)
{
	return vec3(this->x + rhs.x,this->y + rhs.y,this->z + rhs.z);
}

vec3 vec3::operator-(const vec3& rhs)
{
	return vec3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}

vec3 vec3::operator*(float rhs)
{
	return vec3(this->x * rhs, this->y * rhs, this->z * rhs);
}

vec3 vec3::operator/(float rhs)
{
	return vec3(this->x / rhs, this->y / rhs, this->z / rhs);
}

vec3& vec3::operator+=(const vec3 rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;

	return *this;
	
}

vec3& vec3::operator-=(const vec3 rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;

	return *this;
}

vec3& vec3::operator*=(float rhs)
{
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;

	return *this;
}

vec3& vec3::operator/=(float rhs)
{
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;

	return *this;
}


