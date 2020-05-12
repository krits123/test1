#pragma once
class vec3
{

public:

	float x, y, z; // 3D Space Coordinates
	float w; // Fourth element used for mat mult and projection operations


public:
	// Construction 
	vec3();
	vec3(float x_, float y_, float z_);
	vec3(const vec3& rhs);

	// Basic Operations

	float mag();// returns the magnitude of this vector
	vec3 get_unit();// returns a unit vector in the direction of this
	float dot_product(const vec3& v); // returns the dot product of this and v 
	vec3 cross_product(const vec3& v2); // returns the cross product os this and v2 
	vec3 get_normal(const vec3& v); // returns a vector normal to the plane defined by this and v
	void normalize();


	// Operator Overloading

	vec3 operator + (const vec3& rhs);
	vec3 operator - (const vec3& rhs);
	vec3 operator * (float rhs);
	vec3 operator / (float rhs);
	vec3& operator += (const vec3 rhs);
	vec3& operator -= (const vec3 rhs);
	vec3& operator *= (float rhs);
	vec3& operator /= (float rhs);
};

