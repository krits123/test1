#pragma once
#include "Mat4.h"


class KCamera
{

public:
	Mat4 cam_ornt;
	

public:
	// Constructors
	KCamera();
	KCamera(vec3 P);

	// Implemenation of camera Transforamtions
	void moveForward(float dz);
	void moveUp(float dy);
	void moveRight(float dx);
	void rotateY(float dth);
	void rotateX(float dth);
	void rotateZ(float dth);

	void resetCam();
	// Get view matrix by inverting camera matrix
	void getViewMatrix(Mat4& o);

	// Get The camera position
	vec3 getCamPos();
};

